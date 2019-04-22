main: mainThread.c tripCounter
	gcc -o main mainThread.c -Wall -g
add: add.c
	gcc -o add add.c -Wall -g
tripCounter: tripCounter.c
	gcc -o tripCounter tripCounter.c -lwiringPi -Wall -g
clean:
	rm main tripCounter add
