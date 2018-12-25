release :
	g++ -std=c++11 -march=native -O2 main.cpp -o printTable

debug :
	g++ -std=c++11 -march=native -Wall -g -O0 main.cpp -o printTable

.PHONY : clean
clean :
	rm printTable