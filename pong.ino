#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED 0xF800
#define MAGENTA 0xF81F

MCUFRIEND_kbv tft;

int p1_x;
int p1_y = 300;

int p2_x;
int p2_y;

int ball_x = 120;
int ball_y = 160;

int ball_move_x = 5;
int ball_move_y = 5;

int p1_points = 0;
int p2_points = 0;

bool start_game = false;
bool one_player = false;

int ai = 0;

//joysticks
#define joystick1_sw A7
#define joystick1_x A6
#define joystick1_y A8

#define joystick2_sw A11
#define joystick2_x A9
#define joystick2_y A10

//buttons
#define left_button 51
#define right_button 52

//leds for score tracking
#define p1_led1 45
#define p1_led2 47
#define p1_led3 49
#define p1_led4 43
#define p1_led5 41

#define p2_led1 39
#define p2_led2 50
#define p2_led3 44
#define p2_led4 42
#define p2_led5 36

int x1 = 0, x1_temp = 0;
int x2 = 0, x2_temp = 0;
String str;

//////////////////////////////////reset game ///////////////////////////////
void reset_game()
{
  start_game = false;
  one_player = false;
  p1_points = 0;
  p2_points = 0;
  ball_x = 120;
  ball_y = 160;
  digitalWrite(p1_led1, LOW);
  digitalWrite(p1_led2, LOW);
  digitalWrite(p1_led3, LOW);
  digitalWrite(p1_led4, LOW);
  digitalWrite(p1_led5, LOW);

  digitalWrite(p2_led1, LOW);
  digitalWrite(p2_led2, LOW);
  digitalWrite(p2_led3, LOW);
  digitalWrite(p2_led4, LOW);
  digitalWrite(p2_led5, LOW);
}

///////////////////////// menu //////////////////////////////////////////
void menu()
{
  if(!start_game)
  {
    tft.setCursor(0, 10);
    tft.setRotation(-45);
    tft.println("Move joystick left for two players, right for one"); 
  }
  int in = analogRead(joystick1_y);
  Serial.println(in);
  if(in > 700 && start_game == false)
  {
    start_game = true;
    tft.setRotation(0);
    tft.fillScreen(BLACK);
    Serial.println("start game!");
    randomSeed(millis());
    int rand1 = random(0,100);
    Serial.println(rand1);
    if(rand1 > 50)
    {
      ball_move_x = 5;
    }
    else
    {
      ball_move_x = -5;
    }
    
    int rand2 = random(0,100);
    Serial.println(rand2);
    if(rand2 > 50)
    {
      ball_move_y = 5;
    }
    else
    {
      ball_move_y = -5;
    }

  }
  else if(in < 100 && start_game == false)
  {
    start_game = true;
    one_player = true;
    tft.setRotation(0);
    tft.fillScreen(BLACK);
    Serial.println("start game!");
    randomSeed(millis());
    int rand1 = random(0,100);
    Serial.println(rand1);
    if(rand1 > 50)
    {
      ball_move_x = 5;
    }
    else
    {
      ball_move_x = -5;
    }
    
    int rand2 = random(0,100);
    Serial.println(rand2);
    if(rand2 > 50)
    {
      ball_move_y = 5;
    }
    else
    {
      ball_move_y = -5;
    }
  }
}

//////////////////////////// drawing the board /////////////////////////////
void draw_board()
{
  if(start_game)
  {
    tft.fillRect(5,20,10,280,WHITE);
    tft.fillRect(225,20,10,280,WHITE);
  }
  
}
/////////////////////////// move player(s) funcitons ///////////////////////
void move_p1()
{
  //x1 = analogRead(joystick1_y);
  if(start_game)
  {
    
    if (Serial.available()) 
    {
      str = Serial.readStringUntil('\n');
      x1 = str.substring(0, str.indexOf(' ')).toInt();
      
      if(!one_player)
      {
        x2 = str.substring(str.indexOf(' ') + 1).toInt();
      }
    }
    
    //x1 = x1/5;
    p1_x = x1;
    tft.fillRect(x1,300,50,10,WHITE);
    //boundary boxes? idk what they're called
    tft.fillRect(x1 + 50,300,100,10,BLACK);
    tft.fillRect(x1 - 100,300,100,10,BLACK);
  }


  /*
  Serial.print(x1);
  Serial.print(" ");
  Serial.println(y1);
  */
}

void move_p2()
{
  
  //int x2 = analogRead(joystick2_y);
 if(start_game && !one_player)
  {
    //x2 = x2/5;
    p2_x = x2;
    tft.fillRect(x2,10,50,10,WHITE);
    //boundary boxes? idk what they're called
    tft.fillRect(x2 + 50,10,100,10,BLACK);
    tft.fillRect(x2 - 100,10,100,10,BLACK);

  
  }
  else if(start_game && one_player) //player 1 com implementation
  {
    randomSeed(millis());
    if(ball_move_y < 0) //moving to com player
    {
      ai = random(0,20);
      if(ai < 13)
      {
        int b = ball_y - ((ball_move_y/ball_move_x)*ball_x);
        x2 = ((10-b)*ball_move_x)/ball_move_y;
        p2_x = x2;
        tft.fillRect(x2,10,50,10,WHITE);
        //boundary boxes? idk what they're called
        tft.fillRect(x2 + 50,10,100,10,BLACK);
        tft.fillRect(x2 - 100,10,100,10,BLACK);
      }
    }
    
  }

  /*
  Serial.print(x2);
  Serial.print(" ");
  Serial.println(y2);
  */
}

//////////////////////////// ball movement ///////////////////////
void move_ball()
{
  if(start_game)
  {
    tft.fillRect(ball_x, ball_y, 15, 15, WHITE);
    if(ball_move_x > 0 && ball_move_y > 0) //down right
    {
      tft.fillRect(ball_x - 5, ball_y - 5, 15, 5, BLACK); //changing on x axis
      tft.fillRect(ball_x -5, ball_y -5, 5, 15, BLACK); //changing on y axis
    }
    else if(ball_move_x < 0 && ball_move_y > 0) //down left
    {
      tft.fillRect(ball_x , ball_y - 5, 15, 5, BLACK); //changing on x axis
      tft.fillRect(ball_x + 15, ball_y -5, 5, 15, BLACK); //changing on y axis
    }
    else if(ball_move_x < 0 && ball_move_y < 0) //up left
    {
      tft.fillRect(ball_x + 15, ball_y + 5, 5, 15, BLACK); //changing on x axis
      tft.fillRect(ball_x + 5, ball_y + 15, 10, 5, BLACK); //changing on y axis
    }
    else if(ball_move_x > 0 && ball_move_y < 0) // up right
    {
      tft.fillRect(ball_x - 5, ball_y + 5, 5, 15, BLACK); //changing on x axis
      tft.fillRect(ball_x - 5, ball_y + 15, 15, 5, BLACK); //changing on y axis
    }

    ball_x += ball_move_x;
    ball_y += ball_move_y;
  }
  
  /*
  tft.fillCircle(ball_x,ball_y,10,WHITE);
  tft.fillCircle(ball_x - 15,ball_y - 15,10,BLACK);
  ball_x += 15;
  ball_y += 15;
  */
}

////////////////////////// ball collision detection ///////////////
void collision()
{
  if(ball_x == 215)
  {
    ball_move_x *= -1;
    tft.fillRect(ball_x - 5, ball_y - 5, 15, 5, BLACK); 
    tft.fillRect(ball_x -5, ball_y -5, 5, 15, BLACK);
  }
  if(ball_x == 15)
  {
    ball_move_x *= -1;
    tft.fillRect(ball_x , ball_y - 5, 15, 5, BLACK); 
    tft.fillRect(ball_x + 15, ball_y -5, 5, 15, BLACK); 
  }
  if(ball_y >= 290 && ball_y <= 300 && ball_x >= p1_x && ball_x <= p1_x + 50)
  {
    ball_move_y *= -1;
    tft.fillRect(ball_x , ball_y - 5, 15, 5, BLACK); 
    tft.fillRect(ball_x + 15, ball_y -5, 5, 15, BLACK); 
  }
  if(ball_y <= 20 && ball_y >= 10 && ball_x >= p2_x && ball_x <= p2_x + 50)
  {
    ball_move_y *= -1;
    tft.fillRect(ball_x - 5, ball_y + 5, 5, 15, BLACK); 
    tft.fillRect(ball_x - 5, ball_y + 15, 15, 5, BLACK);
  }

}
//////////////////////////// points //////////////////////////////////

void points()
{
  
  if(ball_y > 300)
  {
    p2_points++;
    ball_x = 120;
    ball_y = 160;
    tft.fillRect(0, 305, 300, 15, BLACK); //changing on y axis
    Serial.print(p1_points);
    Serial.print(" ");
    Serial.println(p2_points);
    tft.fillScreen(BLACK);
    if(p2_points == 1)
    {
      digitalWrite(p2_led1, HIGH);
    }
    else if(p2_points == 2)
    {
      digitalWrite(p2_led2, HIGH);
    }
    else if(p2_points == 3)
    {
      digitalWrite(p2_led3, HIGH);
    }
    else if(p2_points == 4)
    {
      digitalWrite(p2_led4, HIGH);
    }
    else if(p2_points == 5)
    {
      digitalWrite(p2_led5, HIGH);
    }
  }
  if(ball_y < 9)
  {
    p1_points++;
    ball_x = 120;
    ball_y = 160;
    tft.fillRect(0, 0, 300, 10, BLACK); //changing on y axis
    Serial.print(p1_points);
    Serial.print(" ");
    Serial.println(p2_points);
    tft.fillScreen(BLACK);

    if(p1_points == 1)
    {
      digitalWrite(p1_led1, HIGH);
    }
    else if(p1_points == 2)
    {
      digitalWrite(p1_led2, HIGH);
    }
    else if(p1_points == 3)
    {
      digitalWrite(p1_led3, HIGH);
    }
    else if(p1_points == 4)
    {
      digitalWrite(p1_led4, HIGH);
    }
    else if(p1_points == 5)
    {
      digitalWrite(p1_led5, HIGH);
    }
  }    
  
  if(p1_points == 5 || p2_points == 5)
  {
    if(p1_points == 5)
    {
      tft.setCursor(0, 30);
      tft.setRotation(-45);
      tft.println("Player 1 Wins!"); 
    }
    if(p2_points == 5)
    {
      tft.setCursor(0, 30);
      tft.setRotation(-45);
      tft.println("Player 2 Wins!"); 
    }
    reset_game();
  }

}
///////////////////////////// random function ///////////////////////////
static unsigned int random_number()
{
  static unsigned int random_num = millis();
  unsigned int start_range = 0;
  unsigned int end_range = 240;
  random_num += 0x3AD;
  random_num %= end_range;

  while(random_num < start_range)
  {
      random_num = random_num + end_range - start_range;
  }

  return random_num;
}

//////////////////////////// states ///////////////////////////
typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;

int delay_gcd;
const unsigned short tasksNum = 6; //number of states
task tasks[tasksNum];

///////////////////////// move player 1 state ///////////////////////

enum SM1_States{SM1_INIT,move_player1};
int SM1_Tick(int state1)
{
  switch (state1)
  {
    case SM1_INIT:
      state1 = move_player1;
      break;
    
    //loop, constantly get input from joystick
    case move_player1:
      state1 = move_player1;
      break;
  }

  switch (state1)
  {
    case SM1_INIT:
      break;
    
    case move_player1:
      move_p1();
      draw_board();
      break;
  }

  return state1;
}

/////////////////////// move player 2 state ////////////////////////////
enum SM2_States{SM2_INIT,move_player2};
int SM2_Tick(int state2)
{
  switch (state2)
  {
    case SM2_INIT:
      state2 = move_player2;
      break;
    
    //loop, constantly get input from joystick
    case move_player2:
      state2 = move_player2;
      break;
  }

  switch (state2)
  {
    case SM2_INIT:
      break;
    
    case move_player2:
      move_p2();
      break;
  }

  return state2;
}

////////////////////////// move ball state ////////////////////////
enum SM3_States{SM3_INIT,move_b};
int SM3_Tick(int state3)
{
  switch (state3)
  {
    case SM3_INIT:
      state3 = move_b;
      break;
    
    case move_b:
      state3 = move_b;
      break;
  }

  switch (state3)
  {
    case SM3_INIT:
      break;
    
    case move_b:
      move_ball();
      break;
  }

  return state3;
}

////////////////////////////// ball collision state///////////////////////
enum SM4_States{SM4_INIT,collide};
int SM4_Tick(int state4)
{
  switch (state4)
  {
    case SM4_INIT:
      state4 = collide;
      break;
    
    
    case collide:
      state4 = collide;
      break;
  }

  switch (state4)
  {
    case SM4_INIT:
      break;
    
    case collide:
      collision();
      break;
  }

  return state4;
}

/////////////////////// points state /////////////////////////////
enum SM5_States{SM5_INIT,point};
int SM5_Tick(int state5)
{
  switch (state5)
  {
    case SM5_INIT:
      state5 = point;
      break;
    
    
    case point:
      state5 = point;
      break;
  }

  switch (state5)
  {
    case SM5_INIT:
      break;
    
    case point:
      points();
      break;
  }

  return state5;
}
////////////////////////////menu////////////////////////////////////
enum SM6_States{SM6_INIT,menuu};
int SM6_Tick(int state6)
{
  switch (state6)
  {
    case SM6_INIT:
      state6 = menuu;
      break;
    
    
    case menuu:
      state6 = menuu;
      break;
  }

  switch (state6)
  {
    case SM6_INIT:
      break;
    
    case menuu:
      if(!start_game)
      {
        menu();
      }
      break;
  }

  return state6;
}

/////////////////////// setup ///////////////////////////////////
void setup() {
  
  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 0; 
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  ++i;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 0; 
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;
  ++i;
  tasks[i].state = SM3_INIT;
  tasks[i].period = 50; 
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM3_Tick;
  ++i;
  tasks[i].state = SM4_INIT;
  tasks[i].period = 50; 
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM4_Tick;
  ++i;
  tasks[i].state = SM5_INIT;
  tasks[i].period = 0; 
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM5_Tick;
  ++i;
  tasks[i].state = SM6_INIT;
  tasks[i].period = 0; 
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM6_Tick;


  //screen settings
  tft.reset();
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(0);
  tft.fillScreen(BLACK);

  //joystick buttons
  pinMode(joystick1_sw,INPUT);
  pinMode(joystick2_sw,INPUT);

  //buttons
  pinMode(left_button, INPUT_PULLUP);
  pinMode(right_button, INPUT);

  //leds
  pinMode(p1_led1, OUTPUT);
  pinMode(p1_led2, OUTPUT);
  pinMode(p1_led3, OUTPUT);
  pinMode(p1_led4, OUTPUT);
  pinMode(p1_led5, OUTPUT);

  pinMode(p2_led1, OUTPUT);
  pinMode(p2_led2, OUTPUT);
  pinMode(p2_led3, OUTPUT);
  pinMode(p2_led4, OUTPUT);
  pinMode(p2_led5, OUTPUT);
  Serial.begin(9600);
}

/////////////////////////////////// loop ///////////////////////////////

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) 
  {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }

}
