#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum { WHATDAY_OK = 0, WHATDAY_NG };

int whatday(int day_of_the_year, int * month, int * day);
void whatday_ja(int);
void whatday_en(int);