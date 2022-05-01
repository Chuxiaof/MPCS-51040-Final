EXAMPLES=vtable screen_demo


LIBS=$(shell pkg-config --libs ncurses) -lm
CFLAGS=$(shell pkg-config --cflags ncurses) -Wall -std=c11 -pedantic -Werror 

all: $(EXAMPLES)


%: %.c
	gcc -o $@ $< $(LIBS)


%.o: %.c
	gcc -c $(CFLAGS)  -o $@ $<

screen_demo: screen_demo.o ../screen.o
	gcc $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm $(EXAMPLES)

