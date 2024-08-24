all: navClient

	navClient: navClient.cpp
	g++ navClient.cpp -o navClient -I /usr/local/include/ -L /usr/local/lib/ -lserial

clean:
	rm navClient