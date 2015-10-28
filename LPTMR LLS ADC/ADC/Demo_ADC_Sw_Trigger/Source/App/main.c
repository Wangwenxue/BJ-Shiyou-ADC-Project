/****************************************Copyright (c)****************************************************
**               Copyright © 2003~2009 Shenzhen uCdragon Technology Co.,Ltd. All Rights Reserved 
**
**                                 http://www.ucdragon.com
**
**      ÉîÛÚÊÐÓÅÁú¿Æ¼¼ÓÐÏÞ¹«Ë¾ËùÌá¹©µÄËùÓÐ·þÎñÄÚÈÝÖ¼ÔÚÐ­Öú¿Í»§¼ÓËÙ²úÆ·µÄÑÐ·¢½ø¶È£¬ÔÚ·þÎñ¹ý³ÌÖÐËùÌá¹©
**  µÄÈÎºÎ³ÌÐò¡¢ÎÄµµ¡¢²âÊÔ½á¹û¡¢·½°¸¡¢Ö§³ÖµÈ×ÊÁÏºÍÐÅÏ¢£¬¶¼½ö¹©²Î¿¼£¬¿Í»§ÓÐÈ¨²»Ê¹ÓÃ»ò×ÔÐÐ²Î¿¼ÐÞ¸Ä£¬±¾¹«Ë¾²»
**  Ìá¹©ÈÎºÎµÄÍêÕûÐÔ¡¢¿É¿¿ÐÔµÈ±£Ö¤£¬ÈôÔÚ¿Í»§Ê¹ÓÃ¹ý³ÌÖÐÒòÈÎºÎÔ­ÒòÔì³ÉµÄÌØ±ðµÄ¡¢Å¼È»µÄ»ò¼ä½ÓµÄËðÊ§£¬±¾¹«Ë¾²»
**  ³Ðµ£ÈÎºÎÔðÈÎ¡£
**                                                                        ¡ªÉîÛÚÊÐÓÅÁú¿Æ¼¼ÓÐÏÞ¹«Ë¾
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               Main.c
** Last modified date:      
** Last version:            V1.0
** Description:             Ö÷º¯ÊýÎÄ¼þ
**
--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
*********************************************************************************************************/
#include "includes.h"
#include "stdio.h"

/*********************************************************************************************************/
#define LLWU_PIN_DIS 0
#define LLWU_PIN_RISING 1
#define LLWU_PIN_FALLING 2
#define LLWU_PIN_ANY 3
#define IO_NMI_DEF 1  // 1 - sets the Port interrupt, 0 - sets the NMI interrupt
#ifdef  ON
#undef  ON
#endif
#define ON      (1)

#ifdef  OFF
#undef  OFF
#endif
#define OFF     (0)

// MCG Mode defines
#define BLPI 1
#define FBI  2
#define FEI  3
#define FEE  4
#define FBE  5
#define BLPE 6
#define PBE  7
#define PEE  8

#define CLK0_FREQ_HZ        8000000
int uart0_clk_khz;

void llwu_configure(unsigned int pin_en, unsigned char rise_fall, unsigned char module_en );
void LLWU_Init(void);
void Port_Init();
unsigned char what_mcg_mode(void);
#define TERM_PORT_NUM       0
void clockMonitor(unsigned char state);
void enter_lls(void);
void set_4_lp(unsigned char outof_lp);
void Enter_Lowpower(void);

/*********************************************************************************************************/


/*********************************************************************************************************
** Function name:           myDelay
** Descriptions:            Çý¶¯GPIO ·­×ªDemo
** input parameters:        ulTime:ÑÓÊ±Ê±¼ä
** output parameters:       none
** Returned value:          none             

*********************************************************************************************************/
void myDelay (INT32U ulTime)
{
    INT32U i;

    i = 0;
    while (ulTime--) {
        for (i = 0; i < 5000; i++);
    }
}
/*********************************************************************************************************
** Function name:           main
** Descriptions:            Èí¼þ´¥·¢16Î»µ¥¶ËADC²É¼¯£¬Ê¹ÓÃPE20¹Ü½ÅÊäÈë£¬²Î¿¼Ô´Ñ¡ÔñµçÔ´,Ê¹ÓÃ²éÑ¯µÄ·½Ê½¶ÁÈ¡ADC
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              

*********************************************************************************************************/
int  main (void)
{
    INT16U Vout = 0;
	
		SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK ); 
	
    SystemCoreClockUpdate();
	
	  OSC0_CR |= OSC_CR_EREFSTEN_MASK;                                      /* wenxue  µÍ¹¦ºÄÏÂCLK¼ÌÐø¹¤×÷  */
	
    uart0Init(115200,0,0,8,1);                                            /* UART0³õÊ¼»¯                 */
	
    printf("\r\n ADC Test Start \r\n"); 
	
		adcInit();                                                          /* ³õÊ¼»¯adc                    */
	
	  LLWU_Init();                                                       /* LLWU Init                    */
	   
	  LPTMR_init(250,LPTMR_USE_OSCERCLK);  // 2ms
	 // LPTMR_init(250000,LPTMR_USE_OSCERCLK);  // 2s	 
	  NVIC_EnableIRQ(LPTimer_IRQn); // wenxue
	 
   // For Test
   //	  printf("Enter in LLS mode \r\n");
   //		Enter_Lowpower(  );
   //	  printf("Back to Normal \r\n");
	
	
    while (1) {
        Vout = adcGetUint();                                            /* »ñÈ¡adcÖµ                    */
        printf("\r\n Vout=%d \r\n",Vout);
			 	//myDelay(100);
			  Enter_Lowpower();
			  
    }
}




//Interrupt handler for LLWU 
#ifdef CMSIS
void LLW_IRQHandler(void) {
#else
void llwu_isr(void){
#endif
   //printf("\n [LLWU ISR] "); 
   if (LLWU_F1 & LLWU_F1_WUF5_MASK) {
   //    printf("****WUF5 was set *****\r\n"); 
       LLWU_F1 |= LLWU_F1_WUF5_MASK;   // write one to clear the flag
   }
   if (LLWU_F1 & LLWU_F1_WUF6_MASK) {
   //    printf("****WUF6 was set *****\r\n"); 
       LLWU_F1 |= LLWU_F1_WUF6_MASK;   // write one to clear the flag
    }
   if (LLWU_F1 & LLWU_F1_WUF7_MASK) {
   //    printf("****WUF7 was set from PTC3 input  *****\r\n"); 
       LLWU_F1 |= LLWU_F1_WUF7_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF8_MASK) {
   //    printf("****WUF8 was set *****\r\n"); 
       LLWU_F2 |= LLWU_F2_WUF8_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF9_MASK) {
   //    printf("****WUF9 was set *****\r\n"); 
       LLWU_F2 |= LLWU_F2_WUF9_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF10_MASK) {
   //    printf("****WUF10 was set *****\r\n"); 
       LLWU_F2 |= LLWU_F2_WUF10_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF11_MASK) {
   //    printf("****WUF11 was set *****\r\n"); 
       LLWU_F2 |= LLWU_F2_WUF11_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF12_MASK) {
   //    printf("****WUF12 was set *****\r\n"); 
       LLWU_F2 |= LLWU_F2_WUF12_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF13_MASK) {
   //    printf("****WUF13 was set *****\r\n"); 
       LLWU_F2 |= LLWU_F2_WUF13_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF14_MASK) {
   //    printf("****WUF14 was set *****\r\n"); 
       LLWU_F2 |= LLWU_F2_WUF14_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF15_MASK) {
   //    printf("****WUF15 was set *****\r\n"); 
       LLWU_F2 |= LLWU_F2_WUF15_MASK;   // write one to clear the flag
   }
   
   /************************************************************************
    * Note: This ISR does not write to the LLWU_F3 register because these
    * are peripheral module wakeups.  The flags contained in the LLWU_F3 
    * register should be cleared through the associated module interrupt 
    * and not through the LLWU_F3 per the Kinetis L Family Reference
    * Manual (LLWU Chapter)
    **********************************************************************/
  if (LLWU_F3 & LLWU_F3_MWUF0_MASK) {
    //   printf("****WUF3_MWUF0 IF  LPTMR  *****\r\n"); 
         SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
         LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   // write 1 to TCF to clear the LPT timer compare flag
         LPTMR0_CSR = ( LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK  );
   }
   if(LLWU_FILT1 & LLWU_FILT1_FILTF_MASK){
	   
	   LLWU_FILT1 |= LLWU_FILT1_FILTF_MASK;
   }
   if(LLWU_FILT2 & LLWU_FILT2_FILTF_MASK){
	   
	   LLWU_FILT2 |= LLWU_FILT2_FILTF_MASK;
   }
//   NVIC_ICPR |= 1 << (LLWU_irq_no%32);
	 NVIC_ICPR |= 1 << (LLW_IRQn%32);	 // CMSIS 
}



/* LLS µÍ¹¦ºÄÄ£Ê½Ïà¹Ø */
/*******************************************************************************************************/
void LLWU_Init(void)
{
	/*Enable all operation modes because this is a write once register*/  
  SMC_PMPROT =  SMC_PMPROT_AVLLS_MASK |
                SMC_PMPROT_ALLS_MASK  |    
                SMC_PMPROT_AVLP_MASK;
	
	
	#ifdef CMSIS
	  NVIC_EnableIRQ(LLW_IRQn);
	#else
    enable_irq(LLWU_irq_no);
	#endif
	
	// llwu_configure(0x0400/*PTC6*/, LLWU_PIN_RISING, 0x1);  
	llwu_configure(0x0000, LLWU_PIN_FALLING, 0x1); // wenxue LPTMR
}		


/* function: llwu_configure

   description: Set up the LLWU for wakeup the MCU from LLS and VLLSx modes 
   from the selected pin or module.
   
   inputs:
   pin_en - unsigned integer, bit position indicates the pin is enabled.  
            More than one bit can be set to enable more than one pin at a time.  
   
   rise_fall - 0x00 = External input disabled as wakeup
               0x01 - External input enabled as rising edge detection
               0x02 - External input enabled as falling edge detection
               0x03 - External input enablge as any edge detection
   module_en - unsigned char, bit position indicates the module is enabled.  
               More than one bit can be set to enabled more than one module                   
   
   for example:  if bit 0 and 1 need to be enabled as rising edge detect call this  routine with
   pin_en = 0x0003 and rise_fall = 0x02
   
   Note: to set up one set of pins for rising and another for falling, 2 calls to this 
         function are required, 1st for rising then the second for falling.
   
*/

void llwu_configure(unsigned int pin_en, unsigned char rise_fall, unsigned char module_en ) {
    uint8 temp;
    
    temp = LLWU_PE1;
    if( pin_en & 0x0001)
    {      
        printf("\n Invalid LLWU configured pin PTE1/SCI1_RX/I2C1_SCL /SPI1_SIN");      
    }
    if( pin_en & 0x0002)
    {      
        printf("\n Invalid LLWU configured pin PTE2/SPI1_SCK/SDHC0_DCLK");       
    }
    if( pin_en & 0x0004)
    {
        printf("\n Invalid LLWU configured pin PTE4/SPI1_PCS0/SDHC0_D3");
    }
    if( pin_en & 0x0008)
    {
        printf("\n Invalid LLWU configured pin PTA4/FTM0_CH1/NMI/EZP_CS");
    }
    LLWU_PE1 = temp;

    temp = LLWU_PE2;
    if( pin_en & 0x0010)
    {
        printf("\n Invalid LLWU configured pin PTA13/FTM1_CH1 /FTM1_QD_PHB");
    }
    if( pin_en & 0x0020)
    {
        temp |= LLWU_PE2_WUPE5(rise_fall);
        printf("\n LLWU configured pins PTB0/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA is LLWU wakeup source ");
        LLWU_F1 |= LLWU_F1_WUF5_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0040)
    {
        temp |= LLWU_PE2_WUPE6(rise_fall);
        printf("\n LLWU configured pins PTC1/SCI1_RTS/FTM0_CH0 is LLWU wakeup source ");
        LLWU_F1 |= LLWU_F1_WUF6_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0080)
    {
        temp |= LLWU_PE2_WUPE7(rise_fall);
        printf("\n LLWU configured pins PTC3/SCI1_RX/FTM0_CH2 is LLWU wakeup source ");
        LLWU_F1 |= LLWU_F1_WUF7_MASK;   // write one to clear the flag
    }
    LLWU_PE2 = temp;

    temp = LLWU_PE3;
    if( pin_en & 0x0100)
    {
        temp |= LLWU_PE3_WUPE8(rise_fall);
        printf("\n LLWU configured pins PTC4/SPI0_PCS0/FTM0_CH3 is LLWU wakeup source ");
        LLWU_F2 |= LLWU_F2_WUF8_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0200)
    {
        temp |= LLWU_PE3_WUPE9(rise_fall);
        printf("\n LLWU configured pins PTC5/SPI0_SCK/I2S0_RXD0 is LLWU wakeup source ");
        LLWU_F2 |= LLWU_F2_WUF9_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0400)
    {
        temp |= LLWU_PE3_WUPE10(rise_fall);
        printf("\n LLWU configured pins PTC6/PDB0_EXTRG to be an LLWU wakeup source "); // wenxue
        LLWU_F2 |= LLWU_F2_WUF10_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0800)
    {
        temp |= LLWU_PE3_WUPE11(rise_fall);
        printf("\n LLWU configured pins PTC11/I2S0_RXD1 to be an LLWU wakeup source ");
        LLWU_F2 |= LLWU_F2_WUF11_MASK;   // write one to clear the flag
    }
    LLWU_PE3 = temp;

    temp = LLWU_PE4;
    if( pin_en & 0x1000)
    {
        temp |= LLWU_PE4_WUPE12(rise_fall);
        printf("\n LLWU configured pins PTD0/SPI0_PCS0/SCI2_RTS to be an LLWU wakeup source ");
        LLWU_F2 |= LLWU_F2_WUF12_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x2000)
    {
        temp |= LLWU_PE4_WUPE13(rise_fall);
        printf("\n LLWU configured pins PTD2/SCI2_RX to be an LLWU wakeup source ");
        LLWU_F2 |= LLWU_F2_WUF13_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x4000)
    {
        temp |= LLWU_PE4_WUPE14(rise_fall);
        printf("\n LLWU configured pins PTD4/SCI0_RTS/FTM0_CH4/EWM_IN is LLWU wakeup source ");
        LLWU_F2 |= LLWU_F2_WUF14_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x8000)
    {
        temp |= LLWU_PE4_WUPE15(rise_fall);
        printf("\n LLWU configured pins PTD6/SCI0_RX/FTM0_CH6/FTM0_FLT0 is LLWU wakeup source ");
        LLWU_F2 |= LLWU_F2_WUF15_MASK;   // write one to clear the flag
    }
    LLWU_PE4 = temp;
    if (module_en == 0){
      LLWU_ME = 0;
    }else  {
    LLWU_ME |= module_en;  //Set up more modules to wakeup up
    printf("\n\r LLWU configured modules as LLWU wakeup sources = 0x%02X,",(LLWU_ME));  // wenxue  2

    }
} // End LLWU Configuration



void Port_Init(int io_nmi){
 
  /*Configure SW1 for LLWU & GPIO IRQ */
  // PORTC_PCR3 =  PORT_PCR_PS_MASK |
	   PORTC_PCR6 =  	PORT_PCR_PE_MASK | 
										//PORT_PCR_PS_MASK|//  ÏÂÀ­
										PORT_PCR_PFE_MASK|
                   PORT_PCR_MUX(1);  
}




void Enter_Lowpower(void)
{
	unsigned char op_mode,fcrdiv_val;
	int mcg_clk_hz;
	
  clockMonitor(OFF); 
	//printf("Enter LLS start");
  set_4_lp(0);
	enter_lls( );
	set_4_lp(1);
	/*After LLS wake up that was enter from PEE the exit will be on PBE */ 
	/*  for this reason after wake up make sure to get back to desired  */
	/*  clock mode                                                      */
	op_mode = what_mcg_mode();
	if(op_mode==PBE)
	{
			mcg_clk_hz = pbe_pee(CLK0_FREQ_HZ);
	    uart0Init(115200,0,0,8,1);
	}
  

}



/*******************************************************************************************************/
void clockMonitor(unsigned char state)
{
    if(state)
      MCG_C6 |= MCG_C6_CME0_MASK;
    else
      MCG_C6 &= ~MCG_C6_CME0_MASK;
}

/**********************************************************/
/* outof_lp = 0 for going into low power mode                */
/* outof_lp = 1 for exiting low power mode                   */
/**********************************************************/

void set_4_lp(unsigned char outof_lp)
{
    if(outof_lp)
    {
												if (TERM_PORT_NUM == 0)
												{
													 SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
											 /* Enable the pins for the selected SCI */
													 /* Enable the SCI1_TXD function on PTA1 */
											 PORTA_PCR1 = PORT_PCR_MUX(0x2); // LPSCI is alt3 function for this pin
											
											 /* Enable the SCI1_RXD function on PTA2 */
											 PORTA_PCR2 = PORT_PCR_MUX(0x2); // LPSCI is alt3 function for this pin
													
													 SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); // select the PLLFLLCLK as UART0 clock source
												}
										if (TERM_PORT_NUM == 1)
										{
													 SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
													 /* Enable the SCI1_TXD function on PTC4 */
													 PORTC_PCR4 = PORT_PCR_MUX(0x3); // SCI is alt3 function for this pin
											
											 /* Enable the SCI1_RXD function on PTC3 */
											 PORTC_PCR3 = PORT_PCR_MUX(0x3); // SCI is alt3 function for this pin
										}
												
												if (TERM_PORT_NUM == 2)
										{
														SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
														/* Enable the SCI2_TXD function on PTD3 */
												PORTD_PCR3 = PORT_PCR_MUX(0x3); // SCI is alt3 function for this pin
											
												/* Enable the SCI2_RXD function on PTD2 */
												PORTD_PCR2 = PORT_PCR_MUX(0x3); // SCI is alt3 function for this pin
										}
											PORTA_PCR0 =  PORT_PCR_MUX(7); // Enable Debug 
											PORTA_PCR3 =  PORT_PCR_MUX(7); // Enable Debug  
											// re-init ports

											if (IO_NMI_DEF){
											/*Configure SW1 for LLWU & GPIO IRQ*/
													 PORTC_PCR6 =  	PORT_PCR_PE_MASK | 
																		//PORT_PCR_PS_MASK|
																		PORT_PCR_PFE_MASK|
																	 PORT_PCR_MUX(1);
											}else {
								//           enable_NMI_button();
											}

										
    }else  // outof_lp = 0 for going into low power mode -  turn things off
    {
										if (TERM_PORT_NUM == 0){
											 /* clock gate off the UART */
							#ifndef NOT_LOWEST_PWR_2
											 SIM_SCGC4 &= ~SIM_SCGC4_UART0_MASK;
							#endif
											 PORTA_PCR1 = PORT_PCR_MUX(0x0); // Analog is alt0 function for this pin
											 PORTA_PCR2 = PORT_PCR_MUX(0x0); // Analog is alt0 function for this pin}
									 
										} else if (TERM_PORT_NUM == 1) {
											 /* clock gate off the UART */
							#ifndef NOT_LOWEST_PWR_2
											 SIM_SCGC4 &= ~SIM_SCGC4_UART1_MASK;
							#endif
											 PORTC_PCR4 = PORT_PCR_MUX(0x0);
											 PORTC_PCR3 = PORT_PCR_MUX(0x0);
										} else if (TERM_PORT_NUM == 2) {
											 /* clock gate off the UART */
							#ifndef NOT_LOWEST_PWR_2
											 SIM_SCGC4 &= ~SIM_SCGC4_UART2_MASK;
							#endif
											 PORTD_PCR3 = PORT_PCR_MUX(0x0);
											 PORTD_PCR2 = PORT_PCR_MUX(0x0);
										}
										PORTA_PCR4 = PORT_PCR_MUX(0); 
							#ifndef NOT_LOWEST_PWR
							//      PORTA_PCR0 =  PORT_PCR_MUX(0); // Disable Debug   // Norman
							//      PORTA_PCR3 =  PORT_PCR_MUX(0); // Disable Debug  
							#endif
   }
}


/****************************************************************/
/* LLS mode entry routine. Puts the processor into LLS mode from
 * normal run mode or VLPR. 
 *
 * Mode transitions:
 * RUN -> LLS
 * VLPR -> LLS
 *
 * NOTE: LLS mode will always exit to RUN mode even if you were 
 * in VLPR mode before entering LLS.
 *
 * Wakeup from LLS mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in LLS mode, so make
 * sure to setup the desired wakeup sources in the LLWU before 
 * calling this function.
 *
 * Parameters:
 * none
 */
 /********************************************************************/

void enter_lls(void)
{
  volatile unsigned int dummyread;
  /* Write to PMPROT to allow LLS power modes this write-once 
     bit allows the MCU to enter the LLS low power mode*/
  SMC_PMPROT = SMC_PMPROT_ALLS_MASK;   
  /* Set the STOPM field to 0b011 for LLS mode  */
  SMC_PMCTRL &= ~SMC_PMCTRL_STOPM_MASK; 
  SMC_PMCTRL |=  SMC_PMCTRL_STOPM(0x3); 
  /*wait for write to complete to SMC before stopping core */  
  dummyread = SMC_PMCTRL;
  dummyread = dummyread + 1;
  /* Now execute the stop instruction to go into LLS */
  #ifdef CMSIS
  /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
  SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK;
  __wfi();
#else
  stop();
#endif
}

int pbe_pee(int crystal_val)
{
  unsigned char prdiv, vdiv;
  short i;

  // Check MCG is in PBE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (MCG_S & MCG_S_PLLST_MASK) &&                                   // check PLLS mux has selected PLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check MCG_C2[LP] bit is not set
  {
    return 0x7;                                                       // return error code
  }

  // As the PLL settings have already been checked when PBE mode was enterred they are not checked here

  // Check the PLL state before transitioning to PEE mode
  
  // Check LOCK bit is set before transitioning MCG to PLL output (already checked in fbe_pbe but good practice
  // to re-check before switch to use PLL)
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_LOCK0_MASK) break; // jump out early if LOCK sets before loop finishes
  }
  if (!(MCG_S & MCG_S_LOCK0_MASK)) return 0x44; // check bit is really set and return with error if not set
  // Use actual PLL settings to calculate PLL frequency
  prdiv = ((MCG_C5 & MCG_C5_PRDIV0_MASK) + 1);
  vdiv = ((MCG_C6 & MCG_C6_VDIV0_MASK) + 24);
  
  MCG_C1 &= ~MCG_C1_CLKS_MASK; // clear CLKS to switch CLKS mux to select PLL as MCG_OUT

  // Wait for clock status bits to update
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) break; // jump out early if CLKST = 3 before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) return 0x1B; // check CLKST is set correctly and return with error if not

  // Now in PEE
  return ((crystal_val / prdiv) * vdiv); //MCGOUT equals PLL output frequency
  
}  // pbe_pee


unsigned char what_mcg_mode(void)
{
  // check if in FEI mode
  if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) &&      // check CLKS mux has selcted FLL output
      (MCG_S & MCG_S_IREFST_MASK) &&                                     // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)))                                     // check PLLS mux has selected FLL
  {
    return FEI;                                                          // return FEI code
  }
  // Check MCG is in PEE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) && // check CLKS mux has selcted PLL output
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (MCG_S & MCG_S_PLLST_MASK))                                    // check PLLS mux has selected PLL 
  {
    return PEE;                                                          // return PEE code
  }
  // Check MCG is in PBE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (MCG_S & MCG_S_PLLST_MASK) &&                                  // check PLLS mux has selected PLL
          (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check MCG_C2[LP] bit is not set
  {
    return PBE;                                                          // return PBE code
  }
  // Check MCG is in FBE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
          (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check MCG_C2[LP] bit is not set   
  {
    return FBE;                                                          // return FBE code
  }
  // Check MCG is in BLPE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (MCG_C2 & MCG_C2_LP_MASK))                                     // check MCG_C2[LP] bit is set   
  {
    return BLPE;                                                         // return BLPE code
  }
  // check if in BLPI mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
          (MCG_S & MCG_S_IREFST_MASK) &&                                 // check FLL ref is internal ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
          (MCG_C2 & MCG_C2_LP_MASK))                                     // check LP bit is set
  {
    return BLPI;                                                         // return BLPI code
  }
  // check if in FBI mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
          (MCG_S & MCG_S_IREFST_MASK) &&                                 // check FLL ref is internal ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
          (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check LP bit is clear
  {  
    return FBI;                                                          // return FBI code 
  }
  // Check MCG is in FEE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)))                                 // check PLLS mux has selected FLL
  {
    return FEE;                                                          // return FEE code
  }
  else
  {
    return 0;                                                            // error condition
  }
} // what_mcg_mode





/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
