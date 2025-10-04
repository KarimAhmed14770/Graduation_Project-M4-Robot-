/**
@file: FILE_NAME.H
@author: M4_TEAM
@creation_date: 9/3/2024
*/

#ifndef _STD_TYPES_H
#define _STD_TYPES_H

/*******************************GPIO_PORT_INDEX*****************************/


#define PORTA						 					(0)
#define PORTB 										(1)
#define PORTC 										(2)

#define USE_STDPERIPH_DRIVER

/*******************************PIN_INDEX*****************************/


#define PIN0 	(0x0001)
#define PIN1 	(0x0002)
#define PIN2 	(0x0004)
#define PIN3 	(0x0008)
#define PIN4 	(0x0010)
#define PIN5 	(0x0020)
#define PIN6 	(0x0040)
#define PIN7 	(0x0080)
#define PIN8 	(0x0100)
#define PIN9 	(0x0200)
#define PIN10 (0x0400)
#define PIN11 (0x0800)
#define PIN12 (0x1000)
#define PIN13 (0x2000)
#define PIN14 (0x4000)
#define PIN15 (0x8000)


/*******************************BIT_INDEX*****************************/

#define BIT0 											(0)
#define BIT1 											(1)
#define BIT2 											(2)
#define BIT3 											(3)
#define BIT4 											(4)
#define BIT5 											(5)
#define BIT6 											(6)
#define BIT7 											(7)
#define BIT8 											(8)
#define BIT9 											(9)
#define BIT10 										(10)
#define BIT11 										(11)
#define BIT12 										(12)
#define BIT13 										(13)
#define BIT14 										(14)
#define BIT15											(15)
#define BIT16 										(16)
#define BIT17 										(17)
#define BIT18 										(18)
#define BIT19 										(19)
#define BIT20 										(20)
#define BIT21 										(21)
#define BIT22											(22)
#define BIT23											(23)
#define BIT24											(24)
#define BIT25 										(25)
#define BIT26 										(26)
#define BIT27 										(27)
#define BIT28 										(28)
#define BIT29 										(29)
#define BIT30 										(30)
#define BIT31											(31)


#define NULL (0)

/********************************LOGIC********************************/
#define HIGH 	(1)
#define LOW		(0)


/*****************TIMERS AND CHANNELS*********************************/
#define TIMER1		(1)
#define TIMER2		(2)
#define TIMER3		(3)
#define TIMER4		(4)

#define CH1				(1)
#define CH2				(2)
#define CH3				(3)
#define CH4				(4)
typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
//typedef unsigned long           uint32_t;
typedef unsigned long long      uint64_t;

typedef signed char             sint8_t;
typedef signed short            sint16_t;
typedef signed long             sint32_t;
typedef signed long long        sint64_t;

typedef float                   float32_t;
typedef double                  float64_t;



typedef enum{
    E_NOT_OK,
    E_OK
}Std_ReturnType_t;


typedef enum{
	NOT_INITIALIZED,
	INITIALIZED
}Initialization_State_t;
/*
typedef enum{
	DISABLE,
	ENABLE
}Status_t;
*/

#endif

