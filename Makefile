CC 		:= gcc
RM		:= rm -rf
MKDIR		:= mkdir -p
QUIET		:= @
BUILD_DIR 	:= build
BIN_DIR		:= $(BUILD_DIR)/bin
SRC_DIR		:= src
VPATH		:= $(SRC_DIR) $(BIN_DIR)

byter: main.c
	$(QUIET)$(MKDIR) $(BIN_DIR)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $(BIN_DIR)/$@

clean:
	$(RM) $(BUILD_DIR)
