#include "Timer.h"

void kRtcReadTime(BYTE* pbHour, BYTE* pbMinute, BYTE* pbSecond)
{
	BYTE bData;

	// Hour
	kWritePortByte(RTC_CMOS_ADDRESS, RTC_ADDRESS_HOUR);
	bData = kReadPortByte(RTC_CMOS_DATA);
	*pbHour = RTC_BCD_TO_BINARY(bData);

	// Minute
	kWritePortByte(RTC_CMOS_ADDRESS, RTC_ADDRESS_MINUTE);
	bData = kReadPortByte(RTC_CMOS_DATA);
	*pbMinute = RTC_BCD_TO_BINARY(bData);

	// Second
	kWritePortByte(RTC_CMOS_ADDRESS, RTC_ADDRESS_SECOND);
	bData = kReadPortByte(RTC_CMOS_DATA);
	*pbSecond = RTC_BCD_TO_BINARY(bData);
}

void kRtcReadDate(WORD* pwYear, BYTE* pbMonth, BYTE* pbDayofMonth, BYTE* pbDayofWeek)
{
	BYTE bData;

	// Year
	kWritePortByte(RTC_CMOS_ADDRESS, RTC_ADDRESS_YEAR);
	bData = kReadPortByte(RTC_CMOS_DATA);
	*pwYear = RTC_BCD_TO_BINARY(bData) + 2000;

	// Month
	kWritePortByte(RTC_CMOS_ADDRESS, RTC_ADDRESS_MONTH);
	bData = kReadPortByte(RTC_CMOS_DATA);
	*pbMonth = RTC_BCD_TO_BINARY(bData);

	// Day of week
	kWritePortByte(RTC_CMOS_ADDRESS, RTC_ADDRESS_DAYOFWEEK);
	bData = kReadPortByte(RTC_CMOS_DATA);
	*pbDayofWeek = RTC_BCD_TO_BINARY(bData);

	// Day of month
	kWritePortByte(RTC_CMOS_ADDRESS, RTC_ADDRESS_DAYOFMONTH);
	bData = kReadPortByte(RTC_CMOS_DATA);
	*pbDayofMonth = RTC_BCD_TO_BINARY(bData);

}



