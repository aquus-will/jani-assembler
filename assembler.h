/* Arquivo de cabe�alho para o assembler do processador Nios II.
   Cria��o: 26-nov-2007
   �ltima modifica��o: 03-mai-2008
   Autor: Willian dos Santos Lima */

/* ========================================================================= */
#ifndef ASSEMBLER
#define ASSEMBLER

#include "cgen.h"

/* ------------------------------------------------------------------------- */

/*fun��o que verifica se uma linha do arquivo de entrada est� sintaticamente
  correta, modificando para a chamada da fun��o de montagem*/
BOOL syntax ( char *l );

/* fun��o para gerar no arquivo de sa�da a instru��o montada*/
BOOL assemble ( char *line, FILE *_out );

/* fun��o para obter o n�mero de par�metros para uma instru��o*/
int get_num_param ( char *mne );

/* fun��es para efetuar a atribui��o efetiva de uma instru��o*/
BOOL assign_instr0 ( TInstr *instr, char *mne );
BOOL assign_instr1 ( TInstr *instr, char *mne, char *param1 );
BOOL assign_instr2 ( TInstr *instr, char *mne, char *param1, char *param2 );
BOOL assign_instr3 ( TInstr *instr, char *mne, char *param1, char *param2, char *param3 );

/* fun��o para mapear pseudo-instru��es que s�o mapeadas para mais de uma
   instru��o*/
BOOL makepseudo ( char *mne, FILE *_out, int np, ... );

/* ========================================================================= */

#endif
