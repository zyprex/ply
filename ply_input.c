#include <unistd.h>
#include "ply.h"

#ifdef TEST_INPUT
#define DEBUG
#else
#define DEBUG while(0)
#endif

int run_by_jump = 0;
int run_by_ctrl_num = 0;

static int jump_mode = 0;

Ihandle* init_input(void) {
  Ihandle* input = IupText(NULL);
  IupSetAttribute(input, "NAME", "INPUT");
  IupSetAttribute(input, "EXPAND", "HORIZONTAL");
  IupSetCallback(input, "VALUECHANGED_CB", (Icallback)input_valuechanged_cb);
  IupSetCallback(input, "K_ANY", (Icallback)input_keypress_cb);
  return input;
}

int input_keypress_cb(Ihandle *self, int c) {
  Ihandle* dlg = IupGetDialog(self);
  Ihandle* wdir = IupGetDialogChild(dlg, "WDIR");
  Ihandle* input = IupGetDialogChild(dlg, "INPUT");
  Ihandle* prompt = IupGetDialogChild(dlg, "PROMPT");
  Ihandle* comment = IupGetDialogChild(dlg, "COMMENT");
  Ihandle* sign = IupGetDialogChild(dlg, "SIGN");
  Ihandle* lin = IupGetDialogChild(dlg, "LIN");
  char* text = IupGetAttribute(input, "VALUE");

  if (jump_mode) {
    IupSetAttribute(input, "READONLY", "YES");
    for (int i = 0; i < 52; ++i) {
      if (jump_index[i] == c) {
        DEBUG printf("jump %d\n", i);
        if (i < item_list_cnt) {
          lnum = i;
          disp_item_select(prompt);
        }
      }
    }
    if (!iup_isShiftXkey(c)) {
      lin_fill(lin);
      jump_mode = 0;
      if (run_by_jump && iup_isprint(c)) {
        run_cmd(dlg, item_list[lnum]);
      }
      IupSetAttribute(input, "READONLY", "NO");
    }
    return IUP_IGNORE;
  }
  /* jump to Nth line */
  if (c == K_cG) {
    lin_fill_a(lin);
    jump_mode = 1;
  }
  /* restart dlg */
  if (c == K_cR) {
    IupExecute(IupGetGlobal("EXEFILENAME"), NULL);
    return IUP_CLOSE;
  }
  /* close dlg */
  if (c == K_ESC) {
    return IUP_CLOSE;
  }
  /* change match mode */
#define MATCH_MODE_CHG_PROC \
  do{\
    page = 0;\
    lnum = 0;\
    disp_item(prompt, text);\
    disp_item_select(dlg);\
    show_sign(sign); }while(0);
  if (c == K_m1) {
    match_mode = MATCH_PREFIX;
    MATCH_MODE_CHG_PROC
  }
  if (c == K_m2) {
    match_mode = MATCH_IN;
    MATCH_MODE_CHG_PROC
  }
  if (c == K_m3) {
    match_mode = MATCH_SUFFIX;
    MATCH_MODE_CHG_PROC
  }
  if (c == K_m4) {
    match_mode = MATCH_ANY;
    MATCH_MODE_CHG_PROC
  }
  if (c == K_m5) {
    match_mode = MATCH_ABBR;
    MATCH_MODE_CHG_PROC
  }
  if (c == K_m0) {
    match_case = !match_case;
    MATCH_MODE_CHG_PROC
  }
  /* next prev page */
#define PAGE_CHG_PROC \
  do{\
    lnum = 0;\
    disp_item(prompt, text);\
    disp_item_select(dlg);\
    show_sign(sign); }while(0);
  if (c == K_PGDN || c == K_cN) {
    page++;
    PAGE_CHG_PROC
  }
  if (c == K_PGUP || c == K_cP) {
    if (page > 0) {
      page--;
      PAGE_CHG_PROC
    }
  }
  /* next prev line */
  if (c == K_cJ || c == K_DOWN) {
    if (lnum < item_list_cnt - 1) {
      lnum++;
      disp_item_select(prompt);
    }
    return IUP_IGNORE;
  }
  if (c == K_cK || c == K_UP) {
    if (lnum > 0) {
      lnum--;
      disp_item_select(prompt);
    }
    return IUP_IGNORE;
  }
  /* select Nth line */
#define LINE_SELECT_PROC(x) if (c == K_c##x) { \
  lnum = x - 1; disp_item_select(prompt);\
  if (run_by_ctrl_num) run_cmd(dlg, item_list[lnum]); }
  LINE_SELECT_PROC(1)
  LINE_SELECT_PROC(2)
  LINE_SELECT_PROC(3)
  LINE_SELECT_PROC(4)
  LINE_SELECT_PROC(5)
  LINE_SELECT_PROC(6)
  LINE_SELECT_PROC(7)
  LINE_SELECT_PROC(8)
  LINE_SELECT_PROC(9)
#define K_c10 K_c0
  LINE_SELECT_PROC(10)
  /* run */
  if (c == K_CR) {
    run_cmd(dlg, item_list[lnum]);
  }
  /* run and close */
  if (c == K_cCR) {
    run_cmd(dlg, item_list[lnum]);
    return IUP_CLOSE;
  }
  /* item info */
  if (c == K_TAB) {
    show_item_info(item_list[lnum]);
    // tab will go to next input control, ignore it
    return IUP_IGNORE;
  }
  /* simulate change directory */
  if (c == K_cBackslash) {
    wdir_button_cb(wdir, IUP_BUTTON1, 0, 0, 0, "1");
  }
  return IUP_DEFAULT;
}

int input_valuechanged_cb(Ihandle *self) {
  Ihandle* dlg = IupGetDialog(self);
  Ihandle* timer = (Ihandle*)IupGetAttribute(dlg, "TIMER");
  IupSetAttribute(timer, "RUN", "NO");
  IupSetAttribute(timer, "RUN", "YES");
  return IUP_DEFAULT;
}

void run_cmd(Ihandle* dlg, struct Item* m) {
  if (m == NULL) {
    return;
  }
  launch(dlg, m);
  /* Run 'name' one by one defined in 'inc'
   * NOTICE: they're separated by '\t'
   */
  if (!(m->sub)) {
    return;
  }
  if (m->sub->inc) {
    char inc[LLEN], *nx;
    strcpy(inc, m->sub->inc);
    DEBUG printf("recursive run: %s\n", inc);
    char* delim = "\t";
    nx = strtok(inc, delim);
    while (nx != NULL) {
      if (!strcmp(nx, m->name)) {
        nx = strtok(NULL, delim);
        continue;
      }
      run_cmd(dlg, item_find_first(item, nx));
      nx = strtok(NULL, delim);
    }
  }
}

void launch(Ihandle* dlg, struct Item* m) {
  if (!(m->sub)) {
    try_execute(m->name, NULL);
    return;
  }
  char* cmd = m->sub->exe ? m->sub->exe : m->name;
  char* params = m->sub->arg;
  char* var = m->sub->var;
  char* dir = m->sub->dir;
  char* inc = m->sub->inc;
  /* Don't back to origin workdir, only if 'dir'
   * is the only params (except 'cmm').
   * So you can change workdir by define dir alone.
   */
  if (dir && !(m->sub->exe) && !params && !var && !inc) {
    chdir(dir);
    Ihandle* wdir = IupGetDialogChild(dlg, "WDIR");
    show_workdir(wdir);
    return;
  } else if (dir) {
    chdir(dir);
  }
  if (var) {
    show_input_extra(cmd, params, var);
  } else {
    try_execute(cmd, params);
  }
  chdir(workdir);
}

void try_execute(char* cmd, char* params) {
  int exit_code;
  if (params == NULL) {
    exit_code = IupExecute(cmd, "");
  } else {
    exit_code = IupExecute(cmd, params);
  }
  if (exit_code < 0) {
    Ihandle* msg = IupMessageDlg();
    IupSetAttribute(msg, "DIALOGTYPE", "ERROR");
    IupSetfAttribute(msg, "VALUE",
        "Failed, exit with %d",  exit_code);
    IupPopup(msg, IUP_CENTER, IUP_CENTER);
  }
}

void show_item_info(struct Item* m) {
  Ihandle *dlg, *text;
  text= IupText(NULL);
  IupSetAttribute(text, "MULTILINE", "YES");
  IupSetInt(text, "VISIBLELINES", 7);
  IupSetAttribute(text, "EXPAND", "YES");
  IupSetAttribute(text, "READONLY", "YES");
  dlg = IupDialog(text);
  IupSetAttribute(dlg, "TITLE", PLY_TITLE_INFO);
  IupSetAttribute(dlg, "SIZE", "THIRD");
  if (m->sub) {
    IupSetStrf(text, "VALUE",
        "%s\n"
        "  dir:%s\n"
        "  exe:%s\n"
        "  arg:%s\n"
        "  var:%s\n"
        "  cmm:%s\n"
        "  inc:%s\n",
        m->name,
        m->sub->dir ? m->sub->dir : " ",
        m->sub->exe ? m->sub->exe : " ",
        m->sub->arg ? m->sub->arg : " ",
        m->sub->var ? m->sub->var : " ",
        m->sub->cmm ? m->sub->cmm : " ",
        m->sub->inc ? m->sub->inc : " "
        );
  } else {
    IupSetAttribute(text, "VALUE", m->name);
  }
  IupShow(dlg);
}

#undef DEBUG
