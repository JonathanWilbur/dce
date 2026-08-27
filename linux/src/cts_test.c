/*
 * CMA test harness (Linux port of src/test/threads/cts_test.c).
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

static char *cts___g_test_name;
static int cts___g_test_success = 1;
static int cts___g_test_completed;

void
cts_test(char *name, char *desc)
{
    cts___g_test_name = name;
    cts___g_test_success = 1;
    cts___g_test_completed = 0;
    printf("%%%s: %s\n", name, desc);
    fflush(stdout);
}

void
cts_comment(char *fmt, ...)
{
    va_list ap;
    printf("%% ");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    fflush(stdout);
}

void
cts_failed(char *fmt, ...)
{
    va_list ap;
    cts___g_test_success = 0;
    printf("%% FAILED: ");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    fflush(stdout);
}

int
cts_exit(void)
{
    cts___g_test_completed = 1;
    return cts___g_test_success ? 0 : -1;
}

int
cts_result(void)
{
    cts___g_test_completed = 1;
    if (cts___g_test_success) {
        printf("%%%%%s passed\n",
            cts___g_test_name ? cts___g_test_name : "test");
        fflush(stdout);
        return 0;
    }
    printf("%%%%%s failed\n",
        cts___g_test_name ? cts___g_test_name : "test");
    fflush(stdout);
    return -1;
}

float
cts_timecvt(cts_timebuf_t *t)
{
    return (float)t->tv_sec + (float)t->tv_usec / 1e6f;
}

float
cts_timediff(cts_timebuf_t *a, cts_timebuf_t *b)
{
    return cts_timecvt(b) - cts_timecvt(a);
}

void
cts_timerecord(cts_timebuf_t *start, cts_timebuf_t *end, cts_timebuf_t *out)
{
    long usec = end->tv_usec - start->tv_usec;
    out->tv_sec = end->tv_sec - start->tv_sec;
    if (usec < 0) {
        out->tv_sec--;
        usec += 1000000;
    }
    out->tv_usec = usec;
}
