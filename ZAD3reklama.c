#include "xc.h"
#include <libpic30.h>

#define FCY 5000000UL   

#define LCD_E       LATDbits.LATD4  
#define LCD_RW      LATDbits.LATD5
#define LCD_RS      LATBbits.LATB15
#define LCD_DATA    LATE

#define LCD_CLEAR       0x01    
#define LCD_HOME        0x02    
#define LCD_ON          0x0C    
#define LCD_OFF         0x08    
#define LCD_CONFIG      0x38    
#define LCD_CURSOR      0x80    
#define LINE1           0x00
#define LINE2           0x40
#define LCD_CUST_CHAR   0X40    
#define LCD_SHIFT_R     0x1D    
#define LCD_SHIFT_L     0x18 

void __delay_us(unsigned long us){
    __delay32(us*FCY/1000000);
}

void __delay_ms(unsigned long ms){
    __delay32(ms*FCY/1000);
}

void LCD_sendCommand(unsigned char command){
    LCD_RW = 0;     
    LCD_RS = 0;     
    LCD_E = 1;      
    LCD_DATA = command;
    __delay_us(50); 
    LCD_E = 0;      
}

void LCD_sendData(unsigned char data){
    LCD_RW = 0;
    LCD_RS = 1;     
    LCD_E = 1;
    LCD_DATA = data;
    __delay_us(50);
    LCD_E = 0;
}

void LCD_print(unsigned char* string){
    while(*string){
        LCD_sendData(*string++);
    }
}

void LCD_setCursor(unsigned char row, unsigned char col){
    unsigned char address;
    if(row==0){
        address = LCD_CURSOR + LINE1 + col;
    }
    if(row==1){
        address = LCD_CURSOR + LINE2 + col;
    }
    LCD_sendCommand(address);
}

void LCD_saveCustChar(unsigned char slot, unsigned char *array) {
    unsigned char i;
    LCD_sendCommand(LCD_CUST_CHAR + (slot*8));
    for(i=0;i<8;i++){
        LCD_sendData(array[i]);
    }
}

void LCD_init(){
    __delay_ms(20);
    LCD_sendCommand(LCD_CONFIG);
    __delay_us(50);     
    LCD_sendCommand(LCD_ON);
    __delay_us(50);
    LCD_sendCommand(LCD_CLEAR);
    __delay_ms(2);
}

unsigned char arrow1[8] = {
    0b00000,
    0b00100,
    0b00010,
    0b11111,
    0b00010,
    0b00100,
    0b00000,
    0b00000
};

unsigned char smile1[8] = {
    0b00000,
    0b01010,
    0b00000,
    0b11111,
    0b10001,
    0b01110,
    0b00000,
    0b00000
};
unsigned char arrow2[8] = {
    0b00000,
    0b00100,
    0b01000,
    0b11111,
    0b01000,
    0b00100,
    0b00000,
    0b00000
};
unsigned char smile2[8] = {
    0b00000,
    0b01010,
    0b00000,
    0b11111,
    0b10001,
    0b01110,
    0b00000,
    0b00000
};

void displayAnimation(unsigned char *arrow1, unsigned char *arrow2,unsigned char *smile1,unsigned char *smile2, int duration) {
    int i;
    for (i = 0; i < duration; i++) {
        LCD_setCursor(1, 3);
        LCD_sendData(0); 
        LCD_setCursor(1, 12);
        LCD_sendData(1); 
        __delay_ms(100);
        LCD_setCursor(1, 3);
        LCD_sendData(2); 
        LCD_setCursor(1, 12);
        LCD_sendData(3); 
        __delay_ms(100); 
    }
}

int main(void) {
    TRISB = 0x7FFF;
    TRISD = 0x0000;
    TRISE = 0x0000;

    LCD_init();
    LCD_saveCustChar(0, arrow1);
    LCD_saveCustChar(1, arrow2);
    LCD_saveCustChar(2, smile1);
    LCD_saveCustChar(3, smile2);

    LCD_setCursor(0, 1); 
    LCD_print("KUP PIWO MARKI"); 
    LCD_setCursor(1, 5); 
    LCD_print("BROWAR"); 

    __delay_ms(250);

    displayAnimation(arrow1, arrow2, smile1,smile2, 1); 
    

    return 0;
}
