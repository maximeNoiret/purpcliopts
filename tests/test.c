#include <stdio.h>
#include "purpcliopts.h"

void print_callback(const char *str)
{
	puts(str);
}

int main(int argc, char **argv)
{
	struct purp_cli_option options[] = {
		{
			.long_opt = "print",
			.desc = "Prints something to screen",
			.flag = 'p',
			.has_arg = 1,
			.callback = print_callback
		},
		{0}
	};

	int err = check_flags(argc, argv, options);

	if (err == PURP_OPT_HELP) {
		purp_printhelp(options);
		return 0;
	}

	if (err != PURP_OPT_OK) {
		puts(purp_errmsg(err));
	}
	return 0;
}
