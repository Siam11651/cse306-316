import sys

fileIn = open(sys.argv[1], 'r')
fileOut = open(sys.argv[2], 'w')
lines = []

for line in fileIn:
    lines.append(line.strip())

size = len(lines)

for i in range(0, size):
    line  = lines[i]

    fileOut.write('0x' + line)

    if i < size - 1:
        fileOut.write(',\n')
    else:
        fileOut.write('\n')

fileIn.close()
fileOut.close()