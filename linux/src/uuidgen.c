#include <dce/uuid.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
usage(const char *argv0)
{
    fprintf(stderr, "Usage: %s [-n count]\n", argv0);
}

int
main(int argc, char **argv)
{
    int count = 1;
    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            count = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        } else {
            usage(argv[0]);
            return 2;
        }
    }

    for (i = 0; i < count; i++) {
        uuid_t u;
        unsigned32 st;
        unsigned_char_p_t str = NULL;
        uuid_create(&u, &st);
        if (st != uuid_s_ok) {
            fprintf(stderr, "uuid_create failed: 0x%x\n", st);
            return 1;
        }
        uuid_to_string(&u, &str, &st);
        if (st != uuid_s_ok) {
            fprintf(stderr, "uuid_to_string failed: 0x%x\n", st);
            return 1;
        }
        printf("%s\n", str);
        free(str);
    }
    return 0;
}
