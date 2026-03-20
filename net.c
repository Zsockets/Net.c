#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "neth.h"

//main function
int main(int argc, char* argv[]){
    
    if(argc != 2){
	    printf("Fix your arguments\n");
	    return 1;
	}
    if(argv[1]==NULL){
	    printf("Add a valid argument!\n-a (all connection states)\n-t (ESTABLISHED connection states)\n-l (LISTENING connection states)");
	}
    
    int mode = 0;
    if(strcmp(argv[1],"-a")==0){
	mode = 1;
    }
    else if(strcmp(argv[1],"-t")==0){
	mode = 2;
    }
    else if(strcmp(argv[1],"-l")==0){
	mode = 3;
    }
    else{
	fprintf(stderr,"Invalid argument\n");
	return -1;
    }
    
    int capacity = 10;
    int count = 0;
	struct Connection *connections = malloc(capacity * sizeof(struct Connection));	
    char local_hex[16], remote_hex[16], state_hex[8];

    FILE *infile;
    infile = fopen(TCP_PATH, "r");
    char line[LINE_LEN];

    if(infile == NULL){
	    printf("Error reading /proc/net/tcp\n");
	    fclose(infile);
	    return 1;
	}

    if(fgets(line,LINE_LEN,infile)==NULL){
	    printf("Error reading header\n");
	    fclose(infile);
	    return 1;
	}
    
    printf("Successfully retrieved /proc/net/tcp! Displaying connections...\n");
    printf("%-25s %-25s %-12s %-12s\n","Local Address:PORT","Remote Address:PORT","STATE","PID");

    while(fgets(line,LINE_LEN,infile) != NULL){
        if(count == capacity){
            capacity *= 2;
            struct Connection *temp = realloc(connections, capacity*sizeof(struct Connection));
            if(temp != NULL){
                connections = temp;
            }
        }	
        if(sscanf(line,"%*d: %[^:]:%x %[^:]:%x %s %*x:%*x %*x:%*x %*x %*d %*d %d",local_hex,&connections[count].local_port,remote_hex,&connections[count].remote_port,state_hex,&connections[count].inode)){
	    connections[count].local_ip = swap_endian(local_hex);
	    connections[count].remote_ip = swap_endian(remote_hex);
	    connections[count].state_int = (unsigned int)strtoul(state_hex,NULL,16);
        count++;
	    }
    }


    for(int i = 0; i < count ; i++){ 
	if(mode == 1){
		print(connections[i].local_ip,connections[i].remote_ip,connections[i].local_port,connections[i].remote_port,state_translation(connections[i].state_int),get_pid_by_inode(connections[i].inode));
        printf("\n");
        }
	else if(mode == 2 && connections[i].state_int == 0x01){
		print(connections[i].local_ip,connections[i].remote_ip,connections[i].local_port,connections[i].remote_port,state_translation(connections[i].state_int),get_pid_by_inode(connections[i].inode));
	    printf("\n");
	    }
	else if(mode == 3 && connections[i].state_int == 0x0A){
		print(connections[i].local_ip,connections[i].remote_ip,connections[i].local_port,connections[i].remote_port,state_translation(connections[i].state_int),get_pid_by_inode(connections[i].inode));
	    printf("\n");
		}
	}


    fclose(infile);
	free(connections);

    return 0;
}

