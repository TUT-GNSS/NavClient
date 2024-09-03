# coding:UTF-8
# Version: V1.5.1
import serial
import socket
import struct
import sys
from time import time

buf_length = 11

RxBuff = [0]*buf_length

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

# time=[0.0]*7
# acc = [0.0]*3
# gyro = [0.0]*3
# Angle = [0.0]*3
# LonLat=[0.0]*2
# Gps=[0.0]*3


time="0"
acc = "0"
gyro = "0"
Angle = "0"
LonLat="0"
Gps="0"

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

    # elif(list_buf[1] == 0x57): #经纬度输出
    #     for i in range(8): 
    #         LonLatData[i] = list_buf[2+i] #有效数据赋值
    #     LonLat = get_lonlat(LonLatData)

    # elif(list_buf[1] == 0x58): #地速输出
    #     for i in range(8): 
    #         GpsData[i] = list_buf[2+i] #有效数据赋值
    #     Gps = get_gps(GpsData)
        

        # print("********************************************************************************************\n")
        # print("time:%d.%d.%d  %d:%d:%d.%d \n" % (time[0],time[1],time[2],time[3],time[4],time[5],time[6]))
        # print("acc:%10.3f %10.3f %10.3f \n" % (acc[0],acc[1],acc[2]))
        # print("gyro:%10.3f %10.3f %10.3f \n" % (gyro[0],gyro[1],gyro[2]))
        # print("angle:%10.3f %10.3f %10.3f \n" % (Angle[0],Angle[1],Angle[2]))
        # print("LonLat:%13d %13d \n" % (LonLat[0],LonLat[1]))
        # print("Gps:%10.3f %10.3f %10.3f \n" % (Gps[0],Gps[1],Gps[2]))
        # print("********************************************************************************************\n\n")

    

    
    

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
            repeated_sending(sock,time+acc+gyro+Angle)
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
    nav_data = f"<bizcode>00201</bizcode><imudata>{data}</imudata>".encode()
    tcpsend(sock, nav_data)


if __name__ == '__main__':

    ip="192.168.0.101"
    sock_port=5005
    device_id=1

    usb_port = '/dev/ttyUSB0' # USB serial port linux

    baud = 115200   # Same baud rate as the INERTIAL navigation module
    ser = serial.Serial(usb_port, baud, timeout=0.5)
    print("Serial is Opened:", ser.is_open)

    sock = create_socket()
    connect_to_server(sock, ip, sock_port)
    login(sock, device_id)

    while(1):
        RXdata = ser.read(1)#一个一个读
        RXdata = int(RXdata.hex(),16) #转成16进制显示
        # repeated_sending(sock,DueData(RXdata))
        DueData(RXdata,sock)
        













