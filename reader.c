#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void sigHandler(int);

key_t key;
int mId;
char *mPtr;


typedef struct {
  int turn;
  char message[1024];
} DataShared;

int main() {
  DataShared data;
  data.turn = 0;
  signal(SIGINT, sigHandler);

  //generates key
  key = ftok("mkey",65);

  //returns an identifier in mId
  if ((mId = shmget(key, 4096, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
    perror("shared memory error");
    exit(1);
  }

  // shmat to attach to shared memory
  if((mPtr = shmat(mId, 0, 0)) == (void*) -1) {
    perror("Can't attach\n");
    exit(1);
  }

  while(strcmp(data.message, "exit") != 0) {
    // request critical section
    while(!data.turn) {
      //not time for the reader, check if token is changed.
      memcpy(&data, mPtr, sizeof(DataShared));
    }

    // enter critical section
    printf("Read from memory: %s\n", data.message);

    // leave critical section
    data.turn = 0;

    memcpy(mPtr, &data, sizeof(DataShared));
  };

  kill(getpid(),SIGINT);

  return 0;
}

/* Shut down process */
void sigHandler(int num) {
  printf("\nThat's it, I'm shutting you down...\n");

  // Destroy shared memory
  if (shmctl(mId, IPC_RMID, NULL) < 0) {
    perror("Can't deallocate");
    exit(1);
  }
  exit(0);
}
