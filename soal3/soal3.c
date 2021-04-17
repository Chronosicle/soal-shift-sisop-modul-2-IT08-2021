#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <string.h>
#include <signal.h>

void generateKiller(int mode) {
	FILE* killer;
	killer = fopen("killer.sh", "w");
	fprintf(killer, "#!/bin/bash\nkill %d\necho \'Killed program.\'\nrm \"$0\"", getpid());
	fclose(killer);
	pid_t child = fork();
	if (child == 0) {
		execl("/bin/chmod", "chmod", "u+x", "killer.sh", NULL);
	}
	int status;
	while(wait(&status) > 0);
}

void statustxt(){
	char msg[100] = "Download Success", arr;
	for(int j = 0; j < 16 ; j++){
		arr = msg[j];
		if(arr >= 'a' && arr <= 'z'){
			arr+=5;
			if(arr > 'z') arr = arr - 'z' + 'a' - 1;
			msg[j] = arr;
		}else if(arr >= 'A' && arr <= 'Z'){
			arr+=5;
			if(arr > 'Z') arr = arr - 'Z' + 'A' - 1;
			msg[j] = arr;
		}
	}
	FILE *tulis;
	tulis = fopen("status.txt", "w");
	fprintf(tulis, "%s", msg);
	fclose(tulis);
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Program hanya bisa dijalankan dengan mode z atau mode x!\n");
		exit(EXIT_FAILURE);
	}
	int mode;
	if (argv[1][1] == 'z') mode = 1;
	else if (argv[1][1] == 'x') mode = 0;
	
	pid_t pid, sid;
	pid = fork();
	if (pid < 0) exit(EXIT_FAILURE);
	if (pid > 0) exit(EXIT_SUCCESS);
	umask(0);
	sid = setsid();
	if (sid < 0) exit(EXIT_FAILURE);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	generateKiller(mode);
	
	while (1) {
		time_t t;
		time(&t);
		struct tm* lt = localtime(&t);
		char namafolder[100];
		sprintf(namafolder, "%d-%02d-%02d_%02d:%02d:%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);

		pid_t child = fork();
		if (child == 0) {
			execl("/bin/mkdir", "mkdir", namafolder, NULL);
		}
		int status;
		wait(&status);

		child = fork();
		if (child == 0) {
			if (mode) prctl(PR_SET_PDEATHSIG, SIGHUP);
			chdir(namafolder);
			
			int i;
			for (i = 0; i < 10; i++) {
				time_t t2;
				time(&t2);
				struct tm* lt2 = localtime(&t2);
				char url[100], name[100];
				sprintf(url, "https://picsum.photos/%ld", (t2 % 1000) + 50);
				sprintf(name, "%d-%02d-%02d_%02d:%02d:%02d", lt2->tm_year + 1900, lt2->tm_mon + 1, lt2->tm_mday, lt2->tm_hour, lt2->tm_min, lt2->tm_sec);
				pid_t child2 = fork();
				if (child2 == 0) {
					execl("/usr/bin/wget", "wget", url, "-O", name, "-o", "/dev/null", NULL);
					exit(EXIT_SUCCESS);
				}
				sleep(5);
			}
			statustxt();

			int status2;

			while(wait(&status2) > 0);
			chdir("..");
			char namazip[150];
			sprintf(namazip, "%s.zip", namafolder);

			pid_t child3 = fork();
			if (child3 == 0) {
				execl("/usr/bin/zip", "zip", "-r", namazip, namafolder, NULL);
			}
			int status3;
			while (wait(&status3) > 0);	
			execl("/bin/rm", "rm", "-rf", namafolder, NULL);
		}
		sleep(40);
	}
}