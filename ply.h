#ifndef PLY_H
#define PLY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Longest Line Length */
#define LLEN 1024

/* Windows Title */
#define PLY_TITLE       "ply"
#define PLY_TITLE_EXTRA "ply: input params"
#define PLY_TITLE_INFO  "ply: info"
#define PLY_TITLE_CD    "ply: change work dir"

/* ply_item.c */
struct Item {
  char* name;
  struct SubItem* sub;
  struct Item* next;
};
struct SubItem {
  char* dir;
  char* exe;
  char* arg;
  char* var;
  char* cmm;
  char* inc;
};
extern struct Item* item;
extern struct Item* item_last;

struct Item* item_init(void);
int item_read(const char* filename, struct Item* item);
struct Item* item_new(const char* name, struct SubItem* sub);
int item_add(struct Item* item, struct Item* new_item);
void item_clean(struct Item** item_ref);
void item_print(struct Item* item);
void sub_item_clean(struct SubItem** sub_ref);
struct Item* item_find_first(struct Item* item, char* name);

#include <iup.h>
/* ply.c */
int timer_cb(Ihandle *self);

/* ply_item_index.c */
extern int page_item_max;
extern int page;
extern int lnum;
extern int item_list_cnt;
#define ITEM_LIST_CAP 52
extern struct Item* item_list[ITEM_LIST_CAP];
void list_item(struct Item* item, char* k);
extern char* match_sign[];
enum MatchMode{
  MATCH_PREFIX = 1,
  MATCH_IN,
  MATCH_SUFFIX,
  MATCH_ANY,
  MATCH_ABBR
};
extern enum MatchMode match_mode;
extern int match_case;
int match(char* name, char* k);
int str_middle(char* name, char* k);
int str_suffix(char* name, char* k);
int str_abbr_cmp(char* str, char* k);

/* ply_item_disp.c */
void disp_item(Ihandle* prompt, char* input);
void disp_item_select(Ihandle* dlg);

/* ply_conf.c */
extern char** confkv;
char** confkv_init(void);
int confkv_add(char*** confkv_ref, char* str);
void confkv_print(char** confkv);
void confkv_clean(char*** confkv_ref);
int confkv_read(const char* filename, char*** confkv_ref);
char* confkv_find_first(char** confkv, char* k);
char* confkv_find_or_default(char** confkv, char* k, char* s);
int confkv_find_or_default_int(char** confkv, char* k, int i);
void iup_try_conf(Ihandle* self, char* attr, char* attr_sym);

////////////////////// UI ////////////////////////
/* ply_wdir.c */
extern char workdir[LLEN];
Ihandle* init_wdir(void);
void show_workdir(Ihandle* wdir);
void change_workdir(Ihandle* wdir, char* dir);
int wdir_change_txt_keypress_cb(Ihandle* self, int c);
int wdir_button_cb(Ihandle* self, int button, int pressed, int x, int y, char* status);

/* ply_input.c */
extern int run_by_jump;
extern int run_by_ctrl_num;
Ihandle* init_input(void);
int input_keypress_cb(Ihandle *self, int c);
int input_valuechanged_cb(Ihandle *self);
void run_cmd(Ihandle* dlg, struct Item* m);
void launch(Ihandle* dlg, struct Item* m);
void try_execute(char* cmd, char* params);
void show_item_info(struct Item* m);

/* ply_input_extra.c */
int vntext_keypress_cb(Ihandle* self, int c);
void show_input_extra(const char* cmd, const char* params, const char* var);

/* ply_prompt.c */
extern int run_by_leftclick;
Ihandle* init_prompt(void);
int prompt_button_cb(Ihandle* self, int button, int pressed, int x, int y, char* status);

/* ply_comment.c */
Ihandle* init_comment(void);
void show_comment(Ihandle* comm, char* commstr);

/* ply_sign.c */
Ihandle* init_sign(void);
void show_sign(Ihandle* sign);

/* ply_lin.c */
Ihandle* init_lin(void);
void lin_fill(Ihandle* lin);
extern char jump_index[52];
void lin_fill_a(Ihandle* lin);

/* TEST macro */
#define TEST_CONF
#define TEST_ITEM
#define TEST_INPUT
#define TEST_INPUT_EXTRA
#define TEST_PROMPT

#endif
