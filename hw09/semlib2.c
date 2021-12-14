#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"


//semaphore를 mutex, condition variable로 생성
int
sem_init(sem_t* sem, int pshared, int value)
{
	if (pshared) {
		fprintf(stderr, "Function not supported\n");
		return -1;
	}

	//mutex 생성
	if (pthread_mutex_init(&sem->mutex, NULL) < 0)
		return -1;

	//condition variable 생성
	if (pthread_cond_init(&sem->cond, NULL) < 0)
		return -1;

	//semaphore 값 초기화
	sem->sval = value;

	return 0;
}

//semaphore를 기다림
int
sem_wait(sem_t* sem)
{
	//cirtical section이므로 mutex lock
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	//semaphore가 없으면
	if (sem->sval == 0) {
		while (sem->sval == 0) {
			//condition variable을 기다림, 일시적으로 mutex unlock
			if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0) {
				if (pthread_mutex_unlock(&sem->mutex) < 0)
					return -1;
				return -1;
			}
		}
		//semaphore의 값 1 감소
		sem->sval--;
	}
	else {
		//semaphore의 값 1 감소
		sem->sval--;
	}

	//critical section을 빠져나옴, mutex unlock
	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

//sem_wait과 유사
//sem_wait과의  차이점 : semaphore가 없으면 기다리지 않고 fail return
int
sem_trywait(sem_t* sem)
{
	//critical section이므로 mutex lock
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	//semaphore가 없으면
	if (sem->sval == 0) {
		//기다리지 않고 critical section을 빠져나옴, mutex unlock
		if (pthread_mutex_unlock(&sem->mutex) < 0)
			return -1;
		return -1;
	}
	else {
		sem->sval--;	//semaphore의 값 1 감소
	}

	//critical section을 빠져나옴, mutex unlock
	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

//semaphore를 반납
int
sem_post(sem_t* sem)
{
	//critical section이므로 mutex lock
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	//원래 semaphore가 없었다면
	if (sem->sval == 0) {
		//condition variable을 pthread_cond_wait으로 기다리는 스레드를 깨움
		if (pthread_cond_signal(&sem->cond) < 0) {
			//critical section을 빠져나옴, mutex unlock
			if (pthread_mutex_unlock(&sem->mutex) < 0)
				return -1;
			return -1;
		}
	}

	sem->sval++;	//semaphore의 값 1 증가

	//critical section을 빠져나옴, mutex unlock
	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

//현재 semaphore의 값 return
int
sem_getvalue(sem_t* sem, int* sval)
{
	*sval = sem->sval;

	return 0;
}

//semaphore 삭제
int
sem_destroy(sem_t* sem)
{
	//mutex, condition variable 삭제
	if (pthread_mutex_destroy(&sem->mutex) < 0)
		return -1;

	if (pthread_cond_destroy(&sem->cond) < 0)
		return -1;

	return 0;
}
