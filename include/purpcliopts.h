#ifndef PURPCLIOPTS_H_
#define PURPCLIOPTS_H_

struct purp_cli_option {
	const char *long_opt;
	void (*callback)(const char *arg);
	char flag;
	unsigned int has_arg : 1;
};

enum purp_cli_err {
	PURP_OPT_OK = 0,
	PURP_OPT_ERR_MISSING_ARG,
	PURP_OPT_ERR_INVALID_CLUSTER,
	PURP_OPT_ERR_UNKNOWN_FLAG
};

#define NO_ARG_CB(func) ((void (*)(const char *))(func))

int check_flags(int argc, char **argv, struct purp_cli_option *opts);

const char *purp_errmsg(int code);

char *purp_helpmsg(struct purp_cli_option *opts);

#endif // PURPCLIOPTS_H_
