#include <pjlib.h>
#include <iostream>
char * pj_print(pj_pool_t *pool,const char * fpm, ...)
{
	va_list list;
	char * src = (char *)pj_pool_zalloc(pool,4000);
	va_start(list, fpm);
	vsprintf(src, fpm, list);
	va_end(list);
	return src;
}
