CROSS_COMPILER:=
CC:= $(CROSS_COMPILE)gcc
AR:= $(CROSS_COMPILE)ar
CFLAGS:= -fPIC

TOP_DIR:= $(shell pwd)

HEADERS_DIR := $(shell pwd)/target/include
LIBS_DIR := $(shell pwd)/target/lib
EXAMPLE_DIR := $(shell pwd)/test/


HEADERS_FILE:= $(TOP_DIR)/target/include/.headers
HEADERS_ALL:= $(wildcard $(TOP_DIR)/share/include/*/*.h)
HEADERS_ALL += $(wildcard $(TOP_DIR)/map/*.h)
HEADERS_ALL += $(wildcard $(TOP_DIR)/set/*.h)
HEADERS_ALL += $(wildcard $(TOP_DIR)/multimap/*.h)
HEADERS_ALL += $(wildcard $(TOP_DIR)/queue/*.h)
HEADERS_ALL += $(wildcard $(TOP_DIR)/stack/*.h)


SRCS += $(wildcard $(TOP_DIR)/queue/*.c)
SRCS += $(wildcard $(TOP_DIR)/stack/*.c)
SRCS += $(wildcard $(TOP_DIR)/map/*.c)
SRCS += $(wildcard $(TOP_DIR)/set/*.c)
SRCS += $(wildcard $(TOP_DIR)/multimap/*.c)
SRCS += $(wildcard $(TOP_DIR)/share/src/*/*.c)

OBJS:=$(patsubst %.c, %.o, $(SRCS))

TEST_SRCS := $(wildcard $(TOP_DIR)/test/*.c)
TEST_OBJS := $(patsubst %.c,%, $(TEST_SRCS))


DEP_LIBS += -lpthread

.phony: all libs example clean

all: example

libs: $(OBJS) 
	$(AR) cr $(LIBS_DIR)/libbase.a $(OBJS)

$(OBJS): $(HEADERS_FILE)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ -I$(HEADERS_DIR) $(DEP_LIBS)

example: $(TEST_OBJS)

$(TOP_DIR)/test/%: $(TOP_DIR)/test/%.c libs 
	$(CC) -o $@ $< -I$(HEADERS_DIR) -L$(LIBS_DIR) -lbase

$(HEADERS_FILE):
	cp $(HEADERS_ALL) $(HEADERS_DIR)
	touch $@

clean:
	rm -f $(HEADERS_DIR)/*
	rm -f $(HEADERS_FILE)
	rm -f $(LIBS_DIR)/*
	rm -f $(OBJS)
	rm -f $(TEST_OBJS)
