#include "ply.h"

char sign_str[80];

Ihandle* init_sign(void) {
  Ihandle* sign =  IupLabel("Page 1");
  IupSetAttribute(sign, "NAME", "SIGN");
  IupSetAttribute(sign, "ALIGNMENT", "ARIGHT:ATOP");
  IupSetAttribute(sign, "EXPAND", "YES");
  return sign;
}

void show_sign(Ihandle* sign) {
  sprintf(sign_str, "%s  %s  Page %d",
      match_case ? "CASE" : "",
      match_sign[match_mode],
      page + 1);
  IupSetAttribute(sign, "TITLE", sign_str);
}
