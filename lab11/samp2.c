#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define D_LEN 1024

int main(int argc, char** argv)
{
   DIR *dirPtr;
   struct dirent *entryPtr;
   struct stat stat_buf;
   int option;

   // checking directory
   char* direc = argv[2];
   if(stat(direc, &stat_buf) < 0) {
      perror("Invalid directory.\n");
      return -1;
   }

   //int j = 0;
   while((option = getopt(argc, argv, "n:i")) != -1) {
      switch(option) {
         case 'n':
            dirPtr = opendir(direc);
            while((entryPtr = readdir(dirPtr))) {
               struct stat st;
               char entry[D_LEN];
               stat((entryPtr->d_name), &st);
               printf("%-20s uid: %d gid: %d\n", entryPtr->d_name, st.st_uid, st.st_gid);
               snprintf(entry, "%-20s uid: %d gid: %d\n", entryPtr->d_name, st.st_uid, st.st_gid, D_LEN);
            }
            break;
         case 'i':
            dirPtr = opendir(direc);
            while((entryPtr = readdir(dirPtr))) {
               struct stat st;
               char entry[D_LEN];
               stat((entryPtr->d_name), &st);
               printf("%-20s inode: %lu\n", entryPtr->d_name, st.st_ino);
               snprintf(entry, "%-20s inode: %lu\n", entryPtr->d_name, st.st_ino D_LEN);
            }
            break;
         default:
            dirPtr = opendir ("./");
            while ((entryPtr = readdir (dirPtr))) {
               //stat(entryPtr->d_name, &st);
               //size = st.st_size;
               //printf ("%-20s--> Size: %d bytes\n", entryPtr->d_name, size);
               struct stat st;
               stat((entryPtr->d_name), &st);
               printf("%-20s\n", entryPtr->d_name);
            }
      
      }
   }

   //closedir (dirPtr);
   //return 0;
} 
