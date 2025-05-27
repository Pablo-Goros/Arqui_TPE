#include "time.h"
#include "naiveConsole.h"
#include "time_display.h"

// static void format(uint8_t v, char *dst);

static char bcd2char(uint8_t bcd);

static time t;

time* getTime(void) {
    t.sec   = bcd2char( read_RTC(0) );  // seconds
    t.min   = bcd2char( read_RTC(2) );  // minutes
    t.hour  = bcd2char( read_RTC(4) );  // hours
    t.day   = bcd2char( read_RTC(7) );  // day of month
    t.month = bcd2char( read_RTC(8) );  // month
    t.year  = bcd2char( read_RTC(9) );  // year (00–99)
    return &t;
}

uint8_t get_secs(void) {
    uint8_t seconds = read_RTC(0);
    return seconds;
}

uint8_t get_mins(void) {
    uint8_t minutes = read_RTC(2);
    return minutes;
}

uint8_t get_hours(void) {
    uint8_t hours = read_RTC(4);
    return hours;
}

uint8_t get_day(void) {
    uint8_t day = read_RTC(7);
    return day;
}

uint8_t get_month(void) {
    uint8_t day = read_RTC(8);
    return day;
}

uint8_t get_year(void) {
    uint8_t year = read_RTC(9);
    return year;
}

// Converts BCD to binary
static char bcd2char(uint8_t bcd) {
    return (char)(((bcd >> 4) * 10) + (bcd & 0x0F));
}

/*
! QUIZAS SIRVA QUIZAS NO, REVISAR
static void format(uint8_t v, char *dst) {
  dst[0] = '0' + v/10;
  dst[1] = '0' + v%10;
}
*/