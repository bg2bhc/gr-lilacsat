#!/bin/sh
echo "proxy starting"
work_path=$(dirname $0)
cd ${work_path}
work_path=$(pwd)
python2 ${work_path}"/mun_downlink_proxy.py" 
