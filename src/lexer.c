#include "./include/lexer.h"
#include "./include/macros.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

lexer_T* init_lexer(char * src){
    lexer_T * lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->src = src;
    lexer->src_size = strlen(src);
    lexer->i = 0;
    lexer->c = src[lexer->i];
    return lexer;
}

void lexer_advance(lexer_T* lexer){
    if(lexer->i < lexer->src_size && lexer->c != '\0'){
        lexer->i += 1;
        lexer->c = lexer->src[lexer->i];
    }
}


token_T* lexer_advance_current(lexer_T* lexer, int type){
    char* value = calloc(2, sizeof(char));
    value[0] = lexer->c;
    value[1] = '\0';
    lexer_advance(lexer);
    return init_token(value, type);
}

token_T* lexer_advance_with(lexer_T* lexer, token_T* token){
    for (int i = 0; i < strlen(token->value); i++)
        lexer_advance(lexer);
    return token;
}


token_T* lexer_next_token(lexer_T* lexer){
    while(lexer->c != '\0'){
        lexer_skip_whitespace(lexer);

        if(isalpha(lexer->c))
            return lexer_parse_id(lexer);
        if(isdigit(lexer->c))
            return lexer_parse_number(lexer);
        switch(lexer->c){
            case '=':{
                if(lexer_peek(lexer, 1) == '>') 
                    return lexer_advance_with(lexer, init_token("=>", TOKEN_RIGHT_ARROW));
                return lexer_advance_current(lexer, TOKEN_EQUALS);
            }
            case '(': return lexer_advance_current(lexer, TOKEN_LPAREN);
            case ')': return lexer_advance_current(lexer, TOKEN_RPAREN);
            case '{': return lexer_advance_current(lexer, TOKEN_LBRACE);
            case '}': return lexer_advance_current(lexer, TOKEN_RBRACE);
            case ':': return lexer_advance_current(lexer, TOKEN_COLON);
            case ',': return lexer_advance_current(lexer, TOKEN_COMMA);
            case '<': return lexer_advance_current(lexer, TOKEN_LT);
            case '>': return lexer_advance_current(lexer, TOKEN_GT);
            case ';': return lexer_advance_current(lexer, TOKEN_SEMI);
            case '\0': break;
            default: printf("[Lexer]: Uexpected character `%c`\n", lexer->c); exit(1); break;
        }


    }

    return init_token(0, TOKEN_EOF);
}

char lexer_peek(lexer_T* lexer, int offset){
    return lexer->src[lexer->i + offset];
}


token_T* lexer_parse_id(lexer_T* lexer){
    char* value = calloc(1, sizeof(char));
    while(isalpha(lexer->c)){
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c ,0});
        lexer_advance(lexer);
    }
    return init_token(value, TOKEN_ID);
}

token_T* lexer_parse_number(lexer_T* lexer){
    char* value = calloc(1, sizeof(char));
    while(isdigit(lexer->c)){
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c ,0});
        lexer_advance(lexer);
    }
    return init_token(value, TOKEN_INT);
}


void lexer_skip_whitespace(lexer_T* lexer){
    while(lexer->c == 13 || lexer->c == 10 || lexer->c == ' ' || lexer->c == '\t')
        lexer_advance(lexer);
}


