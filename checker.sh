g++ -g main.cpp -I. -std=c++17 -O3 -o Cb

#valgrind --leak-check=full --track-origins=yes -v ./Cb

#g++ -fsanitize=address -g main.cpp


#./Cb


