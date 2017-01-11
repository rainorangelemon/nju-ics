#include "common.h"
#include <sys/ioctl.h>

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
static const file_info file_table[] __attribute__((used)) = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

int fs_ioctl(int fd, uint32_t request, void *p) {
	assert(request == TCGETS);
	return (fd >= 0 && fd <= 2 ? 0 : -1);
}

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

/* TODO: implement a simplified file system here. */
typedef struct{
	bool opened;
	uint32_t offset;
}Fstate;

Fstate files[NR_FILES+3];

static inline int strcmp(const char *s1,const char *s2){
	int i=0;
	while(s1[i]==s2[i]){
		if(s1[i]==0)
			break;
		else
			i++;
	}
	return s1[i]-s2[i];
}

int fs_open(const char *pathname,int flags){
	int i;
	for(i=3;i<(NR_FILES)+3;i++){
		if(strcmp(file_table[i-3].name,pathname)==0){
			files[i].opened=true;
			files[i].offset=0;
			return i;
		}
	}
	Log("file not found!\n");
	return 0;
}

int fs_read(int fd,void *buf,int len){
	if((fd>2)&&(fd<(NR_FILES+3))&&(files[fd].opened)){
		int n=(len<(file_table[fd-3].size-files[fd].offset))?(len):(file_table[fd-3].size-files[fd].offset);
		ide_read(buf,file_table[fd-3].disk_offset+files[fd].offset,n);
		files[fd].offset=files[fd].offset+n;
		return n;
	}
	Log("fs_read:not good fd!\n");
	return 0;
}

void serial_printc(char);
int fs_write(int fd,void *buf,int len){
	assert(len>=0);
	if(fd==1||fd==2){
		int i;
		for(i=0;i<len;i++)
			serial_printc(((char*)buf)[i]);
		return len;
	}else if((fd>2)&&(fd<(NR_FILES+3))&&files[fd].opened) {
		int n=(len<(file_table[fd-3].size-files[fd].offset))?(len):(file_table[fd-3].size-files[fd].offset);
		ide_write(buf,file_table[fd-3].disk_offset+files[fd].offset,n);
		files[fd].offset=files[fd].offset+n;
		return n;
	}
	Log("do not print stdin!\n");
	return 0;
}

int fs_lseek(int fd,int offset,int whence){
	int total_offset=0x7fffffff;
	if((fd>2)&&(fd<(NR_FILES+3))&&files[fd].opened){
		switch(whence){
			case SEEK_SET:
				total_offset=offset;
				break;
			case SEEK_CUR:
				total_offset=files[fd].offset+offset;
				break;
			case SEEK_END:
				total_offset=file_table[fd-3].size+offset;
				break;
			default:
				Log("fs_lseek: bad whence!\n");
				break;
		}
		if((total_offset>=0)&&(total_offset<=file_table[fd-3].size)){
			files[fd].offset=total_offset;
			return total_offset;
		}else{
			Log("outside area\n");
			return -1;
		}
	}else{
		Log("bad fd!\n");
		return -1;
	}
}

int fs_close(int fd){
	if((fd>2)&&(fd<(NR_FILES+3))){
		files[fd].opened=false;
		files[fd].offset=0;
		return 0;
	}
	Log("fs_close: bad fd!\n");
	return -1;
}
