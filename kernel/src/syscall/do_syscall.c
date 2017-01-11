#include "irq.h"

#include <sys/syscall.h>
void add_irq_handle(int, void (*)(void));
uint32_t mm_brk(uint32_t);
int fs_ioctl(int, uint32_t, void *);
void serial_printc(char);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax=0;
}

static void sys_ioctl(TrapFrame *tf) {
	tf->eax = fs_ioctl(tf->ebx, tf->ecx, (void *)tf->edx);
}

int fs_write(int fs,void *buf,int len);
int fs_open(const char *pathname,int flags);
int fs_read(int fd,void *buf,int len);
int fs_close(int fd);
int fs_lseek(int fd,int offset,int whence);

static inline void sys_write(TrapFrame *tf){
	if(tf->ebx==1||tf->ebx==2){
		int count;
		for(count=0;count<tf->edx;count++)
			serial_printc(((char*)tf->ecx)[count]);
		tf->eax=tf->edx;
	}else{
		tf->eax=fs_write(tf->ebx,(void*)tf->ecx,tf->edx);
	}
}
void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		/* The `add_irq_handle' system call is artificial. We use it to
		 * let user program register its interrupt handlers. But this is
		 * very dangerous in a real operating system. Therefore such a
		 * system call never exists in GNU/Linux.
		 */
		case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;
		case SYS_brk: sys_brk(tf); break;
		case SYS_ioctl: sys_ioctl(tf); break;
		case SYS_write: 
			sys_write(tf);break;
		case SYS_open: 
			tf->eax=fs_open((const char *)tf->ebx,tf->ecx);break;
		case SYS_read: 
			tf->eax=fs_read(tf->ebx,(void *)tf->ecx,tf->edx);break;
		case SYS_close: 
			tf->eax=fs_close(tf->ebx);break;
		case SYS_lseek: 
			tf->eax=fs_lseek(tf->ebx,tf->ecx,tf->edx);break;
		/* TODO: Add more system calls. */

		default: panic("Unhandled system call: id = %d, eip = 0x%08x", tf->eax, tf->eip);
	}
}

