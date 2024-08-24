# coding:UTF-8
# Version: V1.5.1
import serial
import socket
import struct
import sys
import re
from time import time
import requests
import json



#设置GNSS接收机的USB设备接口
usb_gnss_port = '/dev/ttyUSB1' # USB serial port linux
#设置GNSS接收机的波特率
gnss_baud = 9600   # Same baud rate as the INERTIAL navigation module
ser_gnss = serial.Serial(usb_gnss_port, gnss_baud)

if ser_gnss.isOpen():
    print("GPS Serial Opened! Baudrate={gnss_baud}")
else:
    print("GPS Serial Open Failed!")


#初始化GNSS存放数据的字符串
utctime = ''#UTC时间
lat = ''        #latitude
# ulat = ''  
lon = ''      #longitude
# ulon = ''
numSv = ''#可见卫星数
msl = ''     #海拔高度Altitude，单位米（m）
cogt = ''   #True north heading，单位degree  得到值+'T'+'°‘
cogm = '' #Magnetic north heading，单位degree（'°‘）
sog = ''    #Ground speed，单位Kn
kph = ''    #Ground speed，单位'Km/h'
gps_t = 0

#初始化IMU模块报文长度
buf_length = 11
#初始化IMU模块报文数组
RxBuff = [0]*buf_length

#初始化IMU模块报文数据部分数组
TimeData=[0.0]*8
ACCData = [0.0]*8
GYROData = [0.0]*8
AngleData = [0.0]*8
LonLatData= [0.0]*8
GpsData= [0.0]*8
FrameState = 0  # What is the state of the judgment
CheckSum = 0  # Sum check bit

start = 0 #帧头开始的标志
data_length = 0 #根据协议的文档长度为11 eg:55 51 31 FF 53 02 CD 07 12 0A 1B

#初始化用于存放IMU数据的字符串
time="0"
acc = "0"
gyro = "0"
Angle = "0"

def GetDataDeal(list_buf,sock):
    global time,acc,gyro,Angle,LonLat,Gps

    if(list_buf[buf_length - 1] != CheckSum): #校验码不正确
        return

    if(list_buf[1] == 0x50): #时间输出
        for i in range(8): 
            TimeData[i] = list_buf[2+i] #有效数据赋值
        time = get_time(TimeData)
    elif(list_buf[1] == 0x51): #加速度输出
        for i in range(6): 
            ACCData[i] = list_buf[2+i] #有效数据赋值
        acc = get_acc(ACCData)

    elif(list_buf[1] == 0x52): #角速度输出
        for i in range(6): 
            GYROData[i] = list_buf[2+i] #有效数据赋值
        gyro = get_gyro(GYROData)

    elif(list_buf[1] == 0x53): #姿态角度输出
        for i in range(6): 
            AngleData[i] = list_buf[2+i] #有效数据赋值
        Angle = get_angle(AngleData)
    


def DueData(inputdata,sock):  # New core procedures, read the data partition, each read to the corresponding array 
    global start,time,acc,gyro,Angle,LonLat,Gps
    global CheckSum
    global data_length
    # print(type(inputdata))
    if inputdata == 0x55 and start == 0:
        start = 1
        data_length = 11
        CheckSum = 0
        #清0
        for i in range(11):
            RxBuff[i] = 0

    if start == 1:
        CheckSum += inputdata #校验码计算 会把校验位加上
        RxBuff[buf_length-data_length] = inputdata #保存数据
        data_length = data_length - 1 #长度减一
        if data_length == 0: #接收到完整的数据
            CheckSum = (CheckSum-inputdata) & 0xff 
            start = 0 #清0
            send_imuData(sock,time+acc+gyro+Angle)
        GetDataDeal(RxBuff,sock)  #处理数据

def get_time(datahex):
    yy = datahex[0]
    mm = datahex[1]
    dd = datahex[2]
    hh = datahex[3]
    mn = datahex[4]
    ss = datahex[5]
    msl = datahex[6]
    msh = datahex[7]
    ms=((msh<<8)|msl)
    res=f"{yy}.{mm}.{dd} {hh}:{mn}:{ss}.{ms}\t"
    return res 

def get_acc(datahex):
    axl = datahex[0]
    axh = datahex[1]
    ayl = datahex[2]
    ayh = datahex[3]
    azl = datahex[4]
    azh = datahex[5]
    k_acc = 16.0
    acc_x = (axh << 8 | axl) / 32768.0 * k_acc
    acc_y = (ayh << 8 | ayl) / 32768.0 * k_acc
    acc_z = (azh << 8 | azl) / 32768.0 * k_acc
    if acc_x >= k_acc:
        acc_x -= 2 * k_acc
    if acc_y >= k_acc:
        acc_y -= 2 * k_acc
    if acc_z >= k_acc:
        acc_z -= 2 * k_acc
    res=f"{acc_x}\t{acc_y}\t{acc_z}\t"
    return res 


def get_gyro(datahex):
    wxl = datahex[0]
    wxh = datahex[1]
    wyl = datahex[2]
    wyh = datahex[3]
    wzl = datahex[4]
    wzh = datahex[5]
    k_gyro = 2000.0
    gyro_x = (wxh << 8 | wxl) / 32768.0 * k_gyro
    gyro_y = (wyh << 8 | wyl) / 32768.0 * k_gyro
    gyro_z = (wzh << 8 | wzl) / 32768.0 * k_gyro
    if gyro_x >= k_gyro:
        gyro_x -= 2 * k_gyro
    if gyro_y >= k_gyro:
        gyro_y -= 2 * k_gyro
    if gyro_z >= k_gyro:
        gyro_z -= 2 * k_gyro
    res=f"{gyro_x}\t{gyro_y}\t{gyro_z}\t"
    return res 


def get_angle(datahex):
    rxl = datahex[0]
    rxh = datahex[1]
    ryl = datahex[2]
    ryh = datahex[3]
    rzl = datahex[4]
    rzh = datahex[5]
    k_angle = 180.0
    angle_x = (rxh << 8 | rxl) / 32768.0 * k_angle
    angle_y = (ryh << 8 | ryl) / 32768.0 * k_angle
    angle_z = (rzh << 8 | rzl) / 32768.0 * k_angle
    if angle_x >= k_angle:
        angle_x -= 2 * k_angle
    if angle_y >= k_angle:
        angle_y -= 2 * k_angle
    if angle_z >= k_angle:
        angle_z -= 2 * k_angle
    res=f"{angle_x}\t{angle_y}\t{angle_z}\t"
    return res 

def get_lonlat(datahex):
    lon0 = datahex[0]
    lon1 = datahex[1]
    lon2 = datahex[2]
    lon3 = datahex[3]
    lat0 = datahex[4]
    lat1 = datahex[5]
    lat2 = datahex[6]
    lat3 = datahex[7]
    Latitude=(lon3<<24)|(lon2<<16)|(lon1<<8)|lon0
    Longitude=(lat3<<24)|(lat2<<16)|(lat1<<8)|lat0
    res=f"{Latitude}\t{Longitude}\t"
    return res

def get_gps(datahex):
    gpsHeightL = datahex[0]
    gpsHeightH = datahex[1]
    gpsYawL = datahex[2]
    gpsYawH = datahex[3]
    gpsV0 = datahex[4]
    gpsV1 = datahex[5]
    gpsV2 = datahex[6]
    gpsV3 = datahex[7]

    GpsHeight=((gpsHeightH<<8)|gpsHeightL)/10
    GpsYaw=((gpsYawH<<8)|gpsYawL)/100
    GpsV= ((gpsV3<<24)|(gpsV2<<16)|(gpsV1<<8)|gpsV0)/1000
    if GpsYaw >= 360:
        GpsYaw -= 360
    
    res=f"{GpsHeight}\t{GpsYaw}\t{GpsV}\t"
    return res 


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
        if ser_gnss.inWaiting():
            if ser_gnss.read(1) == b'G':
                if ser_gnss.inWaiting():
                    if ser_gnss.read(1) == b'N':
                        if ser_gnss.inWaiting():
                            choice = ser_gnss.read(1)
                            if choice == b'G':
                                if ser_gnss.inWaiting():
                                    if ser_gnss.read(1) == b'G':
                                        if ser_gnss.inWaiting():
                                            if ser_gnss.read(1) == b'A':
                                                #utctime = ser_gnss.read(7)
                                                GGA = ser_gnss.read(70)
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
                                                    # ulat = GGA_g[4] #lat单位
                                                    # lon = GGA_g[5][0]+GGA_g[5][1]+GGA_g[5][2]+'°'+GGA_g[5][3]+GGA_g[5][4]+'.'+GGA_g[6]+'\''
                                                    lon = "%.8f" % Convert_to_degrees(str(GGA_g[5]), str(GGA_g[6]))
                                                    # ulon = GGA_g[7] #lon单位
                                                    numSv = GGA_g[9]
                                                    # msl = GGA_g[12]+'.'+GGA_g[13]+GGA_g[14]  #GGA_g[14]为单位m
                                                    msl = GGA_g[12]+'.'+GGA_g[13]
                                                    #print(GGA_g)
                                                    gps_t = 1
                                                    return 1
                            elif choice == b'V':
                                if ser_gnss.inWaiting():
                                    if ser_gnss.read(1) == b'T':
                                        if ser_gnss.inWaiting():
                                            if ser_gnss.read(1) == b'G':
                                                if gps_t == 1:
                                                    VTG = ser_gnss.read(40)
                                                    VTG_g = re.findall(r"\w+(?=,)|(?<=,)\w+", str(VTG))
                                                    # cogt = VTG_g[0]+'.'+VTG_g[1]+'T'
                                                    cogt = VTG_g[0]+'.'+VTG_g[1]
                                                    if VTG_g[3] == 'M':
                                                        cogm = '0.00'
                                                        sog = VTG_g[4]+'.'+VTG_g[5]
                                                        kph = VTG_g[7]+'.'+VTG_g[8]
                                                    elif VTG_g[3] != 'M':
                                                        cogm = VTG_g[3]+'.'+VTG_g[4]
                                                        sog = VTG_g[6]+'.'+VTG_g[7]
                                                        kph = VTG_g[9]+'.'+VTG_g[10]
                                                #print(kph)


#创建socket
def create_socket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    return sock


#向服务端发起socket连接
def connect_to_server(sock, ip, port):
    servaddr = (ip, int(port))
    sock.connect(servaddr)
    print("连接成功。")

#向服务端发起登录请求
def login(sock, device_id):
    login_msg = f"<bizcode>00101</bizcode><deviceid>{device_id}</deviceid>".encode()
    tcpsend(sock, login_msg)

#向服务端发送报文data
def tcpsend(sock, data):

    #发送数据格式为4字节的报文长度+报文内容

    # 计算数据的长度，并使用struct打包为4字节的整数
    length_prefix = struct.pack('I', len(data))

    # 将长度前缀和实际数据内容组合后发送
    sock.sendall(length_prefix + data)


#通过socket发送GNSS数据，数据格式<bizcode>00201</bizcode><gnssdata>{data}</gnssdata>
def send_gnssData(sock,data):
    # print(data,"\n")
    nav_data = f"<bizcode>00201</bizcode><gnssdata>{data}</gnssdata>".encode()
    tcpsend(sock, nav_data)

#通过socket发送IMU数据，数据格式<bizcode>00202</bizcode><imudata>{data}</imudata>
def send_imuData(sock,data):
    # print(data,"\n")
    nav_data = f"<bizcode>00202</bizcode><imudata>{data}</imudata>".encode()
    tcpsend(sock, nav_data)




if __name__ == '__main__':


    usb_imu_port = '/dev/ttyUSB0' # USB serial port linux
    imu_baud = 115200   # Same baud rate as the INERTIAL navigation module
    ser_imu = serial.Serial(usb_imu_port, imu_baud, timeout=0.5)

    if ser_imu.isOpen():
        print("IMU Serial Opened! Baudrate={gnss_baud}")
    else:
        print("IMU Serial Open Failed!")

    ip="192.168.102.33"
    sock_port=5005
    device_id=1


    sock = create_socket()
    connect_to_server(sock, ip, sock_port)
    login(sock, device_id)

    while(1):
        RXdata = ser_imu.read(1)#一个一个读
        RXdata = int(RXdata.hex(),16) #转成16进制显示

        DueData(RXdata,sock)
        if GPS_read():
            
            # gnss_data=f"{utctime}\t{lat+ulat}\t{lon+ulon}\t{numSv}\t{msl}\t{cogt+'°'}\t{cogm+'°'}\t{sog+'Kn'}\t{kph+'Km/h'}"
            #输出数据
            gnss_data=f"{utctime}\t{lat}\t{lon}\t{msl}\t{numSv}\t{cogt}\t{cogm}\t{sog}\t{kph}"
            send_gnssData(sock,gnss_data)


