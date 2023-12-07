#include "ply.h"

Ihandle* init_lin(void) {
  Ihandle* lin = IupText(NULL);
  IupSetAttribute(lin, "NAME", "LIN");
  IupSetAttribute(lin, "EXPAND", "NO");
  IupSetAttribute(lin, "VISIBLECOLUMNS", "2");
  IupSetAttribute(lin, "READONLY", "YES");
  IupSetInt(lin, "VISIBLELINES", page_item_max);
  IupSetAttributes(lin, "MULTILINE=YES,SCROLLBAR=NO");
  IupSetAttribute(lin, "VALUE", NULL);
  return lin;
}


char lin_num[3];
void lin_fill(Ihandle* lin) {
  IupSetAttribute(lin, "VALUE", NULL);
  for (int i = 1; i <= page_item_max; ++i) {
    sprintf(lin_num, "%2d", i);
    IupSetAttribute(lin, "APPEND", lin_num);
  }
}


char jump_index[52] = {
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};
void lin_fill_a(Ihandle* lin) {
  IupSetAttribute(lin, "VALUE", NULL);
  for (int i = 0; i < page_item_max; ++i) {
    sprintf(lin_num, "%2c", jump_index[i]);
    IupSetAttribute(lin, "APPEND", lin_num);
  }
}

