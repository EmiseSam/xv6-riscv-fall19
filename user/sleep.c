#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc !=2){
        fprintf(2,"must 1 argument for sleep\n");
        exit();
    }
    int sleeptime = atoi(argv[1]);
    printf("nothing happened\n");
    sleep(sleeptime);
    exit();
}    