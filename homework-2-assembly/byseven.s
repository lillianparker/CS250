
.text                           # Code segment
.align 2                        # ensure data segment items align to 2^2 = 4 bytes
.globl main 

main:
    #NEED to allocate memory on the stack so it isn't forgotten
    addi $sp, $sp, -12  #stack grows down so saves space for 3 words (4 bytes each),
    sw $ra, 0($sp)      #saves return address on the stack
    sw $s0, 4($sp)      #saves $s0 which so it's value isn't lost during a function call
    sw $s1, 8($sp)      #same as above with $s1
    
    li $s1, 0           #this initializes counter (0) which will be used in the loop

    la $a0, prompt      #this loads in the address of the string prompt before syscall
    syscall 
    la $a0, newline     #print new line
    syscall

    li $v0, 5           #initialize $v0
    syscall             #puts user input into $v0
    move $s0, $v0       #moves user input from $v0 to $s0

_loop:
    #set condition so that if counter > user input the loop ends
    bge $s1, $s0, exit

    li $t0, 7           #put 7 in temporary register
    addi $s2, $s1, 1    #multiplier (ex, will multiply $t0 by $s2 and print)
    mul $s3, $s2, $t0   #first time loop runs = 7*1, second = 7*2, etc.
    
    li $v0, 1
    move $a0, $s3       #this moves the desired number to be printed into $a0
    syscall

    li $v0, 4           #reset $v0 
    la $a0, newline     #print new line
    syscall

    addi $s1, $s1, 1    #increment the counter
    j _loop              #repeat

exit:  
    #exit program, restore registers
    lw $s1, 8($sp)
    lw $s0, 4($sp)
    lw $ra, 0($sp)
    addi $sp, $sp, 12
    jr $ra
    li $v0, 10
    syscall

.end main

.data
prompt: .asciiz "Enter N value:"
newline: .asciiz "\n"

