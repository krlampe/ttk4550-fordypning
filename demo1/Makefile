LEX = flex
YACC = bison
CC = gcc

CFLAGS = -Wall -g
LDFLAGS = -g

TARGET = ode-parser

$(TARGET): $(TARGET).l $(TARGET).y
	$(YACC) -d $(TARGET).y
	$(LEX) -o $(TARGET).lex.c $(TARGET).l
	$(CC) -o $@ $(TARGET).tab.c $(TARGET).lex.c $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET) $(TARGET).tab.c $(TARGET).tab.h $(TARGET).lex.c
