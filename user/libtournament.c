#include "kernel/types.h"
#include "user/user.h"
#define MAX_PROCESSES 16
int *peterson_locks;
int *procceses_pids;
int current_index ;
int levels;
int tournament_create(int processes){
    // check if the number of processes is a power of 2 and within the limit
        if (processes <= 0 || processes > MAX_PROCESSES || (processes & (processes - 1)) != 0) {
        return -1;
    }
    //calculate the number of levels in the tournament tree 
        levels = 0;
    for (int temp = processes; temp > 1; temp >>= 1) {
        levels++;
    }
    int num_locks = processes - 1;
    // allocate memory for the locks
    peterson_locks = malloc(sizeof(int) *num_locks);
    //create the peterson locks 
    for (int i=0;i<num_locks;i++){
        peterson_locks[i] = peterson_create();
        if (peterson_locks[i] == -1) {
            return -1;
        }
    }
    // allocate memory for the process ids
    procceses_pids = malloc(sizeof(int) * processes);
    // create the processes
    for (int i=0;i<processes;i++){
        int pid = fork();
        if (pid < 0) {
            return -1;
        } else if (pid == 0) {
            // Child process
            current_index = i;
            //return the procces tournament index
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
    // acquire the locks in order from the leaf to the root
    for (int i=levels-1;i>=0;i--){
        // calculate the role and lock level 
         role_l=(current_index&(1 << (levels- i- 1))) >> (levels- i -1);
         lock_l = current_index >> (levels-i);
         array_index = lock_l + (1 << i) - 1;
         lock_id = peterson_locks[array_index];
         // acquire the lock
        if (peterson_acquire(lock_id, role_l)<0) {
            return -1;
        }
    }
    return 0;
}
int tournament_release(void){   
    int role_l, lock_l, array_index, lock_id;
    // release the locks in order from the root to the leaf
    for (int i=0;i<levels;i++){
         role_l=(current_index&(1 << (levels- i- 1))) >> (levels- i -1);
         lock_l = current_index >> (levels-i);
         array_index = lock_l + (1 << i) - 1;
         lock_id = peterson_locks[array_index];
        // release the lock
        if (peterson_release(lock_id, role_l)<0) {
            return -1;
        }
    }
    return 0;
}