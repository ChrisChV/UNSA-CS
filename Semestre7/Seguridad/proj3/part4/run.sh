#!/bin/sh

sudo kill $(ps aux |grep 'python2 dnsServer.py' |awk '{print $2}')
sudo python2 dnsServer.py > log/dnsOut.txt &
sudo kill $(ps aux |grep 'python2 httpserver.py' |awk '{print $2}')
sudo python2 httpserver.py >log/httpserverOut.txt &
sudo kill $(ps aux |grep 'python2 client.py' |awk '{print $2}')
#sudo python2 client.py >log/clientOut.txt &
sudo python2 client.py &
sudo python2 attacker.py 
