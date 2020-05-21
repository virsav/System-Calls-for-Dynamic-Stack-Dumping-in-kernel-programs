#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/utsname.h>

#define INSDUMP_SYSCALL 359
#define RMDUMP_SYSCALL 360

#define SYMBOL_NAME1 "sys_open"
#define SYMBOL_NAME2 "sys_newuname"
#define SYMBOL_NAME3 "sys_nanosleep"

struct dumpmode_t {			//dumpmode_t struct to pass the dumpstack mode to kernel
	unsigned int mode;
};

void* uname_thread(void* input){		//Thread function to run on pthread created
     
    struct utsname uname_info; 
    uname(&uname_info);
    return 0;
}

int main(int argc, char *argv[]){

	struct dumpmode_t user;					//dumpstack struct to be sent 
	unsigned int dumstack_mode;				//dumpstack mode
	struct utsname uname_info;				//struct for syscall newuname
	pthread_t thread_1;						//pthread 
	pid_t pid_1, pid_2;							//process id
	int ret, return_value1, return_value2, return_value3, fd;			//return variables adn file descriptor
	int return4, return5;

	printf("Please enter dump stack mode -");
    scanf("%d", &dumstack_mode);							//user given dumpstack mode from command line

    user.mode = dumstack_mode;

    //printf("--------------------------------------------Testing start-----------------------------------------------\n");

   	ret = syscall(INSDUMP_SYSCALL,"testing_name", &user);			//testing any random symbol name, should throw error
	if(ret < 0){
		printf("ERROR %d\n", ret);
	}

	//printf("--------------------------------------------Testing end-----------------------------------------------\n");
	//printf("--------------------------------------------Symbol 1 start-----------------------------------------------\n");

    return_value1 = syscall(INSDUMP_SYSCALL,SYMBOL_NAME1, &user);			//insdump for symbol 1
    if(return_value1 < 0){
		printf("ERROR while doing INSDUMP_SYSCALL, return = %d\n", return_value1);
		return -1;
	}

	fd = open("/home/root/script", O_RDWR);								//should dump stack always
	close(fd);

	return_value2 = syscall(RMDUMP_SYSCALL,return_value1);				//rmdump for symbol 1
	if(return_value2 < 0){
		printf("ERROR While doing RMDUMP_SYSCALL, return  = %d\n", return_value2);
		return -1;
	}
	printf("DUMPSTACKID %d is removed\n", return_value1);

	fd = open("/home/root/script", O_RDWR);								//should not dump stack as rmdump happened before
	close(fd);

	//printf("--------------------------------------------Symbol 1 end-----------------------------------------------\n");
	//printf("--------------------------------------------Symbol 2 start-----------------------------------------------\n");

	return_value3 = syscall(INSDUMP_SYSCALL,SYMBOL_NAME2, &user);					//insdump for symbol 2
    if(return_value1 < 0){
		printf("ERROR while doing INSDUMP_SYSCALL, return = %d\n", return_value3);
		return -1;
	}

	pid_1 =fork();												//forking the process to create child A from owner

	if(pid_1 == 0){ 											//checking if child process A
		printf("IN CHILD A:\n");
		uname(&uname_info);

		printf("TRYING TO REMOVE DUMPSTACKID %d\n", return_value3);

		// process other than owner trying the remove the dump stack
		if(syscall(RMDUMP_SYSCALL,return_value3)<0){					//should throw error as someone other than owner trying rmdump
		    printf("ERROR WHILE REMOVING THE DUMPSTACK\n");		
		}

		return4 = syscall(INSDUMP_SYSCALL,SYMBOL_NAME3, &user);				//insdump for symbol 3
		if(return4 < 0){
			printf("ERROR while doing INSDUMP_SYSCALL, return = %d\n", return4);
			return -1;
		}
		nanosleep(1,10);
	}
	else if(pid_1 < 0){
		printf("Child A not created\n");
	}
	else{
			pid_2 = fork();										//forking the process to create child B from owner only

			if(pid_2 == 0){
				printf("IN CHILD B:\n");						//checking if child process B
				uname(&uname_info);

				printf("TRYING TO REMOVE DUMPSTACKID %d\n", return_value3);

				// process other than owner trying the remove the dump stack
				if(syscall(RMDUMP_SYSCALL,return_value3)<0){					//should throw error as someone other than owner trying rmdump
		    		printf("ERROR WHILE REMOVING THE DUMPSTACK\n");	
		    	}
		    	nanosleep(1,10);						//This should dump in mode 1
			}
			else if(pid_2 < 0){
				printf("Child B not created\n");
			}
	}

	pthread_create(&thread_1,NULL, uname_thread, (void *) NULL);			//creating thread (will be created in every child as well)
	pthread_join(thread_1,NULL);

	sleep(4);

	//printf("--------------------------------------------Symbol 2 end-----------------------------------------------\n");

	return 0;
}