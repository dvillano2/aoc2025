.PHONY: all day part run run-day run-part

ifneq ($(part),)
SINGLE_PART = $(wildcard $(day)_*/*_$(part).c)
PART_BINARY = $(patsubst %.c,bin/%,$(SINGLE_PART))
else
ifneq ($(day),)
SINGLE_DAY = $(wildcard $(day)_*/*.c)
DAY_BINARIES = $(patsubst %.c,bin/%,$(SINGLE_DAY))
else
ALL_SOURCES = $(wildcard */*.c)
ALL_BINARIES = $(patsubst %.c,bin/%,$(ALL_SOURCES))
endif
endif

bin/%: %.c
	@mkdir -p $(dir $@)
	@gcc -Wall -Wextra -O2 -std=c11 $< -o $@

ifneq ($(ALL_BINARIES),)
all: $(ALL_BINARIES)
endif

ifneq ($(DAY_BINARIES),)
day: $(DAY_BINARIES)
endif

ifneq ($(PART_BINARY),)
part: $(PART_BINARY)
endif

run: all
	@ for bin in $(ALL_BINARIES); do \
		srcdir=$${bin#bin/}; \
		srcdir=$$(dirname "$$srcdir"); \
		echo "Running $$bin"; \
		(cd "$$srcdir" && ../"$$bin" && echo ""); \
	done

run-day: day
	@ for bin in $(DAY_BINARIES); do \
		srcdir=$${bin#bin/}; \
		srcdir=$$(dirname "$$srcdir"); \
		echo "Running $$bin"; \
		(cd "$$srcdir" && ../"$$bin" && echo ""); \
	done

run-part: part
	@ for bin in $(PART_BINARY); do \
		srcdir=$${bin#bin/}; \
		srcdir=$$(dirname "$$srcdir"); \
		echo "Running $$bin"; \
		(cd "$$srcdir" && ../"$$bin"); \
	done
