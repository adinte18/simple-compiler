.data
var_a: .word 1
var_t0 : .word 0
var_b : .word 0
var_c : .word 0
var_t1 : .word 0
var_d : .word 0
.text
main:
############# COPY START #############
li $t0, 1
sw $t0, var_a
############# COPY FINI #############
############# BOP PLUS START #############
lw $t0, var_a
li $t1, 2
add $t2, $t0, $t1
sw $t2, var_t0
############# BOP PLUS FINI #############
############# COPY START #############
lw $t0, var_t0
sw $t0, var_b
############# COPY FINI #############
############# COPY START #############
lw $t0, var_b
sw $t0, var_c
############# COPY FINI #############
############# BOP PLUS START #############
lw $t0, var_b
lw $t1, var_c
add $t2, $t0, $t1
sw $t2, var_t1
############# BOP PLUS FINI #############
############# COPY START #############
lw $t0, var_t1
sw $t0, var_d
############# COPY FINI #############
li $v0, 1
lw $a0, var_c
syscall
