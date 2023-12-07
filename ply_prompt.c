#include "ply.h"

#ifdef TEST_PROMPT
#define DEBUG
#else
#define DEBUG while(0)
#endif

int run_by_leftclick = 0;

Ihandle* init_prompt(void) {
  Ihandle* prompt = IupText(NULL);
  IupSetAttribute(prompt, "NAME", "PROMPT");
  IupSetAttribute(prompt, "EXPAND", "HORIZONTAL");
  IupSetAttribute(prompt, "READONLY", "YES");
  IupSetInt(prompt, "VISIBLELINES", page_item_max);
  IupSetAttributes(prompt, "MULTILINE=YES,SCROLLBAR=NO");
  IupSetAttribute(prompt, "FORMATTING", "YES");
  IupSetCallback(prompt, "BUTTON_CB", (Icallback)prompt_button_cb);
  return prompt;
}

int sx, sy; // start x, y position
int lin, col;

int prompt_button_cb(Ihandle* self, int button, int pressed, int x, int y, char* status) {
  Ihandle* dlg = IupGetDialog(self);
  // left mouse button released
  if (button == IUP_BUTTON1 && pressed == 0) {
    sscanf(IupGetAttribute(self, "CARET"), "%d,%d", &lin, &col);
    DEBUG printf("select line: %d\n", lin);
    lnum = lin - 1;
    disp_item_select(self);
    if (run_by_leftclick)
      run_cmd(dlg, item_list[lnum]);
  }
  // right mouse button pressed
  if (button == IUP_BUTTON3 && pressed == 1) {
    sx = x;
    sy = y;
  }
  // right mouse button released
  if (button == IUP_BUTTON3 && pressed == 0) {
    Ihandle* prompt = IupGetDialogChild(dlg, "PROMPT");
    Ihandle* input = IupGetDialogChild(dlg, "INPUT");
    char* text = IupGetAttribute(input, "VALUE");
    Ihandle* sign = IupGetDialogChild(dlg, "SIGN");
    if (x - sx < 0) {
      // right mouse drag to left hand
      page++;
      lnum = 0;
      disp_item(prompt, text);
      disp_item_select(dlg);
      show_sign(sign);
    } else if (x - sx > 0) {
      // right mouse drag to right hand
      if (page > 0) {
        page--;
        lnum = 0;
        disp_item(prompt, text);
        disp_item_select(dlg);
        show_sign(sign);
      }
    } else {
      run_cmd(dlg, item_list[lnum]);
      // ignore right mouse click
      return IUP_IGNORE;
    }
  }
  // middle mouse button released
  if (button == IUP_BUTTON2 && pressed == 0) {
    show_item_info(item_list[lnum]);
  }
  return IUP_DEFAULT;
}

#undef DEBUG
