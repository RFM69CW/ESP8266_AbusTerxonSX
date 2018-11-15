# ESP8266_AbusTerxonSX

To integrate an Abus Terxon SX Alarmsystem in your SmartHome.

## Features
* See actual status of the alarm system.
* *rm and disarm the alarm system via browser or Apple Homekit (OpenHab etc. is also possible).
* Push message when alarm was triggered (Homekit).
* *Limitation:* It is possible to disable a triggered alarm but to arm the system again it is necessary to reset it with the control panel.

## Hardware
### Controller

NodeMCU V3.2 Arduino ESP8266 ESP-12 E 

### Alarm system

Abus Terxon SX

### Additional Hardware
* 12 V relay
* Relay board with optocoppler (relay board arduino)
* 10 k resistor

### Installation

| NodeMCU pin   | Terxon        | Relais Board  | Function |
| ------------- |-------------  |---------------| -------- |
| VU (5V)       |               | JD VCC        | 5 V for relais coil |
| D1            | AlarmStatusPin|               | Status armed/disarmed |
| D2            |               | Relais in     | Action armed/disarmed |
| D6            | AlarmActivePin|               | Status alarm triggered |

![Schematic could not be load](img/esp8266_abusterxonsx_schematic_v3.jpg?raw=true "Schematic")

## Apple HomeKit

To control the system with your iPhone you need a running HomeBridge system.
I use this plugin: (https://github.com/codetwice/homebridge-http-securitysystem#readme)

### Configuration

```
{
            "accessory": "Http-SecuritySystem",
            "name": "Home security",
            "debug": false,
            "username": "terxonAlarm",
            "password": "esp8266",
            "immediately": false,
            "polling": true,
            "pollInterval": 30000,
            "http_method": "GET",
            "urls": {
                "stay": {
                    "url": "http://xxx.xxx.xxx.xxx/alarm/arm",
                    "body": "stay"
                },
                "away": {
                    "url": "http://xxx.xxx.xxx.xxx/alarm/arm",
                    "body": "away"
                },
                "night": {
                    "url": "http://xxx.xxx.xxx.xxx/alarm/arm",
                    "body": "night"
                },
                "disarm": {
                    "url": "http://xxx.xxx.xxx.xxx/alarm/disarm",
                    "body": ""
                },
                "readCurrentState": {
                    "url": "http://xxx.xxx.xxx.xxx/alarm/check",
                    "body": ""
                },
                "readTargetState": {
                    "url": "http://xxx.xxx.xxx.xxx/alarm/check",
                    "body": "",
                    "headers": {
                        "Content-Type": "application/json"
                    }
                }
            },
            "mappers": []
        }
```

## Keywords

Smart Home, ABUS Terxon SX, Arduino, ESP8266, Apple Homekit, Alarm system
