#include "countdown.h"

Evas_Object *win = NULL;
// static    Evas_Object *popup = NULL;
Evas_Object *ly = NULL;

static    Ecore_Timer *timer_all = NULL;
static    Ecore_Timer *timer_sec = NULL;
static    Ecore_Timer *timer_over = NULL;
// static E_Gadget_Site_Orient gorient;
// static E_Gadget_Site_Anchor ganchor;
   int sec1;
	int sec;
	int min1;
	int min;
	int hour1;
	int hour;

	int hour_new, min_new, sec_new;
	
typedef struct {
        Eina_List   *configlist_eet;
} Counter_List_Eet;


typedef struct {
        int         id;
        const char *name;
        const char *unit;
        double      value;
        double      factor;
		  int         r;
	     int         g;
        int         b;
        int         a;
		  Eina_Bool   bell;
} My_Conf_Type;
   

static const char MY_CONF_FILE_ENTRY[] = "config";

static Eet_Data_Descriptor *_my_conf_descriptor;
static Eet_Data_Descriptor *_my_conf_sub_descriptor;

static void
_my_conf_descriptor_init(void)
{
    Eet_Data_Descriptor_Class eddc;
    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, Counter_List_Eet);
    _my_conf_descriptor = eet_data_descriptor_stream_new(&eddc);
   
    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, My_Conf_Type);
    _my_conf_sub_descriptor = eet_data_descriptor_stream_new(&eddc);
   
    #define MY_CONF_ADD_BASIC(member, eet_type) \
        EET_DATA_DESCRIPTOR_ADD_BASIC             \
        (_my_conf_descriptor, Counter_List_Eet, # member, member, eet_type)
		  
    #define MY_CONF_SUB_ADD_BASIC(member, eet_type) \
        EET_DATA_DESCRIPTOR_ADD_BASIC                 \
        (_my_conf_sub_descriptor, My_Conf_Type, # member, member, eet_type)

    MY_CONF_SUB_ADD_BASIC(id, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(name, EET_T_STRING);
    MY_CONF_SUB_ADD_BASIC(unit, EET_T_STRING);
	 MY_CONF_SUB_ADD_BASIC(value, EET_T_DOUBLE);
	 MY_CONF_SUB_ADD_BASIC(factor, EET_T_DOUBLE);
	 MY_CONF_SUB_ADD_BASIC(r, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(g, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(b, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(a, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(bell, EET_T_UCHAR);

    // And add the sub descriptor as a linked list at 'subs' in the main struct
    EET_DATA_DESCRIPTOR_ADD_LIST
     (_my_conf_descriptor, Counter_List_Eet, "configlist_eet", configlist_eet, _my_conf_sub_descriptor);

     
    #undef MY_CONF_ADD_BASIC
    #undef MY_CONF_SUB_ADD_BASIC
}


void
_read_eet()
{
    Eet_File *ef;
    
    Counter_List_Eet *my_conf;
        
    eet_init();

   char buf[4096], buf2[4096];

   snprintf(buf2, sizeof(buf2), "%s/countdown", efreet_config_home_get());
   ecore_file_mkpath(buf2);
   snprintf(buf, sizeof(buf), "%s/countdown_config.cfg", buf2);
	
    ef = eet_open(buf, EET_FILE_MODE_READ);
    if (!ef)
    {
//         printf("ERROR: could not open '%s' for read\n", home_file);
        return;
    }
    
    my_conf = eet_data_read(ef, _my_conf_descriptor, MY_CONF_FILE_ENTRY);
        
    configlist =  my_conf->configlist_eet;
  
    eet_close(ef);
    eet_shutdown();
}


void
_save_eet()
{
    Eet_File *ef;
    eet_init();
	 
   char buf[4096];
	 
   snprintf(buf, sizeof(buf), "%s/countdown/countdown_config.cfg", efreet_config_home_get());
   
	ef = eet_open(buf, EET_FILE_MODE_WRITE);
       
    if(!ef)
    {
        printf("ERROR\n");
//         fprintf(stderr, "ERROR: could not open '%s' for write\n", home_file);
//            return EINA_FALSE;   //TODO got elm_main -> END
    }else
    {

        Counter_List_Eet *my_conf = calloc(1, sizeof(Counter_List_Eet));

        if (!my_conf)
        {
            fprintf(stderr, "ERROR: could not calloc My_Conf_Type\n");
//                 return NULL;   //TODO got elm_main -> END
        }

		  my_conf->configlist_eet = configlist;
 
        eet_data_write(ef, _my_conf_descriptor, MY_CONF_FILE_ENTRY, my_conf, EINA_TRUE);
    }
    
    eet_close(ef);
    eet_shutdown();
	 
}
/*
static void
_my_conf_descriptor_shutdown(void)
{
    eet_data_descriptor_free(_my_conf_sub_descriptor);
    eet_data_descriptor_free(_my_conf_descriptor);
}*/


static
Eina_Bool _gadget_exit(void *data, int type, void *event_data) 
{
	
	Eina_List *l;
	Config_Item *list_data;
	Ecore_Event_Signal_User *user = event_data;
	
	if ( user->number == 2) 
	{

   EINA_LIST_FOREACH(configlist, l, list_data)
   {
	   if(list_data->id == id_num)
		{
         configlist = eina_list_remove(configlist, list_data);
		}
   }
   printf("DEL ID\n");
	
	_save_eet();
		
	} 

}

/*
static void
orient_change(void *data, Evas_Object *obj, void *event_info)
{
// 	Evas_Object *ly = data;
   gorient = (uintptr_t)event_info;
//    update_anchor_orient(data, gorient, ganchor, obj);
}

static void
anchor_change(void *data, Evas_Object *obj, void *event_info)
{
// 	Evas_Object *ly = data;
   ganchor = (uintptr_t)event_info;
//    update_anchor_orient(data, gorient, ganchor, obj);
}
*/



void
_set_content(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	Evas_Object *ly = data;
   char buf[4096];
	
	hour1 = 0;
	hour = 0;
	min1 = 0;
	min = 0;
	sec1 = 0;
	sec = 0;
	
	edje_object_part_text_set(ly, "time", "00:00:00");
		
	printf("set content\n");
}


void
set_color(Evas_Object *ly)
{
	edje_object_color_class_set(ly, "colorclass", /* class name   */
                               ci_r, ci_g, ci_b, ci_a,  /* Object color */
                               255, 255, 255, 0,   /* Text outline */
                               39, 90, 187, 255);  /* Text shadow  */
	printf("SET COLOR: %i %i %i %i,\n", ci_r,ci_g,ci_b,ci_a);
}


_sec_timer(void *data)
{
   Evas_Object *edje_obj = elm_layout_edje_get(data);
	const char buf[64];
		
	if(sec_new == 0)
		sec_new = 59;
	else
		sec_new--;
	
	if(sec_new == 59 && hour_new >=0)
		if(min_new == 0)
		{
			min_new = 59;
			if(hour_new != 0)
				hour_new--;
		}
		else
			min_new--;
		
		
	if(hour_new == 00)
		snprintf(buf, sizeof(buf), "%02i:%02i", min_new, sec_new);
	else
     snprintf(buf, sizeof(buf), "%02i:%02i:%02i", hour_new, min_new, sec_new);

   edje_object_part_text_set(edje_obj, "time", buf);
   printf("SEC: %i\n", sec_new);
	
   return ECORE_CALLBACK_RENEW;
}


_alarm_over(void *data)
{
   Evas_Object *edje_obj = elm_layout_edje_get(data);
	const char buf[64];
		
	if(sec_new == 59)
		sec_new = 0;
	else
		sec_new++;
	
	if(sec_new == 0 )
		min_new++;
	
// 	if(min_new == 0)
// 		hour_new++;
		
   snprintf(buf, sizeof(buf), "-%02i:%02i:%02i", hour_new, min_new, sec_new);

   edje_object_part_text_set(edje_obj, "unit", buf);
   printf("SEC: %i\n", sec_new);
	
   return ECORE_CALLBACK_RENEW;
}



_alarm_timer(void *data)
{
	const char buf[64];

   Evas_Object *edje_obj = elm_layout_edje_get(data);
	edje_object_part_text_set(edje_obj, "name", "finished");
	
	if(ci_bell == 0)
	   edje_object_signal_emit(edje_obj, "bell_ring", "bell_ring");
	
	hour1 = 0;
	hour = 0;
	min1 = 0;
	min = 0;
	sec1 = 0;
	sec = 0;
	
	hour_new = 0;
	min_new = 0;
	sec_new = 0;
	
   printf("ALARM\n");
	
   if (timer_sec)
	{
	   ecore_timer_del(timer_sec);
	   timer_sec = NULL;
	   timer_all = NULL;
   }

	edje_object_part_text_set(edje_obj, "time", "00:00:00");
	
	   timer_over = ecore_timer_add(1, _alarm_over, ly);
	  
   return ECORE_CALLBACK_CANCEL;
}


void key_down(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Evas_Object *edje_obj = elm_layout_edje_get(data);

   Evas_Event_Key_Down *ev = event_info;
   Eina_Bool ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");
   const char *k = ev->keyname;
	
	
   if(!strcmp(k, "1") || !strcmp(k, "2") || !strcmp(k, "3") || !strcmp(k, "4") || !strcmp(k, "5") || !strcmp(k, "6") || !strcmp(k, "7") || !strcmp(k, "8") || !strcmp(k, "9") || !strcmp(k, "0"))
   {
		if(timer_all != NULL || timer_over != NULL )
			return;
		
      edje_object_part_text_set(edje_obj, "name", "Countdown");
		
		char buf[64];

		hour1 = hour;
		hour = min1;
		min1 = min;
		min = sec1;
		sec1 = sec;
		sec = atoi(k);
				
		snprintf(buf, sizeof(buf), "%i%i:%i%i:%i%i", hour1, hour, min1, min, sec1, sec);
				
	   edje_object_part_text_set(edje_obj, "time", buf);
   }
		
		
   if(!strcmp(k, "Return") || !strcmp(k, "KP_Enter"))
   {
		char time_buf[64];
		
		if(timer_all != NULL || !strcmp(elm_object_part_text_get(ly, "time"), "00:00:00"))
			return;
		
		
		hour_new = hour1*10 + hour;
		min_new = min1*10 + min;
		sec_new = sec1*10 + sec;

		int buf = 0;
		double isec = fmod(sec_new, 60);
		
		if(isec == sec_new)
			printf("");
		else
			min_new++;
			sec_new = isec;

		double imin = fmod(min_new, 60);
		
		if(imin == min_new)
			printf("");
		else
			hour_new++;
			min_new = imin;
		
      int countdown_time = (((hour1*10) + hour)*3600) + (((min1*10) + min)*60) + (((sec1*10) + sec));
		
	   timer_sec = ecore_timer_add(1, _sec_timer, ly);
	   timer_all = ecore_timer_add(countdown_time, _alarm_timer, ly);

      edje_object_part_text_set(edje_obj, "name", ci_name);
   }


   if(!strcmp(k, "BackSpace") || !strcmp(k, "Delete") || !strcmp(k, "Escape"))
   {
		hour1 = 0;
		hour = 0;
		min1 = 0;
		min = 0;
		sec1 = 0;
		sec = 0;
		
      edje_object_part_text_set(edje_obj, "name", "Countdown");
		edje_object_part_text_set(edje_obj, "time", "00:00:00");
		edje_object_part_text_set(edje_obj, "unit", "");

				
		if (timer_all)
		{
		   ecore_timer_del(timer_all);
			timer_all = NULL;
		}
		if (timer_sec)
		{
		   ecore_timer_del(timer_sec);
			timer_sec = NULL;
		}
		if (timer_over)
		{
		   ecore_timer_del(timer_over);
			timer_over = NULL;
		}		
   }
   
   if(!strcmp(k, "space"))
   {
		if(!strcmp(elm_object_part_text_get(ly, "time"), "00:00:00"))
			return;
		
		if(timer_all && ecore_timer_freeze_get(timer_all))
		{
		   ecore_timer_thaw(timer_all);
			edje_object_part_text_set(edje_obj, "name", "");
		}
		else
		{
		   ecore_timer_freeze(timer_all);
         edje_object_part_text_set(edje_obj, "name", "paused");
		}
		
		if(timer_sec && ecore_timer_freeze_get(timer_sec))
		{
		   ecore_timer_thaw(timer_sec);
		}
		else
		{
		   ecore_timer_freeze(timer_sec);
		}
   }

   printf("KEY: %s\n", k);

}


int elm_main(int argc, char *argv[])
{
   char buf[PATH_MAX];
//    char buf2[PATH_MAX];
//    int gadget = 0;
   char buf1[16];
	
	Config_Item *config;
	config = calloc(1, sizeof(Config_Item));
		
		
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
 
   if (getenv("E_GADGET_ID"))
     {
//         gadget = 1;
        snprintf(buf1, sizeof(buf1), "%s", getenv("E_GADGET_ID"));
        id_num = atoi(buf1);
     }

   _my_conf_descriptor_init();
   _read_eet();
	
   //new window
   win = elm_win_add(NULL, "countdown", ELM_WIN_BASIC);
   elm_win_title_set(win, "Counter");
   elm_win_autodel_set(win, EINA_TRUE);
   elm_win_alpha_set(win, EINA_TRUE);	

	evas_object_size_hint_aspect_set(win, EVAS_ASPECT_CONTROL_BOTH, 1, 1);
	
   // LAYOUT CREATE START// 
   ly = elm_layout_add(win);		  
   snprintf(buf, sizeof(buf), "%s/themes/countdown.edj", PACKAGE_DATA_DIR);
	evas_object_size_hint_weight_set(ly, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	

   elm_win_resize_object_add(win, ly);
   evas_object_show(ly);
    // LAYOUT CREATE END// 

   evas_object_data_set(win, "config", config);
	evas_object_resize(win, 50, 50);
	
//     elm_win_resize_object_add(win, ly);
   evas_object_show(win);
	elm_layout_file_set(ly, buf, "countdown");
	
   edje_object_signal_callback_add(ly, "settings", "settings", _settings_2, win);

	
	
	
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
	
// 	evas_object_smart_callback_add(win, "gadget_site_orient", orient_change, ly);
//    evas_object_smart_callback_add(win, "gadget_site_anchor", anchor_change, ly);
   evas_object_smart_callback_add(win, "gadget_configure", _settings_1, edje_obj);
	ecore_event_handler_add(ECORE_EVENT_SIGNAL_USER, _gadget_exit, NULL);
	
    evas_object_event_callback_add(win, EVAS_CALLBACK_KEY_DOWN, key_down, ly);
	
	_config_load(ly);
	
	set_color(edje_obj);
	
	_set_content(edje_obj, NULL, NULL, NULL);

  //run app RUN!
  elm_run();

  //shutdown!
//         _my_conf_descriptor_shutdown();
  return 0;
}

ELM_MAIN();
