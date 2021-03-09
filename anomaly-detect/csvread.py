import csv
import pprint
import sys

counter = 0
counter2 = 0
with open('1.csv') as f:
    reader = csv.reader(f)

    for row in reader:
        if counter > 0:
            print(row[1])
        counter = counter + 1

        if counter2 > 999:
            exit()

        counter2 = counter2 + 1
            
