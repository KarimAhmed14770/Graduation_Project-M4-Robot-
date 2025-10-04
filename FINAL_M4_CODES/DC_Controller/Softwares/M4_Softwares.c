/**
@file: M4_Softwares.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "M4_Softwares.h"
/********************************INCLUDES SECTION END******************************************/



/********************************DATA TYPES DEFINITIONS SECTION START**************************/

/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/

/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
double map_to_range(double in_min,double in_max,double out_min, double out_max,double value)
{
	double mapped_value=0;
 
	
   double scale_factor = (out_max - out_min) / (in_max - in_min);
		
 
    mapped_value = out_min +( scale_factor * (value - in_min));
	
  return mapped_value;
}

/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/

/********************************HELPER FUNCTIONS DEFINITIONS SECTION END**********************/



