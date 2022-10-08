/* Arquivo de implementações para o assembler do processador Nios II.
   Criação: 26-nov-2007
   Última modificação: 05-mai-2008
   Autor: Willian dos Santos Lima */

/* ========================================================================= */

#include "def_builder.h"
#include "assembler.h"

/* analisador sintático de entrada: a ser implementado*/
BOOL syntax ( char *l ){
   int i = 0, sz = strlen( l ), j = 0;
   char *aux;
   
   aux = malloc( sz * sizeof(char) );
   
   /*consumindo os espaços em branco iniciais*/
   while ( i < sz && l[i] == ' ' )
      i++;
   
   /*necessariamente, vem um mnemônico agora*/
   while ( i < sz && l[i] != ' ' )
      aux[j] = l[i], i++, j++;
   i++;
   
   /*operandos*/
   while ( i < sz ){
      
      i++;
   }
   
   return 1;   /*linha sintaticamente correta!*/
}

/* ------------------------------------------------------------------------- */

/* função responsável pela montagem efetiva das instruções*/
BOOL assemble ( char *line, FILE *_out ){
   TInstr instr;      /*instrução a ser montada*/
   char mne[11];      /*mnemônico da instrução*/
   char p1[16], p2[16], p3[16];  /*parâmetros*/
   char tmp[16];
   BOOL ok;
   register int i, sz = strlen( line ), nparam;
   
   /*obtendo o mnemônico*/
   mne[0] = 0;
   for ( i = 0; line[i] != ' ' && line[i] != '\n'; i++ )
      mne[i] = line[i];
   mne[i] = 0;
   
   /*obtendo os parâmetros da instrução*/
   nparam = get_num_param( mne );
   switch ( nparam ){
      case 0 : ok = assign_instr0( &instr, mne );
               break;
      case 1 : sscanf( line, "%s %s", tmp, p1 );
               ok = assign_instr1( &instr, mne, p1 );
               break;
      case 2 : sscanf( line, "%s %s %s", tmp, p1, p2 );
               p1[strlen(p1)-1] = 0;   /*para consumir a vírgula*/
               ok = assign_instr2( &instr, mne, p1, p2 );
               break;
      case 3 : sscanf( line, "%s %s %s %s", tmp, p1, p2, p3 );
               p1[strlen(p1)-1] = 0;   /*para consumir a vírgula*/
               p2[strlen(p2)-1] = 0;   /*para consumir a vírgula*/
               ok = assign_instr3( &instr, mne, p1, p2, p3 );
               break;
      case 4 : printf("WARNING: custom instruction not implemented.\n");
               ok = 0;
               break;
      case -1: return -1;  /*falha na tentativa de identificar mnemônico*/
      default: return 1;   /*falha no momento de encontrar parâmetros*/
   }
   
   if ( ok > 0 ){
      /*instrução reconhecida com sucesso*/
      fwrite( &instr, sizeof(TInstr), 1, _out );
   }else if ( ok < 0 ){
      /*pseudo-instruções que serão mapeadas a mais de uma instrução*/
      /*direcionado para instr. c/ 2 parâmetros*/
      if ( !makepseudo( mne, _out, 2, p1, p2 ) )
         return 3;   /*falha na escrita da instrução para o arquivo*/
   }else return 2;   /*instrução não reconhecida*/
   
   return 0;
}

/* ------------------------------------------------------------------------- */
/* IMPORTANTE: alterar seleção da instrução para estrutura de SWITCH         */
/* -> melhor desempenho!                                                     */
/* -> estudar maneira de fazer, já que utiliza string como tipo da variável  */
/*    seletora.                                                              */
/* ------------------------------------------------------------------------- */

BOOL assign_instr0 ( TInstr *instr, char *mne ){
   /*considerar somente instruções sem parâmetros*/
   if ( strcmp( mne, "bret" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = 0x1E;
      instr->rb = instr->rc = 0x0;
      instr->op = 0x3A;
      instr->opx = 0x09;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "eret" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = 0x1D;
      instr->rb = instr->rc = 0x0;
      instr->op = 0x3A;
      instr->opx = 0x01;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "flushp" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = 0x0;
      instr->rb = instr->rc = 0x0;
      instr->op = 0x3A;
      instr->opx = 0x04;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "nop" ) == 0 ){
      /*pseudoinstruction: implemented as add r0, r0, r0*/
      instr->type = RTYPE;
      instr->ra = 0x0;
      instr->rb = 0x0;
      instr->rc = 0x0;
      instr->op = 0x3A;
      instr->opx = 0x31;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "ret" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = 0x1F;
      instr->rb = instr->rc = 0x0;
      instr->op = 0x3A;
      instr->opx = 0x05;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "sync" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = 0x0;
      instr->rb = instr->rc = 0x0;
      instr->op = 0x3A;
      instr->opx = 0x36;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "trap" ) == 0 ){
      instr->type = RTYPE;
      instr->rc = 0x1D;
      instr->rb = instr->ra = 0x0;
      instr->op = 0x3A;
      instr->opx = 0x2D;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   return 0;   /*não identificou a instrução*/
}

BOOL assign_instr1 ( TInstr *instr, char *mne, char *param1 ){
   /*considerar somente instruções com 1 parâmetro*/
   if ( strcmp( mne, "br" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = instr->rb = instr->rc = 0x0;
      instr->op = 0x06;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param1 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "break" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = instr->rb = 0x0;
      instr->rc = 0x1E;
      instr->op = 0x3A;
      instr->opx = 0x34;
      instr->imm5 = atoi( param1 );
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "call" ) == 0 ){
      instr->type = JTYPE;
      instr->ra = instr->rb = instr->rc = 0x0;
      instr->op = 0x0;
      instr->opx = 0x0;
      instr->imm5 = instr->imm16 = 0x0;
      instr->imm26 = atoi( param1 );
      return 1;
   }
   
   if ( strcmp( mne, "callr" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param1 );
      instr->rb = 0x0;
      instr->rc = 0x1F;
      instr->op = 0x3A;
      instr->opx = 0x1D;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "flushi" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param1 );
      instr->rb = 0x0;
      instr->rc = 0x0;
      instr->op = 0x3A;
      instr->opx = 0x0C;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "initi" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param1 );
      instr->rb = 0x0;
      instr->rc = 0x0;
      instr->op = 0x3A;
      instr->opx = 0x29;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "jmp" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param1 );
      instr->rb = 0x0;
      instr->rc = 0x0;
      instr->op = 0x3A;
      instr->opx = 0x0D;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "nextpc" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = 0x0;
      instr->rb = 0x0;
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x1C;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   return 0;
}

BOOL assign_instr2 ( TInstr *instr, char *mne, char *param1, char *param2 ){
   /*considerar somente instruções com 2 parâmetros*/
   
   if ( strcmp( mne, "flushd" ) == 0 ){
		instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = 0x0;
      instr->rc = 0x0;
      instr->op = 0x3B;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param1 );
      instr->imm26 = 0x0;
		return 1;
	}
   
   if ( strcmp( mne, "flushda" ) == 0 ){
		instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = 0x0;
      instr->rc = 0x0;
      instr->op = 0x1B;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param1 );
      instr->imm26 = 0x0;
		return 1;
	}
	
   if ( strcmp( mne, "initd" ) == 0 ){
		instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = 0x0;
      instr->rc = 0x0;
      instr->op = 0x33;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param1 );
      instr->imm26 = 0x0;
		return 1;
	}
   
   if ( strcmp( mne, "mov" ) == 0 ){
		/*pseudoinstruction: implemented as add rC, rA, r0*/
		instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = 0x0;
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x31;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
		return 1;
	}
	
	if ( strcmp( mne, "movhi" ) == 0 ){
		/*pseudoinstruction: implemented as orhi rB, r0, IMMED*/
		instr->type = ITYPE;
      instr->ra = 0x0;
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x34;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
		return 1;
	}
	
	if ( strcmp( mne, "movi" ) == 0 ){
		/*pseudoinstruction: implemented as addi rB, r0, IMMED*/
		instr->type = ITYPE;
      instr->ra = 0x0;
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x04;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param2 );
      instr->imm26 = 0x0;
		return 1;
	}
	
	/*movia -> ESPECIAL: é mapeada para 2 outras instruções!*/
	if ( strcmp( mne, "movia" ) == 0 ){
      return -1;
   }
	
	if ( strcmp( mne, "movui" ) == 0 ){
		/*pseudoinstruction: implemented as ori rB, r0, IMMED*/
		instr->type = ITYPE;
      instr->ra = 0x0;
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x14;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
		return 1;
	}
	
	if ( strcmp( mne, "rdctl" ) == 0 ){
		/*pseudoinstruction: implemented as add rC, rA, r0*/
		instr->type = RTYPE;
      instr->ra = 0x0;
      instr->rb = 0x0;
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x26;
      instr->imm5 = atoi( param2 );
      instr->imm16 = instr->imm26 = 0x0;
		return 1;
	}
	
	if ( strcmp( mne, "wrctl" ) == 0 ){
		/*pseudoinstruction: implemented as add rC, rA, r0*/
		instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = 0x0;
      instr->rc = 0x0;
      instr->op = 0x3A;
      instr->opx = 0x2E;
      instr->imm5 = atoi( param1 );
      instr->imm16 = instr->imm26 = 0x0;
		return 1;
	}
	
   return 0;
}

BOOL assign_instr3 ( TInstr *instr, char *mne, char *param1, char *param2, char *param3 ){
   /*considerar somente instruções com 2 parâmetros*/
   if ( strcmp( mne, "add" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x31;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "addi" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x04;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "and" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x0E;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "andhi" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x2C;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "andi" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x0C;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   /*########################################*/
   if ( strcmp( mne, "beq" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param1 );
      instr->rb = atoi( param2 );
      instr->rc = 0x0;
      instr->op = 0x26;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "bge" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param1 );
      instr->rb = atoi( param2 );
      instr->rc = 0x0;
      instr->op = 0x0E;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "bgeu" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param1 );
      instr->rb = atoi( param2 );
      instr->rc = 0x0;
      instr->op = 0x2E;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "bgt" ) == 0 ){
      /*pseudoinstruction - implemented by blt*/
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x16;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "bgtu" ) == 0 ){
      /*pseudoinstruction - implemented by bltu*/
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x36;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "ble" ) == 0 ){
      /*pseudoinstruction - implemented by bge*/
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x0E;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "bleu" ) == 0 ){
      /*pseudoinstruction - implemented by bgeu*/
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x2E;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "blt" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param1 );
      instr->rb = atoi( param2 );
      instr->rc = 0x0;
      instr->op = 0x16;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "bltu" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param1 );
      instr->rb = atoi( param2 );
      instr->rc = 0x0;
      instr->op = 0x36;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "bne" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param1 );
      instr->rb = atoi( param2 );
      instr->rc = 0x0;
      instr->op = 0x1E;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   /*########################################*/
   if ( strcmp( mne, "cmpeq" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x20;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpeqi" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x20;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpge" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x08;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpgei" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x08;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpgeu" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x28;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpgeui" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x28;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpgt" ) == 0 ){
      /*pseudoinstruction - implemented by cmplt*/
      instr->type = RTYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param2 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x10;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpgti" ) == 0 ){
      /*pseudoinstruction - implemented by cmpgei with IMM16 + 1*/
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x08;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 ) + 1;
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpgtu" ) == 0 ){
      /*pseudoinstruction - implemented by cmpltu*/
      instr->type = RTYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param2 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x30;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpgtui" ) == 0 ){
      /*pseudoinstruction - implemented by cmpgeui with IMM16 + 1*/
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x28;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 ) + 1;
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmple" ) == 0 ){
      /*pseudoinstruction - implemented by cmpge*/
      instr->type = RTYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param2 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x08;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmplei" ) == 0 ){
      /*pseudoinstruction - implemented by cmplti with IMM16 + 1*/
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x10;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 ) + 1;
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpleu" ) == 0 ){
      /*pseudoinstruction - implemented by cmpgeu*/
      instr->type = RTYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param2 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x28;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpleui" ) == 0 ){
      /*pseudoinstruction - implemented by cmpltui with IMM16 + 1*/
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x30;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 ) + 1;
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmplt" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x10;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmplti" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x10;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpltu" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x30;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpltui" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x30;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpne" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x18;
      instr->imm5 = instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "cmpnei" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x18;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   /*########################################*/
   if ( strcmp( mne, "div" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x25;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "divu" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x24;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   /*ldb/ldbio*/
	if ( strcmp( mne, "ldb" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x07;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
	
	if ( strcmp( mne, "ldbio" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x27;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
   
	/*ldbu/ldbuio*/
	if ( strcmp( mne, "ldbu" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x03;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
	
	if ( strcmp( mne, "ldbuio" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x23;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
   
	/*ldh/ldhio*/
	if ( strcmp( mne, "ldh" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x0F;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
	
	if ( strcmp( mne, "ldhio" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x2F;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
   
	/*ldhu/ldhuio*/
	if ( strcmp( mne, "ldhu" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x0B;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
	
	if ( strcmp( mne, "ldhuio" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x2B;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
   
	/*ldw/ldwio*/
	if ( strcmp( mne, "ldw" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x17;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
	
	if ( strcmp( mne, "ldwio" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x37;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
   
	/*########################################*/
	if ( strcmp( mne, "mul" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x27;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
	
	if ( strcmp( mne, "muli" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x24;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 =0x0;
      instr->imm16 = atoi( param3 );
      return 1;
   }
   
   if ( strcmp( mne, "mulxss" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x1F;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "mulxsu" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x17;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "mulxuu" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x07;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   /*########################################*/
   if ( strcmp( mne, "nor" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x06;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "or" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x16;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "orhi" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x34;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param3 );
      return 1;
   }
   
   if ( strcmp( mne, "ori" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x14;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param3 );
      return 1;
   }
   
   /*########################################*/
   if ( strcmp( mne, "rol" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x03;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "roli" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = 0x0;
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x02;
      instr->imm5 = atoi( param3 );
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "ror" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x0B;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "sll" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x13;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "slli" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = 0x0;
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x12;
      instr->imm5 = atoi( param3 );
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "sra" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x3B;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "srai" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = 0x0;
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x3A;
      instr->imm5 = atoi( param3 );
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "srl" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x1B;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "srli" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = 0x0;
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x1A;
      instr->imm5 = atoi( param3 );
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   /*stb/stbio*/
   if ( strcmp( mne, "stb" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x05;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
	
	if ( strcmp( mne, "stbio" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x25;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
   
   /*sth/sthio*/
   if ( strcmp( mne, "sth" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x0D;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
	
	if ( strcmp( mne, "sthio" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x2D;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
   
   /*stw/stwio*/
   if ( strcmp( mne, "stw" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x15;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
	
	if ( strcmp( mne, "stwio" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param3 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x35;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param2 );
      return 1;
   }
   
   /*########################################*/
	if ( strcmp( mne, "sub" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x39;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "subi" ) == 0 ){
      /*pseudoinstruction: implemented as addi rB, rA, -IMMED*/
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x04;
      instr->opx = instr->imm5 = 0x0;
      instr->imm16 = -atoi( param3 );
      instr->imm26 = 0x0;
      return 1;
   }
   
   /*########################################*/
   if ( strcmp( mne, "xor" ) == 0 ){
      instr->type = RTYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param3 );
      instr->rc = atoi( param1 );
      instr->op = 0x3A;
      instr->opx = 0x1E;
      instr->imm5 = 0x0;
      instr->imm16 = instr->imm26 = 0x0;
      return 1;
   }
   
   if ( strcmp( mne, "xorhi" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x3C;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param3 );
      return 1;
   }
   
   if ( strcmp( mne, "xori" ) == 0 ){
      instr->type = ITYPE;
      instr->ra = atoi( param2 );
      instr->rb = atoi( param1 );
      instr->rc = 0x0;
      instr->op = 0x1C;
      instr->opx = 0x0;
      instr->imm5 = instr->imm26 = 0x0;
      instr->imm16 = atoi( param3 );
      return 1;
   }
   
   return 0;
}


/* ------------------------------------------------------------------------- */

int get_num_param ( char *mne ){
   if ( strcmp( mne, "add" ) == 0 ) return 3;
   if ( strcmp( mne, "addi" ) == 0 ) return 3;
   if ( strcmp( mne, "and" ) == 0 ) return 3;
   if ( strcmp( mne, "andhi" ) == 0 ) return 3;
   if ( strcmp( mne, "andi" ) == 0 ) return 3;
   
   if ( strcmp( mne, "beq" ) == 0 ) return 3;
   if ( strcmp( mne, "bge" ) == 0 ) return 3;
   if ( strcmp( mne, "bgeu" ) == 0 ) return 3;
   if ( strcmp( mne, "bgt" ) == 0 ) return 3;
   if ( strcmp( mne, "bgtu" ) == 0 ) return 3;
   if ( strcmp( mne, "ble" ) == 0 ) return 3;
   if ( strcmp( mne, "bleu" ) == 0 ) return 3;
   if ( strcmp( mne, "blt" ) == 0 ) return 3;
   if ( strcmp( mne, "bltu" ) == 0 ) return 3;
   if ( strcmp( mne, "bne" ) == 0 ) return 3;
   if ( strcmp( mne, "br" ) == 0 ) return 1;
   if ( strcmp( mne, "break" ) == 0 ) return 1;
   if ( strcmp( mne, "bret" ) == 0 ) return 0;
   
   if ( strcmp( mne, "call" ) == 0 ) return 1;
   if ( strcmp( mne, "callr" ) == 0 ) return 1;
   if ( strcmp( mne, "cmpeq" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpeqi" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpge" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpgei" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpgeu" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpgt" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpgti" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpgtu" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpgtui" ) == 0 ) return 3;
   if ( strcmp( mne, "cmple" ) == 0 ) return 3;
   if ( strcmp( mne, "cmplei" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpleu" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpleui" ) == 0 ) return 3;
   if ( strcmp( mne, "cmplt" ) == 0 ) return 3;
   if ( strcmp( mne, "cmplti" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpltu" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpltui" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpne" ) == 0 ) return 3;
   if ( strcmp( mne, "cmpnei" ) == 0 ) return 3;
   if ( strcmp( mne, "custom" ) == 0 ) return 4;
   
   if ( strcmp( mne, "div" ) == 0 ) return 3;
   if ( strcmp( mne, "divu" ) == 0 ) return 3;
   
   if ( strcmp( mne, "eret" ) == 0 ) return 0;
   if ( strcmp( mne, "flushd" ) == 0 ) return 2;
   if ( strcmp( mne, "flushda" ) == 0 ) return 2;
   if ( strcmp( mne, "flushi" ) == 0 ) return 1;
   if ( strcmp( mne, "flushp" ) == 0 ) return 0;
   
   if ( strcmp( mne, "initd" ) == 0 ) return 2;
   if ( strcmp( mne, "initi" ) == 0 ) return 1;
   if ( strcmp( mne, "jmp" ) == 0 ) return 1;
   if ( strcmp( mne, "jmpi" ) == 0 ) return 1;
   
   if ( strcmp( mne, "ldb" ) == 0 ) return 3;
   if ( strcmp( mne, "ldbio" ) == 0 ) return 3;
   if ( strcmp( mne, "ldbu" ) == 0 ) return 3;
   if ( strcmp( mne, "ldbuio" ) == 0 ) return 3;
   if ( strcmp( mne, "ldh" ) == 0 ) return 3;
   if ( strcmp( mne, "ldhio" ) == 0 ) return 3;
   if ( strcmp( mne, "ldhu" ) == 0 ) return 3;
   if ( strcmp( mne, "ldhuio" ) == 0 ) return 3;
   if ( strcmp( mne, "ldhw" ) == 0 ) return 3;
   if ( strcmp( mne, "ldhwio" ) == 0 ) return 3;
   
   if ( strcmp( mne, "mov" ) == 0 ) return 2;
   if ( strcmp( mne, "movhi" ) == 0 ) return 2;
   if ( strcmp( mne, "movi" ) == 0 ) return 2;
   if ( strcmp( mne, "movia" ) == 0 ) return 2;
   if ( strcmp( mne, "movui" ) == 0 ) return 2;
   if ( strcmp( mne, "mul" ) == 0 ) return 3;
   if ( strcmp( mne, "muli" ) == 0 ) return 3;
   if ( strcmp( mne, "mulxss" ) == 0 ) return 3;
   if ( strcmp( mne, "mulxsu" ) == 0 ) return 3;
   if ( strcmp( mne, "mulxuu" ) == 0 ) return 3;
   
   if ( strcmp( mne, "nextpc" ) == 0 ) return 1;
   if ( strcmp( mne, "nop" ) == 0 ) return 0;
   if ( strcmp( mne, "nor" ) == 0 ) return 3;
   if ( strcmp( mne, "or" ) == 0 ) return 3;
   if ( strcmp( mne, "orhi" ) == 0 ) return 3;
   if ( strcmp( mne, "ori" ) == 0 ) return 3;
   
   if ( strcmp( mne, "rdctl" ) == 0 ) return 2;
   if ( strcmp( mne, "ret" ) == 0 ) return 0;
   if ( strcmp( mne, "rol" ) == 0 ) return 3;
   if ( strcmp( mne, "roli" ) == 0 ) return 3;
   if ( strcmp( mne, "ror" ) == 0 ) return 3;
   
   if ( strcmp( mne, "sll" ) == 0 ) return 3;
   if ( strcmp( mne, "slli" ) == 0 ) return 3;
   if ( strcmp( mne, "sra" ) == 0 ) return 3;
   if ( strcmp( mne, "srai" ) == 0 ) return 3;
   if ( strcmp( mne, "srl" ) == 0 ) return 3;
   if ( strcmp( mne, "srli" ) == 0 ) return 3;
   
   if ( strcmp( mne, "stb" ) == 0 ) return 3;
   if ( strcmp( mne, "stbio" ) == 0 ) return 3;
   if ( strcmp( mne, "sth" ) == 0 ) return 3;
   if ( strcmp( mne, "sthio" ) == 0 ) return 3;
   if ( strcmp( mne, "stw" ) == 0 ) return 3;
   if ( strcmp( mne, "stwio" ) == 0 ) return 3;
   if ( strcmp( mne, "sub" ) == 0 ) return 3;
   if ( strcmp( mne, "subi" ) == 0 ) return 3;
   if ( strcmp( mne, "sync" ) == 0 ) return 0;
   
   if ( strcmp( mne, "trap" ) == 0 ) return 0;
   if ( strcmp( mne, "wrctl" ) == 0 ) return 2;
   if ( strcmp( mne, "xor" ) == 0 ) return 3;
   if ( strcmp( mne, "xorhi" ) == 0 ) return 3;
   if ( strcmp( mne, "xori" ) == 0 ) return 3;
   
   return -1;
}

/* ------------------------------------------------------------------------- */

/*para esse tipo de função, usar va_arg()... (pg. 486 - C Completo e Total)*/
BOOL makepseudo ( char *mne, FILE *_out, int np, ... ){
   TInstr instr;
   va_list argptr;   /*controle dos parâmetros "variáveis"*/
   char *p[4];
   u1 i = 0;
   u4 tmp_u4;
   
   va_start( argptr, np );  /*inicialização de argptr*/
   
   while ( ++i <= np ){
      p[i] = va_arg( argptr, char* );  /*obtenção do próximo argumento*/
      /*printf("%s\n",p[i]);*/
   }
   
   switch ( np ){
      case 1 : break;
      case 2 : if ( strcmp( mne, "movia" ) == 0 ){
                  /*rB -> p[1], label -> p[2]*/
                  tmp_u4 = atoi( p[2] );
                  /*orhi rB, r0, %hiadj(label)*/
                  instr.type = ITYPE;
                  instr.ra = 0x0;
                  instr.rb = atoi( p[1] );
                  instr.rc = 0x0;
                  instr.op = 0x34;
                  instr.opx = 0x0;
                  instr.imm5 = instr.imm26 = 0x0;
                  /*atenção abaixo! (pg. 8-7, ref. Altera)*/
                  instr.imm16 = ( tmp_u4 >> 16 ) + 0xFFFF + ( ( tmp_u4 >> 15 ) & 0x1 );
                  fwrite( &instr, sizeof(TInstr), 1, _out );
                  /*addi rB, r0, %lo(label)*/
                  instr.type = ITYPE;
                  instr.ra = 0x0;
                  instr.rb = atoi( p[1] );
                  instr.rc = 0x0;
                  instr.op = 0x04;
                  instr.opx = instr.imm5 = 0x0;
                  instr.imm16 = tmp_u4;
                  instr.imm26 = 0x0;
                  fwrite( &instr, sizeof(TInstr), 1, _out );
                  break;
               }
               break;
      case 3 : break;
   }
   
   va_end( argptr );
   
   return 1;
}
