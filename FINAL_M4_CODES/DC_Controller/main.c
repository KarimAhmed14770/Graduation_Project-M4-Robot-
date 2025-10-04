
/*******************DC_controller Code ***********************/
#include "MCAL_Layer/SPI/MCAL_SPI1.h"
#include "main.h"
#include "MCAL_Layer/TIMERS/General_Purpose_Timers/MCAL_TIMERS.h"
#include "HAL_Layer/DC_MOTOR/HAL_DC_MOTOR.h"



//spi1 to communicate with servo controllers
Spi1_t spi1={
	.mode=SLAVE,
	.clock_phase=IDLE_LOW,
	.clock_phase=FIRST_EDGE,
	.data_frame=SPI_EIGHT_BIT,
};

//extern timer2_config_t tim2;
uint8_t 	Rc_Data[13];//data recieved from the remote control
uint16_t 	Used_Data[8];//data used by all controllers and sent to servo controller
uint8_t delay_counter;


//dc motors
Dc_motor_t FR={
	//IN1=PA0
	.IN1_pin.port_index=PORTA,
	.IN1_pin.pin_index=PIN1,

	//IN2=PB12
	.IN2_pin.port_index=PORTA,
	.IN2_pin.pin_index=PIN0,

	//PWM=PA10
	.pwm_timer=TIMER3,
	.pwm_channel=CH3,
};

Dc_motor_t FL={
	//IN1=PA0
	.IN1_pin.port_index=PORTA,
	.IN1_pin.pin_index=PIN2,

	//IN2=PB12
	.IN2_pin.port_index=PORTA,
	.IN2_pin.pin_index=PIN3,

	//PWM=PA10
	.pwm_timer=TIMER3,
	.pwm_channel=CH4,
};

Dc_motor_t BR={
	//IN1=PA0
	.IN1_pin.port_index=PORTA,
	.IN1_pin.pin_index=PIN9,

	//IN2=PB12
	.IN2_pin.port_index=PORTA,
	.IN2_pin.pin_index=PIN8,

	//PWM=PA10
	.pwm_timer=TIMER1,
	.pwm_channel=CH4,
};

Dc_motor_t BL={
	//IN1=PA0
	.IN1_pin.port_index=PORTB,
	.IN1_pin.pin_index=PIN14,

	//IN2=PB12
	.IN2_pin.port_index=PORTB,
	.IN2_pin.pin_index=PIN15,

	//PWM=PA10
	.pwm_timer=TIMER1,
	.pwm_channel=CH3,
};
/*
timer4_config_t tim4_={
	.TIM4_OF_InterruptHandler=NULL,
	.time_base_us=1000000
};*/
//global variables to store radio control data values 
	uint8_t RC_Mode;
	uint8_t Servos_Mode;
	uint16_t TL_joystick;
	uint16_t TR_joystick;
	uint16_t BR_joystick;
	uint16_t BL_joystick;
	uint16_t POT_value;

int main()
{
	spi1_init(&spi1);
	//timer2_init(&tim2);
	Dc_motor_init(&FR);
	Dc_motor_init(&FL);
	Dc_motor_init(&BR);
	Dc_motor_init(&BL);
	//timer4_init(&tim4_);
	while(1)
	{
		/*
		if(delay_counter==0)
		{
			//timer2_delay(&tim2,2500);
			delay_counter++;
		}
		*/
		
		spi1_recieve_8(Rc_Data,sizeof(Rc_Data));
		if(Rc_Data[CONNECTIVITY_TEST_F]==69 &&Rc_Data[CONNECTIVITY_TEST_L]==96)
			{
			RC_Mode=Rc_Data[MODE];
			TL_joystick=((Rc_Data[TL_JOYSTICK_L8])|(Rc_Data[TL_JOYSTICK_H8])<<8);
			TR_joystick=((Rc_Data[TR_JOYSTICK_L8])|(Rc_Data[TR_JOYSTICK_H8])<<8);
			BR_joystick=((Rc_Data[BR_JOYSTICK_L8])|(Rc_Data[BR_JOYSTICK_H8])<<8);
			BL_joystick=((Rc_Data[BL_JOYSTICK_L8])|(Rc_Data[BL_JOYSTICK_H8])<<8);
			POT_value=((Rc_Data[POT_VALUE_L8])|(Rc_Data[POT_VALUE_H8])<<8);
				
		}
			if(RC_Mode==MOBILE_ROBOT)
		{
			Dc_motor_move(&FR,TR_joystick);
			Dc_motor_move(&FL,TR_joystick);
			Dc_motor_move(&BR,TR_joystick);
			Dc_motor_move(&BL,TR_joystick);
		}
		else
		{
						Dc_motor_stop(&FR);
						Dc_motor_stop(&BR);

						Dc_motor_stop(&FL);

						Dc_motor_stop(&BL);

		}
		
		
		}
		
	}




	