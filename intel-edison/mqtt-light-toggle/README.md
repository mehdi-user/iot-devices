# MQTT Light Toggle

This code toggles device's light whenever an mqtt message arrives

Compile the code
```
gcc mqtt_light_toggle.c -lmraa -lmosquitto
```
and run it.

If Mosquitto not installed on the device, follow:

https://tekytech.wordpress.com/topics/mqtt-on-intel-edison/

or

https://software.intel.com/en-us/blogs/2015/02/20/building-and-running-mosquitto-mqtt-on-intel-edison