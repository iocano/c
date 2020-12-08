LIBS = -lallegro -lallegro_ttf -lallegro_font -lallegro_primitives -lallegro_audio -lallegro_acodec -lm
# SRCS = $(wildcard *.c) $(wildcard **/*.c)
SRCS = $(shell find src/ -type f -name '*.c')

target1:
	$(CC) -g $(SRCS) $(LIBS) -o asteroids
