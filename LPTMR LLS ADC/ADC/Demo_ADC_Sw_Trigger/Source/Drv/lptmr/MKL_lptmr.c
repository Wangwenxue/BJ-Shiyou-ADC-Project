/****************************************Copyright (c)****************************************************
**               Copyright � 2003~2009 Shenzhen uCdragon Technology Co.,Ltd. All Rights Reserved 
**
**                                 http://www.ucdragon.com
**
**      �����������Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        �������������Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               MKL_lptmr.c
** Last modified date:      
** Last version:            V1.0
** Descriptions:            lptmr��������
**
**--------------------------------------------------------------------------------------------------------
** Created by:             
** Created date 
** Version:                 
** Descriptions:            lptmr��ʼ�� ��ʼ�汾
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
** Descriptions:            lptmr�ĳ�ʼ�� 
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
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;                                   /* ��lptmrʱ��               */
    LPTMR0_CSR |= LPTMR_CSR_TCF_MASK | LPTMR_CSR_TIE_MASK;               /* �����־λ �����ж�         */
    LPTMR0_PSR |= LPTMR_PSR_PCS(1);                                      /* ʹ��1KHzʱ��Դ              */
    LPTMR0_CMR = 500;                                                    /* Ĭ��2��Ƶ 500��Ϊ1s         */
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;                                    /* ����LPTMR                   */
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
** Descriptions:            lptmr 1s�ж�
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
    LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;                                   /* lptmr�Ƚϱ�־λ���          */
   // gpioToggle();                                                       /* gpio��ת                     */
}
/*********************************************************************************************************
  END FILE
**/

