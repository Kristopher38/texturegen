#ifndef H_COMMON
#define H_COMMON

#define GETOBJ(a) gtk_builder_get_object(builder, a)
#define GET_SLIDER_VAL(a) gtk_adjustment_get_value(GTK_ADJUSTMENT(GETOBJ(a)))
#define ARRAYLEN(a) ((sizeof(a) > 0) ? (sizeof(a)/sizeof(a[0])) : 0)

#endif
