# To compile
sudo apt-get install libwebsockets-dev
sudo apt-get install libconfig-dev
sudo apt-get install libcjson-dev
sudo apt-get install libncurses-dev
gcc proxy_mmt_gui.c -o proxy_mmt_gui -lncurses -lzmq -lwebsockets -lconfig -lcjson

# Config before use
Please make sure to edit proxy_nickname, proxy_long, proxy_lat and proxy_alt fields in config.cfg before use.

# To run
./proxy_mmt_gui
