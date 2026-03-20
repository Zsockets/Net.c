#include "neth.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <limits.h>

int is_numeric(const char* str){

    int i=0;
    while(str[i]!='\0'){

        bool test = isdigit(str[i]);

        if(test == false){
            return 0;
        }
        i++;
    }
return 1;
}


int get_pid_by_inode(uint32_t target_inode){

    DIR *dir;
    dir = opendir("/proc/");
    struct dirent *entry;
    struct dirent *inner_entry;
    char path[PATH_MAX];

    if(dir == NULL){
        printf("Failed to access /proc/\n");
        return 1;
    }

    while((entry = readdir(dir)) != NULL){
        bool test = is_numeric(entry->d_name);
        if(test == 1){
            snprintf(path,PATH_MAX,"/proc/%s/fd/",entry->d_name);

            DIR *fd_dir = opendir(path);
            char fd_path[PATH_MAX];

            while((inner_entry = readdir(fd_dir)) != NULL){
                if(is_numeric(inner_entry->d_name)){

                    snprintf(fd_path,PATH_MAX,"%s%s",path,inner_entry->d_name);
                    char target_path[PATH_MAX];
                    int len = readlink(fd_path,target_path,PATH_MAX);
                    target_path[len] = '\0';
                    uint32_t f_inode;
                    sscanf(target_path,"socket:[%u]",&f_inode);

                    if(target_inode == f_inode){
                        uint32_t pid = strtoul(entry->d_name,NULL,10);
                        closedir(dir);
                        closedir(fd_dir);
                        return pid;
                    }
                }
            }
            closedir(fd_dir);
        }
    }
    closedir(dir);
    return 0;
}

void print(uint32_t l_ip,uint32_t r_ip,int l_port,int r_port,const char* state,int inode_pid){
    uint32_t l_first_oct,l_second_oct,l_third_oct,l_fourth_oct;
    uint32_t r_first_oct,r_second_oct,r_third_oct,r_fourth_oct;
    char local_string [32];
    char remote_string [32];

    l_first_oct = (l_ip >> 24) & 0xFF;
    l_second_oct = (l_ip >> 16) & 0xFF;
    l_third_oct = (l_ip >> 8) & 0xFF;
    l_fourth_oct = (l_ip & 0xFF);

    r_first_oct = (r_ip >> 24) & 0xFF;
    r_second_oct = (r_ip >> 16) & 0xFF;
    r_third_oct = (r_ip >> 8) & 0xFF;
    r_fourth_oct = (r_ip & 0xFF);

    snprintf(local_string,32,"%u.%u.%u.%u:%u",l_first_oct,l_second_oct,l_third_oct,l_fourth_oct,l_port);
    snprintf(remote_string,32,"%u.%u.%u.%u:%u",r_first_oct,r_second_oct,r_third_oct,r_fourth_oct,r_port);

    printf("%-25s %-25s %-12s %d",local_string,remote_string,state,inode_pid);
}


//swaps little endian to big endian, (1.0.0.127 to 127.0.0.1) but in hex, returns unsigned int.
uint32_t swap_endian(char *val1){
    //turning the string into an unsigned long/int, stopping at '\0'
    uint32_t val = (uint32_t)strtoul(val1,NULL,16);

    //the name assignment is post swap, to make the return easier to understand.
    uint32_t left_most = (val & 0x000000FF) << 24;
    uint32_t right_most = (val & 0xFF000000) >> 24;
    uint32_t mid_left = (val & 0x0000FF00) << 8;
    uint32_t mid_right = (val & 0x00FF0000) >> 8;

    // returned in order!
    return left_most | mid_left | mid_right | right_most;
}

//function to translate state from a value to a string
const char* state_translation(unsigned int state_val){
    switch(state_val){
        case 0x01:
            return "ESTABLISHED";
        case 0x02:
            return "SYN_SENT";
        case 0x03:
            return "SYN_RECV";
        case 0x06:
            return "TIME_WAIT";
        case 0x08:
            return "CLOSE_WAIT";
        case 0x0A:
            return "LISTENING";
        default:
            return "UNKNOWN";
        }
}
