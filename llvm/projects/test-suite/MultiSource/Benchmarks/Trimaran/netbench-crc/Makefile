LEVEL = ../../../..

PROG     = netbench-crc
ifdef LARGE_PROBLEM_SIZE
RUN_OPTIONS = 40000
else
ifdef SMALL_PROBLEM_SIZE
RUN_OPTIONS = 1000
else
RUN_OPTIONS = 12000
ifeq ($(ARCH),XCore)
XCORE_TARGET_NEEDS_MEMORY := 256
endif
endif
endif

include $(LEVEL)/MultiSource/Makefile.multisrc
