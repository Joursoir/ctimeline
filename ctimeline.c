#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "xstring.h"

#define COMMENT_CHAR '#'

struct branch {
	string *name;
	string *position;
	string *age;
	string *desc;
};

struct branch_ll {
	struct branch *branch;
	struct branch_ll *prev;
};

struct branch_ll *first_branch;
struct branch *current_branch;

void emit_timeline_html()
{
	printf("\t<ul class=\"timeline\">\n");

	struct branch_ll *ptr;
	for(ptr = first_branch; ptr; ptr = ptr->prev) {
		printf(
		"\t<li>\n"
			"\t\t<div class=\"direction-%s\">\n"
				"\t\t\t<div class=\"flag-wrapper\">\n"
					"\t\t\t\t<span class=\"flag\">%s</span>\n"
					"\t\t\t\t<span class=\"time-wrapper\"><span class=\"time\">%s</span></span>\n"
				"\t\t\t</div>\n"
				"\t\t\t<div class=\"desc\">%s</div>\n"
			"\t\t</div>\n"
		"\t</li>\n", ptr->branch->position->s, ptr->branch->name->s,
			ptr->branch->age->s, ptr->branch->desc->s);
	}

	printf("\t</ul>\n");
}

struct branch *add_branch(const char *name)
{
	struct branch *br;

	if(!first_branch) {
		first_branch = malloc(sizeof(struct branch_ll));
		first_branch->branch = malloc(sizeof(struct branch));
		first_branch->prev = NULL;

		br = first_branch->branch;
	}
	else {
		struct branch_ll *ptr;
		for(ptr = first_branch; ptr; ptr = ptr->prev) {
			if(!ptr->prev) {
				ptr->prev = malloc(sizeof(struct branch_ll));
				ptr->prev->branch = malloc(sizeof(struct branch));
				ptr->prev->prev = NULL;

				br = ptr->prev->branch;
				break;
			}
		}
	}

	br->name = string_alloc(name);
	br->position = string_alloc("right");
	br->age = string_alloc("No age");
	br->desc = string_alloc("No description");

	return br;
}

void handle_config_context(const char *name, const char *value)
{
	if(strcmp(name, "branch.name") == 0) {
		current_branch = add_branch(value);
	}
	else if(strcmp(name, "branch.position") == 0) {
		if(!current_branch)
			return;

		if(strcmp(value, "right") != 0 && strcmp(value, "left") != 0)
			return;

		string_release(current_branch->position);
		current_branch->position = string_alloc(value);
	}
	else if(strcmp(name, "branch.age") == 0) {
		if(!current_branch)
			return;

		string_release(current_branch->age);
		current_branch->age = string_alloc(value);
	}
	else if(strcmp(name, "branch.desc") == 0) {
		if(!current_branch)
			return;

		string_release(current_branch->desc);
		current_branch->desc = string_alloc(value);
	}
}

void skip_line(FILE *f)
{
	int c;
	while( (c = fgetc(f)) && c != '\n' && c != EOF )
		;
}

int read_config_line(FILE *f, string *name, string *value)
{
	int c;

	// skip comments and white spaces
	for(;;) {
		c = fgetc(f);
		if(c == EOF)
			return 1;
		else if(c == COMMENT_CHAR)
			skip_line(f);
		else if(!isspace(c))
			break;
	}

	// read variable name
	while(c != '=') {
		if(isspace(c) || c == EOF)
			return 1;
		string_addch(name, c);
		c = fgetc(f);
	}

	// read variable value
	c = fgetc(f);
	while(c != '\n' && c != EOF) {
		string_addch(value, c);
		c = fgetc(f);
	}

	return 0;
}

int parse_config_file(const char *filename)
{
	string *name = string_alloc(NULL);
	string *value = string_alloc(NULL);
	FILE *f;
	
	f = fopen(filename, "r");
	if(!f)
		return 1;
		
	while(read_config_line(f, name, value) == 0) {
		// printf("name: %s; value: %s\n", name->s, value->s);
		handle_config_context(name->s, value->s);
		string_reset(name);
		string_reset(value);
	}
		
	fclose(f);
	string_release(name);
	string_release(value);
	return 0;
}

void usage()
{
	printf("Usage: ctimeline [-h, --help] config_path\n"
		"Copyright (C) 2021 Aleksandr D. Goncharov (Joursoir)\n"
		"License: GNU GPL version 3 + X11 License\n"
		"This is free software: you are free to change and redistribute it.\n"
		"This program comes with ABSOLUTELY NO WARRANTY.\n");
}

int main(int argc, char **argv)
{
	int result;
	const char *config;
	if(argc < 2) {
		usage();
		return 1;
	}
	if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
		usage();
		return 0;
	}
	config = argv[1];

	result = parse_config_file(config);
	if(result) {
		perror(config);
		return 1;
	}

	emit_timeline_html();
	return 0;	
}
