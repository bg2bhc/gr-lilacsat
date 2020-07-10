# Install gnu radio 
**if** you Located in China we recommend you follow this page to install gnu radio  
http://www.hackrf.net/2016/06/pybombs-mirror-tuna/  

----

**if you are not in China** you can follow this page to install  
https://wiki.gnuradio.org/index.php/InstallingGRFromSource  
**NOTICE** we recommend you use **Using the build-gnuradio script** to build up gnu radio

## make sure your system works!
if you installed gnu radio you can follow those step to check your system:
- run gnu radio  
```$ gnuradio-companion```   
  after this command you will see a GUI of gnuradio.  
![grc](https://raw.githubusercontent.com/ArtisticZhao/DSLWP-Downlink-Proxy/master/guide_images/00.png)
- connect your device(like USRP) to your computer.
- run the script  
open the script from "grc_dslwp_new/frontend_dslwp_rx_uhd.grc"
and click run button. if the window show up, it works!!  
![grc is working](https://raw.githubusercontent.com/ArtisticZhao/DSLWP-Downlink-Proxy/master/guide_images/01.png)

# Downlink Proxy
----------------------------------------------------------------------------------
Proxy to send decoded telemetry from DSLWP for realtime display.

Use this proxy with on-air receiving only.

, lilacsat_proxy.desktop may also be useful.

## To setup
----------------------------------------------------------------------------------
$ sudo chmod 777 setup.sh
$ sudo ./setup.sh

## Install env
----------------------------------------------------------------------------------
make sure you have pip(for python2.7)
```
$ pip -V
```
if it return:  
**pip 8.1.1 from /usr/lib/python2.7/dist-packages (python 2.7)**  
pip works!

to install pip for python2.7  
`$ sudo apt install python-pip`

to install python environment  
```
$ sudo python -m pip install tornado
$ sudo python -m pip install requests
$ sudo apt install python-dbus
$ sudo apt install python-qt4
```

$ chmod a+x launch.sh
 
## To run
----------------------------------------------------------------------------------
change dir to proxy path and run  
`$ ./launch.sh`  
or  
`python ./mun_downlink_proxy.py`


## To configure proxy
----------------------------------------------------------------------------------
Usually, the proxy is configured.   
You only to change your **nickname**, **longitude**, **latitude**, **altitude**(altitude is not necessary)  
![configure proxy](https://raw.githubusercontent.com/ArtisticZhao/DSLWP-Downlink-Proxy/master/guide_images/02.png)

if you send the date to server sucessful. server will return some text.  
![proxy send](https://raw.githubusercontent.com/ArtisticZhao/DSLWP-Downlink-Proxy/master/guide_images/03.png)


## Tornado Issue
----------------------------------------------------------------------------------
The proxy does not work with new versions of tornado. To fix this:
$ sudo python -m pip uninstall tornado
$ sudo python -m pip install tornado==4.5.3

