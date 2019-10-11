#ifndef FILE_H
#define FILE_H

typedef struct{
  int fd;
  int status;
} simpsh_file_t;

typedef struct{
  simpsh_file_t* files;
  int num;
} simpsh_filetable_t;

simpsh_filetable_t* SIMPSH_FILETABLE_T_INIT();
void SIMPSH_FILETABLE_T_DESTROY();

int addFile(simpsh_filetable_t*, int, int);
int closeAllFiles(simpsh_filetable_t*);
int findFile(simpsh_filetable_t*, int, int*);
int replaceFiles(simpsh_filetable_t*, int*);

#endif
