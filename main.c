#include <driverlib.h>
#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include "temp.h"
#include "ultrasonic.h"


#define SLAVE_ADDRESS 0x3

void show_value(unsigned char value);

void main(void)
{


    uint16_t dist = 0;
    char kiori[20];
    char str[20];


    WDT_A_hold(WDT_A_BASE);

    //Set DCO frequency to 1MHz
    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_0);
    //Set ACLK = VLO with frequency divider of 1
    CS_initClockSignal(CS_ACLK,CS_VLOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    //Set SMCLK = DCO with frequency divider of 1
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    //Set MCLK = DCO with frequency divider of 1
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);

    // Configure Pins for I2C
    //Set P1.6 and P1.7 as Secondary Module Function Input.
    /*

    * Select Port 1
    * Set Pin 6, 7 to input Secondary Module Function, (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
    */
    GPIO_setAsPeripheralModuleFunctionInputPin(
    	GPIO_PORT_P1,
    	GPIO_PIN6 + GPIO_PIN7,
    	GPIO_SECONDARY_MODULE_FUNCTION
    );

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();

    EUSCI_B_I2C_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = CS_getSMCLK();
    param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
    param.byteCounterThreshold = 0;
    param.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;
    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

    //Specify slave address
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE,
    			SLAVE_ADDRESS
    			);

    //Set Master in receive mode
    EUSCI_B_I2C_setMode(EUSCI_B0_BASE,
    			EUSCI_B_I2C_TRANSMIT_MODE
    			);

    //Enable I2C Module to start operations
    EUSCI_B_I2C_enable(EUSCI_B0_BASE);


    LCD_init();
    LCD_clear_home();

    UltrasonicInit(); //initialize ultrasonic module


    while(1)
    {
        dist = UltrasonicGetDistance();
        snprintf(kiori, sizeof(kiori), "Distanta: %d", dist);
        sprintf(str,"%d",dist);
        LCD_goto(1, 0);
        LCD_putstr(kiori);
        LCD_goto(1, 1);
        LCD_putstr(str);

        __delay_cycles(100);
    };
}
/*
void show_value(unsigned char value)
{
   unsigned char ch = 0x00;

   ch = ((value / 100) + 0x30);
   LCD_goto(6, 1);
   LCD_putchar(ch);

   ch = (((value / 10) % 10) + 0x30);
   LCD_goto(7, 1);
   LCD_putchar(ch);

   ch = ((value % 10) + 0x30);
   LCD_goto(8, 1);
   LCD_putchar(ch);
}
*/




