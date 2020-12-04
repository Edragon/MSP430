
/*                                                                           *
 * Revision history:                                                         *
 *                                                                           *
 * $Log: configure.h,v $
 * Revision 2.1  2003/04/24 12:43:02  tos
 * Cosmetic change: added revision history for CVS.
 *
 *                                                                           *
 ****************************************************************************/


/* Constants containing current settings for CC1000 */

#define TX_CURRENT_868 0xF3
#define TX_CURRENT_433 0x81
#define RX_CURRENT_868 0x8C
#define RX_CURRENT_433 0x44

extern unsigned char PA_Power;
extern unsigned char TXCurrent;
extern unsigned char RXCurrent;

extern unsigned char Button1Char,Button2Char,Button3Char,Button4Char;
extern unsigned char RXPLL, TXPLL;


void SetupCC1000All(void);
void ConfigurationMode(void);
unsigned char hexdigit2int(unsigned char val);
void DumpCC1000Regs(void);
