#include "ex14.h"

static char * month_name[] = {"睦月", "如月", "弥生", "卯月",   "皐月", "水無月",
                              "文月", "葉月", "長月", "神無月", "霜月", "師走"};

extern int month, days;

extern int whatday(int elapsed_days_, int * month, int * day);

void whatday_ja(int elapsed_days_)
{
  if (whatday(elapsed_days_, &month, &days)) {
    printf("与えられた日数が正しい範囲にありません．\n");
    return;
  }

  printf("年頭から%d日目は%s%d日。 \n", elapsed_days_, month_name[month], days);
  return;
}