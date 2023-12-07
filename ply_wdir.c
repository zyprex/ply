#include <unistd.h>
#include "ply.h"
char workdir[LLEN];

Ihandle* init_wdir(void) {
  Ihandle* wdir =  IupLabel(NULL);
  IupSetAttribute(wdir, "NAME", "WDIR");
  IupSetCallback(wdir, "BUTTON_CB", (Icallback)wdir_button_cb);
  show_workdir(wdir);
  return wdir;
}

void show_workdir(Ihandle* wdir) {
  getcwd(workdir, LLEN);
  IupSetAttribute(wdir, "TITLE", workdir);
}

void change_workdir(Ihandle* wdir, char* dir) {
  chdir(dir);
  show_workdir(wdir);
}

int wdir_change_txt_keypress_cb(Ihandle* self, int c) {
  if (c == K_ESC) {
    return IUP_CLOSE;
  }
  if (c == K_CR) {
    char* path = IupGetAttribute(self, "VALUE");
    strcpy(workdir, path);
    return IUP_CLOSE;
  }
  return IUP_DEFAULT;
}

int wdir_button_cb(Ihandle* self, int button, int pressed, int x, int y, char* status) {
  if (button == IUP_BUTTON1 && pressed == 0) {
    Ihandle* wdir_change_txt = IupText(NULL);
    IupSetAttribute(wdir_change_txt, "SIZE", "120");
    IupSetCallback(wdir_change_txt, "K_ANY", (Icallback)wdir_change_txt_keypress_cb);
    Ihandle* wdir_change_dlg = IupDialog(wdir_change_txt);
    IupSetAttribute(wdir_change_dlg, "TITLE", PLY_TITLE_CD);
    IupPopup(wdir_change_dlg, IUP_CENTER, IUP_CENTER);
    change_workdir(self, workdir);
  }
  return IUP_DEFAULT;
}
