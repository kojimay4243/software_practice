#include "ex14.h"

static char * month_name[] = {"January",   "February", "March",    "April",
                              "May",       "June",     "July",     "August",
                              "September", "October",  "November", "December"};
extern int month, days;

extern int whatday(int elapsed_days_, int * month, int * day);

void whatday_en(int elapsed_days_)
{
  if (whatday(elapsed_days_, &month, &days)) {
    printf("Day is not in the range of a year.\n");
    return;
  }

  printf("The %dst day of a year is %s %dst. \n", elapsed_days_, month_name[month], days);
  return;
}