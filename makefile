.PHONY:all clean rebuild

SRCS := $(wildcard *.cpp)
DIR_BUILD := build
TARGET := error-generator
TARGET := $(addprefix $(DIR_BUILD)/,$(TARGET))

all: $(DIR_BUILD)
	g++ $(SRCS) -o $(TARGET)

$(DIR_BUILD):
	mkdir -p $@

clean:
	rm -r $(DIR_BUILD)

rebuild: clean all

