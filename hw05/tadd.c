#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <pthread.h>

/*
[Function Name] : Add50
[Description]   : �Է¹��� ������ 50���� ������ ���� return
[Input]
   int   start   //50�� ������ ù��° ��
[Output]        : 50���� ������ ��
[Call By]       : main()
[Calls]
   pthread_exit()   //thread ����
[Given]         : Nothing
[Returns]       : Nothing
*/
void*
Add50(void* s)
{
    static int sum = 0;
    int   start = *((int*)s);//void* to int convert
    for (int i = start; i < start + 50; i++) {
        sum += i;
    }
    pthread_exit((void*)&sum);//thread ����, ���ڵ��� ���� ����Ű�� pointer�� return
}

/*
[Program Name]  : thread program
[Description]   : �ΰ��� thread�� �����Ͽ� ���� 1~50, 51~100������ ���� ���ϰ�, main thread�� �� thread�� ����Ǳ⸦ ��ٸ� �� ������ ���� ���Ͽ� ���
[Input]         : Nothing.
[Output]        : 1-100������ ��
[Calls]as
        perror()        //���� �޼��� ���
        exit()          //���α׷� ����
        printf()        //�޼��� ���
   pthread_create()//thread ����
   pthread_join()   //thread�� ����Ǳ⸦ ��ٸ�
   Add50()      //�Է¹��� ������ 50���� ������ ���� return
*/
int
main()
{
    pthread_t   tid1, tid2;
    void* sum;
    int      start1 = 1;
    int      start2 = 51;

    //&tid : ������ thread��id
    //NULL : thread�� attribute
    //Add50 : thread���� ������ �Լ�
    //start1 : add50 �Լ��� �Ķ����
    //add50(start1)�� �����ϴ� thread ����
    if (pthread_create(&tid1, NULL, (void*)Add50, (void*)&start1) < 0) {//thread ������ �����ϸ� ���α׷� ����
        perror("pthread_create");
        exit(1);
    }

    //add50(start2)�� �����ϴ� thread ����
    if (pthread_create(&tid2, NULL, (void*)Add50, (void*)&start2) < 0) {//thread ������ �����ϸ� ���α׷� ����
        perror("pthread_create");
        exit(1);
    }

    //�� thread�� ���������� ���
    printf("Threads created: tid=%ld, %ld\n", tid1, tid2);

    //tid1�� id�� thread�� ����Ǳ⸦ ��ٸ�
    //���ڵ��� ���� ����Ű�� void* ������ return value�� �޾ƿ�
    if (pthread_join(tid1, &sum) < 0) {
        perror("pthread_join");
        exit(1);
    }

    //tid2�� id�� thread�� ����Ǳ⸦ ��ٸ�
    //���ڵ��� ���� ����Ű�� void* ������ return value�� �޾ƿ�
    if (pthread_join(tid2, &sum) < 0) {
        perror("pthread_join");
        exit(1);
    }

    //�� thread�� ����Ǿ����� ���
    printf("Threads terminated: tid=%ld, %ld\n", tid1, tid2);

    //�� thread�κ��� ���� 1-100������ �� ���
    printf("The sum from 1 to 100 is %d\n", *((int*)sum));
}
