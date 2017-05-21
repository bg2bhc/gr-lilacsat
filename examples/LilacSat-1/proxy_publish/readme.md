# LilacSat Downlink Proxy
----------------------------------------------------------------------------------
Proxy to send decoded telemetry from LilacSat-1 and LilacSat-2 for realtime display.

Use this proxy with on-air receiving only.

icon.xpm, lilacsat_proxy.desktop may also be useful.

## To setup
----------------------------------------------------------------------------------
$ sudo chmod 777 setup.sh
$ sudo ./setup.sh
 
## To run
----------------------------------------------------------------------------------
$ python lilacsat_proxy.py

## For LilacSat-1
----------------------------------------------------------------------------------
Use 61261 for Server Port.

Use http://lilacsat.hit.edu.cn/tle/lilacsat1.txt for TLE URL.

View http://lilacsat.hit.edu.cn/lilac1_en/Dashboard.html for realtime telemetry.

## For LilacSat-2
----------------------------------------------------------------------------------
Use 61161 for Server Port.

Use http://lilacsat.hit.edu.cn/tle/lilacsat.txt for TLE URL.

View http://lilacsat.hit.edu.cn/lilac_en/Dashboard.html for realtime telemetry.



