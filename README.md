# mqtt-bridge
## Purpose
An Arduino Uno with ethernet shield, to receive sensor inputs, pack to json and send using mqtt to a broker.

Secrets are in secret.h

Using [arduino-mqtt](https://github.com/256dpi/arduino-mqtt)

## Testing
### Publish
mosquitto_pub -h test.mosquitto.org -p 1883 -t statsnail/steinvikveien/ras -m "message" -d

Add -u username -P password if necessary

### Subscribe
mosquitto_sub -h test.mosquitto.org -p 1883 -t statsnail/steinvikveien/ras

Add -u username -P password if necessary

### Mosquitto broker local
brew install mosquitto
/usr/local/sbin/mosquitto -c mosquitto.conf

### mosquitto.conf
listener 10010
allow_anonymous true

## References
[w5100 bug](https://arduinodiy.wordpress.com/2017/04/12/the-w5100-bug/)
[arduino-mqtt](https://github.com/256dpi/arduino-mqtt)
[arduinojson + pubsubclient](https://arduinojson.org/v6/how-to/use-arduinojson-with-pubsubclient/)