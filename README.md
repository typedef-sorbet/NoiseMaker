Description
===========
Have you ever wanted to annoy your roommates with random sounds playing throughout the day? You've come to the right place.
NoiseMaker is an application intended for Raspberry Pi that will detect a door opening or closing via a sensor attached to the Pi, and will play a number of user-specified sounds for either door opening or door closing events.

Dependencies
============
* cvlc
* aplay
* wiringpi

Building and Running
====================
To build the main components of Noisemaker, simply run

`make`

To build the additional module to add sounds that the program can reference, run

`make add`

To run, do

`./main`

Notes
=====
This repository is intended for showcasing purposes only. Many of the inner workings of the application (file paths, etc.) are hard coded, and were never generalized for public use. 
Feel free to clone and make modifications if you wish, but god help ye who decides to take on that burden.
