#include "ply.h"

void disp_item(Ihandle* prompt, char* input) {
  list_item(item, input);
  if (item_list[0] == NULL) {
    IupSetAttribute(prompt, "VALUE", NULL);
    return;
  }
  IupSetAttribute(prompt, "VALUE", NULL);
  int i;
  for (i = 0; i < page_item_max && item_list[i] ; ++i) {
    IupSetAttribute(prompt, "APPEND", item_list[i]->name);
  }
  return;
}

void disp_item_select(Ihandle* dlg) {
  if (!item_list[lnum]) {
    return;
  }
  Ihandle* prompt = IupGetDialogChild(dlg, "PROMPT");
  IupSetAttribute(prompt, "REMOVEFORMATTING", "ALL");
  Ihandle* selectformat = IupUser();
  char selection[20];
  sprintf(selection, "%d,1:%d,%d", lnum + 1, lnum + 1, strlen(item_list[lnum]->name) + 1);
  /* printf("selection: %s\n", selection); */
  IupSetAttributes(selectformat, "BGCOLOR=\"255 128 64\",FGCOLOR=\"0 0 0\",UNDERLINE=SINGLE,WEIGHT=BOLD");
  iup_try_conf(selectformat, "FGCOLOR", "hl.fgcolor=");
  iup_try_conf(selectformat, "BGCOLOR", "hl.bgcolor=");
  iup_try_conf(selectformat, "UNDERLINE", "hl.underline=");
  iup_try_conf(selectformat, "WEIGHT", "hl.weight=");
  iup_try_conf(selectformat, "ITALIC", "hl.italic=");
  IupSetAttribute(selectformat, "SELECTION", selection);
  IupSetAttribute(prompt, "ADDFORMATTAG_HANDLE", (char*)selectformat);

  Ihandle* comm = IupGetDialogChild(dlg, "COMMENT");
  show_comment(comm, NULL);
  if (item_list[lnum]->sub != NULL) {
    if (item_list[lnum]->sub->cmm) {
      show_comment(comm, item_list[lnum]->sub->cmm);
    }
  }
}
