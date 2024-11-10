#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h" 
#include "kernel/param.h"
#define MAXSTRLEN 100
int main(int argc, char *argv[])
{

    if(!strcmp(argv[0],"trace")){
    

      
      int n;
      n = atoi(argv[1]);
      trace(n);

      char *b_argv[MAXARG];  
      char b_strings[MAXARG][MAXSTRLEN];  


      int b_argc = 0;

      for (int i = 2; i < argc; i++) {

       strcpy(b_strings[b_argc], argv[i]);
       b_strings[b_argc][MAXSTRLEN - 1] = '\0';  

 
       b_argv[b_argc] = b_strings[b_argc];
       b_argc++;
}

       b_argv[b_argc] = 0;  
       exec(b_argv[0],b_argv);

    } else{
      trace(0);
      exec(argv[0],argv);
    }
    exit(0);
}

                            