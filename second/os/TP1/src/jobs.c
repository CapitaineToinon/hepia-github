#include "jobs.h"

/**
 * Handler for the signal SIGCHLD
 * @param int the signal number
 */
static void sigchld_handler(int);

/**
 * Handler for the signal SIGHUP
 * @param int the signal number
 */
static void sighup_handler(int);

/**
 * Handler for the signal SIGINT
 * @param int the signal number
 */
static void sigint_handler(int);

struct sigaction sa_sighup;
struct sigaction sa_sigint;
struct sigaction sa_sigchld;

volatile pid_t foreground_jobs[WSH_MAX_JOBS] = {-1};
volatile pid_t background_job = -1;

void free_job(int index)
{
    if (index >= WSH_MAX_JOBS)
    {
        return;
    }

#ifdef DEBUG
    printf("[DEBUG]: Freed foreground job with pid [%d]\n", foreground_jobs[index]);
#endif

    foreground_jobs[index] = -1;
}

int save_foreground_job(pid_t pid)
{
    for (int i = 0; i < WSH_MAX_JOBS; i++)
    {
        if (foreground_jobs[i] == -1)
        {
#ifdef DEBUG
            printf("[DEBUG]: Saved foreground job with pid [%d]\n", pid);
#endif
            foreground_jobs[i] = pid;
            return i;
        }
    }

    return -1;
}

void free_background_job()
{
    if (background_job != -1)
    {
#ifdef DEBUG
        printf("[DEBUG]: Freed background job with pid [%d]\n", background_job);
#endif
        background_job = -1;
    }
}

bool save_background_job(pid_t pid)
{
    if (has_background_job())
    {
        fprintf(stderr, "There is already a background job running\n");
        return false;
    }

#ifdef DEBUG
    printf("[DEBUG]: Saved background job with pid [%d]\n", pid);
#endif

    background_job = pid;
    return true;
}

bool has_background_job()
{
    return background_job != -1;
}

static void sigchld_handler(int sig)
{
    (void)sig;
    int status;
    pid_t child;

#ifdef DEBUG
    printf("[DEBUG]: Recieved SIGCHLD\n");
#endif

    // Use waitpid after we recieve a signal from a child to process it
    while ((child = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            int code = WEXITSTATUS(status);
            printf(WSH_BACKGROUND_JOB_EXIT, child, code);
            free_background_job();
        }

        if (WIFSIGNALED(status))
        {
            int code = WIFSIGNALED(status);
            printf(WSH_BACKGROUND_JOB_EXIT, child, code);
            free_background_job();
        }
    }
}

static void sighup_handler(int sig)
{
#ifdef DEBUG
    printf("[DEBUG]: Recieved SIGHUP\n");
#endif
    // Kill all the currently running jobs
    // when sighup is recieved then exit
    for (int i = 0; i < WSH_MAX_JOBS; i++)
    {
        if (foreground_jobs[i] != -1)
        {
            if (kill(foreground_jobs[i], sig) != 0)
            {
                perror(WSH_SHELL_NAME);
            }
            else
            {
                free_job(i);
            }
        }
    }

    // Kill background job if possible
    if (has_background_job())
    {
        if (kill(background_job, sig) != 0)
        {
            perror(WSH_SHELL_NAME);
        }
    }

    exit(EXIT_SUCCESS);
}

static void sigint_handler(int sig)
{
#ifdef DEBUG
    printf("[DEBUG]: Recieved SIGINT\n");
#endif
    // Kill all the currently running foreground jobs
    // when sigint is recieved
    for (int i = 0; i < WSH_MAX_JOBS; i++)
    {
        if (foreground_jobs[i] != -1)
        {
            if (kill(foreground_jobs[i], sig) != 0)
            {
                perror(WSH_SHELL_NAME);
            }
            else
            {
                free_job(i);
            }
        }
    }
}

int init_signals()
{
    // SIGHUP
    sa_sighup.sa_handler = sighup_handler;
    sigemptyset(&sa_sighup.sa_mask);
    sa_sighup.sa_flags = SA_RESTART;

    if (sigaction(SIGHUP, &sa_sighup, NULL) == -1)
    {
        fprintf(stderr, "Failed to sigaction in main: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // SIGINT
    sa_sigint.sa_handler = sigint_handler;
    sigemptyset(&sa_sigint.sa_mask);
    sa_sigint.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa_sigint, NULL) == -1)
    {
        fprintf(stderr, "Failed to sigaction in main: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // SIGCHLD
    sa_sigchld.sa_handler = &sigchld_handler;
    sigemptyset(&sa_sigchld.sa_mask);
    sa_sigchld.sa_flags = SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa_sigchld, 0) == -1)
    {
        fprintf(stderr, "Failed to sigaction in main: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}