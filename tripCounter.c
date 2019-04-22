#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "Error: No parent PID giveen to tripCounter\n");
		exit(1);
	}
	
	wiringPiSetup();
	
	pinMode(6, INPUT);
	
	int parentPID = atoi(argv[1]);
	int currentState = digitalRead(1);
	int prevState;
	struct timeval prevTrip, currentTrip;
	gettimeofday(&currentTrip, NULL);
	
	while(1)
	{
		usleep(1000);
		prevState = currentState;
		currentState = digitalRead(6);
		
		//detect posedge (which is technically negedge but whatever) with debouncing
		if(currentState == LOW)
		{
			//set up debouncer
			memcpy(&prevTrip, &currentTrip, sizeof(struct timeval));
			gettimeofday(&currentTrip, NULL);
		
			if(prevState == HIGH && currentTrip.tv_usec - prevTrip.tv_usec < 500000)
			{
				printf("from tripCounter: \tPOSEDGE DETECTED\n");
				kill(parentPID, SIGFPE);
			}
		}
	}
}
