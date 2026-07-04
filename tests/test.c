#include <stdio.h>
#include "purpcliopts.h"

#define L_FLAG_MASK 0x0001

struct data {
	const char *content;
	unsigned int flags;
};

void print_callback(const char *str, void *data)
{
	((struct data *)data)->content = str;
}

void l_callback(const char *str, void *data)
{
	(void)str;
	((struct data *)data)->flags |= L_FLAG_MASK;
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
		{
			.long_opt = "very-long-param",
			.desc = "Idk, do something neat.",
			.flag = 'l',
			.has_arg = 1,
			.callback.with_arg = l_callback
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
	printf("FLAGS: %d\n", usr_data.flags);
	return 0;
}
