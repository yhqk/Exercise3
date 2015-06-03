// Harjoitus 4 – Consumer daemon 
// Luodaan daemon prosessi, joka kuluttaa tiedostoja.
//
// Luo demoniprosessi, joka tarkkailee tiedostoa tietyssä
// hakemistossa. Kun tiedosto ilmestyy, demoni poistaa sen ja loggaa
// asiasta daemon-logiin.
//
// Tiedoston olemassaolon testaaminen onnistuu, kun aukaiset
// fopenilla “r” moodissa tiedoston. Jos tiedostoa ei ole, niin paluuarvo
// on null.
// 
// Tiedostot tuhotaan unlink(“path”) komennolla.
//
// Compiling and run
// $ gcc -o exec_daemon consumer_daemon.c -Wall
// $ ./exec_daemon 
// Using another terminal to create test.txt for example $ touch test.txt
// $  more daemon_log.txt 
//
// $  ps -A | grep exec_daemon 
// $  killall exec_daemon 
// $  ps -A | grep exec_daemon 
//

#include <stdio.h>
#include <unistd.h>	// fork, getpid, setsid
#include <sys/types.h>	// pid_t
#include <sys/stat.h>	// umask
#include <stdlib.h>	// exit
#include <syslog.h>	// syslog
#include <sys/time.h>
#include <time.h>

// make a process a daemon
void daemonize(void) {
	pid_t pid;
	pid_t ppid;
	
	pid = fork();
	if (pid < 0) {
		perror("on fork while daemonizing");
		exit(EXIT_FAILURE);
	}
	if (pid > 0) {
	    // if we're the parent, we will terminate now
	    // so that the child will be moved under init
	    ppid = getppid();
	    printf("daemonize(): pid as %d; ppid as %d\n", pid, ppid);
	    exit(EXIT_SUCCESS);
	}
	
	// reset umask
	umask(0);

	// close the normal file descs.
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void print_time (FILE *fp) {
    struct timeval tv;
    struct tm* ptm;
    char time_string[40];
    long milliseconds;

    /* Obtain the time of day, and convert it to a tm struct. */
    gettimeofday (&tv, NULL);
    ptm = localtime (&tv.tv_sec);
    /* Format the date and time, down to a single second. */
    strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);
    /* Compute milliseconds from microseconds. */
    milliseconds = tv.tv_usec / 1000;
    /* Print the formatted time, in seconds, followed by a decimal point
       and the milliseconds. */
    fprintf (fp, "%s.%03ld ", time_string, milliseconds);
    fflush(fp); 
}

int main(int argc, char** argv) {
	
	pid_t pid, ppid;
	FILE *fp_log;
	FILE *fp_check;

	pid = getpid();
	ppid = getppid();

	printf("main(): pid as %d; ppid as %d\n", pid, ppid);

	daemonize();
	fp_log = fopen("daemon_log.txt","a");

	// refresh the IDs
	pid = getpid();
	ppid = getppid();
	fprintf(fp_log, "===== Logfile for consumer_daemon.c=====\n");
	fprintf(fp_log, "pid as %d; ppid as %d\n", pid, ppid);
	while(1) {
	    fp_check = fopen("test.txt","r");  
	    if (fp_check != NULL ) {
		unlink("test.txt"); 
		print_time(fp_log); 
		fprintf(fp_log, "test.txt is removed.\n");
		fflush(fp_log); 
	    }
	    sleep(1);
	}
	    
	fprintf(fp_log, "main: ******** ending here *********\n");
	fclose(fp_log);
	return EXIT_SUCCESS;
}

