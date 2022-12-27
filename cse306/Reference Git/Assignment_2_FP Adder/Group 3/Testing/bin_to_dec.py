-------------------------generating test cases-------------------------------------
number = input("Floating Point Representation: ")

sign_in = number[0]
exp_in = number[1:5]
fraction_in = number[5:]

exp = 0
for i, b in enumerate(exp_in[::-1]):
	exp += 2**i * int(b)
exp -= 7

power = -1
fraction = 0
for f in fraction_in:
	fraction += 2**power * int(f)
	power -= 1

answer = (fraction+1)* 2**exp

if sign_in == '1':
    answer *= -1
 
print("Decimal Value:", answer)
