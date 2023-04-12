Comp 3016 – 3D Tetris - 10612662
==========
For My Project I decided to develop Tetris using 3D shapes in Open GL. In this documentation Ill describe how to run the application and how the code works before moving on to reflection on how the end product turned out compared to my initial expectation and what I learnt from it. 

### Software Used

*	OS: Windows 10
*	GUI: Visual Studio 2019
*	Libraries Used: Freeglut,GLEW,GLFW,GLM


Links
==========

Video - [Youtube Video](https://www.youtube.com/watch?v=x9x4dojHRfg)

Inital Project Proposal - [Link](https://github.com/Plymouth-Comp/2020-comp3016-cw2-TKRHinton/blob/main/Screenshots/COMP3016%20Project%20Proposal.pdf)


How To Use
==========

### .EXE

The Executable of the program is located in the “Executable” Folder. In the folder the EXE is be called “Tetris3D.exe” which can be run by double clicking/right click open, which in turn will start the program. 

![screenshot 1](/Screenshots/exe.png?raw=true)
![screenshot 2](/Screenshots/exe2.png?raw=true)

### Visual Studio

When trying to run the program using visual studio you need to locate the “Coursework2.sln” file which is in the “Source” Folder. 
When you open this file, you can then use the debugger tool by clicking or using F5.

![screenshot 3](/Screenshots/vs.png?raw=true)


How To Play
==========
The Games objective to accumulate as many points in the game as possible which can be done by creating a line. This is done by the user controlling 5 different shapes that will fall. The user can the currently falling shape by rotating or moving the shape on the 15X15 board that the shape is on. When a line is completed the line will disappear and the speed of the falling will increase making the game harder and harder till the user as fill up the board to the top. 


### Menu

Running the “Tetris3D.exe” will bring up a window showing the game menu. This menu contains the background and two options (two cubes) which are “Play” and Exit. 

This is area the user can interact with the game by using the mouse (Left Click). By choosing the “Exit” Option the program will close the window and stop the program. This function can also be perform by pressing the “ESC” key.

By selecting the “Play” Option the program will then construct (change game state) the play area and start the game. 

![screenshot 4](/Screenshots/menu.png?raw=true)

### Game

When the “Play” option is selected the user will be presented with the game itself. This is a 15X15 board which is brightly coloured showing the title of the program with a random Tetris Piece at the top of the board. As the program is running this piece will slowly fall down to the bottom with no user interaction but while its falling, the user can use the Keyboard to control its position and rotation.

Controls:

*	A Key – Move Piece to the left
*	D Key – Move Piece to the Right
*	S Key – Move Piece Down
*	Right Key – Rotate Piece 90 Degrees Right
* Left Key – Rotate Piece 90 Degrees Left

The User can interact with this piece till it reaches the bottom of the play area. When this occurs, the piece will stop being reacting to user interaction and another piece will spawn at the top of the play area which the user gets control of instead. There are 5 different pieces the user can interact with and its random which piece the user will get when a new one spawns. 

As the games goes on and the pieces fall, the pieces will stop falling if it reaches the bottom or it touches another piece. When this event occurs, the program will check each row to see if any are filled, this is the case, before spawning a new block the program will remove the line and move the blocks above down (shown in the two pictures).  As you can also see, this event also make the score go up, this score will go up with each line filled. 

![screenshot 4](/Screenshots/line.png?raw=true)
![screenshot 4](/Screenshots/line2.png?raw=true)

The way the scoring works is the more lines get fill with one shape the more points per a row you get  which incentives the user to strategies and engage more with the game. The scoring: 1 line – 20 points | 2 lines – 50 points | 3 lines – 100 points | 4 lines – 250 Points. 

The game is designed to get harder as the game progresses, this is done by increasing the speed at which the current block will fall. This increase of speed is added each time the user gets a row filled/removed. This will eventually make the game too hard to continue and make the blocks will the play space and when the blocks reach the top the game will end. 

### End

Ending the game will change the game state and the user will be shown the end game menu, this is similar to the start menu, but it has some changes. 

One big difference is that it includes the users end score. This allows the user to know what they got and try for high scores. 

By selecting the  “Play Again” options using their mouse they can reset the game and start it over again but if they don’t want to play it, they can select the “Exit” Button to quit the program. 

![screenshot 4](/Screenshots/end.png?raw=true)


Code
==========

I developed the program in Open GL Using the programming language C++. In the project the code functions and classes a created to make the program are these files:
*	Shapes.h – Contains the Colours/Vertices/Indices/Texture Cords/Normals of all the shapes used
*	Tetris.h – Contains the Class Tetris which hold information on the piece the user can control
*	Tetris.ccp – Contains the initiation of the Tetris Class
*	Project.h – Contains all the global variables used for the program
*	Project.ccp – Contains all the functions that create the program

### Start 
The program will start at main and first initiate all the required functions such as glfwInit() before moving to creating the VAO’s. Each VAO is created in its own function which is called before the window is created. In those functions it takes all the needed information from the Shapes.h file and binds to the buffer before then creating the setting up all the lights and view matrix which will be used in the shaders to calculate the lighting of the object.

```c++
	//CREATES OBJECT
	gameTextures();
	init();
	initBackground();
	initScore();
	initBackgroundMenuStart();
```

When the VAO’s are created and put in the VAO array the Main will then go in a while loop. In this while loop the program will first check for an input by the user. If true, the program will either change game state (if Game == Game_Menu) or change the rotation/position of the current Tetris object (if Game == Game_Menu). After this the program will then call the display function which displays the objects to the screen, there are 3 display functions but only one will be called and which one depends on the game state (Game_Menu, Game_Active, Game_ End). When the program starts the initial game, state is Game_Menu which means the displayMenu() function will be called.

```c++
	while (!glfwWindowShouldClose(window))
	{
		//CHECKS FOR INPUT
		inputUpdate(window, position, rotation);

		//DISPLAYS GAME
		if (Game == Game_Menu) {
			displayMenu(timer);
		}
		else if (Game == Game_Active) {
			display(timer, position, rotation);
			timer += multiplier; //WILL INCREASE SPEED AT MUTIPLAYER INCREASES
		}
		else if (Game == Game_End) {
			timer = 0.0f;
			time = 1.f;
			multiplier = 0.01f;
			displayEnd();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();	
	}
```

### Menu

When this function is called it would first get the VAO and then create a view matrix. This object will be the background in this case. Then a texture would be applied, and the object will be drawn onto the window. This is repeated for the two options in the menu when the displaybuttons function is called just after creating the end product of the Menu. The only interaction is by clicking the option which will either quit the program or when “Play” is clicked the game state will change is “Game_Active”. This will change the Display function bring called to Display() which is where the main game is displayed. 

### Game 
In the Display() function while it resets the window check the time function (makes block fall automatically) it calls four functions which sets up the game space. The first one (background) sets up the background of the game, after this the score function is called which draws the score in the bottom corner of the window.

```c++
//MOVES BLOCK DOWN AUTOMATICALLY
	if (delta > time)
	{
		position.y -= 1.f;
		time = time + 1.f;
		Sleep(20);
	}

	//CREATES TETRIS GAME SPACE
	background(rotation);
	score();
	currentPiece(position, rotation);
	tetrisPieces();
```

The Next function (CurrentPiece()) is the biggest function of the program and its job is to get the current rotation and position and move the current piece in the Tetris array, this is a 15x15 array which is used to store where all the pieces on the game board is located. This function will check the shape/position and rotation, then it will check if the new position that it is going is empty or not. If not, the piece will move to that position but if not, the reset piece function is called

```c++
	x = piece.getPosition().x + 7;
		y = piece.getPosition().y + 7;

		if (piece.getRotation().z == 0)
		{
			//CHECKS IF NEW POSITION IS EMPTY OR NOT 
			if ((y < 1) || (tetrisBlocks[y - 1][x] == 1) || (tetrisBlocks[y - 1][x - 1] == 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y + 1][x] == 1))
			{
				//RESETS CURRENT PIECE 
				resetPiece();
			}
			else {
				//PUTS CURRENT PIECE INTO TETRIS ARRAY
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y + 1][x] = 1;
				tetrisBlocks[y - 1][x] = 1;
				tetrisBlocks[y - 1][x - 1] = 1;
			}
		}
```

This function will leave the piece (oldposition()) and sets up a new piece. While also checking if any of the rows are filled (checkrow()). If true then the rows will be removed from the array and the blocks above are moved down (removerow()).

Tetrispices() takes the array and loops through it all printing the pieces that are stored in the array (as 1’s). This display function will keep being called and the same thing will repeat until the game state changes (when the user losses the game) and then the next display function will then start being called.


### End
When the game state changes to Game_End the main will then start calling the Display End Function. This performs in a similar was to the displayMenu() function but instead called the score() function instead of drawing a background. This creates the two options cubes to select while also having the score. Also, In this game state when the user selects the play again option the reset function is called which resets all the game data and reset the position and rotation.

```c++
void displayEnd()
{
	//RESET WINDOW TO BLACK
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);

	//BIND TEXTURES ON TEXTURE UNITS
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//DISPLAYS BUTTONS AND END SCORE
	displayButtons();
	score();

}

```


Idea
==========
Starting the project, I used the Initial Project that you got when signing up to the classroom coursework 2 which contained a program that spawned a single cube and with that base developed the Tetris program. 

The idea of the project came from the desire to enhance a 2D game that I really liked, this game was Tetris as Tetris was one of the games I loved and still do so I decided to use this as a bases. When researching for this I didn’t find many Open GL versions of Tetris and the ones that was there were old versions of Open GL (Fixed Function Pipelines) so I knew that my idea was unique enough to try and create.

The Goal of the project was to make the Tetris game but in 3D. This would include the same goal of the game, but it will be performed in a 3D space. This Would add to the Tetris Game and make it unique to the original. 

Reflection 
==========
Looking at the end product there was a few changes from the initial goal that I set out. This was the game space where I initial set out to create a 3D play space for the Tetris game. When developing the product, I realised that in order to allow the user to see the whole play area they need to move the camera around and the way I generated the blocked need to change. This was a problem on the user side as moving a camera/rotation and position is a lot for a user and having this many options may hinder the game experience. One other problem was that this greatly expanded the scope I initial had for the development of the project. Because of this I decided to focus on make the game with a 2D game space and develop that into a playable state.  This version still used 3D shapes and I believe that by reducing the scope I created a more polished version of the game then if I decided to stick with the original plan.

Looking at the project now there are many things that I learnt that I would use to develop the project differently if I had to. One would be design the project more. This mean planning the design and structure of the code more before going into the implementation phase of the project. This would have allowed me to identify the scope of the project quicker and would allow me to make the code more readable and efficient as a result. Another thing I learnt was the importance of a separate place for work, due to the pandemic I had to work from home which made uni work a lot harder to engage and learn from, from this I know that if I had to do this project again, I would try and find a space and just designate that space to work to try and separate work from home to make work easier to engage in. 

Overall, I believe that I have meet most of my initial aims for the project and have made an end product that performs the tasks that I designed it to do and not have any crashes and not game breaking bugs while also creating a menu that make the product more user friendly. 

