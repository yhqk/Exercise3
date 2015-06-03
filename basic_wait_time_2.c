/* Harjoitus 2 odottavan aika on pitkä 
   Luo prosessi, joka luo kolme lasta. Tämän jälkeen prosessi 
   menee nukkumaan satunnaiseksi ajaksi (5-10 sekuntia). 
   Unen jälkeen prosessi kutsuu wait(&status) 
   (status on int-tyyppiä) funktiota kolme kertaa ja tulostaa 
   joka kerran statuksen ja lapsiprosessin PIDin (waitin paluuarvo)

   Lapsiprosessit nukkuvat myös satunnaisen ajan (5-10 sekuntia).
   Unen jälkeen ne tulostavat, että ovat heränneet ja kuolevat pois.
   Tämän jälkeen ne palautuvat main funktiostaan joko
   EXIT_SUCCESS tai EXIT_FAILURE paluuarvolla.
   
   Lisää tarpeellinen määrä tulostuksia, että ymmärrät mitä tapahtuu.
*/

#include <sys/types.h>	// fork, getpid, getppid
#include <sys/wait.h>   // wait
#include <unistd.h>	// fork, getpid, getppid, sleep
#include <stdio.h>	// printf
#include <stdlib.h>	// rand()
#include <time.h>	// time()

// Display PID-data, modify global data, sleep and redisplay

void random_sleep(void){

    int wait_time; 
    pid_t myPid; 
    
    myPid = getpid();
    srand(time(0)+myPid);     
    /* wait random time between 5 to 10 */
    wait_time = (rand() % 6) + 5; 
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
	// fork returns either zero for child or childs pid
	printf("fork child No. %d\n", i+1);
	child = fork();	
	if (child == 0) {
	    random_sleep(); 
	    // beware of a fork bomb here, Google will know!
//	    exit(EXIT_SUCCESS); // 0
	    exit(EXIT_FAILURE); //256
	} else if (child == -1) {
	    perror("Problem while forking");
	    return 1;
	} else {
	  printf("main: fork child No. %d with PID %d\n", 
		 i+1, child);
	}
    }

    random_sleep();     
    for (i = 0; i < 3; i++) {
	returnPid = wait(&status); 
	printf("Child process ID as %d with status as %d\n", 
	       returnPid, status);
    }
    printf("main: ******** ending here *********\n");
    return 0;
}

