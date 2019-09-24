#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX_THREADS 30

int files = 0; //keeps count of files sent
int served = 0; //keeps count of files serviced

void* fileget(void* arg);
void usrHandler(int signNum);

pthread_mutex_t lock;

int main() {
  pthread_t threads[MAX_THREADS];
  int status;
  char input[256];
  char* inputstr = input;
  int counter = 0; // count spot in threads

  if(pthread_mutex_init(&lock, NULL) != 0){
    printf("\n mutex init failed\n");
    return 1;
  }

  signal(SIGINT, usrHandler); //creating SIGINT for termination

  // dispatcher reads user input, creates thread, detaches thread, and repeats
  while(1) {
    
    // gets the user's information
    printf("\nEnter File Name: ");
    scanf("%s", inputstr);
    
    // creates worker thread
    if ((status = pthread_create(&threads[counter], NULL, fileget, (void*)inputstr)) != 0) {
      fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
      exit(1);
    }

    // detach the child to execute independently until termination
    if ((status = pthread_detach (threads[counter])) != 0) {
      fprintf(stderr, "thread detach error %d: %s\n", status, strerror(status));
      exit(1);
    }
    
    // update counter
    counter < MAX_THREADS ? (counter++) : (counter = 0);
    files = files + 1; //increment # of files sent		
    pthread_mutex_unlock(&lock);
  }
  
  pthread_mutex_destroy(&lock);

  return 0;

}

void* fileget(void* arg) {
  pthread_mutex_lock(&lock);

  char* str = malloc(strlen((char*) arg) + 1);
  strcpy(str, (char*) arg);
  srand(time(NULL));

  // generate 80/20 probability for sleep time
  float ran = (float)rand()/RAND_MAX;
  ran <= 0.8 ? sleep(1) : sleep((7 + rand() % (10 + 1 - 7)));

  //diagnostic message
  printf("\nThread got the string: %s\n", str);
  served = served + 1; //increment # of files done
	
  free(str);

 // pthread_mutex_unlock(&lock);

  return NULL;
}

void usrHandler(int x) { 
  //terminates when control c is recieved
  if (x == SIGINT){
    printf("\n\nControl-c registered.\n");
    printf("Number of file requests recieved: %d\n", files);
    printf("Number of file requests serviced: %d\n", served);
    printf("Exiting...\n\n");
    exit(0);
  }
}
