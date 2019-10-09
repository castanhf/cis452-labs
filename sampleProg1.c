#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SIZE 16

int main (int argc, char *argv[])
{
   int status;
   long int i, loop, temp, *shmPtr;
   int shmId;
   int semId;
   pid_t pid;

   // NEW: sembufs used to lock and unlock the semaphore
   struct sembuf sbuf[2];

      // get value of loop variable (from command-line argument)
   if(argc < 2) {
      perror("Usage: /a.out num\n");
      exit(1);
   }
   loop = atoi(argv[1]);

   // create semaphore set
   if((semId = semget(IPC_PRIVATE, 1, 00600)) < 0) {
      perror("semget() call failed, could not create semaphore!\n");
      exit(1);
   }
   // initialize semaphore
   if((semctl(semId, 0, SETVAL, 1)) < 0) {
      perror("semop() call failed, could not initialize semaphore!\n");
      exit(1);
   }

   if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("i can't get no..\n");
      exit (1);
   }
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit (1);
   }

   shmPtr[0] = 0;
   shmPtr[1] = 1;

   // initialize sbuf variables
   // sbuf[0]
   sbuf[0].sem_num = 0;
   sbuf[0].sem_op = -1;
   sbuf[0].sem_flg = 0;

   // sbuf[1]
   sbuf[1].sem_num = 0;
   sbuf[1].sem_op = 1;
   sbuf[1].sem_flg = 0;

   if (!(pid = fork())) {
      for (i=0; i<loop; i++) {
	 semop(semId, &sbuf[0], 1);
               // swap the contents of shmPtr[0] and shmPtr[1]
	 temp = shmPtr[0];
	 shmPtr[0] = shmPtr[1];
	 shmPtr[1] = temp;

	 semop(semId, &sbuf[1], 1);
      }
      if (shmdt (shmPtr) < 0) {
         perror ("just can't let go\n");
         exit (1);
      }
      exit(0);
   }
   else
      for (i=0; i<loop; i++) {
	 semop(semId, &sbuf[0], 1);
               // swap the contents of shmPtr[1] and shmPtr[0]
	 temp = shmPtr[1];
	 shmPtr[1] = shmPtr[0];
	 shmPtr[0] = temp;

	 semop(semId, &sbuf[1], 1);
      }

   wait (&status);
   printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

   // Free pointer
   if (shmdt (shmPtr) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }
   // Delete shared memory
   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   // Delete semaphore
   if(semctl(semId, 0, IPC_RMID) < 0) {
      perror("Could not remove/destroy semaphore \n");
      exit(1);
   }

   return 0;
}
