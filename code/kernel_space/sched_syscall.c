#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/timekeeping.h>
#include <linux/uaccess.h>
#include <linux/smp.h>

#define MAX_EVENTS 1000

struct sched_event {
    u64 timestamp;
    int cpu;
    int event; // 0 = IN, 1 = OUT
    pid_t pid;
};

static struct sched_event buffer[MAX_EVENTS];
static int index = 0;

void log_sched_event(pid_t pid, int event)
{
    buffer[index].timestamp = ktime_get_ns();
    buffer[index].cpu = smp_processor_id();
    buffer[index].event = event;
    buffer[index].pid = pid;

    index = (index + 1) % MAX_EVENTS;
}

SYSCALL_DEFINE3(get_sched_history,
                pid_t, pid,
                struct sched_event __user *, user_buf,
                int, max_events)
{
    int i, count = 0;

    for (i = 0; i < MAX_EVENTS && count < max_events; i++) {
        if (buffer[i].pid == pid) {
            if (copy_to_user(&user_buf[count],
                             &buffer[i],
                             sizeof(struct sched_event)))
                return -EFAULT;
            count++;
        }
    }

    return count;
}
