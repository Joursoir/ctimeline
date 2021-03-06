#include <stdlib.h>
#include <string.h>

#include "xstring.h"

string *string_alloc(string *str, const char *text)
{
	return string_nalloc(str, text, 
		(text) ? strlen(text) : INIT_LEN_STRING);
}

string *string_nalloc(string *str, const char *text, size_t text_len)
{
	if(!str) {
		str = malloc(sizeof(string));
		str->s = malloc(sizeof(char) * (text_len + 1));
		str->capacity = text_len;
	}
	else {
		if(text_len >= str->capacity) {
			free(str->s);
			str->s = malloc(sizeof(char) * (text_len + 1));
			str->capacity = text_len;
		}

	}

	if(text) {
		strncpy(str->s, text, text_len);
		str->len = text_len;
	}
	else
		str->len = 0;

	str->s[str->len] = '\0';
	return str;
}

void string_release(string *str)
{
	free(str->s);
	free(str);
}

void string_reset(string *str)
{
	str->s[0] = '\0';
	str->len = 0;
}

static void string_realloc(string *str, int cap)
{
	int i;
	char *tmp_s = str->s;
	str->s = malloc(sizeof(char) * (cap + 1));
	for(i = 0; i < cap; i++)
		str->s[i] = (i >= str->len) ? '\0' : tmp_s[i];

	str->capacity = cap;
	free(tmp_s);
}

void string_addch(string *str, int ch)
{
	if((str->len + 1) >= str->capacity)
		string_realloc(str, str->capacity + INIT_LEN_STRING);

	str->s[str->len] = ch;
	str->len++;
	str->s[str->len] = '\0';
}

void string_addstr(string *str, const char *src, size_t src_len)
{
	if((str->len + src_len) >= str->capacity)
		string_realloc(str, str->capacity + src_len);

	while(src_len > 0 && (str->s[str->len] = *src++)) {
		src_len--;
		str->len++;
	}
	str->s[str->len] = '\0';
}
