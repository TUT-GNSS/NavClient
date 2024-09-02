all: navClient

navClient: navClient.cpp NavClient.cpp DeviceSerial.cpp DeviceSocket.cpp DeviceType.hpp BufferProcess.cpp ImuBufferProcess.cpp GnssBufferProcess.cpp  ImuDataProcess.cpp  GnssDataProcess.cpp 
	g++ navClient.cpp NavClient.cpp DeviceSerial.cpp DeviceSocket.cpp DeviceType.hpp BufferProcess.cpp ImuBufferProcess.cpp GnssBufferProcess.cpp  ImuDataProcess.cpp  GnssDataProcess.cpp -o navClient -I /usr/local/include/ -L /usr/local/lib/ -lserial

clean:
	rm navClient