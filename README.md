# Finite-Volumes-Immersed-Boundary-Methods

This project contains exactly the code I wrote for my master degree dissertation which dates back to April 2012.
The code you can see here is so awful that I don't even list why.
If you are on this page and you are a recruiter please leave as this is not representative of my current programming skills
in any way. If you are one my colleagues please leave this page because it is embarassing. If you are the interesting and fascinating girl that inspired me to publish this please go on.

## How to run a simulation
### Compile the code
You told me you have Linux and by chance this could run ONLY on Linux (just the tip of the iceberg about why this code is awful).
Executes the commands ($ represents the command line)  
$ cd example  
$ make  

You'll be prompted a huge amount of incomprehensible warnings. Just discard them.  
### Run the simulation  
Create the directory example/data/unknowns (the program is so stupid that does not check if the directory exist and will not save any data!!!).
Execute the process  
$ ./example  
The program will display some info and for no reason will wait for an input. Just type a random string and press Enter.
Wait a looong time. It has to generate 500 frames so go for a walk or do the laundry in the meanwhile.
### Generate fancy pictures and a video
You need matlab for this, so open it and set the working directory to example/m_files. You can have a look at my scripts see how they are crappy.
Create the directory example/data/z_frames and run  
$ z_frames(499)  
Wait another century until all the images have been generated. Now move to the directory data/z_frames and run  
$ avconv -r 25 -start_number 10000 -i %d.png -b:v 1000k out.mp4  
Open the video out.mp4 with your favourite app (I use VLC). You'll see the water moving around the swimming pool.



