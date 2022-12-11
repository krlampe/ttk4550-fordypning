LEX = flex
YACC = bison
CC = g++

CFLAGS = -Wall -g -fsanitize=address
LDFLAGS = -g -fsanitize=address

TARGET = sim-gen
LEXSOURCES = ode-parser.l
YACCSOURCES = ode-parser.y
SOURCES = main.cc ast.cc matlab.cc

BUILD_DIR = build

SOURCES += $(LEXSOURCES:.l=.lex.cc) $(YACCSOURCES:.y=.tab.cc)
OBJS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)
OBJS := $(OBJS:%.cc=$(BUILD_DIR)/%.o)

default: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.tab.c %.tab.h: %.y
	$(YACC) -d $<

%.lex.c: %.l %.tab.h
	$(LEX) -o $@ $<

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/%.o: %.cc | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) *.tab.[ch] *.lex.c

cleanup: clean
	rm -f *.m *.tex

rebuild: clean default

run: $(TARGET)
	./$<

.PHONY: default clean cleanup rebuild run
.SECONDARY: