#!/bin/bash

if [ $UID -ne 0 ]; then
    echo "Superuser privileges are required to run this script."
    echo "e.g. \"sudo $0\""
    exit 1
fi


basepath=$(cd `dirname $0`; pwd)

echo $basepath > /usr/local/lib/python2.7/dist-packages/grc.pth

echo "[Desktop Entry]
Name=LilacProxy
Exec="$PWD"/mun_downlink_proxy.py
Icon="$PWD"/icon.xpm
Terminal=false
Type=Application
Name[en_US]=dslwp_proxy.desktop">>$PWD/lilacsat_dslwp.desktop
sudo chmod 777 $PWD/lilacsat_dslwp.desktop



