ola1:	a-star.cpp random_board.cpp
	g++ -o a-star a-star.cpp
	g++ -o random_board random_board.cpp

clean:
	rm -f a-star
	rm -f random_board
	rm -f *.o
