
%{

# include  <iostream>
# include <string>
#include <ctype.h>
#include <stack>
#include "parsehelp.h"
#include "ifdefproc.h"
#include "parser.tab.hh"

#ifdef YY_BUF_SIZE
#undef YY_BUF_SIZE
#endif
#define YY_BUF_SIZE 2048000

extern int lexor_keyword_handler(const char*str, unsigned nstr);
extern void yyerror(char const*);
#define KEYWORD_LOOKUP lexor_keyword_handler(yytext, yyleng)
using std::cerr;
using std::endl;
using std::stack;
struct YYLTYPE
    {
        int first_line;
        char *text;
};

struct YYLTYPE yylloc;
void line_directive(){}
void line_directive2() {}
stack<YY_BUFFER_STATE> bufferStates;
void createBufferState();
void switchToPreviousBufferState();

%}

%x CCOMMENT
%x PCOMMENT
%x LCOMMENT
%x CSTRING
%s UDPTABLE
%x PPTIMESCALE
%x PPDEFAULT_NETTYPE
%x SKIPMOD
%x INCLUDE
%x TICKDEF
%x SKIPDEF
%x SKIPTIMINGCMDS 

W [ \t\b\f\r]+
ID [a-zA-Z_][a-zA-Z0-9$_]*

%%

^"#line"[ ]+\"[^\"]*\"[ ]+[0-9]+.* { line_directive(); }
^"`line"[ ]+[0-9]+[ ]+\"[^\"]*\".* { line_directive2(); }

[ \t\b\f\r] { ; }
\n { OMCreator::instance()->incLine(); }

  /* C++ style comments start with / / and run to the end of the
     current line. These are very easy to handle. */

"//".* { /*comment_enter = YY_START; BEGIN(LCOMMENT);*/ }
<LCOMMENT>.    { yymore(); }
<LCOMMENT>\n   {  /*BEGIN(comment_enter);*/ }


  /* The contents of C-style comments are ignored, like white space. */

"/*" { BEGIN(CCOMMENT); }
<CCOMMENT>.    { yymore(); }
<CCOMMENT>\n   { OMCreator::instance()->incLine();  yymore(); }
<CCOMMENT>"*/" { BEGIN(0); }

<SKIPMOD>"endmodule" { yymore(); BEGIN 0;}
<SKIPMOD>\n {OMCreator::instance()->incLine(); yymore();}
<SKIPMOD>. {yymore(); }

"(*" { return VLOG_PSTAR; }
"*)" { return VLOG_STARP; }
"<<" { return VLOG_LS; }
"<<<" { return VLOG_LS; /* Note: Functionally, <<< is the same as <<. */}
">>"  { return VLOG_RS; }
">>>" { return VLOG_RSS; }
"<=" { return VLOG_LE; }
">=" { return VLOG_GE; }
"=>" { return VLOG_EG; }
"*>" { return VLOG_SG; }
"==" { return VLOG_EQ; }
"!=" { return VLOG_NE; }
"===" { return VLOG_CEQ; }
"!==" { return VLOG_CNE; }
"||" { return VLOG_LOR; }
"&&" { return VLOG_LAND; }
"&&&" { return VLOG_TAND; }
"~|" { return VLOG_NOR; }
"~^" { return VLOG_NXOR; }
"^~" { return VLOG_NXOR; }
"~&" { return VLOG_NAND; }
"->" { return VLOG_TRIGGER; }
"+:" { return VLOG_PO_POS; }
"-:" { return VLOG_PO_NEG; }

"(*"{W}*")" { return '*'; }
"**" { return VLOG_POW;}


[}{;:\[\],()#=.@&!?<>%|^~+*/-] { return yytext[0]; }

\"            { BEGIN(CSTRING); }
<CSTRING>\\\" { yymore(); }
<CSTRING>\n   { BEGIN(0);
                yylval.text = strdup(yytext);
        return STRING; }
<CSTRING>\"   { BEGIN(0);
        yylval.text = strdup(yytext);
        yylval.text[strlen(yytext)-1] = 0;
        return STRING; }
<CSTRING>.    { yymore(); }

<UDPTABLE>\(\?0\)    { return '_'; }
<UDPTABLE>\(\?1\)    { return '+'; }
<UDPTABLE>\(\?[xX]\) { return '%'; }
<UDPTABLE>\(\?\?\)  { return '*'; }
<UDPTABLE>\(01\)    { return 'r'; }
<UDPTABLE>\(0[xX]\) { return 'Q'; }
<UDPTABLE>\(b[xX]\) { return 'q'; }
<UDPTABLE>\(b0\)    { return 'f'; /* b0 is 10|00, but only 10 is meaningful */}
<UDPTABLE>\(b1\)    { return 'r'; /* b1 is 11|01, but only 01 is meaningful */}
<UDPTABLE>\(0\?\)   { return 'P'; }
<UDPTABLE>\(10\)    { return 'f'; }
<UDPTABLE>\(1[xX]\) { return 'M'; }
<UDPTABLE>\(1\?\)   { return 'N'; }
<UDPTABLE>\([xX]0\) { return 'F'; }
<UDPTABLE>\([xX]1\) { return 'R'; }
<UDPTABLE>\([xX]\?\) { return 'B'; }
<UDPTABLE>[bB]     { return 'b'; }
<UDPTABLE>[lL]     { return 'l'; /* IVL extension */ }
<UDPTABLE>[hH]     { return 'h'; /* IVL extension */ }
<UDPTABLE>[fF]     { return 'f'; }
<UDPTABLE>[rR]     { return 'r'; }
<UDPTABLE>[xX]     { return 'x'; }
<UDPTABLE>[nN]     { return 'n'; }
<UDPTABLE>[pP]     { return 'p'; }
<UDPTABLE>[01\?\*\-] { return yytext[0]; }
"automatic" { return VLOG_automatic;}
"generate" { return VLOG_generate;}
"endgenerate" { return VLOG_endgenerate;}
"genvar" { return VLOG_genvar;}
"PATHPULSE"${ID}(${ID})* { 
          yylval.text = strdup(yytext);
          return PATHPULSE_IDENTIFIER; 
}		  
{ID} { 
        int token = KEYWORD_LOOKUP;
		if((token == VLOG_module) && OMCreator::instance()->
			getCurrFile()->isLibFile()){
			if(!skipMod()){
				return VLOG_module;
			}
		}
		else if(token == VLOG_table){
			BEGIN UDPTABLE;
			return token;
		}
		else if(token == VLOG_endtable) {
		    BEGIN 0;
			return token;
		}	
		else {
			if(token == VLOG_if) {
				handleGenIf();
			}
	    	yylval.text = strdup(yytext);		
        	return token ? token : IDENTIFIER;
		}
}


\\[^ \t\b\f\r\n]+         {
      return IDENTIFIER; }

\$([a-zA-Z0-9$_]+)        {
      if(isSystemTimingCheckCommand(yytext)) {
	      BEGIN SKIPTIMINGCMDS;
	  }
	  else 
	  {
          yylval.text = strdup(yytext);
          return SYSTEM_IDENTIFIER; 
	  }
}	  


\'[sS]?[dD][ \t]*[0-9][0-9_]*  {
    setNumString();
    return BASED_NUMBER; 
}
\'[sS]?[bB][ \t]*[0-1xzXZ_\?]+ { 
    setNumString();
    return BASED_NUMBER;
 }
\'[sS]?[oO][ \t]*[0-7xzXZ_\?]+ { 
    setNumString();
    return BASED_NUMBER;
}
\'[sS]?[hH][ \t]*[0-9a-fA-FxzXZ_\?]+ { 
    setNumString();
    return BASED_NUMBER;
}

[0-9][0-9_]* {
    setNumString();
    return DEC_NUMBER; 
}

[0-9][0-9_]*\.[0-9][0-9_]*([Ee][+-]?[0-9][0-9_]*)? {
	yylval.text = strdup(yytext);
    return REALTIME; }

[0-9][0-9_]*[Ee][+-]?[0-9][0-9_]* {
	yylval.text = strdup(yytext);
    return REALTIME; }

<INCLUDE>\"[^ \n\t\r\b\f]+\"  {
				string str(yytext);
				str = str.substr(1, str.size()-2);
                OMCreator::instance()->parseSetup(str);
                BEGIN 0;
              }
<INCLUDE>[^ \n\t\r\b\f]+  {
                OMCreator::instance()->parseSetup(yytext);
                BEGIN 0;
              }
<INCLUDE>\n   {OMCreator::instance()->incLine();}


`include {BEGIN INCLUDE;}

`default_nettype { return DEFAULT_NETTYPE;}

  /* Notice and handle the timescale directive. */

^{W}?`timescale { OMCreator::instance()->incLine(); BEGIN(PPTIMESCALE); }
<PPTIMESCALE>.* { }
<PPTIMESCALE>\n {
      BEGIN(0); }


  /* These are directives that I do not yet support. I think that IVL
     should handle these, not an external preprocessor. */

^{W}?`celldefine{W}?.*           {  }
^{W}?`delay_mode_distributed{W}?.*  {  }
^{W}?`delay_mode_unit{W}?.*      {  }
^{W}?`delay_mode_path{W}?.*      {  }
^{W}?`disable_portfaults{W}?.*   {  }
^{W}?`enable_portfaults{W}?.*    {  }
^{W}?`endcelldefine{W}?.*        {  }
^{W}?`endprotect{W}?.*           {  }
^{W}?`nosuppress_faults{W}?.*    {  }
^{W}?`nounconnected_drive{W}?.*  {  }
^{W}?`protect{W}?.*              {  }
^{W}?`resetall{W}?.*             {  }
^{W}?`suppress_faults{W}?.*      {  }
^{W}?`unconnected_drive{W}?.*    {  }
^{W}?`uselib{W}?.*               {  }


^{W}?`define{W}?.* {
    string str(yytext);
	if(!TickDefine::instance()->process(str.substr(str.find_first_of('`')+7))) {
	    yyerror("error processing `define");
	}	
  }

<SKIPTIMINGCMDS>";" { BEGIN 0; }
<SKIPTIMINGCMDS>. { }

<SKIPDEF>^{W}?`ifdef{W}.* { if(!processIfdef(0)) { BEGIN 0; } }
<SKIPDEF>^{W}?`ifndef{W}.* { if(!processIfdef(1)) { BEGIN 0; } }
<SKIPDEF>^{W}?`elsif{W}.* { if(!processIfdef(2)) { BEGIN 0; } }
<SKIPDEF>^{W}?`else { if(!processIfdef(3)) { BEGIN 0; } }
<SKIPDEF>^{W}?`endif { IfdefProc::instance()->processEndif(); BEGIN 0; }
<SKIPDEF>\n { OMCreator::instance()->incLine();; }
<SKIPDEF>. { }

^{W}?`ifdef{W}.* { processIfdef(0); }
^{W}?`ifndef{W}.* { processIfdef(1); }
^{W}?`elsif{W}.* { processIfdef(2); }
^{W}?`else { processIfdef(3); }
^{W}?`endif { IfdefProc::instance()->processEndif(); }

^`undef{W}?.* {
    processUndef();
  }


`{W} { std::cerr << yylloc.text << ":" << yylloc.first_line << ": error: "
        << "Stray tic (`) here. Perhaps you put white space" << endl;
       std::cerr << yylloc.text << ":" << yylloc.first_line << ":      : "
        << "between the tic and preprocessor directive?"
        << endl;
       //error_count += 1;
     }
`[a-zA-Z_][a-zA-Z0-9$_]* { 
    yyless(1);
	BEGIN TICKDEF;
	}	 
<TICKDEF>[a-zA-Z_][a-zA-Z0-9$_]* { 
        if(!TickDefine::instance()->isDefined(yytext)) {
		    yyerror("undefined tick variable found");
		}
		else
		{
            std::string str = TickDefine::instance()->getRepText(yytext);
			std::reverse(str.begin(), str.end());
	        for(int i = 0; i < str.size(); ++i) {
	            unput(str[i]);
	        }
		}	
		BEGIN 0;
	}	
<TICKDEF>`[a-zA-Z_][a-zA-Z0-9$_]* { 
        if(!TickDefine::instance()->isDefined(yytext+1)) {
		    yyerror("undefined tick variable found");
		}
		else
		{
            std::string str = TickDefine::instance()->getRepText(yytext+1);
			std::reverse(str.begin(), str.end());
	        for(int i = 0; i < str.size(); ++i) {
	            unput(str[i]);
	        }
		}	
	}	
<TICKDEF>. { yyless(0); BEGIN 0; }	


. {   std::cerr << yylloc.text << ":" << yylloc.first_line
       << ": error: unmatched character (";
      if (isgraph(yytext[0]))
        std::cerr << yytext[0];
     // else
//        std::cerr << "hex " << hex << (0xffU & ((unsigned) (yytext[0])));

      std::cerr << ")" << endl;
      //error_count += 1; 
    }

%%

int yywrap()
{
    MsgHandler::instance()->print(3, 
	    OMCreator::instance()->getCurrFile()->getFileTypeStr().c_str(), 
		OMCreator::instance()->getCurrFile()->getName().c_str());
	return OMCreator::instance()->parseSetup();
      //return 1;
}

bool
skipMod()
{
    yylex();
    if(SemCheck::instance()->isUndefMod(yytext))
    {
        yyless(0);
        return false;
    }

    while(strcmp(yytext, "endmodule")){
        yylex();
    }
    return true;
}

void
skipUpto(const std::vector<std::string>& tokens)
{
    bool done = false;
	do {
	    yylex();
		for(int i = 0; i < tokens.size(); ++i) 
		{
		    if(tokens[i] == std::string(yytext))
			{
			    yyless(0);
				return;
			}
		}
	} while(!done);    
}
void
handleGenIf()
{
	//if(OMCreator::instance()->getCurrGenStmt() && 
	//    !OMCreator::instance()->getCurrGenDecl()){
	//	OMCreator::instance()->createGenCond(0);
	//}
}

void
createBufferState()
{
    bufferStates.push(YY_CURRENT_BUFFER);
    yy_switch_to_buffer(yy_create_buffer(
    yyin, YY_BUF_SIZE ));
}

void
switchToPreviousBufferState() 
{
    yy_delete_buffer(YY_CURRENT_BUFFER);
    yy_switch_to_buffer(bufferStates.top());
    bufferStates.pop();
}

int
processIfdef(int what)
{
    //what: 0 ->ifdef, 1->ifndef, 2->elsif, 3->else
	int size = (what == 3) ? 5 : ((what == 0) || (what == 2)) ? 6 :  7;

    string str(yytext); 
	if(what != 3)
	{
	    str = str.substr(str.find_first_of("`"));
	    str = str.substr(size); 
	    str = str.substr(str.find_first_not_of(" \t\b\r"));
	    str = str.substr(0, str.find_first_of(" \t\b\r\n"));
	}	

	int status = (what == 0) ? IfdefProc::instance()->processIfdef(str) :
	    (what == 1) ? IfdefProc::instance()->processIfndef(str) :
		(what == 2) ? IfdefProc::instance()->processElsif(str) :
		IfdefProc::instance()->processElse();
	if(status) {
	    BEGIN SKIPDEF;
	}	
	return status;	
}

void processUndef()
{
    string str(yytext); 
    str = str.substr(str.find_first_of("`"));
    str = str.substr(6); 
    str = str.substr(str.find_first_not_of(" \t\b\r"));
    str = str.substr(0, str.find_first_of(" \t\b\r\n"));
	if(!TickDefine::instance()->undef(str)) {
	    MsgHandler::instance()->print(73, OMCreator::instance()->getLine(),
		str.c_str());
	}	
}

bool isSystemTimingCheckCommand(const char *text)
{
      if ((strcmp(text,"$setuphold") == 0) ||
          (strcmp(text,"$attribute") == 0) ||
          (strcmp(text,"$hold") == 0) || 
          (strcmp(text,"$period") == 0) ||
          (strcmp(text,"$recovery") == 0) || 
          (strcmp(text,"$recrem") == 0) ||
          (strcmp(text,"$setup") == 0) || 
          (strcmp(text,"$width") == 0) ||
          (strcmp(text,"$removal") == 0) ||
          (strcmp(text,"$skew") == 0) ||
          (strcmp(text,"$timeskew") == 0) ||
          (strcmp(text,"$fullskew") == 0) ||
          (strcmp(text,"$nochange") == 0) ) {
		  return true;
	   }
	   return false;
}

void setNumString()
{
    yylval.text = strdup(yytext);
	int len = strlen(yylval.text);
	int firstix = 0;
	for(int secondix = 0; secondix < len; ++secondix)
	{
	    //remove useless '_'
	    if(yylval.text[secondix] != '_') {
		    yylval.text[firstix++] = yylval.text[secondix];
		}	
	}
	yylval.text[firstix] = '\0';
}


