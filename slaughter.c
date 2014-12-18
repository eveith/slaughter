/* slaughter -- A simple program to reliably end a process's life.
 * Copyright (C) 2014  Eric MSP Veith <eveith@veith-m.de>
 */


#include <stdlib.h>
#define _GNU_SOURCE 1
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#define DEFAULTWAIT 15


void print_usage()
{
    printf("sl -- A reliable process terminator\n"
            "Usage: sl [-s SECS] PID\n\n"
            "Options:\n\n"
            "-w SECS    Wait SECS seconds for the process to end\n"
            "           (Default: 15 seconds)\n"
            "-e         Waits until the process has terminated "
                "and does not time out.\n");
}


int wait_for_termination(pid_t pid, unsigned seconds)
{
    int rc;

    while (0 != (rc = kill(pid, 0))) {
        sleep(1);
        if (seconds != -1 && 0 == seconds--) {
            break;
        }
    }

    if (0 == rc) {
        rc = -1;
    } else {
        rc = 0;
    }

    return rc;
}


int main(int argc, char *argv[])
{
    pid_t pid;
    unsigned waitsecs = DEFAULTWAIT,
        wait_endlessly;

    int waitrc, c;

    while (-1 != (c = getopt(argc, argv, "ehs:"))) {
        switch (c) {
            case 'w':
                waitsecs = (unsigned)strtoul(optarg, NULL, 10);
                break;
            case 'e':
                wait_endlessly = 1;
                break;
            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
            default:
                exit(1);
        }
    }

    if (optind == argc) {
        fprintf(stderr, "ERROR: Need a PID\n");
        exit(1);
    }

    pid = (pid_t)strtoul(argv[optind], NULL, 10);

    kill(pid, 15);
    if (0 == wait_for_termination(pid, waitsecs)) goto success;
    kill(pid, 2);
    if (0 == wait_for_termination(pid, waitsecs)) goto success;
    kill(pid, 1);
    if (0 == wait_for_termination(pid, waitsecs)) goto success;
    kill(pid, 9);
    if (wait_endlessly) waitsecs = -1;
    if (0 == wait_for_termination(pid, waitsecs)) goto success;

    exit(2); /* Could not terminate the process */

success:
    return EXIT_SUCCESS;
}
