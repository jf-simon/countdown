#include "countdown.h"

Evas_Object *win = NULL;
// static    Evas_Object *popup = NULL;
Evas_Object *ly = NULL;

static    Ecore_Timer *timer_all = NULL;
static    Ecore_Timer *timer_sec = NULL;
static    Ecore_Timer *timer_over = NULL;
static    Ecore_Timer *timer_stopwatch = NULL;
// static E_Gadget_Site_Orient gorient;
// static E_Gadget_Site_Anchor ganchor;
   int mil_sec;
   int sec1;
	int sec;
	int min1;
	int min;
	int hour1;
	int hour;

	
typedef struct {
        Eina_List   *configlist_eet;
} Counter_List_Eet;


typedef struct {
        int         id;
        const char *name;
		  int         r;
	     int         g;
        int         b;
        int         a;
		  Eina_Bool   bell;
// 		  int			  min_new;
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
//     MY_CONF_SUB_ADD_BASIC(unit, EET_T_STRING);
// 	 MY_CONF_SUB_ADD_BASIC(value, EET_T_DOUBLE);
// 	 MY_CONF_SUB_ADD_BASIC(factor, EET_T_DOUBLE);
	 MY_CONF_SUB_ADD_BASIC(r, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(g, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(b, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(a, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(bell, EET_T_UCHAR);
//     MY_CONF_SUB_ADD_BASIC(min_new, EET_T_INT);

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


static Eina_Bool 
_gadget_exit(void *data, int type, void *event_data) 
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
return EINA_TRUE;
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

static void
_values_to_zero()
{
	hour1 = 0;
	hour = 0;
	min1 = 0;
	min = 0;
	sec1 = 0;
	sec = 0;
	
	hour_new = 0;
	min_new = 0;
	sec_new = 0;
}

void
_set_content(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	Evas_Object *ly = data;
   char buf[4096];
	
	_values_to_zero();
	
     snprintf(buf, sizeof(buf), "%02i:%02i:%02i", hour_new, min_new, sec_new);
// 	edje_object_part_text_set(ly, "time", "00:00:00");
		
	edje_object_part_text_set(ly, "time", buf);
		
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



void
_hour_increase(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	if(timer_stopwatch != NULL || timer_all != NULL)
		return;
	
	if(hour < 9 && hour1 < 9)
	{
		hour++;
	}
	else
	{
		hour = 0;
		if(hour1 <=8)
			hour1++;
	}
		
		char buf[64];
		snprintf(buf, sizeof(buf), "%i%i:%i%i:%i%i", hour1, hour, min1, min, sec1, sec);
	   edje_object_part_text_set(obj, "time", buf);
}


void
_hour_decrease(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	if(timer_stopwatch != NULL || timer_all != NULL)
		return;
	
	if(hour > 0)
	{
		hour--;
	}
	else
	{
		if(hour1 != 0)
			hour = 9;
		
		if(hour1 != 0)
			hour1--;
	}
		char buf[64];
		snprintf(buf, sizeof(buf), "%i%i:%i%i:%i%i", hour1, hour, min1, min, sec1, sec);
	   edje_object_part_text_set(obj, "time", buf);
}





void
_min_increase(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	if(timer_stopwatch != NULL || timer_all != NULL)
		return;
	
	if(min < 9 && min1 < 9)
	{
		min++;
	}
	else
	{
		min = 0;
		if(min1 <=8)
			min1++;
	}
		
		char buf[64];
		snprintf(buf, sizeof(buf), "%i%i:%i%i:%i%i", hour1, hour, min1, min, sec1, sec);
	   edje_object_part_text_set(obj, "time", buf);
}


void
_min_decrease(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	if(timer_stopwatch != NULL || timer_all != NULL)
		return;
	
	if(min > 0)
	{
		min--;
	}
	else
	{
		if(min1 != 0)
			min = 9;
		
		if(min1 != 0)
			min1--;
	}
		char buf[64];
		snprintf(buf, sizeof(buf), "%i%i:%i%i:%i%i", hour1, hour, min1, min, sec1, sec);
	   edje_object_part_text_set(obj, "time", buf);
}



void
_sec_increase(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	if(timer_stopwatch != NULL || timer_all != NULL)
		return;
	
	if(sec < 9 && sec1 < 9)
	{
		sec++;
	}
	else
	{
		sec = 0;
		if(sec1 <=8)
			sec1++;
	}
		
		char buf[64];
		snprintf(buf, sizeof(buf), "%i%i:%i%i:%i%i", hour1, hour, min1, min, sec1, sec);
	   edje_object_part_text_set(obj, "time", buf);
}


void
_sec_decrease(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	if(timer_stopwatch != NULL || timer_all != NULL)
		return;
		
	if(sec > 0)
	{
		sec--;
	}
	else
	{
		if(sec1 != 0)
			sec = 9;
		
		if(sec1 != 0)
			sec1--;
	}
		char buf[64];
		snprintf(buf, sizeof(buf), "%i%i:%i%i:%i%i", hour1, hour, min1, min, sec1, sec);
	   edje_object_part_text_set(obj, "time", buf);
}

static Eina_Bool
_sec_timer(void *data)
{
   Evas_Object *edje_obj = elm_layout_edje_get(data);
	char buf[64];
		
	if(sec_new == 0)
		sec_new = 59;
	else
		sec_new--;
	
	if((sec_new == 59) && (hour_new >=0))
	{
		if(min_new == 0)
		{
			min_new = 59;
			if(hour_new != 0)
				hour_new--;
		}
		else
			min_new--;
	}	
		
	if(hour_new == 00)
		snprintf(buf, sizeof(buf), "%02i:%02i", min_new, sec_new);
	else
     snprintf(buf, sizeof(buf), "%02i:%02i:%02i", hour_new, min_new, sec_new);

   edje_object_part_text_set(edje_obj, "time", buf);
   printf("SEC: %i\n", sec_new);
	
   return ECORE_CALLBACK_RENEW;
}

static Eina_Bool
_alarm_over(void *data)
{
   Evas_Object *edje_obj = elm_layout_edje_get(data);
	char buf[64];
		

	
	if(sec_new == 59)
		sec_new = 0;
	else
		sec_new++;
	
	if(sec_new == 0 )
		min_new++;
	
// 	if(min_new == 0)
// 		hour_new++;

// 	if((sec_new == 0) && (ci_bell == 0))
// 	   edje_object_signal_emit(edje_obj, "bell_ring", "bell_ring");
	
   snprintf(buf, sizeof(buf), "-%02i:%02i:%02i", hour_new, min_new, sec_new);

   edje_object_part_text_set(edje_obj, "unit", buf);
   printf("SEC OVER: %i\n", sec_new);
	

	
   return ECORE_CALLBACK_RENEW;
}


static Eina_Bool
_stopwatch_run(void *data)
{
   Evas_Object *edje_obj = elm_layout_edje_get(data);
	char buf[64];
	
	if(mil_sec == 99)
	{
		mil_sec = 0;
		sec_new++;
	}else
	{
		mil_sec++;
	}
	
	if(sec_new == 59)
	{
		sec_new = 0;
		min_new++;
	}
	
	if(min_new == 59)
	{
		hour_new++;
	}
	
	
   snprintf(buf, sizeof(buf), "%02i:%02i:%02i:%02i", hour_new, min_new, sec_new, mil_sec);

   edje_object_part_text_set(edje_obj, "time", buf);
	
   return ECORE_CALLBACK_RENEW;
}


static Eina_Bool
_alarm_timer(void *data)
{
	char buf[64];

   snprintf(buf, sizeof(buf), "%s done", ci_name);
	
   Evas_Object *edje_obj = elm_layout_edje_get(data);
	
	edje_object_part_text_set(edje_obj, "name", buf);
	
	if(ci_bell == 0)
	   edje_object_signal_emit(edje_obj, "bell_ring", "bell_ring");
	

	_values_to_zero();
	
   printf("ALARM\n");
	
   if (timer_sec)
	{
	   ecore_timer_del(timer_sec);
	   timer_sec = NULL;
	   timer_all = NULL;
   }

	edje_object_part_text_set(edje_obj, "time", "00:00:00");
	
	   timer_over = ecore_timer_add(1.0, _alarm_over, ly);
	  
   return ECORE_CALLBACK_CANCEL;
}

void key_down1(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
//    Evas_Object *edje_obj = elm_layout_edje_get(data);

   Evas_Event_Key_Down *ev = event_info;
//    Eina_Bool ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");
   const char *k = ev->keyname;
	
	if(!strcmp(k, "s"))
   {
// 	my_fl_6(data, NULL, NULL);
	printf("FLIP\n");
	}
}


void
_pause_countdown(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
		char buf[64];

		Evas_Object *edje_obj = elm_layout_edje_get(data);
		
      if(timer_all == NULL || !strcmp(elm_object_part_text_get(ly, "time"), "00:00:00"))
			return;
		
		if(timer_all && ecore_timer_freeze_get(timer_all))
		{
		   ecore_timer_thaw(timer_all);
			edje_object_part_text_set(edje_obj, "name", ci_name);
		}
		else
		{
		   ecore_timer_freeze(timer_all);
			
			snprintf(buf, sizeof(buf), "%s paused", ci_name);
         edje_object_part_text_set(edje_obj, "name", buf);
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
	

void
_start_countdown(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
   Evas_Object *edje_obj = elm_layout_edje_get(data);
		
		if(timer_all != NULL || !strcmp(elm_object_part_text_get(ly, "time"), "00:00:00"))
			return;
		
// 		if(timer_all != NULL && strcmp(elm_object_part_text_get(ly, "name"), "Countdown"))
// 		{
// 			_pause_countdown(data, NULL, NULL, NULL);
// 			return;
// 		}
		
		hour_new = hour1*10 + hour;
		min_new = min1*10 + min;
		sec_new = sec1*10 + sec;

		double isec = fmod(sec_new, 60);
		
		if(isec == sec_new)
			printf(" ");
		else
		{
			min_new++;
			sec_new = isec;
		}
			
		double imin = fmod(min_new, 60);
		
		if(imin == min_new)
			printf(" ");
		else
		{
			hour_new++;
			min_new = imin;
		}
      double countdown_time = (((hour1*10) + hour)*3600) + (((min1*10) + min)*60) + (((sec1*10) + sec));
		
	   timer_sec = ecore_timer_add(1.0, _sec_timer, ly);
	   timer_all = ecore_timer_add(countdown_time, _alarm_timer, ly);

      edje_object_part_text_set(edje_obj, "name", ci_name);
}


void key_down(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Evas_Object *edje_obj = elm_layout_edje_get(data);

   Evas_Event_Key_Down *ev = event_info;
//    Eina_Bool ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");
   const char *k = ev->keyname;
	
	
   if(!strcmp(k, "1") || !strcmp(k, "2") || !strcmp(k, "3") || !strcmp(k, "4") || !strcmp(k, "5") || !strcmp(k, "6") || !strcmp(k, "7") || !strcmp(k, "8") || !strcmp(k, "9") || !strcmp(k, "0") ||
		!strcmp(k, "KP_Insert") || !strcmp(k, "KP_End") || !strcmp(k, "KP_Down") || !strcmp(k, "KP_Next") || !strcmp(k, "KP_Left") || !strcmp(k, "KP_Begin") || !strcmp(k, "KP_Right") || !strcmp(k, "KP_Home") || !strcmp(k, "KP_Up") || !strcmp(k, "KP_Prior"))
   {
		if(timer_all != NULL || timer_over != NULL )
			return;
	
		const char *key;
		
		if(!strcmp(k, "1") || !strcmp(k, "2") || !strcmp(k, "3") || !strcmp(k, "4") || !strcmp(k, "5") || !strcmp(k, "6") || !strcmp(k, "7") || !strcmp(k, "8") || !strcmp(k, "9") || !strcmp(k, "0"))
			key = k;
		else if(!strcmp(k, "KP_Insert")) key = "0";
		else if(!strcmp(k, "KP_End")) key = "1";
		else if(!strcmp(k, "KP_Down")) key = "2";
		else if(!strcmp(k, "KP_Next")) key = "3";
		else if(!strcmp(k, "KP_Left")) key = "4";
		else if(!strcmp(k, "KP_Begin")) key = "5";
		else if(!strcmp(k, "KP_Right")) key = "6";
		else if(!strcmp(k, "KP_Home")) key = "7";
		else if(!strcmp(k, "KP_Up")) key = "8";
		else if(!strcmp(k, "KP_Prior")) key = "9";
		

		if(!strcmp(ci_name, ""))
		   edje_object_part_text_set(edje_obj, "name", "Countdown");
		else
			edje_object_part_text_set(edje_obj, "name", ci_name);
		
		char buf[64];

		hour1 = hour;
		hour = min1;
		min1 = min;
		min = sec1;
		sec1 = sec;
		sec = atoi(key);
				
		snprintf(buf, sizeof(buf), "%i%i:%i%i:%i%i", hour1, hour, min1, min, sec1, sec);
				
	   edje_object_part_text_set(edje_obj, "time", buf);
   }
		
		
   if(!strcmp(k, "Return") || !strcmp(k, "KP_Enter"))
   {
		_start_countdown(data, NULL, NULL, NULL);
/*		
		if(timer_all != NULL || !strcmp(elm_object_part_text_get(ly, "time"), "00:00:00"))
			return;
		
		
		hour_new = hour1*10 + hour;
		min_new = min1*10 + min;
		sec_new = sec1*10 + sec;

		double isec = fmod(sec_new, 60);
		
		if(isec == sec_new)
			printf(" ");
		else
		{
			min_new++;
			sec_new = isec;
		}
			
		double imin = fmod(min_new, 60);
		
		if(imin == min_new)
			printf(" ");
		else
		{
			hour_new++;
			min_new = imin;
		}
      double countdown_time = (((hour1*10) + hour)*3600) + (((min1*10) + min)*60) + (((sec1*10) + sec));
		
	   timer_sec = ecore_timer_add(1.0, _sec_timer, ly);
	   timer_all = ecore_timer_add(countdown_time, _alarm_timer, ly);

      edje_object_part_text_set(edje_obj, "name", ci_name);
*/
   }
/*
   if(!strcmp(k, "BackSpace"))
   {		

	}
		*/
		
   if(!strcmp(k, "Delete") || !strcmp(k, "Escape") || !strcmp(k, "BackSpace"))
   {		
		if(timer_stopwatch == NULL && timer_all == NULL && timer_over == NULL && !strcmp(k, "BackSpace"))
		{		
		   char buf[64];
		
		   sec = sec1;
			sec1 = min;
		   min = min1;
		   min1 = hour;
		   hour = hour1;
   	   hour1 = 0;
		
		   snprintf(buf, sizeof(buf), "%i%i:%i%i:%i%i", hour1, hour, min1, min, sec1, sec);
			
			if(!strcmp(ci_name, ""))
			   edje_object_part_text_set(edje_obj, "name", "Countdown");
			else
				edje_object_part_text_set(edje_obj, "name", ci_name);
			
		}else
		{		
			_values_to_zero();
			
			if(!strcmp(ci_name, ""))
			   edje_object_part_text_set(edje_obj, "name", "Countdown");
			else
				edje_object_part_text_set(edje_obj, "name", ci_name);
			
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
			if (timer_stopwatch)
			{
				ecore_timer_del(timer_stopwatch);
				timer_stopwatch = NULL;
			}
			if (timer_over)
			{
				ecore_timer_del(timer_over);
				timer_over = NULL;
			}
		}
   }
   
   if(!strcmp(k, "space"))
   {
		_pause_countdown(data, NULL, NULL, NULL);
   }
   
   
   if(!strcmp(k, "s"))
   {
		if(timer_stopwatch)
		{
		   ecore_timer_del(timer_stopwatch);
			timer_stopwatch = NULL;
         edje_object_part_text_set(edje_obj, "time", "00:00:00:000");

			_values_to_zero();
		}
		else
		{
			timer_stopwatch = ecore_timer_add(0.01, _stopwatch_run, ly);
         edje_object_part_text_set(edje_obj, "name", "Stopwatch");
		}
   }
   printf("KEY: %s\n", k);
}

/*
void
my_fl_6(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Evas_Object *fl = data;
   elm_flip_go(fl, ELM_FLIP_CUBE_RIGHT);
}*/


int elm_main(int argc, char *argv[])
{
   char buf[PATH_MAX];
//    char buf2[PATH_MAX];
//    int gadget = 0;
   char buf1[16];
	
// 	Config_Item *config;
// 	config = calloc(1, sizeof(Config_Item));
		
		
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
   elm_win_title_set(win, "Countdown");
   elm_win_autodel_set(win, EINA_TRUE);
   elm_win_alpha_set(win, EINA_TRUE);	

	evas_object_size_hint_aspect_set(win, EVAS_ASPECT_CONTROL_BOTH, 1, 1);
	
   // LAYOUT CREATE START// 
   ly = elm_layout_add(win);		  
   snprintf(buf, sizeof(buf), "%s/themes/countdown.edj", PACKAGE_DATA_DIR);
	evas_object_size_hint_weight_set(ly, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, ly);
    // LAYOUT CREATE END// 

//    evas_object_data_set(win, "config", config);
	evas_object_resize(win, 50, 50);

//     elm_win_resize_object_add(win, ly);
	elm_layout_file_set(ly, buf, "countdown");
	
   evas_object_show(ly);
/*	
	///////////////////////////////////////////////////////////////////////////////////////////7
   // LAYOUT CREATE START// 
   Evas_Object *ly1 = elm_layout_add(win);		  
   snprintf(buf, sizeof(buf), "%s/themes/countdown.edj", PACKAGE_DATA_DIR);
	evas_object_size_hint_weight_set(ly1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, ly1);
    // LAYOUT CREATE END// 

//    evas_object_data_set(win, "config", config);
	evas_object_resize(win, 50, 50);

//     elm_win_resize_object_add(win, ly);
	elm_layout_file_set(ly1, buf, "countdown");
	
	/////////


	Evas_Object *fl = elm_flip_add(win);
   evas_object_size_hint_align_set(fl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(fl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   evas_object_show(fl);
	
//    evas_object_data_set(win, "fl", fl);
	
   elm_object_part_content_set(fl, "front", ly);
   evas_object_show(ly);

   elm_object_part_content_set(fl, "back", ly1);
   evas_object_show(ly1);
	
	
	
//    evas_object_show(ly);
	
	
	*/
	
	///////////////////////////////////////////////////////////////////////////////////////////7
	
	
   evas_object_show(win);
	
   edje_object_signal_callback_add(ly, "settings", "settings", _settings_2, win);
   edje_object_signal_callback_add(ly, "hour", "hour_decrease", _hour_decrease, NULL);
   edje_object_signal_callback_add(ly, "hour", "hour_increase", _hour_increase, NULL);
   edje_object_signal_callback_add(ly, "min", "min_decrease", _min_decrease, NULL);
   edje_object_signal_callback_add(ly, "min", "min_increase", _min_increase, NULL);
   edje_object_signal_callback_add(ly, "sec", "sec_decrease", _sec_decrease, NULL);
   edje_object_signal_callback_add(ly, "sec", "sec_increase", _sec_increase, NULL);
	
   edje_object_signal_callback_add(ly, "start", "countdown", _start_countdown, ly);
	
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
// 	evas_object_smart_callback_add(win, "gadget_site_orient", orient_change, ly);
//    evas_object_smart_callback_add(win, "gadget_site_anchor", anchor_change, ly);
   evas_object_smart_callback_add(win, "gadget_configure", _settings_1, edje_obj);
	ecore_event_handler_add(ECORE_EVENT_SIGNAL_USER, _gadget_exit, NULL);
// 	ecore_event_handler_add(ECORE_EVENT_SIGNAL_USER, _config_save, ly);
	
    evas_object_event_callback_add(win, EVAS_CALLBACK_KEY_DOWN, key_down, ly);
	 
// 	 evas_object_event_callback_add(win, EVAS_CALLBACK_DEL, _config_save, ly);
//     evas_object_event_callback_add(win, EVAS_CALLBACK_KEY_DOWN, key_down1, fl);
	 
	 ////
	 
	Evas_Object *en1 = elm_entry_add(win);
    elm_entry_line_wrap_set(en1, ELM_WRAP_WORD);
   
    elm_object_text_set(en1, "");
    elm_object_part_content_set(ly, "name1", en1);
	///
	_config_load(ly);
	
	set_color(edje_obj);
	
	_set_content(edje_obj, NULL, NULL, NULL);
	_save_eet();
  //run app RUN!
  elm_run();

  //shutdown!
//         _my_conf_descriptor_shutdown();
  return 0;
}

ELM_MAIN();
