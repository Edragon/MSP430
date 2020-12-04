#ifndef _IODEFINE
#define _IODEFINE

#include <msp430x14x.h>

//定义控制端口
#define Ctrl_Port			P6OUT
#define Ctrl_DIR		        P6DIR
#define Ctrl_INPORT   	                P6IN
#define PALE				BIT3
#define PDATA		    		BIT4
#define PCLK				BIT5
#define PALE_1		   		Ctrl_Port |= PALE
#define PALE_0		    		Ctrl_Port &= ~ PALE
#define PDATA_1		    		Ctrl_Port |= PDATA
#define PDATA_0		    		Ctrl_Port &= ~ PDATA
#define PCLK_1		    		Ctrl_Port |= PCLK
#define PCLK_0		    		Ctrl_Port &= ~ PCLK
#define PDATA_IN			Ctrl_DIR &= ~PDATA
#define PDATA_OUT	    	        Ctrl_DIR |= PDATA
#define PDATA_Value	    	        (Ctrl_INPORT & PDATA)

//定义数据端口
#define DCLK 			        BIT4
#define DCLK_IN  			P1DIR &= ~DCLK 
#define DCLK_INT_EN                     P1IE |= DCLK 
#define DCLK_INT_UP		        P1IES &= ~DCLK				// 0,上升沿
#define DCLK_INT_DN	                P1IES |= DCLK					// 1,下降沿

#define DIO					BIT6
#define DIO_Port				P6OUT
#define DIO_DIR 				P6DIR
#define DIO_INPORT		                P6IN
#define DIO_1					DIO_Port |= DIO
#define DIO_0					DIO_Port &= ~DIO
#define DIO_OUT				        DIO_DIR |= DIO
#define DIO_IN					DIO_DIR &= ~DIO
#define DIO_Value			        (DIO_INPORT & DIO)

#endif
