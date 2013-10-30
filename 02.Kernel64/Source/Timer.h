#ifndef __TIMER_H__R
#define __TIMER_H__


#include "Types.h"
#include "InlineAssemblyUtility.h"


// __PIT__START__

#define PIT_CLOCK				1193182
#define PIT_MILLI_TO_COUNT(X)	( PIT_CLOCK * (X) / 1000 )
#define PIT_MICRO_TO_COUNT(X)	( PIT_CLOCK * (X) / 1000000 )

#define PIT_PORT_CONTROL	0x43
#define PIT_PORT_COUNTER0	0x40
#define PIT_PORT_COUNTER1	0x41
#define PIT_PORT_COUNTER2	0x42

#define PIT_CONTROL_COUNTER0		0x00
#define PIT_CONTROL_COUNTER1		0x40
#define PIT_CONTROL_COUNTER2		0x80
#define PIT_CONTROL_LSBMSBRW		0x30
#define PIT_CONTROL_LATCH			0x00
#define PIT_CONTROL_MODE0			0x00
#define PIT_CONTROL_MODE2			0x04
#define PIT_CONTROL_BINARY			0x00
#define PIT_CONTORL_BCD				0x01

#define PIT_COUNTER0_ONCE		( PIT_CONTROL_COUNTER0 | PIT_CONTROL_LSBMSBRW | PIT_CONTROL_MODE0 | PIT_CONTROL_BINARY )
#define PIT_COUNTER0_PERIODIC	( PIT_CONTROL_COUNTER0 | PIT_CONTROL_LSBMSBRW | PIT_CONTROL_MODE2 | PIT_CONTROL_BINARY )
#define PIT_COUNTER0_LATCH		( PIT_CONTROL_COUNTER0 | PIT_CONTROL_LATCH )

void kPitInitialize(WORD wCount, BOOL bPeriodic);
WORD kPitReadCounter0(void);
void kPitWait(WORD wCount);


// __PIT__END__


// __RTC__START__

#define RTC_CMOS_ADDRESS	0x70
#define RTC_CMOS_DATA		0x71

#define RTC_ADDRESS_SECOND		0x00
#define RTC_ADDRESS_MINUTE		0x02
#define RTC_ADDRESS_HOUR		0x04
#define RTC_ADDRESS_DAYOFWEEK	0x06
#define RTC_ADDRESS_DAYOFMONTH	0x07
#define	RTC_ADDRESS_MONTH		0x08
#define RTC_ADDRESS_YEAR		0x09

#define RTC_BCD_TO_BINARY(x)		( ( ( (x) >> 4 ) * 10 ) + ( (x) & 0x0F ) )

void kRtcReadTime(BYTE* pbHour, BYTE* pbMinute, BYTE* pbSecond);
void kRtcReadDate(WORD* pwYear, BYTE* pbMonth, BYTE* pbDayofMonth, BYTE* pbDayofWeek);
// __RTC__END__

















#endif
