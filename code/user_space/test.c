#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_get_sched_history 451
#define MAX_EVENTS 1000

struct sched_event {
    unsigned long long timestamp;
    int cpu;
    int event; // 0 = IN, 1 = OUT
    int pid;
};

int main()
{
    struct sched_event buffer[MAX_EVENTS];

    int pid = getpid();
    printf("PID: %d\n\n", pid);

    //  generate scheduling activity
    for (volatile long i = 0; i < 2000000000; i++);

    int ret = syscall(__NR_get_sched_history,
                      pid,
                      buffer,
                      MAX_EVENTS);

    if (ret <= 0) {
        printf("No events captured\n");
        return 0;
    }

    printf("%-12s %-5s %-6s %-5s\n", "Time(ns)", "CPU", "PID", "Event");

    int printed = 0;

    for (int i = 0; i < ret; i++) {
        if (buffer[i].pid == pid) {

            printf("%-12llu %-5d %-6d %-5s\n",
                   buffer[i].timestamp,
                   buffer[i].cpu,
                   buffer[i].pid,
                   buffer[i].event == 0 ? "IN" : "OUT");

            printed++;

            if (printed >= 6) break;  //  3 IN/OUT pairs
        }
    }

    return 0;
}

