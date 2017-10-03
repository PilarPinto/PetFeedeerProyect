
/*- Includes ---------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soc-hw.h"

/*- Definitions ------------------------------------------------------------*/
#ifndef UART_COMMANDS_BUFFER_SIZE
#define UART_COMMANDS_BUFFER_SIZE 100
#endif

#define FINALCHARACTER1 '\r'
#define FINALCHARACTER2 '\n'

/*- Variables --------------------------------------------------------------*/
char UartBuffer[UART_COMMANDS_BUFFER_SIZE];
uint32_t UartBufferPtr = 0;

char *itoa(int i, char b[])
{
    char const digit[] = "0123456789";
    char *p = b;
    if (i < 0)
    {
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do
    { //Move to where representation ends
        ++p;
        shifter = shifter / 10;
    } while (shifter);
    *p = '\0';
    do
    { //Move back, inserting digits as u go
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}

/*************************************************************************/ /**
Función que implementa una calculadora sencilla
*****************************************************************************/
void commandProcessing(const char *buffer)
{

    int rgbw = atoi(buffer);
    SK6812RGBW_rgbw(rgbw);

    return;
}

/*************************************************************************/ /**
*****************************************************************************/
void commandUart_TaskHandler(void)
{

    //lee un byte del buffer de la UART
    char byte_u8 = uart_getchar();

    //verifica que no se ha exedido el buffer de comandos UART_COMMANDS_BUFFER_SIZE
    //Si se exede se envia una alerta y se borran los datos del buffer
    if (UartBufferPtr >= UART_COMMANDS_BUFFER_SIZE)
    {
        uart_putstr("\r\nUartBuffer size exceeded:serial commands\r\n-->");
        UartBufferPtr = 0;
        return;
    }

    //si se recibe un FINALCHARACTER se procesa el comando, de lo contrario
    //se almacena el caracter si es un caracter imprimible
    if (byte_u8 == FINALCHARACTER1 || byte_u8 == FINALCHARACTER2)
    {
        UartBuffer[UartBufferPtr++] = '\0'; // null character manually added
        uart_putstr("\r\n");
        commandProcessing(UartBuffer);
        uart_putstr("-->");
        UartBufferPtr = 0;
    }
    else if (byte_u8 >= ' ' && byte_u8 <= '~')
    {
        UartBuffer[UartBufferPtr++] = byte_u8;
        //Se hace un echo de caracteres almacenados
        uart_putchar(byte_u8);
    }
}

int main(void)
{

    // Init Commands
    isr_init();
    //tic_init();
    irq_set_mask(0x00000002);
    irq_enable();

    uart_init();

    SK6812RGBW_init();


	

//i2c_addr(0x00000005);

//uint32_t dat = i2c_dataRd();
//uint32_t i =  bcd2bin (dat);
//SK6812RGBW_rgbw(0x0012faff);
//mSleep(2000);
/*
    uint32_t RGBW = 0x00ff0000;
    uint32_t i;
    mSleep(2000);
    for (i = 1; i <= 35; i++)
    {
        SK6812RGBW_nBits(i * 32); //i Leds
        SK6812RGBW_rgbw(RGBW);
        mSleep(1000);
        RGBW -= 0x70000;
        RGBW += 0x7000000;
    }
*/

i2c_addr(0x00000068);
i2c_rw(0x00000000);
i2c_dataWr(0x00000000);
i2c_enable(0x00000001);
//i2c_enable(0x00000000);


/*
    for (i = 0; i < 35; i++)
    {
        if (i >= 1 && i <= 6)
        {
            SK6812RGBW_ram(0x00ffff00, i);
        }
        else if (i >= 8 && i <= 13)
        {
            SK6812RGBW_ram(0x0000ff00, i);
        }
        else if (i >= 15 && i <= 20)
        {
            SK6812RGBW_ram(0xff00ff00, i);
        }
        else if (i >= 22 && i <= 27)
        {
            SK6812RGBW_ram(0xff000000, i);
        }
        else if (i >= 29 && i <= 34)
        {
            SK6812RGBW_ram(0xffff0000, i);
        }
        else
        {
            SK6812RGBW_ram(0x00ff0000, i);
        }
    }
    SK6812RGBW_source(1);
    SK6812RGBW_ram_w();
    SK6812RGBW_source(0);

*/

    while (1)
    {
        commandUart_TaskHandler();
    }

    return 0;
}
