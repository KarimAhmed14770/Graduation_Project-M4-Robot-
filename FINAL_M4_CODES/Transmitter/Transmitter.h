
#ifndef TRANSMITTER_H
#define TRANSMITTER_H


/*********************************** Macros definition section start*************************/
//Reading User input Array Elements Index 
#define TL_JOYSTICK  (0)
#define TR_JOYSTICK  (1)
#define BR_JOYSTICK  (2)
#define BL_JOYSTICK  (3)
#define POT_VALUE     (4)
#define SWITCH1_VALUE (5)
#define SWITCH2_VALUE (6)
#define SWITCH3_VALUE (7)
#define SWITCH4_VALUE (8)
#define SWITCH5_VALUE (9)
#define SWITCH6_VALUE (10)



//Sent Data Array Index 
#define CONNECTIVITY_TEST_F (0)
#define MODE                (1)
#define TL_JOYSTICK_L8      (2)
#define TL_JOYSTICK_H8      (3)
#define TR_JOYSTICK_L8      (4)
#define TR_JOYSTICK_H8      (5)
#define BR_JOYSTICK_L8      (6)
#define BR_JOYSTICK_H8      (7)
#define BL_JOYSTICK_L8      (8)
#define BL_JOYSTICK_H8      (9)
#define POT_VALUE_L8        (10)
#define POT_VALUE_H8        (11)
#define CONNECTIVITY_TEST_L (12)


//Joystick Deadbands
#define JOYSTICK_LOW_DEADBAND   (491)
#define JOYSTICK_HIGH_DEADBAND  (531)
#define JOYSTICK_CENTER_VALUE   (511)



//Robot Modes

#define   MOBILE_ROBOT (11)
#define   DRONE        (22)
#define   CROUCH       (33)
#define   FREE_SERVOS1 (44)
#define   FREE_SERVOS2 (55)



/*********************************** Macros definition section end*************************/



/**********************************Functions Declaration Section start********************/
void adjust_joystick_values(unsigned short *value, unsigned char is_inverse);

/**********************************Functions Declaration Section end********************/




#endif