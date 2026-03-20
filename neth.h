#ifndef NETH_H
#define NETH_H

#include <stdint.h>

#define TCP_PATH "/proc/net/tcp"
#define LINE_LEN 256

struct Connection{
    int state_int,local_port,remote_port,inode;
    uint32_t local_ip,remote_ip;
};

int is_numeric(const char* str);
int get_pid_by_inode(uint32_t target_inode);
void print(uint32_t l_ip,uint32_t r_ip,int l_port,int r_port,const char* state,int inode_pid);
uint32_t swap_endian(char *val1);
const char* state_translation(unsigned int state_val);

#endif
