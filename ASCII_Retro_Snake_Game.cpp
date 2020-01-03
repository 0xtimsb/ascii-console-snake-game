// Open-source snake project made by Smit Barmase (@smitbarmase). CS Freshman.
// 24 Dec 2019. (v1.0)
// Learn. Modify. Share.
// I will keep updating this repo, adding more things to it.
// Thank you! Giving credit is appreciated. :)
// Current version. 3 Jan 2019 (v1.4)

#include<iostream>
#include<windows.h> //For cursor info
#include<conio.h> //For get keyboard hit
#include<chrono> //For clock
#include<string> //String
#include<time.h> //For srand to get seed from nanosec.
using namespace std;
using namespace std::chrono;

//Game setting data
//--------------------------
  const int H_size = 20; //Predefined canvas size.
  const int W_size = 40;
  const int maxSize = 100; //Max tail size
  const float defautWaitTime = 0.1;   //Waiting time in sec for every snake step.
  const string gameOverTitle = "Game Over";
  const string gameStartAgain = "Starts in ";
  const int startSize = 3;  //Start tail size
//---------------------------

//Private data no need to change.
//---------------------------
high_resolution_clock::time_point t1, t2; //For keeping track of time duration
int x_dir, y_dir; //X = 1 for Right and -1 for Left.  Y = 1 for Down and -1 for Up.
int tail_pos[maxSize][2]; //[size of tail] [0--> x_Position, 1--> y_Position]
int food_pos[2]; //food spawn position.[0--> x_Position, 1--> y_Position]
int size = startSize;
float waitingTime = defautWaitTime; //Varying time for vertical axis.
bool gameOver = false;
int score = 0;
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

void RandomPos(int& m,int& n)
{
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  srand((time_t)t.tv_nsec); // Random seed value from nanosec. //used time.h.

  m = (rand() % (W_size-1));
  m == 0 ? m += 1 : NULL; //If X_pos coincide wall.
  n = (rand() % (H_size-1));
  n == 0 ? n += 1 : NULL; //If Y_pos coincide wall.
}

void GiveFoodPosition()
{
  int x,y;
  RandomPos(x,y);
  loop:
  for(int i=0;i<size;i++) //Condition to check body collision with food.
  {
    if(x==tail_pos[i][0] && y==tail_pos[i][1]) //if food spawn on body.
    {
      RandomPos(x,y); //use random position of food.
      goto loop; //repeat checking if food is on body.
    }
  }
  food_pos[0] = x;
  food_pos[1] = y;
}

void GenerateFood()  //Function to spawn food at random position inside canvas.
{
  GiveFoodPosition();
  setCursorPosition(food_pos[0],food_pos[1]);
  cout<<"*";
}

void MakeCanvas()
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

  //Make Score panel.
  setCursorPosition(W_size+3,(H_size/2)); //Align string in center-right.
  cout<<"Score : "<<score;
}

void UpdateState()
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
    //Condition to check body collision.
    for(int i=1;i<size;i++)
    {
      if(tail_pos[0][0]==tail_pos[i][0] && tail_pos[0][1]==tail_pos[i][1])
      {
        gameOver = true;
        break;
      }
    }

    //Set coursor to new head position.
    setCursorPosition(tail_pos[0][0],tail_pos[0][1]);
    //Generate food at random point of grid if head ate food.
    if(tail_pos[0][0]==food_pos[0] && tail_pos[0][1]==food_pos[1])
    {
      GenerateFood();
      size++;

      score += 10; //Increase score here.

      //Display score panel.
      setCursorPosition(W_size+3,(H_size/2)); //Align string in center-right.
      cout<<"Score : "<<score;
    }
    cout.flush();
  }
  else
  {
    gameOver = true;
  }
}

void CheckInput()  //Keyboard input function.
{
  if(_kbhit())
  {
    switch(_getch())
    {
      //If conditions to stop, backward inputs.
      case 72: if(y_dir!=1){x_dir = 0; y_dir = -1;} //72 Up Arrow
          break;
      case 75: if(x_dir!=1){x_dir = -1; y_dir = 0;} //68 Left Arrow
          break;
      case 77: if(x_dir!=-1){x_dir = 1; y_dir = 0;}  //67 Right Arrow
          break;
      case 80: if(y_dir!=-1){x_dir = 0; y_dir = 1;}  //80 Down Arrow
          break;
      default:
          break;
    }
    //Increse wait time by x2 while traveling y axis.
    if(x_dir==0){
      waitingTime = 2*defautWaitTime;
    }else if(y_dir==0){
      waitingTime = defautWaitTime;
    }
  }
}

int main()
{
  ShowConsoleCursor(false);  //Hide cursor.
  MakeCanvas();

  while(true) //For replaying game after game over.
  {
    GenerateFood();  //Generate food for first time.
    for(int i=0;i<maxSize;i++) //Resetting all values of tail.
    {
      tail_pos[i][0]=1, tail_pos[i][1]=1;
    }
    x_dir = 1; y_dir = 0; //Reseting snake directions.
    size = startSize; //Reset snake size.

    t1 = high_resolution_clock::now(); //Resetting time counter.
    t2 = {};

    while (gameOver==false) //Runs every frame.
    {
        CheckInput(); //Get input.
        t2 = high_resolution_clock::now();
        if(duration_cast<duration<float>>(t2 - t1).count()>=waitingTime)
        {
            UpdateState();  //Runs every defautWaitTime.
            t1 = high_resolution_clock::now();
            t2 = {};
        }
    }

    //Game Over >>>
    //Score.
    score = 0; //Resetting score.

    //Display score panel.
    setCursorPosition(W_size+3,(H_size/2)); //Align string in center-right.
    cout<<"Score : 0              "; //Spaces to clear everything.

    //Clearing existing snake.
    for(int i=size;i>0;i--)
    {
      setCursorPosition(tail_pos[i][0],tail_pos[i][1]);
      cout<<" ";
    }
    //Clearing existing Food.
      setCursorPosition(food_pos[0],food_pos[1]);
      cout<<" ";
    //Game over screen.
    setCursorPosition((W_size/2)-(gameOverTitle.size()/2),(H_size/2)-1); //Align string in center.
    cout<<gameOverTitle;
    for(int i=5;i>0;i--)
    {
      setCursorPosition((W_size/2)-(gameStartAgain.size()/2)-2,(H_size/2));
      cout<<gameStartAgain;
      cout<<i<<"...";
      Sleep(1000);
    }
    //Clearing all text from canvas.
    setCursorPosition((W_size/2)-(gameOverTitle.size()/2),(H_size/2)-1);
    for(int i=gameOverTitle.size();i>0;i--)
    {
      cout<<" ";
    }
    setCursorPosition((W_size/2)-(gameStartAgain.size()/2)-2,(H_size/2));
    for(int i=(gameStartAgain.size()+4);i>0;i--)  //+4 for "x..."
    {
      cout<<" ";
    }
    //Setting gameOver to flase.
    gameOver = false;
  }
  return 0;
}
