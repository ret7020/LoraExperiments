mobile:
	arduino-cli compile ./MobileStationRX/MobileStationRX.ino --fqbn esp32:esp32:nodemcu-32s --port $(PORT) --upload --verbose

base:
	arduino-cli compile ./BaseStationTX/BaseStationTX.ino --fqbn esp8266:esp8266:d1_mini --port $(PORT) --upload --verbose

arduinoBaseStation:
	arduino-cli compile ./ArduinoBaseStation/ArduinoBaseStation.ino --fqbn arduino:avr:nano:cpu=atmega328old --port $(PORT) --upload --verbose