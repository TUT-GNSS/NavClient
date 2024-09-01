all: navClient

navClient: navClient.cpp NavClient.cpp DeviceSocket.cpp DeviceType.hpp ImuBufferProcess.cpp ImuDataProcess.cpp BufferProcess.cpp GnssDataProcess.cpp GnssBufferProcess.cpp DeviceConn.cpp
	g++ navClient.cpp NavClient.cpp DeviceSocket.cpp DeviceType.hpp ImuBufferProcess.cpp ImuDataProcess.cpp BufferProcess.cpp GnssDataProcess.cpp GnssBufferProcess.cpp DeviceConn.cpp -o navClient -I /usr/local/include/ -L /usr/local/lib/ -lserial

clean:
	rm navClient