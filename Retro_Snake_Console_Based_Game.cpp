// Open-source snake project made by Smit Barmase (@smitbarmase). CS Freshman.
// 24 Dec 2019. (v1.0)
// Learn. Modify. Share.
// Going to update this repo, adding more things to it.
// Like varying speed, scores, gameover and game start screen.
// Thank you! Giving credit is appreciated. :)
// Current version. 26 Dec 2019 (v1.1)

#include<iostream>
#include<windows.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
using namespace std;

//Predefined canvas size.
const int H_size = 20;
const int W_size = 40;
//Waiting time in millisec for every update.
float defautWaitTime = 50;

//----------------------------
int x_dir = 1, y_dir = 0; //X = 1 for Right and -1 for Left.  Y = 1 for Down and -1 for Up.
int tail_pos[50][2] = {}; //[size of tail] [0--> x_Position, 1--> y_Position]
int food_pos[2]={}; //food spawn position.[0--> x_Position, 1--> y_Position]
int size = 3; //Tail size
bool gameOver = false;
float varingWaitTime = defautWaitTime;
//----------------------------


//Function to Show/Hide console cursor. Used it for hiding blinking cursor.
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

//Function to set cursor position in console.
void setCursorPosition(int x, int y)
{
    static const HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(out, coord);
}

void Start()
{
  //Making canvas wall here.
  for(int i=0;i<W_size;i++)
  {
    for(int j=0;j<H_size;j++)
    {
      if(i==0 || i==W_size-1)
      {
        setCursorPosition(i,j);
        cout<<']';
      }
      else
      {
        if(j==0 || j==H_size-1)
        {
          setCursorPosition(i,j);
          cout<<']';
        }
        else
        {
          setCursorPosition(i,j);
          cout<<' ';
        }
      }
    }
  }
}

void GenerateFood()  //Function to spawn food at random position inside canvas.
{
  srand(time(0)); // Random seed value for rand based on time
  food_pos[0] = (rand() % (W_size-1));
  food_pos[0]==0 ? food_pos[0]+=1 : NULL; //If food coincide wall.
  food_pos[1] = (rand() % (H_size-1));
  food_pos[1]==0 ? food_pos[1]+=1 : NULL; //If food coincide wall.
  setCursorPosition(food_pos[0],food_pos[1]);
  cout<<"*";
}

void Update()
{
  //Remove previous printed last part of tail.
  if(tail_pos[size-1][0]!=0 || tail_pos[size-1][1]!=0)
  {
    setCursorPosition(tail_pos[size-1][0],tail_pos[size-1][1]);
    cout<<" ";
  }
  //Shift every x-y position of tail co-ordinate, by one. Tail size remains same.
  for(int i=size-1;i>=0;i--)
  {
    tail_pos[i+1][0] = tail_pos[i][0];
    tail_pos[i+1][1] = tail_pos[i][1];
  }
  //Change head position as per direction.
  tail_pos[0][0] += x_dir;
  tail_pos[0][1] += y_dir;

  //Condition to check if snake hit the wall.
  if(tail_pos[0][0]>0 && tail_pos[0][1]>0 && tail_pos[0][0]<W_size-1 && tail_pos[0][1]<H_size-1)
  {
    //Set coursor to new head position.
    setCursorPosition(tail_pos[0][0],tail_pos[0][1]);
    //Print head.
    cout<<"o";
    //Generate food at random point of grid if head ate food.
    if(tail_pos[0][0]==food_pos[0] && tail_pos[0][1]==food_pos[1])
    {
      GenerateFood();
      size++;
    }
    //score += 10; //Increase score here.
    cout.flush();
  }
  else
  {
    gameOver = true;
  }
}

void Input()  //Keyboard input function.
{
  if(_kbhit())
  {
    switch(_getch())
    {
      case 72: x_dir = 0; y_dir = -1; //72 Up Arrow
          break;
      case 75: x_dir = -1; y_dir = 0; //68 Left Arrow
          break;
      case 77: x_dir = 1; y_dir = 0;  //67 Right Arrow
          break;
      case 80: x_dir = 0; y_dir = 1;  //80 Down Arrow
          break;
      default:
          break;
    }

    //Decrese speed by half while traveling y dir. And back to normal when x.
    if(x_dir==0 && varingWaitTime==defautWaitTime) //check if traveling in y
      varingWaitTime = defautWaitTime*2; //Increase wait time because y cell distance is also double of x cell. So, that speed remain constant.
    else if(y_dir==0 && varingWaitTime==(defautWaitTime*2)) //check if traveling in x
      varingWaitTime = defautWaitTime;
  }
}

int main()
{
  ShowConsoleCursor(false);  //Hide cursor.
  Start();
  GenerateFood();  //Generate food for first time.
  tail_pos[0][0] = 1, tail_pos[0][1] = 1;  //Snake head start point.
  while(true) //Loop until game over.
  {
    if(gameOver==false)
    {
      Input(); //Get key pressed input.
      Update(); //Update grid as per game.
    }
    Sleep((int)varingWaitTime);
  }
  return 0;
}
