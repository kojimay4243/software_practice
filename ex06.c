#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef int (*subr)(const void *, const void *);

typedef struct school_record SRec;
struct school_record
{
  float gpa;
  int credit;
  char name[200];
  SRec * next;
  SRec *left, *right;
};

SRec * input(char * input_txt, int * count)
{
  SRec * head;
  SRec ** tail = &head;
  FILE * fp;
  int n = 0;
  fp = fopen(input_txt, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open file: %s\n", input_txt);
    return 0;
  }

  while (!feof(fp)) {
    SRec * p = (SRec *)malloc(sizeof(SRec));
    if (p == NULL) {
      fprintf(stderr, "Failed to allocate memory for new node.\n");
      return NULL;
    }
    p->next = NULL;
    p->left = NULL;
    p->right = NULL;
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

int comp_gpa(const void * a, const void * b)
{
  // GPAによる比較処理
  if (((SRec *)a)->gpa > ((SRec *)b)->gpa) {
    return 1;
  } else if (((SRec *)a)->gpa < ((SRec *)b)->gpa) {
    return -1;
  } else {
    return 0;
  }
}

int comp_credit(const void * a, const void * b)
{
  // 修得単位数による比較処理
  if (((SRec *)a)->credit > ((SRec *)b)->credit) {
    return 1;
  } else if (((SRec *)a)->credit < ((SRec *)b)->credit) {
    return -1;
  } else {
    return 0;
  }
}

int comp_name(const void * a, const void * b)
{
  // 名前による比較処理
  return strcmp(((SRec *)a)->name, ((SRec *)b)->name);
}

// SRec *mysort(SRec *head, subr comp) {
//   SRec **p, **max;
//   SRec *temp;
//   SRec *new_head = NULL;
//   while (head != NULL) {
//     for (p = &head, max = &head; (*p) != NULL; p = &(*p)->next) {
//       if (comp(*max, *p) > 0) {
//         max = p;
//       }
//     }
//    //古いリストからの切り離し
//     temp = *max;    //maxが指すポインタをtempにコピー
//     *max = (*max)->next;    //maxのポインタが指す先をmaxの次の要素に変更
//     temp->next = new_head;
//     //tempの次の要素をnew_headに変更,これでtempが指す先はmaxの次,tempの次の要素はnew_headになる
//    //つまりこれでtempが古いリストから切り離され、新しいリストのheadが指す先につながる
//    //新しいリストへの接続
//     new_head = temp;
//     //new_headをtempに変更,つまり新しいリストの先頭がtempになる
//   }
//   fprintf(stderr, " sort finished\n");
//   return new_head;
// }

SRec * scan(SRec * node, SRec * next)
{
  if (node == NULL) {
    // 探索が終了したらnextを返す
    return next;
  }
  // 左の子ノードを探索
  node->next = scan(node->left, next);
  // 右の子ノードを探索
  return scan(node->right, node);
}

SRec * sort(SRec * head, subr comp)
{
  SRec * tree = NULL;
  while (head) {
    // headからnodeを切り離す
    SRec * current = head;  // 現在のheadをcurrentにコピー
    head = head->next;      // headを次に進める

    if (tree == NULL) {
      tree = current;  // ツリーが空の場合はcurrentをツリーのルートにする
    } else {
      SRec ** p = &tree;
      // SRec *pだとpの指す先がNULLになったときどうしようもないので
      // Srec **pにすることでNULLを指すポインタを指すことで対処する
      // pにtreeのアドレスをコピーして探索していく
      while (*p) {
        if (comp(current, *p) < 0) {
          p = &(*p)->left;  // currentが小さい場合は左に進む
        } else {
          p = &(*p)->right;  // currentが大きい場合は右に進む
        }
      }
      *p = current;  // pが指す先にcurrentを格納
    }
  }

  SRec * sorted = NULL;
  sorted = scan(tree, NULL);
  return sorted;
}

// void dump_list(SRec * list)
// {
//   if (list == NULL) {
//     return;
//   } else {
//     printf(
//       "(%5.3f %3d %10s) n:%p l:%p r:%p \n", list->gpa, list->credit, list->name, list, list->left,
//       list->right);
//     dump_list(list->next);
//   }
// }

void output(char * output_txt, SRec * record, int n)
{
  int i;
  FILE * fo;
  SRec * next_record;
  fo = fopen(output_txt, "w");
  for (i = 0; i < n; i++) {
    fprintf(fo, "%.1f %d %s\n", record->gpa, record->credit, record->name);

    next_record = record->next;
    free(record);
    record = next_record;
  }
  fclose(fo);
}

int main(int argc, char * argv[])
{
  int (*comp)(const void *, const void *);
  if (argc != 4) {
    printf("argc error");
    return 0;
  }
  int count = 0;
  SRec * input_;

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
  input_ = sort(input_, comp);
  // dump_list(input_);  // リストの中身を表示(デバッグ用)
  output(argv[3], input_, count);

  return 0;
}