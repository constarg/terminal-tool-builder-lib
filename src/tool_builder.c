#include <string.h>
#include <stdarg.h>
#include <tool_builder.h>
#include <queue.h>


// Priavte implementation of structs.
struct tool_builder_c_help
{
    char *c_name;                           // The name of the command.
    char *(*c_alias);                       // The alias of the command.
    char *c_description;                    // The description of the command.
};

struct tool_builder_command
{
    char *c_name;                           // The name of the command.
    int c_argc;                             // How many argcs the command require.
    char *(*c_alias);                       // The alias of the commnad. 5 is the maximum number of alias.
    int c_alias_c;

    /**
        This callback is user defined and is executed
        when the user request a call of the specific
        command defined in @name field from terminal.
        @param value The parameters that have been
        retrieved from the terminal when the command
        was called.
    */
    void (*c_callback)(const struct tool_builder_args *info); // The action to take when the command has been requested.
};


static inline struct tool_builder_command *find_command(const struct tool_builder_command *commands, const char *c_name,
                                                        int commandsc);

static struct tb_queue c_queue;


static void help_defualt_action(const struct tool_builder_args *info)
{
    // Prints the help message.
    struct tool_builder *builder = info->c_builder;
    if (builder == NULL) return;
    printf("%s\n", builder->t_help.t_usage_sec);

    if (builder->t_help.t_description == NULL) goto skip_dec;
    printf("%s\n\n\n", builder->t_help.t_description);
    struct tool_builder_c_help *commands_h;
    skip_dec:
    commands_h = builder->t_help.t_commands;
    for (int h = 0; h < builder->t_help.t_commandsc; h++) {
        printf("\t%s", commands_h[h].c_name);
        if (commands_h[h].c_alias == NULL) goto skip_alias;
        for (int c_a = 0; commands_h[h].c_alias[c_a]; c_a++)
            printf(" ,%s,", commands_h[h].c_alias[c_a]);
        skip_alias:
        printf("%s\n", commands_h[h].c_description);
    }

    if (builder->t_help.t_close_description == NULL) return;
    printf("%s\n", builder->t_help.t_close_description);
}

void tool_builder_init(struct tool_builder *c_builder)
{
    memset(c_builder, 0x0, sizeof(struct tool_builder));
    c_builder->t_commands  = (struct tool_builder_command *) calloc(1, sizeof(struct tool_builder_command));
    c_builder->t_commandsc = 0x0;
    c_builder->t_mc        = 0x1;
}

void tool_builder_destroy(struct tool_builder *c_builder)
{
    for (int h = 0; h < c_builder->t_help.t_commandsc; h++) {
        free(c_builder->t_help.t_commands[h].c_name);
        free(c_builder->t_help.t_commands[h].c_description);
    }
    // Free help.
    free(c_builder->t_help.t_commands);
    free(c_builder->t_help.t_usage_sec);
    free(c_builder->t_help.t_close_description);
    free(c_builder->t_help.t_description);

    // Free the memory for the commands.
    for (int c = 0; c < c_builder->t_commandsc; c++) {
        free(c_builder->t_commands[c].c_name);
        for (int a = 0; a < c_builder->t_commands[c].c_alias_c; a++)
            free(c_builder->t_commands[c].c_alias[a]);
        free(c_builder->t_commands[c].c_alias);
    }
    free(c_builder->t_commands);
}


int tool_builder_init_help(struct tool_builder *c_builder, const char *tool_name)
{
    const char *str_u_1 = "Usage:";
    const char *str_u_2 = "[OPTION]...";

    size_t usage_size = strlen(tool_name) + strlen(str_u_1)
                        + strlen(str_u_2) + 3;

    char *(*usage_sec) = &c_builder->t_help.t_usage_sec;
    *usage_sec = malloc(sizeof(char) * usage_size);
    if ((*usage_sec) == NULL) return TOOL_BUILDER_FAILED_TO_ADD;

    sprintf((*usage_sec), "%s %s %s", str_u_1, tool_name, str_u_2);

    // Initialize help as a command.
    // Add command.
    int error = tool_builder_add_command(
            c_builder,
            "--help",
            0,
            &help_defualt_action
    );
    if (error != 0) return error;

    return tool_builder_add_alias(c_builder, "--help", "-h", NULL);
}

int tool_builder_set_desc(struct tool_builder *c_builder, const char *c_description)
{
    struct tool_builder_help *help_tmp = &c_builder->t_help;
    help_tmp->t_description = (char *) malloc(sizeof(char) * strlen(c_description) + 1);
    strcpy(help_tmp->t_description, c_description);

    return 0;
}


int tool_builder_add_command_doc(struct tool_builder *c_builder, const char *c_name,
                                 const char *c_description)
{
    struct tool_builder_c_help new_command_h;
    memset(&new_command_h, 0x0, sizeof(struct tool_builder_c_help));
    new_command_h.c_name = (char *) malloc(sizeof(char) * (strlen(c_name) + 1));
    new_command_h.c_description = (char *) malloc(sizeof(char) * (strlen(c_description) + 1));
    if (new_command_h.c_name == NULL || new_command_h.c_description == NULL)
        return TOOL_BUILDER_FAILED_TO_ADD;

    // Copy the contents.
    strcpy(new_command_h.c_name, c_name);
    strcpy(new_command_h.c_description, c_description);

    int last_c = c_builder->t_help.t_commandsc;
    c_builder->t_help.t_commands = (struct tool_builder_c_help *) realloc(c_builder->t_help.t_commands,
                                                                          sizeof(struct tool_builder_c_help) *
                                                                          (last_c + 1));
    if (c_builder->t_help.t_commands == NULL)
        return TOOL_BUILDER_FAILED_TO_ADD;

    // Save the new command.
    c_builder->t_help.t_commands[last_c] = new_command_h;
    // Increase the number of commands.
    c_builder->t_help.t_commandsc += 1;
    return 0;
}

int tool_builder_add_alias_doc(struct tool_builder *c_builder, const char *c_name)
{
    struct tool_builder_command *c_found = find_command((const struct tool_builder_command *) c_builder->t_commands,
                                                        c_name, c_builder->t_commandsc);
    if (c_found == NULL) return TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS;

    for (int h = 0; h < c_builder->t_help.t_commandsc; h++) {
        if (!strcmp(c_builder->t_help.t_commands[h].c_name, c_found->c_name))
            c_builder->t_help.t_commands[h].c_alias = c_found->c_alias;
    }
}

int tool_builder_set_closing_desc(struct tool_builder *c_builder, const char *close_description)
{
    struct tool_builder_help *help_tmp = &c_builder->t_help;

    help_tmp->t_close_description = (char *) malloc(sizeof(char) * (strlen(close_description) + 1));
    if (help_tmp->t_close_description == NULL) return TOOL_BUILDER_FAILED_TO_ADD;
    strcpy(help_tmp->t_close_description, close_description);

    return 0;
}

int tool_builder_add_command(struct tool_builder *c_builder, const char *c_name, int c_argc,
                             void (*c_callback)(const struct tool_builder_args *info))
{
    // Allocate memory for the new command.
    int last_c = c_builder->t_commandsc;
    struct tool_builder_command new_command;
    memset(&new_command, 0x0, sizeof(struct tool_builder_command));
    new_command.c_name = (char *) malloc(sizeof(char) * (strlen(c_name) + 1));
    if (new_command.c_name == NULL) return TOOL_BUILDER_FAILED_TO_ADD;

    strcpy(new_command.c_name, c_name);
    new_command.c_argc = c_argc;

    new_command.c_callback = c_callback;
    c_builder->t_commands  = (struct tool_builder_command *) realloc(c_builder->t_commands,
                                                                    sizeof(struct tool_builder_command) * (last_c + 1));
    if (c_builder->t_commands == NULL) return TOOL_BUILDER_FAILED_TO_ADD;

    c_builder->t_commands[last_c] = new_command;
    c_builder->t_commandsc += 1;
    return 0;
}

int tool_builder_add_alias(struct tool_builder *c_builder, const char *c_name,
                           const char *c_alias, ...)
{
    struct tool_builder_command *command = find_command((const struct tool_builder_command *) c_builder->t_commands,
                                                        c_name, c_builder->t_commandsc);

    if (command == NULL) return TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS;
    if (c_alias == NULL) return TOOL_BUILDER_FAILED_TO_ADD;

    command->c_alias = (char **) calloc(1, sizeof(char *));
    command->c_alias_c = 0; // initialize count.	
    if (command->c_alias == NULL) TOOL_BUILDER_FAILED_TO_ADD;

    // the list of alias.
    va_list alias_list;
    va_start(alias_list, c_alias);

    char *tmp_a = (char *) c_alias;
    while (tmp_a) {
        // allocate space for the curr alias.
        command->c_alias[command->c_alias_c] = (char *) malloc(sizeof(char) * (strlen(tmp_a) + 1));
        // copy the alias.
        strcpy(command->c_alias[command->c_alias_c], tmp_a);
        // increase the aliases.
        command->c_alias = (char **) realloc(command->c_alias, (++command->c_alias_c + 1) * sizeof(char *));
        if (command->c_alias == NULL) return TOOL_BUILDER_FAILED_TO_ADD;
        tmp_a = va_arg(alias_list, char*);
    }

    // null terminate the alias.
    command->c_alias[command->c_alias_c] = NULL;
    va_end(alias_list);

    return 0;
}


int tool_builder_set_action(struct tool_builder *c_builder, const char *c_name,
                            void (*c_callback)(const struct tool_builder_args *info))
{
    struct tool_builder_command *ch_command = find_command((const struct tool_builder_command *) c_builder->t_commands,
                                                           c_name,
                                                           c_builder->t_commandsc);
    if (ch_command == NULL) return TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS;
    ch_command->c_callback = c_callback;    // Set the requested callback.
    return 0;
}


static inline struct tool_builder_command *find_command(const struct tool_builder_command *commands, const char *c_name,
                                                        int commandsc)
{

    for (int curr_c = 0; curr_c < commandsc; curr_c++) {
        // Check if any of the names in the commands is the c_name.
        if (!strcmp(commands[curr_c].c_name, c_name)) return (struct tool_builder_command *) &commands[curr_c];
        else {
            // Check if any of the alias of the current command is the c_name.
            if (commands[curr_c].c_alias == NULL) continue;
            for (int curr_a = 0; commands[curr_c].c_alias[curr_a]; curr_a++) {
                if (!strcmp(commands[curr_c].c_alias[curr_a], c_name))
                    return (struct tool_builder_command *)
                            &commands[curr_c];
            }
        }
    }
    return NULL;
}

int tool_builder_call_command(const char *c_name, const struct tool_builder *c_builder)
{
    struct tool_builder_command *command = find_command((const struct tool_builder_command *) c_builder->t_commands,
                                                        c_name, c_builder->t_commandsc);
    if (command == NULL) return TOOL_BUILDER_WRONG_NAME_OR_ALIAS;

    struct tool_builder_args exec_inf;
    memset(&exec_inf, 0x0, sizeof(struct tool_builder_args));
    exec_inf.c_name         = command->c_name;
    exec_inf.c_used_alias   = command->c_name;
    exec_inf.c_argc         = command->c_argc;
    exec_inf.c_values       = NULL;    // This points to the first argument of the requested command.
    exec_inf.c_builder      = (struct tool_builder *) c_builder;
    // call the command.
    command->c_callback(&exec_inf);
    return 0;
}

int tool_builder_prepare(int argc, char *argv[], const struct tool_builder *c_builder)
{
    if (argv[0] == NULL) return TOOL_BUILDER_EMPTY_NAME;
    if (argv[1] == NULL) return TOOL_BUILDER_EMPTY_NAME;

    tb_queue_init(&c_queue);

    struct tool_builder_command *c_curr;
    struct tb_queue_node_d c_data;

    memset(&c_data, 0x0, sizeof(struct tb_queue_node_d));

    char *(*command) = (++argv); // point to the first command.

    while (*command) {
        c_curr = find_command((const struct tool_builder_command *) c_builder->t_commands, *command,
                              c_builder->t_commandsc);
        if (c_curr == NULL) return TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS;
        if (c_curr->c_callback == NULL) return TOOL_BUILDER_NO_ACTION_DEFINED;

        c_data.c_args.c_name        = c_curr->c_name;
        c_data.c_args.c_used_alias  = *command;
        c_data.c_args.c_argc        = c_curr->c_argc;
        c_data.c_args.c_values      = (char **) malloc(sizeof(char *) * (c_curr->c_argc + 1));
        c_data.c_args.c_builder     = (struct tool_builder *) c_builder;
        c_data.c_callback           = c_curr->c_callback;

        if (c_data.c_args.c_values == NULL) return TOOL_BUILDER_FAILED_TO_ADD;
        // store the values.
        for (int v = 0; v < c_curr->c_argc; v++) {
            c_data.c_args.c_values[v] = (*(command + v + 1) == NULL) ? NULL : *(command + v + 1);
            if (
                    c_data.c_args.c_values[v] == NULL
                    ||
                    find_command((const struct tool_builder_command *) c_builder->t_commands, *(command + v + 1),
                                 c_builder->t_commandsc) != NULL
                    ) {
                free(c_data.c_args.c_values);
                tb_queue_destroy(&c_queue);
                return TOOL_BUILDER_WRONG_ARG_NUM;
            }
        }
        c_data.c_args.c_values[c_curr->c_argc] = NULL;
        tb_queue_enqueue(&c_data, &c_queue);
        command += c_data.c_args.c_argc + 1;

        if (!c_builder->t_mc) break;
    }

    return 0;
}

void tool_builder_execute()
{
    // execute each command in the queue.
    struct tb_queue_node_d c_to_exec;
    memset(&c_to_exec, 0x0, sizeof(struct tb_queue_node_d));

    while (!tb_queue_is_empty(&c_queue)) {
        tb_queue_dequeue(&c_to_exec, &c_queue);
        c_to_exec.c_callback(&c_to_exec.c_args);
        free(c_to_exec.c_args.c_values);
    }
}
