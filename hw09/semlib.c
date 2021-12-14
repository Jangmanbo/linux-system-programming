#include <stdio.h>
#include <unistd.h>
#include "semlib.h"

//semaphore 생성
int
semInit(key_t key)
{
    int     semid;

    //1개의 semaphore를 생성, return id
    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0) {
        perror("semget");
        return -1;
    }

    return semid;
}

//semaphore의 값을 초기화
int
semInitValue(int semid, int value)
{
    union semun {
        int     val;
    } semun;

    semun.val = value;
    //id가 semid인 semaphore의 값을 semun으로 초기화
    if (semctl(semid, 0, SETVAL, semun) < 0) {
        perror("semctl");
        return -1;
    }

    return semid;
}

//semaphore가 있으면 사용, 없으면 기다림
int
semWait(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0;	//semaphore가 하나이므로 0
    semcmd.sem_op = -1;	//semaphore를 기다림
    //SEM_UNDO : 프로세스의 비정상적인 종료 시 semaphore를 반납
    semcmd.sem_flg = SEM_UNDO;
    //id가 semid인 semaphore의 값을 1 감소, 값이 0이면 1이 될 때까지 wait
    if (semop(semid, &semcmd, 1) < 0) {
        perror("semop");
        return -1;
    }

    return 0;
}

//semaphore가 있으면 사용, 없어도 기다리지 않음
int
semTryWait(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0; //semaphore가 하나이므로 0
    semcmd.sem_op = -1; //semaphore를 기다림
    //IPC_NOWAIT : semaphore가 없다면 기다리지 않고 fail을 return
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO;
    //semaphore가 있으면 값을 1 감소, 없으면 fail을 return
    if (semop(semid, &semcmd, 1) < 0) {
        perror("semop");
        return -1;
    }

    return 0;
}

//semaphore 반납
int
semPost(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0;
    semcmd.sem_op = 1;
    //SEM_UNDO : 프로세스의 비정상적인 종료 시 semaphore를 반납
    semcmd.sem_flg = SEM_UNDO;
    //semaphore의 값을 1 증가
    if (semop(semid, &semcmd, 1) < 0) {
        perror("semop");
        return -1;
    }

    return 0;
}

//semaphore의값을 return
int
semGetValue(int semid)
{
    union semun {
        int     val;
    } dummy;

    //semaphore의값을 return
    return semctl(semid, 0, GETVAL, dummy);
}

//semaphore 삭제
//semDestroy를 하지 않고 프로세스가 비정상적인 종료 시 semaphore가 계속 축적
int
semDestroy(int semid)
{
    union semun {
        int     val;
    } dummy;

    //IPC_RMID flag를 통해 semaphore삭제
    if (semctl(semid, 0, IPC_RMID, dummy) < 0) {
        perror("semctl");
        return -1;
    }
    close(semid);

    return 0;
}
