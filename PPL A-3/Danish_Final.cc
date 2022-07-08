#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE", "INT", "REAL", "BOOL",
                      "TRUE", "FALSE", "IF", "WHILE", "SWITCH",
                      "CASE", "PUBLIC", "PRIVATE", "NUM",
                      "REALNUM", "NOT", "PLUS", "MINUS", "MULT",
                      "DIV", "GTEQ", "GREATER", "LTEQ", "NOTEQUAL",
                      "LESS", "LPAREN", "RPAREN", "EQUAL", "COLON",
                      "COMMA", "SEMICOLON", "LBRACE", "RBRACE", "ID",
					  "ERROR" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 11

string sangroor[] = { "int", "real", "bool", "true",
                     "false", "if", "while", "switch",
                     "case", "public", "private" };



LexicalAnalyzer input;
Token prem;
struct Symbol_TE
{
    string name;
    int lineNo;
    int type;
    bool print_2;
};

struct aivein_Table
{
    Symbol_TE* saini;
    aivein_Table *prev;
    aivein_Table *next;
};

aivein_Table* S_T;
string Final_Danish = "";

void Token::Print()
{
    cout << "{" << this->purani << " , "
        << reserved[(int) this->gurjant] << " , "
        << this->number_bol << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->number_bol = 1;
    tmp.purani = "";
    tmp.number_bol = 1;
    tmp.gurjant = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    number_bol += (c == '\n');
    while (!input.EndOfInput() and isspace(c))
    {
        space_encountered = true;
        input.GetChar(c);
        number_bol += (c == '\n');
    }

    if (!input.EndOfInput())
    {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::SkipComments()
{
    bool comments = false;
    char c;
    if(input.EndOfInput() )
    {
        input.UngetChar(c);
        return comments;
    }

    input.GetChar(c);

    if(c == '/')
    {
        input.GetChar(c);
        if(c == '/')
        {
            comments = true;
            while(c != '\n')
            {
                comments = true;
                input.GetChar(c);
            }
            number_bol++;
            SkipComments();
        }
        else
        {
            comments = false;
            cout << "Syntax Error\n";
            exit(0);
        }
    }
    else
    {
        input.UngetChar(c);
        return comments;
    }
    return true;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++)
    {
        if (s == sangroor[i])
        {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++)
    {
        if (s == sangroor[i])
        {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
    char c;
    bool realNUM = false;
    input.GetChar(c);
    if (isdigit(c))
    {
        if (c == '0')
        {
            tmp.purani = "0";
            input.GetChar(c);
            if(c == '.')
            {
                input.GetChar(c);

                if(!isdigit(c))
                {
                    input.UngetChar(c);
                }
                else
                {
                    while (!input.EndOfInput() and isdigit(c))
                    {
                        tmp.purani += c;
                        input.GetChar(c);
                        realNUM = true;
                    }
                    input.UngetChar(c);
                }
            }
            else
            {
                input.UngetChar(c);
            }
        }
        else
        {
            tmp.purani = "";
            while (!input.EndOfInput() and isdigit(c))
            {
                tmp.purani += c;
                input.GetChar(c);
            }
            if(c == '.')
            {
                input.GetChar(c);

                if(!isdigit(c))
                {
                    input.UngetChar(c);
                }
                else
                {
                    while (!input.EndOfInput() and isdigit(c))
                    {
                        tmp.purani += c;
                        input.GetChar(c);
                        realNUM = true;
                    }
                }
            }
            if (!input.EndOfInput())
            {
                input.UngetChar(c);
            }
        }
        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
        if(realNUM)
        {
            tmp.gurjant = REALNUM;
        }
        else
        {
            tmp.gurjant = NUM;
        }
        tmp.number_bol = number_bol;
        return tmp;
    }
    else
    {
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.purani = "";
        tmp.gurjant = ERROR;
        tmp.number_bol = number_bol;
        return tmp;
    }
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c))
    {
        tmp.purani = "";
        while (!input.EndOfInput() and isalnum(c))
        {
            tmp.purani += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.number_bol = number_bol;

        if (IsKeyword(tmp.purani))
        {
             tmp.gurjant = FindKeywordIndex(tmp.purani);
        }
        else
        {
            tmp.gurjant = ID;
        }
    }
    else
    {
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.purani = "";
        tmp.gurjant = ERROR;
    }
    return tmp;
}


TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.gurjant;
}

Token LexicalAnalyzer::aja()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a prem and
    // return it without reading from input
    if (!tokens.empty())
    {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    // SkipComments();
    SkipSpace();
    tmp.purani = "";
    tmp.number_bol = number_bol;
    input.GetChar(c);
    //cout << "\n Char obtained " << c << "\n";
    switch (c)
    {
        case '!':
            tmp.gurjant = NOT;
            return tmp;
        case '+':
            tmp.gurjant = PLUS;
            return tmp;
        case '-':
            tmp.gurjant = MINUS;
            return tmp;
        case '*':
            tmp.gurjant = MULT;
            return tmp;
        case '/':
            tmp.gurjant = DIV;
            return tmp;
        case '>':
            input.GetChar(c);
            if(c == '=')
            {
                tmp.gurjant = GTEQ;
            }
            else
            {
                input.UngetChar(c);
                tmp.gurjant = GREATER;
            }
            return tmp;
        case '<':
            input.GetChar(c);
            if(c == '=')
            {
                tmp.gurjant = LTEQ;
            }
            else if (c == '>')
            {
                tmp.gurjant = NOTEQUAL;
            }
            else
            {
                input.UngetChar(c);
                tmp.gurjant = LESS;
            }
            return tmp;
        case '(':
            //cout << "\n I am here" << c << " \n";
            tmp.gurjant = LPAREN;
            return tmp;
        case ')':
            tmp.gurjant = RPAREN;
            return tmp;
        case '=':
            tmp.gurjant = EQUAL;
            return tmp;
        case ':':
            tmp.gurjant = COLON;
            return tmp;
        case ',':
            tmp.gurjant = COMMA;
            return tmp;
        case ';':
            tmp.gurjant = SEMICOLON;
            return tmp;
        case '{':
            tmp.gurjant = LBRACE;
            return tmp;
        case '}':
            tmp.gurjant = RBRACE;
            return tmp;
        default:
            if (isdigit(c))
            {
                input.UngetChar(c);
                return ScanNumber();
            }
            else if (isalpha(c))
            {
                input.UngetChar(c);
                //cout << "\n ID scan " << c << " \n";
                return ScanIdOrKeyword();
            }
            else if (input.EndOfInput())
            {
                tmp.gurjant = END_OF_FILE;
            }
            else
            {
                tmp.gurjant = ERROR;
            }
            return tmp;
    }
}

void raj_brar(std::string name,int type)
{
    if(S_T != NULL)
    {
		aivein_Table* mukju = S_T;
        while(mukju->next != NULL)
        {
          if (mukju->saini->name == name)
          {
            mukju->saini->type = type;
            return;
          }
            mukju = mukju->next;
        }

        aivein_Table* newEntry = new aivein_Table();
        Symbol_TE* newItem = new Symbol_TE();

        newItem->name = name;
        newItem->lineNo = prem.number_bol;
        newItem->type = type;
        newItem->print_2 = false;

        newEntry->saini = newItem;
        newEntry->next = NULL;
        newEntry->prev = mukju;
        mukju->next = newEntry;
    }
    else
    {
        aivein_Table* newEntry = new aivein_Table();
        Symbol_TE* newItem = new Symbol_TE();

        newItem->name = name;
        newItem->lineNo = prem.number_bol;
        newItem->type = type;
        newItem->print_2 = false;

        newEntry->saini = newItem;
        newEntry->next = NULL;
        newEntry->prev = NULL;

        S_T = newEntry;
    }
}

int emraan = 4;
int sweetaj_brar(std::string n)
{
    aivein_Table* mukju = S_T;
    bool labgya = false;
    if (S_T != NULL)
    {
		while(mukju->next != NULL)
        {
            if(strcmp(mukju->saini->name.c_str(), n.c_str()) == 0){
                labgya = true;
                return(mukju->saini->type);
            }
            else  mukju = mukju->next;    
        }
        if(strcmp(mukju->saini->name.c_str(), n.c_str()) == 0){
            labgya = true;
            return(mukju->saini->type);
        }
    }
	if (S_T == NULL or !labgya){
		raj_brar(n,emraan);
        emraan++;
        return (emraan-1);  
    }
}



void variable_list_di_parsing_jatto(){
    prem = input.aja();
	//adding to list
    raj_brar(prem.purani,0);
	
	//checking if the input is ID
    if(prem.gurjant == ID){
        prem = input.aja();
		if(prem.gurjant == COLON) input.UngetToken(prem);
        if(prem.gurjant == COMMA) variable_list_di_parsing_jatto();
    }
}

void body_di_parsing_jatto();
int binary_di_parsing_jatto(){
    prem = input.aja();
    if(prem.gurjant != GREATER and prem.gurjant !=LESS and prem.gurjant !=GTEQ and prem.gurjant !=LTEQ and prem.gurjant !=EQUAL and prem.gurjant !=NOTEQUAL and prem.gurjant != PLUS  and prem.gurjant != MINUS and prem.gurjant != MULT and prem.gurjant != DIV) return (-1);
} // parsing the binary operators

int expression_di_parsing_jatto(){
    int amrit;   // parsing the expression to check for C3 and C2 errors respectively.
    prem = input.aja();
	if(prem.gurjant == NOT)
    {
        input.UngetToken(prem);
        prem = input.aja();
		if(prem.gurjant != NOT) return (0);
        amrit = expression_di_parsing_jatto();
        if(amrit== (9-6))
		{
			
		}
		else
        {
            cout << "TYPE MISMATCH " << prem.number_bol << " C3"<<endl;
            exit(1);
        }
    }
    else if( prem.gurjant == DIV or prem.gurjant == GREATER or prem.gurjant == LESS or prem.gurjant == GTEQ or prem.gurjant == LTEQ or prem.gurjant == EQUAL or prem.gurjant == NOTEQUAL or prem.gurjant == PLUS or prem.gurjant == MINUS or prem.gurjant == MULT )
    {
        input.UngetToken(prem);
        amrit = binary_di_parsing_jatto();
        int expression_from_left = expression_di_parsing_jatto(); //comparing left and right expressions
        int expression_from_right = expression_di_parsing_jatto();
        if((!(amrit==(23+3)) and !(amrit>=(23-8) and amrit<=(23-0))) or (expression_from_left != expression_from_right))
        {
			if((amrit >= (23-4) and amrit <= (23-0)) or amrit == (23+3))
            {
                if(expression_from_left > 3 and expression_from_right > 3){
					aivein_Table* mukju = S_T;
					while(mukju->next != NULL)
					{
						if(mukju->saini->type == expression_from_right) mukju->saini->type = expression_from_left;
						mukju = mukju->next;
					}
					if(mukju->saini->type == expression_from_right) mukju->saini->type = expression_from_left;
                    expression_from_right = expression_from_left;
                    return(3);
                }
                else{
                    cout << "TYPE MISMATCH " << prem.number_bol << " C2"<<endl;
                    exit(1);
                }
            }
            if(amrit<=(23-5) and amrit >= (23-8))
            {
				if((expression_from_left <= 2 and expression_from_right > 3) or (expression_from_left > 3 and expression_from_right > 3))
                {
                    aivein_Table* mukju = S_T; // updating types   
					while(mukju->next != NULL) 
					{
						if(mukju->saini->type == expression_from_right) mukju->saini->type = expression_from_left;
						mukju = mukju->next;
					}
					if(mukju->saini->type == expression_from_right) mukju->saini->type = expression_from_left;
                    expression_from_right = expression_from_left;
                }
                else if(expression_from_left > 3 and expression_from_right <= 2)
                {
                    aivein_Table* mukju = S_T; // updating types
					while(mukju->next != NULL)
					{
						if(mukju->saini->type == expression_from_right) mukju->saini->type = expression_from_left;
						mukju = mukju->next;
					}
					if(mukju->saini->type == expression_from_right) mukju->saini->type = expression_from_left;
                    expression_from_left = expression_from_right;
                }
                else
                {
                    cout << "TYPE MISMATCH " << prem.number_bol << " C2"<<endl;
                    exit(1);
                }
            }
            else
            {
                cout << "TYPE MISMATCH " << prem.number_bol << " C2"<<endl;
                exit(1);
            }
        }
        if(amrit >= 19 and amrit <= 23 or amrit == 26) amrit = 3;
        else amrit = expression_from_right;
        
    }
	else if(prem.gurjant == REALNUM or  prem.gurjant == TRUE or  prem.gurjant == ID or prem.gurjant == NUM or prem.gurjant == FALSE )
    {
        input.UngetToken(prem); //parse_primary for NUM , REALNUM , TRUE , FALSE
        prem = input.aja();
		if(prem.gurjant == TRUE or prem.gurjant == FALSE) amrit=(3-0);
		if(prem.gurjant == NUM ) amrit=(3-2);
		if(prem.gurjant == ID ) amrit= sweetaj_brar(prem.purani);
		if(prem.gurjant == REALNUM) amrit=(3-1);		
    }
    return amrit;
}

void caselist_di_parsing_jatto(){ //parsing rhe caselist
    prem = input.aja();
    if(prem.gurjant == CASE)
	{
		prem = input.aja();
		if(prem.gurjant == NUM) //checking if there is num or case after case. and if there's right brace.
		{
			prem = input.aja();
			if(prem.gurjant == COLON) body_di_parsing_jatto();            
		}
        prem = input.aja();
        if(prem.gurjant == CASE)
		{
            input.UngetToken(prem);
            caselist_di_parsing_jatto();
        }
        if(prem.gurjant == RBRACE) input.UngetToken(prem);
    }
}

void statementlist_di_parsing_jatto()
{		
    prem = input.aja();  //parsing the statement and checking if token we get is either if while switch or id
	if(prem.gurjant == WHILE or prem.gurjant == IF)
	{
		prem = input.aja();
		if(prem.gurjant == LPAREN){
			int amrit;
			amrit = expression_di_parsing_jatto();
			if(amrit != (5-2))
			{
				cout<< "TYPE MISMATCH " << prem.number_bol << " C4" << endl;
				exit(1);
			}
			prem = input.aja();
			if(prem.gurjant == RPAREN) body_di_parsing_jatto();
		}
		
    }
	if(prem.gurjant == SWITCH)
    {
		prem = input.aja();
		if(prem.gurjant == LPAREN)
		{
			if(expression_di_parsing_jatto() == 2 or expression_di_parsing_jatto() == 3)
			{
				cout<< "TYPE MISMATCH " << prem.number_bol << " C5"<<endl;
				exit(1);
			}
			prem = input.aja();
			if(prem.gurjant == RPAREN){
				prem = input.aja();
				if(prem.gurjant == LBRACE){
					prem = input.aja();
					if(prem.gurjant == CASE)
					{
						prem = input.aja();
						if(prem.gurjant == NUM){
							prem = input.aja();
							if(prem.gurjant == COLON) body_di_parsing_jatto();            
						}
						prem = input.aja();
						if(prem.gurjant == CASE){
							input.UngetToken(prem);
							caselist_di_parsing_jatto();
						}
						if(prem.gurjant == RBRACE) input.UngetToken(prem);
					}
				}
			}
		}	
    }
	if(prem.gurjant == ID)
    {
		int lefthand__side;
		lefthand__side = sweetaj_brar(prem.purani);
		prem = input.aja();
		if(prem.gurjant == EQUAL)
		{
			prem = input.aja();
			if(prem.gurjant == FALSE or prem.gurjant == PLUS or prem.gurjant == MINUS or prem.gurjant == MULT or prem.gurjant == DIV or prem.gurjant == LESS or prem.gurjant == GREATER or prem.gurjant== GTEQ or prem.gurjant== LTEQ or prem.gurjant == EQUAL or prem.gurjant == NOTEQUAL or prem.gurjant == NOT or prem.gurjant == ID or prem.gurjant == NUM or prem.gurjant == REALNUM or prem.gurjant == TRUE )
			{
				input.UngetToken(prem);
				int righthand__side;
				righthand__side = expression_di_parsing_jatto();
				if(lefthand__side <= (6-3) and lefthand__side != righthand__side)
				{						
					if(lefthand__side >(6-3)){
						aivein_Table* mukju = S_T;
						while(mukju->next != NULL)
						{
							if(mukju->saini->type == righthand__side) mukju->saini->type = lefthand__side;
							mukju = mukju->next;
						}
						if(mukju->saini->type == righthand__side) mukju->saini->type = lefthand__side;
						righthand__side = lefthand__side;
					}
					else
					{
						cout << "TYPE MISMATCH " << prem.number_bol << " C1" << endl;
						exit(1);
					}
					
				}
				else
				{
					aivein_Table* mukju = S_T;
					while(mukju->next != NULL)
					{
						if(mukju->saini->type == lefthand__side) mukju->saini->type = righthand__side;
						mukju = mukju->next;
					}
					if(mukju->saini->type == lefthand__side) mukju->saini->type = righthand__side;
					lefthand__side = righthand__side;
				}
				prem = input.aja();
				if(prem.gurjant != SEMICOLON) cout << "\n Syntax Error " << prem.gurjant << " \n";
			}
		}
	
	}				
	prem = input.aja();
	if (prem.gurjant == RBRACE) input.UngetToken(prem);
	if(prem.gurjant == WHILE or prem.gurjant == SWITCH or prem.gurjant == ID or prem.gurjant == IF)
	{
		input.UngetToken(prem);
		statementlist_di_parsing_jatto();
	}
}

void body_di_parsing_jatto()
{
    prem = input.aja(); //parsing the body of the program or concept
    if(prem.gurjant == LBRACE)
    {
        statementlist_di_parsing_jatto();
        prem = input.aja();
    }
    if(prem.gurjant == END_OF_FILE) input.UngetToken(prem);
}

void program_di_parsing_jatto()
{   //parsing the program and checking and processing until we reach the end of file. 
    prem = input.aja();
    while (prem.gurjant != END_OF_FILE)
    {
		if(prem.gurjant == LBRACE){
			statementlist_di_parsing_jatto();
			prem = input.aja();
		}
        if(prem.gurjant == ID)  //if token is ID.
        {
			while(prem.gurjant == ID){
				input.UngetToken(prem);
				variable_list_di_parsing_jatto();
				prem = input.aja();
				if(prem.gurjant == COLON){
					prem = input.aja();
					if(prem.gurjant == INT or prem.gurjant == REAL or prem.gurjant == BOOLEAN){
						aivein_Table* mukju = S_T;
						while(mukju->next != NULL){
							if(mukju->saini->lineNo == prem.number_bol)  mukju->saini->type =  prem.gurjant;
							mukju = mukju->next;
						}
						if(mukju->saini->lineNo == prem.number_bol) mukju->saini->type =  prem.gurjant;   
					}
				}	   
			}
			
        }
		body_di_parsing_jatto();
		prem = input.aja();
    }
	
	
    aivein_Table* mukju = S_T;
    int shahJI;
//checking for list and printing list.
    while(mukju->next != NULL)
    {
		if (mukju->saini->print_2 == false){
			
			if((5-1)> (mukju->saini->type))
			{
				shahJI = mukju->saini->type;
				cout << mukju->saini->name + ": " + sangroor[shahJI-1] + " #" <<endl;
				Final_Danish = "";
				mukju->saini->print_2 = true;

				while(mukju->next->saini->type == shahJI and mukju->next != NULL)
				{
					mukju = mukju->next;
					cout << mukju->saini->name + ": " + sangroor[(mukju->saini->type)-1] + " #" <<endl;
					mukju->saini->print_2 = true;
					Final_Danish = "";
				}
			}
			
		   if((5-2) < (mukju->saini->type))
			{
				shahJI = mukju->saini->type;
				Final_Danish += mukju->saini->name;
				mukju->saini->print_2 = true;
				while(mukju->next != NULL)
				{
					mukju = mukju->next;
					if(mukju->saini->type == shahJI)
					{
						Final_Danish += ", " + mukju->saini->name;
						mukju->saini->print_2 = true;
					}
				}
				Final_Danish += ": ? #";
				cout << Final_Danish <<endl;
				mukju->saini->print_2 = true;
				Final_Danish = "";
				mukju = S_T;
			}
		}
        else  mukju = mukju->next;
    }
	string left_case_3;
	if (mukju->saini->print_2 == false){
		if (mukju->saini->type >(4-1)) left_case_3="?";
		else left_case_3 = sangroor[(mukju->saini->type)-(2-1)];
        Final_Danish += mukju->saini->name + ": " + left_case_3 + " #";
        cout << Final_Danish <<endl;
        Final_Danish = "";
	}
}
int main()
{
   program_di_parsing_jatto();
}