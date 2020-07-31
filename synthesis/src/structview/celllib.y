%{

#include <iostream>
#include <map>
#include <list>

#include <stdio.h>

#include "scell.h"
#include "sport.h"
#include "libraryreader.h"

using namespace stview;
using namespace std;

extern int 		yylex();
extern char*	yytext;
extern int		CellLiblineno;
extern int 		Lineno;

void yyerror(char const *str)
{
    std::cout << "Syntax error at line " << Lineno << ".\n";
    //std::cout << yytext << Lineno << ".\n";
}

char *func_type_to_str(int type);
static SCell *cell = 0;
static std::list<Attribute> attributes;

%}
%union {
	char				*TEXT;
	int					INT;
	float				REAL;
	std::pair<int, int>	*INTPAIR;
	std::list< SPort* >	*PORTS;
	std::list< SCell* >	*SCELLS;
	SCell				*SCELL;
	SPort::Dir			PORTTYPE;
	SCell::CellType		CELLTYPE;
	Attribute*			ATTRIB;
};

%token	LIBRARY AUTHOR VERSION CELL PIN IN OUT BIDI FF RFF SFF RSFF SRFF CMP
%token	BUF TRIBUF OR AND NOR NAND XOR XNOR NOT ADD SUB MUL LSH FUNCTION
%token	DIV MUX ENC	DEC VCC GND AREA DELAY ATTRIBUTE LD RLD RSH INC

%token		<TEXT>	STRING IDENTIFIER 
%token		<INT>	NUMBER
%token		<REAL>	REAL_NUMBER

%type	<REAL> delay	
%type	<REAL> area	
%type	<SCELL> cell_decl
%type	<SCELLS> list_of_cells
%type	<TEXT> cell_name
%type	<TEXT> pin_name
%type	<INTPAIR> range_opt
%type	<PORTS> pin_declaration
%type	<PORTS> pin_declaration_list
%type	<PORTS> pin_description
%type	<PORTTYPE> direction
%type	<CELLTYPE> function_type
%type	<CELLTYPE> function_declaration
%type	<ATTRIB> attribute


%%

library_description : library_header_opt cell_description_opt
;

library_header_opt : library_header_items
	|
;

library_header_items: library_header_item
	| library_header_items library_header_item
;

library_header_item:	
	LIBRARY	':' STRING
	|	AUTHOR	':' STRING
	|	VERSION	':' STRING
;

cell_description_opt:	list_of_cells {
		LibraryReader::instance()->setCells($1);
	}	
	|
;

list_of_cells: cell_decl { $$ = new list<SCell*>(); $$->push_back($1); }
	| list_of_cells cell_decl { $1->push_back($2); }
;

cell_decl:	CELL cell_name { 
    cell = new stview::SCell($2, SCell::CELLTYPE_BEGIN); 
	attributes.clear()
	}
	'{' cell_items '}' { $$ = cell; }
;
cell_name: IDENTIFIER 
    | function_type { $$ = func_type_to_str($1); }
;

cell_items : pin_description function_declaration other_cell_items_opt {
		for(list<SPort*>::const_iterator port = $1->begin();
			port != $1->end(); ++port) {
			cell->addPort(*port);
		}	
		cell->setCellType($2);
		LibraryReader::instance()->processAttributes(attributes, $1, cell);
	}	
;

pin_description : pin_declaration_list
;

pin_declaration_list : pin_declaration { $$ = $1; }
	| pin_declaration_list pin_declaration { $1->splice($1->end(), *$2); }
;	

pin_declaration: PIN ':' direction range_opt pin_name ';' {
    $$ = new list<SPort*>();
	if(!$4) {
	    $$->push_back(new SPort($5, $3));
	}
	else
	{
	    int msb = $4->first;
		int lsb = $4->second;
		for(int i = (msb > lsb) ? lsb : msb; msb > lsb ? i <= msb : i >= lsb;
			msb > lsb ? ++i : --i) 
		{
		    char tmp[128];
			sprintf(tmp, "#%d", i);
			
			$$->push_back(new SPort($5 + string(tmp), $3));
			//delete $4;
		}	
	}
}	
;

direction: IN { $$ = SPort::IN; }
	| OUT { $$ = SPort::OUT; }
	| BIDI { $$ = SPort::INOUT; }
;

range_opt : '[' NUMBER ':' NUMBER ']' { $$ = new std::pair<int, int>($2, $4); }
	| { $$ = NULL; }
;

pin_name: IDENTIFIER
;

function_declaration : FUNCTION ':' function_type ';' { $$ = $3; }
function_type : 
		BUF			{ $$ = SCell::BUF; }
		| TRIBUF 	{ $$ = SCell::TRIBUF; }
		| OR 		{ $$ = SCell::OR; }
		| AND		{ $$ = SCell::AND; }
		| NOT 		{ $$ = SCell::NOT; }
		| NAND 		{ $$ = SCell::NAND; }
		| NOR 		{ $$ = SCell::NOR; }
		| XOR 		{ $$ = SCell::XOR; }
		| XNOR 		{ $$ = SCell::XNOR; }
		| ADD 		{ $$ = SCell::ADD; }
		| SUB 		{ $$ = SCell::SUB; }
		| MUL 		{ $$ = SCell::MUL; }
		| DIV 		{ $$ = SCell::DIV; }
		| MUX 		{ $$ = SCell::MUX; }
		| DEC 		{ $$ = SCell::DEC; }
		| ENC 		{ $$ = SCell::ENC; }
		| VCC 		{ $$ = SCell::VCC; }
		| GND 		{ $$ = SCell::GND; }
		| FF		{ $$ = SCell::FF; }
		| RFF 		{ $$ = SCell::RFF; }
		| SFF 		{ $$ = SCell::SFF; }
		| RSFF 		{ $$ = SCell::RSFF; }
		| SRFF 		{ $$ = SCell::SRFF; }
		| LD 		{ $$ = SCell::LD; }
		| RLD		{ $$ = SCell::RLD; } 
		| RSH		{ $$ = SCell::RSH; } 
		| LSH		{ $$ = SCell::LSH; } 
		| CMP		{ $$ = SCell::CMP; } 
		| INC		{ $$ = SCell::INC; }
;		

other_cell_items_opt: other_cell_items
	|
;	
other_cell_items : other_cell_item
	| other_cell_items other_cell_item
;

other_cell_item : area ';' 	{ cell->setArea($1); }
	|	delay ';'			{ cell->setDelay($1); }
	|	attribute ';'       { attributes.push_back(*$1); delete $1; }
;	

area:	AREA ':' REAL_NUMBER { $$ = $3; }
;
delay:	DELAY ':' REAL_NUMBER { $$ = $3; }
;

attribute: 
		ATTRIBUTE ':' '(' IDENTIFIER ',' IDENTIFIER ',' IDENTIFIER ')' {
		$$ = new Attribute($4, $6, $8);
		}
	|	ATTRIBUTE ':' '(' IDENTIFIER ',' IDENTIFIER ',' NUMBER ')' {
	    $$ = new Attribute($4, $6, (void*)$8);
	    }	
	|	ATTRIBUTE ':' '(' IDENTIFIER ',' IDENTIFIER ',' REAL_NUMBER ')' {
	    //TBD if required
	    $$ = new Attribute($4, $6, (void*)((int)$8));
	    }	
;
%%
char *func_type_to_str(int type)
{
    switch(type)
	{
	    case SCell::BUF: return strdup("BUF");
	    case SCell::TRIBUF: return strdup("TRIBUF");
	    case SCell::OR: return strdup("OR");
	    case SCell::AND: return strdup("AND");
	    case SCell::NOR: return strdup("NOR");
	    case SCell::NAND: return strdup("NAND");
	    case SCell::XOR: return strdup("XOR");
	    case SCell::XNOR: return strdup("XNOR");
	    case SCell::NOT: return strdup("NOT");
	    case SCell::ADD: return strdup("ADD");
	    case SCell::SUB: return strdup("SUB");
	    case SCell::MUL: return strdup("MUL");
	    case SCell::DIV: return strdup("DIV");
	    case SCell::FF: return strdup("FF");
	    case SCell::RFF: return strdup("RFF");
	    case SCell::SFF: return strdup("SFF");
	    case SCell::RSFF: return strdup("RSFF");
	    case SCell::SRFF: return strdup("SRFF");
	    case SCell::LD: return strdup("LD");
	    case SCell::RLD: return strdup("RLD");
	    case SCell::MUX: return strdup("MUX");
	    case SCell::ENC: return strdup("ENC");
	    case SCell::DEC: return strdup("DEC");
	    case SCell::VCC: return strdup("VCC");
	    case SCell::GND: return strdup("GND");
	    case SCell::LSH: return strdup("LSH");
	    case SCell::RSH: return strdup("RSH");
	    case SCell::CMP: return strdup("CMP");
	    case SCell::INC: return strdup("INC");
	}
	return 0;
}

