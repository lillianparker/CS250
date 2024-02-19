
.data
newline: .asciiz "\n"
errortext: .asciiz "N must be greater than 0:\n"
prompt:    .asciiz "Enter positive int N:"

.text                         
.globl main 
#NEED to allocate memory on the stack so it isn't forgotten

main:
    addi $sp, $sp, -4   #stack grows down so saves space for 1 word
    sw $ra, 0($sp)      #stores return address

    li $v0, 4
    la $a0, prompt  #prints prompt for user
    syscall

    li $v0, 5       #ready to read for integer input from user
    syscall
    
    move $t0, $v0   #store N in $t0

    blt $v0, $0, error #branch if N <= 0
    beqz $t0, ret_zero #branch if N = 0

    move $a0, $t0   #moves N to $a0
    jal function    #calls function on f(N)
    j continue

ret_zero:
    li $v0, 2
continue:
    move $a0, $v0   #moves final result to $a1 register
    li $v0, 1
    syscall

    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

function:
    addi $sp, $sp, -8   #stack grows down so saves space for 1 word
    sw $ra, 4($sp)      #stores return address
    sw $a0, 0($sp)      #stores argument n

    #goes to base case if argument is 0
    beq $a0, $0, base_case 

    #f(N) = 3 * (N - 1) + f(N - 1) + 1
    addi $a0, $a0, -1    # N-1
    jal function        #calls function (recursive call)
    addi $v0, $v0, 1    #add one to v0
    li $v1, 3           #loads constant 3 into $v1 register 
    mul $t7, $v1, $a0   #will calc value of 3 * (N-1)
    add $v0, $t7, $v0   #sum 

 # have something that takes base case and runs the condition right before the base case
unpackstack:
    lw $ra, 4($sp) 
    lw $a0, 0($sp)
    addi $sp, $sp, 8
    jr $ra
base_case:
    li $v0, 2          #load immediate because f(0) = 2
    j  unpackstack
error:
    li $v0, 4
    la $a0, errortext
    syscall
    j main

.end main