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
main: main.o libpath.so
	gcc $(OUT_DIR)/main.o -L $(OUT_DIR) -I $(LIB_SRC_INCLUDE) -lpath -o $(OUT_DIR)/main

main.o: $(MAIN_SRC_PATH)/main.c
	gcc -c $(MAIN_SRC_PATH)/main.c -I $(LIB_SRC_INCLUDE) -o $(OUT_DIR)/main.o

# create library
libpath.so: path.o
	gcc -shared $(OUT_DIR)/path.o -o $(OUT_DIR)/libpath.so

# create object file with fPIC flag
path.o: $(LIB_SRC_PATH)/path.c
	gcc -c $(LIB_SRC_PATH)/path.c -I $(LIB_SRC_INCLUDE) -fPIC -o $(OUT_DIR)/path.o

# remove *.o files
clean:
	rm $(OUT_DIR)/*.o

