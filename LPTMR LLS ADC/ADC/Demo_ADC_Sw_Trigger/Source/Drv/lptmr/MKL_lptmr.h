#ifndef __ZMD_LPTMR_MKL_H
#define __ZMD_LPTMR_MKL_H

//
//-------- <<< Use Configuration Wizard in Context Menu >>> ----------------------------------------------
//
#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************************************************
  �궨��
*********************************************************************************************************/
#define LPTMR_USE_IRCLK 0 
#define LPTMR_USE_LPOCLK 1
#define LPTMR_USE_ERCLK32 2
#define LPTMR_USE_OSCERCLK 3
	
	
/*********************************************************************************************************
  �������� 
*********************************************************************************************************/  
void lptmrInit(void);
void LPTMR_init(int count, int clock_source);

#ifdef __cplusplus
}
#endif


#endif


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/  

