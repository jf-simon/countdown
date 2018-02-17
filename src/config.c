#include "countdown.h"

void
_config_load(void *data)
{
   Evas_Object *edje_obj = elm_layout_edje_get(data);
	
   int found = 0;
	Eina_List *l;
	Config_Item *list_data;

   EINA_LIST_FOREACH(configlist, l, list_data)
   {
	   if(list_data->id == id_num)
		{
			ci_name = eina_stringshare_add(list_data->name);
			ci_r = list_data->r;
			ci_g = list_data->g;
			ci_b = list_data->b;
			ci_a = list_data->a;
			ci_bell = list_data->bell;
			ci_vbell = list_data->vbell;
			hour_new = list_data->hour_new;
			min_new = list_data->min_new;
			sec_new = list_data->sec_new;
			diff_save = list_data->diff_save;
			resume = list_data->resume;
			found = 1;
		}
   }

   if(found == 0)
	{
		ci_name = eina_stringshare_add("NAME");
		ci_r = 116;
		ci_g = 13;
		ci_b = 14;
		ci_a = 255;
		ci_bell = 1;
		ci_vbell = 0;
		hour_new = 0;
		min_new = 0;
		min_new = 0;
		resume = 0;
	}

   edje_object_part_text_set(edje_obj, "name", ci_name);
}


void
_config_save(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
	Eina_List *l;
	Config_Item *list_data;
	Config_Item *list_data1;
	list_data1 = calloc(1, sizeof(Config_Item));
	int found = 0;
	
	printf("SAVE\n");
	
	Evas_Object *mainbox = data;
   Evas_Object *ly = evas_object_data_get(mainbox, "ly");

	if(timer_all != NULL)
		resume = 1;
	else
		resume = 0;
		
   diff_save = (unsigned long)time(NULL);
	
	if(data != NULL)
	{
   Evas_Object *en_name = evas_object_data_get(mainbox, "en_name");
   Evas_Object *check_bell = evas_object_data_get(mainbox, "check_bell");
   Evas_Object *check_vbell = evas_object_data_get(mainbox, "check_vbell");
	ci_name = elm_object_text_get(en_name);
   ci_bell = elm_check_state_get(check_bell);
   ci_vbell = elm_check_state_get(check_vbell);
	}

	EINA_LIST_FOREACH(configlist, l, list_data)
   {
	   if(list_data->id == id_num)
		{
			list_data->name = ci_name;
			list_data->r = ci_r;
			list_data->g = ci_g;
			list_data->b = ci_b;
			list_data->a = ci_a;
			list_data->bell = ci_bell;
			list_data->vbell = ci_vbell;
			list_data->hour_new = hour_new;
			list_data->min_new = min_new;
			list_data->sec_new = sec_new;
			list_data->diff_save = diff_save;
			list_data->resume = resume;
			found = 1;
			}
   }

	if(found == 0)
	{
		list_data1->id = id_num;
		list_data1->name = eina_stringshare_add("Countdown");
		list_data1->r = ci_r;
		list_data1->g = ci_g;
		list_data1->b = ci_b;
		list_data1->a = ci_a;
		list_data1->bell = 1;
		list_data1->vbell = 0;
		list_data1->hour_new = hour_new;
		list_data1->min_new = min_new;
		list_data1->sec_new = sec_new;
		list_data1->diff_save = diff_save;
		list_data1->resume = resume;

		configlist = eina_list_append(configlist, list_data1);
	}
	
   edje_object_part_text_set(ly, "name", ci_name);
	
	printf("UNIX TIME %lu\n", (unsigned long)time(NULL));
	_save_eet();
}


static void
_colorselector_changed_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *mainbox = data;
   Evas_Object *ic = evas_object_data_get(mainbox, "ic");
   Evas_Object *ly = evas_object_data_get(mainbox, "ly");

   elm_colorselector_color_get(obj, &ci_r, &ci_g, &ci_b, &ci_a);
	
	evas_object_color_set(ic, ci_r, ci_g, ci_b, ci_a);
	
	set_color(ly);
}


static void
_popup_del(void *data EINA_UNUSED, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_data EINA_UNUSED)
{
   evas_object_del(obj);
}


void
_settings_1(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
	Evas_Object *win = obj;
	Evas_Object *ly = data;
	
	_settings(win, ly, NULL);
}


void
_settings_2(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	Evas_Object *win = data;
	Evas_Object *ly = obj;
	
	_settings(win, ly, NULL);
}


void
_settings(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *en_name, *popup, *fr, *cs, *lbl, *fr_help;
   Evas_Object *o, *mainbox, *box_settings, *box_name;
   Evas_Object *check_bell, *check_vbell;

	Evas_Object *ly = obj;
	Evas_Object *win = data;
		  
   popup = elm_win_add(win, "win", ELM_WIN_BASIC);
   elm_win_alpha_set(popup, 1);
	
   mainbox = elm_box_add(popup);
	
	
   elm_box_horizontal_set(mainbox, EINA_FALSE);
   E_EXPAND(mainbox);
   E_ALIGN(mainbox, 0.0, 0.0);
   evas_object_show(mainbox);
	
	
   fr = elm_frame_add(mainbox);
   elm_object_text_set(fr, "Countdown Settings");
   E_EXPAND(fr);
   E_ALIGN(fr, 0.5, 0.5);
   E_FILL(fr);
   evas_object_show(fr);

   box_settings = elm_box_add(fr);
   elm_box_horizontal_set(box_settings, EINA_FALSE);
   E_EXPAND(box_settings);
   evas_object_show(box_settings);
	
		   box_name = elm_box_add(box_settings);
			elm_box_horizontal_set(box_name, EINA_TRUE);
			E_EXPAND(box_name);
			E_ALIGN(box_name, 0.0, 0.5);
			evas_object_show(box_name);
			
			lbl = elm_label_add(box_name);
			elm_object_text_set(lbl, "Name: ");
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_box_pack_end(box_name, lbl);
			evas_object_show(lbl);
			evas_object_data_set(mainbox, "lbl", lbl);
			
			en_name = elm_entry_add(box_name);
			elm_config_context_menu_disabled_set(EINA_TRUE);
			elm_object_text_set(en_name, ci_name);
			elm_entry_editable_set(en_name, EINA_TRUE);
			elm_entry_single_line_set(en_name, EINA_TRUE);
			evas_object_size_hint_weight_set(en_name, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(en_name, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_box_pack_end(box_name, en_name);
			evas_object_show(en_name);
			evas_object_data_set(mainbox, "en_name", en_name);
			
			elm_box_pack_end(box_settings, box_name);
	

	o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);

	check_bell = elm_check_add(box_settings);
	elm_object_text_set(check_bell, "Disable Audiobell");
   elm_check_state_set(check_bell, ci_bell);
   E_ALIGN(check_bell, 0.0, 0.0);
 	E_WEIGHT(check_bell, EVAS_HINT_EXPAND, 0);
	elm_box_pack_end(box_settings, check_bell);
	evas_object_show(check_bell);
   evas_object_data_set(mainbox, "check_bell", check_bell);	

	
	o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	
	check_vbell = elm_check_add(box_settings);
	elm_object_text_set(check_vbell, "Disable Visualbell");
   elm_check_state_set(check_vbell, ci_vbell);
   E_ALIGN(check_vbell, 0.0, 0.0);
 	E_WEIGHT(check_vbell, EVAS_HINT_EXPAND, 0);
	elm_box_pack_end(box_settings, check_vbell);
	evas_object_show(check_vbell);
   evas_object_data_set(mainbox, "check_vbell", check_vbell);	
	
	
	o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	
	cs = elm_colorselector_add(box_settings);
   evas_object_size_hint_weight_set(cs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(cs, EVAS_HINT_FILL, EVAS_HINT_FILL);

	elm_colorselector_mode_set(cs, ELM_COLORSELECTOR_BOTH);
	elm_colorselector_palette_name_set(cs, "countdown");
	elm_colorselector_palette_color_add(cs, 253, 232, 82, 255);
   elm_colorselector_palette_color_add(cs, 255, 186, 0, 255);
   elm_colorselector_palette_color_add(cs, 223, 147, 37, 255);
   elm_colorselector_palette_color_add(cs, 238, 119, 99, 255);
   elm_colorselector_palette_color_add(cs, 164, 182, 166, 255);
   elm_colorselector_palette_color_add(cs, 146, 175, 29, 255);
   elm_colorselector_palette_color_add(cs, 41, 68, 59, 255);
   elm_colorselector_palette_color_add(cs, 0, 109, 128, 255);
   elm_colorselector_palette_color_add(cs, 11, 54, 71, 255);
   elm_colorselector_palette_color_add(cs, 41, 46, 68, 255);
   elm_colorselector_palette_color_add(cs, 116, 13, 14, 255);
   elm_colorselector_palette_color_add(cs, 101, 33, 44, 255);
   elm_colorselector_palette_color_add(cs, 132, 50, 118, 255);
   elm_colorselector_palette_color_add(cs, 68, 40, 55, 255);
   elm_colorselector_palette_color_add(cs, 63, 37, 60, 255);
   elm_colorselector_palette_color_add(cs, 115, 87, 63, 255);
   elm_colorselector_palette_color_add(cs, 66, 70, 73, 255);
   elm_colorselector_palette_color_add(cs, 0, 0, 0, 255);
   elm_box_pack_end(box_settings, cs);
   evas_object_show(cs);
	evas_object_data_set(mainbox, "cs", cs);

   elm_object_content_set(fr, box_settings);
   elm_box_pack_end(mainbox, fr);

   fr_help = elm_frame_add(mainbox);
   elm_object_text_set(fr_help, "Help");
   E_EXPAND(fr_help);
   E_ALIGN(fr_help, 0.5, 0.5);
   E_FILL(fr_help);
   evas_object_show(fr_help);

   box_settings = elm_box_add(fr_help);
   elm_box_horizontal_set(box_settings, EINA_FALSE);
   E_EXPAND(box_settings);
   evas_object_show(box_settings);
	

   lbl = elm_label_add(box_settings);
   elm_object_text_set(lbl, "<custom align=left>"
									"CLICK into gadget to activate it<br>"
									"- TYPE to set the time<br>"
									"- ENTER to start countdown<br>"
									"- BACKSPACE to delete time<br>"
									"- ESC/DEL to clear time values<br>"
									"- SPACE to pause/resume countdown<br>"
									"<br>"
									"- MOUSE WHEEL on \"hour:min:sec\" sets time<br>"
									"- KLICK an \"name\" to start countdown<br>"
									"</custom>");
   evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box_settings, lbl);
	evas_object_show(lbl);
   evas_object_data_set(mainbox, "lbl", lbl);	


   o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);


   elm_object_content_set(fr_help, box_settings);
   elm_box_pack_end(mainbox, fr_help);
	
	evas_object_smart_callback_add(cs, "changed", _colorselector_changed_cb, mainbox); 
	

   evas_object_data_set(mainbox, "win", win);
   evas_object_data_set(mainbox, "ly", ly);
	
   elm_object_content_set(popup, mainbox);
	
	evas_object_event_callback_add(popup, EVAS_CALLBACK_MOUSE_OUT, _config_save, mainbox);
	evas_object_event_callback_add(popup, EVAS_CALLBACK_HIDE, _popup_del, NULL);
   evas_object_show(popup);
}
