
/*                                                                           *
 * Revision history:                                                         *
 *                                                                           *
 * $Log: cc1000.h,v $
 * Revision 2.2  2003/05/07 15:02:27  tos
 * Modified LOCK monitor in RX/TX setup: wait for LOCK before turning PA on.
 *
 * Revision 2.1.2.2  2003/05/07 14:34:49  tos
 * Modified LOCK monitor in RX/TX setup: wait for LOCK before turning PA on.
 *
 * Revision 2.1.2.1  2003/04/24 14:15:32  tos
 * Verification of corrected calibration+lock monitor: RX/TX switching.
 *
 * Revision 2.1  2003/04/24 12:41:03  tos
 * Corrected inconsistent monitoring of CC1000: [calibration complete] + [lock].
 *
 *                                                                           *
 ****************************************************************************/


/* Constants defined for CC1000 */

/* Register addresses */

#define CC1000_MAIN            0x00
#define CC1000_FREQ_2A         0x01
#define CC1000_FREQ_1A         0x02
#define CC1000_FREQ_0A         0x03
#define CC1000_FREQ_2B         0x04
#define CC1000_FREQ_1B         0x05
#define CC1000_FREQ_0B         0x06
#define CC1000_FSEP1           0x07
#define CC1000_FSEP0           0x08
#define CC1000_CURRENT         0x09
#define CC1000_FRONT_END       0x0A
#define CC1000_PA_POW          0x0B
#define CC1000_PLL             0x0C
#define CC1000_LOCK            0x0D
#define CC1000_CAL             0x0E
#define CC1000_MODEM2          0x0F
#define CC1000_MODEM1          0x10
#define CC1000_MODEM0          0x11
#define CC1000_MATCH           0x12
#define CC1000_FSCTRL          0x13
#define CC1000_FSHAPE7         0x14
#define CC1000_FSHAPE6         0x15
#define CC1000_FSHAPE5         0x16
#define CC1000_FSHAPE4         0x17
#define CC1000_FSHAPE3         0x18
#define CC1000_FSHAPE2         0x19
#define CC1000_FSHAPE1         0x1A
#define CC1000_FSDELAY         0x1B
#define CC1000_PRESCALER       0x1C
#define CC1000_TEST6           0x40
#define CC1000_TEST5           0x41
#define CC1000_TEST4           0x42
#define CC1000_TEST3           0x43
#define CC1000_TEST2           0x44
#define CC1000_TEST1           0x45
#define CC1000_TEST0           0x46

#define  LOCK_NOK         0x00
#define  LOCK_OK          0x01
#define  LOCK_RECAL_OK    0x02

/* Functions for accessing the CC1000 */

void ConfigureCC1000(unsigned char Count, unsigned int Configuration[]);
void ConfigureCC1000EEPROM(unsigned char Count, unsigned int *Configuration);
void WriteToCC1000Register(unsigned char addr, unsigned char data);
void WriteToCC1000RegisterWord(unsigned int addranddata);
unsigned char ReadFromCC1000Register(unsigned char addr);
void ResetCC1000(void);
unsigned char CalibrateCC1000(void);
unsigned char SetupCC1000RX(unsigned char RXCurrent, unsigned char RXPLL);
unsigned char SetupCC1000TX(unsigned char TXCurrent, unsigned char TXPLL);
void SetupCC1000PD(void);
void WakeUpCC1000ToRX(unsigned char RXCurrent, unsigned char RXPLL);
void WakeUpCC1000ToTX(unsigned char TXCurrent, unsigned char TXPLL);
void AverageManualLockCC1000(void);
void AverageFreeRunCC1000(void);
void AverageAutoLockCC1000(void);
void ReadCurrentCalibration(unsigned char *val1, unsigned char *val2);
void OverrideCurrentCalibration(unsigned char val1, unsigned char val2);
void StopOverridingCalibration(void);
void ResetFreqSynth(void);
