#include "Semaphore.h"
#include <sys/types.h>
#include <unistd.h>

// Esta funcion no debe ser ejecutada por dos procesos que traten de crearlo
// al mismo tiempo, OJO! Si se puede ejecutar si ya fue creado...
int Semaphore_init(Semaphore *S, const char *filename, int init_val, int crear){
    //Creo la key
    key_t key = ftok(filename, SEM_DEFAULT_NUM);
    int flags = 0666;
    if (crear)
      flags |= IPC_EXCL | IPC_CREAT;
    if (key == ERROR_FTOK) {
        return ERROR_FTOK;
    }

    S->init_val = init_val;

    S->id = semget(key, 1, flags);
    if (S->id < 0) {
        return ERROR_SEMGET;
    }
    if (!crear)
      return SEMAPHORE_OK;

    union semun dummy;
    dummy.val = S->init_val;

    return semctl(S->id, 0 , SETVAL, dummy);
}

int Semaphore_v(Semaphore *S){
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1; //incremento 1
    op.sem_flg = SEM_UNDO;
    return semop(S->id, &op, 1);
}

int Semaphore_p(Semaphore *S){
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1; //decremento 1
    op.sem_flg = SEM_UNDO;
    return semop(S->id, &op, 1);
}

int Semaphore_eliminar(Semaphore *S){
    return semctl(S->id, 0, IPC_RMID);
}
