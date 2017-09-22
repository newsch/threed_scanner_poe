# Installing platformIO:
```
sudo pip install -U platformio
wget https://raw.githubusercontent.com/platformio/platformio-core/develop/scripts/99-platformio-udev.rules

sudo cp 99-platformio-udev.rules /etc/udev/rules.d/99-platformio-udev.rules
sudo service udev restart
```

# Find boards:
```
platformio boards
```
Arduino uno is ```uno```

## Init a project in a directory:
```
cd /path/to/project
platformio init --board uno
```

## Add code:
Add code in /src/main.cpp

## Upload:
```
platformio run --target upload
```

## Monitor Serial:
```
platformio device monitor
```

## Upload and monitor:
```
platformio run --target upload ;and platformio device monitor
```
Note the ```;and``` stops execution on a build/upload error


# Hardware:
## Removing noise
Tried a low-pass filter before the arduino, at 30hz it was effective.
I need to try a by-pass cap between Vin and GND of the IR sensor (10uF or more recommended)

# Serial
