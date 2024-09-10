#!/bin/bash

# Shell script to unload the kernel module, remove the device, compile,
# reload the module, initialize the device and open the app and the listener script
sudo rmmod pad
sudo rm /dev/virtual_keypad
make 
sudo insmod pad.ko
sudo mknod /dev/virtual_keypad c 237 0 
sudo ./keypadlistener.sh yatharth &
sudo ./gtapp &