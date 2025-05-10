#include "kernel/types.h"
#include "user/user.h"
#define MAX_PROCESSES 16
int *peterson_locks;
int *procceses_pids;
int current_index ;
int levels;
int tournament_create(int processes){
        if (processes <= 0 || processes > MAX_PROCESSES || (processes & (processes - 1)) != 0) {
        return -1;
    }
        levels = 0;
    for (int temp = processes; temp > 1; temp >>= 1) {
        levels++;
    }
    int num_locks = processes - 1;
    peterson_locks = malloc(sizeof(int) *num_locks);
    for (int i=0;i<num_locks;i++){
        peterson_locks[i] = peterson_create();
        if (peterson_locks[i] == -1) {
            return -1;
        }
    }
    procceses_pids = malloc(sizeof(int) * processes);
    for (int i=0;i<processes;i++){
        int pid = fork();
        if (pid < 0) {
            return -1;
        } else if (pid == 0) {
            // Child process
            current_index = i;
            return i;
        }else {
            // Parent process
            procceses_pids[i] = pid;
        }

    }
    return -2;
}
int tournament_acquire(void){
    int role_l, lock_l, array_index, lock_id;
    for (int i=levels-1;i>=0;i--){
         role_l=(current_index&(1 << (levels- i- 1))) >> (levels- i -1);
         lock_l = current_index >> (levels-i);
         array_index = lock_l + (1 << i) - 1;
         lock_id = peterson_locks[array_index];
        if (peterson_acquire(lock_id, role_l)<0) {
            return -1;
        }
    }
    return 0;
}
int tournament_release(void){   
    int role_l, lock_l, array_index, lock_id;
    for (int i=0;i<levels;i++){
         role_l=(current_index&(1 << (levels- i- 1))) >> (levels- i -1);
         lock_l = current_index >> (levels-i);
         array_index = lock_l + (1 << i) - 1;
         lock_id = peterson_locks[array_index];
        if (peterson_release(lock_id, role_l)<0) {
            return -1;
        }
    }
    return 0;
}