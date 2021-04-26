#include <stdlib.h>
#include <string.h>

#include "xstring.h"

string *string_alloc(const char *text)
{
	string *str = malloc(sizeof(string));
	if(text) {
		int length = strlen(text);
		str->s = malloc(sizeof(char) * (length + 1));
		strcpy(str->s, text);
		str->capacity = length;
		str->len = length;
	}
	else {
		str->s = malloc(sizeof(char) * (INIT_LEN_STRING + 1));
		str->len = 0;
		str->capacity = INIT_LEN_STRING;
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
