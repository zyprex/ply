#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ply.h"

#define LLEN 1024 /* Longest Line Length */

#ifdef TEST_ITEM
#define DEBUG
#else
#define DEBUG while(0)
#endif

struct Item* item;
struct Item* item_last;

struct Item* item_init(void);
int item_read(const char* filename, struct Item* item);
struct Item* item_new(const char* name, struct SubItem* sub);
int item_add(struct Item* item, struct Item* new_item);
void item_clean(struct Item** item_ref);
void item_print(struct Item* item);
void sub_item_clean(struct SubItem** sub_ref);
struct Item* item_find_first(struct Item* item, char* name);

struct Item* item_init(void) {
  struct Item* new_item = (struct Item*)malloc(sizeof(struct Item));
  new_item->sub = NULL;
  new_item->next = NULL;
  item_last = new_item;
  return new_item;
}

struct Item* item_new(const char* name, struct SubItem* sub) {
  struct Item* new_item = (struct Item*)malloc(sizeof(struct Item));
  int nlen = strlen(name) + 1;
  char* new_name = (char*)malloc(nlen*sizeof(char));
  strncpy(new_name, name, nlen);
  new_item->name = new_name;
  new_item->sub = sub;
  return new_item;
}

int item_add(struct Item* item, struct Item* new_item) {
  new_item->next = NULL;
  item_last->next = new_item;
  item_last = new_item;
  DEBUG printf("<item_add(name:%s)>\n", new_item->name);
  return 0;
}

void item_clean(struct Item** item_ref) {
  struct Item* m = *item_ref;
  struct Item* temp;
  while(m && m->next != NULL) {
    m = m->next;
    if (m->name != NULL)
      DEBUG printf("<free name: %s>\n", m->name);
    temp = m;
    if (m->sub != NULL) {
      sub_item_clean(&(m->sub));
    }
    free(m->name);
    free(m);
    m =  temp;
  }
  DEBUG printf("<free end>\n");
  /* system("pause"); */
  *item_ref = NULL;
}

void item_print(struct Item* item) {
  struct Item* m = item;
  while (m->next != NULL) {
    m = m->next;
    if (m->sub == NULL) {
      printf("%s\n", m->name);
    } else {
      printf(
          "%s"
          " dir:%s "
          " exe:%s "
          " arg:%s "
          " var:%s "
          " cmm:%s "
          " inc:%s\n",
          m->name,
          m->sub->dir,
          m->sub->exe,
          m->sub->arg,
          m->sub->var,
          m->sub->cmm,
          m->sub->inc
          );
    }
  }
}

struct SubItem* sub_item_init(void) {
  struct SubItem* new_sub = (struct SubItem*)malloc(sizeof(struct SubItem));
  new_sub->dir = NULL;
  new_sub->exe = NULL;
  new_sub->arg = NULL;
  new_sub->var = NULL;
  new_sub->cmm = NULL;
  new_sub->inc = NULL;
  return new_sub;
}

void sub_item_add(struct SubItem* sub, char* sub_key, char* sub_val) {
  int vlen = strlen(sub_val) + 1;
  if (!strcmp("  dir:", sub_key)) {
    char* new_dir = (char*)malloc(vlen*sizeof(char));
    strncpy(new_dir, sub_val, vlen);
    sub->dir = new_dir;
  }
  if (!strcmp("  exe:", sub_key)) {
    char* new_exe = (char*)malloc(vlen*sizeof(char));
    strncpy(new_exe, sub_val, vlen);
    sub->exe = new_exe;
  }
  if (!strcmp("  arg:", sub_key)) {
    char* new_arg = (char*)malloc(vlen*sizeof(char));
    strncpy(new_arg, sub_val, vlen);
    sub->arg = new_arg;
  }
  if (!strcmp("  var:", sub_key)) {
    char* new_var = (char*)malloc(vlen*sizeof(char));
    strncpy(new_var, sub_val, vlen);
    sub->var = new_var;
  }
  if (!strcmp("  cmm:", sub_key)) {
    char* new_cmm = (char*)malloc(vlen*sizeof(char));
    strncpy(new_cmm, sub_val, vlen);
    sub->cmm = new_cmm;
  }
  if (!strcmp("  inc:", sub_key)) {
    char* new_inc = (char*)malloc(vlen*sizeof(char));
    strncpy(new_inc, sub_val, vlen);
    sub->inc = new_inc;
  }
}

void sub_item_clean(struct SubItem** sub_ref) {
  struct SubItem* s = *sub_ref;
  free(s->dir);
  free(s->exe);
  free(s->arg);
  free(s->var);
  free(s->cmm);
  free(s->inc);
  free(s);
  *sub_ref = NULL;
  DEBUG printf("<sub free end>\n");
}


int item_read(const char* filename, struct Item* item) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return 1;
  }
  char line[LLEN], buf[LLEN], subbuf[LLEN], subfmt[15];
  char* sub_key[] = {
    "  dir:",
    "  exe:",
    "  arg:",
    "  var:",
    "  cmm:",
    "  inc:"
  };

  struct SubItem* s = NULL;

  buf[0] = '\0';
  subbuf[0] = '\0';

  int cnt = 0;

  while (fgets(line, LLEN, file) != NULL) {
    if (line[0] == '\n')
      continue;
    int llen = strlen(line);
    if (line[0] != ' ' && line[0] != '\t') {
      if (buf[0] == '\0') {
        DEBUG printf("<empty buffer>\n");
      } else {
        cnt++;
        if (cnt != 0) {
          item_add(item, item_new(buf, s));
          s = NULL;
          buf[0] = '\0';
          subbuf[0] = '\0';
        }
      }
      strncpy(buf, line, llen);
      buf[llen - 1] = '\0'; /* remove the '\n' */
    }
    for (int i = 0; i < 6; ++i) {
      if (!strncmp(line, sub_key[i], 6)) {
        sprintf(subfmt, "%s%%[^\n]s", sub_key[i]);
        sscanf(line, subfmt, subbuf);
        if (s == NULL) {
          s = sub_item_init();
        }
        sub_item_add(s, sub_key[i], subbuf);
        DEBUG printf("<item_sub_add(%s:%s)>\n", sub_key[i] + 2, subbuf);
      }
    }
  }
  if (buf[0] != '\0') {
    item_add(item, item_new(buf, s));
  }
  fclose(file);
  return 0;
}

struct Item* item_find_first(struct Item* item, char* name) {
  struct Item* m = item;
  while (m->next != NULL) {
    m = m->next;
    if (!strcmp(name, m->name)) {
      return m;
    }
  }
  return NULL;
}

#if 0
int main(int argc, char *argv[]) {
  item = item_init();

/* #define TEST1 */
#ifdef TEST1
  item_add(item, item_new("you", NULL));
  item_add(item, item_new("rise", NULL));
  item_add(item, item_new("me", NULL));
  item_add(item, item_new("up", NULL));
  item_print(item);
#endif

  item_read("item.txt", item);
  item_print(item);

  item_clean(&item);
  return 0;
}
#endif

#undef DEBUG
