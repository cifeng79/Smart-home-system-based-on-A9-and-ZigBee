#ifndef __MONITOR_SEM_H__
#define __MONITOR_SEM_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
								(Linux-specific) */
};


int init_sem(int semid, int num, int val);
int sem_p(int semid, int num);
int sem_v(int semid, int num);

#endif

