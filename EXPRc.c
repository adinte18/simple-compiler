
#include "EXPRc.h"
#include <string.h>

struct symtable * SYMTAB;
struct code * CODE;

void generate_mips(struct code * interCode) {
    FILE* output_file = fopen("output.asm", "w");
    if (output_file == NULL) {
        fprintf(stderr, "error openning the file");
        exit(EXIT_FAILURE);
    }

    // Déclarer les variables temporaires dans la section .data
    fprintf(output_file, ".data\n");


    // Initialiser les variables dans MIPS ( Il y a que 7 registres temporaires en MIPS, plus facile a faire comme ca )
    for (int i = 0; i < interCode->nextquad; ++i) {
        struct quad currentQuad = interCode->quads[i];

        if (currentQuad.sym2) {
            if (currentQuad.sym2->u.value == CONSTANT) {
                // si la variable a une valeur , c'est a dire a = 1, on va lui attribuer un espace memoire et on l'initialise a 1
                fprintf(output_file, "var_%s: .word %ld\n", currentQuad.sym1->u.name, currentQuad.sym2->u.value);
            }
            // sinon on fait pareil si la variable n'a pas une valeur
            // c'est a dire si on a b = a + c, on va creer la variable b et on initialise a 0 pour stocker le resultat de a + c
            else fprintf(output_file, "var_%s : .word 0\n", currentQuad.sym1->u.name);
        }
    }

    fprintf(output_file, ".text\n");
    fprintf(output_file, "main:\n");

    // On va parcourir chaque quad en regardant qu'est-ce qu'il y a dedans
    for (int i = 0; i < interCode->nextquad; ++i) {
        struct quad currentQuad = interCode->quads[i];
        // on fait un switch pour chaque type d'operation
        switch (currentQuad.kind) {
            case COPY:
                fprintf(output_file, "############# COPY START #############\n");
                if (currentQuad.sym2->kind == CONSTANT) {
                    fprintf(output_file, "li $t0, %ld\n", currentQuad.sym2->u.value);
                } else {
                    fprintf(output_file, "lw $t0, var_%s\n", currentQuad.sym2->u.name);
                }
                fprintf(output_file, "sw $t0, var_%s\n", currentQuad.sym1->u.name);
                fprintf(output_file, "############# COPY FINI #############\n");
                break;
            case BOP_PLUS:
                fprintf(output_file, "############# BOP PLUS START #############\n");
                if (currentQuad.sym2->kind == CONSTANT) {
                    fprintf(output_file, "li $t0, %ld\n", currentQuad.sym2->u.value);
                } else {
                    fprintf(output_file, "lw $t0, var_%s\n", currentQuad.sym2->u.name);
                }
                if (currentQuad.sym3->kind == CONSTANT) {
                    fprintf(output_file, "li $t1, %ld\n", currentQuad.sym3->u.value);
                } else {
                    fprintf(output_file, "lw $t1, var_%s\n", currentQuad.sym3->u.name);
                }     
                fprintf(output_file, "add $t2, $t0, $t1\n");
                fprintf(output_file, "sw $t2, var_%s\n", currentQuad.sym1->u.name);
                fprintf(output_file, "############# BOP PLUS FINI #############\n");
                break;

            case BOP_MULT:
                fprintf(output_file, "############# BOP MULT START #############\n");
                if (currentQuad.sym2->kind == CONSTANT) {
                    fprintf(output_file, "li $t0, %ld\n", currentQuad.sym2->u.value);
                } else {
                    fprintf(output_file, "lw $t0, var_%s\n", currentQuad.sym2->u.name);
                }
                if (currentQuad.sym3->kind == CONSTANT) {
                    fprintf(output_file, "li $t1, %ld\n", currentQuad.sym3->u.value);
                } else {
                    fprintf(output_file, "lw $t1, var_%s\n", currentQuad.sym3->u.name);
                }     
                fprintf(output_file, "mul $t2, $t0, $t1\n");
                fprintf(output_file, "sw $t2, var_%s\n", currentQuad.sym1->u.name);
                fprintf(output_file, "############# BOP MULT FINI #############\n");
                break;

            case UOP_MINUS:
                fprintf(output_file, "############# UOP MINUS START #############\n");
                if (currentQuad.sym2->kind == CONSTANT) {
                    fprintf(output_file, "li $t0, %ld\n", currentQuad.sym2->u.value);
                } else {
                    fprintf(output_file, "lw $t0, var_%s\n", currentQuad.sym2->u.name);
                }
                fprintf(output_file, "sub $t1 $zero, $t0\n");
                fprintf(output_file, "sw $t1, var_%s\n", currentQuad.sym1->u.name);
                fprintf(output_file, "############# UOP MINUS FINI #############\n");
                break;
            case CALL_PRINT:
                fprintf(output_file, "li $v0, 1\n");
                fprintf(output_file, "lw $a0, var_%s\n", currentQuad.sym1->u.name);
                fprintf(output_file, "syscall\n");
                break;
            default:
                // Gérer les cas par défaut si nécessaire
                break;        
            }
    }
}

int main()
{
    SYMTAB = symtable_new();
    CODE = code_new();
    int r = yyparse();
    symtable_dump(SYMTAB);
    code_dump(CODE);

    generate_mips(CODE);
    
    symtable_free(SYMTAB);
    code_free(CODE);
    return r;
}
