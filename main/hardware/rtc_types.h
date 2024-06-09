#pragma once
#include <driver/i2c_types.h>
#include <stdint.h>

typedef struct {
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;
} rtc_handle_t;

enum weekday : uint8_t {
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
};
typedef enum weekday weekday_t;

enum month : uint8_t {
    JANUARY,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER,
};
typedef enum month month_t;

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    weekday_t weekday;
    month_t month;
    uint8_t year;
} rtc_time_t;