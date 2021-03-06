#ifndef TAGESLOSUNG_H
#define TAGESLOSUNG_H

#endif
#include "config.h"

#include <Ecore_Con.h>
#include <Elementary.h>

#include <e_gadget_types.h>

#define E_WEIGHT evas_object_size_hint_weight_set
#define E_ALIGN  evas_object_size_hint_align_set
#define E_EXPAND(X) E_WEIGHT((X), EVAS_HINT_EXPAND, EVAS_HINT_EXPAND)
#define E_FILL(X)   E_ALIGN((X), EVAS_HINT_FILL, EVAS_HINT_FILL)

void          _settings_1(void *data, Evas_Object *obj, void *event_info EINA_UNUSED);
void          _settings_2(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);
void          _settings(void *data, Evas_Object *obj, void *event_info EINA_UNUSED);
void          _sl_font_changed(void *data, Evas_Object *obj, void *event_info EINA_UNUSED);
void          _set_text(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);
int           _xml_parse(void *data);
void          set_color(Evas_Object *ly);
void          _read_eet();
void          _save_eet();
unsigned int  createHEX(int r, int g, int b, int a);
void         _config_load();
void         _config_save(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED);
void         _set_content(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);

int        id_num;

Ecore_Timer *timer_all;
Ecore_Timer *timer_sec;
Ecore_Timer *timer_over;
Ecore_Timer *timer_stopwatch;

Evas_Object *win;
Evas_Object *ly;

int         ci_id;
const char *ci_name;
const char *name;
int         ci_r;
int         ci_g;
int         ci_b;
int         ci_a;
Eina_Bool   ci_bell;
Eina_Bool   ci_vbell;
int 			hour_new;
int 			min_new;
int			sec_new;
unsigned long diff_save;
Eina_Bool   resume;

Eina_List *configlist;


typedef struct {
        int         id;
        const char *name;
		  int         r;
	     int         g;
        int         b;
        int         a;
		  Eina_Bool   bell;
		  Eina_Bool   vbell;
		  int         hour_new;
		  int         min_new;
		  int         sec_new;
		  unsigned long diff_save;
		  Eina_Bool   resume;
} Config_Item;

