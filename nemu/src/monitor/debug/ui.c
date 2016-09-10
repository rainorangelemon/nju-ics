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
        int n=0;
        if(args == NULL){
	    n = 1;
	}
	{
	    if((sscanf(args,"%d",&n))>0)
		    cpu_exec(n);
	    else
		    printf("wrong number!\n");
	}
	cpu_exec(n);
	return 0;	
}

void cmd_info_wp();

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
	}/*else if(*args=='w'){
	    cmd_info_wp();
	    return 0;
	}*/
        printf("input Error");
	return 0;
}

void cmd_x(unsigned int area_size,unsigned int result){
	        unsigned int memory_number;
	        unsigned int i;		
		for(i=0; i<area_size; i++){
		       	memory_number = swaddr_read(result,4);
	                printf("0x%x",result);          				 	        printf(":  0x%x",memory_number);
	                if(result!=(area_size-1)){					                        printf("\n");						                }      
	                result++;     
		}
}

static int cmd_x_total(char *args){
        /*char* expr_string;*/	
	unsigned int area_size;
	/*"sscanf(args,"%d%s",&area_size,expr_string);
        unsigned int result = expr(expr_string,true);*/ ///Finish in Step two
	///check after Step 2
	unsigned int expr_number;
       	sscanf(args,"%d%x",&area_size,&expr_number);
	cmd_x(area_size,expr_number);
        return 0;
}


static int cmd_help(char *args);

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
