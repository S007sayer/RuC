DEFARR1:
    sw $a1, 4($a0)
    li $v0, 4
    mul $v0, $v0, $a1
    sub $v0, $a0, $v0
    addi $v0, $v0, -4
    jr $ra
DEFARR2:
    sw $a0, 0($a2)
    move $t0, $ra
    jal DEFARR1
    move $ra, $t0
    addi $a2, $a2, -4
    addi $a0, $v0, -4
    addi $a3, $a3, -1
    bne $a3, $0, DEFARR2
    jr $ra
