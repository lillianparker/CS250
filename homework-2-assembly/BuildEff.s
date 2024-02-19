.data
name_prompt: .asciiz "\nBuilding name: "
sqFeet_prompt: .asciiz "\nSquare footage: "
kwh_prompt: .asciiz "\nKwH per year: "

newline: .asciiz "\n"
fileempty: .asciiz "BUILDING FILE IS EMPTY\n"
donestr: .asciiz "DONE\n"
memfault: .asciiz "Memory allocation failed\n"
space: .asciiz " "

build_size: .word 72
build_list: .space 0        #initialize to 0
build_list_tail: .space 0     #initialize to 0

name: .space 64
.text
.globl main

main:
    addi $sp, $sp, -20      
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $s2, 12($sp)
    sw $s3, 16($sp)

    li $v0, 9               #malloc
    li $a0, 72              #build_size is 72 bytes
    syscall
    move $s2, $v0

    li $v0, 9               #malloc
    li $a0, 72              #build_size is 72 bytes
    syscall
    move $s3, $v0

_new_build_loop:
    li $v0, 9               #malloc
    li $a0, 72              #build_size is 72 bytes
    syscall
    move $s0, $v0

    li $v0, 4               #print name prompt
    la $a0, name_prompt
    syscall 
    li $v0, 8               #reads the building name
    move $a0, $s0            #moves building name to $s0
    li $a1, 64              #specifies that max name length is 64
    syscall
    move $t0, $a0

    addi $sp, $sp -8        #save t registers before done compare
    sw $t0, 0($sp)
    sw $t1, 4($sp)          
    move $a0, $t0            #Check if input is "DONE"
    la $a1, donestr          #puts done and build name as a1 and a0 
    jal _strcomp
    lw $t0, 0($sp)
    lw $t1, 4($sp)
    addi $sp, $sp 8
    beqz $v0, print_loop

    loop:
        lb $t1, 0($t0)
        beqz $t1, fin
        beq $t1, 10, replace
        addi $t0, $t0, 1
        j loop
    
    replace:
        sb $zero, 0($t0)
        j fin

    fin:

    li $v0, 4               #print sqfeet prompt       
    la $a0, sqFeet_prompt
    syscall
    li $v0, 6               #read sqfeet as float      
    syscall
    mov.s $f5, $f0          #sqfeet read into f0 moved to f5

    li $v0, 4               #print kwh prompt
    la $a0, kwh_prompt
    syscall
    li $v0, 6               #reads kwh as float
    syscall
    mov.s $f6, $f0          #kwh to float in f6

    mov.s $f2, $f5          #create a catch case if sqfeet = 0
    li.s $f3, 0.0
    c.eq.s $f3, $f2
    bc1t _is_zero

    div.s $f4, $f6, $f5     #calculate energy efficiency per year (kwh/sqfeet)
    j _store_result

 _is_zero:
    li.s $f4, 0.0
    s.s $f4, 64($s0)        #store efficiency

_store_result:
    s.s $f4, 64($s0)        #store efficiency
    sw $0, 68($s0)          #initialize next to null

_list_builder:
    beq $s1, $0 _start_list #see if list is null/empty

    move $s0, $s0           #$s0 = new node
    lw $s3, 68($s1)         #puts head->next in s3 (for traverse)
                            #s3 = current/iterator
    move $s2, $s1           #make s2 (previous) = head 

    move $a0, $s0           #new node in a0
    move $a1, $s1           #head in a1
    jal _build_compare      #compare newnode and head node
    bltz $v0, _insertfront  #if compare < 1 then put newnode as head    
_traverse:
    move $a0, $s0           #newnode in a0
    move $a1, $s3           #head->next in a1, compare to newnode
    jal _build_compare      #compare newnode with head->next
    beqz $v0, _insertend    #if v0==0 then newnode goes on end
    bltz $v0, _insertmid    #if v0 <1 then newnode goes here

    lw $s2, 68($s2)         #prev = prev->next
    lw $s3, 68($s3)         #current = current->next
    j _traverse

_insertfront:
    sw $s1, 68($s0)         #newnode->next = head
    move $s1, $s0           #head = newnode
    j _new_build_loop
_insertend:
    sw $s0, 68($s2)         #prev->next = newnode
    j _new_build_loop
_insertmid:
    sw $s0, 68($s2)         #prev->next = newnode
    sw $s3, 68($s0)         #newnode->next = current
    j _new_build_loop

_start_list: 
    move $s1, $s0          #first node in $s1
    j _new_build_loop
_build_compare: #a0 and a1 are a and b
    beqz $a0, _null_error       #check if null
    beqz $a1, _null_error

    addi $sp, $sp -24
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $s2, 12($sp)
    s.s $f20, 16($sp)
    s.s $f21, 20($sp)

    l.s $f20, 64($a0)           #put float vals in f20, ff21
    l.s $f21, 64($a1)

    c.eq.s $f20, $f21           #branch if not equal
    bc1f _effcomp
    addi $sp, $sp -4
    sw $ra, 24($sp)
    jal _strcomp
    lw $ra, 24($sp)
    addi $sp, $sp 4
    j _done_build_compare

_done_build_compare:
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $s2, 12($sp)
    s.s $f20, 16($sp)
    s.s $f21, 20($sp)
    addi $sp, $sp 24
    jr $ra

_null_error:
    li $v0, 0
    j _done_build_compare
_effcomp: 
    c.lt.s $f20, $f21       #if f20 < f21 -> = 1 -> less
    bc1t _less
    j _greater
_less:                      #if f20 < f21 $v0 = 1
    li $v0, 1
    j _done_build_compare
    
_greater:                   #if f20 > f21 $v0 = -1
    li $v0, -1              
    j _done_build_compare

_strcomp:
    lb $t0, 0($a0)          #load byte by byte
    lb $t1, 0($a1)
    bne $t0, $t1, _return_strcomp
    addi $a0,$a0, 1
    addi $a1, $a1, 1
    bnez $t0, _strcomp
    li $v0, 0
    jr $ra
_return_strcomp:              #returns difference (if 0 str ==)
    sub $v0, $t0, $t1
    jr $ra

print_loop:                 #reallocate, save s1 in t0
    beqz $s1, after_print
    li $v0, 4
    move $a0, $s1
    syscall 
    
    li $v0, 4
    la $a0, space
    syscall

    l.s $f12, 64($s1)
    li $v0, 2
    syscall

    la $a0, newline
    li $v0, 4
    syscall
    lw $s1, 68($s1)
    j print_loop

after_print:
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $s2, 12($sp)
    lw $s3, 16($sp)
    addi $sp, $sp, 20       #allocate back after getting structures
    j done_exit

done_exit: 
    li $v0, 10
    syscall