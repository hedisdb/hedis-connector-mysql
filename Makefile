CC = gcc
CFLAGS = -O1 -std=c99 -fPIC
MYSQLFLAGS = $(shell mysql_config --cflags --libs)
NAME = $(shell basename $(shell pwd))
TARGET = lib${NAME}.so

main: main.o
	${CC} -shared $< ${MYSQLFLAGS} -o ${TARGET}

%.o: %.c
	${CC} $< ${CFLAGS} ${MYSQLFLAGS} -c

.PHONY: install

install:
	cp ${TARGET} /usr/lib/

.PHONY: uninstall

uninstall:
	rm /usr/lib/${TARGET}

.PHONY: clean

clean:
	@rm -rf *.o *.so
