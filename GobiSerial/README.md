### T&W 4G/5G moudle GobiSerial use sample

Step by step bring up USB serial with GobiSerial.

Clone GobiSerial
```
git clone https://github.com/TWxa001/TW_5G_open_linux.git
```
check T&W USB device attached with `lsusb`
```
~/GobiSerial$ lsusb
Bus 002 Device 002: ID 2077:2004 Taicang T&W Electronics Co. Ltd
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
```

make `GobiSerial`

```
cd TW_5G_open_linux/GobiSerial/driver/
make clean; make
```

Insmod `usbserial`  ko
```
sudo modprobe usbserial
```

Insmod `GobiSerial` ko

```
 sudo insmod ./GobiSerial.ko
```

Let's check whether the device enumerated successful.
```
ls /dev/ttyU*
/dev/ttyUSB0  /dev/ttyUSB1  /dev/ttyUSB2  /dev/ttyUSB3
```
usb serial device index and their function
|device name    | usb function  | interface id  |    vid:pid    |
| ------------- | ------------- | :------------ | ------------- |
|     ttyUSB0   |  Diagnosetic  |MI_00|2077:2004|
| ttyUSB1 | Modem |MI_01|2077:2004|
| ttyUSB2 | Application |MI_02|2077:2004|
| ttyUSB3 | Nmea |MI_03|2077:2004|

Do note that :
#### the ttyUSB devce  maybe not start of ttyUSB0 if you have other USB serial device attached to system bus .

Now, it should run just fine!
```
sudo minicom -s

Welcome to minicom 2.7

OPTIONS: I18n
Compiled on Nov 15 2018, 20:20:04.
Port /dev/ttyUSB1, 11:12:55

Press CTRL-A Z for help on special keys

at
OK
at+cops?
+cops: 0,0,"CHINA MOBILE",7

OK
```