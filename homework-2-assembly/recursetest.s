
.text
.globl main 
main:
    addi $sp, $sp, -4   #stack grows down so saves space for 1 word
    sw $ra, 0($sp)      #stores return address

    li $v0, 4
    la $a0, prompt  #prints prompt for user
    syscall

    li $v0, 5       #ready to read for integer input from user
    syscall

    beqz $v0, _iszero
    j continue

_iszero:
    li $v0, $0
continue:
    move $a0, $v0   #moves final result to $a1 register
    syscall

    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

