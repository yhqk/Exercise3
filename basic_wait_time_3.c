// Harjoitus 3 similiar as exercise 2 except main does not 
// Muokkaa harjoitus 2:sta siten, että pääohjelman wait tapahtuu
// luupissa eikä pääohjelma nuku ollenkaan. Tulosta jälleen ulos pid:t
// jokaisesta wait komennosta (eli paluuarvo).
//
// Lapset toimivat kuten aiemminkin ja tulostelevat, kun menevät unille
// ja kun heräilevät.
//
// Terminaalista “sudo kill -SIGSTOP <prosessinpid>” jollekin
// lapsiprosessille. Käytä toista terminaalia kuin mistä ajat ohjelmaa ja
// tarkista pid kutsumalla “ps –A”. Jatkaa voi vaihtamalla –SIGSTOP
// muotoon -SIGCONT 

#include <sys/types.h>	// fork, getpid, getppid
#include <sys/wait.h>   // wait
#include <unistd.h>	// fork, getpid, getppid, sleep
#include <stdio.h>	// printf
#include <stdlib.h>	// rand()
#include <time.h>	// time()

// compiling and run 
// $ gcc -o exec3 basic_wait_time_3.c -Wall
// $ ./exec3


void random_sleep(void){
    int wait_time; 
    pid_t myPid; 
    
    myPid = getpid();
    srand(time(0)+myPid);     
    /* wait random time between 10 to 20 in order to give enough time to 
     * type > kill -SIGSTOP childPid; > kill -SIGCONT childPid  
     */
    wait_time = (rand() % 11) + 10; 
    printf("random_sleep() my process ID %d. Waiting %d seconds\n", 
	   myPid, wait_time );
    sleep(wait_time);
    return; 
}

int main(int argc, char** argv)
{
    pid_t child, returnPid, parentPid;
    int i, status;

    parentPid = getpid();
    printf("main: parent Pid %d\n", parentPid);
    
    for (i = 0; i < 3; i++) {
	printf("fork child No. %d\n", i+1);
	child = fork();	
	if (child == 0) {
	    random_sleep(); 
	    // beware of a fork bomb here, Google will know!
	    exit(EXIT_SUCCESS); 
	    //	    exit(EXIT_FAILURE);
	} else if (child == -1) {
	    perror("Problem while forking");
	    return 1;
	} else {
	  printf("main: fork child No. %d with PID %d\n", 
		 i+1, child);
	}
    }

    while(1){
	returnPid = wait(&status); 
	if (returnPid < 0 ) break; 
	printf("Child process ID as %d with status as %d\n", 
	       returnPid, status);
    }
    printf("main: ******** ending here *********\n");

    return 0;
}

