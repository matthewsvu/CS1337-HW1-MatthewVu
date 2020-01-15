/*
Program name - ModifySnakeGame.cpp
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
Notes:
    1. 1/14/20 - finish draw fruit function and rename Logic variable names
Comments:

*/

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

bool gameOver;

const int WIDTH = 20;
const int HEIGHT = 20;
const int NUM_FRUIT = 2;
const int SOLID_BLOCK_ASCII = 254;
const int SMALL_SOLID_BLOCK_ASCII = 220;

int headX, headY, fruitX, fruitY, score;
int tailX[100], tailY[100];
int lengthTail;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

void Setup()
{
    gameOver = false;
    dir = STOP;
    headX = WIDTH / 2;
    headY = HEIGHT / 2;
    // randomly places a fruit on the board
   // srand(time(0)); // changes the seed
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
}

void drawTopAndBottomBorder()
{
    for (int i = 0; i < WIDTH+1; i++)
    {
        cout << "#";
    }
    cout << endl;
}

// Will draws tail based on length given else draws an empty space to signify nothing there
void drawTailOrSpace(int boardHeight, int boardWidth)
{
    bool printTail = false;
    for (int length = 0; length < lengthTail; length++)
    {
        if (tailX[length] == boardWidth && tailY[length] == boardHeight)
        {
            cout << "o";
            printTail = true;
        }
    }
    if (!printTail)
    {
        cout << " ";
    }
}

// Part of Draw function, makes it cleaner to read.
void drawBoard(int boardHeight, int boardWidth)
{
    if (boardWidth == 0) // draws when at the left edge
    {
        cout << "#";
    }
    else if (boardHeight == headY && boardWidth == headX)
    {
        cout << (char)(SOLID_BLOCK_ASCII);
    }
    else if (boardHeight == fruitY && boardWidth == fruitX)
    {
        cout << "F";
    }
    else
    {
        drawTailOrSpace(boardHeight, boardWidth);
    }
    if(boardWidth == WIDTH - 1) // draws when at right edge
    {
        cout << "#";
    }
}

// To do: will draw multiple fruits or just a single one
void drawFruit(int boardHeight, int boardWidth)
{

}

// main draw function
void Draw()
{
    system("cls"); //system("clear");

    // top border
    drawTopAndBottomBorder();

    for (int h = 0; h < HEIGHT; h++)
    {
        for (int w = 0; w < WIDTH; w++)
        {
            drawBoard(h, w); // pass in current height and width to drawBoard
        }
        cout << endl;
    }

    // bottom border
    drawTopAndBottomBorder();
    cout << "Score:" << score << endl;
}
// keyboard input from user
void Input()
{
    if (_kbhit())
    {
        switch (_getch())
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
        case 'x':
            gameOver = true;
            break;
        }
    }
}

// Report location of snake's head in real time
void getXandYLocation()
{
    cout << "x=" << headX << "    y=" << headY << endl;
}

// method for when snake hits a wall and comes out the other side
void wallCollisionLogic()
{
    if (headX >= WIDTH)
    {
        headX = 1;
    }
    else if (headX <= 0)
    {
        headX = WIDTH - 1;
    }
    if (headY >= HEIGHT)
    {
        headY = 0;
    }
    else if (headY < 0)
    {
        headY = HEIGHT - 1;
    }
}

//
void tailCollisionLogic()
{
    for (int i = 0; i < lengthTail; i++)
    {
        if (tailX[i] == headX && tailY[i] == headY)
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
        headX--;
        break;
    case RIGHT:
        headX++;
        break;
    case UP:
        headY--;
        break;
    case DOWN:
        headY++;
        break;
    default:
        break;
    }
}
// to do: fix variable names
void Logic()
{
    //
    int prevTailX = tailX[0];
    int prevTailY = tailY[0];
    int prev2TailX, prev2TailY;
    tailX[0] = headX;
    tailY[0] = headY;

    for (int i = 1; i < lengthTail; i++)
    {
        prev2TailX = tailX[i];
        prev2TailY = tailY[i];
        tailX[i] = prevTailX;
        tailY[i] = prevTailY;
        prevTailX = prev2TailX;
        prevTailY = prev2TailY;
    }

    changeDirection();
    // Report location of snake's head
    getXandYLocation();
    wallCollisionLogic();
    tailCollisionLogic();
    // This section increases the score when the snake touches the fruit, randomizes the location of a another fruit,
    // and increases the length of the tail
    if (headX == fruitX && headY == fruitY)
    {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        lengthTail++;
    }
}
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

    return 0;
}
