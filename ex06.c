#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*subr)(const void *, const void *);

typedef struct school_record SRec;
 struct school_record {
 float gpa;
 int credit;
 char name[200];
 SRec *next;
};


SRec *input(char *input_txt,int* count) {
 SRec *head;
 SRec **tail = &head;
 	FILE *fp;
	int n= 0;
	fp = fopen(input_txt, "r");
	if (fp == NULL) {
	        fprintf(stderr, "Cannot open file: %s\n", input_txt);
	        return 0;
        }

    while(!feof(fp)){
      SRec *p = (SRec *)malloc(sizeof(SRec));
        if (p == NULL) {
            fprintf(stderr, "Failed to allocate memory for new node.\n");
            return NULL;
        }
        p->next = NULL;
         fscanf(fp, "%f", &p->gpa);
          if(p->gpa < 0 || p->gpa > 4){
              printf("gpa range error.\n");
              exit(0);
          }
          fscanf(fp, "%d", &p->credit);
            if(p->credit < 0){
                printf("credit range error.\n");
                exit(0);
            }
            fscanf(fp, "%199s", p->name);
            *tail = p;
            tail = &(p->next);
            n++;
            fprintf(stderr, "n = %d\t gpa = %.1f\t credit = %d\t name = %s\n", n, p->gpa, p->credit, p->name);
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
/*
void sort(SRec data[], int n, subr comp) {
  SRec temp;
  int max;
  for (int i = 0; i < n-1; i++) {
    max = 0;
        for (int j = 0; j < n-i; j++) {
          if (comp(&data[j], &data[max]) > 0) {
            max = j;
            }
        }
        temp = data[n - i - 1];
        data[n - i - 1] = data[max];
        data[max] = temp;
    }
}*/

// SRec *listsort(SRec *head, int (*compar)(const void *, const void *)) {
//   SRec *sorted_head = NULL;
//   SRec dummy = {0, 0, "", NULL};
//   SRec *pp, *max;
//   dummy.next = head;

//   while (head != NULL) {
//     pp = &dummy;
//     max = head;
//     for (pp; pp->next != NULL; pp = pp->next) {
//       //ppが最大値を見つけるまで探索(nullなら終了)
//       if (compar(pp->next, max->next) > 0) {
//         max = pp;    //最大値の直前の要素をmaxにする
//       }
//     }
//     fprintf(stderr, "max = %.1f %d %s\n", max->gpa, max->credit, max->name);
//     pp = max;
//     fprintf(stderr, "find max = %.1f %d %s\n", pp->gpa, pp->credit,
//     pp->name); pp->next = max->next->next; // max->nextをリストから外す
//     max->next->next = sorted_head;     //max->nextをsorted_headにつなげる
//     sorted_head = max->next;           // maxをsorted_headにする
//   }
//   return sorted_head;
// }

SRec *mysort(SRec *head, subr comp) {
  SRec **p, **max;
  SRec *cursor = head;
  SRec *temp;
  SRec **tail;
  while (cursor != NULL) {
    for(p = &head, max = &head; (*p) != NULL; p = &(*p)->next) {
      if (comp(*max, *p) < 0) {
        max = p;
      }
    }
    //fprintf(stderr, "max = %.1f %d %s\n", (*max)->gpa, (*max)->credit, (*max)->name);
    temp =  *max;
    *max = (*max)->next;
    

    cursor = cursor->next;
  }
  return head;
}

void output(char *output_txt, SRec *record, int n){
	int i;
        FILE *fo;
        SRec *temp;
	if(strcmp(output_txt,"output.txt")){
		fprintf(stderr, "Incorrect file name: %s\n", output_txt);
			        return;
	}
	fo = fopen(output_txt, "w");
        for (i = 0; i < n; i++) {
          fprintf(fo, "%.1f %d %s\n", record->gpa, record->credit,
                  record->name);
          free(record);
          record = record->next;
	}
        fclose(fo);
}

int main(int argc, char *argv[]) {
  int (*comp)(const void *, const void *);
	if(argc!=4){
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
    input_ = mysort(input_,comp);
    output(argv[3],input_,count);

	return 0;
}