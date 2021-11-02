#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <pthread.h>

/*
[Function Name] : Add50
[Description]   : 입력받은 값부터 50개의 숫자의 합을 return
[Input]
   int   start   //50개 숫자의 첫번째 값
[Output]        : 50개의 숫자의 합
[Call By]       : main()
[Calls]
   pthread_exit()   //thread 종료
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
    pthread_exit((void*)&sum);//thread 종료, 숫자들의 합을 가리키는 pointer를 return
}

/*
[Program Name]  : thread program
[Description]   : 두개의 thread를 생성하여 각각 1~50, 51~100까지의 합을 구하고, main thread가 각 thread가 종료되기를 기다린 후 각각의 합을 더하여 출력
[Input]         : Nothing.
[Output]        : 1-100까지의 합
[Calls]as
        perror()        //에러 메세지 출력
        exit()          //프로그램 종료
        printf()        //메세지 출력
   pthread_create()//thread 생성
   pthread_join()   //thread가 종료되기를 기다림
   Add50()      //입력받은 값부터 50개의 숫자의 합을 return
*/
int
main()
{
    pthread_t   tid1, tid2;
    void* sum;
    int      start1 = 1;
    int      start2 = 51;

    //&tid : 생성한 thread의id
    //NULL : thread의 attribute
    //Add50 : thread에서 실행할 함수
    //start1 : add50 함수의 파라미터
    //add50(start1)를 실행하는 thread 생성
    if (pthread_create(&tid1, NULL, (void*)Add50, (void*)&start1) < 0) {//thread 생성에 실패하면 프로그램 종료
        perror("pthread_create");
        exit(1);
    }

    //add50(start2)를 실행하는 thread 생성
    if (pthread_create(&tid2, NULL, (void*)Add50, (void*)&start2) < 0) {//thread 생성에 실패하면 프로그램 종료
        perror("pthread_create");
        exit(1);
    }

    //두 thread를 생성했음을 출력
    printf("Threads created: tid=%ld, %ld\n", tid1, tid2);

    //tid1이 id인 thread가 종료되기를 기다림
    //숫자들의 합을 가리키는 void* 변수를 return value로 받아옴
    if (pthread_join(tid1, &sum) < 0) {
        perror("pthread_join");
        exit(1);
    }

    //tid2가 id인 thread가 종료되기를 기다림
    //숫자들의 합을 가리키는 void* 변수를 return value로 받아옴
    if (pthread_join(tid2, &sum) < 0) {
        perror("pthread_join");
        exit(1);
    }

    //두 thread를 종료되었음을 출력
    printf("Threads terminated: tid=%ld, %ld\n", tid1, tid2);

    //두 thread로부터 구한 1-100까지의 합 출력
    printf("The sum from 1 to 100 is %d\n", *((int*)sum));
}
