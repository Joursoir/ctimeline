#ifndef CTIMELINE_XSTRING_H
#define CTIMELINE_XSTRING_H

#define INIT_LEN_STRING 32

typedef struct tag_string {
	char *s;
	int len;
	int capacity;
} string;

string *string_alloc(const char *text);
string *string_nalloc(const char *text, size_t text_len);
void string_release(string *str);
void string_reset(string *str);
void string_addch(string *str, int ch);
void string_addstr(string *str, const char *src, size_t src_len);

#endif /* CTIMELINE_XSTRING_H */
