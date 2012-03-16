#ifndef __EASYOPTION_H__
#define __EASYOPTION_H__

#include <stdio.h>
#include "strkeymap.h"

typedef struct easyoption {
	strkeymap *map;	
	char err[1024];
	int max_valdesc;
}easyoption;

typedef struct option_context {
	const char *key;
	const char *valdesc;
	const char *desc;
	char *value;
	int on;
}option_context;



inline easyoption * easyoption_new() {
	easyoption *opt = (easyoption *)malloc(sizeof(easyoption));
	memset(opt, 0, sizeof(easyoption));
	opt->map = strkeymap_new();
	return opt;
}


inline int easyoption_add(easyoption *opt, const char *key, const char *valdesc, const char *desc) {

	strkeymap_iterator it = strkeymap_find(opt->map, key);
	if(it.result) {
		return -1;
	}

	option_context *ctx = (option_context *)malloc(sizeof(option_context));
	ctx->key = key;
	ctx->value = NULL;
	ctx->valdesc = valdesc ? valdesc : "";
	ctx->desc = desc;
	ctx->on = 0;

	if(opt->max_valdesc < strlen(ctx->valdesc)) {
		opt->max_valdesc = strlen(ctx->valdesc);
	}

	strkeymap_insert(opt->map, key, ctx);
	return 0;
}

inline int easyoption_parse(easyoption *opt, int argc, char **argv) {
	int i;
	for(i = 1; i < argc; i++) {
		const char *key = argv[i];
		strkeymap_iterator it = strkeymap_find(opt->map, key);
		if(!it.result) {
			// unknown option..
			sprintf(opt->err, "'%s' option not available.", key);
			return -1;
		} 
		option_context *ctx = *it.second;
		if(!ctx->valdesc || strlen(ctx->valdesc) <= 0) {
			ctx->on = 1;
			continue;
		}

		if(i+1 >= argc) {
			sprintf(opt->err, "wrong '%s' option parameter..", key);
			break;
		}

		char *val = argv[++i];
		strkeymap_iterator it2 = strkeymap_find(opt->map, val);
		if(it2.result) {
			// invalid argument..
			sprintf(opt->err, "wrong '%s' option parameter.", key);
			return -1;
		} 
		ctx->value = val;
		ctx->on = 1;
	}

	return 0;
}


inline int easyoption_value(easyoption *opt, const char *key, char **value) {
	strkeymap_iterator it = strkeymap_find(opt->map, key);
	if(it.result) {
		option_context *ctx = *it.second;
		if(!ctx->on)
			return 0;

		if(value) {
			*value = ((option_context *)*it.second)->value;
		}
		return 1;
	}
	return 0;
}


inline const char * easyoption_get_lasterror(easyoption *opt) {
	return opt->err;
}


inline void easyoption_free(easyoption *opt) {
	strkeymap_free(opt->map);
	free(opt);
}


inline void easyoption_loop(easyoption *opt, void(*loop)(option_context *)) {

	const strkeymap_iterator* it = strkeymap_iterator_new(opt->map);
	if(it) {
		do {
			option_context *ctx = *it->second;
			loop(ctx);
			it = strkeymap_iterator_next(it);
		} while(it);
	}
	strkeymap_iterator_free(opt->map);
}


inline void easyoption_print_option(easyoption *opt, FILE *out) {
	const strkeymap_iterator* it = strkeymap_iterator_new(opt->map);
	if(it) {
		do {
			option_context *ctx = *it->second;
			fprintf(out, "%s => %s (%d)\n", it->first, ctx->value, ctx->on);
			it = strkeymap_iterator_next(it);
		} while(it);
	}
	strkeymap_iterator_free(opt->map);
}

#endif
