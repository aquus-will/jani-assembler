/* Arquivo de cabeçalho para o gerador de código para o processador Nios II.
   Criação: 31-ago-2007
   Última modificação: 31-ago-2007
   Autor: Willian dos Santos Lima */

/* ========================================================================= */
#ifndef CGEN
#define CGEN

/* ------------------------------------------------------------------------- */

#include "def_builder.h"

/* ------------------------------------------------------------------------- */

/* tipos de instrução*/
typedef enum { ITYPE, JTYPE, RTYPE } TIType;

/* representação de instruções*/
typedef struct INSTR{
   TIType type;      /* identificação de quais campos são válidos*/
   u1 ra, rb, rc;    /* registradores referenciados na instrução*/
   u1 op, opx;       /* identificadores de instruções*/
   u1 imm5;          /* valor imediato de 5 bits (R-Type)*/
   u2 imm16;         /* valor imediato de 16 bits (I-Type)*/
   u4 imm26;         /* valor imediato de 26 bits (J-Type)*/
}TInstr;

/* ------------------------------------------------------------------------- */
/* protótipos das funções de geração de código*/

/* função para gerar o código para o Nios II através da representação
   interna de instruç~eos*/
BOOL code_gen ( FILE *_in, FILE *_out );

/* função para montar o nome do arquivo de saída*/
void get_bin_name ( char *out_name, char *in_name );

/* ========================================================================= */

#endif
