/* Arquivo de implementa??es para o gerador de c?digo para o processador Nios II.
   Cria??o: 31-ago-2007
   ?ltima modifica??o: 03-jul-2008
   Autor: Willian dos Santos Lima */

/* ========================================================================= */

#include "cgen.h"

/* ------------------------------------------------------------------------- */

BOOL code_gen ( FILE *_in, FILE *_out ){
   TInstr instr;
   u4 word, tmp_u4, tmp2_u4, nw, i;
   u1 tmp_u1;
   
   /*leitura da primeira instru??o do arquivo tempor?rio*/
   fread( &instr, sizeof(TInstr), 1, _in );
   while ( !feof( _in ) ){
      /*verificando o tipo de instru??o para montar a palavra*/
      if ( instr.type == ITYPE ){
         /*ITYPE word -> A, B, IMMED16, OP*/
         tmp_u4 = instr.ra;
         word = ( tmp_u4 << 27 );
         tmp_u4 = instr.rb;
         tmp_u4 = ( tmp_u4 << 22 );
         word = word + tmp_u4;
         tmp_u4 = instr.imm16;
         tmp_u4 = ( tmp_u4 << 6 );
         word = word + tmp_u4;
         word = word + instr.op;
      }else if ( instr.type == JTYPE ){
         /*JTYPE word -> IMMED26, OP*/
         tmp_u4 = instr.imm16;
         tmp_u4 = ( tmp_u4 << 6 );
         word = tmp_u4;
         word = word + instr.op;
      }else if ( instr.type == RTYPE ){
         /*RTYPE word -> A, B, C, OPX, IMM5, OP*/
         tmp_u4 = instr.ra;
         word = ( tmp_u4 << 27 );
         tmp_u4 = instr.rb;
         tmp_u4 = ( tmp_u4 << 22 );
         word = word + tmp_u4;
         tmp_u4 = instr.rc;
         tmp_u4 = ( tmp_u4 << 17 );
         word = word + tmp_u4;
         tmp_u4 = instr.opx;
         tmp_u4 = ( tmp_u4 << 11 );
         word = word + tmp_u4;
         tmp_u4 = instr.imm5;
         tmp_u4 = ( tmp_u4 << 6 );
         word = word + tmp_u4;
         word = word + instr.op;
      }else return 1;   /*falha na identifica??o do tipo de instru??o*/
      
      /* ------------------------------------------------------------------- */
      /*teste:*/
      tmp_u4 = 0;
      for ( i = 0; i < 16; i++ ){
         /*if ( i > 0 )
            tmp_u4 = tmp_u4 << 1;*/
         tmp_u4 = tmp_u4 | ( ( word & ( 1 << ( 31 - i ) ) ) >> ( 31 - 2 * i ) );
         /*printf("%d) tmp_u4 = 0x%X\n",i,tmp_u4);*/
      }
      tmp2_u4 = 0;
      nw = word << 16;
      for ( i = 0; i < 16; i++ ){
         /*if ( i > 0 )
            tmp_u4 = tmp_u4 << 1;*/
         tmp2_u4 = tmp2_u4 | ( ( nw & ( 1 << ( 31 - i ) ) ) >> ( 31 - 2 * i ) );
         /*printf("%d) tmp_u4 = 0x%X\n",i,tmp_u4);*/
      }
      
      tmp_u4 = tmp_u4 + ( tmp2_u4 << 16 );
      /*word = tmp_u4;*/
      /*printf("\nword = 0x%08X\ntmp_u4 = 0x%08X\ntmp2_u4 = 0x%08X\n",word,tmp_u4,tmp2_u4);*/
      /* ------------------------------------------------------------------- */
      
      tmp_u1 = word >> 24;
      fwrite( &tmp_u1, sizeof(u1), 1, _out );
      /*printf("0x%02X",tmp_u1);*/
      
      tmp_u1 = ( word << 8 ) >> 24;
      fwrite( &tmp_u1, sizeof(u1), 1, _out );
      /*printf("%02X",tmp_u1);*/
      
      tmp_u1 = ( word << 16 ) >> 24;
      fwrite( &tmp_u1, sizeof(u1), 1, _out );
      /*printf("%02X",tmp_u1);*/
      
      tmp_u1 = ( word << 24 ) >> 24;
      fwrite( &tmp_u1, sizeof(u1), 1, _out );
      /*printf("%02X\n",tmp_u1);*/
      
      /*grava??o da palavra no arquivo bin?rio*/
      /*fwrite( &word, sizeof(u4), 1, _out );*/
      /*leitura da pr?xima instru??o do arquivo tempor?rio*/
      fread( &instr, sizeof(TInstr), 1, _in );
   }
      
   return 0;
}

/* ------------------------------------------------------------------------- */

void get_bin_name ( char *out_name, char *in_name ){
   register int i, sz = strlen( in_name );
   
   strcpy( out_name, in_name );
   
   for ( i = sz - 1; i >= 0; i-- )
      if ( out_name[i] == '.' ){
         out_name[i+1] = 0;
         break;
      }
   
   strcat( out_name, "bin" );
}
