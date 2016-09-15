#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE=256, EQ=0, DEC=1, HEX=2, NEQ=3, DEREF=4, LO_AND=5, LO_OR=6, NEG=7

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	{"\\$[a-zA-Z]+",'$'},		//find the register
	{"0[xX][0-9a-fA-F]+",HEX},			//12 number
	{"[0-9]+",DEC},			//10 number
       	{" +", NOTYPE},			// spaces
	{"\\*",'*'},			//multiply
	{"/",'/'},			//divide
	{"\\+", '+'},			// plus
	{"-",'-'},			//minus
	{"\\(",'('},
	{"\\)",')'},
	{"==", EQ},			// equal
	{"!=",NEQ},			//not equal
	{"&&",LO_AND},			//logical and
	{"\\|\\|",LO_OR},		//logical or
	{"!",'!'}			//logical not
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				printf("position:%d type:%d\n",position-substr_len,rules[i].token_type);  ///for debug
				switch(rules[i].token_type) {
					case NOTYPE:
						break;
					case DEC:
						if(substr_len>10){
							printf("too big integer!\n");
							return false;
						}
						tokens[nr_token].type=rules[i].token_type;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len]='\0';
						nr_token++;
						break;
					case HEX:
						if(substr_len>10){
							printf("too big integer!\n");
							return false;
						}
						tokens[nr_token].type=rules[i].token_type;
						strncpy(tokens[nr_token].str,substr_start+2,substr_len-2);
						tokens[nr_token].str[substr_len-2]='\0';
						nr_token++;
						break;
					case '$':
						if(substr_len>4){
							printf("There is no this type of register.\n");
							return false;
						}
						tokens[nr_token].type=rules[i].token_type;
						strncpy(tokens[nr_token].str,substr_start+1,substr_len-1);
						tokens[nr_token].str[substr_len-1]='\0';
						nr_token++;
						break;
					default:
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].str[0]='\0';
						nr_token++;
						break;
				}

				printf("nr_token:%d type:%d\n",nr_token-1,tokens[nr_token-1].type);  ///for debug

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

struct made{
	int order; 
	int type;
}priority[]={
	{13,DEREF},
	{13,'!'},
	{11,'+'},
        {11,'-'},
        {12,'*'},
        {12,'/'},
	{8,EQ},
	{8,NEQ},
	{4,LO_AND},
	{3,LO_OR}
};

bool find_operator(int m){
        int size_prio = sizeof(priority)/sizeof(priority[0]);
        int i;
        for(i=0;i<size_prio;i++){
                if(priority[i].type == m)
                        return true;
        }
	printf("%d is not an operator\n",m);
        return false;	
}

int find_order(int m){
	int size_prio = sizeof(priority)/sizeof(priority[0]);
	int i;
	for(i=0;i<size_prio;i++){
		if(priority[i].type == m)
			return priority[i].order;
	}
	return -1;
}

int domi_op(int p,int q){
	int index;
	int total_order = 257;
	int total_position = -1;
	int left=0;
	for(index=q;index>=p;index--){
		if(tokens[index].type == '('){
			left--;
		}else if(tokens[index].type == ')'){
			left++;
		}
		if((!find_operator(tokens[index].type))||(left!=0)){
			printf("%d is not a operator\n",index);
			continue;
		}
		int current_order = find_order(tokens[index].type);
		if(current_order < total_order){
			total_order = current_order;
			total_position = index;
		}
	}
	printf("total_position:%d\n",total_position);
	return total_position;
}

bool check_parentneheses(int p,int q){
	if((tokens[p].type!='(')||(tokens[q].type!=')'))
		return false;
	int index;
	int left=0;
	for(index=p;index<=q;index++){
		if((tokens[index].type!='(')&&(tokens[index].type!=')'))
			continue;
		if(tokens[index].type=='('){
			left++;
		}else{
			left--;
		}
		if(left==0){
			break;
		}
	}
	if(index==q)
		return true;
	return false;
}

struct registers{
	char name[4];
	int index;
};

struct registers reg_4[]={
	{"eax",R_EAX},
	{"ecx",R_ECX},
	{"edx",R_EDX},
	{"ebx",R_EBX},
	{"esp",R_ESP},
	{"ebp",R_EBP},
	{"esi",R_ESI},
	{"edi",R_EDI}
};

struct registers reg_2[]={
	{"ax",R_AX},
	{"cx",R_CX},
	{"dx",R_DX},
	{"bx",R_BX},
	{"sp",R_SP},
	{"bp",R_BP},
	{"si",R_SI},
	{"di",R_DI}
};

struct registers reg_1[]={
	{"al",R_AL},
	{"cl",R_CL},
	{"dl",R_DL},
	{"bl",R_BL},
	{"ah",R_AH},
	{"ch",R_CH},
	{"dh",R_DH},
	{"bh",R_BH}
};

bool read_register(char* start,int* number){
	if(strcmp(start,"eip")==0){
		*number=cpu.eip;
		return true;
	}
	int i;
	for(i=0;i<8;i++){
		if(strcmp(start,reg_4[i].name)==0){
			*number=reg_l(reg_4[i].index);
			return true;
		}
	}
        for(i=0;i<8;i++){
                if(strcmp(start,reg_2[i].name)==0){
                        *number=reg_w(reg_2[i].index);
                        return true;
                }
        }
        for(i=0;i<8;i++){
                if(strcmp(start,reg_1[i].name)==0){
                        *number=reg_b(reg_1[i].index);
                        return true;
                }
        }
	return false;
}

uint32_t eval(int p,int q){
	if(p > q){
		printf("Bad expression!\n");
		assert(0);
	}else if(p == q){
		/* TODO: Finish the HEX function*/
		int eval_number;
		switch(tokens[p].type){
			case DEC:
				sscanf(tokens[p].str,"%d",&eval_number);
				break;
			case HEX:
				sscanf(tokens[p].str,"%x",&eval_number);
				break;
			case '$':
				read_register(tokens[p].str,&eval_number);
				break;
			default:
				break;
		}
		return eval_number;
	}else if(check_parentneheses(p,q) == true){
		return eval(p+1,q-1);
	}else{
		int op = domi_op(p,q);
		printf("dominant operator:%d\n",op); //TODO: for debug
		int val2 = eval(op+1,q);
		switch(tokens[op].type){
			case DEREF: return swaddr_read((swaddr_t)val2,4);
			case '!': return (!val2);
			case NEG: return -val2;
			default:{
				int val1 = eval(p,op-1);
				printf("val1:%d\n",val1); //TODO: for debug
				printf("val2:%d\n",val2); //TODO: for debug
				switch(tokens[op].type){
					case '+': return val1 + val2;
					case '-': return val1 - val2;
					case '*': return val1 * val2;
					case '/': return val1 / val2;
					case EQ : return (val1 == val2);
					case NEQ: return (val1 != val2);
					case LO_AND: return (val1 && val2);
					case LO_OR: return (val1 || val2);	     
					default:assert(0);
				}
			}
		}
	}
}

uint32_t expr(char *e, bool *success) {
	init_regex();
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	int i;
	for(i=0;i<nr_token;i++){
		if((tokens[i].type=='*')&&((i==0)||(tokens[i-1].type=='(')||(find_operator(tokens[i-1].type))))
			tokens[i].type=DEREF;
		if((tokens[i].type=='-')&&((i==0)||(tokens[i-1].type=='(')||(find_operator(tokens[i-1].type))))
                        tokens[i].type=NEG;
	}
	return eval(0,nr_token-1);
}

