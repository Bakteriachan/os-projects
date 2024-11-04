#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if(argc == 1) {
		printf("Expected more than 1 argument\n");
		exit(1);
	}

	int fd[2];
	pid_t pid;
	char *buff; 
	char *n_args[argc];
	for(int i = 1; i < argc; i++) n_args[i-1] =  argv[i];
	n_args[argc-1] = NULL;

	struct timeval tv;
	pipe(fd);
	gettimeofday(&tv, NULL);
	pid = fork();
	if(pid == 0) { /** child process */
		close(fd[0]);
		int null_fd = open("/dev/null", O_WRONLY, 0600);
		dup2(null_fd, STDOUT_FILENO);
		sprintf(buff, "%ld %ld", tv.tv_sec, tv.tv_usec);
		write(fd[1], buff, strlen(buff) + 1);
		close(fd[1]);
		execvp(argv[1], n_args);
	} else { /* parent process */ 
		close(fd[1]);
		wait(NULL);
		char buff[1024];
		read(fd[0], buff, 1024);
		long int secs, usecs;
		sscanf(buff, "%ld %ld", &secs, &usecs);
		gettimeofday(&tv, NULL);
		secs = tv.tv_sec - secs;
		if(tv.tv_usec > usecs) {
			usecs = tv.tv_usec - usecs;
		} else {
			usecs = (unsigned int) (1e7) - usecs + tv.tv_usec; 
		}
		printf("%ld.%ld\n", secs, usecs);
		close(fd[0]);
	}


	return 0;
}
