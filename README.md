# My Projects

Writer : Yedidya Freundlich\
Email : yedidya03@gmail.com

### Summary
In this reposetory I gathered a few projects I did over the years. 
You can find a short explenation about them in this ```README.md``` file. You can find the raw code in directories above.
For more information about the projects you may contact me using the email adress mentioned above.

### TeamWork Project
The TeamWork project simulates the work that [TeamViewer](https://www.teamviewer.com/en/) does.\
It enables one computer to view the screen of another computer, and lets it control its mouse and keyboad. This way two people can work together on two different computers as if they are sitting together.

### Motion Profiling Project
The Motion Profiling project plans a route for a tank drivetrain robots, and generates java-code for the robot to execute.
The project plans the route to ease accurate motion of the robot along the route with minimal deviation errors. 

Part of this project was developing a way of calculating this path in a way that can be controlled and feedbacked with the robot's sensors.\
I did this project for the team in which I was participating in the First FRC competition.\
This project was a big success and my team still uses it til this day.\
\
I encourage you to view those files in the project:
* ~/MotionProfilingGenerator/Improved_MP/MP_Path.java
* ~/MotionProfilingGenerator/Improved_MP/MP_Radius.java
* ~/MotionProfilingGenerator/MP_ImprovedCommands/MP_AutoGenerator.java
* ~/MotionProfilingGenerator/MP_ImprovedCommands/MP_DrivePathFollower.java

For runnign the project you can open it as an ```eclips``` project and run it (GUI is implemented with java Apllet).

### Games
In the games directory you will find snake and tetis implemented in c++.
The games have a console based GUI but the game logic and flow are good and intuitive.
You can try to play the games simply by compiling them with MinGW or any other c++ compiler and run.\
\
For example:
```
$ g++ ./Snake.cpp -o Snake
$ ./Snake
```

### Sudoku Solver
This project is a simple and short python script that has a grid representing a sudoku board and solves it. 
If there's more then one solution, the script will present all posible solutions.\
\
Running this script simply by typing in the command line:
```
$ python ./Sudoku_Solver.py
```
