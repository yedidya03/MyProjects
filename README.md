# My Projects

Writer : Yedidya Freundlich\
Email : yedidya03@gmail.com

### Summary
In this reposetory I gathered a few projects I did over the years. 
You can find a short explenation about them in this ```README.md``` file. You can find the raw code in directories above.
For more information about the projects you may contact me using the email adress mentioned above.

### Motion Profiling Project
The Motion Profiling project plans a route for a tank drivetrain robots, and generates java-code for the robot to execute.
The project plans the route to ease accurate motion of the robot along the route with minimal deviation errors. 

In this project I developed a way of planning the route to meet the objectives of accurate motion and minimal deviation (based on work of other people in the field).
In my method, the robot itself plans the route, which enables it to change plans in mid course (the common methods have the entire route planned and loaded into the robot before starting).

This project was used in practice in several FIRST FRC international robotics compentitions (in fact, this code is still used by the robotics team to this day).\
\
I recommend viewing the following files:
* ~/MotionProfilingGenerator/Improved_MP/MP_Path.java
* ~/MotionProfilingGenerator/Improved_MP/MP_Radius.java
* ~/MotionProfilingGenerator/MP_ImprovedCommands/MP_AutoGenerator.java
* ~/MotionProfilingGenerator/MP_ImprovedCommands/MP_DrivePathFollower.java

For runnign the project you can open it as an ```eclips``` project and run it (the GUI is implemented with java Applet).

### TeamWork Project
The TeamWork project simulates the work that [TeamViewer](https://www.teamviewer.com/en/) does.\
It enables one computer to view the screen of another computer, and lets it control its mouse and keyboad. This way two people can work together on two different computers as if they are sitting together.

### Games
In the games directory you will find the games Snake and Tetris implemented in C++.
The games have a console-based user-interface; nevertheless, the game logic and flow are good and intuitive.
You can try to play the games simply by compiling them with MinGW or any other c++ compiler and run.\
\
For example:
```
$ g++ ./Snake.cpp -o Snake
$ ./Snake
```
### Nonogram Solver
This project is a simple and short python script that has a grid representing a N bonogram board and solves it. 
If there's more then one solution, the script will present all posible solutions.\
\
Running this script simply by typing in the command line:
```
$ python ./Nonogram_Solver.py
```

### Sudoku Solver
This project is a simple and short python script that has a grid representing a sudoku board and solves it. 
If there's more then one solution, the script will present all posible solutions.\
\
Running this script simply by typing in the command line:
```
$ python ./Sudoku_Solver.py
```
