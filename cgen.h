/* Arquivo de cabe�alho para o gerador de c�digo para o processador Nios II.
   Cria��o: 31-ago-2007
   �ltima modifica��o: 31-ago-2007
   Autor: Willian dos Santos Lima */

/* ========================================================================= */
#ifndef CGEN
#define CGEN

/* ------------------------------------------------------------------------- */

#include "def_builder.h"

/* ------------------------------------------------------------------------- */

/* tipos de instru��o*/
typedef enum { ITYPE, JTYPE, RTYPE } TIType;

/* representa��o de instru��es*/
typedef struct INSTR{
   TIType type;      /* identifica��o de quais campos s�o v�lidos*/
   u1 ra, rb, rc;    /* registradores referenciados na instru��o*/
   u1 op, opx;       /* identificadores de instru��es*/
   u1 imm5;          /* valor imediato de 5 bits (R-Type)*/
   u2 imm16;         /* valor imediato de 16 bits (I-Type)*/
   u4 imm26;         /* valor imediato de 26 bits (J-Type)*/
}TInstr;

/* ------------------------------------------------------------------------- */
/* prot�tipos das fun��es de gera��o de c�digo*/

/* fun��o para gerar o c�digo para o Nios II atrav�s da representa��o
   interna de instru�~eos*/
BOOL code_gen ( FILE *_in, FILE *_out );

/* fun��o para montar o nome do arquivo de sa�da*/
void get_bin_name ( char *out_name, char *in_name );

/* ========================================================================= */

#endif
