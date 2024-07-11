#include <stdio.h>
#include <stdlib.h>
#define WHATDAY_OK 1
#define WHATDAY_NG 0

int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

typedef struct month_name Mname;

struct month_name
{
  char name[256];
};

int what_day(int elapsed_days, Mname * month_name)
{
  if (elapsed_days < 0 || elapsed_days > 365) return WHATDAY_NG;

  int month, days;

  for (int i = 0; i < 12; i++) {
    if (elapsed_days <= month_days[i]) {
      month = i + 1;
      days = elapsed_days;
      break;
    }
    elapsed_days -= month_days[i];
  }

  printf("month: %s\t days: %d\n", month_name[month].name, days);

  return WHATDAY_OK;
}

Mname * read()
{
  Mname * month_name = (Mname *)malloc(sizeof(Mname) * 12);
  FILE * fp;
  fp = fopen("test.txt", "r");

  if (fp == NULL) {
    fprintf(stderr, "Cannot open file: %s\n", "japanese.txt");
    return month_name;
  }
  int i = 0;
  while (!feof(fp)) {
    fscanf(fp, "%s", month_name[i].name);
    // printf("%s\t", month_name[i].name);
    i++;
  }
  fclose(fp);
  return month_name;
}

int main()
{
  int days;
  Mname * month_name;
  scanf("%d", &days);
  month_name = read();
  what_day(days, month_name);
  printf("Hello, World!\n");
  return 0;
}