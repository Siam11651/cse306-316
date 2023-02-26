label1:
addi $t0, $zero, 4
addi $t1, $zero, 5
j label1
addi $t1, $zero, 4
beq $t0, $t1, label1