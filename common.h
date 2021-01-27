#ifndef H_COMMON
#define H_COMMON

#define GETOBJ(a) gtk_builder_get_object(builder, a)
#define GET_SLIDER_VAL(a) gtk_adjustment_get_value(GTK_ADJUSTMENT(GETOBJ(a)))

#endif