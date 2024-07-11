#include "ex14.h"

int month, days;

const int month_days[13] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

extern void whatday_ja(int);
extern void whatday_en(int);

int whatday(int elapsed_days_, int * month, int * day)
{
  if (elapsed_days_ < 0 || elapsed_days_ > 365) {
    return WHATDAY_NG;
  }
  for (int i = 0; i < 12; i++) {
    if (elapsed_days_ <= month_days[i]) {
      *month = i;
      *day = elapsed_days_;
      break;
    }
    elapsed_days_ -= month_days[i];
  }

  return WHATDAY_OK;
}

int main()
{
  int input_days;
  char input_file[256];
  printf("Language ? \t");
  scanf("%s", input_file);
  printf("Day of the year ? \t");
  scanf("%d", &input_days);

  if (strcmp(input_file, "Japanese") == 0) {
    whatday_ja(input_days);
  } else if (strcmp(input_file, "English") == 0) {
    whatday_en(input_days);
  } else {
    printf("Sorry, %s is not supported.\n", input_file);
    return 0;
  }
  return 0;
}