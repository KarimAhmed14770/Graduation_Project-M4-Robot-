/*******************Mode Controller code***********************/
#include "MCAL_Layer/SPI/MCAL_SPI1.h"
#include "MCAL_Layer/SPI/MCAL_SPI2.h"
#include "MCAL_Layer/UART/MCAL_UART.h"
#include "HAL_Layer/SERVO_MOTOR/HAL_SERVO.h"
#include "main.h"
#include "Softwares/M4_Softwares.h"
#include "MCAL_Layer/TIMERS/General_Purpose_Timers/MCAL_TIMERS.h"


/******************************8initialization for objects*******************/
uint64_t prev_millis;
uint64_t curr_millis;
//spi2 object to communicate with flight controller
Spi2_t spi2={
	.mode=SLAVE,
	.clock_phase=IDLE_LOW,
	.clock_phase=FIRST_EDGE,
	.data_frame=SPI_EIGHT_BIT,
};

//spi1 object to communicate with dc motors controller
Spi1_t spi1={
	.mode=MASTER,
	.clock_phase=IDLE_LOW,
	.clock_phase=FIRST_EDGE,
	.data_frame=SPI_EIGHT_BIT,
	.baud_rate=SPI1_BR_HZ_1125000,
	.SLAVE_SELECT.port_index=PORTA,
	.SLAVE_SELECT.pin_index=PIN4
};

void tim1_isr(void);
void tim3_isr(void);


/*************************88Servo Motor Objects******************************/
/***********************8MODE FLAGS*****************************/
uint8_t MOBILE_TO_DRONE=0;
uint8_t DRONE_TO_MOBILE=0;
uint8_t ARMS_FROM_0_TO_90=0;
uint8_t BOX_FROM_0_TO_90=0;
uint8_t	ARMS_FROM_90_TO_0=0;
uint8_t BOX_FROM_90_TO_0=0;

uint8_t counter;
//uint8_t tim3_counter;

typedef enum{
	Mobile_Robot=11,
	Crouch=22,
	Drone=33

}MOde_t;

MOde_t MODE_;


/**********************************************************/
Servo_t FLB={
	.pwm_timer=TIMER4_PWM, //B9
	.pwm_channel=PWM_CH4,
	.is_reverse=1,
	.gear_ratio=1.7,
	.curr_pos=FLB_START
};

Servo_t FRB={
	.pwm_timer=TIMER4_PWM, //B6
	.pwm_channel=PWM_CH1,
	.is_reverse=0,
	.gear_ratio=1.7,
	.curr_pos=FRB_START

};

Servo_t FRA={
	.pwm_timer=TIMER4_PWM, //B7
	.pwm_channel=PWM_CH2,
	.is_reverse=1,
	.gear_ratio=1.5,
	.curr_pos=FRA_START
	
};

Servo_t FLA={
	.pwm_timer=TIMER4_PWM, //B8
	.pwm_channel=PWM_CH3,
	.is_reverse=0,
	.gear_ratio=1.5,
	.curr_pos=FLA_START
	
};


Servo_t BRA={
	.pwm_timer=TIMER2_PWM, //A2
	.pwm_channel=PWM_CH3,
	.is_reverse=0,
	.gear_ratio=1.5,
	.curr_pos=BRA_START

};

Servo_t BRB={
	.pwm_timer=TIMER2_PWM, //A3
	.pwm_channel=PWM_CH4,
		.is_reverse=1,
	.gear_ratio=1.7,
	.curr_pos=BRB_START
};

Servo_t BLB={
	.pwm_timer=TIMER2_PWM, //A0
	.pwm_channel=PWM_CH1,
	.is_reverse=0,
	.gear_ratio=1.7,
	.curr_pos=BLB_START

};

Servo_t BLA={
	.pwm_timer=TIMER2_PWM, //A1
	.pwm_channel=PWM_CH2,
	.is_reverse=1,
	.gear_ratio=1.5,
	.curr_pos=BLA_START

};


/**********************************************************/

//timer object for timing purposes
//extern timer1_config_t tim1;

uint8_t Rc_Data[13];//data recieved from the flight controller
uint16_t Used_Data[8];
char usart_char=0x88;
uint8_t delay_counter;

void tim1_isr(void);
//void tim3_isr(void);
// 3 debugging leds objects
Pin_Config_t ledc13={
	.port_index=PORTC,
	.pin_index=PIN13,
	.mode=OUTPUT_MODE_50,
	.conf=OUTPUT_PP,
};

Pin_Config_t ledc14={
	.port_index=PORTC,
	.pin_index=PIN14,
	.mode=OUTPUT_MODE_50,
	.conf=OUTPUT_PP,
};

Pin_Config_t ledc15={
	.port_index=PORTC,
	.pin_index=PIN15,
	.mode=OUTPUT_MODE_50,
	.conf=OUTPUT_PP,
};

//global variables to store radio control data values 
	uint8_t RC_Mode;
	uint8_t Servos_Mode;
	uint16_t TL_joystick;
	uint16_t TR_joystick;
	uint16_t BR_joystick;
	uint16_t BL_joystick;
	uint16_t POT_value;
	uint8_t test;
	uint8_t value=60;
extern timer1_config_t tim1;

//extern timer3_config_t tim3;
uint8_t curr_pos;
uint8_t tim1_counter;

int main()
 {
	spi1_init(&spi1);//sending to dc controller
	spi2_init(&spi2); //recieving from flight controller
	timer1_init(&tim1);
	servo_init(&FLB);	
	servo_init(&FRB);
	servo_init(&BRB);
	servo_init(&BLB);
	servo_init(&FRA);
	servo_init(&FLA);		
	servo_init(&BLA);
	servo_init(&BRA);
	gpio_pin_init(&ledc13);
	gpio_pin_init(&ledc14);
	gpio_pin_init(&ledc15);
	while(1)
	{
		if (tim1_counter==0)
		{
		tim1.TIM1_OF_InterruptHandler=tim1_isr;
		tim1.time_base_us=15000;
		timer1_init(&tim1);
		tim1_counter++;
		}
		
		/*if(tim3_counter==0)
		{
		tim3.TIM3_OF_InterruptHandler=tim3_isr;
		tim3.time_base_us=50000;
		timer3_init(&tim3);
		tim3_counter++;
		}*/
		spi2_recieve_8(Rc_Data, sizeof(Rc_Data));
		if(Rc_Data[CONNECTIVITY_TEST_F]==69 &&Rc_Data[CONNECTIVITY_TEST_L]==96)
		{
			gpio_pin_write(&ledc14,HIGH);
			RC_Mode=Rc_Data[MODE];
			TL_joystick=((Rc_Data[TL_JOYSTICK_L8])|(Rc_Data[TL_JOYSTICK_H8])<<8);
			TR_joystick=((Rc_Data[TR_JOYSTICK_L8])|(Rc_Data[TR_JOYSTICK_H8])<<8);
			BR_joystick=((Rc_Data[BR_JOYSTICK_L8])|(Rc_Data[BR_JOYSTICK_H8])<<8);
			BL_joystick=((Rc_Data[BL_JOYSTICK_L8])|(Rc_Data[BL_JOYSTICK_H8])<<8);
			POT_value=((Rc_Data[POT_VALUE_L8])|(Rc_Data[POT_VALUE_H8])<<8);
			spi1_transmit_8(Rc_Data,sizeof(Rc_Data));
	}
}
 
	if(RC_Mode==MOBILE_ROBOT)
	{
	MODE_=Mobile_Robot;
	servo_go(&FRB,0);
	servo_go(&FRA,0);
	servo_go(&FLB,0);
	servo_go(&FLA,0);
	servo_go(&BRB,0);
	servo_go(&BLB,0);
	servo_go(&BRA,0);
	servo_go(&BLA,0);
	}
	else if(RC_Mode==DRONE )
	{
		MODE_=Drone;
		MOBILE_TO_DRONE=1;
		ARMS_FROM_0_TO_90=1;
	}
	else if(RC_Mode==CROUCH )
	{
		MODE_=Crouch;
		MOBILE_TO_DRONE=1;
		ARMS_FROM_0_TO_90=1;
	}
	
}


	void tim1_isr(void)
	{
		if(MOBILE_TO_DRONE)
		{
			DRONE_TO_MOBILE=0;
			if (ARMS_FROM_0_TO_90){
		if(FLA.curr_pos<(FLA_90*FLA.gear_ratio))
		{
			uint8_t temp=FLA.curr_pos+1;
			servo_go(&FLA,temp);
		}
		if(map_to_range(0,180,180,0,FRA.curr_pos)<(FRA_90*FRA.gear_ratio))
		{
			uint8_t temp=map_to_range(0,180,180,0,FRA.curr_pos)+1;
			servo_go(&FRA,temp);
		}
		if(BRA.curr_pos<(BRA_90*BRA.gear_ratio))
		{
			uint8_t temp=BRA.curr_pos+1;
			servo_go(&BRA,temp);
		}
		if(map_to_range(0,180,180,0,BLA.curr_pos)<(BLA_90*BLA.gear_ratio))
		{
			uint8_t temp=map_to_range(0,180,180,0,BLA.curr_pos)+1;
			servo_go(&BLA,temp);
		}
		if(FLA.curr_pos>=(FLA_90*FLA.gear_ratio) &&(map_to_range(0,180,180,0,FRA.curr_pos)>=(FRA_90*FRA.gear_ratio))
			&&(map_to_range(0,180,180,0,BLA.curr_pos)>=(BLA_90*BLA.gear_ratio)) &&(BRA.curr_pos>=(BRA_90*BRA.gear_ratio)))
		{
			ARMS_FROM_0_TO_90=0;
			ARMS_FROM_90_TO_0=1;
			BOX_FROM_0_TO_90=1;
		}
	}
			if(BOX_FROM_0_TO_90==1 && ARMS_FROM_90_TO_0==1)
			{
				if(BRA.curr_pos>1)
		{
			uint8_t temp=BRA.curr_pos-1;
			servo_go(&BRA,temp);
		}
		if(FLA.curr_pos>1)
		{
			uint8_t temp=FLA.curr_pos-1;
			servo_go(&FLA,temp);
		}
		if(map_to_range(0,180,180,0,FRA.curr_pos)>0)
		{
			uint8_t temp=map_to_range(0,180,180,0,FRA.curr_pos)-1;
			servo_go(&FRA,temp);
		}
			if(map_to_range(0,180,180,0,BLA.curr_pos)>0)
		{
			uint8_t temp=map_to_range(0,180,180,0,BLA.curr_pos)-1;
			servo_go(&BLA,temp);
		}
		
		if(FLA.curr_pos==0 &&((map_to_range(0,180,180,0,FRA.curr_pos)))==0
			&&(map_to_range(0,180,180,0,BLA.curr_pos))==0 &&(BRA.curr_pos==0))
		{
			
		ARMS_FROM_90_TO_0=0;
			
		}
		/*******************************************************************/
		
		if(FRB.curr_pos<(FRB_90*FRB.gear_ratio))
		{
			uint8_t temp=FRB.curr_pos+1;
			servo_go(&FRB,temp);
		}

		
		if(map_to_range(0,180,180,0,FLB.curr_pos)<(FLB_90*FLB.gear_ratio))
		{
			uint8_t temp=map_to_range(0,180,180,0,FLB.curr_pos)+1;
			servo_go(&FLB,temp);
		}
		if(BLB.curr_pos<(BLB_90*BLB.gear_ratio))
		{
			uint8_t temp=BLB.curr_pos+1;
			servo_go(&BLB,temp);
		}
		if(map_to_range(0,180,180,0,BRB.curr_pos)<(BRB_90*BRB.gear_ratio))
		{
			uint8_t temp=map_to_range(0,180,180,0,BRB.curr_pos)+1;
			servo_go(&BRB,temp);
		}
		if(FRB.curr_pos>=(FRB_90*FRB.gear_ratio) &&(map_to_range(0,180,180,0,FLB.curr_pos)>=(FLB_90*FLB.gear_ratio))
			&&(map_to_range(0,180,180,0,BRB.curr_pos)>=(BRB_90*BRB.gear_ratio)) &&(BLB.curr_pos>=(BLB_90*BLB.gear_ratio)))
		
		{
			BOX_FROM_0_TO_90=0;
		}
		
	}
}
	
		}
	/*
		void tim3_isr(void)
		{
			
		}
			
		*/