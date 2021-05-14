#ifndef CTIMELINE_H
#define CTIMELINE_H

#include "xstring.h"

#define CTIMELINE_VERSION "1.0"
#define CTIMELINE_CONFIG "/etc/ctimelines"

struct ctimeline_branch {
	string *name;
	int age_from;
	int age_to;
	string *desc;
};

struct ctimeline_branch_list {
	struct ctimeline_branch *list;
	int count;
	int capacity;
};

struct ctimeline_context {
	char *head_title;
	char *css;
	// char *favicon;
	char *header_title;
	char *header_desc;
	
	struct ctimeline_branch_list branches;
	struct ctimeline_branch *cur_branch;
};

extern struct ctimeline_context ctx;

#endif /* CTIMELINE_H */
