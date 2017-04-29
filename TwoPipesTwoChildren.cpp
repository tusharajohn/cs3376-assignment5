#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <fcntl.h>

#include <sys/types.h>

#include <sys/stat.h>

int main(int argc, char **argv) {

int status;

char *cat_args[] = {"ls", "-ltr", NULL};

char *grep_args[] = {"grep", "3340", NULL};

char *wc_args[] = {"wc", "-l", NULL};

//create the necessary pipes
int pipe_One[2];

int pipe_Two[2];

//send the output of the process to grep and wc
pipe(pipe_One);

pipe(pipe_Two);

pid_t pid_One, pid_Two;

//produce the first child
//fork here

if( !(pid_One = fork()) ) {

close(pipe_One[0]);

//allow the output to go to the writing end of the first pipe
dup2(pipe_One[1], 1); 

close(pipe_One[1]);

execvp(*cat_args, cat_args);

exit(0);

}

//produce the second child
//fork again here

else if( !(pid_Two = fork()) ) {

close(pipe_One[1]);

//here, redirect the output to the reading end of the first pipe
dup2(pipe_One[0], 0); 

close(pipe_One[0]);

close(pipe_Two[0]);

//here, redirect the output to the writing end of the second pipe
dup2(pipe_Two[1], 1);

close(pipe_Two[1]);

execvp(*grep_args, grep_args);

}

//parent

else {

close(pipe_One[1]);

close(pipe_One[0]);

close(pipe_Two[1]);

//here, redirect the standard input to the reading end of the second pipe
dup2(pipe_Two[0], 0);

close(pipe_Two[0]);

execvp(*wc_args, wc_args);

}

close(pipe_Two[1]);

close(pipe_Two[0]);

return(0);

}
