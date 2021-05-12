#ifndef __JOBS_H_SEEN__
#define __JOBS_H_SEEN__

#include <sys/types.h>
#include <stdbool.h>
#include "wsh.h"

#define WSH_MAX_JOBS 2

typedef struct job_t
{
    pid_t pid;
    bool background;
} job_t;

/**
 * Free a foreground job when it's been waited properly
 * @param int the index of the pid in the foreground jobs
 */
void free_job(int);

/**
 * Saves a foreground job if possible
 * @param pid_t the pid if the job to be saved
 * @returns index where the job was saved or -1 if the job list was already full
 */
int save_foreground_job(pid_t);

/**
 * Free a background job when it's been waited properly
 */
void free_background_job();

/**
 * Saves a background job if possible
 * @param pid_t the pid if the job to be saved
 * @returns if the background job was saved
 */
bool save_background_job(pid_t);

/**
 * Resturns if a background job is already currently running
 * @returns if a background job is already currently running
 */
bool has_background_job();

/**
 * Start listening to the signals required for the jobs
 * @return 0 if no error
 */
int init_signals();

#endif
