#include <stdio.h>
#include "purpcliopts.h"

struct data {
	const char *content;
};

void print_callback(const char *str, void *data)
{
	((struct data *)data)->content = str;
}

int main(int argc, char **argv)
{
	struct purp_cli_option options[] = {
		{
			.long_opt = "print",
			.desc = "Prints arg to screen",
			.flag = 'p',
			.has_arg = 1,
			.callback.with_arg = print_callback
		},
		{0}
	};

	struct data usr_data = {0};

	int err = check_flags(argc, argv, options, &usr_data);

	if (err == PURP_OPT_HELP) {
		purp_printhelp(options);
		return 0;
	}

	if (err != PURP_OPT_OK) {
		puts(purp_errmsg(err));
		return err;
	}

	if (usr_data.content)
		puts(usr_data.content);
	return 0;
}
