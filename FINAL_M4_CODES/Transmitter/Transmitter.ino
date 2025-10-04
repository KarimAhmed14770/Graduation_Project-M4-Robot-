#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Transmitter.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows



RF24 radio(9, 10); // select CE,CSN pin
const byte address[6] = {0xCC,0xCE,0xCC,0xCE,0xCC};

//arrays of Data
uint16_t User_input[11];
uint8_t Sent_Data[13];

//wifi connectivity test variables
uint8_t connection_test_f=69;
uint8_t connection_test_l=96;
uint8_t mode;



//MODE flags
uint8_t Mobile_robot_flag;
uint8_t Crouch_flag;
uint8_t Drone_flag;
uint8_t Free_Servos_flag;
uint8_t Free_Servos1_flag;
uint8_t Free_Servos2_flag;

uint16_t BL_joystick_pr;
void setup()
{
  Serial.begin(115200);
  radio.begin();
  radio.setDataRate(RF24_1MBPS);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();



  lcd.init(); // initialize the lcd
  lcd.backlight();

  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);   
  pinMode(6,INPUT_PULLUP);      
  pinMode(7,INPUT_PULLUP);      

}


void loop()
{

  
  //Reading input Data and storing it in input array
  User_input[TL_JOYSTICK]   =   analogRead(A1);
  User_input[TR_JOYSTICK]    =  analogRead(A0);
  User_input[BR_JOYSTICK]    =  analogRead(A2);
  User_input[BL_JOYSTICK]    =  analogRead(A3);
  User_input[POT_VALUE]      =  analogRead(A5);
  User_input[SWITCH1_VALUE]   = digitalRead(5);
  User_input[SWITCH2_VALUE]   = digitalRead(4);
  User_input[SWITCH3_VALUE]   = digitalRead(2);
  User_input[SWITCH4_VALUE]   = digitalRead(6);
  User_input[SWITCH5_VALUE]   = digitalRead(3);
  User_input[SWITCH6_VALUE]   = digitalRead(7);


  //adjusting some of the input data
  //adjust_joystick_values(&User_input[TL_JOYSTICK],1); till we replace the joystick  
  adjust_joystick_values(&User_input[TR_JOYSTICK],1);
  adjust_joystick_values(&User_input[BR_JOYSTICK],1);
  adjust_joystick_values(&User_input[BL_JOYSTICK],0);



  //choosing the mode based on the user input
  if(User_input[SWITCH1_VALUE]==1 && User_input[SWITCH2_VALUE]==0 &&User_input[SWITCH3_VALUE]==0 && User_input[SWITCH4_VALUE]==0 && 
    User_input[SWITCH6_VALUE]==0)
    {
      Mobile_robot_flag=1;
      Crouch_flag=0;
      Drone_flag=0;
      Free_Servos_flag=0;
   

    }

    else if(User_input[SWITCH1_VALUE]==0 && User_input[SWITCH2_VALUE]==1 &&User_input[SWITCH3_VALUE]==0 && User_input[SWITCH4_VALUE]==0  && 
    User_input[SWITCH6_VALUE]==0)
    {
      Mobile_robot_flag=0;
      Crouch_flag=1;
      Drone_flag=0;
      Free_Servos_flag=0;
    }

   else if(User_input[SWITCH1_VALUE]==0 && User_input[SWITCH2_VALUE]==0 &&User_input[SWITCH3_VALUE]==1 && User_input[SWITCH4_VALUE]==0  && 
    User_input[SWITCH6_VALUE]==0)
    {
      Mobile_robot_flag=0;
      Crouch_flag=0;
      Drone_flag=1;
      Free_Servos_flag=0;
    }

    else if(User_input[SWITCH1_VALUE]==0 && User_input[SWITCH2_VALUE]==0 &&User_input[SWITCH3_VALUE]==0 && User_input[SWITCH4_VALUE]==1 && 
    User_input[SWITCH6_VALUE]==0)
    {
      Mobile_robot_flag=0;
      Crouch_flag=0;
      Drone_flag=0;
      Free_Servos_flag=1;
    }

  if(Free_Servos_flag==1 &&User_input[SWITCH5_VALUE]==1)
  {
    mode=FREE_SERVOS1;
  }
  else if(Mobile_robot_flag==1)
  {
    mode=MOBILE_ROBOT;
  }

  else if(Crouch_flag==1)
  {
    mode=CROUCH;
  }
  else if (Drone_flag==1)
  {
    mode=DRONE;
  }
  //writing the data to be sent 
  Sent_Data[CONNECTIVITY_TEST_F]=connection_test_f;
  Sent_Data[MODE]=mode;
  Sent_Data[TL_JOYSTICK_L8]=uint8_t(User_input[TL_JOYSTICK]);//till we replace the joystick 
  Sent_Data[TL_JOYSTICK_H8]=uint8_t(User_input[TL_JOYSTICK]>>8);//till we replace the joystick
  Sent_Data[TR_JOYSTICK_L8]=uint8_t(User_input[TR_JOYSTICK]);
  Sent_Data[TR_JOYSTICK_H8]=uint8_t(User_input[TR_JOYSTICK]>>8);

  Sent_Data[BR_JOYSTICK_L8]=uint8_t(User_input[BR_JOYSTICK]);
  Sent_Data[BR_JOYSTICK_H8]=uint8_t(User_input[BR_JOYSTICK]>>8);  

  Sent_Data[BL_JOYSTICK_L8]=uint8_t(User_input[BL_JOYSTICK]);
  Sent_Data[BL_JOYSTICK_H8]=uint8_t(User_input[BL_JOYSTICK]>>8);  

  Sent_Data[POT_VALUE_L8]=uint8_t(User_input[POT_VALUE]);
  Sent_Data[POT_VALUE_H8]=uint8_t(User_input[POT_VALUE]>>8);

   Sent_Data[CONNECTIVITY_TEST_L]=connection_test_l;      
  
  //debugging user input
  
  Serial.print(User_input[TL_JOYSTICK]);
  Serial.print("\t");
  Serial.print(User_input[TR_JOYSTICK]);
  Serial.print("\t");
  Serial.print(User_input[BR_JOYSTICK]);
  Serial.print("\t");
  Serial.print(User_input[BL_JOYSTICK]);
  Serial.print("\t");
  Serial.print(User_input[POT_VALUE]);
  Serial.print("\t");
  radio.write(&User_input, sizeof(User_input));
  Serial.print(User_input[SWITCH1_VALUE]);
  Serial.print("\t");
  Serial.print(User_input[SWITCH2_VALUE]);
  Serial.print("\t");
  Serial.print(User_input[SWITCH3_VALUE]);
  Serial.print("\t");
  Serial.print(User_input[SWITCH4_VALUE]);
  Serial.print("\t");
  Serial.print(User_input[SWITCH5_VALUE]);
  Serial.print("\t");
  Serial.print(User_input[SWITCH6_VALUE]);
  Serial.print("\t");
  Serial.println(mode);
  
  /*
  //debugging sent data
  Serial.print(Sent_Data[CONNECTIVITY_TEST_F]);
  Serial.print("\t");
  Serial.print(Sent_Data[MODE]);
  Serial.print("\t");
  Serial.print(Sent_Data[TL_JOYSTICK_L8]);
  Serial.print("\t");
  Serial.print(Sent_Data[TL_JOYSTICK_H8]);
  Serial.print("\t");
  Serial.print(Sent_Data[TR_JOYSTICK_L8]);
  Serial.print("\t");
  Serial.print(Sent_Data[TR_JOYSTICK_H8]);
  Serial.print("\t");
  Serial.print(Sent_Data[BR_JOYSTICK_L8]);
  Serial.print("\t");
  Serial.print(Sent_Data[BR_JOYSTICK_H8]);
  Serial.print("\t");
  Serial.print(Sent_Data[BL_JOYSTICK_L8]);
  Serial.print("\t");
  Serial.print(Sent_Data[BL_JOYSTICK_H8]);
  Serial.print("\t");
  Serial.print(Sent_Data[POT_VALUE_L8]);
  Serial.print("\t");
  Serial.print(Sent_Data[POT_VALUE_H8]);
  Serial.print("\t");
  Serial.println(Sent_Data[CONNECTIVITY_TEST_L]);
  */
  radio.write(&Sent_Data, sizeof(Sent_Data));


 lcd.setCursor(0, 0);         // move cursor to   (0, 0)
 lcd.print(User_input[TL_JOYSTICK]);  
 lcd.setCursor(0, 0);         // move cursor to   (0, 0)
 lcd.print(User_input[TR_JOYSTICK]); 
 lcd.setCursor(0, 0);         // move cursor to   (0, 0)
 lcd.print(User_input[TL_JOYSTICK]); 
 lcd.setCursor(0, 0);         // move cursor to   (0, 0)
 lcd.print(User_input[TL_JOYSTICK]); 
 lcd.setCursor(0, 0);         // move cursor to   (0, 0)
 lcd.print(User_input[TL_JOYSTICK]); 
 lcd.setCursor(0, 0);         // move cursor to   (0, 0)
 lcd.print(User_input[TL_JOYSTICK]); 












}
