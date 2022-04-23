# terminal-tool-builder-lib 
This library can be used to create terminal tools easily and quickly. 

# Installation requirements
The following packages must be installed.<br>
```
sudo apt-get install build-essential cmake git gcc
```

# Download & Build

First download the program from GitHub and go to the terminal-tool-builder-lib folder.

```
% git clone https://github.com/rounnus/terminal-tool-builder-lib.git
% cd terminal-tool-builder-lib/
```

After installation the program must be built. In order to build the program, the following instructions must be
followed.<br>

```
% mkdir build
% cd build/
% cmake ../
% make
% sudo make install // This does not work at the time. But i will make it soon.
```

`
The cmake should be from version 3.21 and above, if you do not have this version please go to the following site and download the latest version:
https://cmake.org/download/
`

After this the program will be installed and ready to run.

# Documentation
This library is designed to make it easy to create tools that run on the terminal. Below it will be analyzed how it can be used.

## Library Stractures

### tool_builder
The builder structure is the most basic element of the library. It contains all the necessary information for the construction of the requested tool.
#### Stracture definition
```C
struct tool_builder 
{
	struct tool_builder_command *t_commands;		// array of commands.
	int t_commandsc;					// The number of the commands.
	struct tool_builder_help t_help;			// The help of the tool.
	int t_mc: 1;						// enable or disable multiple commands in one line.
};
```
**The user should not be interested in the above definition.**

### tool_builder_args
The parameter structure is the structure that is "sent" to the user function that executes a command. It is used so that the user (**tool developer**) receives the necessary information about the command that was executed.
#### Stracture definition
```C
struct tool_builder_args 
{
	char *c_name;						// The name of the command that has been executed.
	char *c_used_alias;					// The alias that has been used.
	char *(*c_values);					// The values that has been retrieved.
	int c_argc;						// The arguments of the command.
	struct tool_builder *c_builder;				// The builder.
};
```
**Above is the information he receives.**

## Library functions

### tool_builder_init
#### Description
The **tool_builder_init** function initializes the tool builder. <br>

#### Function signature
```C
void tool_builder_init(struct tool_builder *c_builder);
```

#### Arguments
`c_builder` Is a pointer to the builder to be used.

#### Example
```C
struct tool_builder builder;
tool_builder_init(&builder);
```
  
### tool_builder_destroy
#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```




### tool_builder_add_command

#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```



### tool_builder_add_alias
#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```



### tool_builder_add_both

#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```




### tool_builder_add_alias_both

#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```



### tool_builder_set_actions

#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```





### tool_builder_prepare
#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```




### tool_builder_execute
#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```




### tool_builder_call_command
#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```




### tool_builder_init_help
#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```




### tool_builder_set_desc
#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```






### tool_builder_add_command_doc
#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```






### tool_builder_add_alias_doc
#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```






### tool_builder_set_closing_desc

#### Description


#### Function signature
```C

```

#### Arguments


#### Example
```C

```



## Example
An example of the library in use can be found in the **tests** folder

## Done!
This is my library, I hope you find it useful. Any contribution or any proposal for change is acceptable!! 
