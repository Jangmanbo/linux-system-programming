#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"


//semaphore�� mutex, condition variable�� ����
int
sem_init(sem_t* sem, int pshared, int value)
{
	if (pshared) {
		fprintf(stderr, "Function not supported\n");
		return -1;
	}

	//mutex ����
	if (pthread_mutex_init(&sem->mutex, NULL) < 0)
		return -1;

	//condition variable ����
	if (pthread_cond_init(&sem->cond, NULL) < 0)
		return -1;

	//semaphore �� �ʱ�ȭ
	sem->sval = value;

	return 0;
}

//semaphore�� ��ٸ�
int
sem_wait(sem_t* sem)
{
	//cirtical section�̹Ƿ� mutex lock
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	//semaphore�� ������
	if (sem->sval == 0) {
		while (sem->sval == 0) {
			//condition variable�� ��ٸ�, �Ͻ������� mutex unlock
			if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0) {
				if (pthread_mutex_unlock(&sem->mutex) < 0)
					return -1;
				return -1;
			}
		}
		//semaphore�� �� 1 ����
		sem->sval--;
	}
	else {
		//semaphore�� �� 1 ����
		sem->sval--;
	}

	//critical section�� ��������, mutex unlock
	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

//sem_wait�� ����
//sem_wait����  ������ : semaphore�� ������ ��ٸ��� �ʰ� fail return
int
sem_trywait(sem_t* sem)
{
	//critical section�̹Ƿ� mutex lock
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	//semaphore�� ������
	if (sem->sval == 0) {
		//��ٸ��� �ʰ� critical section�� ��������, mutex unlock
		if (pthread_mutex_unlock(&sem->mutex) < 0)
			return -1;
		return -1;
	}
	else {
		sem->sval--;	//semaphore�� �� 1 ����
	}

	//critical section�� ��������, mutex unlock
	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

//semaphore�� �ݳ�
int
sem_post(sem_t* sem)
{
	//critical section�̹Ƿ� mutex lock
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	//���� semaphore�� �����ٸ�
	if (sem->sval == 0) {
		//condition variable�� pthread_cond_wait���� ��ٸ��� �����带 ����
		if (pthread_cond_signal(&sem->cond) < 0) {
			//critical section�� ��������, mutex unlock
			if (pthread_mutex_unlock(&sem->mutex) < 0)
				return -1;
			return -1;
		}
	}

	sem->sval++;	//semaphore�� �� 1 ����

	//critical section�� ��������, mutex unlock
	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

//���� semaphore�� �� return
int
sem_getvalue(sem_t* sem, int* sval)
{
	*sval = sem->sval;

	return 0;
}

//semaphore ����
int
sem_destroy(sem_t* sem)
{
	//mutex, condition variable ����
	if (pthread_mutex_destroy(&sem->mutex) < 0)
		return -1;

	if (pthread_cond_destroy(&sem->cond) < 0)
		return -1;

	return 0;
}
