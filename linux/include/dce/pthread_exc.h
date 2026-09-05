#ifndef PTHREAD_EXC
#define PTHREAD_EXC

/*
 * Exception-raising DCE pthreads interface. For the Linux port the
 * draft-4 status-returning API is used; callers that wrap pthread
 * operations in TRY/CATCH still compile.
 */
#include <dce/pthread.h>
#include <dce/exc_handling.h>

#endif
