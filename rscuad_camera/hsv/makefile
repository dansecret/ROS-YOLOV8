
INC = -I ../gateway
SRC = hsv.cpp ../gateway/gateway.cpp running.cpp

all:
	g++ -C $(INC) $(SRC) `pkg-config --cflags --libs opencv` -pthread -o run -std=c++11
cv4:
	g++ -C $(INC) $(SRC) `pkg-config --cflags --libs opencv4` -pthread -o run -std=c++11

check:
	valgrind --leak-check=full --show-leak-kinds=all --log-file=memoryleak.rpt ./run
	
clean:
	rm run
