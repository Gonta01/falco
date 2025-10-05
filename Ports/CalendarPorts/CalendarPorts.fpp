module Drv {
  module Calendar {
    struct Time {
      Tm_sec:   I32 @< Seconds after the minute; range: 0-61* * - tm_sec is generally 0-59. The extra range is to accommodate for leap seconds in certain systems.
      Tm_min:   I32 @< Minutes after the hour; range: 0-59
      Tm_hour:  I32 @< Hours since midnight; range: 0-23
      Tm_mday:  I32 @< Day of the month; range: 1-31
      Tm_mon:   I32 @< Months since January; range: 0-11
      Tm_year:  I32 @< Years since 1900;
      Tm_wday:  I32 @< Days since Sunday; range: 0-6
      Tm_yday:  I32 @< Days since January; range: 0-365
      Tm_isdst: I32 @< Daylight Saving Time flag** ** - The Daylight Saving Time flag (tm_isdst) is greater than zero if Daylight Saving Time is in effect, zero if Daylight Saving Time is not in effect, and less than zero if the information is not available.
    }

    port Set($time: Drv.Calendar.Time)
    port Get(ref $time: Drv.Calendar.Time)
  }
}
