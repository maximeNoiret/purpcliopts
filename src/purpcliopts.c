#include <stdlib.h>
#include "purpcliopts.h"

int handle_option(char next, char *arg, struct purp_cli_option *opt)
{
	if (opt->has_arg) {
		if (next != '\0')
			return PURP_OPT_ERR_INVALID_CLUSTER;
		else if (!arg || arg[0] == '-')
			return PURP_OPT_ERR_MISSING_ARG;
		opt->callback(arg);
	} else {
		opt->callback(NULL);
	}
	return PURP_OPT_OK;
} // handle_option

int short_flags(char *flags, char *arg, struct purp_cli_option *opts)
{
	int ret = 0;

	for (size_t i = 1; flags[i] != '\0'; ++i) {
		size_t j = 0;

		for (; opts[j].flag != '\0'; ++j) {
			if (flags[i] == opts[j].flag) {
				ret = handle_option(flags[i + 1], arg, &opts[j]);
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

int check_flags(int argc, char **argv, struct purp_cli_option *opts)
{
	for (int i = 0; i < argc; ++i) {
		char *opt = argv[i];

		if (opt[0] != '-' || opt[1] == '\0')
			continue;
		if (opt[1] == '-')
			// TODO: long flags
			return -1;
		return short_flags(opt, argv[i + 1], opts);
	}

	return PURP_OPT_OK;
} // check_flags

const char *purp_errmsg(int code)
{
	switch(code) {
	case PURP_OPT_ERR_MISSING_ARG:
		return "[ERROR] Missing Argument!";
	case PURP_OPT_ERR_INVALID_CLUSTER:
		return "[ERROR] Invalid Short Flag Cluster!";
	case PURP_OPT_ERR_UNKNOWN_FLAG:
		return "[ERROR] Unknown Flag!";
	}
	return NULL;
} // purp_errmsg

char *purp_helpmsg(struct purp_cli_option *opts)
{
	// TODO: implement function
} // purp_helpmsg