/* Arquivo de cabeçalho para o assembler do processador Nios II.
   Criação: 26-nov-2007
   Última modificação: 03-mai-2008
   Autor: Willian dos Santos Lima */

/* ========================================================================= */
#ifndef ASSEMBLER
#define ASSEMBLER

#include "cgen.h"

/* ------------------------------------------------------------------------- */

/*função que verifica se uma linha do arquivo de entrada está sintaticamente
  correta, modificando para a chamada da função de montagem*/
BOOL syntax ( char *l );

/* função para gerar no arquivo de saída a instrução montada*/
BOOL assemble ( char *line, FILE *_out );

/* função para obter o número de parâmetros para uma instrução*/
int get_num_param ( char *mne );

/* funções para efetuar a atribuição efetiva de uma instrução*/
BOOL assign_instr0 ( TInstr *instr, char *mne );
BOOL assign_instr1 ( TInstr *instr, char *mne, char *param1 );
BOOL assign_instr2 ( TInstr *instr, char *mne, char *param1, char *param2 );
BOOL assign_instr3 ( TInstr *instr, char *mne, char *param1, char *param2, char *param3 );

/* função para mapear pseudo-instruções que são mapeadas para mais de uma
   instrução*/
BOOL makepseudo ( char *mne, FILE *_out, int np, ... );

/* ========================================================================= */

#endif
