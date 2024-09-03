# coding: utf-8
# last modified:20220824
import time
import serial
import re
import socket
import struct
import sys
from time import time


utctime = ''
lat = ''
ulat = ''
lon = ''
ulon = ''
numSv = ''
msl = ''
cogt = ''
cogm = ''
sog = ''
kph = ''
gps_t = 0


usb_port = '/dev/ttyUSB1' # USB serial port linux
baud = 115200   # Same baud rate as the INERTIAL navigation module
ser = serial.Serial(usb_port, baud)

if ser.isOpen():
    print("GPS Serial Opened! Baudrate=",baud)
else:
    print("GPS Serial Open Failed!")


def Convert_to_degrees(in_data1, in_data2):
    len_data1 = len(in_data1)
    str_data2 = "%05d" % int(in_data2)
    temp_data = int(in_data1)
    symbol = 1
    if temp_data < 0:
        symbol = -1
    degree = int(temp_data / 100.0)
    str_decimal = str(in_data1[len_data1-2]) + str(in_data1[len_data1-1]) + str(str_data2)
    f_degree = int(str_decimal)/60.0/100000.0
    # print("f_degree:", f_degree)
    if symbol > 0:
        result = degree + f_degree
    else:
        result = degree - f_degree
    return result


def GPS_read():
        global utctime
        global lat
        global ulat
        global lon
        global ulon
        global numSv
        global msl
        global cogt
        global cogm
        global sog
        global kph
        global gps_t
        if ser.inWaiting():
            if ser.read(1) == b'G':
                if ser.inWaiting():
                    if ser.read(1) == b'N':
                        if ser.inWaiting():
                            choice = ser.read(1)
                            if choice == b'G':
                                if ser.inWaiting():
                                    if ser.read(1) == b'G':
                                        if ser.inWaiting():
                                            if ser.read(1) == b'A':
                                                #utctime = ser.read(7)
                                                GGA = ser.read(70)
                                                GGA_g = re.findall(r"\w+(?=,)|(?<=,)\w+", str(GGA))
                                                # print(GGA_g)
                                                if len(GGA_g) < 13:
                                                    print("GPS no found")
                                                    gps_t = 0
                                                    return 0
                                                else:
                                                    utctime = GGA_g[0]
                                                    # lat = GGA_g[2][0]+GGA_g[2][1]+'°'+GGA_g[2][2]+GGA_g[2][3]+'.'+GGA_g[3]+'\''
                                                    lat = "%.8f" % Convert_to_degrees(str(GGA_g[2]), str(GGA_g[3]))
                                                    ulat = GGA_g[4]
                                                    # lon = GGA_g[5][0]+GGA_g[5][1]+GGA_g[5][2]+'°'+GGA_g[5][3]+GGA_g[5][4]+'.'+GGA_g[6]+'\''
                                                    lon = "%.8f" % Convert_to_degrees(str(GGA_g[5]), str(GGA_g[6]))
                                                    ulon = GGA_g[7]
                                                    numSv = GGA_g[9]
                                                    msl = GGA_g[12]+'.'+GGA_g[13]+GGA_g[14]
                                                    #print(GGA_g)
                                                    gps_t = 1
                                                    return 1
                            elif choice == b'V':
                                if ser.inWaiting():
                                    if ser.read(1) == b'T':
                                        if ser.inWaiting():
                                            if ser.read(1) == b'G':
                                                if gps_t == 1:
                                                    VTG = ser.read(40)
                                                    VTG_g = re.findall(r"\w+(?=,)|(?<=,)\w+", str(VTG))
                                                    cogt = VTG_g[0]+'.'+VTG_g[1]+'T'
                                                    if VTG_g[3] == 'M':
                                                        cogm = '0.00'
                                                        sog = VTG_g[4]+'.'+VTG_g[5]
                                                        kph = VTG_g[7]+'.'+VTG_g[8]
                                                    elif VTG_g[3] != 'M':
                                                        cogm = VTG_g[3]+'.'+VTG_g[4]
                                                        sog = VTG_g[6]+'.'+VTG_g[7]
                                                        kph = VTG_g[9]+'.'+VTG_g[10]
                                                #print(kph)




def create_socket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    return sock

def tcpsend(sock, data):

    #发送数据格式为4字节的报文长度+报文内容

    # 计算数据的长度，并使用struct打包为4字节的整数
    length_prefix = struct.pack('I', len(data))
    
    # 将长度前缀和实际数据内容组合后发送
    sock.sendall(length_prefix + data)

# def tcprecv(sock):
#     # 首先接收4字节的消息长度信息
#     len_data = sock.recv(4)
#     if len(len_data) < 4:
#         return None
#     length = struct.unpack('I', len_data)[0]
#     # 根据长度接收剩余的数据
#     return sock.recv(length)

def connect_to_server(sock, ip, port):
    servaddr = (ip, int(port))
    sock.connect(servaddr)
    print("连接成功。")

def login(sock, device_id):
    login_msg = f"<bizcode>00101</bizcode><deviceid>{device_id}</deviceid>".encode()
    tcpsend(sock, login_msg)

def repeated_sending(sock,data):
    # print(data,"\n")
    gnss_data = f"<bizcode>00201</bizcode><gnssdata>{data}</gnssdata>".encode()
    tcpsend(sock, gnss_data)



try:
    ip="192.168.0.101"
    sock_port=5005
    device_id=2

    sock = create_socket()
    connect_to_server(sock, ip, sock_port)
    login(sock, device_id)

    while True:

        if GPS_read():
            # print("*********************")
            # print('UTC Time:'+utctime)
            # print('Latitude:'+lat+ulat)
            # print('Longitude:'+lon+ulon)
            # print('Number of satellites:'+numSv)
            # print('Altitude:'+msl)
            # print('True north heading:'+cogt+'°')
            # print('Magnetic north heading:'+cogm+'°')
            # print('Ground speed:'+sog+'Kn')
            # print('Ground speed:'+kph+'Km/h')
            # print("*********************")
            data=f"{utctime}\t{lat+ulat}\t{lon+ulon}\t{numSv}\t{msl}\t{cogt+'°'}\t{cogm+'°'}\t{sog+'Kn'}\t{kph+'Km/h'}"
            # latitude = lat+ulat
            # latitude1=latitude[:-1]
            # lonitude = lon+ulon
            # lonitude1=lonitude[:-1]
            # data=f"{utctime}\t{latitude1}\t{lonitude1}"
            repeated_sending(sock,data)
        # else:
        #     print("wuxinhao")
except KeyboardInterrupt:
    ser.close()
    print("GPS serial Close!")
