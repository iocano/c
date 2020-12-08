MAIN_SRC_PATH = ./src/program
LIB_SRC_INCLUDE = ./src/library/include
LIB_SRC_PATH = ./src/library
OUT_DIR = ./bin

MKDIR_P = mkdir -p

all: bin main clean

# create bin file
bin:
	$(MKDIR_P) bin

# create executable
main: main.o libpath.a
	gcc $(OUT_DIR)/main.o -L $(OUT_DIR) -I $(LIB_SRC_INCLUDE) -lpath -o $(OUT_DIR)/main

main.o: $(MAIN_SRC_PATH)/main.c
	gcc -c $(MAIN_SRC_PATH)/main.c -I $(LIB_SRC_INCLUDE) -o $(OUT_DIR)/main.o

# create library
libpath.a: path.o
	ar -rcs $(OUT_DIR)/libpath.a $(OUT_DIR)/path.o

path.o: $(LIB_SRC_PATH)/path.c
	gcc -c $(LIB_SRC_PATH)/path.c -I $(LIB_SRC_INCLUDE) -o $(OUT_DIR)/path.o

# remove *.o files
clean:
	rm $(OUT_DIR)/*.o

