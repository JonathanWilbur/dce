#include <dce/pthread.h>
#include <dce/cts_test.h>
#include <dce/exc_handling.h>
#include <dce/uuid.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static pthread_mutex_t count_lock;
static int count;

static pthread_addr_t
worker(pthread_addr_t arg)
{
    int i;
    for (i = 0; i < 1000; i++) {
        pthread_mutex_lock(&count_lock);
        count++;
        pthread_mutex_unlock(&count_lock);
    }
    return arg;
}

static int
test_threads(void)
{
    pthread_t t1, t2;
    pthread_addr_t r1, r2;

    cts_test("linux_threads", "create, mutex, join");
    pthread_mutex_init(&count_lock, pthread_mutexattr_default);
    count = 0;
    pthread_create(&t1, pthread_attr_default, worker, (pthread_addr_t)1);
    pthread_create(&t2, pthread_attr_default, worker, (pthread_addr_t)2);
    pthread_join(t1, &r1);
    pthread_join(t2, &r2);
    pthread_mutex_destroy(&count_lock);
    if (count != 2000) {
        cts_failed("count=%d expected 2000", count);
        return cts_result();
    }
    return cts_result();
}

static int
test_exceptions(void)
{
    EXCEPTION boom;
    int caught = 0;

    cts_test("linux_exc", "TRY/CATCH");
    EXCEPTION_INIT(boom);
    TRY {
        RAISE(boom);
        cts_failed("RAISE did not transfer control");
    }
    CATCH(boom) {
        caught = 1;
    }
    ENDTRY
    if (!caught) {
        cts_failed("exception not caught");
        return cts_result();
    }
    return cts_result();
}

static int
test_uuid(void)
{
    uuid_t a, b, n;
    unsigned32 st;
    unsigned_char_p_t s = NULL;
    uuid_t parsed;

    cts_test("linux_uuid", "create / string round-trip");
    uuid_create_nil(&n, &st);
    if (!uuid_is_nil(&n, &st)) {
        cts_failed("nil uuid not nil");
        return cts_result();
    }
    uuid_create(&a, &st);
    uuid_create(&b, &st);
    if (uuid_equal(&a, &b, &st)) {
        cts_failed("two random uuids collided");
        return cts_result();
    }
    uuid_to_string(&a, &s, &st);
    if (st != uuid_s_ok || s == NULL || strlen((char *)s) != 36) {
        cts_failed("uuid_to_string produced %s", s ? (char *)s : "(null)");
        free(s);
        return cts_result();
    }
    uuid_from_string(s, &parsed, &st);
    if (st != uuid_s_ok || !uuid_equal(&a, &parsed, &st)) {
        cts_failed("round-trip mismatch: %s", s);
        free(s);
        return cts_result();
    }
    free(s);
    return cts_result();
}

int
main(void)
{
    int rc = 0;
    rc |= test_threads();
    rc |= test_exceptions();
    rc |= test_uuid();
    return rc ? 1 : 0;
}
