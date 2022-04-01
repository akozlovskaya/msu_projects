CC = gcc
CFLAGS = -O2 -ftrapv -fsanitize=address,undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11 -lm
SRCMODULES = stack.c poliz.c init_var.c calc.c recurs_expl.c
OBJMODULES = $(SRCMODULES:.c=.o)
%.o :%.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
all: prog clean
tests: prog_test prog_test1 prog_test2 prog_test3 prog_test4 prog_test5 prog_test6 prog_test7 prog_test8 prog_test9 prog_test10 prog_test11 prog_test12 prog_test13 prog_test14 prog_test15 prog_test16 prog_test17 prog_test18 prog_test19 prog_test20 prog_test21 prog_test22 clean_tests
prog: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@
prog_test: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test.txt
prog_test1: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test1.txt
prog_test2: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test2.txt
prog_test3: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test3.txt
prog_test4: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test4.txt
prog_test5: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test5.txt
prog_test6: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test6.txt
prog_test7: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test7.txt
prog_test8: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test8.txt
prog_test9: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test9.txt
prog_test10: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test10.txt
prog_test11: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test11.txt
prog_test12: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test12.txt
prog_test13: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test13.txt
prog_test14: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test14.txt
prog_test15: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test15.txt
prog_test16: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test16.txt
prog_test17: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test17.txt
prog_test18: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test18.txt
prog_test19: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test19.txt
prog_test20: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test20.txt
prog_test21: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test21.txt
prog_test22: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ && ./$@  < test22.txt
clean:
	rm -f *.o prog
	rm -f *.exe prog
clean_tests:
	rm -f *.o tests
	rm -f *.exe tests