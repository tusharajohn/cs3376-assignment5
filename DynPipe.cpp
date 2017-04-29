//Tushara John
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
void input(int src, int dst) {
char buf[BUFSIZ];
ssize_t readingbytes, writtenbytes;
while ((readingbytes  = read(src, buf, BUFSIZ)) > 0) {
writtenbytes = 0;
while (writtenbytes < readingbytes)
writtenbytes += write(dst,
buf + writtenbytes,
readingbytes - writtenbytes);
}
}
int main(int argc, char** argv) {
//specify parameters for the argument 
if (argc < 2) {
printf("usage: %s <program> [<arg> ...]\n", basename(argv[0]));
return EXIT_FAILURE;
}
// create pipes 
//standard input
int stdin_pipe[2];
//standard output
int stdout_pipe[2];
//standard output
int stderr_pipe[2];

pipe(stdin_pipe);
pipe(stdout_pipe);
pipe(stderr_pipe);
if (fork() == 0) {
// make the child process
//right now, the writing end of stdin is open
//the reading ends of stdout and stderr are also open
//close these 
close(stdin_pipe[1]);
close(stdout_pipe[0]);
close(stderr_pipe[0]);
// make the child's stdin stdout stderr use pipes 
dup2(stdin_pipe[0], STDIN_FILENO);
dup2(stdout_pipe[1], STDOUT_FILENO);
dup2(stderr_pipe[1], STDERR_FILENO);
// finally, use execvp for the program
if (execvp(argv[1], argv+1) == -1) {
perror("failed to start subprocess");
return EXIT_FAILURE;
}
}
// parent process
// close readinf end of stdin and writing ends of stdout and stderr
close(stdin_pipe[0]);
close(stdout_pipe[1]);
close(stderr_pipe[1]);
// give input for the child process
input(STDIN_FILENO, stdin_pipe[1]);
close(stdin_pipe[1]);
// need to wait for the child process to finish
wait(NULL);
// now read the child's stdout stderr
puts("\nchild's stdout:");
fflush(stdout);
input(stdout_pipe[0], STDOUT_FILENO);
close(stdout_pipe[0]);
puts("\nchild's stderr:");
fflush(stdout);
input(stderr_pipe[0], STDOUT_FILENO);
close(stderr_pipe[0]);
return EXIT_SUCCESS;
}
