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

  //generate a key
  key = ftok("mkey",65);

  if((mId = shmget(key, 4096, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0 ) {
    perror("Error creating shared memory\n");
    exit(1);
  }

  if((mPtr = shmat(mId, 0, 0)) == (void*) -1) {
    perror("Can't attach\n");
    exit(1);
  }

  while(strcmp(data.message, "exit") != 0) {
    // request the critical section
    while (data.turn) {
      // not time for the writer, check if token is changed.
      memcpy(&data, mPtr, sizeof(DataShared));
    }
    
    // enter critical section
    printf("Enter a message: \n" );
    scanf("%s", data.message);
    
    // leave critical section
    printf("Message written to memory: %s\n", data.message);
    data.turn = 1;
    memcpy(mPtr, &data, sizeof(DataShared));
  };

  kill(getpid(),SIGINT);

  return 0;
}

/* Shut down process */
void sigHandler(int num) {
  printf("That's it, I'm shutting you down...\n");
  
  // detach from memory
  if (shmdt (mPtr) < 0) {
    perror ("just can't let go\n");
    exit (1);
  }

  // destroy shared memory
  if (shmctl (mId, IPC_RMID, 0) < 0) {
    perror ("can't deallocate\n");
    exit(1);
  }

  exit(0);
}
