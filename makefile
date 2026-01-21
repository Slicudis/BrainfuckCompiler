FLAGS = -Wno-format-overflow

all: src/main.c
	rm -f gbf
	gcc src/main.c src/cmd_processor.c src/compiler.c -O2 $(FLAGS) -o gbf
