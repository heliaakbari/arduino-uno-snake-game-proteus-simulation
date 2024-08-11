#include <Adafruit_NeoPixel.h>    //import RGB LED library
#define up A0
#define down A2
#define left A3
#define right A1
Adafruit_NeoPixel tira = Adafruit_NeoPixel(256, 6, NEO_GRB + NEO_KHZ800); //the display device initiation

int ground [16] [16] { 
{1, 1, 1, 1, 1,1,1,1,1,1,1,1,1,1,1,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1},
{1, 1, 1, 1, 1,1,1,1,1,1,1,1,1,1,1,1}
};
int snake [100] [2]  {  //max snake length 100
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
};


int snake_size=3;
int direction[2]={0,0};
int time=0;
void setup(){
  pinMode(up, INPUT);
  pinMode(down, INPUT);
  pinMode(left, INPUT);
  pinMode(right, INPUT);
  digitalWrite(up, HIGH);
  digitalWrite(down, HIGH);
  digitalWrite(left, HIGH);
  digitalWrite(right, HIGH);
  startSetting();
  tira.setBrightness(255);
  tira.begin();       // inicializacion de la tira
  showBoard();
}


void loop(){
  delay(1); //read buttons every 1 milisecond
  read_button();
  time++;
  if(time==30){ //move & update board every 30 milisecond
    moveSnake();
    showBoard();
    time=0;
  }
}

void read_button()
{
 if(!digitalRead(left))
 {
   if(direction[1] != 1){
      direction[0] =  0;
      direction[1] = -1;
   }
   while(!digitalRead(left));
 }

 if(!digitalRead(right))
 {
   if(direction[1] != -1){
      direction[0] =  0;
      direction[1] = 1;
   }
   while(!digitalRead(right));
 }

 if(!digitalRead(up))
 {
   if(direction[0] != 1){
      direction[1] =  0;
      direction[0] = -1;
   }
   while(!digitalRead(up));
 }

 if(!digitalRead(down))
 {
   if(direction[0] != -1){
      direction[1] = 0;
      direction[0] = 1;
   }
   while(!digitalRead(down));
 }
}

void moveSnake(){
  int food = 0;
  if(direction[0]==0 && direction[1]==0){ //start of game 
    return;
  }
  int head[2] = {snake[0][0]+direction[0],snake[0][1]+direction[1]};
  if(ground[head[0]][head[1]] ==1 || ground[head[0]][head[1]]==3 ){  //wall or its body
    gameOver();
    return;
  }
  else if(ground[head[0]][head[1]] ==4 ){ //food
    snake[snake_size][0]=snake[snake_size -1][0];
    snake[snake_size][1]=snake[snake_size -1][1];
    snake_size++;
    food = 1;
  }

  for(int i=0;i<snake_size;i++){  //updating the ground and the snake array
    ground[snake[i][0]][snake[i][1]]=0;
  }
  for(int i=snake_size-1;i>=1;i=i-1){
    snake[i][0]=snake[i-1][0];
    snake[i][1]=snake[i-1][1];
  }
  snake[0][0]=head[0];
  snake[0][1]=head[1];
  for(int i=1;i<snake_size;i++){
    ground[snake[i][0]][snake[i][1]]=3;
  }
  ground[snake[0][0]][snake[0][1]]=2;

  if(food==1){      //place new food
    generateSeed();
  }
}

void gameOver(){
  delay(10);
  startSetting();
}

void generateSeed(){
  int seed0,seed1;
  while(free){
    seed0 = random(1,15);
    seed1 = random(1,15);
    if (ground[seed0][seed1]==0){
      ground[seed0][seed1]=4;
      break;
    }
  }

}

void startSetting(){
  for(int i=1;i<15;i++){
    for(int j=1;j<15;j++)
      ground[i][j]=0;
  }
  for (int i = 0 ;i < 16 ;i++){
    ground[i][0]=1;
    ground[0][i]=1;
    ground[15][i]=1;
    ground[i][15]=1;
  }
  snake_size=3;
  snake[0][0]=9;
  snake[0][1]=10;
  snake[1][0]=9;
  snake[1][1]=11;
  snake[2][0]=9;
  snake[2][1]=12;
  ground[9][10]=2;
  ground[9][11]=3;
  ground[9][12]=3;
  ground[9][7]=4;
  direction[0]=0;
  direction[1]=0;
}

void showBoard(){
  int k=0;
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 16; j++){
            switch (ground[i][j]) {
        case 0:
          tira.setPixelColor(k, 255,255, 255);
          break;
        case 1:
          tira.setPixelColor(k, 0, 0, 0);
          break;
        case 3:
          tira.setPixelColor(k,0,255,0); 
          break;
        case 2:
          tira.setPixelColor(k, 0,120,0);
          break;
        case 4:
          tira.setPixelColor(k, 255,0,0); 
      }
      k++;
    }
  }   
  tira.show();
}