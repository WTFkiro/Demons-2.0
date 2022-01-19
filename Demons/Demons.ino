/*
https://arduino.esp8266.com/stable/package_esp8266com_index.json

数字引脚 0  D0  16
数字引脚 1  D1  5
数字引脚 2  D2  4
数字引脚 3  D3  0
数字引脚 4  D4  2
数字引脚 5  D5  14
数字引脚 6  D6  12
数字引脚 7  D7  13
数字引脚 8  D8  15
发送      发送  1
接收      接收  3
*/

#include <SoftwareSerial.h>
#include "FastLED.h"

#define NUM_LEDS 64 //灯珠数量
#define DATA_PIN 4 //数据引脚
#define KEY 15
#define S1 12
#define S2 13
#define LED_TYPE WS2812
#define COLOR_ORDER GRB

SoftwareSerial MySerial(0, 2); //19连接模块TX，18连接模块RX
uint8_t max_bright = 30;         //设置最大亮度为50
const byte StampPin = 5;        //盖章按键引脚
const byte LinePin = 13;         //台词按键引脚
const byte FunctionPin = 0;     //功能切换按键引脚
int i, j, k;                     //循环变量
int count = 0;                   //旋钮模块计数
int linecount = 1;               //台词计数
int S1Last;
int S2Last;
int Musicnumber;
int MusicLast;
int S1number;
int button_state = 0;             //变身步骤状态
int switch_state = true;          //模式选择状态
int knob_state = true;            //旋钮功能选择
int henshin_state = 0;            //当前变身状态
int NowAnimal = 0;                //当前选择印章
unsigned long previousMillis = 0; //记录当前时间
unsigned long currentMillis;      //用于获取millis
long interval = 60000;            //定义常量来表示固定的时间间隔
double ADDED[4] = {0, 0, 0, 0};   //记录增强状态
int ADDcount = 0;                 //记录增强印章数量
int played = 0;                   //是否选择新印章
CRGB leds[NUM_LEDS];              //初始化灯带

void ICACHE_RAM_ATTR CHOOSEBUTTON();
void ICACHE_RAM_ATTR CHOOSEFUNCTION();
void ICACHE_RAM_ATTR SWITCHBUTTON();
void ICACHE_RAM_ATTR CHOOSEKNOB();
void ICACHE_RAM_ATTR CHOOSEKEY();
void ICACHE_RAM_ATTR GOADD();
void ICACHE_RAM_ATTR SelectMusic();
void ICACHE_RAM_ATTR Set_Open();
void ICACHE_RAM_ATTR ANIMAL();
//void ICACHE_RAM_ATTR 

void setup()
{
  Serial.begin(115200);
  MySerial.begin(9600);
  delay(1000);
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  pinMode(StampPin, INPUT_PULLUP);
  pinMode(LinePin, INPUT_PULLUP);
  pinMode(FunctionPin, INPUT_PULLUP);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(KEY, INPUT);
  attachInterrupt(digitalPinToInterrupt(StampPin), CHOOSEBUTTON, FALLING);
  attachInterrupt(digitalPinToInterrupt(LinePin), CHOOSEFUNCTION, FALLING);
  attachInterrupt(digitalPinToInterrupt(FunctionPin), SWITCHBUTTON, FALLING);
  attachInterrupt(digitalPinToInterrupt(S1), CHOOSEKNOB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(KEY), CHOOSEKEY, FALLING);
  volume(0x10);       //音量设置
  playmode(0x02);     //0x01:单曲循环 0x02:单次播放
  playmusic(1, 0x00); //开机音效
  Demons_Eye(0, 1000);
  S1Last = digitalRead(S1);
  S2Last = digitalRead(S2);
}

void loop()
{
   Demons_Deal();
   Demons_DecideUP();
   Demons_Spider();
   Demons_AddPush();
   //Demons_Add();
   Demons_Finish();
   Demons_Bata();
   Demons_Lex();
   Demons_Lion();
   Demons_DominateUP();
    /*
  if (button_state == 5)
  {
    ADD(); //印章追加动画封装函数
  }
  if (button_state == 7)
  {
    REQUIEM(); //镇魂曲必杀动画封装函数
  }
  henshin_state = 0;
  */
}

void Set_Open()
{
  if (button_state == 0)
  {
    button_state = 1;
  }
  else if (button_state == 2)
  {
    button_state = 3;
  }
  else
  {
    button_state = 0;
  }
}

void RESET()
{
  count = 0;
}

void ANIMAL() //通过旋钮模块选择印章
{
  S1number = digitalRead(S1);
  if (S1number != S1Last)
  {
    if (digitalRead(S2) != S1number)
    {
      count++;
    }
    else
    {
      count--;
    }
    Serial.println(count);
    S1Last = S1number;
    if (count < 10)
    {
      NowAnimal = 1;
    }
    else if (count < 20)
    {
      NowAnimal = 2;
    }
    else if (count < 30)
    {
      NowAnimal = 3;
    }
    else if (count < 40)
    {
      NowAnimal = 4;
    }
    else
    {
      count = 0;
      NowAnimal = 1;
    }
    if (played != NowAnimal)
    {
      insertmusic(NowAnimal);
      played = NowAnimal;
    }
  }
}

void GOADD()
{
  if (button_state == 10)
  {
    button_state = 7;
  }
  else if (button_state != 5)
  {
    button_state = 5;
  }
}

void SWITCHBUTTON()
{
  switch_state = !switch_state;
}

void CHOOSEKNOB()
{
  if (switch_state)
  {
    ANIMAL();
  }
  else
  {
    if (knob_state)
    {
      VolumeSet();
    }
    else
    {
      SelectMusic();
    }
  }
}

void SelectMusic()
{
  Musicnumber = digitalRead(S1);
  if (Musicnumber != MusicLast)
  {
    if (digitalRead(S2) != Musicnumber)
    {
      nextmusic();
    }
    else
    {
      lastmusic();
    }
    MusicLast = Musicnumber;
  }
}

void CHOOSEBUTTON()
{
  if (switch_state)
  {
    Set_Open();
  }
  else
  {
    lastfolder();
  }
}
void CHOOSEFUNCTION()
{
  if (switch_state)
  {
    playline();
  }
  else
  {
    nextfolder();
  }
}
void CHOOSEKEY()
{
  if (switch_state)
  {
    GOADD();
  }
  else
  {
    knob_state = !knob_state;
  }
}
