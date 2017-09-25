import numpy as np
import matplotlib.pyplot as plt
import sys

def plotArray(in_array):
    # Plot data as heatmap
    plt.clf()
    plt.imshow(np.fliplr(np.flipud(in_array.T)), cmap='gray')
    plt.show()


if len(sys.argv) == 2:
    # Import data from csv
    print("Importing", sys.argv[1])
    scan_data = np.genfromtxt(sys.argv[1], dtype=np.int32, delimiter=",", skip_header=1)
    plotArray(scan_data)

elif len(sys.argv) == 3:
    print("Calibration data:", sys.argv[1])
    print("Scan data:", sys.argv[2])
    calibration_data = np.genfromtxt(sys.argv[1], dtype=np.int32, delimiter=",", skip_header=1)
    scan_data = np.genfromtxt(sys.argv[2], dtype=np.int32, delimiter=",", skip_header=1)

    low_value_indices = scan_data - calibration_data < 20
    scan_data[low_value_indices] = 0


    plotArray(scan_data)
