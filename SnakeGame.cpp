/*
Program name - ModifySnakeGame.cpp
original program from web
    1/15/20 version 2

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
Notes:
    1. 1/14/20 - finish draw fruit function and rename Logic variable names
    2. 1/15/20 - comments on snake logic because I don't understand them
Comments:
How would I add a way to have multiple fruits on one board that are fully interactable with the snake head?
I think it would be similar to the snake growth logic for some reason.

*/

#include <iostream>
#include <conio.h>
#include <windows.h> // sleep function
#include <ctime> // time and srand function

using namespace std;

bool gameOver;

const int SCREEN_WIDTH = 20;
const int SCREEN_HEIGHT = 20;
const int MAX_TAIL_LENGTH = 100;
const int NUM_FRUIT = 2; // to be used when multiple fruits are on board
const int SOLID_BLOCK_ASCII = 254;

// head location, fruit location, and score
int snakeX, snakeY, fruitX, fruitY, score;
int tailX[MAX_TAIL_LENGTH], tailY[MAX_TAIL_LENGTH];
int lengthTail, sleepAmt;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir; // creates a object of eDirection
// debug code, show location of programming
void showFlow(string location) {
    cout << "---> at location: " << location << endl;
}

void Setup()
{
    //showFlow("Setup")
    gameOver = false;
    dir = STOP;
    sleepAmt = 10;
    // places the snake in the middle of the board
    snakeX = SCREEN_WIDTH / 2;
    snakeY = SCREEN_HEIGHT / 2;
    // randomly places a fruit on the board
    // srand(time(0)); // changes the seed for random num generator
    fruitX = rand() % SCREEN_WIDTH;
    fruitY = rand() % SCREEN_HEIGHT;
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
    bool printTail = false; // will be set to true if a tail exists at the x and y coordinates
    if(lengthTail > MAX_TAIL_LENGTH) // check for array overflow
    {
        cout << "ERROR! Array overflow\n";
        gameOver = true;
    }
    for (int currLength = 0; currLength < lengthTail; currLength++)
    {
        if (tailX[currLength] == boardWidth && tailY[currLength] == boardHeight)
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
// places fruit anywhere within the screen width and height
void randomizeFruitLocation()
{
    fruitX = rand() % SCREEN_WIDTH;
    fruitY = rand() % SCREEN_HEIGHT;
} // end randomizeFruit function
// To do: will draw multiple fruits or just a single one
void drawFruit()
{
    cout << "F";
} // end draw fruit function
// Part of Draw function, makes it cleaner to read.
void drawBoard(int boardHeight, int boardWidth)
{
    if (boardWidth == 0) // draws border when at the left edge
    {
        cout << "#";
    }
    else if (boardHeight == snakeY && boardWidth == snakeX) // if the snake's head is at a location draw it there
    {
        cout << (char)(SOLID_BLOCK_ASCII); // draws the ascii character - concatenate int to char
    }
    else if (boardHeight == fruitY && boardWidth == fruitX) // if the board hieg
    {
        drawFruit();
        // if(NUM_FRUIT > 1)
        // {  Save this for later when adding multiple fruits
        //   randomizeFruitLocation();
        //  }
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
// draw a matrix and then board
void drawMatrixBoard()
{
    for (int currHeight = 0; currHeight < SCREEN_HEIGHT; currHeight++)
    {
        for (int currWidth = 0; currWidth < SCREEN_WIDTH; currWidth++)
        {
            drawBoard(currHeight, currWidth); // pass in current height and width to drawBoard
        }
        cout << endl;
    }
}
// main draw function
void Draw()
{
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
    int key;
    cout << "\nPress a key to continue...";
    key = getch(); // retrieves input from keyboard to pause
    if (key == 0 || key == 224)
    {
        getch(); // starts game
    }
} // end pauseGame function
// keyboard input from user
void Input()
{
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
            if(sleepAmt <= 100)
            {
                sleepAmt += 5;
            }
            break;
        case 'l':
            if(sleepAmt > 0)
            {
                sleepAmt -= 5;
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
// Report location of snake's head in real time
void getXandYLocation()
{
    cout << "x=" << snakeX << "    y=" << snakeY << endl;
} // end getXY func
// method for when snake hits a wall and comes out the other side
void wallCollisionLogic()
{
    if (snakeX >= SCREEN_WIDTH)
    {
        snakeX = 1;
    }
    else if (snakeX <= 0)
    {
        snakeX = SCREEN_WIDTH - 1;
    }
    if (snakeY >= SCREEN_HEIGHT)
    {
        snakeY = 0;
    }
    else if (snakeY < 0)
    {
        snakeY = SCREEN_HEIGHT - 1;
    }
} // end wallCollisionLogic
// when snake head hits tail
void tailCollisionLogic()
{
    for (int i = 0; i < lengthTail; i++)
    {
        if (tailX[i] == snakeX && tailY[i] == snakeY) // xy values of tail are the same as the head xy values
        {
            gameOver = true;
        }
    }
} // end tailCollisionLogic
// change direction of the snake's head
void changeDirection()
{
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
    if (snakeX == fruitX && snakeY == fruitY)
    {
        score += 10;
        randomizeFruitLocation();
        lengthTail++;
    }
} // end increaseSnakeLengthAndScore function
// Calculates how a snake is drawn
void coreGrowthLogic()
{
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
// Where all game logic occurs
void Logic()
{
    coreGrowthLogic();
    changeDirection();
    // Report location of snake's head
    getXandYLocation();
    wallCollisionLogic();
    tailCollisionLogic();

    // This section increases the score when the snake touches the fruit, randomizes the location of a another fruit,
    // and increases the length of the tail
    increaseSnakeLengthAndScore();
} // end logic function
void displayInstructions() {
    cout << "W - MOVE UP\n"
         << "A - MOVE LEFT\n"
         << "S - MOVE DOWN\n"
         << "D - MOVE RIGHT\n" << endl;
    cout << "X or Q - QUIT GAME\n"
         << "P - PAUSE GAME\n"
         << "L - SPEED UP GAME\n"
         << "; - SLOW DOWN GAME\n" << endl;
    cout << "This is a game called Snake, when you eats fruit 'F' your snake grows longer.\n"
         << "Eat as much as you can and grow as big as possible!\n"
         << "Remember dying is fun!\n";
}
int main()
{
    displayInstructions();
    pauseGame();
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(sleepAmt);
    }
    cout << "Game over!\n";
    cout << "Total fruits eaten: " << lengthTail << endl;
    return 0;
}
// end main
