# GNU Make Linux port of OSF DCE 1.2.2
#
# PORTING_GUIDE.pdf §1.7.2.1: ODE makefiles target a complete DCE build.
# Piecemeal ports may supply new makefiles. ODE's make dialect is not GNU
# make; this file is the Linux build.

ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)
LROOT := $(ROOT)/linux
SRC := $(LROOT)/src
INC := $(LROOT)/include
BUILD := $(LROOT)/build
DCE_SRC := $(ROOT)/dce/src

CC ?= gcc
CFLAGS ?= -O2 -g -Wall -Wextra
DCE_CPPFLAGS := -I$(INC) -I$(INC)/dce -I$(SRC) -D_GNU_SOURCE -D_REENTRANT -DLINUX
NATIVE_CPPFLAGS := -D_GNU_SOURCE -D_REENTRANT
LDFLAGS += -pthread -ldl

LEGACY_CFLAGS := -std=gnu89 -Wno-implicit-function-declaration \
	-Wno-implicit-int -Wno-return-type -Wno-unused-parameter \
	-Wno-old-style-definition -Wno-missing-prototypes \
	-Wno-incompatible-pointer-types -include $(INC)/dce/cts_test.h

.PHONY: all clean test tools lib

all: lib tools test-bins

lib: $(BUILD)/libdcethreads.a $(BUILD)/libdceuuid.a

tools: $(BUILD)/uuidgen

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/native_pthreads.o: $(SRC)/native_pthreads.c $(SRC)/native_pthreads.h | $(BUILD)
	$(CC) $(CFLAGS) $(NATIVE_CPPFLAGS) -c -o $@ $<

$(BUILD)/dcethreads.o: $(SRC)/dcethreads.c $(SRC)/native_pthreads.h | $(BUILD)
	$(CC) $(CFLAGS) $(DCE_CPPFLAGS) -Wno-unused-parameter -c -o $@ $<

$(BUILD)/cts_test.o: $(SRC)/cts_test.c | $(BUILD)
	$(CC) $(CFLAGS) $(DCE_CPPFLAGS) -Wno-unused-parameter -c -o $@ $<

$(BUILD)/uuid.o: $(SRC)/uuid.c | $(BUILD)
	$(CC) $(CFLAGS) $(DCE_CPPFLAGS) -c -o $@ $<

$(BUILD)/uuidgen.o: $(SRC)/uuidgen.c | $(BUILD)
	$(CC) $(CFLAGS) $(DCE_CPPFLAGS) -c -o $@ $<

$(BUILD)/libdcethreads.a: $(BUILD)/native_pthreads.o $(BUILD)/dcethreads.o $(BUILD)/cts_test.o
	$(AR) rcs $@ $^

$(BUILD)/libdceuuid.a: $(BUILD)/uuid.o
	$(AR) rcs $@ $^

$(BUILD)/uuidgen: $(BUILD)/uuidgen.o $(BUILD)/libdceuuid.a
	$(CC) -o $@ $(BUILD)/uuidgen.o $(BUILD)/libdceuuid.a $(LDFLAGS)

$(BUILD)/linux_selftest: $(LROOT)/tests/linux_selftest.c lib
	$(CC) $(CFLAGS) $(DCE_CPPFLAGS) -o $@ $< \
		$(BUILD)/libdcethreads.a $(BUILD)/libdceuuid.a $(LDFLAGS)

$(BUILD)/pthread_hello: $(DCE_SRC)/test/threads/pthread_hello.c lib
	$(CC) $(CFLAGS) $(LEGACY_CFLAGS) $(DCE_CPPFLAGS) -o $@ $< \
		$(BUILD)/libdcethreads.a $(LDFLAGS)

$(BUILD)/crvb_thd_001: $(DCE_SRC)/test/threads/crvb_thd_001.c lib
	$(CC) $(CFLAGS) $(LEGACY_CFLAGS) $(DCE_CPPFLAGS) -o $@ $< \
		$(BUILD)/libdcethreads.a $(LDFLAGS)

$(BUILD)/crvb_mut_001: $(DCE_SRC)/test/threads/crvb_mut_001.c lib
	$(CC) $(CFLAGS) $(LEGACY_CFLAGS) $(DCE_CPPFLAGS) -o $@ $< \
		$(BUILD)/libdcethreads.a $(LDFLAGS)

test-bins: $(BUILD)/linux_selftest $(BUILD)/pthread_hello \
	$(BUILD)/crvb_thd_001 $(BUILD)/crvb_mut_001 $(BUILD)/uuidgen

test: test-bins
	@echo "=== linux_selftest ==="
	$(BUILD)/linux_selftest
	@echo "=== pthread_hello (OSF test) ==="
	$(BUILD)/pthread_hello
	@echo "=== CRVB_THD_001 (OSF test) ==="
	$(BUILD)/crvb_thd_001
	@echo "=== CRVB_MUT_001 (OSF test, 8 threads x 2000 locks) ==="
	$(BUILD)/crvb_mut_001 8 2000
	@echo "=== uuidgen ==="
	$(BUILD)/uuidgen -n 3
	@echo "All Linux port tests passed."

clean:
	rm -rf $(BUILD)
