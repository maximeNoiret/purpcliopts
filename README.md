# Command Line Interface Argument Parser
This is a library written in C designed to be used in C that allows one to parse arguments more easily. It comes at the cost of memory of course, so if performances are actually genuinely important I recommend not using this and either using a more reputable one or even just making your own very specific one.

## NOTE
This library isn't released yet. Anything in the API (like function names, syntax, etc) might be changed, so don't use it yet.

---

## Installing
I recommend installing from source because if you install some binaries from some guy, you never know what's in them unless you look at the assembly.

### Building from Source
To build from source, simply clone the repository and install the library to your system.
```bash
git clone https://github.com/maximeNoiret/purpcliopts
cd purpcliopts
make && sudo make install
```

To uninstall, simply run ``make uninstall``.

---

## Using
Actually using the library is pretty straightforward.
After installing it, you need to import the header for the library:
```c
#include <purpcliopts.h>
```

Then, you need to create an array of the `purp_cli_option` struct. Here is an example with 2 options:
```c
struct purp_cli_option options[] = {
	{
		.long_opt = "cool",
		.desc = "This is a cool description",
		.flag = 'c',
		.has_arg = 0,
		.callback.no_arg = cool_function
	},
	{
		.long_opt = "another-option",
		.desc = NULL,
		.flag = 'a',
		.has_arg = 1,
		.callback.with_arg = another_callback
	},
	{0}
};
```

`no_arg` functions should have this signature:
```c
void function_name(void *data);
```
The `data` parameter is used to manipulate variables through a struct.

`with_arg` functions should have this signature:
```c
void function_name(const char *arg, void *data);
```

When passing a struct to manipulate variables through those callbacks, you should cast it to your struct's type. For example, if we wanted to copy the pointer `arg` out of the callback function (to print it, for example):
```c
((struct my_data *)data)->content = arg;
```

That pointer is now accessible wherever `data` is accessible. See `tests/test.c` for an example on how to do this.

An example on how to pass that struct pointer to `check_flags`:
```c
struct my_data usr_data = {0};
check_flags(argc, argv, options, &usr_data);
```

Of course if there is only one variable that will be affected by options, passing an integer pointer or something like that works too. You're the programmer, you're in control lol.

Make sure to create a function for each option. If the option doesn't have an argument (`has_arg` is set to 0), use `.callback.no_arg`. Otherwise, use `.callback.with_arg`.
> [!WARNING]
> MAKE SURE TO INCLUDE A ZERO STRUCT AT THE END FOR SENTINEL. (This might be fixed in the future, but too lazy for now :x)

Better documentation on usage will be written in the future. But if you're programming in C you should understand this much :x

---

# Contributing
I accept pull requests because I'm bad at coding so you fixing up things or making it better overall would be greatly appreciated (and I might learn a thing or two from it).

However, you must strictly follow the Linux Kernel Programming Style. There is a `check` command in the Makefile, but you need to get checkpatch.pl from the Linux repository to use it. (I don't know if I should include it myself in my repository, but getting a couple files shouldn't be hard if you code in C)
Also I don't know why GitHub has 4-spaces tabs, but use 8-spaces tabs.

I recommend writing tests in `tests/test.c` whenever you change the code so you can test it out. I didn't put in any unit testing because I'm lazy (if you wanna do that too all the power to you)