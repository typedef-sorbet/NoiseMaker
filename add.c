#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define 	OPEN 		0
#define 	CLOSE 		1
#define 	INVALID 	2

int openFlag;

int main(int argc, char *argv[])
{
	//usage: addSound [-open | -close] [filename].wav ...

	if(argc < 3)
	{
		fprintf(stderr, "Error: Invalid number of arguments to %s; please try again\n", argv[0]);
		exit(1);
	}
	
	int currentFileCount;
	FILE *countfd;
	if( strcmp(argv[1], "-open") == 0 )
	{
		countfd = fopen("/home/pi/Desktop/Programs/SoundsTest/sounds/Open/count.txt", "r");
		openFlag = OPEN;
	}
	else if( strcmp(argv[1], "-close") == 0 )
	{
		countfd = fopen("/home/pi/Desktop/Programs/SoundsTest/sounds/Close/count.txt", "r");
		openFlag = CLOSE;
	}
	else
	{
		fprintf(stderr, "Error: open/close flag not specified by first argument to %s; please try again\n", argv[0]);
		openFlag = INVALID;
		exit(1);
	}
	char countBuf[120];
	fgets(countBuf, 120, countfd);
	currentFileCount = atoi(countBuf);
	fclose(countfd);
	
	int soundfd;
	for(int i = 2; i < argc; i++)
	{
		if( (soundfd = open(argv[i], O_RDONLY)) < 0 )
		{
			fprintf(stderr, "Error: unable to open %s; this file will not be included.\n", argv[i]);
		}
		else
		{
			close(soundfd);
			char commandBuf[512];
			if( openFlag == OPEN )
			{
				sprintf(commandBuf, "cp %s /home/pi/Desktop/Programs/SoundsTest/sounds/Open/%d", argv[i], currentFileCount++);
				system(commandBuf);
				printf("%s copied.\n", argv[i]);
			}
			else if( openFlag == CLOSE )
			{
				sprintf(commandBuf, "cp %s /home/pi/Desktop/Programs/SoundsTest/sounds/Close/%d", argv[i], currentFileCount++);
				system(commandBuf);
				printf("%s copied.\n", argv[i]);
			}
			else
			{
				fprintf(stderr, "This line should never be run, but it's here.\n");
				exit(1);
			}
		}
	}
	
	countfd = (openFlag == OPEN)? fopen("/home/pi/Desktop/Programs/SoundsTest/sounds/Open/count.txt", "w") :
										   fopen("/home/pi/Desktop/Programs/SoundsTest/sounds/Close/count.txt", "w");
	fprintf(countfd, "%d\n", currentFileCount);
	fclose(countfd);
	printf("Count file updated to %d files.\n", currentFileCount);
}
