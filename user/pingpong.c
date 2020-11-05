#include "kernel/types.h"
#include "user/user.h"

int main (){
    int p1[2],p2[2];
    char buffer[] = {'X'};
    long length = sizeof(buffer);
    pipe(p1);
    pipe(p2);
    if(fork() == 0){
        close(p1[1]);
        close(p2[0]);
        if(read(p1[0], buffer, length) != length){
			printf("a--->b read error!");
			exit();
		}
        printf("%d: received ping\n", getpid());
		if(write(p2[1], buffer, length) != length){
			printf("a<---b write error!");
			exit();
		}
        exit();
    }
    close(p1[0]);
    close(p2[1]);
    if(write(p1[1], buffer, length) != length){
        printf("a--->b write error!");
        exit();
	}
    if(read(p2[0], buffer, length) != length){
        printf("a<---b read error!");
        exit();
    }
    printf("%d: received pong\n", getpid());
    exit();
}