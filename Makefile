CROSS_COMPILER:=
CC:= $(CROSS_COMPILE)gcc
AR:= $(CROSS_COMPILE)ar
CFLAGS:= -fPIC
TOP_DIR:= $(shell pwd)
HEADERS_DIR := $(shell pwd)/target/include
LIBS_DIR := $(shell pwd)/target/lib
EXAMPLE_DIR := $(shell pwd)/test/
HEADERS_FILE:= $(TOP_DIR)/target/include/.headers

SRCS +=$(wildcard $(TOP_DIR)/hash/*.c)
SRCS += $(wildcard $(TOP_DIR)/list/*.c)
SRCS += $(wildcard $(TOP_DIR)/map/*.c)

OBJS:=$(patsubst %.c, %.o, $(SRCS))

TEST_SRCS := $(wildcard $(TOP_DIR)/test/*.c)
TEST_OBJS := $(patsubst %.c,%, $(TEST_SRCS))


.phony: all headers libs example clean

all: example

libs: $(OBJS)
	$(AR) cr $(LIBS_DIR)/libbase.a $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

example: $(TEST_OBJS)

$(TOP_DIR)/test/%: $(TOP_DIR)/test/%.c libs headers
	$(CC) -o $@ $< -I$(HEADERS_DIR) -L$(LIBS_DIR) -lbase

headers: $(HEADERS_FILE)

$(HEADERS_FILE):
	cp $(TOP_DIR)/hash/*.h $(HEADERS_DIR)
	cp $(TOP_DIR)/list/*.h $(HEADERS_DIR)
	cp $(TOP_DIR)/map/*.h $(HEADERS_DIR)
	touch $@

clean:
	rm -f $(HEADERS_DIR)/*
	rm -f $(HEADERS_FILE)
	rm -f $(LIBS_DIR)/*
	rm -f $(OBJS)
	rm -f $(TEST_OBJS)
