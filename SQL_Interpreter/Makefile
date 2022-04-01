CXXC=g++
CC=gcc
FLAGS=-c -Wall
CXXFLAGS=-O2 -ftrapv -fsanitize=undefined,address -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wcast-qual -Wwrite-strings -lm
CFLAGS=-O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11 -lm

all: client server

Server.o: Server.hpp Server.cpp RDBMSTable.o Query_messages.o Query_execute.o
	$(CXXC) $(CXXFLAGS) -c Server.cpp -o Server.o
Client.o: Client.hpp Client.cpp Parser.o Query_messages.o
	$(CXXC) $(CXXFLAGS) -c Client.cpp -o Client.o
table.o: table.h table.c
	$(CC) $(CFLAGS) -c table.c
RDBMSTable.o: table.o RDBMSTable.cpp RDBMSTable.hpp
	$(CXXC) $(CXXFLAGS) -c RDBMSTable.cpp
Lexeme.o: Lexeme.hpp Lexeme.cpp
	$(CXXC) $(CXXFLAGS) -c Lexeme.cpp
Scanner.o: Scanner.hpp Scanner.cpp Lexeme.o
	$(CXXC) $(CXXFLAGS) -c Scanner.cpp
Parser.o: Parser.hpp Parser.cpp Lexeme.o Scanner.o Query.o Query_execute.o
	$(CXXC) $(CXXFLAGS) -c Parser.cpp
Poliz_elem.o: Poliz_elem.hpp Poliz_elem.cpp
	$(CXXC) $(CXXFLAGS) -c Poliz_elem.cpp -o Poliz_elem.o
Query.o: Query.hpp Query.cpp table.o RDBMSTable.o Poliz_elem.o
	$(CXXC) $(CXXFLAGS) -c Query.cpp -o Query.o
Query_messages.o: Query.hpp Query_messages.cpp
	$(CXXC) $(CXXFLAGS) -c Query_messages.cpp -o Query_messages.o
Query_execute.o: Query.o Query_calc_like.o Query_execute.cpp Poliz_elem.o
	$(CXXC) $(CXXFLAGS) -c Query_execute.cpp -o Query_execute.o
Query_calc_like.o: Query_calc_like.hpp Query_calc_like.cpp
	$(CXXC) $(CXXFLAGS) -c Query_calc_like.cpp -o Query_calc_like.o
client: Client.o Client_main.cpp 
	$(CXXC) $(CXXFLAGS) Lexeme.o Scanner.o Parser.o Poliz_elem.o Query.o Query_messages.o Client.o Client_main.cpp -o client
server: Server.o Server_main.cpp
	$(CXXC) $(CXXFLAGS) table.o RDBMSTable.o Poliz_elem.o Query_calc_like.o Query_execute.o Query_messages.o Server.o Server_main.cpp -o server
clean:
	-rm *.o client server
