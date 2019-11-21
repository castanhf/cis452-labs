#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char** argv) {
  DIR *dir_ptr;
  struct dirent *entry_ptr;
  struct stat stat_buf;

  int opt;

  char* directory = argv[2];
  if (stat(directory, &stat_buf) < 0) {
    perror("Invalid input supplied");
    return -1;
  }

  while ((opt = getopt(argc, argv, "n:i")) != -1) {
    switch(opt) {
      case 'n':
        dir_ptr = opendir(directory);
        while ((entry_ptr = readdir(dir_ptr))) {
          struct stat st;
          stat((entry_ptr->d_name), &st);
          printf("%-20s uid: %d gid: %d\n", entry_ptr->d_name, st.st_uid, st.st_gid);
        }
        break;
      case 'i':
        dir_ptr = opendir(directory);
        while ((entry_ptr = readdir(dir_ptr))) {
          struct stat st;
          stat((entry_ptr->d_name), &st);
          printf("%-20s inode: %lu\n", entry_ptr->d_name, st.st_ino);
        }
        break;
      case '?':
        printf("invalid option specified");
        break;
      default:
        dir_ptr = opendir("./");
        while ((entry_ptr = readdir(dir_ptr))) {
          struct stat st;
          stat((entry_ptr->d_name), &st);
          printf("%-20s\n", entry_ptr->d_name);
        }
    }
  }
}
