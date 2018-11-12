# ESP8266_AbusTerxonSX

To integrate an Abus Terxon SX Alarmsystem in your SmartHome.

## Hardware


## Apple HomeKit

To control the system with your iPhone you need a running HomeBridge system.
I use this plugin: (https://github.com/codetwice/homebridge-http-securitysystem#readme)

### Configuratioin

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
            "mappers": [
                {
                    "type": "xpath",
                    "parameters": {
                        "xpath": "//partition[3]/text()"
                    }
                },
                {
                    "type": "regex",
                    "parameters": {
                        "regexp": "^The system is currently (ARMED|DISARMED), yo!$",
                        "capture": "1"
                    }
                },
                {
                    "type": "static",
                    "parameters": {
                        "mapping": {
                            "ARMED": "0",
                            "DISARMED": "3"
                        }
                    }
                }
            ]
        }
```