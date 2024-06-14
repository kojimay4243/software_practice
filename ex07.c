#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef int (*subr)(const void *, const void *);

typedef struct school_record {
  float gpa;
  int credit;
  char name[200];
} SRec;

SRec *input(char *input_txt, int *count) {
  FILE *fp;
  int i, n;
  fp = fopen(input_txt, "r");

  if (fp == NULL) {
    fprintf(stderr, "Cannot open file: %s\n", input_txt);
    return 0;
  }

  fscanf(fp, "%d", &n);
  if (n <= 0) {
    printf("input number error");
    exit(0);
  }
  // printf("input number is %d\n", n);

  SRec *record = (SRec *)calloc(n, sizeof(SRec));
  if (record == NULL) {
    printf("calloc error");
    exit(0);
  }

  for (i = 0; i < n; i++) {
    fscanf(fp, "%f", &record[i].gpa);
    if (record[i].gpa < 0 || record[i].gpa > 4) {
      printf("gpa range error.\n");
      exit(0);
    }
    fscanf(fp, "%d", &record[i].credit);
    if (record[i].credit < 0) {
      printf("credit range error.\n");
      exit(0);
    }
    fscanf(fp, "%199s", record[i].name);
  }
  *count = n;
  fclose(fp);
  return record;
}

int comp_gpa(const void *a, const void *b) {
  // GPAによる比較処理
  if (((SRec *)a)->gpa > ((SRec *)b)->gpa) {
    return 1;
  } else if (((SRec *)a)->gpa < ((SRec *)b)->gpa) {
    return -1;
  } else {
    return 0;
  }
}

int comp_credit(const void *a, const void *b) {
  // 修得単位数による比較処理
  if (((SRec *)a)->credit > ((SRec *)b)->credit) {
    return 1;
  } else if (((SRec *)a)->credit < ((SRec *)b)->credit) {
    return -1;
  } else {
    return 0;
  }
}

int comp_name(const void *a, const void *b) {
  // 名前による比較処理
  return strcmp(((SRec *)a)->name, ((SRec *)b)->name);
}

void sort(SRec data[], int n, subr comp) {
  SRec temp;
  int max;
  for (int i = 0; i < n - 1; i++) {
    max = 0;
    for (int j = 0; j < n - i; j++) {
      if (comp(&data[j], &data[max]) > 0) {
        max = j;
      }
    }
    temp = data[n - i - 1];
    data[n - i - 1] = data[max];
    data[max] = temp;
  }
}

void output(char *output_txt, SRec *record, int n) {
  int i;
  FILE *fo;
  fo = fopen(output_txt, "w");
  fprintf(fo, "%d\n", n);
  for (i = 0; i < n; i++) {
    fprintf(fo, "%.1f,%d,%s\n", record[i].gpa, record[i].credit,
            record[i].name);
  }
  fclose(fo);
}

int main(int argc, char *argv[]) {
  int (*comp)(const void *, const void *);
  if (argc != 4) {
    printf("argc error");
    return 0;
  }
  int count = 0;
  SRec *input_;

  if (strcmp(argv[1], "gpa") == 0) {
    comp = comp_gpa;
  } else if (strcmp(argv[1], "credit") == 0) {
    comp = comp_credit;
  } else if (strcmp(argv[1], "name") == 0) {
    comp = comp_name;
  } else {
    printf("Invalid argument\n");
    return 1;
  }

  input_ = input(argv[2], &count);
  clock_t start = clock();
  sort(input_, count, comp);
  clock_t end = clock();
  double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
  printf("mysort function took %f seconds to execute \n", time_taken);
  output(argv[3], input_, count);
  free(input_);

  return 0;
}