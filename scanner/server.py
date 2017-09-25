from serial import Serial, SerialException
import numpy as np

cxn = Serial('/dev/ttyACM0', baudrate=9600)




while(True):
    cmd_id = input("Press enter to start scanning...")
    print(cmd_id)
    cxn.write('s'.encode())
    while cxn.inWaiting() < 1:
        pass
    while(True):
        result = cxn.readline().decode().strip()
        print(result)
        if result == "*** Stopped ***":
            break

print('done')
