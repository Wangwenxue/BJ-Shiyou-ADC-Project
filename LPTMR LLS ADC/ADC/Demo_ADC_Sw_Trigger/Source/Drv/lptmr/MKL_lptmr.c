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
** File name:               MKL_lptmr.c
** Last modified date:      
** Last version:            V1.0
** Descriptions:            lptmrÇý¶¯³ÌÐò
**
**--------------------------------------------------------------------------------------------------------
** Created by:             
** Created date 
** Version:                 
** Descriptions:            lptmr³õÊ¼»¯ ³õÊ¼°æ±¾
**--------------------------------------------------------------------------------------------------------
** Modified by:        
** Modified date:      
** Version:            
** Descriptions:       
**
*********************************************************************************************************/
#include "..\..\App\includes.h"


/*********************************************************************************************************
** Function name:           lptmrInit
** Descriptions:            lptmrµÄ³õÊ¼»¯ 
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created date:           
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void  lptmrInit (void) 
{
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;                                   /* ´ò¿ªlptmrÊ±ÖÓ               */
    LPTMR0_CSR |= LPTMR_CSR_TCF_MASK | LPTMR_CSR_TIE_MASK;               /* Çå³ý±êÖ¾Î» ¿ªÆôÖÐ¶Ï         */
    LPTMR0_PSR |= LPTMR_PSR_PCS(1);                                      /* Ê¹ÓÃ1KHzÊ±ÖÓÔ´              */
    LPTMR0_CMR = 500;                                                    /* Ä¬ÈÏ2·ÖÆµ 500¼´Îª1s         */
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;                                    /* ¿ªÆôLPTMR                   */
}

// wenxue  add

void LPTMR_init(int count, int clock_source)
{
  SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
	
	if (clock_source== 0)
    printf("\n\r LPTMR Clock source is the MCGIRCLK \n\r");
  if (clock_source== 1)
      printf("\n\r LPTMR Clock source is the LPOCLK \n\r");
  if (clock_source== 2)
      printf("\n\r LPTMR Clock source is the ERCLK32 \n\r");
  if (clock_source== 3)
      printf("\n\r LPTMR Clock source is the OSCERCLK \n\r");
	
	
	 LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(5) // 0101 is div 5
                 | LPTMR_PSR_PCS(clock_source)) ; // use the choice of clock
             
   LPTMR0_CMR = LPTMR_CMR_COMPARE(count);  //Set compare value

   LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK   // Clear any pending interrupt
                 | LPTMR_CSR_TIE_MASK   // LPT interrupt enabled
                 | LPTMR_CSR_TPS(0)     //TMR pin select
                 |!LPTMR_CSR_TPP_MASK   //TMR Pin polarity
                 |!LPTMR_CSR_TFC_MASK   // Timer Free running counter is reset whenever TMR counter equals compare
                 |!LPTMR_CSR_TMS_MASK   //LPTMR0 as Timer
                );
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;   //Turn on LPT and start counting
}


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:           LPTimer_IRQHandler
** Descriptions:            lptmr 1sÖÐ¶Ï
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created date:           
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void LPTimer_IRQHandler(void)
{
    LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;                                   /* lptmr±È½Ï±êÖ¾Î»Çå³ý          */
   // gpioToggle();                                                       /* gpio·­×ª                     */
}
/*********************************************************************************************************
  END FILE
**/

