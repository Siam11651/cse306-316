file = open("truth_table.txt", "r")
log = open("error.log", "w")

file_list = file.readlines()[1:]

for ind in range(0, len(file_list)):
    file_list[ind] = "".join(file_list[ind].split())

wrongCount = 0
lineCount = 0
# A0A1A2A3B0B1B2B3S0S1S2FlagVFlagCFlagZFlagSF0F1F2F3C_out
for line in file_list:
    lineCount += 1

    # because frigging logisim does truth table in lsb first format 🤲
    op = line[8:11]
    op = op[::-1]
    A = line[:4]
    A = A[::-1]
    B = line[4:8]
    B = B[::-1]
    Output = line[15:19]
    Output = Output[::-1]


    if (op == '000'):
        # OR

        if (int(Output, 2) != (int(A, 2)) | (int(B, 2))):
            wrongCount += 1
            log.write(f"{lineCount = }, "   +A + " OR " + B + " equalling " + Output +
                      ", should be " + str(bin((int(A, 2)) | (int(B, 2)))) + "\n")

    elif (op == '001'):
        # Transfer

        if (Output != A):
            wrongCount += 1
            log.write(f"{lineCount = }, "   +"Transfer " + A + " equalling " + Output +
                      ", should be: " + A + "\n")

    elif (op == '010' or op == '011'):
        # Complement
        
        correctAns = ~(int(A, 2))
        correctAns = format(correctAns + 2**32, 'b').zfill(4)[-4:]

        if (Output != correctAns):
            wrongCount += 1
            log.write(f"{lineCount = }, "   +"Complement " + A + " equalling " + Output +
                      ", should be: " + correctAns + "\n")

    elif (op == '100'):
        # A + B
        
        correctAns = (((int(A, 2)) + (int(B, 2))))

        correctAns = format(correctAns + 2**32, 'b').zfill(4)[-4:]

        if (Output != correctAns):
            wrongCount += 1
            log.write(f"{lineCount = }, "   +A + " + " + B + " equalling " + Output +
                      ", should be: " + correctAns + "\n")

    elif(op == '101'):
        # Increment
        
        correctAns = (((int(A, 2)) + (1)))

        correctAns = format(correctAns + 2**32, 'b').zfill(4)[-4:]

        if (Output != correctAns):
            wrongCount += 1
            log.write(f"{lineCount = }, " +A + " + 1 equalling " + Output +
                      ", should be: " + correctAns + "\n")

    elif (op == '110' or op == '111'):
        # A - B - 1
        
        correctAns = (((int(A, 2)) - (int(B, 2)) - 1))
        correctAns = format(correctAns + 2**32, 'b').zfill(4)[-4:]

        if (Output != correctAns):
            wrongCount += 1
            log.write(f"{lineCount = }, "   +A + " - " + B + " - 1 equalling " + Output +
                      ", should be: " + correctAns + "\n")

print(f"{wrongCount=}")
