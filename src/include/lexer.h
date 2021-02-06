#ifndef TAC_LEXER_H
#define TAC_LEXER_H

typedef struct LEXER_STRUCT{
    char * src;
    char c;
    unsigned int i;
} lexer_T;

lexer_T*  init_lexer(char* src);

endif
