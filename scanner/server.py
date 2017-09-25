from serial import Serial, SerialException
import time
import numpy as np

cxn = Serial('/dev/ttyACM0', baudrate=100000)


cmd_id = input("Press enter to start scanning...")
print(cmd_id)
cxn.write('s'.encode())
while cxn.inWaiting() < 1:
    pass

result = ""
while(result != "*** Starting ***"):
    result = cxn.readline().decode().strip()
    print(result)
    cxn.write('s'.encode())

parameters = cxn.readline().decode().strip().split(" ")
print(parameters)
x_values = [int(parameters[0]), int(parameters[2])]
y_values = [int(parameters[1]), int(parameters[3])]
print("x:", x_values)
print("y:", y_values)

data = np.zeros([x_values[1]-x_values[0]+1, y_values[1]-y_values[0]+1], np.int32)
print("Dimensions:", data.shape)

while(True):
    new_line = cxn.readline().decode().strip()
    if new_line == "Complete!":
        break
    result = [int(x) for x in new_line.split(" ")]
    print(result)
    print("Setting ", result[0]-x_values[0], result[1]-y_values[0], "to", result[2])
    data[result[0]-x_values[0], result[1]-y_values[0]] = result[2]


print(data)

np.savetxt("scan"+ str(int(time.time())) +".csv", data, delimiter=",")
print('done')
