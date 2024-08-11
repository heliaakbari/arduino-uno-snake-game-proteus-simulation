#include <Adafruit_NeoPixel.h>    // importar libreria
//   NEO_KHZ800  800 KHz velocidad de datos (WS2812 y nuevos)
//   NEO_KHZ400  400 KHz velocidad de datos (WS2811 y mas antiguos)
//   NEO_GRB     flujo de datos en orden GRB (WS2812 y nuevos)
//   NEO_RGB     flujo de datos en orden RGB (versiones mas antiguas)
#define up A0
#define down A2
#define left A3
#define right A1
Adafruit_NeoPixel tira = Adafruit_NeoPixel(256, 6, NEO_GRB + NEO_KHZ800); // creacion de objeto "tira"
// con una cantidad de 256 pixeles, sobre pin 2 del Arduino
int ground[256];
int snake[200]={104,105,106};
int snake_size;
int direction;
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
  delay(1); //1 milisecond
  read_button();
  time++;
  if(time==30){
    moveSnake();
    showBoard();
    time=0;
  }
}

void read_button()
{
 if(!digitalRead(left))
 {
   if(direction !=1){
      direction = -1;
   }
   while(!digitalRead(left));
 }

 if(!digitalRead(right))
 {
   if(direction !=-1){
      direction = 1;
   }
   while(!digitalRead(right));
 }

 if(!digitalRead(up))
 {
   if(direction !=16){
      direction = -16;
   }
   while(!digitalRead(up));
 }

 if(!digitalRead(down))
 {
   if(direction !=-16){
      direction = 16;
   }
   while(!digitalRead(down));
 }
}

void moveSnake(){
  int food = 0;
  if(direction==0){
    return;
  }
  int head = snake[0]+direction;
  if(ground[head]==1 || ground[head]==3 ){  //wall or its body
    gameOver();
    return;
  }
  else if(ground[head]==4){ //food
    snake[snake_size]=snake[snake_size -1];
    snake_size++;
    food = 1;
  }

  for(int i=0;i<snake_size;i++){
    ground[snake[i]]=0;
  }
  for(int i=snake_size-1;i>=1;i=i-1){
    snake[i]=snake[i-1];
  }
  snake[0]=head;
  for(int i=1;i<snake_size;i++){
    ground[snake[i]]=3;
  }
  ground[snake[0]]=2;

  if(food==1){
    generateSeed();
  }
}

void gameOver(){
  startSetting();
  showBoard();
}
void generateSeed(){
  int seed;
  while(free){
    seed = random(17,239);
    if (ground[seed]==0){
      ground[seed]=4;
      break;
    }
  }

}

void startSetting(){
  for(int i=0;i<256;i++){
    ground[i]=0;
  }
  for (int i = 0 ;i < 16 ;i=i+1){
  ground[i]=1;
  ground[i*16]=1;
  ground[i*16 + 15]=1;
  ground[i+240]=1;
  }
  snake_size=4;
  snake[0]=104;
  snake[1]=105;
  snake[2]=106;
  snake[3]=107;
  ground[104]=2;
  ground[105]=3;
  ground[106]=3;
  ground[107]=3;
  ground[102]=4;
  direction=-1;
}
void showBoard(){
  for(int i = 0; i < 256; i++){
      if(ground[i]==1){                          //wall
        tira.setPixelColor(i, 0, 0, 0);             //black
      }
      else if (ground[i]==0) {                  //ground
        tira.setPixelColor(i, 255,255, 255);        //white
      }
      else if (ground[i]==2) {                  //snake head
        tira.setPixelColor(i, 0,120,0);        //
      }
      else if (ground[i]==3) {                  //snake body
        tira.setPixelColor(i,0,255,0);        //
      }
      else if (ground[i]==4) {                  //food
        tira.setPixelColor(i, 255,0,0);        //
      }
  }   
  tira.show();
}