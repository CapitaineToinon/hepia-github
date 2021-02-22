import matplotlib.pyplot as plt
import numpy as np
import csv
import os
from cycler import cycler


plt.rcParams['axes.prop_cycle'] = cycler(color='bgrcmyk')

# Grab current directory
directory = os.fsencode(os.path.dirname(os.path.realpath(__file__)))

csvFileCount = 0

# Get the amount of csv file in folder
for file in os.listdir(directory):
    filename = os.fsdecode(file)
    if filename.endswith('.csv'): 
        csvFileCount += 1


# Loop through all csv files in current directory
for file in os.listdir(directory):
    # Convert our filename from bytes to string
    filename = os.fsdecode(file)

    # Grab csv file only
    if filename.endswith('.csv'): 
        intercept = 0.0
        slope = 0.0
        pointsX = []
        pointsY = []
        i = 0

        with open(filename) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=';')
            for row in csv_reader:
                if i == 0:
                    print('Slope is ' + row[0] + ' and intercept is ' + row[1])
                    slope = float(row[0])
                    intercept = float(row[1])
                else:
                    pointsX.append(float(row[0]))
                    pointsY.append(float(row[1]))
                i += 1

                        
        lineX = [0, intercept]
        lineY = [10, 10*slope+intercept]


        plt.style.use('seaborn-whitegrid')

        plotColor = (np.random.uniform(0, 1), np.random.uniform(0, 1), np.random.uniform(0, 1))
        plt.plot(pointsX, pointsY, '.', markersize="2", color=plotColor)
        plt.axline((0,intercept), slope=slope, linewidth=1, linestyle="--", color=plotColor, label=filename + " " + "(slope=" + str(slope)  + " intercept=" + str(intercept) + ")")

        plt.title("Linear regression (concatenation of " + str(csvFileCount) + " files)")
        plt.ylabel("y axis")
        plt.xlabel("x axis")
        plt.legend()
        continue
    else:
        continue


plt.show()
