#!/bin/bash

major_nr=$(grep mydev /proc/devices | awk '{print $1}')
sudo mknod /dev/mydev c $major_nr 0
sudo chmod 666 /dev/mydev