#include "ply.h"

Ihandle* init_comment(void) {
  Ihandle* comm =  IupLabel("...");
  IupSetAttribute(comm, "NAME", "COMMENT");
  IupSetAttribute(comm, "EXPAND", "HORIZONTAL");
  return comm;
}

void show_comment(Ihandle* comm, char* commstr) {
  IupSetAttribute(comm, "TITLE", commstr);
}
