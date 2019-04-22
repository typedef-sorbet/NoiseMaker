#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


#define 	CLOSED 	0
#define 	OPEN 	1

#define AM_HOURS	9
#define PM_HOURS	12+9

void cleanupHandler(int code);
void signalHandler(int code);

int numTrips = 0;
int state = CLOSED;
int tripPID = -1;
int soundPID = -1;
int silentPID = -1;

int main()
{
		
	char thisPID[7];
	sprintf(thisPID, "%d", getpid());
	
	if( (tripPID = fork()) )	//Parent
	{
		
	}
	else				//Child
	{
		//give pid of this process to the tripcounter child process
		execl("./tripCounter", "tripCounter", thisPID, (char *) NULL);
	}
	
	//reroute SIGFPE handler to custom handler to count trips and start sounds
	signal(SIGFPE, signalHandler);
	signal(SIGINT, cleanupHandler);
	
	
	printf("Running...\n");

	if( (silentPID = fork()) )
	{
		
	}
	else
	{
		execl("usr/bin/aplay", "aplay", "-r", "48000", "-f", "S16_LE", "/dev/zero", (char *)NULL);
	}
	//only do stuff when we catch a SIGFPE
	while(1);
}

void signalHandler(int code)
{
	//is it bedtime? Then make sure this thing stays quiet.
	time_t rawtime;
	struct tm *current;

	time(&rawtime);
	current = localtime(&rawtime);

	//if it's between 9 PM and 9 AM
	if(current->tm_hour < AM_HOURS || current->tm_hour > PM_HOURS)
	{
		return;
	}


	//check if a sound child is still running; if there is, kill it
	if(soundPID != -1 && waitpid(soundPID, NULL, WNOHANG) == 0)
	{
		printf("from main: \tKILLING PREV CVLC\n");
		kill(soundPID, SIGTERM);
	}

	numTrips++;
	state = (numTrips % 2 == 0)? CLOSED : OPEN;
	
	int chdirCheck;
	
	srand(time(0));
	
	//usleep(150000);
	
	printf("from main: \tDOOR OPENED\n");
	
	//open door sound
	if( (soundPID = fork()) )
	{
		
	}
	else
	{
		chdirCheck = chdir("/home/pi/Desktop/Programs/SoundsTest/sounds/Open");
		if(chdirCheck != 0)
		{
			fprintf(stderr, "Error: Unable to change directory to the Open directory, aborting...\n");
			exit(1);
		}
			
		//get count from count.txt; select a rand int between 1 and count-1; play that waveform
		FILE *countFile= fopen("count.txt" ,"r");
		if(countFile == NULL)
		{
			fprintf(stderr, "Error: Unable to find count.txt in sounds/Open, aborting...\n");
			exit(1);
		}
		
		char countBuf[120];
		
		fgets(countBuf, 120, countFile);
		
		fclose(countFile);
		
		int soundCount = atoi(countBuf);
		
		int selectedSound = rand() % soundCount;
		
		printf("from main: \t%d is the selected sound\n", selectedSound);
		
		char fileName[120];
		
		sprintf(fileName, "%d", selectedSound);
		
		execl("/usr/bin/cvlc", "cvlc", "--play-and-exit", fileName, (char *) NULL);
	}
}

void cleanupHandler(int code)
{
	if(soundPID != -1 && waitpid(soundPID, NULL, WNOHANG) == 0)
	{
		printf("from main: \tKILLING PREV CVLC (CLEANUP)\n");
		kill(soundPID, SIGTERM);
	}
	if(tripPID != -1 && waitpid(tripPID, NULL, WNOHANG) == 0)
	{
		printf("from main: \tKILLING TRIPCOUNTER (CLEANUP)\n");
		kill(tripPID, SIGTERM);
	}
	//check if a sound child is still running; if there is, kill it
	if(silentPID != -1 && waitpid(silentPID, NULL, WNOHANG) == 0)
	{
		printf("from main: \tKILLING SILENCE (CLEANUP)\n");
		kill(silentPID, SIGTERM);
	}
	
	exit(0);
}
