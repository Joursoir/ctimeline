#include <stddef.h>
#include <ctype.h>

#include "xstring.h"
#include "url.h"

static int url_decode_hex_char(const char *query)
{
	int out = 0, ch, i;
	for(i = 0; i < 2; i++) {
		ch = *query;
		out <<= 4;
		if(isdigit(ch))
			out += ch - '0';
		else if(isalpha(ch))
			out += tolower(ch) - 'a' + 10;
		else
			return -1;
		query++;
	}
	return out;
}

static string *url_decode(const char **query, char delimiter)
{
	int ch;
	const char *str = *query;
	string *dest = string_alloc(NULL, NULL);

	while((ch = *str)) {
		str++;

		if(ch == delimiter)
			break;

		if(ch == '%') {
			ch = url_decode_hex_char(str);
			if(ch) {
				string_addch(dest, ch);
				str += 2;
			}
			continue;
		}
		
		if(ch == '+')
			string_addch(dest, ' ');
		else
			string_addch(dest, ch);
	}
	*query = str;
	return dest;
}

string *url_get_param_name(const char **query)
{
	return url_decode(query, '=');
}

string *url_get_param_value(const char **query)
{
	return url_decode(query, '&');
}