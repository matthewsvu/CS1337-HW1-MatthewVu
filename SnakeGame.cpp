/*
Program name - ModifySnakeGame.cpp
original program from web
    1/22/20 version 5

Name - Matthew Vu
Purpose - To read a piece of code written by someone else and clean it up
Class # and Section: CS 1337.003
Change log 1 1/14/20:
1. Changed variable name x and y to headX and headY (describes them better and fits with the variable naming scheme)
2. set HeadX in logic method = to 1 instead of 0 when making a collision with the rightwall;
    which used to cause the snake to disappear when it reached the rightmost wall
3. Changed headX < 0 to headX <= 0; fixed the bug of when the snake moves vertically on the leftmost wall causing it to disappear.
4. Added a get X and Y function to return the location of the snake's head in real time
5. added function that draws the top and bottom border of the board.
6. Separation of logic in the Logic function - tailcollision, wall collision, and change direction
Change log 2 1/15/20:
1. Changed variable names in logic to be more descriptive.
2. added pause functionality with pressAKeyToStart function
3. further segmented logic code
4. refactored variable names throughout code in best practices format
5. Check for array overflow in drawTailOrSpace function.
Change log 3 1/18/20:
1. Added drawFruit function
2. Added code for later use in drawing multiple fruits
3. Added randomizeFruit location function
Change log 3 1/21/20:
1. Completed commenting and modularization of the Logic function
2. Made coreGameLogic function to separate it from the other functions
Change log 4 1/22/20:
1. Added debugging functions
2. matrix board function added
3. instructions for game at startup added.
4. startGame function added
5. deathByWall function added, need to implement as option later
6  added more comments to various function for clarity.
7. Finished drawing multiple fruits functionality.
8. added hardmode and death by wall
9. fixed bug where fruit gets stuck inside a wall
10. added restart game function and initialized it at the top of the program.

Notes:
    1. 1/14/20 - finish draw fruit function and rename Logic variable names
    2. 1/15/20 - comments on snake logic because I don't understand them
    3. 1/22/20 - make start game function and a restart function together
Comments:
How would I add a way to have multiple fruits on one board that are fully interactable with the snake head?
I think it would be similar to the snake growth logic for some reason.
A lot of this debugging is tedious.

*/

#include <iostream>
#include <conio.h>
#include <windows.h> // sleep function
#include <ctime> // time and srand function

using namespace std;

void restartGame(); // initiate restartGame function here

bool gameOver, hardMode = false;

const int SCREEN_WIDTH = 20;
const int SCREEN_HEIGHT = 20;
const int MAX_TAIL_LENGTH = 100;
const int NUM_FRUIT = 2; // to be used when multiple fruits are on board
const int SOLID_BLOCK_ASCII = 254;
const int SLEEP_MAX_AMT = 100;
const int SLEEP_INCREMENT_AMT = 5;

// head location, fruit location, and score
int snakeX, snakeY, fruitX, fruitY, fruit2ndX, fruit2ndY, score;
int tailX[MAX_TAIL_LENGTH], tailY[MAX_TAIL_LENGTH];
int lengthTail, sleepAmt;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir; // creates a object of eDirection
// debug code, show location of programming
void showFlow(string location)
{
    cout << "---> at location: " << location << endl;
}
// initiates the game variables
void Setup()
{
    //showFlow("Setup")
    gameOver = false;
    dir = STOP;
    sleepAmt = 10;
    // places the snake in the middle of the board
    snakeX = SCREEN_WIDTH / 2;
    snakeY = SCREEN_HEIGHT / 2;
    lengthTail = 0;
    // randomly places a fruit on the board
    srand(time(0)); // changes the seed for random num generator
    fruitX = rand() % SCREEN_WIDTH;
    fruitY = rand() % SCREEN_HEIGHT;
    fruit2ndX = rand() % SCREEN_WIDTH;
    fruit2ndY = rand() % SCREEN_HEIGHT;
    score = 0;
} // end set up function
void drawTopAndBottomBorder()
{
    //showFlow("drawTopAndBottomBorder");
    for (int i = 0; i < SCREEN_WIDTH+1; i++)
    {
        cout << "#";
    }
    cout << endl;
} // end draw top and bottom border function
// Will draw tail based on length given else draws an empty space to signify nothing there
void drawTailOrSpace(int boardHeight, int boardWidth)
{
    //showFlow("drawTailOrSpace");
    bool printTail = false; // will be set to true if a tail exists at the x and y coordinates
    if(lengthTail > MAX_TAIL_LENGTH) // check for array overflow
    {
        cout << "ERROR! Array overflow\n";
        gameOver = true;
    }
    for (int currLength = 0; currLength < lengthTail; currLength++)
    {
        if (tailX[currLength] == boardWidth && tailY[currLength] == boardHeight) // if the current array value is at the xy location on the board print it out
        {
            cout << "o";
            printTail = true; // means a tail is being printed at those x y coordinates
        }
    }
    if (!printTail) // draws a space instead if the tail
    {
        cout << " ";
    }
} // end draw tail and space function
// places fruit anywhere within the screen width and height, reference the real fruit xy vals
void randomizeFruitLocation(int &instanceFruitY, int &instanceFruitX)
{
    //showFlow("randomizeFruitLocation");
    // will change location of either 1st or 2nd fruit depending on what is passed in
    do
    {
        instanceFruitX = rand() % SCREEN_WIDTH;
        instanceFruitY = rand() % SCREEN_HEIGHT;
    } // make sure that the fruit does not get placed within the leftmost edge
    while(fruit2ndX == fruitX && fruit2ndY == fruitY || (fruitX == 0 || fruit2ndX == 0)
           || (fruitY == 0 || fruit2ndY == 0))  // make sures the fruit does not get put in the same place
        ;
} // end randomizeFruit function
void drawFruit()
{
    cout << "F";
} // end draw fruit function
// Part of Draw function, makes it cleaner to read.
void drawBoard(int boardHeight, int boardWidth)
{
    //showFlow("drawBoard");
    if (boardWidth == 0) // draws border when at the left edge
    {
        cout << "#";
    }
    else if (boardHeight == snakeY && boardWidth == snakeX) // if the snake's head is at a location draw it there
    {
        cout << (char)(SOLID_BLOCK_ASCII); // draws the ascii character - concatenate int to char
    }
    else if (boardHeight == fruitY && boardWidth == fruitX || boardHeight == fruit2ndY && boardWidth == fruit2ndX ) // if the board height is the same as the locations of the fruits
    {
        drawFruit();
    }
    else
    {
        drawTailOrSpace(boardHeight, boardWidth);
    }
    if(boardWidth == SCREEN_WIDTH - 1) // draws border when at right edge
    {
        cout << "#";
    }
} // end drawBoard function
void showUserScore()
{
    cout << "Score:" << score << endl;
} // end user score
// draw a matrix and then the board itself
void drawMatrixBoard()
{
    //showFlow("drawMatrixBoard");
    for (int currHeight = 0; currHeight < SCREEN_HEIGHT; currHeight++)
    {
        for (int currWidth = 0; currWidth < SCREEN_WIDTH; currWidth++)
        {
            drawBoard(currHeight, currWidth); // pass in current height and width to drawBoard
        }
        cout << endl;
    }
} // end drawMatrixBoard function
// main draw function
void Draw()
{
    //showFlow("Draw");
    system("cls"); //system("clear");
    // top border
    drawTopAndBottomBorder();
    // setups the matrix for the board and then draws the board itself along with snake and fruit location
    drawMatrixBoard();
    // bottom border
    drawTopAndBottomBorder();
    showUserScore();
} // end Draw function
void pauseGame()
{
    //showFlow("pauseGame");
    int key;
    cout << "\nPress a key to continue...";
    key = getch(); // retrieves input char from keyboard to pause
    if (key == 0 || key == 224)
    {
        getch(); // starts game
    }
} // end pauseGame function
// keyboard input from user
void Input()
{
    //showFlow("Input");
    if (_kbhit())
    {
        char userChar = tolower(_getch()); // automatically turns user inputs into lower case letters
        switch (userChar)
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case ';':
            if(sleepAmt <= SLEEP_MAX_AMT)
            {
                sleepAmt += SLEEP_INCREMENT_AMT;
            }
            break;
        case 'l':
            if(sleepAmt > 0)
            {
                sleepAmt -= SLEEP_INCREMENT_AMT;
            }
            break;
        case 'x':
        case 'q':
            gameOver = true;
            break;
        case 'p': // will stop game once p is pressed
            pauseGame();
            break;
        }
    }
} // end Input function
// Report location of snake's head and fruits in real time
void getXandYLocation()
{
    cout << "x=" << snakeX << "    y=" << snakeY << endl;
    cout << "1st fruit x=" << fruitX << "     y=" << fruitY << endl;
    cout << "2nd fruit x=" << fruit2ndX << "     y=" << fruit2ndY << endl;

} // end getXYfunc
// method for when snake hits a wall and comes out the other side
void wallCollisionLogic()
{
    // showFlow("wallCollisionLogic");
    if (snakeX >= SCREEN_WIDTH) // when it hits the rightmost wall
    {
        snakeX = 1; // location just to the right of the left most wall
    }
    else if (snakeX <= 0) // when it hits the leftmost wall
    {
        snakeX = SCREEN_WIDTH - 1;
    }
    if (snakeY >= SCREEN_HEIGHT) // when it is at the upper most edge
    {
        snakeY = 0; // set snake head back to the bottom
    }
    else if (snakeY < 0)
    {
        snakeY = SCREEN_HEIGHT - 1;
    }
} // end wallCollisionLogic
// when snake head hits tail
void tailCollisionLogic()
{
    //showFlow("tailCollisionLogic");
    for (int currLocation = 0; currLocation < lengthTail; currLocation++)
    {
        if (tailX[currLocation] == snakeX && tailY[currLocation] == snakeY) // xy values of tail are the same as the head xy values
        {
            gameOver = true;
        }
    }
} // end tailCollisionLogic
// change direction of the snake's head
void changeDirection()
{
    //showFlow("changeDirection");
    switch (dir)
    {
    case LEFT:
        snakeX--;
        break;
    case RIGHT:
        snakeX++;
        break;
    case UP:
        snakeY--;
        break;
    case DOWN:
        snakeY++;
        break;
    default:
        break;
    }
} // end changeDirection function
// This method will increase the snake's tail length, score, and randomize apple location
void increaseSnakeLengthAndScore()
{
    // showFlow("increaseSnakeLengthAndScore");
    if (snakeX == fruitX && snakeY == fruitY || snakeX == fruit2ndX && snakeY == fruit2ndY) // IF the snake eats the 1st or 2nd fruit
    {
        score += 10; // score increases
        if(snakeX == fruitX && snakeY == fruitY) // when it eats the first fruit, randomize a new location for the fruit
        {
            randomizeFruitLocation(fruitY, fruitX);
        }
        else if(snakeX == fruit2ndX && snakeY == fruit2ndY) // 2nd fruit randomizer
        {
            randomizeFruitLocation(fruit2ndY, fruit2ndX);
        }
        lengthTail++; // increment the length of the tail after randomizing the fruit
    }
} // end increaseSnakeLengthAndScore function
// Calculates how a snake is drawn
void coreGrowthLogic()
{
    // showFlow("coreGrowthLogic");
    // sets the first tail = to the first val of array
    int prevTailX = tailX[0];
    int prevTailY = tailY[0];
    // temp value for tail
    int tempTailX, tempTailY;
    // the snake head is considered the first tail
    tailX[0] = snakeX;
    tailY[0] = snakeY;
    // starting from the 2nd value of the array set the
    for (int i = 1; i < lengthTail; i++) //
    {
        // set temp value equal to current tail position
        tempTailX = tailX[i];
        tempTailY = tailY[i];
        // replace the current tail with position of the new tail's X and Y
        tailX[i] = prevTailX;
        tailY[i] = prevTailY;
        // previous tail xy is now the next tail value xy
        prevTailX = tempTailX;
        prevTailY = tempTailY;
    }
} // end coreGrowthLogic function
// call this function if you want to die by a wall.
void deathByWall()
{
    if(snakeX >= SCREEN_WIDTH || snakeX <= 0 || snakeY >= SCREEN_HEIGHT || snakeY < 0)
    {
        gameOver = true;
    }
} // end deathByWall
// Where all game logic occurs
void Logic()
{
    // showflow("Logic");
    coreGrowthLogic();
    changeDirection();
    // Report location of snake's head
    getXandYLocation();
    if(hardMode == false)   // when hard mode is not on, go by the normal games rules, else do hard mode rules
    {
        wallCollisionLogic();
    }
    else
    {
        deathByWall();
    }
    tailCollisionLogic();
    // This section increases the score when the snake touches the fruit, randomizes the location of a another fruit,
    // and increases the length of the tail
    increaseSnakeLengthAndScore();
} // end logic function
//
void startGame()
{
    //showFlow("StartGame");
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(sleepAmt);
    }
    cout << "Game over!\n";
    cout << "Total fruits eaten: " << lengthTail << endl << endl;
    restartGame();

} // end  startGame function
void restartGame()
{
    char restartKey; // when user is prompted for y, h, or n
    cout << "Do you want to restart? Type 'y' for YES, 'h' for restarting in HARDMODE,\n 'n' or any other key for NO. Press ENTER to submit the input" << endl;
    cin >> restartKey;
    switch(restartKey)
    {
    case 'y':
        hardMode = false; // resets hardmode in case it was already in hardmode
        startGame();
        break;
    case 'h':
        hardMode = true;
        startGame();
        break;
    default:
        exit(true);
        break;
    }
} // end restartGame func
void displayInstructions()
{
    // showFlow("displayInstruction");
    cout << "Controls:\n"
         << "---------\n";
    cout << "W - MOVE UP\n"
         << "A - MOVE LEFT\n"
         << "S - MOVE DOWN\n"
         << "D - MOVE RIGHT\n"
         <<"---------\n"
         << "X or Q - QUIT GAME\n"
         << "P - PAUSE GAME\n"
         << "L - SPEED UP GAME\n"
         << "; - SLOW DOWN GAME\n\n"
         << "Description:\n"
         << "This is a game called Snake, when you eat fruits that look like --> 'F' your snake grows longer.\n"
         << "Try to eat as much as you can and grow as big as possible without touching your own tail!\n"
         << "Remember dying is fun!\n";
}   // end displayInstructions function
int main()
{
    displayInstructions();
    pauseGame();
    startGame();

    return 0;
}
// end main
