#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
int main(int argc,char *argv[])
{
    int b_argc=1;
    char * b_argv[MAXARG];
    int i=0;
    char buf[512];
    char *start;
    char *end;
    if(strcmp(argv[1],"-n")==0)
    {
        i=3;
    }
    else{
        i=1;
    }
    b_argv[0]=argv[i];
    i++;
    for(;i<argc;i++)
    {
        b_argv[b_argc++]=argv[i];
    }
    start=end=buf;
    while(read(0,end,sizeof(char)))
    {
        if(*end=='\n')
        {
            if(b_argc==MAXARG-1)
            {
                fprintf(2,"too many arguments\n");
            }
            else{
                *end='\0';
                b_argv[b_argc++]=start;
                if(fork()==0)
                {
                    exec(b_argv[0],b_argv);
                    fprintf(2, "exec failed\n");
                    exit(1);
                }
                else
                { 
                    wait(0);
                }
            }
            start=end=buf;
            b_argc=argc-1;
        }
        else if(*end==' ')
        {
            if(b_argc==MAXARG-1)
            {
                fprintf(2,"too many arguments\n");
                while(*end!='\n')
                {
                    read(0,end,sizeof(char));
                }
                end=start=buf;
                b_argc=argc-1;
            }
            else
            {
                *end='\0';
                b_argv[b_argc++]=start;
                end++;
                start=end;
            }
        }
        else
        {
            end++;
            if(end>=buf+sizeof(buf))
            {
                end--;
                while(*end!='\n')
                {
                    read(0,end,sizeof(char));
                }
                fprintf(2,"too many inputs\n");
                end=start=buf;
                b_argc=argc-1;
            }
        }
    }
    exit(0);
}