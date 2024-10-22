#include "cfgutil.h"

#include <stddef.h>

#include <macros.h>

static struct cfgutil_entry cpuinfo_window_entries[] = {
    {
        .name = "Processor Identifier",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = "MC68030",
        },
    },
    {
        .name = "Processor Vendor",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = "Motorola",
        },
    },
    {
        .name = "Processor Speed",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = "33 MHz",
        },
    },
    {
        .name = "Processor ISA",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = "M68k",
        },
    },
};

static struct cfgutil_window cpuinfo_window = {
    .name = "CPU Information",
};

static struct cfgutil_entry meminfo_window_entries[] = {
    {
        .name = "Total Memory",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = NULL,
        },
    },
    {
        .name = "Memory Bus Speed",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = "Asynchronous",
        },
    },
    CFG_BLANK_ENTRY,
    CFG_TEXT_ENTRY("Memory Bank Information"),
    {
        .name = "Slot #0 Bank #0",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = NULL,
        },
    },
    {
        .name = "Slot #0 Bank #1",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = NULL,
        },
    },
    {
        .name = "Slot #1 Bank #0",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = NULL,
        },
    },
    {
        .name = "Slot #1 Bank #1",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = NULL,
        },
    },
    {
        .name = "Slot #2 Bank #0",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = NULL,
        },
    },
    {
        .name = "Slot #2 Bank #1",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = NULL,
        },
    },
    {
        .name = "Slot #3 Bank #0",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = NULL,
        },
    },
    {
        .name = "Slot #3 Bank #1",
        .flags = 0,
        .type = CET_STRING,
        .cfg_string = {
            .str = NULL,
        },
    },
};

static struct cfgutil_window meminfo_window = {
    .name = "Memory Information",
};

static struct cfgutil_entry overview_entries[] = {
    {
        .name = "System Date",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_DATE,
        .tick = NULL,  // update clock
        .cfg_date = {
            .handler = NULL,  // value modification handler
        },
    },
    {
        .name = "System Time",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_TIME,
        .tick = NULL,  // update clock
        .cfg_time = {
            .handler = NULL,  // value modification handler
        },
    },
    CFG_BLANK_ENTRY,
    CFG_TEXT_ENTRY("Detailed System Information"),
    {
        .name = "CPU Information",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_SUBWIN,
        .cfg_subwin = {
            .win = &cpuinfo_window,
        },
    },
    {
        .name = "Memory Information",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_SUBWIN,
        .cfg_subwin = {
            .win = &meminfo_window,
        },
    },
    {
        .name = "Board Information",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_SUBWIN,
        .cfg_subwin = {
            .win = NULL,
        },
    },
};

static const char *const avec_level_str[] = {
    "Disabled",
    "Level 1",
    "Level 2",
    "Level 3",
    "Level 4",
    "Level 5",
    "Level 6",
    "NMI",
};

static struct cfgutil_entry pconfig_window_entries[] = {
    {
        .name = "IRQ0 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ1 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ2 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ3 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ4 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ5 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ6 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ7 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ8 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ9 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ10 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ11 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ12 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ13 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ14 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ15 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ16 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ17 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ18 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ19 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ20 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ21 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ22 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
    {
        .name = "IRQ23 Autovector Level",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENUM,
        .cfg_enum = {
            .str_list = avec_level_str,
            .handler = NULL,
        },
    },
};

static struct cfgutil_window pconfig_window = {
    .name = "Peripheral Configuration",
};

static struct cfgutil_entry advanced_entries[] = {
    {
        .name = "Memory Configuration",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_SUBWIN,
        .cfg_subwin = {
            .win = NULL,
        },
    },
    {
        .name = "Interface Configuration",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_SUBWIN,
        .cfg_subwin = {
            .win = NULL,
        },
    },
    {
        .name = "Peripheral Configuration",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_SUBWIN,
        .cfg_subwin = {
            .win = NULL,
        },
    },
};

static struct cfgutil_entry boot_entries[] = {
    CFG_TEXT_ENTRY("Boot Configuration"),
    {
        .name = "Bootup NumLock State",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_ENABLE,
        .cfg_enable = {
            .handler = NULL,
        },
    },
    CFG_BLANK_ENTRY,
    CFG_TEXT_ENTRY("Boot Option Priority"),
};

static struct cfgutil_entry utility_entries[] = {
    {
        .name = "Memory Test Utility",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_EXEC,
        .cfg_exec = {
            .handler = NULL,
        },
    },
    {
        .name = "VGA Display Test Utility",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_EXEC,
        .cfg_exec = {
            .handler = NULL,
        },
    },
    {
        .name = "System Monitor Utility",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_EXEC,
        .cfg_exec = {
            .handler = NULL,
        },
    },
    {
        .name = "Integrated Shell",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_EXEC,
        .cfg_exec = {
            .handler = NULL,
        },
    },
};

static struct cfgutil_entry exit_entries[] = {
    {
        .name = "Save Changes and Reset",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_EXEC,
        .cfg_exec = {
            .handler = NULL,
        },
    },
    {
        .name = "Discard Changes and Continue Boot",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_EXEC,
        .cfg_exec = {
            .handler = NULL,
        },
    },
    {
        .name = "Discard Changes",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_EXEC,
        .cfg_exec = {
            .handler = NULL,
        },
    },
    {
        .name = "Restore Defaults",
        .help = "",
        .flags = CEF_SELECTABLE,
        .type = CET_EXEC,
        .cfg_exec = {
            .handler = NULL,
        },
    },
};

static struct cfgutil_window cfgwins[] = {
    {
        .name = "Overview",
    },
    {
        .name = "Advanced",
    },
    {
        .name = "Boot",
    },
    {
        .name = "Utility",
    },
    {
        .name = "Exit",
    },
};

__attribute__((constructor))
static void init_cfgutil_entries(void)
{
    for (int i = 0; i < ARRAY_SIZE(cpuinfo_window_entries); i++) {
        cfgutil_add_entry(&cpuinfo_window, &cpuinfo_window_entries[i]);
    }
    
    for (int i = 0; i < ARRAY_SIZE(meminfo_window_entries); i++) {
        cfgutil_add_entry(&meminfo_window, &meminfo_window_entries[i]);
    }

    for (int i = 0; i < ARRAY_SIZE(overview_entries); i++) {
        cfgutil_add_entry(&cfgwins[0], &overview_entries[i]);
    }

    for (int i = 0; i < ARRAY_SIZE(pconfig_window_entries); i++) {
        cfgutil_add_entry(&pconfig_window, &pconfig_window_entries[i]);
    }

    for (int i = 0; i < ARRAY_SIZE(advanced_entries); i++) {
        cfgutil_add_entry(&cfgwins[1], &advanced_entries[i]);
    }

    for (int i = 0; i < ARRAY_SIZE(boot_entries); i++) {
        cfgutil_add_entry(&cfgwins[2], &boot_entries[i]);
    }

    for (int i = 0; i < ARRAY_SIZE(utility_entries); i++) {
        cfgutil_add_entry(&cfgwins[3], &utility_entries[i]);
    }

    for (int i = 0; i < ARRAY_SIZE(exit_entries); i++) {
        cfgutil_add_entry(&cfgwins[4], &exit_entries[i]);
    }

    for (int i = 0; i < ARRAY_SIZE(cfgwins); i++) {
        cfgutil_add_root_window(&cfgwins[i]);
    }
}
