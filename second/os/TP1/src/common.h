#ifndef __COMMON_H_SEEN__
#define __COMMON_H_SEEN__

// colours from https://gist.github.com/RobinMalfait/7881398
#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */
#define WSH_RL_BUFFERSIZE 1024
#define WSH_SHELL_LOGO "ヅ"
#define WSH_TOK_SEP " \t\r\n\a"
#define WSH_TOK_BUFFERSIZE 64
#define WSH_SHELL_NAME "wsh"
#define WSH_JOB_EXIT "Foreground job exited with code %d.\n"

#endif
