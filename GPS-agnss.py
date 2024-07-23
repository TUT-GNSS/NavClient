# coding: utf-8
import requests
import json
import socket
import serial
import re
import time

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

ser = serial.Serial("/dev/ttyUSB0", 9600)
if ser.isOpen():
    print("GPS Serial Opened! Baudrate=9600")
else:
    print("GPS Serial Open Failed!")


ak = "填上自己申请的ak, 请看文档 百度地图api申请教程"	
baiduUrl = "http://api.map.baidu.com/location/ip?ak=%s&coor=bd09ll" % (ak)
req = requests.get(baiduUrl)
content = req.text
baiduAddr = json.loads(content)
city = baiduAddr["content"]["address_detail"]["city"]
maplng = baiduAddr["content"]["point"]["x"]
maplat = baiduAddr["content"]["point"]["y"]
#print(city)
#print(maplng)
#print(maplat)

addr = "121.41.40.95"
port = 2621
message = "user=pm@yahboom.com;pwd=yahboom;cmd=full;gnss=gps+bd;lat=%s;lon=%s;" % (maplat,maplng)
socket.setdefaulttimeout(4)
client = socket.socket()
client.connect((addr, port))
client.send(message.encode())
reply_data = ""
print("GPS Agnss start")
if True:
    current_reply = client.recv(1024)
    current_reply = str(current_reply)
    # if len(current_reply) == 0:
    #     break
    # else:
    #     reply_data += current_reply
        #print(reply_data)
ser.write(reply_data)
print("GPS Agnss success")


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
                                                #print(GGA_g)
                                                if len(GGA_g) < 13:
                                                    print("GPS no found")
                                                    print('Latitude:'+maplat)
                                                    print('Longitude:'+maplng)
                                                    gps_t = 0
                                                    return 0
                                                else:
                                                    utctime = GGA_g[0]
                                                    # lat = GGA_g[2][0]+GGA_g[2][1]+'degree'+GGA_g[2][2]+GGA_g[2][3]+'.'+GGA_g[3]+'\''
                                                    lat = "%.8f" % Convert_to_degrees(str(GGA_g[2]), str(GGA_g[3]))
                                                    ulat = GGA_g[4]
                                                    # lon = GGA_g[5][0]+GGA_g[5][1]+GGA_g[5][2]+'degree'+GGA_g[5][3]+GGA_g[5][4]+'.'+GGA_g[6]+'\''
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

try:
    while True:
        if GPS_read():
            print("*********************")
            print('UTC Time:'+utctime)
            print('Latitude:'+lat+ulat)
            print('Longitude:'+lon+ulon)
            print('Number of satellites:'+numSv)
            print('Altitude:'+msl)
            print('True north heading:'+cogt+'degree')
            print('Magnetic north heading:'+cogm+'degree')
            print('Ground speed:'+sog+'Kn')
            print('Ground speed:'+kph+'Km/h')
            print("*********************")
except KeyboardInterrupt:
    ser.close()
    print("GPS serial Close!")


