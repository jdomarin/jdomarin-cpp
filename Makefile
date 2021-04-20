CC=g++
CFLAGS=-Wall -g
NAME=MountainCar

./$(NAME) : $(NAME)1.o tiles.o
	$(CC) $(CFLAGS) -o $@ $^

$(NAME)1.o : $(NAME)1.cpp tiles.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME)2.o : $(NAME)2.cpp tiles.h
	$(CC) $(CFLAGS) -c $< -o $@

tiles.o : tiles.cpp tiles.h
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm *.o
	rm ./$(NAME)

dist :
	zip -r ../$(NAME).zip ./
