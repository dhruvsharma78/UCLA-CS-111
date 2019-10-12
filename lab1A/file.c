// NAME: Dhruv Sharma
// EMAIL: dhruvsharma78@ucla.edu
// ID: 605082988

#include "file.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

simpsh_filetable_t* SIMPSH_FILETABLE_T_INIT(){
  simpsh_filetable_t* ft = (simpsh_filetable_t*) malloc(sizeof(simpsh_filetable_t));
  ft->num = 0;
  ft->files = (simpsh_file_t*) malloc(sizeof(simpsh_file_t));
  return ft;
}

void SIMPSH_FILETABLE_T_DESTROY(simpsh_filetable_t* ft){
  free(ft->files);
  free(ft);
  ft = NULL;
}

int addFile(simpsh_filetable_t* ft, int fd, int status){
  ft->files = (simpsh_file_t*) realloc(ft->files, sizeof(simpsh_file_t) * (ft->num+2));
  if(ft->files == NULL) return 1;
  ft->files[ft->num].fd = fd;
  ft->files[ft->num].status = status;
  ft->num++;
  return 0;
}

int findFile(simpsh_filetable_t* ft, int fd, int* status){
  if(fd < ft->num){
     *(status) = ft->files[fd].status;
     return 1;
  }else{
    return 0;
  }
}

int closeAllFiles(simpsh_filetable_t* ft){
  int err;
  int i;
  for(i=0; i<ft->num; ++i){
    if(ft->files[i].status){
      int err_c = close(ft->files[i].fd);
      if(err_c<0){
        err = 1;
      }
    }
  }
  return err;
}

int replaceFiles(simpsh_filetable_t* ft, int* newFiles){
  int i=0;
  for(;i<3;++i){
    if(dup2(ft->files[newFiles[i]].fd, i) < 0) return 1;
  }
  return 0;
}
