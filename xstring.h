#ifndef CTIMELINE_XSTRING_H
#define CTIMELINE_XSTRING_H

#define INIT_LEN_STRING 32

typedef struct tag_string {
	char *s;
	int len;
	int capacity;
} string;

string *string_alloc(const char *text);
void string_release(string *str);
void string_reset(string *str);
void string_addch(string *str, int ch);

#endif /* CTIMELINE_XSTRING_H */
