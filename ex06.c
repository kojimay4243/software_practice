#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef int (*subr)(const void *, const void *);

typedef struct school_record SRec;
struct school_record {
  float gpa;
  int credit;
  char name[200];
  SRec *next;
};

SRec *input(char *input_txt, int *count) {
  SRec *head;
  SRec **tail = &head;
  FILE *fp;
  int n = 0;
  fp = fopen(input_txt, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open file: %s\n", input_txt);
    return 0;
  }

  while (!feof(fp)) {
    SRec *p = (SRec *)malloc(sizeof(SRec));
    if (p == NULL) {
      fprintf(stderr, "Failed to allocate memory for new node.\n");
      return NULL;
    }
    p->next = NULL;
    fscanf(fp, "%f", &p->gpa);
    if (p->gpa < 0 || p->gpa > 4) {
      printf("gpa range error.\n");
      exit(0);
    }
    fscanf(fp, "%d", &p->credit);
    if (p->credit < 0) {
      printf("credit range error.\n");
      exit(0);
    }
    fscanf(fp, "%199s", p->name);
    *tail = p;
    tail = &(p->next);
    n++;
  }

  *count = n;
  fclose(fp);
  return head;
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

SRec *mysort(SRec *head, subr comp) {
  SRec **p, **max;
  SRec *temp;
  SRec *new_head = NULL;
  while (head != NULL) {
    for (p = &head, max = &head; (*p) != NULL; p = &(*p)->next) {
      if (comp(*max, *p) > 0) {
        max = p;
      }
    }
    temp = *max;
    *max = (*max)->next;
    temp->next = new_head;
    new_head = temp;
  }
  fprintf(stderr, " sort finished\n");
  return new_head;
}

void output(char *output_txt, SRec *record, int n) {
  int i;
  FILE *fo;
  SRec *next_record;
  fo = fopen(output_txt, "w");
  for (i = 0; i < n; i++) {
    fprintf(fo, "%.1f %d %s\n", record->gpa, record->credit, record->name);

    next_record = record->next;
    free(record);
    record = next_record;
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
  input_ = mysort(input_, comp);
  clock_t end = clock();
  // 実行時間の計算と表示
  double time_taken = ((double)end - start) / CLOCKS_PER_SEC;
  printf("mysort function took %f seconds to execute \n", time_taken);
  output(argv[3], input_, count);

  return 0;
}