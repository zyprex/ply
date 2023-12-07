#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ply.h"

#ifdef TEST_CONF
#define DEBUG
#else
#define DEBUG while(0)
#endif

char** confkv;

static int confkv_len = 0; /* actual length - 1 */
static int confkv_cap = 15; /* init capacity */
static int confkv_extend_size = 10; /* capacity extend size */

char** confkv_init(void) {
  return (char**)malloc(confkv_cap * sizeof(char*));
}

int confkv_add(char*** confkv_ref, char* str) {
  char** confkv = *confkv_ref;
  /* extend memory allocation */
  if (confkv_len >= confkv_cap) {
    DEBUG printf("<realloc %d %d>\n", confkv_cap, confkv_cap + confkv_extend_size);
    confkv_cap += confkv_extend_size;
    confkv = (char**)realloc(*confkv_ref, (confkv_cap) * sizeof(char*));
  }
  /* store the whole string */
  confkv[confkv_len] = (char*)malloc((strlen(str) + 1) * sizeof(char));
  sprintf(confkv[confkv_len++], "%s", str);
  DEBUG printf("<add%d %s>\n", confkv_len - 1, confkv[confkv_len - 1]);
  /* update original point  */
  *confkv_ref = confkv;
  return 0;
}

void confkv_print(char** confkv) {
  for (int i = 0; i < confkv_len; ++i) {
    printf("<%d %s>\n", i, confkv[i]);
  }
}

void confkv_clean(char*** confkv_ref) {
  char** confkv = *confkv_ref;
  for (int i = 0; i < confkv_len; ++i) {
    DEBUG printf("<free(%d) %s>\n", i, confkv[i]);
    free(confkv[i]);
  }
  free(*confkv_ref);
  *confkv_ref = NULL;
  DEBUG printf("<free end>\n");
}

int confkv_read(const char* filename, char*** confkv_ref) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    return 1;
  }
  char line[120];
  while (fgets(line, 120, file)) {
    if (isalpha(line[0])) {
      /* printf("%s", line); */
      line[strlen(line) - 1] = '\0';
      confkv_add(confkv_ref, line);
    }
  }
  return 0;
}

static char* strip(char* s) {
  while (*s && (*s == ' ' || *s == '\t'))
    s++;
  return s;
}

char* confkv_find_first(char** confkv, char* k) {
  int klen = strlen(k);
  for (int i = 0; i < confkv_len; ++i) {
    if (!strncmp(confkv[i], k, klen)) {
      char* ret = strip(confkv[i] + klen);
      return ret[0] == '\0' ? NULL : ret;
    }
  }
  return NULL;
}

char* confkv_find_or_default(char** confkv, char* k, char* s) {
  char* ret = confkv_find_first(confkv, k);
  return ret ? ret : s;
}

int confkv_find_or_default_int(char** confkv, char* k, int i) {
  char* ret = confkv_find_first(confkv, k);
  if (ret == NULL) {
    return i;
  }
  int integer;
  int success = sscanf(ret, "%d", &integer);
  if (success == -1) {
    return i;
  } else {
    return integer;
  }
}


#if 0
int main(int argc, char *argv[]) {
  confkv = confkv_init();
  confkv_read("confkv.txt", &confkv);
  confkv_print(confkv);
  printf("FONTSIZE(%s)\n", confkv_find_first(confkv, "fontsize="));
  printf("EMPTY(%s)\n", confkv_find_first(confkv, "empty="));
  confkv_clean(&confkv);
  return 0;
}
#endif

void iup_try_conf(Ihandle* self, char* attr, char* attr_sym) {
  char* value = confkv_find_first(confkv, attr_sym);
  if (!value) {
    return;
  }
  IupSetAttribute(self, attr, confkv_find_first(confkv, attr_sym));
}

#undef DEBUG
