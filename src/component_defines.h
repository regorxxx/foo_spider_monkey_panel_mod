#pragma once

#define SMP_NAME              "Spider Monkey Panel"
#define SMP_UNDERSCORE_NAME   "foo_spider_monkey_panel"
#define SMP_WINDOW_CLASS_NAME SMP_UNDERSCORE_NAME "_class"
#define SMP_DLL_NAME          SMP_UNDERSCORE_NAME ".dll"

#define SMP_VERSION "1.6.2.25.05.19"
#define SMP_NAME_WITH_VERSION SMP_NAME " v" SMP_VERSION

#define SMP_ABOUT                                                   \
    SMP_NAME_WITH_VERSION " by TheQwertiest\n"                      \
                          "Based on JScript Panel by marc2003\n"    \
                          "Based on WSH Panel Mod by T.P. Wang\n\n" \
                          "Build: " __TIME__ ", " __DATE__ "\n"     \
                          "Columns UI SDK Version: " UI_EXTENSION_VERSION
