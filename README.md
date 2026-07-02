# Command Line Interface Argument Parser
This is a library written in C designed to be used in C that allows one to parse arguments more easily. It comes at the cost of memory of course, so if performances are actually genuinely important I recommend not using this and either using a more reputable one or even just making your own very specific one.

---

## Installing
I recommend installing from source because if you install some binaries from some guy, you never know what's in them unless you look at the assembly.

### Building from Source
To build from source, simply clone the repository and install the library to your system.
```
git clone https://github.com/maximeNoiret/purpcliopts
cd purpcliopts
make && sudo make install
```

To uninstall, simply run ``make uninstall``.

---

## Using
Actually using the library is pretty straightforward.
After installing it, you need to import the header for the library:
```
#include <purpcliopts.h>
```

Then, you need to create an array of the `purp_cli_option` struct. Here is an example with 2 options:
```
struct purp_cli_option options[] = {
	{
		.long_opt = "help",
		.flag = 'h',
		.has_arg = 0,
		.callback = NO_ARG_CB(help_callback)
	},
	{
		.long_opt = "another-option",
		.flag = 'a',
		.has_arg = 1,
		.callback = another_callback
	},
	{0}
};
```
Make sure to create a function for each option. If the option doesn't have an argument (`has_arg` is set to 0), use the `NO_ARG_CB` macro on the function.
> [!WARNING]
> MAKE SURE TO INCLUDE A ZERO STRUCT AT THE END. (This might be fixed in the future, but too lazy for now :x)

Better documentation on usage will be written in the future. But if you're programming in C you should understand this much :x

---

# Contributing
I accept pull requests because I'm bad at coding so you fixing up things or making it better overall would be greatly appreciated (and I might learn a thing or two from it).

However, you must strictly follow the Linux Kernel Programming Style. There is a `check` command in the Makefile, but you need to get checkpatch.pl from the Linux repository to use it. (I don't know if I should include it myself in my repository, but getting a couple files shouldn't be hard if you code in C)

I recommend writing tests in `tests/test.c` whenever you change the code so you can test it out. I didn't put in any unit testing because I'm lazy (if you wanna do that too all the power to you)