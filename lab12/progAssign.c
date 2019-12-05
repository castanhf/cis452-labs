#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

//#define SIZE 30

int main (int argc, char *argv[]) {
    if (argc != 4) {
      printf("Usage:\n./a.out <link_type(h, s)> <target> <link_name>\n");
      exit(0);
    }

    //printf("argv[0] = %s\nargv[1] = %s\targv[1][1] = %c\nargv[2] = %s\nargv[3] = %s\n", argv[0], argv[1], argv[1][1], argv[2], argv[3]);

    if (argv[1][0] == 'h') {
      if ((link(argv[2], argv[3])) < 0) {
		perror("error creating hard link");
		exit(1);
      }
      printf("hard link called %s was created\n", argv[3]);
    }
    else if (argv[1][0] == 's') {
      if ((symlink(argv[2], argv[3])) < 0) {
		perror("error creating symbolic link");
		exit(1);
      }
      printf("symbolic link called %s was created\n", argv[3]);
    }

    return 0;
}
