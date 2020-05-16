## T&W 4G/5G moudle NDIS qmi-wwan use sample

Step by step bring up USB NDIS net card with qmi-wwan.

Clone qmi-wwan
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
### use qmi-wwan as driver
make `qmi-wwan`

```
cd TW_5G_open_linux/qmi-wwan/driver/
make clean; make
```

Insmod `usbnet` and `cdc-wdm`    ko
```
sudo modprobe usbnet
sudo modprobe cdc-wdm
```

Insmod `qmi_wwan` ko

```
 sudo insmod ./qmi_wwan.ko
```

Let's check whether the device enumerated successful with `ifconfig -a` and `ls /dev/cdc-wdm*`.
```
ifconfig -a

lo        Link encap:Local Loopback
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING  MTU:65536  Metric:1
          RX packets:339579 errors:0 dropped:0 overruns:0 frame:0
          TX packets:339579 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1
          RX bytes:21733630 (21.7 MB)  TX bytes:21733630 (21.7 MB)

wwan0     Link encap:Ethernet  HWaddr 52:e2:46:8f:45:4d
          BROADCAST NOARP MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

```

```
ls /dev/cdc-wdm*
/dev/cdc-wdm0
```

Do note that
#### the cdc-wdm0 device  maybe not start of cdc-wdm0 if you have other USB NDIS NET device attached to system bus .

- 

Now, the driver is  just fine, let's begin the Application
----
### use libqmi and qmi-network as application

install dependences
```
sudo apt-get install libqmi-utils
sudo apt-get install udhcpc
```
Darling to Internet with qmi-network:`sudo qmi-network /dev/cdc-wdm0 start`
```
sudo qmi-network /dev/cdc-wdm0 start
Starting network with 'qmicli -d /dev/cdc-wdm0 --wds-start-network=  --client-no-release-cid'...
error: couldn't open the QmiDevice: Cannot open QMI device: Cannot open device file '/dev/cdc-wdm0': Permission denied
error: network start failed, client not allocated
qiaoyk@gwserver-ThinkCentre-M720t-N000:~/qmi_wwan/driver$ sudo qmi-network /dev/cdc-wdm0 start
Starting network with 'qmicli -d /dev/cdc-wdm0 --wds-start-network=  --client-no-release-cid'...
Saving state... (CID: 19)
Saving state... (PDH: 2033441216)
Network started successfully
```
use `sudo udhcpc -i wwan0` to get ip and gateway
```
sudo udhcpc -i wwan0
udhcpc (v1.21.1) started
Sending discover...
Sending select for 10.227.5.130...
Lease of 10.227.5.130 obtained, lease time 7200
/etc/udhcpc/default.script: Resetting default routes
SIOCDELRT: No such process
/etc/udhcpc/default.script: Adding DNS 211.137.130.2
/etc/udhcpc/default.script: Adding DNS 211.137.130.18
```
Tested with `ifconfig` and `ping`
```
lo        Link encap:Local Loopback
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING  MTU:65536  Metric:1
          RX packets:270353 errors:0 dropped:0 overruns:0 frame:0
          TX packets:270353 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1
          RX bytes:17303094 (17.3 MB)  TX bytes:17303094 (17.3 MB)

wwan0     Link encap:Ethernet  HWaddr 52:e2:46:8f:45:4d
          inet addr:10.227.5.130  Bcast:10.227.5.131  Mask:255.255.255.252
          inet6 addr: fe80::50e2:46ff:fe8f:454d/64 Scope:Link
          UP BROADCAST RUNNING NOARP MULTICAST  MTU:1500  Metric:1
          RX packets:3 errors:0 dropped:0 overruns:0 frame:0
          TX packets:28 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:738 (738.0 B)  TX bytes:5511 (5.5 KB)

```
```
ping 114.114.114.114 -I wwan0
PING 114.114.114.114 (114.114.114.114) from 10.227.5.130 wwan0: 56(84) bytes of data.
64 bytes from 114.114.114.114: icmp_seq=1 ttl=94 time=99.6 ms
64 bytes from 114.114.114.114: icmp_seq=2 ttl=87 time=138 ms
64 bytes from 114.114.114.114: icmp_seq=3 ttl=82 time=94.9 ms
64 bytes from 114.114.114.114: icmp_seq=4 ttl=85 time=54.6 ms

--- 114.114.114.114 ping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 3005ms
rtt min/avg/max/mdev = 54.610/97.007/138.824/29.821 ms

```
Now enjoy the 4G/5G facilities

### use uqmi as application

Clone qmi-wwan
```
git clone https://git.openwrt.org/project/uqmi.git
```
install dependences 
```
 libubox which is dependented on lua
 libjason-c 
```
rebuild uqmi

Darling to Internet with qmi:`sudo qmi-network /dev/cdc-wdm0 start`
```
//get a wds clientID
#./uqmi -d /dev/cdc-wdm0 --get-client-id wds
# 1
/dailing with this clientID
# uqmi -d /dev/cdc-wdm0 --set-client-id wds,1 --start-network ctlte autoconnect
//get IP
# udhcpc -f -n -q -t 5 -i wwan0
//disconnect from Internet
# ./uqmi -d /dev/cdc-wdm0 --set-client-id wds,1 --stop-network 0xffffffff –autoconnect
```

