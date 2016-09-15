#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE=256, EQ=0, DEC=1, HEX=2

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	{"0[xX][0-9a-fA-F]+",HEX},			//12 number
	{"[0-9]+",DEC},			//10 number
       	{" +", NOTYPE},			// spaces
	{"\\*",'*'},			//multiply
	{"/",'/'},			//divide
	{"\\+", '+'},			// plus
	{"-",'-'},			//minus
	{"\\(",'('},
	{"\\)",')'},
	{"==", EQ}						// equal
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

				switch(rules[i].token_type) {
					Token temp;
					temp.type=rules[i].token_type;
					case NOTYPE:
						break;
					case DEC:
						if(substr_len>10){
							printf("too big integer!\n");
							return false;
						}
						strncpy(temp.str,substr_start,substr_len);
						temp.str[substr_len]='\0';
						tokens[nr_token]=temp;
						nr_token++;
						break;
					case HEX:
						if(substr_len>10){
							printf("too big integer!\n");
							return false;
						}
						strncpy(temp.str,substr_start+2,substr_len-2);
						temp.str[substr_len-2]='\0';
						tokens[nr_token]=temp;
						nr_token++;
						break;
					default:
						temp.str[0]='\0';
						tokens[nr_token]=temp;
						nr_token++;
						break;
				}

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
        {5,'+'},
        {5,'-'},
        {7,'*'},
        {7,'/'}	
};

bool find_operator(int m){
        int size_prio = sizeof(priority)/sizeof(priority[0]);
        int i;
        for(i=0;i<size_prio;i++){
                if(priority[i].type == m)
                        return true;
        }
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
	int total_position = q;
	for(index=q;index>=p;index--){
		int left=0;
		if(tokens[index].type == '('){
			left++;
		}else if(tokens[index].type == ')'){
			left--;
		}
		if((!find_operator(tokens[index].type))||(left!=0))
			continue;
		int current_order = find_order(tokens[index].type);
		if(current_order < total_order){
			total_order = current_order;
			total_position = index;
		}
	}
	return total_position;
}

bool check_parentneheses(int p,int q){
///TODO:Finish this
	return true;
}

uint32_t eval(int p,int q){
	if(p > q){
		printf("Bad expression!\n");
		assert(0);
	}else if(p == q){
		/* TODO: Finish the HEX function*/
		int eval_number;
		sscanf(tokens[p].str,"%d",&eval_number);
		return eval_number;
	}else if(check_parentneheses(p,q) == true){
		return eval(p+1,q-1);
	}else{
		int op = domi_op(p,q);
		int val1 = eval(p,op-1);
		int val2 = eval(op+1,q);
		switch(tokens[op].type){
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;  	  
			default:assert(0);
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
	/*panic("please implement me");*/
	return eval(0,nr_token-1);
}

