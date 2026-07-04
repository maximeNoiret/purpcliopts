#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "purpcliopts.h"

#ifndef _WIN32
#include <sys/ioctl.h>
#include <unistd.h>

#else
#define get_terminal_width() 80
#endif

int handle_long(char *arg, struct purp_cli_option *opt, void *usr_data)
{
	if (opt->has_arg) {
		if (!arg || arg[0] == '-')
			return PURP_OPT_ERR_MISSING_ARG;
		opt->callback.with_arg(arg, usr_data);
	} else {
		opt->callback.no_arg(usr_data);
	}
	return PURP_OPT_OK;
}

int long_flag(char *flag,
	      char *next,
	      struct purp_cli_option *opts,
	      void *usr_data)
{
	if (strncmp("--help", flag, 7) == 0)
		return PURP_OPT_HELP;

	size_t i = 0;

	for (; opts[i].flag != '\0'; ++i) {
		if (!opts[i].long_opt)
			continue;
		if (strcmp(opts[i].long_opt, (flag + 2)) == 0) {
			int ret = handle_long(next, &opts[i], usr_data);

			if (ret)
				return ret;
			break;
		}
	}
	if (opts[i].flag == '\0')
		return PURP_OPT_ERR_UNKNOWN_FLAG;
	return PURP_OPT_OK;
}

int handle_short(char next,
		 char *arg,
		 struct purp_cli_option *opt,
		 void *usr_data)
{
	if (opt->has_arg) {
		if (next != '\0')
			return PURP_OPT_ERR_INVALID_CLUSTER;
		else if (!arg || arg[0] == '-')
			return PURP_OPT_ERR_MISSING_ARG;
		opt->callback.with_arg(arg, usr_data);
	} else {
		opt->callback.no_arg(usr_data);
	}
	return PURP_OPT_OK;
} // handle_option

int short_flags(char *flags,
		char *arg,
		struct purp_cli_option *opts,
		void *usr_data)
{
	int ret = 0;

	for (size_t i = 1; flags[i] != '\0'; ++i) {
		if (flags[i] == 'h')
			return PURP_OPT_HELP;

		size_t j = 0;

		for (; opts[j].flag != '\0'; ++j) {
			if (flags[i] == opts[j].flag) {
				ret = handle_short(flags[i + 1],
						   arg,
						   &opts[j],
						   usr_data);
				if (ret)
					return ret;
				break;
			}
		}
		if (opts[j].flag == '\0')
			return PURP_OPT_ERR_UNKNOWN_FLAG;
	}
	return PURP_OPT_OK;
} // short_flags

int check_flags(int argc,
		char **argv,
		struct purp_cli_option *opts,
		void *usr_data)
{
	int ret = PURP_OPT_OK;

	for (int i = 0; i < argc; ++i) {
		char *opt = argv[i];

		if (opt[0] != '-' || opt[1] == '\0')
			continue;
		if (opt[1] == '-') {
			ret = long_flag(opt, argv[i + 1], opts, usr_data);
			if (ret)
				return ret;
			continue;
		}
		ret = short_flags(opt, argv[i + 1], opts, usr_data);
		if (ret)
			return ret;
	}

	return PURP_OPT_OK;
} // check_flags

const char *purp_errmsg(int code)
{
	switch (code) {
	case PURP_OPT_ERR_MISSING_ARG:
		return "[ERROR] Missing Argument!";
	case PURP_OPT_ERR_INVALID_CLUSTER:
		return "[ERROR] Invalid Short Flag Cluster!";
	case PURP_OPT_ERR_UNKNOWN_FLAG:
		return "[ERROR] Unknown Flag!";
	}
	return NULL;
} // purp_errmsg

#ifndef _WIN32  // for windows, macro at top of file makes this 80.
int get_terminal_width(void)
{
	struct winsize w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0)
		return w.ws_col;
	return 80;
}
#endif

size_t purp_getmaxlong(struct purp_cli_option *opts)
{
	size_t max = 0;

	for (size_t i = 0; opts[i].flag != '\0'; ++i) {
		if (!opts[i].long_opt)
			continue;
		size_t len = strlen(opts[i].long_opt);

		if (len > max)
			max = len;
	}

	return max;
}

int print_description(const char *desc, int desc_p)
{
	int term_w = get_terminal_width();
	int max_eff_w = desc_p + 45;
	int max_w = (term_w > max_eff_w) ? max_eff_w : term_w;
	int next_word_bytes = 0;
	int current_line_width = desc_p;

	for (const char *desc_ptr = desc; *desc_ptr != '\0'; ++desc_ptr) {
		for (const char *next = desc_ptr;
		     *next != '\0' && !isspace((unsigned char)*next);
		     ++next)
			++next_word_bytes;

		if (current_line_width + next_word_bytes > max_w) {
			putchar('\n');
			printf("%*s", desc_p, " ");
			current_line_width = desc_p;
		}
		printf("%.*s", next_word_bytes, desc_ptr);
		current_line_width += next_word_bytes;
		desc_ptr += next_word_bytes;
		next_word_bytes = 0;
		if (*desc_ptr != '\0') {
			if (*desc_ptr == '\n') {
				putchar('\n');
				printf("%*s", desc_p, " ");
				current_line_width = desc_p;
			} else {
				putchar(*desc_ptr);
			}
		} else {
			break;
		}
	}

	return 0;
}

int purp_printhelp(struct purp_cli_option *opts)
{
	puts("Help Menu");
	size_t max_len = purp_getmaxlong(opts);
	int desc_p = 22 + (int)max_len;

	for (size_t i = 0; opts[i].flag != '\0'; ++i) {
		struct purp_cli_option *opt = &opts[i];

		printf(" -%c", opt->flag);
		if (opt->long_opt)
			printf(" | --%-*s", (int)max_len, opt->long_opt);
		else
			printf("%-*s", (int)(max_len + 5), "");
		if (opt->has_arg)
			fputs(" <arg>", stdout);
		else
			fputs("      ", stdout);

		if (opt->desc) {
			fputs("        ", stdout);
			print_description(opt->desc, desc_p);
		}
		putchar('\n');
	}
	putchar('\n');
	return 0;
} // purp_helpmsg
