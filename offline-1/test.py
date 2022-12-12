file = open("truth_table.txt", "r")
log = open("error.log", "w")

file_list = file.readlines()[1:]

for ind in range(0, len(file_list)):
    file_list[ind] = "".join(file_list[ind].split())

wrongCount = 0
lineCount = 0
# 00010203040506070809101112131415   16   17   18
# A3A2A1A0B3B2B1B3S2S1S0F3F2F1F0FlagVFlagZFlagCFlagS
for line in file_list:
    lineCount += 1

    op = line[8:11]
    # op = op[::-1]
    A = line[:4]
    # A = A[::-1]
    B = line[4:8]
    # B = B[::-1]
    Output = line[11:15]
    # Output = Output[::-1]
    FlagZ = line[16]

    if(line[11] != line[18]):
        # sign flag
        wrongCount += 1
        log.write(f"{lineCount = } sign flag mismatch for output :" +
                  Output + ", sign flag is: " + line[18] + "\n")

    if (Output == '0000' and FlagZ != '1'):
        # zero flag
        wrongCount += 1
        log.write(f"{lineCount = }, flag z wrong for " +
                  f"{Output =}" + ", is: " + FlagZ + ", should be 0\n")

    if (op == '000'):
        # OR

        if (int(Output, 2) != (int(A, 2)) | (int(B, 2))):
            wrongCount += 1
            log.write(f"{lineCount = }, " + A + " OR " + B + " equalling " + Output +
                      ", should be " + str(bin((int(A, 2)) | (int(B, 2)))) + "\n")

    elif (op == '001'):
        # Transfer

        if (Output != A):
            wrongCount += 1
            log.write(f"{lineCount = }, " + "Transfer " + A + " equalling " + Output +
                      ", should be: " + A + "\n")

    elif (op == '010' or op == '011'):
        # Complement

        correctAns = ~(int(A, 2))
        correctAns = format(correctAns + 2**32, 'b').zfill(4)[-4:]

        if (Output != correctAns):
            wrongCount += 1
            log.write(f"{lineCount = }, " + "Complement " + A + " equalling " + Output +
                      ", should be: " + correctAns + "\n")

    elif (op == '100'):
        # A + B

        correctAns = (((int(A, 2)) + (int(B, 2))))

        correctAns = format(correctAns + 2**32, 'b').zfill(4)[-4:]

        if (Output != correctAns):
            wrongCount += 1
            log.write(f"{lineCount = }, " + A + " + " + B + " equalling " + Output +
                      ", should be: " + correctAns + "\n")

    elif(op == '101'):
        # Increment

        correctAns = (((int(A, 2)) + (1)))

        correctAns = format(correctAns + 2**32, 'b').zfill(4)[-4:]

        if (Output != correctAns):
            wrongCount += 1
            log.write(f"{lineCount = }, " + A + " + 1 equalling " + Output +
                      ", should be: " + correctAns + "\n")

    elif (op == '110' or op == '111'):
        # A - B - 1

        correctAns = (((int(A, 2)) - (int(B, 2)) - 1))
        correctAns = format(correctAns + 2**32, 'b').zfill(4)[-4:]

        if (Output != correctAns):
            wrongCount += 1
            log.write(f"{lineCount = }, " + A + " - " + B + " - 1 equalling " + Output +
                      ", should be: " + correctAns + "\n")

print(f"{wrongCount=}")
