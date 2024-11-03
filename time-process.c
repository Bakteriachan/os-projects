#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
	if(argc == 1) {
		printf("Expected more than 1 argument\n");
		exit(1);
	}
	char *name = "shm_time_process";
	int fd = shm_open(name, O_RDWR | O_CREAT, 0666);
	if(fd == -1) {
		printf("Error while creating shared memory file descriptor\n");
		exit(1);
	}
	struct timeval tv;
	size_t size = sizeof(tv) + 1;
	ftruncate(fd, size);
	char *ptr;
	ptr = (char *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	gettimeofday(&tv, NULL);
	pid_t pid = fork();
	if(pid == -1) {
		printf("An error occurred when forking\n");
		exit(1);
	}

	if(pid == 0) { /* child process */
		sprintf(ptr, "%ld %ld", tv.tv_sec, tv.tv_usec);
		
		int nullfd = open("/dev/null", O_WRONLY);
		dup2(nullfd, STDOUT_FILENO);
		char *n_argv[argc];
		for(int i = 1; i < argc;i++) {
			n_argv[i-1] = argv[i];
		}
		n_argv[argc-1] = (char *)NULL;

		execvp(argv[1], n_argv);
	} else { /* parent process */
		int wstatus;
		wait(&wstatus);
		if(!WIFEXITED(wstatus)) {
			printf("child didnt exited correctly");
			exit(1);
		}
		gettimeofday(&tv, NULL);
		time_t secs = 0;
		suseconds_t usecs = 0;
		sscanf(ptr, "%ld %ld", &secs, &usecs);
		int total = tv.tv_sec - secs;
		int utotal = 0;
		if(tv.tv_usec > usecs) {
			utotal = tv.tv_usec - usecs;
		} else {
			utotal = ((int)1e6 - usecs) + tv.tv_usec;
		}
		printf("Time: %d.%ds\n", total, utotal);
		
	}
	shm_unlink(name);
	return 0;
}
