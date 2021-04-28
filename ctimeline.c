#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "xstring.h"

#define COMMENT_CHAR '#'
#define INIT_CAPACITY_LIST 8

struct branch {
	string *name;
	int age_from;
	int age_to;
	string *desc;
};

struct branch_list {
	struct branch *list;
	int count;
	int capacity;
};

struct branch_ll {
	struct branch *branch;
	struct branch_ll *prev;
};

struct branch_list branches = { NULL, 0, 0 };
struct branch *current_branch;

void sort_branches_by_age()
{
	int i, j;
	struct branch *i_ptr, *j_ptr;
	struct branch tmp;

	for(i = 0; i < branches.count; i++) {
		i_ptr = &branches.list[i];
		for(j = i+1; j < branches.count; j++) {
			j_ptr = &branches.list[j];
			if((j_ptr->age_from > i_ptr->age_from) ||
						(j_ptr->age_to > i_ptr->age_to) ) {
				tmp = *i_ptr;
				*i_ptr = *j_ptr;
				*j_ptr = tmp;
			}
		}
	}
}

void emit_timeline_html()
{
	int i;
	struct branch *ptr;

	printf("\t<ul class=\"timeline\">\n");

	for(i = 0; i < branches.count; i++) {
		ptr = &branches.list[i];
		printf(
		"\t<li>\n"
			"\t\t<div class=\"direction-%s\">\n"
				"\t\t\t<div class=\"flag-wrapper\">\n"
					"\t\t\t\t<span class=\"flag\">%s</span>\n"
					"\t\t\t\t<span class=\"time-wrapper\"><span class=\"time\">%d-%d</span></span>\n"
				"\t\t\t</div>\n"
				"\t\t\t<div class=\"desc\">%s</div>\n"
			"\t\t</div>\n"
		"\t</li>\n", (i % 2) ? "left" : "right", ptr->name->s,
			ptr->age_from, ptr->age_to, ptr->desc->s);
		string_release(ptr->name);
		string_release(ptr->desc);
	}

	printf("\t</ul>\n");
}

struct branch *add_branch(const char *name)
{
	struct branch *br;
	branches.count++;

	if(branches.count > branches.capacity) {
		if(branches.capacity == 0)
			branches.capacity = INIT_CAPACITY_LIST;
		else
			branches.capacity *= 2;
		branches.list = realloc(branches.list,
			sizeof(struct branch) * branches.capacity);
	}

	br = &branches.list[branches.count-1];
	br->name = string_alloc(name);
	br->age_from = 0;
	br->age_to = 0;
	br->desc = string_alloc("No description");
	return br;
}

void handle_config_context(const char *name, const char *value)
{
	if(strcmp(name, "branch.name") == 0) {
		current_branch = add_branch(value);
	}
	else if(strcmp(name, "branch.age_from") == 0) {
		if(!current_branch)
			return;

		current_branch->age_from = atoi(value);
	}
	else if(strcmp(name, "branch.age_to") == 0) {
		if(!current_branch)
			return;

		current_branch->age_to = atoi(value);
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

	sort_branches_by_age();

	emit_timeline_html();
	free(branches.list);
	return 0;	
}
