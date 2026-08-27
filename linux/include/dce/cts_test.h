#ifndef	_CTS_TEST_H_
#define	_CTS_TEST_H_

#include <dce/cma.h>
#include <sys/time.h>

#define check(status,string) \
    if ((status) == -1) perror(string);

extern void cts_test(char *, char *);
extern void cts_comment(char *, ...);
extern int cts_exit(void);
extern void cts_failed(char *, ...);
extern int cts_result(void);

typedef struct timeval cts_timebuf_t;

#define cts_gettime(pbuf)	gettimeofday((pbuf), (struct timezone *)0)

float cts_timecvt(cts_timebuf_t *);
float cts_timediff(cts_timebuf_t *, cts_timebuf_t *);
void cts_timerecord(cts_timebuf_t *, cts_timebuf_t *, cts_timebuf_t *);

#endif
