# easyoption
`easyoption` is a program argument parser written by c language.

## Usage

just include "easyoption.h" in your source or header files. (and copy `strkeymap.h` to your project directory)

## Example

	#include <assert.h>
	#include "easyoption.h"

	// useful for making usage() function
	void loop_func(option_context *ctx) {
		printf("%5s %10s       %s\n", ctx->key, ctx->valdesc, ctx->desc);
	}

	int test_main(int argc, char *argv[]) {

		easyoption *opt = easyoption_new();
		
		easyoption_add(opt, "-n", "n count", "1 description.. need next parameter");
		easyoption_add(opt, "-p", "p count", "2 description.. need next parameter");
		easyoption_add(opt, "-c", "", "solo options");

		if(easyoption_parse(opt, argc, argv) != 0) {
			// parse error!
			printf("%s\n", easyoption_get_lasterror(opt));
			exit(1);
		}

		// print active options
		easyoption_print_option(opt, stderr);

		char *value;
		int result;
		result = easyoption_value(opt, "-n", &value);
		assert(result == 1);
		assert(strcmp(value, "1000") == 0);

		result = easyoption_value(opt, "-p", &value);
		assert(result == 1);
		assert(strcmp(value, "100") == 0);

		result = easyoption_value(opt, "-c", &value);
		assert(result == 1);
		assert(value == NULL);

		easyoption_loop(opt, loop_func);

		easyoption_free(opt);

		return 0;
	}


	int main() {
		char *argv1[] = {
			"tester",
			"-n",
			"1000",
			"-p",
			"100",
			"-c",
		};
		test_main(5, argv1);

		printf("test ok\n");

		return 0;
	}

