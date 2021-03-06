

#include "timer.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	clock_t start, end;
	TIMER * _timer = (TIMER*) malloc (sizeof(TIMER));
    char *p, *t;
	int m, n;
	double *time;
	if(!strcmp("shared", argv[1])) {
		if(argc < 7) {
			printf("error in input parameter\nfive parameters needed when used with shared memory\n");
			return 1;
		}
		int pshmid, tshmid, eshmid;
	    key_t pkey = atoi(argv[2]); //segment name for the pattern
		m = atoi(argv[3]); //segment size for the pattern
	    key_t tkey = atoi(argv[4]); //segment name for the text
		n = atoi(argv[5]); //segment size for the text
	    key_t ekey = atoi(argv[7]); //segment name for the running time
   	 	/* Locate the pattern. */
    	if ((pshmid = shmget(pkey, m, 0666)) < 0) {
        	perror("shmget");
        	return 1;
    	}
    	/* Now we attach the segment to our data space. */
    	if ((p = shmat(pshmid, NULL, 0)) == (char *) -1) {
        	perror("shmat");
        	return 1;
    	}
   	 	/* Locate the text. */
    	if ((tshmid = shmget(tkey, n, 0666)) < 0) {
        	perror("shmget");
        	return 1;
    	}
    	/* Now we attach the segment to our data space. */
    	if ((t = shmat(tshmid, NULL, 0)) == (char *) -1) {
        	perror("shmat");
        	return 1;
    	}
   	 	/* Locate the running time variable */
    	if ((eshmid = shmget(ekey, 8, 0666)) < 0) {
        	perror("shmget");
        	return 1;
    	}
    	/* Now we attach the segment to our time variable space. */
    	if ((time = shmat(eshmid, NULL, 0)) == (double *) -1) {
        	perror("shmat");
        	return 1;
    	}
		
		
		timer_start(_timer);
		start = clock();
		int count = search(p,m,t,n);
		timer_stop(_timer);
		end = clock();
		(*time) = timer_elapsed(_timer)*1000;
		
		int rshmid, *result;
	    key_t rkey = atoi(argv[6]); //segment name for the occurrences
   	 	// Locate the int value. 
    	if ((rshmid = shmget(rkey, 4, 0666)) < 0) {
        	perror("shmget");
        	return 1;
    	}
    	// Now we attach the segment to our data space. 
    	if ((result = shmat(rshmid, NULL, 0)) == (int *) -1) {
        	perror("shmat");
        	return 1;
    	}
		*result = count;
    	return 0;				
	}
	else {
		if(argc < 5) {
			printf("error in input parameter\nfour parameters needed in standard mode\n");
			return 1;
		}
		p = argv[1];
		m = atoi(argv[2]);
		t = argv[3];
		n = atoi(argv[4]);
		int occ = search(p,m,t,n);
		printf("found %d occurrences\n",occ);
		return 0;
	}
}

