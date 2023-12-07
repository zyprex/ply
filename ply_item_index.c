#include <ctype.h>
#include "ply.h"

int page_item_max = 10;
int page = 0;
int lnum = 0;

int item_list_cnt;
struct Item* item_list[ITEM_LIST_CAP];

char* match_sign[] = {"", "[a]bc", "a[b]c", "ab[c]", "[abc]", "a*b*c*"};
enum MatchMode match_mode = MATCH_ANY;

int match_case = 0;

void list_item(struct Item* item, char* k) {

  int index = 0;

  int counter = 0;
  int jump_cnt = page * page_item_max;

  struct Item* m = item;
  int klen = strlen(k);

  item_list[0] = NULL;
  item_list_cnt = 0;

  while (m->next != NULL) {
    m = m->next;
    if (!match(m->name, k)) {
      if (counter++ < jump_cnt) {
        // do nothing
      } else {
        item_list[index++] = m;
      }
    }
    if (index >= page_item_max) {
      break;
    }
  }
  if (index < page_item_max) {
    item_list[index] = NULL;
    item_list_cnt = index;
  } else {
    item_list_cnt = page_item_max;
  }
}

static void str_tolower(char* s, int len) {
  for (int i = 0; i < len; ++i) {
    if (isalpha(s[i])) {
      s[i] = tolower(s[i]);
    }
  }
}

// if match , return 0
int match(char* name, char* k) {
  char nstr[LLEN];
  strcpy(nstr, name);
  if (!match_case)
    str_tolower(nstr, strlen(nstr));
  switch (match_mode) {
    case  MATCH_PREFIX:
      return strncmp(nstr, k, strlen(k));
    case MATCH_IN:
      return str_middle(nstr, k);
    case MATCH_SUFFIX:
      return str_suffix(nstr, k);
    case MATCH_ANY:
      return strstr(nstr, k) ? 0 : 1;
    case MATCH_ABBR:
      return str_abbr_cmp(nstr, k);
    default:
      break;
  }
}

int str_middle(char* name, char* k) {
  if (k[0] == '\0') {
    return 0;
  }
  char* ch = strstr(name, k);
  if (!ch || *ch == *name || *(ch + strlen(k)) == '\0') {
    return 1;
  }
  return 0;
}

int str_suffix(char* name, char* k) {
  if (k[0] == '\0') {
    return 0;
  }
  char* ch = strstr(name, k);
  if (ch && *(ch + strlen(k)) == '\0') {
    return 0;
  }
  return 1;
}

/* match every word's first char  */
int str_abbr_cmp(char* str, char* k) {
  char abbr[20];
  int idx = 0;
  char* s = str;
  if (isalpha(*str)) {
    abbr[idx++] = *str;
  }
  while (*s && idx < 20) {
    if (!isalpha(*s) && *(s+1) && isalpha(*(s+1))) {
      abbr[idx++] = *(s+1);
    }
    s++;
  }
  abbr[idx] = '\0';
  return strncmp(abbr, k, strlen(k));
}
