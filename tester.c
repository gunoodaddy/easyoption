
#include "easyoption.h"

void loop_func(option_context *ctx) {
	printf("%5s %10s       %20s\n", ctx->key, ctx->valdesc, ctx->desc);
}

int test_main(int argc, char *argv[]) {

	easyoption *opt = easyoption_new();
	
	easyoption_add(opt, "-n", "n count", "description.. blah blah 1");
	easyoption_add(opt, "-c", "", "description.. blah blah 2");
	easyoption_add(opt, "-p", "p count", "description.. blah blah 3");

	if(easyoption_parse(opt, argc, argv) != 0) {
		// parse error!
		//easyoption_print_help(opt);
		printf("%s\n", easyoption_get_lasterror(opt));
		exit(1);
	}

/*
	char *value;
	int result;
	result = easyoption_value(opt, "-n", &value);
	if(result) {
		printf("-n option => %s\n", value);
	}

	result = easyoption_value(opt, "-p", &value);
	if(result) {
		printf("-p option => %s\n", value);
	}
	*/

	easyoption_loop(opt, loop_func);
	easyoption_print_option(opt, stderr);

	easyoption_free(opt);
	return 0;
}


int main() {
	char *argv1[] = {
		"tester",
		"-n",
		"1",
		"-p",
		"100",
		"-c",
	};
	test_main(5, argv1);

	return 0;
}
