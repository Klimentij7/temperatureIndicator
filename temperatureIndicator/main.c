/************************************************************************/
/* @Author	Kliments Krasovskis											*/
/* @Company	Daugavpils University										*/
/* @MCU		Atmega328P													*/
/*																		*/
/*				Used I2C LCD Display 16x2 and DHT-11 sensor             */
/*				 Signal wire from DHT-11 connected to PD5				*/
/*			   I2C display uses SDA and SCL pins (A4 and A5)			*/
/************************************************************************/

#define _LCD_FIRST_ROW          0b10000000
#define _LCD_SECOND_ROW         0b11000000

#define _LCD_CLEAR              0b00000001
#define _LCD_RETURN_HOME        0b00000002

#define _LCD_CURSOR_OFF         0b00001100
#define _LCD_UNDERLINE_ON       0b00001110
#define _LCD_BLINK_CURSOR_ON    0b00001111
#define _LCD_MOVE_CURSOR_LEFT   0b00010000
#define _LCD_MOVE_CURSOR_RIGHT  0b00010100
#define _LCD_TURN_ON            0b00001100
#define _LCD_TURN_OFF           0b00001000
#define _LCD_SHIFT_LEFT         0b00011000
#define _LCD_SHIFT_RIGHT        0b00011110

// LCD Definitions
#define LCD_ADDR 0b01001110

//CPU frequency 16MHz
#define F_CPU 16000000UL

//DHT Sensor pin on PD5
#define DHT_SENSOR 5

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>

uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;

void Request()
{
	DDRD |= (1<<DHT_SENSOR);
	PORTD &= ~(1<<DHT_SENSOR);
	_delay_ms(20);
	PORTD |= (1<<DHT_SENSOR);
}

void Response()
{
	DDRD &= ~(1<<DHT_SENSOR);
	while(PIND & (1<<DHT_SENSOR));
	while((PIND & (1<<DHT_SENSOR))==0);
	while(PIND & (1<<DHT_SENSOR));
}

uint8_t Receive_data()
{
	for (int q=0; q<8; q++)
	{
		while((PIND & (1<<DHT_SENSOR)) == 0);
		_delay_us(30);
		if(PIND & (1<<DHT_SENSOR))// if high pulse is greater than 30ms
		c = (c<<1)|(0x01);	// then its logic HIGH
		else			// otherwise its logic LOW
		c = (c<<1);
		while(PIND & (1<<DHT_SENSOR));
	}
	return c;
}

void TWI_Init(void)
{
	TWSR = 0x00;
	TWBR = 0x0C;
	TWCR = (1<<TWEN);
}

void TWI_Start(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR & (1<<TWINT)) == 0);
}

void TWI_Stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void TWI_Write(uint8_t u8data)
{
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR & (1<<TWINT)) == 0);
}

void I2C_LCD_Cmd(char out_char) {

	char hi_n, lo_n;
	char rs = 0x00;

	hi_n = out_char & 0b11110000;
	lo_n = (out_char << 4) & 0xF0;

	TWI_Start();
	TWI_Write(LCD_ADDR);
	TWI_Write(hi_n | rs | 0x04 | 0x08);
	_delay_us(50);
	TWI_Write(hi_n | rs | 0x00 | 0x08);
	_delay_us(100);
	TWI_Write(lo_n | rs | 0x04 | 0x08);
	_delay_us(50);
	TWI_Write(lo_n | rs | 0x00 | 0x08);
	TWI_Stop();

	if(out_char == 0x01)_delay_ms(2);
}

void I2C_LCD_Chr(char row, char column, char out_char) {

	char hi_n, lo_n;
	char rs = 0x01;

	switch(row){

		case 1:
		I2C_LCD_Cmd(0x80 + (column - 1));
		break;
		case 2:
		I2C_LCD_Cmd(0xC0 + (column - 1));
		break;
	};

	hi_n = out_char & 0xF0;
	lo_n = (out_char << 4) & 0xF0;

	TWI_Start();
	TWI_Write(LCD_ADDR);
	TWI_Write(hi_n | rs | 0x04 | 0x08);
	_delay_us(50);
	TWI_Write(hi_n | rs | 0x00 | 0x08);
	_delay_us(100);
	TWI_Write(lo_n | rs | 0x04 | 0x08);
	_delay_us(50);
	TWI_Write(lo_n | rs | 0x00 | 0x08);
	TWI_Stop();
}

void I2C_LCD_Chr_Cp(char out_char) {

	char hi_n, lo_n;
	char rs = 0x01;

	hi_n = out_char & 0xF0;
	lo_n = (out_char << 4) & 0xF0;


	TWI_Start();
	TWI_Write(LCD_ADDR);
	TWI_Write(hi_n | rs | 0x04 | 0x08);
	_delay_us(50);
	TWI_Write(hi_n | rs | 0x00 | 0x08);
	_delay_us(100);
	TWI_Write(lo_n | rs | 0x04 | 0x08);
	_delay_us(50);
	TWI_Write(lo_n | rs | 0x00 | 0x08);
	TWI_Stop();
}


void I2C_LCD_Init() {

	char rs = 0x00;

	TWI_Start();
	TWI_Write(LCD_ADDR);
	_delay_ms(30);

	TWI_Write(0x30 | rs | 0x04 | 0x08);
	_delay_us(50);
	TWI_Write(0x30 | rs | 0x00 | 0x08);
	_delay_ms(10);

	TWI_Write(0x30 | rs | 0x04 | 0x08);
	_delay_us(50);
	TWI_Write(0x30 | rs | 0x00 | 0x08);
	_delay_ms(10);

	TWI_Write(0x30 | rs | 0x04 | 0x08);
	_delay_us(50);
	TWI_Write(0x30 | rs | 0x00 | 0x08);
	_delay_ms(10);

	TWI_Write(0x20 | rs | 0x04 | 0x08);
	_delay_us(50);
	TWI_Write(0x20 | rs | 0x00 | 0x08);
	TWI_Stop();

	_delay_ms(10);

	I2C_LCD_Cmd(0x28);
	I2C_LCD_Cmd(0x06);
}

void I2C_LCD_Out(char row, char col, char *text) {
	while(*text)
	I2C_LCD_Chr(row, col++, *text++);
}

void I2C_LCD_Out_Cp(char *text) {
	while(*text)
	I2C_LCD_Chr_Cp(*text++);
}

void requestDht()
{
	DDRD |= (1<<DHT_SENSOR);
	PORTD &= ~(1<<DHT_SENSOR);
	_delay_ms(20);
	PORTD |= (1<<DHT_SENSOR);
}

void responseDht()
{
	DDRD &= ~(1<<DHT_SENSOR);
	while(PIND & (1<<DHT_SENSOR));
	while((PIND & (1<<DHT_SENSOR)) == 0);
	while(PIND & (1<<DHT_SENSOR));
}

int main()
{
	DDRC = 0x00;
	PORTC = 0x00;

	char data[5];
	TWI_Init();

	I2C_LCD_Init();
	I2C_LCD_Cmd(_LCD_CURSOR_OFF);
	I2C_LCD_Cmd(_LCD_CLEAR);

	while(1)
	{
		Request();
		Response();
		I_RH=Receive_data();
		D_RH=Receive_data();
		I_Temp=Receive_data();
		D_Temp=Receive_data();
		CheckSum=Receive_data();
		
		if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
		{
			I2C_LCD_Out(1,1,"Sensor ERROR");
		}
		
		else
		{			
			itoa(I_RH,data,10);
			I2C_LCD_Out(1,1,"Humidity: ");
			I2C_LCD_Out(1,11,data);
			I2C_LCD_Out(1,13,"%");

			itoa(I_Temp,data,10);
			I2C_LCD_Out(2,1,"Temperat: ");
			I2C_LCD_Out(2,11,data);
			I2C_LCD_Out(2,13,"C");
		}
		_delay_ms(400);
		I2C_LCD_Cmd(_LCD_CURSOR_OFF);
		I2C_LCD_Cmd(_LCD_CLEAR);
	}
}