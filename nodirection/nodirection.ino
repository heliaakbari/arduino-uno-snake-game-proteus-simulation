#include <Adafruit_NeoPixel.h>    // importar libreria
//   NEO_KHZ800  800 KHz velocidad de datos (WS2812 y nuevos)
//   NEO_KHZ400  400 KHz velocidad de datos (WS2811 y mas antiguos)
//   NEO_GRB     flujo de datos en orden GRB (WS2812 y nuevos)
//   NEO_RGB     flujo de datos en orden RGB (versiones mas antiguas)

Adafruit_NeoPixel tira = Adafruit_NeoPixel(256, 2, NEO_GRB + NEO_KHZ800); // creacion de objeto "tira"
// con una cantidad de 256 pixeles, sobre pin 2 del Arduino
int ground[256];
int snake[200]={104,105,106};
int snake_size;
int direction;

void setup(){
  startSetting();
  tira.setBrightness(255);
  tira.begin();       // inicializacion de la tira
  showBoard();
}

void loop(){
  showBoard();
  delay(50);
  moveSnake();
  delay(50);
}

void moveSnake(){
  if(direction==0){
    return;
  }
  int head = snake[0]+direction;
  if(ground[head]==1 || ground[head]==3 ){  //wall or its body
    gameOver();
  }
  else if(ground[head]==4){ //food
    snake[snake_size]=snake[snake_size-1] - direction;
    snake_size++;
    generateSeed();
  }

  for(int i=0;i<snake_size;i++){
    ground[snake[i]]=0;
    snake[i]=snake[i]+direction;
    ground[snake[i]]=3;
    ground[snake[0]]=2;
  }

  if(ground[head]==4){
    generateSeed();
  }
}

void gameOver(){
  delay(500);
  startSetting();
  showBoard();
  delay(500);
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
  snake_size=3;
  snake[0]=104;
  snake[1]=105;
  snake[2]=106;
  ground[104]=2;
  ground[105]=3;
  ground[106]=3;
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