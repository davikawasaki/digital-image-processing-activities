#!/usr/bin/env python
# Usage: ./convert2arff.py algoritmo_bic_imagem_fish

# PUT THE RAW TXT FILE INSIDE EXTRACTED_DATA/ FOLDER AND DON'T RUN THE COMMAND
# WITH THE FILENAME EXTENSION

import sys

filename = sys.argv[1]
inputFileName = "extracted_data/" + filename + ".txt"

inputFile = open(inputFileName, "r")
inputFileData = inputFile.read()
inputFile.close()

outputFile = open("extracted_data/" + filename + ".arff", "w")
outputFile.write("@relation " + filename + "\n\n")

for count, row in enumerate(inputFileData.split("\n"), 0):
    row = row[:-1]
    if(not count):
        for x in row.split(','):
            outputFile.write("@attribute " + x + " real\n")
        outputFile.write("\n")
    else:
        outputFile.write("@data\n")
        outputFile.write(row)
outputFile.close()