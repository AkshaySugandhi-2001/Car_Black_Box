
#include <xc.h>
#include"i2c.h"

void init_i2c(void)
{
	/* Set SCL and SDA pins as inputs */
	TRISC3 = 1;
	TRISC4 = 1;
	/* Set I2C master mode */
	SSPCON1 = 0x28;

	SSPADD = 0x31;
	/* Use I2C levels, worked also with '0' */
	CKE = 0;
	/* Disable slew rate control  worked also with '0' */
	SMP = 1;
	/* Clear SSPIF interrupt flag */
	SSPIF = 0;
	/* Clear bus collision flag */
	BCLIF = 0;
}

void i2c_idle(void)
{
	while (!SSPIF);   // 0 - instruction is not completed   1 - instruction is completed
	SSPIF = 0;         
}

void i2c_ack(void)
{
	if (ACKSTAT)
	{
		/* Do debug print here if required */
	}
}

void i2c_start(void)
{
	SEN = 1;                 // to start the i2c communication
	i2c_idle();        // in each function we have to check whether the instruction is completed or not by calling i2c_ideal() function)
} 

void i2c_stop(void)
{
	PEN = 1;          // to stop the i2c commn we have to make PEN bit as 1
	i2c_idle();
}

void i2c_rep_start(void)
{
	RSEN = 1;             // to repeat start the communication
	i2c_idle();
}

void i2c_write(unsigned char data)     // to write the data data to i2c
{
	SSPBUF = data;             // we have to write out data to buffer register
	i2c_idle();
}

void i2c_rx_mode(void)         // to make i2c in receive mode
{
	RCEN = 1;                // make RCEN = 1
	i2c_idle();
}
  
void i2c_no_ack(void)       // to make no need ack 
{
	ACKDT = 1;
	ACKEN = 1;
}

unsigned char i2c_read(void)     // to read the data from i2c
{
	i2c_rx_mode();       // make receive mode
	i2c_no_ack();        // and making no ack mode 

	return SSPBUF;      // to return the data which is present in SBUFF register
}