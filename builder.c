/* ========================================================================= */
/* Montador para o processador Nios II da Altera
   Criação: 31-ago-2007
   Modificado: 03-mai-2008
   Criado por: Willian dos Santos Lima
   Descrição: obtém como parâmetro o nome do arquivo a ser montado */
   
/* ========================================================================= */
#include "def_builder.h"

/* programa principal*/
int main ( int argc, char *argv[] ){
   FILE *_prog, *_tmp, *_bin;
   char line[MAX_LINE+1], tmp_name[31], bin_name[31];
   int cl;
   
   /*verificando quantidade de parâmetros*/
   if ( argc != 2 )
      printf("ERROR: invalid number of parameters.\n");
   else{
      /*abrindo o arquivo com o código a ser montado*/
      if ( (_prog = fopen( argv[1], "r" )) == NULL ){
         printf("ERROR: couldn't load program.\n");
         exit(1);
      }
      
      /*abrindo o arquivo que receberá o código intermediário de rep. de instruções*/
      strcpy( tmp_name, argv[1] );
      strcat( tmp_name, ".tmp" );
      if ( (_tmp = fopen( tmp_name, "wb" )) == NULL ){
         printf("ERROR: couldn't write to disk.\n");
         exit(1);
      }
      
      /*lendo as linhas do arquivo e montando na relação 1:1*/
      cl = 1;
      while ( !feof( _prog ) ){
         fgets( line, MAX_LINE, _prog );
         if ( line[0] == 0 ) continue;
         
         /*analisador sintático para a linha, antes da montagem*/
         if ( !syntax( line ) )
            printf("ERROR: syntax erros at line %d.\n",cl);
         
         /*printf("Line %d: %s",cl++,line);*/
         if ( assemble( line, _tmp ) )
            printf("ERROR: couldn't assembly line %d.\n",cl);
         line[0] = 0;
         cl++;
      }
      fclose( _tmp );
      if ( (_tmp = fopen( tmp_name, "rb" )) == NULL ){
         printf("ERROR: couldn't write to disk.\n");
         exit(1);
      }
      
      /*chamando gerador de código*/
      get_bin_name( bin_name, argv[1] );
      if ( (_bin = fopen( bin_name, "wb" )) == NULL ){
         printf("ERROR: couldn't write to disk.\n");
         /*fechando os arquivos*/
         fclose( _prog );
         fclose( _tmp );
         remove( tmp_name );
         exit(1);
      }
      if ( code_gen( _tmp, _bin ) ){
         printf("ERROR: couldn't generate final code.\n");
         /*fechando os arquivos*/
         fclose( _prog );
         fclose( _bin );
         remove( bin_name );
         fclose( _tmp );
         remove( tmp_name );
         exit(1);
      }else printf("Binary generated successfully!\n");
      
      /*fechando os arquivos*/
      fclose( _prog );
      fclose( _tmp );
      remove( tmp_name );
      fclose( _bin );
   }
   
   return 0;
}
