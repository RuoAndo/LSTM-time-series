import csv
import pprint
import sys

counter = 0
counter2 = 0
with open(sys.argv[1]) as f:
    reader = csv.reader(f)

    for row in reader:
        if counter > 0:
            print(row[1])
        counter = counter + 1

        if counter2 > int(sys.argv[2])+1:
            exit()

        counter2 = counter2 + 1
            
