#define _LCD_FIRST_ROW          0x80     //Move cursor to the 1st row
#define _LCD_SECOND_ROW         0xC0     //Move cursor to the 2nd row
#define _LCD_THIRD_ROW          0x94     //Move cursor to the 3rd row
#define _LCD_FOURTH_ROW         0xD4     //Move cursor to the 4th row
#define _LCD_CLEAR              0x01     //Clear display
#define _LCD_RETURN_HOME        0x02     //Return cursor to home position, returns a shifted display to
                                         //its original position. Display data RAM is unaffected.
#define _LCD_CURSOR_OFF         0x0C     //Turn off cursor
#define _LCD_UNDERLINE_ON       0x0E     //Underline cursor on
#define _LCD_BLINK_CURSOR_ON    0x0F     //Blink cursor on
#define _LCD_MOVE_CURSOR_LEFT   0x10     //Move cursor left without changing display data RAM
#define _LCD_MOVE_CURSOR_RIGHT  0x14     //Move cursor right without changing display data RAM
#define _LCD_TURN_ON            0x0C     //Turn Lcd display on
#define _LCD_TURN_OFF           0x08     //Turn Lcd display off
#define _LCD_SHIFT_LEFT         0x18     //Shift display left without changing display data RAM
#define _LCD_SHIFT_RIGHT        0x1E     //Shift display right without changing display data RAM

// LCD Definitions
#define LCD_ADDR 0x4E

// Lcd constants
char txt1[] = "TWI LCD";
char txt2[] = "16x2, 20x2, 20x4";
char txt3[] = "ATMega8";
char txt4[] = "4 MHz";

void I2C_LCD_Cmd(char out_char) {

    char hi_n, lo_n;
    char rs = 0x00;

    hi_n = out_char & 0xF0;
    lo_n = (out_char << 4) & 0xF0;

    TWI_Start();
    TWI_Write(LCD_ADDR);
    TWI_Write(hi_n | rs | 0x04 | 0x08);
    Delay_us(50);
    TWI_Write(hi_n | rs | 0x00 | 0x08);
    Delay_us(100);
    TWI_Write(lo_n | rs | 0x04 | 0x08);
    Delay_us(50);
    TWI_Write(lo_n | rs | 0x00 | 0x08);
    TWI_stop();

    if(out_char == 0x01)Delay_ms(2);
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
        case 3:
        I2C_LCD_Cmd(0x94 + (column - 1));
        break;
        case 4:
        I2C_LCD_Cmd(0xD4 + (column - 1));
        break;
    };

    hi_n = out_char & 0xF0;
    lo_n = (out_char << 4) & 0xF0;

    TWI_Start();
    TWI_Write(LCD_ADDR);
    TWI_Write(hi_n | rs | 0x04 | 0x08);
    Delay_us(50);
    TWI_Write(hi_n | rs | 0x00 | 0x08);
    Delay_us(100);
    TWI_Write(lo_n | rs | 0x04 | 0x08);
    Delay_us(50);
    TWI_Write(lo_n | rs | 0x00 | 0x08);
    TWI_stop();
}

void I2C_LCD_Chr_Cp(char out_char) {

    char hi_n, lo_n;
    char rs = 0x01;

    hi_n = out_char & 0xF0;
    lo_n = (out_char << 4) & 0xF0;

    TWI_Start();
    TWI_Write(LCD_ADDR);
    TWI_Write(hi_n | rs | 0x04 | 0x08);
    Delay_us(50);
    TWI_Write(hi_n | rs | 0x00 | 0x08);
    Delay_us(100);
    TWI_Write(lo_n | rs | 0x04 | 0x08);
    Delay_us(50);
    TWI_Write(lo_n | rs | 0x00 | 0x08);
    TWI_stop();
}


void I2C_LCD_Init() {

    char rs = 0x00;

    TWI_Start();
    TWI_Write(LCD_ADDR);
    Delay_ms(30);

    TWI_Write(0x30 | rs | 0x04 | 0x08);
    Delay_us(50);
    TWI_Write(0x30 | rs | 0x00 | 0x08);
    Delay_ms(10);

    TWI_Write(0x30 | rs | 0x04 | 0x08);
    Delay_us(50);
    TWI_Write(0x30 | rs | 0x00 | 0x08);
    Delay_ms(10);

    TWI_Write(0x30 | rs | 0x04 | 0x08);
    Delay_us(50);
    TWI_Write(0x30 | rs | 0x00 | 0x08);
    Delay_ms(10);

    TWI_Write(0x20 | rs | 0x04 | 0x08);
    Delay_us(50);
    TWI_Write(0x20 | rs | 0x00 | 0x08);
    TWI_Stop();

    Delay_ms(10);

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

void main() {

    DDRC = 0x00;
    PORTC = 0x00;

    TWI_Init(100000);

    I2C_LCD_Init();
    I2C_LCD_Cmd(_LCD_CURSOR_OFF);
    I2C_LCD_Cmd(_LCD_CLEAR);
    I2C_Lcd_Out(1,1,txt1);                 // Write text in first row
    I2C_Lcd_Out(2,1,txt2);                 // Write text in second row
    I2C_Lcd_Out(3,1,txt3);                 // Write text in third row
    I2C_Lcd_Out(4,1,txt4);                 // Write text in fourth row

    while(1) {

    }
}