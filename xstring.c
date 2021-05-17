#include <stdlib.h>
#include <string.h>

#include "xstring.h"

string *string_alloc(const char *text)
{
	return string_nalloc(text, 
		(text) ? strlen(text) : INIT_LEN_STRING);
}

string *string_nalloc(const char *text, size_t text_len)
{
	string *str = malloc(sizeof(string));
	str->s = malloc(sizeof(char) * (text_len + 1));
	if(text) {
		strncpy(str->s, text, text_len);
		str->capacity = text_len;
		str->len = text_len;
		str->s[text_len] = '\0';
	}
	else {
		str->len = 0;
		str->capacity = text_len;
		str->s[0] = '\0';
	}
	return str;
}

void string_release(string *str)
{
	if(str->s) {
		free(str->s);
		str->s = NULL;
	}
	free(str);
}

void string_reset(string *str)
{
	if(str->s) {
		str->s[0] = '\0';
		str->len = 0;
	}
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
	if(str->capacity <= (str->len + 1)) {
		int i;
		int updated_cap = str->capacity + INIT_LEN_STRING;
		char *tmp_s = str->s;
		str->s = malloc(sizeof(char) * (updated_cap + 1));
		for(i = 0; i < updated_cap; i++)
			str->s[i] = (i >= str->capacity) ? '\0' : tmp_s[i];

		str->capacity = updated_cap;
		free(tmp_s);
	}

	str->s[str->len] = ch;
	str->len++;
	str->s[str->len] = '\0';
}
