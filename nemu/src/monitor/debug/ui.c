#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_si(char *args){
        unsigned si_number;
        if(args == NULL){
	    si_number = 1;
	    return 0;
	}
	sscanf(args,"%u",&si_number);
        if((sscanf(args,"%u",&si_number))>0)
 	{
	      cpu_exec(si_number);
	}
	else
	    printf("wrong number!\n");
	return 0;	
}

void cmd_info_wp(){
	WP * temp=get_head();
	while(temp!=NULL){
		printf("watchpoint %d  %s: %d\n",(*temp).NO,(*temp).expr_str,(*temp).data);
		temp=(*temp).next;
	}
}

void printf_re(unsigned int r){
	char map[][2]={"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};
        int i;
        for(i=28;i>=0;i=i-4){
                  printf("%s",map[((r>>i)&(0xf))]);
	}
}

static int cmd_info(char *args){
	if(*args=='r'){
	    printf("eax:0x");
	    printf_re(cpu.eax);
	    printf("\n");
	    printf("ecx:0x");
	    printf_re(cpu.ecx);
	    printf("\n");
	    printf("edx:0x");
	    printf_re(cpu.edx);
	    printf("\n");
	    printf("ebx:0x");
	    printf_re(cpu.ebx);
	    printf("\n"); 
	    printf("esp:0x");
	    printf_re(cpu.esp);
	    printf("\n");
	    printf("ebp:0x"); 
	    printf_re(cpu.ebp);
	    printf("\n");
	    printf("esi:0x");
	    printf_re(cpu.esi);
	    printf("\n"); 
	    printf("edi:0x");
            printf_re(cpu.edi);	    
	    printf("\n");
	    printf("eip:0x");
	    printf_re(cpu.eip);		
	    printf("\n");
	    return 0;
	}else if(*args=='w'){
	    cmd_info_wp();
	    return 0;
	}
        printf("input Error");
	return 0;
}

void cmd_x(unsigned int area_size,unsigned int result){
	        unsigned int memory_number;
	        unsigned int i;
		unsigned int counter=0;		
		for(i=0; i<area_size; i++){
			printf("0x%x:  0x",result);
			for(counter=0;counter<4;counter++){
		         	memory_number = swaddr_read(result,1);          		 	        
				printf("%02x",memory_number);
				result++;
			}
	                if(result!=(area_size-1)){					                        
				printf("\n");						                
			}     
		}
}

static int cmd_x_total(char *args){	
	unsigned int area_size;
	char * expr_str=malloc((sizeof(char))*(strlen(args)));
	bool success=true;
	sscanf(args,"%d%s",&area_size,expr_str);
        int result = expr(expr_str,&success);
	if(success==false){
		free(expr_str);
		printf("Wrong expr.\n");
		return 0;
	}
	cmd_x(area_size,result);
	free(expr_str);
        return 0;
}

static int cmd_p(char *args){
	bool * success=malloc(sizeof(bool));
	* success=true;
	int result;
	result=expr(args,success);
	if(success==false)
		printf("Input wrong!\n");
	else
		printf("The result is %d.\n",result);
	free(success);
	return 0;
}

static int cmd_w(char *args){
	bool success=true;
	int result;
	result=expr(args,&success);
	if(success==false){
		printf("Create failed. Wrong input!\n");
	}else{
		WP * wp =new_wp();
		(*wp).expr_str=malloc((sizeof(char))*(1+strlen(args)));
		sscanf(args,"%s",(*wp).expr_str);
		(*wp).data=result;
	}
	return 0;
}

static int cmd_dw(char *args){
	WP * temp=get_head();
	int index;
	sscanf(args,"%d",&index);
	while(temp!=NULL){
		if((*temp).NO==index){
			free((*temp).expr_str);
			free_wp(temp);
			return 0;
		}
		temp=(*temp).next;
	}
	printf("Cannot find watchpoint!\n");
	return 0;
}

static int cmd_flag(){
	printf("cf: %d\n",cpu.flags.bits.cf);
	printf("pf: %d\n",cpu.flags.bits.pf);
	printf("zf: %d\n",cpu.flags.bits.zf);
	printf("sf: %d\n",cpu.flags.bits.sf);
	printf("of: %d\n",cpu.flags.bits.of);
	return 0;
}

static int cmd_help(char *args);

bool find_func(int eip,int maximum,char *func_name,int *address);

static int cmd_bt(){
	swaddr_t eip=cpu.eip;
	swaddr_t ebp=cpu.ebp;
	int case_number=0;
	while(ebp!=0){
		int maximum=100;
		char function[100];
		int addr;
		bool success = find_func(eip,maximum,function,&addr);
		if(success==0){
			printf("Unknown eip address.\n");
			return 0;
		}else{
			printf("case:%d:  ",case_number);
			printf("address:0x%x ",addr);
			printf("name:%s ",function);
			int i;
			for(i=0;i<4;i++)
				printf("args%d:0x%x  ",i,swaddr_read(ebp+8+4*i,4));
		}
		printf("\n");
		eip = swaddr_read(ebp+4,4);
		ebp = swaddr_read(ebp,4);
		case_number++;
	}
	return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Step Into", cmd_si },
	{"info","Get Infomation",cmd_info},
	{"x","Print Memory",cmd_x_total},
	{"p","Caculate the exper",cmd_p},
	{"w","Create a watchpoint",cmd_w},
	{"d","Delete a watchpoint",cmd_dw},
	{"flags","Print Flags",cmd_flag},
	{"bt","Print the Stack Frame Chain",cmd_bt},

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
