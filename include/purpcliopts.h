#ifndef PURPCLIOPTS_H_
#define PURPCLIOPTS_H_

union purp_callback {
	void (*no_arg)(void *usr_data);
	void (*with_arg)(const char *arg, void *usr_data);
};

struct purp_cli_option {
	const char *long_opt;
	const char *desc;
	union purp_callback callback;
	char flag;
	unsigned int has_arg : 1;
};

enum purp_cli_err {
	PURP_OPT_OK = 0,
	PURP_OPT_HELP,
	PURP_OPT_ERR_MISSING_ARG,
	PURP_OPT_ERR_INVALID_CLUSTER,
	PURP_OPT_ERR_UNKNOWN_FLAG
};

int check_flags(int argc,
		char **argv,
		struct purp_cli_option *opts,
		void *usr_data);

const char *purp_errmsg(int code);

int purp_printhelp(struct purp_cli_option *opts);

#endif // PURPCLIOPTS_H_
