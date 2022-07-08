#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"

// ------- token types -------------------

typedef enum { END_OF_FILE, INT, REAL, BOOLEAN,
               TRUE, FALSE, IF, WHILE, SWITCH, CASE,
               PUBLIC, PRIVATE, NUM, REALNUM, NOT,
               PLUS, MINUS, MULT, DIV, GTEQ, GREATER,
               LTEQ, NOTEQUAL, LESS, LPAREN, RPAREN,
               EQUAL, COLON, COMMA, SEMICOLON, LBRACE,
               RBRACE, ID, ERROR // TODO: Add labels for new token types here
} TokenType;

class Token {
  public:
    void Print();

    std::string purani;
    TokenType gurjant;
    int number_bol;
};

class LexicalAnalyzer {
  public:
    Token aja();
    TokenType UngetToken(Token);
    LexicalAnalyzer();

  private:
    std::vector<Token> tokens;
    int number_bol;
    Token tmp;
    InputBuffer input;

    bool SkipSpace();
    bool SkipComments();
    bool IsKeyword(std::string);
    TokenType FindKeywordIndex(std::string);
    Token ScanIdOrKeyword();
    Token ScanNumber();
};
void raj_brar(std::string n,int t);
int sweetaj_brar(std::string n);

#endif  //__LEXER__H__