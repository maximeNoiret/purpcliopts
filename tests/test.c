#include <stdio.h>
#include "purpcliopts.h"

void help_callback(void)
{
	puts("Help Menu blablabla\n\tblablabla");
}

void print_callback(const char *str)
{
	puts(str);
}

int main(int argc, char **argv)
{
	struct purp_cli_option options[] = {
		{
			.long_opt = "help",
			.flag = 'h',
			.has_arg = 0,
			.callback = NO_ARG_CB(help_callback)
		},
		{
			.long_opt = "print",
			.flag = 'p',
			.has_arg = 1,
			.callback = print_callback
		},
		{0}
	};

	int err = check_args(argc, argv, options);

	if (err != PURP_OPT_OK)
		puts(purp_errmsg(err));

	return 0;
}
