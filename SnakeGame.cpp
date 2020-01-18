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
4.
Notes:
    1. 1/14/20 - finish draw fruit function and rename Logic variable names
    2. 1/15/20 - comments on snake logic because I don't understand them
Comments:
How would I add a way to have multiple fruits on one board that are fully interactable with the snake head?
I think it would be similar to the snake growth logic for some reason.

*/

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

bool gameOver;

const int SCREEN_WIDTH = 20;
const int SCREEN_HEIGHT = 20;
const int MAX_TAIL_LENGTH = 100;
const int NUM_FRUIT = 2;
const int SOLID_BLOCK_ASCII = 254;

int snakeX, snakeY, fruitX, fruitY, score;
int tailX[MAX_TAIL_LENGTH], tailY[MAX_TAIL_LENGTH];
int lengthTail;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

void Setup()
{
    gameOver = false;
    dir = STOP;
    // places the snake in the middle of the board
    snakeX = SCREEN_WIDTH / 2;
    snakeY = SCREEN_HEIGHT / 2;
    // randomly places a fruit on the board
    //srand(time(0)); // changes the seed for random num generator
    fruitX = rand() % SCREEN_WIDTH;
    fruitY = rand() % SCREEN_HEIGHT;
    score = 0;
} // end set up function
void drawTopAndBottomBorder()
{
    for (int i = 0; i < SCREEN_WIDTH+1; i++)
    {
        cout << "#";
    }
    cout << endl;
} // end draw top and bottom border function
// Will draw tail based on length given else draws an empty space to signify nothing there
void drawTailOrSpace(int boardHeight, int boardWidth)
{
    bool printTail = false;
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
            printTail = true;
        }
    }
    if (!printTail)
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
    else if (boardHeight == snakeY && boardWidth == snakeX)
    {
        cout << (char)(SOLID_BLOCK_ASCII); // draws the ascii character - concatenate int to char
    }
    else if (boardHeight == fruitY && boardWidth == fruitX)
    {
        drawFruit();
       // if(NUM_FRUIT > 1) {  Save this for later when adding multiple fruits
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
// main draw function
void Draw()
{
    system("cls"); //system("clear");
    // top border
    drawTopAndBottomBorder();
    for (int currHeight = 0; currHeight < SCREEN_HEIGHT; currHeight++)
    {
        for (int currWidth = 0; currWidth < SCREEN_WIDTH; currWidth++)
        {
            drawBoard(currHeight, currWidth); // pass in current height and width to drawBoard
        }
        cout << endl;
    }
    // bottom border
    drawTopAndBottomBorder();
    showUserScore();
} // end Draw function
void pauseGame()
  {
  int key;
  cout << "\nPress a key to continue...";
  key = getch(); // retrieves input from keyboard
  if (key == 0 || key == 224) {
    getch(); // starts game
  }
  } // end pauseGame function
// keyboard input from user
void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
        case 'A':
            dir = LEFT;
            break;
        case 'd':
        case 'D':
            dir = RIGHT;
            break;
        case 'w':
        case 'W':
            dir = UP;
            break;
        case 's':
        case 'S':
            dir = DOWN;
            break;
        case 'q':
        case 'Q':
            gameOver = true;
            break;
        case 'x':
        case 'X':
            gameOver = true;
            break;
        case 'p': // will stop game once p is pressed
        case 'P':
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
} // end wallLogic
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
}
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
}

// This method will increase the snake's tail length, score, and randomize apple location
void increaseSnakeLengthAndScore()
{
    if (snakeX == fruitX && snakeY == fruitY)
    {
        score += 10;
        randomizeFruitLocation();
        lengthTail++;
    }
}
// to do: fix variable names and understand logic
void Logic()
{
    //
    int prevTailX = tailX[0];
    int prevTailY = tailY[0];
    int prev2TailX, prev2TailY;
    tailX[0] = snakeX;
    tailY[0] = snakeY;

    for (int i = 1; i < lengthTail; i++) //
    {
        prev2TailX = tailX[i];
        prev2TailY = tailY[i];
        tailX[i] = prevTailX;
        tailY[i] = prevTailY;
        prevTailX = prev2TailX;
        prevTailY = prev2TailY;
    }
    //
    changeDirection();
    // Report location of snake's head
    getXandYLocation();
    wallCollisionLogic();
    tailCollisionLogic();
    // This section increases the score when the snake touches the fruit, randomizes the location of a another fruit,
    // and increases the length of the tail
    increaseSnakeLengthAndScore();
} // end logic function
int main()
{
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(10); //sleep(10);
    }
    cout << "Game over!\n";
    cout << "Total fruits eaten: " << lengthTail << endl;
    return 0;
}
// end main
