#include "ply.h"

#ifdef TEST_INPUT_EXTRA
#define DEBUG
#else
#define DEBUG while(0)
#endif


static int var_cnt = 0;

static char* str_replace(char* orig, char* var, char* s);
static void varlist_fill(char vars[1024], char varlist[10][50]);

/* replace `var` to `s` in `orig`, if success, free `orig` and return new string */
static char* str_replace(char* orig, char* var, char* s) {
  char* before = strstr(orig, var);
  if (!before) {
    return orig;
  }
  int blen = before - orig;
  int vlen = strlen(var);
  char* ret = (char*)malloc((strlen(orig) + strlen(s) - strlen(var)) * sizeof(char));
  strncpy(ret, orig, blen);
  ret[blen] = '\0';
  strcat(ret, s);
  if (blen + vlen < strlen(orig)) {
    strcat(ret, orig + blen + vlen);
  }
  free(orig);
  orig = NULL;
  return ret;
}

static void varlist_fill(char vars[1024], char varlist[10][50]) {
  char* delim = ",";
  char* vcur = NULL;
  vcur = strtok(vars, delim);
  int i = 0;
  while (vcur != NULL) {
    int vlen = strlen(vcur);
    if (vlen < 50) {
      strcpy(varlist[i++], vcur);
    } else {
      strncpy(varlist[i++], vcur, 50);
    }
    var_cnt = i;
    if (i > 9) {
      var_cnt = 9;
      break;
    }
    vcur = strtok(NULL, delim);
  }
}

int vntext_keypress_cb(Ihandle* self, int c) {
  Ihandle* dlg = IupGetDialog(self);
  if (c == K_ESC) {
    IupDestroy(dlg);
  }

  if (c == K_CR) {

    char* cmd = IupGetAttribute(dlg, "vCMD");
    char* params = IupGetAttribute(dlg, "vPARAMS");

    char* new_params = (char*)malloc(strlen(params) * sizeof(char));
    strcpy(new_params, params);

    char vnname[3];
    for (int i = 0; i < var_cnt; ++i) {
      sprintf(vnname, "V%d", i);
      Ihandle* vntext = IupGetDialogChild(dlg, vnname);
      char* text = IupGetAttribute(vntext, "VALUE");
      char* var = IupGetAttribute(vntext, "vStr");
      /* printf("%s [%s]\n", vnname, IupGetAttribute(vntext, "VALUE")); */
      new_params = str_replace(new_params, var, text);
    }
    /* printf(":%s %s\n", cmd, new_params); */
    try_execute(cmd, new_params);
    free(new_params);
    new_params = NULL;
    /* return IUP_CLOSE; */
    IupDestroy(dlg);
  }
  return IUP_DEFAULT;
}

void show_input_extra(const char* cmd, const char* params, const char* var) {
  Ihandle *dlg, *vbox, *label;
  label = IupLabel(cmd);
  vbox = IupVbox(label);
  dlg = IupDialog(vbox);

  IupSetAttribute(dlg, "MARGIN", "2x5");
  IupSetAttribute(dlg, "GAP", "5");
  IupSetAttribute(dlg, "TITLE", PLY_TITLE_EXTRA);

  IupSetAttribute(dlg, "vCMD", cmd);
  IupSetAttribute(dlg, "vPARAMS", params);

  // convert  string var to a string list var
  char vars[1024];
  strcpy(vars, var);
  char varlist[10][50];
  varlist_fill(vars, varlist);

  // accroding to string list length build input controls
  char vnname[3];
  Ihandle *vnlabel, *vntext, *vnhhox;

  for (int i = 0; i < 1; ++i) {
    sprintf(vnname, "V%d", i);
    vnlabel = IupLabel(varlist[i]);
    vntext = IupText(NULL);
    vnhhox = IupHbox(vnlabel, vntext);
    IupAppend(vbox, vnhhox);

    IupSetStrAttribute(vntext, "NAME", vnname);
    IupSetStrAttribute(vntext, "vStr", varlist[i]);
    IupSetStrAttribute(vntext, "SIZE", "120");
    IupSetCallback(vntext, "K_ANY", (Icallback)vntext_keypress_cb);
  }


  IupShow(dlg);
}

#undef DEBUG
