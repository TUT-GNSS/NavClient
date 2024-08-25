all: navClient

navClient: navClient.cpp ImuDataProcess.cpp
	g++ navClient.cpp ImuDataProcess.cpp -o navClient -I /usr/local/include/ -L /usr/local/lib/ -lserial

clean:
	rm navClient