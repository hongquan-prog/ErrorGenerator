.PHONY:all clean rebuild

TARGET := error-generator

DIR_SRC := tools/src
DIR_INC := tools/inc
DIR_BUILD := build

SRCS := $(wildcard $(DIR_SRC)/*.cpp)
TARGET := $(addprefix $(DIR_BUILD)/,$(TARGET))

all: $(DIR_BUILD)
	g++ $(SRCS) -o $(TARGET) -I${DIR_INC}

$(DIR_BUILD):
	mkdir -p $@

clean:
	rm -r $(DIR_BUILD)

rebuild: clean all

