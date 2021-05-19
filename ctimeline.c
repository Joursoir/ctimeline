#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ctimeline.h"
#include "ui-common.h"

#define COMMENT_CHAR '#'
#define INIT_CAPACITY_LIST 8

struct ctimeline_context ctx; // from ctimeline.h

static void prepare_context()
{
	memset(&ctx, 0, sizeof(ctx));
	ctx.head_title = string_alloc(NULL, "Timeline browser");
	ctx.css = string_alloc(NULL, "static/ctimeline.css");
	ctx.header_title = string_alloc(NULL, "CTimeline");
	ctx.header_desc = string_alloc(NULL, "Web frontend for timelines written in C");

	ctx.branches.list = NULL;
	ctx.branches.count = 0;
	ctx.branches.capacity = 0;
	ctx.cur_branch = NULL;
}

/*
 * The function lies.
 * It doesn't forget the context, just frees dynamic memory
*/
static void forget_context()
{
	string_release(ctx.head_title);
	string_release(ctx.css);
	string_release(ctx.header_title);
	string_release(ctx.header_desc);
	free(ctx.branches.list);
}

void sort_branches_by_age()
{
	int i, j;
	struct ctimeline_branch *i_ptr, *j_ptr;
	struct ctimeline_branch tmp;

	for(i = 0; i < ctx.branches.count; i++) {
		i_ptr = &ctx.branches.list[i];
		for(j = i+1; j < ctx.branches.count; j++) {
			j_ptr = &ctx.branches.list[j];
			if((j_ptr->age_from > i_ptr->age_from) ||
						(j_ptr->age_to > i_ptr->age_to) ) {
				tmp = *i_ptr;
				*i_ptr = *j_ptr;
				*j_ptr = tmp;
			}
		}
	}
}

struct ctimeline_branch *add_branch(const char *name)
{
	struct ctimeline_branch *br;
	ctx.branches.count++;

	if(ctx.branches.count > ctx.branches.capacity) {
		if(ctx.branches.capacity == 0)
			ctx.branches.capacity = INIT_CAPACITY_LIST;
		else
			ctx.branches.capacity *= 2;
		ctx.branches.list = realloc(ctx.branches.list,
			sizeof(struct ctimeline_branch) * ctx.branches.capacity);
	}

	br = &ctx.branches.list[ctx.branches.count-1];
	br->name = string_alloc(NULL, name);
	br->age_from = 0;
	br->age_to = 0;
	br->desc = string_alloc(NULL, "No description");
	return br;
}

void handle_branch_context(const char *name, const char *value)
{
	if(strcmp(name, "branch.name") == 0) {
		ctx.cur_branch = add_branch(value);
	}
	else if(!ctx.cur_branch) {
		return;
	}
	else if(strcmp(name, "branch.age_from") == 0) {
		ctx.cur_branch->age_from = atoi(value);
	}
	else if(strcmp(name, "branch.age_to") == 0) {
		ctx.cur_branch->age_to = atoi(value);
	}
	else if(strcmp(name, "branch.desc") == 0) {
		ctx.cur_branch->desc = string_alloc(
			ctx.cur_branch->desc, value);
	}
}

void handle_config_context(const char *name, const char *value)
{
	if(strcmp(name, "title") == 0) {
		ctx.head_title = string_alloc(
			ctx.head_title, value);
	}
	else if(strcmp(name, "css") == 0) {
		ctx.css = string_alloc(
			ctx.css, value);
	}
	else if(strcmp(name, "header.title") == 0) {
		ctx.header_title = string_alloc(
			ctx.header_title, value);
	}
	else if(strcmp(name, "header.desc") == 0) {
		ctx.header_desc = string_alloc(
			ctx.header_desc, value);
	}
	else
		handle_branch_context(name, value);
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

int parse_config_file(const char *fname)
{
	FILE *f = fopen(fname, "r");
	if(!f) // #TODO: print warning in logs
		return 1; 

	string *name = string_alloc(NULL, NULL);
	string *value = string_alloc(NULL, NULL);

	while(read_config_line(f, name, value) == 0) {
		handle_config_context(name->s, value->s);
		string_reset(name);
		string_reset(value);
	}
	
	string_release(name);
	string_release(value);
	fclose(f);
	return 0;
}

int main(int argc, char **argv)
{
	prepare_context();
	parse_config_file(CTIMELINE_CONFIG);

	print_http_headers();
	print_document_start();
	print_document_header();
	print_timelines();
	print_document_end();

	forget_context();
	return 0;	
}
