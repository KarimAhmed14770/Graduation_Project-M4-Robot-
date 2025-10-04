/**
@file: MCAL_SPI_GEN_CFG.h
@author: M4_TEAM 
@creation_date: date
*/

#ifndef MCAL_SPI_GEN_CFG_H
#define MCAL_SPI_GEN_CFG_H

#define SPI1_MASTER (1)
#define SPI1_SLAVE 	(0)

#define SPI2_MASTER (0)
#define SPI2_SLAVE 	(1)

typedef enum{
	SLAVE,
	MASTER
}spi_mode_t;



typedef enum{
	IDLE_LOW,
	IDLE_HIGH
}spi_clock_polarity_t;

typedef enum{
	FIRST_EDGE,
	TRAILING_EDGE
}spi_clock_phase_t;

typedef enum{
	SPI_EIGHT_BIT,
	SPI_SIXTEEN_BIT
}spi_data_frame_t;
#endif

