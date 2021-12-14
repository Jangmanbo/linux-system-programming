#include <stdio.h>
#include <unistd.h>
#include "semlib.h"

//semaphore ����
int
semInit(key_t key)
{
    int     semid;

    //1���� semaphore�� ����, return id
    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0) {
        perror("semget");
        return -1;
    }

    return semid;
}

//semaphore�� ���� �ʱ�ȭ
int
semInitValue(int semid, int value)
{
    union semun {
        int     val;
    } semun;

    semun.val = value;
    //id�� semid�� semaphore�� ���� semun���� �ʱ�ȭ
    if (semctl(semid, 0, SETVAL, semun) < 0) {
        perror("semctl");
        return -1;
    }

    return semid;
}

//semaphore�� ������ ���, ������ ��ٸ�
int
semWait(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0;	//semaphore�� �ϳ��̹Ƿ� 0
    semcmd.sem_op = -1;	//semaphore�� ��ٸ�
    //SEM_UNDO : ���μ����� ���������� ���� �� semaphore�� �ݳ�
    semcmd.sem_flg = SEM_UNDO;
    //id�� semid�� semaphore�� ���� 1 ����, ���� 0�̸� 1�� �� ������ wait
    if (semop(semid, &semcmd, 1) < 0) {
        perror("semop");
        return -1;
    }

    return 0;
}

//semaphore�� ������ ���, ��� ��ٸ��� ����
int
semTryWait(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0; //semaphore�� �ϳ��̹Ƿ� 0
    semcmd.sem_op = -1; //semaphore�� ��ٸ�
    //IPC_NOWAIT : semaphore�� ���ٸ� ��ٸ��� �ʰ� fail�� return
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO;
    //semaphore�� ������ ���� 1 ����, ������ fail�� return
    if (semop(semid, &semcmd, 1) < 0) {
        perror("semop");
        return -1;
    }

    return 0;
}

//semaphore �ݳ�
int
semPost(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0;
    semcmd.sem_op = 1;
    //SEM_UNDO : ���μ����� ���������� ���� �� semaphore�� �ݳ�
    semcmd.sem_flg = SEM_UNDO;
    //semaphore�� ���� 1 ����
    if (semop(semid, &semcmd, 1) < 0) {
        perror("semop");
        return -1;
    }

    return 0;
}

//semaphore�ǰ��� return
int
semGetValue(int semid)
{
    union semun {
        int     val;
    } dummy;

    //semaphore�ǰ��� return
    return semctl(semid, 0, GETVAL, dummy);
}

//semaphore ����
//semDestroy�� ���� �ʰ� ���μ����� ���������� ���� �� semaphore�� ��� ����
int
semDestroy(int semid)
{
    union semun {
        int     val;
    } dummy;

    //IPC_RMID flag�� ���� semaphore����
    if (semctl(semid, 0, IPC_RMID, dummy) < 0) {
        perror("semctl");
        return -1;
    }
    close(semid);

    return 0;
}
