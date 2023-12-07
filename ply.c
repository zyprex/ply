#include "ply.h"

inline static int parent_dir(char* parent, const char* path) {
  int idx = strlen(path) - 1;
  while (idx > -1 && path[idx] != '\\')
    idx--;
  if (idx > 0) {
    strncpy(parent, path, idx + 1);
    return 0;
  }
  return 1;
}

int timer_cb(Ihandle *self) {
  IupSetAttribute(self, "RUN", "NO");
  Ihandle* dlg = (Ihandle*)IupGetAttribute(self, "DLG");
  Ihandle* input = IupGetDialogChild(dlg, "INPUT");
  Ihandle* prompt = IupGetDialogChild(dlg, "PROMPT");
  Ihandle* sign = IupGetDialogChild(dlg, "SIGN");
  char* text = IupGetAttribute(input, "VALUE");
  page = 0;
  lnum = 0;
  disp_item(prompt, text);
  disp_item_select(dlg);
  show_sign(sign);
  printf("<input idle>\n");
  return IUP_DEFAULT;
}

int main(int argc, char *argv[]) {
  Ihandle *dlg, *wdir, *input,
          *prompt, *lin, *hbox,
          *comment, *sign,
          *vbox, *timer;

  IupOpen(&argc, &argv);

  confkv = confkv_init();
  if (argc > 1) {
    confkv_read(argv[1], &confkv);
  } else {
    confkv_read("ply.conf", &confkv);
  }

//------init var on conf
  /* printf("%s\n", confkv_find_or_default(confkv, "ipp=", "10")); */
  page_item_max = confkv_find_or_default_int(confkv, "ipp=", 10);
  if (page_item_max > ITEM_LIST_CAP || page_item_max < 1) {
    page_item_max = 10;
  }

  match_mode = confkv_find_or_default_int(confkv, "matchmode=", MATCH_ANY);
  if (match_mode > 5 || match_mode < 1) {
    match_mode = 4;
  }
  match_case = confkv_find_or_default_int(confkv, "matchcase=", 0);

  run_by_jump = confkv_find_or_default_int(confkv,"run.jump=",0);
  run_by_ctrl_num = confkv_find_or_default_int(confkv,"run.ctrl+num=",0);
  run_by_leftclick = confkv_find_or_default_int(confkv,"run.leftclick=",0);

//------init controls
  timer = IupTimer();
  IupSetAttribute(timer, "TIME",
      confkv_find_or_default(confkv, "promptdelay=", "300"));
  IupSetCallback(timer, "ACTION_CB", (Icallback)timer_cb);

  wdir = init_wdir();
  input = init_input();
  iup_try_conf(wdir, "FGCOLOR", "workdir.fgcolor=");
  iup_try_conf(wdir, "FONTSIZE", "workdir.fontsize=");

  lin = init_lin();
  prompt = init_prompt();
  hbox = IupHbox(lin, prompt, NULL);

  comment = init_comment();
  iup_try_conf(comment, "FGCOLOR", "comment.fgcolor=");
  iup_try_conf(comment, "FONTSIZE", "comment.fontsize=");
  sign = init_sign();
  iup_try_conf(sign, "FGCOLOR", "sign.fgcolor=");
  iup_try_conf(sign, "FONTSIZE", "sign.fontsize=");

  vbox = IupVbox(wdir, input, hbox, comment, sign, NULL);

  dlg = IupDialog(vbox);
  IupSetAttribute(dlg, "TITLE", PLY_TITLE);
  iup_try_conf(dlg, "SIZE", "size=");
  /* bind  dlg & timer */
  IupSetAttribute(dlg, "TIMER", (char*) timer);
  IupSetAttribute(timer, "DLG", (char*) dlg);

//------init theme
  iup_try_conf(NULL, "DLGFGCOLOR", "fgcolor=");
  iup_try_conf(NULL, "DLGBGCOLOR", "bgcolor=");
  iup_try_conf(NULL, "TXTFGCOLOR", "txtfgcolor=");
  iup_try_conf(NULL, "TXTBGCOLOR", "txtbgcolor=");
  iup_try_conf(NULL, "TXTHLCOLOR", "txthlcolor=");
  iup_try_conf(NULL, "LINKFGCOLOR", "txthlcolor=");
  iup_try_conf(NULL, "DEFAULTFONTFACE", "fontface=");
  iup_try_conf(NULL, "DEFAULTFONTSIZE", "fontsize=");

//------init item
  char root_dir[LLEN];
  parent_dir(root_dir, IupGetGlobal("EXEFILENAME"));
  change_workdir(wdir, root_dir);

  item = item_init();
  item_read(confkv_find_or_default(confkv, "itemfile=", "item.txt"), item);
  item_read(confkv_find_or_default(confkv, "itemfile1=", ""), item);
  item_read(confkv_find_or_default(confkv, "itemfile2=", ""), item);
  item_read(confkv_find_or_default(confkv, "itemfile3=", ""), item);
  item_read(confkv_find_or_default(confkv, "itemfile4=", ""), item);
  item_read(confkv_find_or_default(confkv, "itemfile5=", ""), item);
  item_read(confkv_find_or_default(confkv, "itemfile6=", ""), item);
  item_read(confkv_find_or_default(confkv, "itemfile7=", ""), item);
  item_read(confkv_find_or_default(confkv, "itemfile8=", ""), item);
  item_read(confkv_find_or_default(confkv, "itemfile9=", ""), item);
  /* item_print(item); */

  change_workdir(wdir, workdir);

//------show dialog
  IupShow(dlg);

//------init interface
  lin_fill(lin);
  page = 0;
  lnum = 0;
  disp_item(prompt, "");
  disp_item_select(dlg);
  show_sign(sign);

//------start
  IupMainLoop();

//------stop
  IupClose();

  item_clean(&item);

  confkv_clean(&confkv);

  return EXIT_SUCCESS;
}

