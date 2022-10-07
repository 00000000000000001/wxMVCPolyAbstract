NAME=wxMVCPolyAbstract

run: $(NAME)
	./$(NAME)

$(NAME): $(NAME).o
	g++ -o $(NAME) $(NAME).o `wx-config --libs` 
$(NAME).o: $(NAME).cc
	g++ -c -std=c++11 `wx-config --cxxflags` $(NAME).cc