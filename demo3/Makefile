LEX = flex
YACC = bison
CC = gcc

CFLAGS = -Wall -g -fsanitize=address
LDFLAGS = -g -fsanitize=address

TARGET = sim-gen
LEXSOURCES = ode-parser.l
YACCSOURCES = ode-parser.y
SOURCES = ast.c

BUILD_DIR = build

SOURCES += $(LEXSOURCES:.l=.lex.c) $(YACCSOURCES:.y=.tab.c)
OBJS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)

default: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.tab.c %.tab.h: %.y
	$(YACC) -d $<

%.lex.c: %.l %.tab.h
	$(LEX) -o $@ $<

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) *.tab.[ch] *.lex.c

rebuild: clean default

run: $(TARGET)
	./$<

.PHONY: default clean rebuild run
.SECONDARY: