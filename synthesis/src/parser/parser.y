
%{
#include<iostream>
#include <string>
#include "parsehelp.h"
# define YYSTYPE_IS_TRIVIAL 1
# define YYLTYPE_IS_TRIVIAL 1

#define PRINT cout <<yytext <<endl;

    extern int yylex();
    extern char *yytext;
    void yyerror(char const *str) {
		//MsgHandler::instance()->print(str);
		MsgHandler::instance()->print(50, OMCreator::instance()->getLine());
    }

%}

%union {
    char *text;
    std::vector < std::pair < char *, Expr * > >*vtext;
    bool BOOL;
    int INT;
    char CHAR;
    Module *mod;
    Param *param;
    LocalParam *lparam;
    Port *port;
    Range *range;
    Expr *expr;
    Const *CONST;
    Inst *inst;
    NetType nettype;
    Dir dir;
     std::vector < char >*VCHAR;
     std::pair < int, Range * >*typerange;
     std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >*dirsigtyperange;
     std::pair < char *, Expr * >*idassp;
     std::pair < Expr *, Expr * >*assgp;
     std::pair < char *, std::vector < Range * >*>*iddim;
     std::pair < std::vector < Expr * >, GenerateItem * >*exprsgenitems;
     std::vector < std::pair < std::vector < Expr * >,
        GenerateItem * > >*exprsgenitemsv;
    ParseInfo *parseInfo;
    GateType gtype;
    NameConn *nconn;
    DelayVal *dval;
    Delay *delay;
    DrStr drstr;
    DriveSt *dst;
    Stmt *stmt;
    RegInfo *reginfo;
    CaseItem *caseitem;
    EventCntrl *evctrl;
    Defparam *defparam;
    Specparam *sparam;
    SpecRefEvent *specre;
    SpecifyPathBase *specpath;
    PolOp pop;
    UDP *udp;
    GenerateItem *genitem;
     std::vector < GenerateItem * >*genitems;
     std::pair < EdgeType, Expr * >*edexpr;
     std::vector < std::pair < EdgeType, Expr * > >*edexprs;
     std::vector < Stmt * >*stmts;
     std::vector < ParseInfo * >*parseInfos;
     std::vector < Param * >*params;
     std::vector < LocalParam * >*lparams;
     std::vector < Expr * >*exprs;
     std::vector < Inst * >*insts;
     std::vector < NameConn * >*nconns;
     std::vector < std::pair < Expr *, Expr * >*>*assgps;
     std::vector < RegInfo * >*reginfos;
     std::vector < CaseItem * >*caseitems;
     std::vector < Defparam * >*defparams;
     std::vector < Specparam * >*sparams;
     std::vector < DelayVal * >*dvals;
     std::vector < char *>*ids;
     std::vector < Range * >*ranges;
     std::vector < std::pair < char *, std::vector < Range * >*> >*iddims;
     std::map < std::string, Expr *, compare_str > *attribtable;
};

%token < text > IDENTIFIER SYSTEM_IDENTIFIER STRING
%token < text > PATHPULSE_IDENTIFIER
%token < text > BASED_NUMBER 
%token < text > DEC_NUMBER
%token < realtime > REALTIME
%token VLOG_LE VLOG_GE VLOG_EG VLOG_EQ VLOG_NE VLOG_CEQ VLOG_CNE VLOG_LS VLOG_RS VLOG_RSS VLOG_SG 
%token VLOG_PO_POS VLOG_PO_NEG 
%token VLOG_PSTAR VLOG_STARP DEFAULT_NETTYPE
%token VLOG_LOR VLOG_LAND VLOG_NAND VLOG_NOR VLOG_NXOR VLOG_TRIGGER VLOG_POW
%token VLOG_always VLOG_and VLOG_assign VLOG_begin VLOG_buf VLOG_bufif0 VLOG_bufif1 VLOG_case 
%token VLOG_casex VLOG_casez VLOG_cmos VLOG_deassign VLOG_default VLOG_defparam VLOG_disable 
%token VLOG_edge VLOG_else VLOG_end VLOG_endcase VLOG_endfunction VLOG_endmodule%token VLOG_endprimitive VLOG_endspecify VLOG_endtable VLOG_endtask VLOG_event VLOG_for VLOG_automatic 
%token VLOG_force VLOG_forever VLOG_fork VLOG_function VLOG_highz0 VLOG_highz1 VLOG_if 
%token VLOG_initial VLOG_inout VLOG_input VLOG_integer VLOG_join VLOG_large VLOG_localparam 
%token VLOG_macromodule 
%token VLOG_medium VLOG_module VLOG_nand VLOG_negedge VLOG_nmos VLOG_nor VLOG_not VLOG_notif0 
%token VLOG_notif1 VLOG_or VLOG_output VLOG_parameter VLOG_pmos VLOG_posedge VLOG_primitive 
%token VLOG_pull0 VLOG_pull1 VLOG_pulldown VLOG_pullup VLOG_rcmos VLOG_real VLOG_realtime 
%token VLOG_reg VLOG_release VLOG_repeat 
%token VLOG_rnmos VLOG_rpmos VLOG_rtran VLOG_rtranif0 VLOG_rtranif1 VLOG_scalared 
%token VLOG_signed VLOG_small VLOG_specify 
%token VLOG_specparam VLOG_strong0 VLOG_strong1 VLOG_supply0 VLOG_supply1 VLOG_table VLOG_task 
%token VLOG_time VLOG_tran VLOG_tranif0 VLOG_tranif1 VLOG_tri VLOG_tri0 VLOG_tri1 VLOG_triand 
%token VLOG_trior VLOG_trireg VLOG_vectored VLOG_wait VLOG_wand VLOG_weak0 VLOG_weak1 
%token VLOG_while VLOG_wire 
%token VLOG_wor VLOG_xnor VLOG_xor 
%token VLOG_Shold VLOG_Speriod VLOG_Srecovery VLOG_Srecrem VLOG_Ssetup VLOG_Swidth VLOG_Ssetuphold 
%token VVLOG_attribute VLOG_Sremoval VLOG_Sskew VLOG_Stimeskew VLOG_Sfullskew VLOG_Snochange
%token VLOG_TAND VLOG_generate VLOG_endgenerate VLOG_genvar 
%right '?' ':' 
%left VLOG_LOR 
%left VLOG_LAND 
%left '|' 
%left '^' VLOG_NXOR VLOG_NOR 
%left '&' VLOG_NAND 
%left VLOG_EQ VLOG_NE VLOG_CEQ VLOG_CNE 
%left VLOG_GE VLOG_LE '<' '>' 
%left VLOG_LS VLOG_RS VLOG_RSS 
%left '+' '-' 
%left '*' '/' '%' 
%left UNARY_PREC
/* to resolve dangling else ambiguity. */
%nonassoc less_than_VLOG_else
%nonassoc VLOG_else
%type < mod > module
%type < parseInfo > parameter_assign
%type < parseInfos > parameter_assign_list
%type < params > parameter_assign_decl
%type < params > module_parameter_port_list
%type < params > module_parameter_port_list_opt
%type < parseInfo > localparam_assign
%type < parseInfos > localparam_assign_list
%type < lparams > localparam_assign_decl
%type < range > range_opt
%type < range > range
%type < range > range_delay
%type < expr > expression
%type < exprs > expression_list
%type < expr > expr_primary
%type < parseInfo > port
%type < parseInfo > port_opt
%type < parseInfo > port_reference
%type < parseInfo > port_declaration
%type < parseInfos > port_reference_list
%type < parseInfos > list_of_ports
%type < parseInfos > module_port_list_opt
%type < parseInfos > list_of_port_declarations
%type < nettype > net_type
%type < nettype > net_type_opt
%type < BOOL > signed_opt
%type < vtext > udp_list_of_identifiers
%type < vtext > net_decl_assigns
%type < idassp > net_decl_assign
%type < text > identifier
%type < dir > port_type
%type < CONST > number
%type < inst > gate_instance
%type < insts > gate_instance_list
%type < gtype > gatetype
%type < nconn > parameter_value_byname
%type < nconns > parameter_value_byname_list
%type < nconns > parameter_value_opt
%type < nconn > port_name
%type < nconns > port_name_list
%type < dval > delay_value_simple
%type < dval > delay_value
%type < delay > delay1
%type < delay > delay3
%type < delay > delay3_opt
%type < dst > drive_strength
%type < dst > drive_strength_opt
%type < drstr > dr_strength0
%type < drstr > dr_strength1
%type < stmt > statement
%type < stmt > statement_opt
%type < stmts > statement_list
%type < assgp > assign
%type < expr > lavalue
%type < expr > lpvalue
%type < assgps > assign_list
%type < reginfo > register_variable
%type < reginfos > register_variable_list
%type < caseitem > case_item
%type < caseitems > case_items
%type < edexpr > event_expression
%type < edexprs > event_expression_list
%type < evctrl > event_control
%type < typerange > function_range_or_type_opt
%type < defparam > defparam_assign
%type < defparams > defparam_assign_list
%type < sparam > specparam
%type < sparams > specparam_list
%type < CHAR > spec_polarity
%type < exprs > specify_path_identifiers
%type < specre > spec_reference_event
%type < exprs > spec_notifier
%type < exprs > spec_notifier_opt
%type < pop > polarity_operator
%type < specpath > specify_edge_path
%type < specpath > specify_edge_path_decl
%type < specpath > specify_simple_path
%type < specpath > specify_simple_path_decl
%type < dvals > specify_delay_value_list
%type < ids > udp_port_list
%type < parseInfo > udp_port_decl
%type < parseInfos > udp_port_decls
%type < udp > udp_primitive
%type < expr > udp_initial_expr_opt
%type < expr > udp_initial
%type < expr > udp_init_opt
%type < CHAR > udp_input_sym
%type < CHAR > udp_output_sym
%type < VCHAR > udp_input_list
%type < ids > udp_input_declaration_list
%type < text > reg_opt
%type < ranges > dimension
%type < ranges > dimension_opt
%type < iddims > list_of_identifiers
%type < exprs > dimension_select
%type < dirsigtyperange > task_port_decl
%type < dirsigtyperange > task_port_decl_list
%type < dirsigtyperange > func_input_decls
%type < dirsigtyperange > task_func_in_port_decl
%type < dirsigtyperange > task_out_port_decl
%type < dirsigtyperange > task_inout_port_decl
%type < BOOL > auto_tf_opt
%type < idassp > attribute
%type < attribtable > attribute_list
%type < attribtable > attribute_list_opt
%type < INT > task_func_param_type
%type < exprsgenitems > genvar_case_item
%type < exprsgenitemsv > genvar_case_item_list
%type < genitem > generate_item
%type < genitem > generate_item_opt
%type < genitem > generate_case_statement
%type < genitem > generate_conditional_statement
%type < genitem > generate_loop_statement
%type < genitem > generate_block
%type < genitems > generate_item_list
%type < genitems > generate_item_list_opt
%type < text > genvar_declaration
%type < text > genvar_identifier
%type < text > generate_block_identifier 
%type < assgp > genvar_assign 
%type < gtype > pullupdown_gate 
%%
 main:source_file |;

 source_file:description | source_file description;

 number:BASED_NUMBER {
    $$ = OMCreator::instance()->createBasedNum($1);
}

|DEC_NUMBER {
    $$ = new Num(atol($1));
}

|DEC_NUMBER BASED_NUMBER {
    $$ = OMCreator::instance()->createBasedNum(string($1) + string($2));
}

;

 attribute_list_opt:VLOG_PSTAR attribute_list VLOG_STARP {
    $$ = $2;
}

|VLOG_PSTAR VLOG_STARP {
    $$ = NULL;
}

| {
    $$ = NULL;
}

;

 attribute_list:attribute_list ',' attribute {
    $$->insert(*($3));
    delete $3;
}

|attribute {
    $$ = new std::map < std::string, Expr *, compare_str > ();
    $$->insert(*($1));
	OMCreator::instance()->setCurrAttribTable($$);
    delete $1;
}

;

 attribute:IDENTIFIER {
    $$ = new std::pair < char *, Expr * >();
    $$->first = $1;
    $$->second = NULL;
}

|IDENTIFIER '=' expression {
    $$ = new std::pair < char *, Expr * >();
    $$->first = $1;
    $$->second = $3;
}

;

 block_item_decl:attribute_list_opt VLOG_reg signed_opt range register_variable_list ';'
{
    OMCreator::instance()->createBlockDecls($1, $5, $4, $3, "reg");
}

|attribute_list_opt VLOG_reg signed_opt register_variable_list ';' {
    OMCreator::instance()->createBlockDecls($1, $4, NULL, $3, "reg");
}

|attribute_list_opt VLOG_integer register_variable_list ';' {
    OMCreator::instance()->createBlockDecls($1, $3, NULL, true, "integer");
}

|VLOG_time register_variable_list ';' {
    OMCreator::instance()->createBlockDecls(NULL, $2, NULL, false, "time");
}

|VLOG_real list_of_identifiers ';' {
    OMCreator::instance()->createBlockDecls(NULL, $2, "real");
}

|VLOG_realtime list_of_identifiers ';' {
    OMCreator::instance()->createBlockDecls(NULL, $2, "realtime");
}

|VLOG_parameter parameter_assign_decl ';' {
    OMCreator::instance()->addParams($2);
}

|VLOG_localparam localparam_assign_decl ';' {
    OMCreator::instance()->addLocalParams($2);
}
|VLOG_event list_of_identifiers ';' {
    //Presently being added to module. Should be
	//added to current scope
    OMCreator::instance()->createEvents($2);
}


|attribute_list_opt VLOG_reg error ';' {
    yyerrok;
}

|attribute_list_opt VLOG_integer error ';' {
    yyerrok;
}

|VLOG_time error ';' {
    yyerrok;
}

|VLOG_real error ';' {
    yyerrok;
}

|VLOG_realtime error ';' {
    yyerrok;
}

|VLOG_parameter error ';' {
    yyerrok;
}

|VLOG_localparam error ';' {
    yyerrok;
}

;

 block_item_decls:block_item_decl | block_item_decls block_item_decl;

 block_item_decls_opt:block_item_decls |;

 case_item:expression_list ':' statement_opt {
    $$ = $1 ? new CaseItem(*$1, $3) : 0;
}

|VLOG_default ':' statement_opt {
    std::vector < Expr * >defexpr;
    defexpr.push_back(new VString("default"));
    $$ = new CaseItem(defexpr, $3);
}

|VLOG_default statement_opt {
    std::vector < Expr * >defexpr;
    defexpr.push_back(new VString("default"));
    $$ = new CaseItem(defexpr, $2);
}

|error ':' statement_opt {
    $$ = NULL;
}

;

 case_items:case_items case_item {
    $$ = $1;
    $$->push_back($2);
}

|case_item {
    $$ = new std::vector < CaseItem * >();
    $$->push_back($1);
}

;

 charge_strength:'(' VLOG_small ')' | '(' VLOG_medium ')' | '(' VLOG_large ')';

 charge_strength_opt:charge_strength |;

 defparam_assign:identifier '=' expression {
    Expr *id = OMCreator::instance()->createIdUse($1);
    if ($1) {
        $$ = new Defparam(id, $3);
    }
}

;

 defparam_assign_list:defparam_assign {
    $$ = new std::vector < Defparam * >();
    $$->push_back($1);
}

|range defparam_assign {
    $$ = new std::vector < Defparam * >();
    $2->setRange($1);
    $$->push_back($2);
}

|defparam_assign_list ',' defparam_assign {
    $$ = $1;
    $$->push_back($3);
}

;

 delay1:'#' delay_value_simple {
    $$ = new Delay($2);
}

|'#' '(' delay_value ')' {
    $$ = new Delay($3);
}

;

 delay3:'#' delay_value_simple {
    $$ = new Delay($2);
}

|'#' '(' delay_value ')' {
    $$ = new Delay($3);
}

|'#' '(' delay_value ',' delay_value ')' {
    $$ = new Delay($3, $5);
}

|'#' '(' delay_value ',' delay_value ',' delay_value ')' {
    $$ = new Delay($3, $5, $7);
}

;

 delay3_opt:delay3 {
    $$ = $1;
}

| {
    $$ = NULL;
}

;

 delay_value:expression {
    $$ = new DelayVal($1);
}

|expression ':' expression ':' expression {
    $$ = new DelayVal($1, $3, $5);
}

;

 delay_value_simple:DEC_NUMBER {
    $$ = new DelayVal(new Num(atol(yytext)));
}

|REALTIME {
    $$ = new DelayVal(new RealConst(yytext));
}

|IDENTIFIER {
    $$ = new DelayVal(OMCreator::instance()->createIdUse($1));
}

;

 description:module
    | udp_primitive
    | DEFAULT_NETTYPE net_type {OMCreator::instance()->setDefNetType($2);}
    | VVLOG_attribute '(' IDENTIFIER ',' STRING ',' STRING ')' {
}

;

 drive_strength:'(' dr_strength0 ',' dr_strength1 ')' {
    $$ = new DriveSt($2, $4);
}

|'(' dr_strength1 ',' dr_strength0 ')' {
    $$ = new DriveSt($2, $4);
}

|'(' dr_strength0 ',' VLOG_highz1 ')' {
    $$ = new DriveSt($2, DHIGHZ1);
}

|'(' dr_strength1 ',' VLOG_highz0 ')' {
    $$ = new DriveSt($2, DHIGHZ0);
}

|'(' VLOG_highz1 ',' dr_strength0 ')' {
    $$ = new DriveSt(DHIGHZ1, $4);
}

|'(' VLOG_highz0 ',' dr_strength1 ')' {
    $$ = new DriveSt(DHIGHZ0, $4);
}

;

 drive_strength_opt:drive_strength {
    $$ = $1;
}

| {
    $$ = NULL;
}

;

 dr_strength0:VLOG_supply0 {
    $$ = DSUPPLY0;
}

|VLOG_strong0 {
    $$ = DSTRONG0;
}

|VLOG_pull0 {
    $$ = DPULL0;
}

|VLOG_weak0 {
    $$ = DWEAK0;
}

;

 dr_strength1:VLOG_supply1 {
    $$ = DSUPPLY1;
}

|VLOG_strong1 {
    $$ = DSTRONG1;
}

|VLOG_pull1 {
    $$ = DPULL1;
}

|VLOG_weak1 {
    $$ = DWEAK1;
}

;

 event_control:'@' identifier {
    Expr *expr = OMCreator::instance()->createIdUse($2);
    std::vector < std::pair < EdgeType, Expr * > >edgeexprs;
    edgeexprs.push_back(std::make_pair(NOEDGE, expr));
    $$ = new EventCntrl(edgeexprs, NULL, false);
}

|'@' '(' event_expression_list ')' {
    $$ = new EventCntrl((*$3), NULL, false);
    delete $3;
}

|'@' '(' error ')' {
$$ = NULL}

;

 event_expression_list:event_expression {
    $$ = new std::vector < std::pair < EdgeType, Expr * > >();
    $$->push_back(*($1));
    delete $1;
}

|event_expression_list VLOG_or event_expression {
    $$ = $1;
    $$->push_back(*($3));
    delete $3;
}

|event_expression_list ',' event_expression {
    $$ = $1;
    $$->push_back(*($3));
    delete $3;

}

;

 event_expression:VLOG_posedge expression {
    $$ = new std::pair < EdgeType, Expr * >(POSEDGE, $2);
}

|VLOG_negedge expression {
    $$ = new std::pair < EdgeType, Expr * >(NEGEDGE, $2);
}

|expression {
    $$ = new std::pair < EdgeType, Expr * >(NOEDGE, $1);
}

;

 expression:expr_primary {
    $$ = $1;
}

|'+' expr_primary %prec UNARY_PREC {
    $$ = new UnaryExpr($2, UNPLUS);
}

|'-' expr_primary %prec UNARY_PREC {
    $$ = new UnaryExpr($2, UNMINUS);
}

|'~' expr_primary %prec UNARY_PREC {
    $$ = new UnaryExpr($2, BITNEG);
}

|'&' expr_primary %prec UNARY_PREC {
    $$ = new UnaryExpr($2, REDAND);
}

|'!' expr_primary %prec UNARY_PREC {
    $$ = new UnaryExpr($2, LOGNEG);
}

|'|' expr_primary %prec UNARY_PREC {
    $$ = new UnaryExpr($2, REDOR);
}

|'^' expr_primary %prec UNARY_PREC {
    $$ = new UnaryExpr($2, REDXOR);
}

|VLOG_NAND expr_primary %prec UNARY_PREC {
    $$ = new UnaryExpr($2, REDNAND);
}

|VLOG_NOR expr_primary %prec UNARY_PREC {
    $$ = new UnaryExpr($2, REDNOR);
}

|VLOG_NXOR expr_primary %prec UNARY_PREC {
    $$ = new UnaryExpr($2, REDXNOR);
}

|'!' error %prec UNARY_PREC {
    $$ = NULL;

}

|'^' error %prec UNARY_PREC {
    $$ = NULL;

}

|expression '^' expression {
    $$ = new BinExpr($1, $3, BITXOR);
}

|expression '*' expression {
    $$ = new BinExpr($1, $3, MULT);
}

|expression '/' expression {
    $$ = new BinExpr($1, $3, DIV);
}

|expression '%' expression {
    $$ = new BinExpr($1, $3, MOD);
}

|expression '+' expression {
    $$ = new BinExpr($1, $3, PLUS);
}

|expression '-' expression {
    $$ = new BinExpr($1, $3, MINUS);
}

|expression VLOG_POW expression {
    $$ = new BinExpr($1, $3, POW);
}
|expression '&' expression {
    $$ = new BinExpr($1, $3, BITAND);
}

|expression '|' expression {
    $$ = new BinExpr($1, $3, BITOR);
}

|expression VLOG_NAND expression {
    $$ = new BinExpr($1, $3, BITNAND);
}

|expression VLOG_NOR expression {
    $$ = new BinExpr($1, $3, BITNOR);
}

|expression VLOG_NXOR expression {
    $$ = new BinExpr($1, $3, BITNXOR);
}

|expression '<' expression {
    $$ = new BinExpr($1, $3, LTHAN);
}

|expression '>' expression {
    $$ = new BinExpr($1, $3, GTHAN);
}

|expression VLOG_LS expression {
    $$ = new BinExpr($1, $3, LSHIFT);
}

|expression VLOG_RS expression {
    $$ = new BinExpr($1, $3, RSHIFT);
}

|expression VLOG_RSS expression {
    $$ = new BinExpr($1, $3, RRSHIFT);
}

|expression VLOG_EQ expression {
    $$ = new BinExpr($1, $3, LOGEQ);
}

|expression VLOG_CEQ expression {
    $$ = new BinExpr($1, $3, CASEEQ);
}

|expression VLOG_LE expression {
    $$ = new BinExpr($1, $3, LTHANEQ);
}

|expression VLOG_GE expression {
    $$ = new BinExpr($1, $3, GTHANEQ);
}

|expression VLOG_NE expression {
    $$ = new BinExpr($1, $3, LOGENEQ);
}

|expression VLOG_CNE expression {
    $$ = new BinExpr($1, $3, CASEENEQ);
}

|expression VLOG_LOR expression {
    $$ = new BinExpr($1, $3, LOGOR);
}

|expression VLOG_LAND expression {
    $$ = new BinExpr($1, $3, LOGAND);
}

|expression '?' expression ':' expression {
    $$ = new CondExpr($1, $3, $5);
}

;

 expression_list:expression_list ',' expression {
    if ($1) {
        $$ = $1;
    } else {
        $$ = new std::vector < Expr * >();
        // for case like inst( , in);
        $$->push_back(NULL);
    }
    $$->push_back($3);
}

|expression {
    $$ = new std::vector < Expr * >();
    $$->push_back($1);
}

| {
    $$ = NULL;
}

|expression_list ',' {
    if($$ == NULL) {
        $$ = new std::vector < Expr * >();
	}
    $$->push_back(NULL);
}

;

 expr_primary:number {
    $$ = $1;
}

|REALTIME {
    $$ = new RealConst(yytext);
}

|STRING {
    $$ = new VString($1);
}

|identifier {
    $$ = OMCreator::instance()->createIdUse($1, true);
    if(!($$)){
        $$ = new ImplicitNet($1, OMCreator::instance()->getDefNetType());
    }
}

|SYSTEM_IDENTIFIER {
    $$ = new FuncCall($1, std::vector < Expr * >(), true);
}

|identifier dimension_select {
    $$ = OMCreator::instance()->createBitSelect($1, $2);
}

|identifier dimension_select '[' expression polarity_operator expression ']' {
    $$ = OMCreator::instance()->createPartSelect($1,
                             new Range($4, $6), $2, $5);
}

|identifier '[' expression polarity_operator expression ']' {
    $$ = OMCreator::instance()->createPartSelect($1,
                             new Range($3, $5), NULL,
                             $4);
}

|identifier '(' expression_list ')' {
    //TBD: resolve function call
    std::vector < Expr * >vexpr;
    if ($3) {
        vexpr = *($3);
    }
    $$ = new FuncCall($1, vexpr, false);
}

|SYSTEM_IDENTIFIER '(' expression_list ')' {
    $$ = new FuncCall($1, $3 ? *($3) : std::vector < Expr * >(), true);
}

|'(' expression ')' {
    $$ = $2;
}

|'{' expression_list '}' {
    $$ = new Concat(*$2);
    delete $2;
}

|'{' expression '{' expression_list '}' '}' {
    $$ = new MultiConcat($2, *$4);
    delete $4;
}

;
 task_func_in_port_decl:
VLOG_input reg_opt signed_opt range_opt IDENTIFIER dimension_opt {
    OMCreator::instance()->addTaskFuncPort($5, IN, $4, $6, $3, 5);
    $$ = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >(std::make_pair(IN, $3), std::make_pair(5, $4));
}

|VLOG_input function_range_or_type_opt IDENTIFIER dimension_opt {
    OMCreator::instance()->addTaskFuncPort($3, IN, $2 ? $2->second :
                           NULL, $4, false,
                           $2 ? $2->first : -1);
    $$ = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >();
    $$->first = std::make_pair(IN, false);
    if ($2) {
        $$->second = *($2);
        delete $2;
    }
}

;
 task_out_port_decl:
VLOG_output VLOG_reg signed_opt range_opt IDENTIFIER dimension_opt {
    OMCreator::instance()->addTaskFuncPort($5, OUT, $4, $6, $3, 5);
    $$ = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >();
    $$->first = std::make_pair(OUT, $3);
    $$->second = std::make_pair(5, $4);
}

|VLOG_output function_range_or_type_opt IDENTIFIER dimension_opt {
    OMCreator::instance()->addTaskFuncPort($3, OUT, $2 ? $2->second :
                           NULL, $4, false,
                           $2 ? $2->first : -1);
    $$ = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >();
    $$->first = std::make_pair(OUT, false);
    if ($2) {
        $$->second = *($2);
        delete $2;
    }
}

;
 task_inout_port_decl:
VLOG_inout VLOG_reg signed_opt range_opt IDENTIFIER dimension_opt {
    OMCreator::instance()->addTaskFuncPort($5, INOUT, $4, $6, $3, 5);
    $$ = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >();
    $$->first = std::make_pair(INOUT, $3);
    $$->second = std::make_pair(5, $4);
}

|VLOG_inout function_range_or_type_opt IDENTIFIER dimension_opt {
    OMCreator::instance()->addTaskFuncPort($3, OUT, $2 ? $2->second :
                           NULL, $4, false,
                           $2 ? $2->first : -1);
    $$ = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >();
    $$->first = std::make_pair(INOUT, false);
    if ($2) {
        $$->second = *($2);
        delete $2;
    }
}

;

 func_input_decls:task_func_in_port_decl {
    $$ = $1;
}

|func_input_decls ',' task_func_in_port_decl {
    $$ = $3;
}

|func_input_decls ',' IDENTIFIER dimension_opt {
    $$ = $1;
    OMCreator::instance()->addTaskFuncPort($3, IN, $$->second.second,
                           $4, $$->first.second,
                           $$->second.first);
}

;

 function_item:
func_input_decls ';' | block_item_decl {
}

;

 function_item_list:function_item {
}

|function_item_list function_item {
}

;

 gate_instance:IDENTIFIER '(' expression_list ')' {
    std::vector < NameConn * >*nameConns = NULL;
    if ($3) {
        nameConns = new std::vector < NameConn * >();
        for (unsigned int i = 0; i < $3->size(); ++i) {
            nameConns->push_back(new NameConn(std::string(),
                              (*$3)[i]));
        }
    }
    $$ = OMCreator::instance()->createInst($1, nameConns);
	if($$){
        $$->setAttribTable(OMCreator::instance()->getCurrAttribTable());
	}	
}

|IDENTIFIER range '(' expression_list ')' {
    std::vector < NameConn * >*nameConns = NULL;
    if ($4) {
        nameConns = new std::vector < NameConn * >();
        for (unsigned int i = 0; i < $4->size(); ++i) {
            nameConns->push_back(new NameConn(std::string(),
                              (*$4)[i]));
        }
    }
    $$ = OMCreator::instance()->createInst($1, nameConns);
    $$->setAttribTable(OMCreator::instance()->getCurrAttribTable());
}

|'(' expression_list ')' {
    std::vector < NameConn * >*nameConns = NULL;
    if ($2) {
        nameConns = new std::vector < NameConn * >();
        for (unsigned int i = 0; i < $2->size(); ++i) {
            nameConns->push_back(new NameConn(std::string(),
                              (*$2)[i]));
        }
    }
    $$ = OMCreator::instance()->
        createInst(GetGateName(OMCreator::instance()->getCurrGateType()),
               nameConns);
    $$->setAttribTable(OMCreator::instance()->getCurrAttribTable());
}


|IDENTIFIER range {
    $$ = OMCreator::instance()->createInst($1, NULL);
    $$->setAttribTable(OMCreator::instance()->getCurrAttribTable());
}


|IDENTIFIER '(' port_name_list ')' {
    $$ = OMCreator::instance()->createInst($1, $3);
}

;

 gate_instance_list:gate_instance_list ',' gate_instance {
    $$ = $1;
    $$->push_back($3);
}

|gate_instance {
    $$ = new std::vector < Inst * >();
    $$->push_back($1);
}

;

 gatetype:VLOG_and {
    $$ = AND;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_nand {
    $$ = NAND;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_or {
    $$ = OR;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_nor {
    $$ = NOR;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_xor {
    $$ = XOR;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_xnor {
    $$ = XNOR;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_buf {
    $$ = BUF;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_bufif0 {
    $$ = BUFIF0;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_bufif1 {
    $$ = BUFIF1;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_not {
    $$ = NOT;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_notif0 {
    $$ = NOTIF0;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_notif1 {
    $$ = NOTIF1;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_nmos {
    $$ = NMOS;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_rnmos {
    $$ = RNMOS;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_pmos {
    $$ = PMOS;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_rpmos {
    $$ = RPMOS;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_cmos {
    $$ = CMOS;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_rcmos {
    $$ = RCMOS;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_tran {
    $$ = TRAN;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_rtran {
    $$ = RTRAN;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_tranif0 {
    $$ = TRANIF0;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_tranif1 {
    $$ = TRANIF1;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_rtranif0 {
    $$ = RTRANIF0;
    OMCreator::instance()->setCurrGateType($$);
}

|VLOG_rtranif1 {
    $$ = RTRANIF1;
    OMCreator::instance()->setCurrGateType($$);
}

;

pullupdown_gate: VLOG_pullup {
    $$ = PULLUP;
    OMCreator::instance()->setCurrGateType($$);
}
| VLOG_pulldown {
    $$ = PULLUP;
    OMCreator::instance()->setCurrGateType($$);
}
;

  /* A general identifier is a hierarchical name, with the right most
     name the base of the identifier. This rule builds up a
     hierarchical name from left to right, forming a list of names. */
 identifier:IDENTIFIER {
    $$ = yylval.text;
}

|identifier '.' IDENTIFIER {
    $$ = (char *)malloc(strlen($1) + 2 + strlen(yytext));
    sprintf($$, "%s.%s", $1, yytext);
}

;

 list_of_identifiers:IDENTIFIER dimension_opt {
    $$ = new std::vector < std::pair < char *, std::vector < Range * >*> >();
    $$->push_back(std::make_pair($1, $2));
}

|list_of_identifiers ',' IDENTIFIER dimension_opt {
    $1->push_back(std::make_pair($3, $4));
    $$ = $1;
}

 udp_list_of_identifiers:IDENTIFIER {
    $$ = new std::vector < std::pair < char *, Expr * > >();
    $$->push_back(std::make_pair(yylval.text, (Expr *) 0));
}

|udp_list_of_identifiers ',' IDENTIFIER {
    $1->push_back(std::make_pair($3, (Expr *) 0));
    $$ = $1;
}

;

 dimension:range {
    if($1) 
	{
        $$ = new std::vector < Range * >();
        $$->push_back($1);
	}
	else {
	    $$ = 0;
	}	
}

|dimension range {
    $$ = $1;
	if($$ && $2) {
        $$->push_back($2);
	}	
}

;

 dimension_opt:dimension {
    $$ = $1;
}

| {
    $$ = NULL;
}

;

 dimension_select:'[' expression ']' {
    $$ = new std::vector < Expr * >();
    $$->push_back($2);
}

|dimension_select '[' expression ']' {
    $$ = $1;
    $$->push_back($3);
}

;

 list_of_ports:port_opt {
    $$ = new std::vector < ParseInfo * >();
	if($1) {
        $$->push_back($1);
	}	
}

|list_of_ports ',' port_opt {
    if($3) {
        $1->push_back($3);
	}	
    $$ = $1;
}

;

 list_of_port_declarations:port_declaration {
    $$ = new std::vector < ParseInfo * >();
    $$->push_back($1);
}

|list_of_port_declarations ',' port_declaration {
    $1->push_back($3);
    $$ = $1;
}

|list_of_port_declarations ',' IDENTIFIER {
    ParseInfo *tpi = new ParseInfo(*(*$1)[$1->size() - 1]);
    tpi->name = $3;
    $1->push_back(tpi);
    $$ = $1;
}

;

 port_declaration:attribute_list_opt
    VLOG_input net_type_opt signed_opt range_opt IDENTIFIER dimension_opt
{
    $$ = new ParseInfo();
    $$->name = std::string($6);
    $$->isSigned = $4;
    $$->range = $5;
    $$->dir = IN;
    $$->net = $3;
    $$->dim = $7;
    $$->table = $1;
}

|attribute_list_opt
    VLOG_inout net_type_opt signed_opt range_opt IDENTIFIER dimension_opt {
    $$ = new ParseInfo();
    $$->name = std::string($6);
    $$->isSigned = $4;
    $$->range = $5;
    $$->dir = INOUT;
    $$->net = $3;
    $$->dim = $7;
    $$->table = $1;
}

|attribute_list_opt
    VLOG_output net_type_opt signed_opt range_opt IDENTIFIER dimension_opt {
    $$ = new ParseInfo();
    $$->name = std::string($6);
    $$->isSigned = $4;
    $$->range = $5;
    $$->dir = OUT;
    $$->net = $3;
    $$->dim = $7;
    $$->table = $1;

}

|attribute_list_opt
    VLOG_output var_type signed_opt range_opt IDENTIFIER dimension_opt {
    $$ = new ParseInfo();
    $$->name = std::string($6);
    $$->isSigned = $4;
    $$->range = $5;
    $$->dir = OUT;
    $$->net = REG;
    $$->dim = $7;
    $$->table = $1;
}

|attribute_list_opt
    VLOG_output var_type signed_opt range_opt IDENTIFIER '=' expression {
    $$ = new ParseInfo();
    $$->name = std::string($6);
    $$->isSigned = $4;
    $$->range = $5;
    $$->dir = OUT;
    $$->net = REG;
    $$->exprs.push_back($8);
    $$->table = $1;
}

;

 net_type_opt:
net_type {
    $$ = $1;
}

| {
    $$ = NONET;
}

;

 signed_opt:VLOG_signed {
    $$ = true;
}

| {
    $$ = false;
}

;

 lavalue:identifier {
    $$ = OMCreator::instance()->createIdUse($1, true);
	if(!$$) {
	    $$ = new VString($1);
	}	
}

|identifier dimension_select {
    $$ = OMCreator::instance()->createBitSelect($1, $2);
}

|identifier dimension_select '[' expression polarity_operator expression ']' {
    $$ = OMCreator::instance()->createPartSelect($1, new Range($4, $6), $2,
                             $5);
}

|identifier '[' expression polarity_operator expression ']' {
    $$ = OMCreator::instance()->createPartSelect($1, new Range($3, $5),
                             NULL, $4);
}

|'{' expression_list '}' {
    $$ = new Concat(*$2);
    delete $2;
}

;

 lpvalue:identifier {
    $$ = OMCreator::instance()->createIdUse($1);
}

|identifier dimension_select {
    $$ = OMCreator::instance()->createBitSelect($1, $2);
}

|identifier dimension_select '[' expression polarity_operator expression ']' {
    $$ = OMCreator::instance()->createPartSelect($1,
                             new Range($4, $6), $2, $5);
}

|identifier '[' expression polarity_operator expression ']' {
    $$ = ($3 && $5) ? OMCreator::instance()->createPartSelect($1,
                             new Range($3, $5), NULL,
                             $4) : NULL;
}

|'{' expression_list '}' {
    $$ = new Concat(*$2);
    delete $2;
}

;

 assign:lavalue '=' expression {
    $$ = new std::pair < Expr *, Expr * >($1, $3);
}

;

 assign_list:assign_list ',' assign {
    $$ = $1;
    $$->push_back($3);
}

|assign {
    $$ = new std::vector < std::pair < Expr *, Expr * >*>();
    $$->push_back($1);
}

;


 module:attribute_list_opt module_start IDENTIFIER {
    OMCreator::instance()->createModule($1, yylval.text);
}

module_parameter_port_list_opt {
    OMCreator::instance()->addParams($5);
}

module_port_list_opt ';' {
    if ($7) {
        OMCreator::instance()->setParseInfo($7);
        OMCreator::instance()->addV2kPorts();
    }
}

module_item_list_opt VLOG_endmodule {
    OMCreator::instance()->scopeEndAction();
} {
	$$ = const_cast<Module*>(OMCreator::instance()->getCurrModule());
}	

;

 module_start:VLOG_module {
}

|VLOG_macromodule;

 range_delay:range_opt delay3_opt {
    $$ = $1;
}

;

 module_port_list_opt:'(' list_of_ports ')' {
    $$ = $2;
}

|'(' list_of_port_declarations ')' {
    $$ = $2;
}

| {
    $$ = NULL;
}

;

 module_parameter_port_list_opt:
'#' '(' module_parameter_port_list ')' {
    $$ = $3;
}

| {
    $$ = NULL;
}

;

 module_parameter_port_list:VLOG_parameter function_range_or_type_opt parameter_assign
{
    std::vector < ParseInfo * >*tpinfo = new std::vector < ParseInfo * >();
    tpinfo->push_back($3);
    $$ = (OMCreator::instance()->createParams < Param, VecParam > (tpinfo));
}

|module_parameter_port_list ',' parameter_assign {
    std::vector < ParseInfo * >*tpinfo = new std::vector < ParseInfo * >();
    tpinfo->push_back($3);
    std::vector < Param * >*tpv =
        OMCreator::instance()->createParams < Param, VecParam > (tpinfo);
    for (unsigned int i = 0; i < tpv->size(); ++i) {
        $$->push_back((*tpv)[i]);
    }
    delete tpv;
}

|module_parameter_port_list ',' VLOG_parameter parameter_assign {
    std::vector < ParseInfo * >*tpinfo = new std::vector < ParseInfo * >();
    tpinfo->push_back($4);
    std::vector < Param * >*tpv =
        OMCreator::instance()->createParams < Param, VecParam > (tpinfo);
    for (unsigned int i = 0; i < tpv->size(); ++i) {
        $$->push_back((*tpv)[i]);
    }
    delete tpv;
}

;
/*  Delays will be ignored all through. I just need a netlist representation to
    work with. Delay if ever required will come from gate library. Also ignored
    is the drive strength of the nets.
    [PA]
*/
 module_item:attribute_list_opt net_type signed_opt range_delay list_of_identifiers ';'
{
    OMCreator::instance()->addNets($1, $2, $3, $4, $5);
}

|attribute_list_opt net_type signed_opt range_delay net_decl_assigns ';' {
    OMCreator::instance()->addNets($1, $2, $3, $4, $5);
}

|attribute_list_opt net_type signed_opt drive_strength net_decl_assigns ';' {
    OMCreator::instance()->addNets($1, $2, $3, NULL, $5);
}

|VLOG_trireg charge_strength_opt range_delay list_of_identifiers ';' {
    OMCreator::instance()->addNets(NULL, TRIREG, false, $3, $4);
}

|port_type signed_opt range_delay list_of_identifiers ';' {
    OMCreator::instance()->addPorts($1, WIRE, $2, $3, $4);
}


|port_type net_type signed_opt range_opt list_of_identifiers ';' {
    OMCreator::instance()->addPorts($1, $2, $3, $4, $5);
}

|VLOG_output var_type signed_opt range_opt list_of_identifiers ';' {
    OMCreator::instance()->addPorts(OUT, REG, $3, $4, $5);
}

|module_or_generate_item | attribute_list_opt generated_instantiation
    | VLOG_input var_type signed_opt range_opt list_of_identifiers ';' {
    //yyerror("Input port can not be of reg type.");
	MsgHandler::instance()->print(48, OMCreator::instance()->getLine(), "Input");
}

|VLOG_inout var_type signed_opt range_opt list_of_identifiers ';' {
    //yyerror("Inout port can not be of reg type.");
	MsgHandler::instance()->print(48, OMCreator::instance()->getLine(), "Inout");
}

|port_type signed_opt range_delay error ';' {
    yyerrok;
}

|VLOG_specify VLOG_endspecify {
    OMCreator::instance()->createSpecifyBlock();
	OMCreator::instance()->scopeEndAction();
}

|VLOG_specify {
    OMCreator::instance()->createSpecifyBlock();
}

specify_item_list VLOG_endspecify {
	OMCreator::instance()->scopeEndAction();
} |VLOG_specify error VLOG_endspecify {
    yyerrok;
}

;

 module_or_generate_item:
block_item_decl | VLOG_defparam defparam_assign_list ';' {
    OMCreator::instance()->addDefparams($2);
}

//|VLOG_event list_of_identifiers ';' {
//    OMCreator::instance()->createEvents($2);
//}

|attribute_list_opt gatetype 

gate_instance_list ';' {
    OMCreator::instance()->OMCreator::setDelayAndDrStr($3, NULL, NULL);
    OMCreator::instance()->setCurrAttribTable(NULL);
}

|attribute_list_opt gatetype delay3 gate_instance_list ';' {
    OMCreator::instance()->OMCreator::setDelayAndDrStr($4, $3, NULL);
    OMCreator::instance()->setCurrAttribTable(NULL);
} 

|attribute_list_opt gatetype drive_strength gate_instance_list ';' {
    OMCreator::instance()->OMCreator::setDelayAndDrStr($4, NULL, $3);
    OMCreator::instance()->setCurrAttribTable(NULL);
}

|attribute_list_opt gatetype drive_strength_opt delay3 gate_instance_list ';' {
    OMCreator::instance()->OMCreator::setDelayAndDrStr($5, $4, $3);
    OMCreator::instance()->setCurrAttribTable(NULL);
}


|pullupdown_gate  
	gate_instance_list ';' {
    OMCreator::instance()->OMCreator::setDelayAndDrStr($2, NULL, NULL);
}

|pullupdown_gate '(' dr_strength1 ')' gate_instance_list ';' {
    if($1 == PULLDOWN) {
	    yyerror("");
	}
	else {
        OMCreator::instance()->OMCreator::setDelayAndDrStr($5, NULL, NULL);
	}	
}

|pullupdown_gate '(' dr_strength0 ')' gate_instance_list ';' {
    if($1 == PULLUP) {
	    yyerror("");
	}
	else {
        OMCreator::instance()->OMCreator::setDelayAndDrStr($5, NULL, NULL);
	}	
}


|IDENTIFIER {
    OMCreator::instance()->setCurrMasterName($1);
}

parameter_value_opt gate_instance_list ';' {
    if ($3) {
        const int size = $4->size();
        for (int i = 0; i < size; ++i) {
            OMCreator::instance()->setInstParams(static_cast <
                                 ModInst *
                                 >((*$4)[i]), *$3);
    }}

}
|VLOG_assign drive_strength_opt delay3_opt assign_list ';' {
    OMCreator::instance()->createContAssigns($2, $3, $4);
}


|attribute_list_opt VLOG_always statement {
    OMCreator::instance()->createAlwaysBlock($1, $3);
}

|attribute_list_opt VLOG_initial statement {
    OMCreator::instance()->createInitialBlock($1, $3);
}


|VLOG_task auto_tf_opt IDENTIFIER ';' {
    OMCreator::instance()->createTask($3, $2);
}

task_item_list_opt statement_opt {
    OMCreator::instance()->setTaskOrFuncStmt($7);
} VLOG_endtask {
    OMCreator::instance()->scopeEndAction();
}

|VLOG_task auto_tf_opt IDENTIFIER {
    OMCreator::instance()->createTask($3, $2);
}

'(' task_port_decl_list ')' ';' block_item_decls_opt statement_opt {
    OMCreator::instance()->setTaskOrFuncStmt($10);
}

VLOG_endtask {
    OMCreator::instance()->scopeEndAction();
}


|VLOG_function auto_tf_opt signed_opt function_range_or_type_opt IDENTIFIER ';' {
    OMCreator::instance()->createFunc($5, $2, $3, $4);
}

function_item_list statement VLOG_endfunction {
    OMCreator::instance()->setTaskOrFuncStmt($9);
    OMCreator::instance()->scopeEndAction();
} |VLOG_function auto_tf_opt signed_opt function_range_or_type_opt IDENTIFIER {
    OMCreator::instance()->createFunc($5, $2, $3, $4);
}

'(' func_input_decls ')' ';' block_item_decls_opt statement VLOG_endfunction {
    OMCreator::instance()->setTaskOrFuncStmt($12);
    OMCreator::instance()->scopeEndAction();
}

|genvar_declaration {
    OMCreator::instance()->createGenvar($1);
}


|error ';' {
    yyerrok;
}

|VLOG_assign error '=' expression ';' {
    yyerrok;
}

|VLOG_assign error ';' {
    yyerrok;
}

|VLOG_function error VLOG_endfunction {
    yyerrok;
}

|VVLOG_attribute '(' IDENTIFIER ',' STRING ',' STRING ')' ';' {
}

|VVLOG_attribute '(' error ')' ';' {
}

;

 generated_instantiation:
VLOG_generate {
    OMCreator::instance()->createGenStmt();
}

generate_item_list_opt {
    OMCreator::instance()->setGenerateItems($3);
}

VLOG_endgenerate {
    OMCreator::instance()->scopeEndAction();
}

;
 generate_item_list_opt:
generate_item_list {
    $$ = $1;
}

| {
    $$ = NULL;
}

;
 generate_item_list:
generate_item {
    $$ = new std::vector < GenerateItem * >();
    $$->push_back($1);
}

|generate_item_list generate_item {
    $$ = $1;
    if (std::find($$->begin(), $$->end(), $2) == $$->end()) {
        $$->push_back($2);
    }
}

;

 generate_decl_item:attribute_list_opt net_type signed_opt range_delay list_of_identifiers ';'
{
    OMCreator::instance()->addNets($1, $2, $3, $4, $5);
}

|attribute_list_opt net_type signed_opt range_delay net_decl_assigns ';' {
    OMCreator::instance()->addNets($1, $2, $3, $4, $5);
}

|attribute_list_opt net_type signed_opt drive_strength net_decl_assigns ';' {
    OMCreator::instance()->addNets($1, $2, $3, NULL, $5);
}

|VLOG_trireg charge_strength_opt range_delay list_of_identifiers ';' {
    OMCreator::instance()->addNets(NULL, TRIREG, false, $3, $4);
}

|module_or_generate_item {
}

;

 generate_item_opt:generate_item {
    $$ = $1;
}

| {
    $$ = NULL;
}

;

 generate_item:
generate_case_statement {
    $$ = $1;
}

|generate_conditional_statement {
    $$ = $1;
}

|generate_loop_statement {
    $$ = $1;
    OMCreator::instance()->removeGenItem();
}

|generate_block {
    $$ = $1;
}

|generate_decl_item {
    $$ = OMCreator::instance()->getCurrGenItem();
}

;
 generate_if_clause: VLOG_if '('  expression ')' {
	OMCreator::instance()->createGenCond($3);
}
;
 generate_conditional_statement: generate_if_clause
     generate_item %prec less_than_VLOG_else {
    OMCreator::instance()->removeGenItem(true);
    GenerateCond *gc =
        dynamic_cast <
        GenerateCond * >(OMCreator::instance()->getCurrGenItem());
    $$ = gc;
	if(gc) {
        gc->ifpart = $2;
	}	
}

|generate_if_clause generate_item VLOG_else {
    OMCreator::instance()->removeGenItem(true);
}

generate_item {
    OMCreator::instance()->removeGenItem(true);
    GenerateCond *gc =
        dynamic_cast <
        GenerateCond * >(OMCreator::instance()->getCurrGenItem());
    $$ = gc;
	if(gc)
	{
        gc->ifpart = $2;
        gc->elsepart = $5;
	}	
}

 |generate_if_clause error { $$ = NULL;}
 

;
 generate_case_statement:
VLOG_case '(' expression ')' {
    OMCreator::instance()->createGenCase($3);
}

genvar_case_item_list VLOG_endcase {
    const int size = $6->size();
    OMCreator::instance()->removeGenItem(true);
    GenerateCase *gc =
        dynamic_cast <
        GenerateCase * >(OMCreator::instance()->getCurrGenItem());
    for (int i = 0; i < size; ++i) {
        if ((*($6))[i].first.empty()) {
            gc->defstmt = (*($6))[i].second;
        } else {
            gc->caseitems.push_back((*($6))[i]);
        }
    }
    $$ = gc;
}

;

 genvar_declaration:
VLOG_genvar IDENTIFIER ';' {
    $$ = $2;
}

;
 genvar_case_item_list:
genvar_case_item {
    $$ = new std::vector < std::pair < std::vector < Expr * >,
        GenerateItem * > >();
    $$->push_back(*($1));
    delete $1;
}

|genvar_case_item_list genvar_case_item {
    $$->push_back(*($2));
    delete $2;
}

;

 genvar_case_item:
expression_list ':' generate_item_opt {
    $$ = new std::pair < std::vector < Expr * >, GenerateItem * >();
    $$->first = *($1);
    $$->second = $3;
    if ($3) {
        OMCreator::instance()->removeGenItem(true);
    }
}

|VLOG_default ':' generate_item {
    $$ = new std::pair < std::vector < Expr * >, GenerateItem * >();
    $$->second = $3;
    OMCreator::instance()->removeGenItem(true);
}

|VLOG_default generate_item {
    $$ = new std::pair < std::vector < Expr * >, GenerateItem * >();
    $$->second = $2;
    OMCreator::instance()->removeGenItem(true);
}

;

 generate_loop_statement:
VLOG_for '(' genvar_assign ';' expression ';' genvar_assign ')' {
    OMCreator::instance()->createGenLoop($3->first, $3->second,
                         $5, $7->second);
}

generate_item {
    OMCreator::instance()->removeGenItem(true);
    GenerateLoop *gl =
        dynamic_cast <
        GenerateLoop * >(OMCreator::instance()->getCurrGenItem());
	if(gl) {
        gl->gitem = $10;
	}	
    $$ = gl;
	OMCreator::instance()->scopeEndAction();
}

;
 genvar_identifier:IDENTIFIER {
    $$ = $1;
}

;
 generate_block_identifier:IDENTIFIER {
    $$ = $1;
}

 genvar_assign:
genvar_identifier '=' expression {
    $$ = new std::pair < Expr *, Expr * >();
    $$->first = OMCreator::instance()->createIdUse($1);
    $$->second = $3;
}

;

 generate_block:
VLOG_begin ':' generate_block_identifier {
    OMCreator::instance()->createGenBlock($3);
}

generate_item_list VLOG_end {
    OMCreator::instance()->removeGenItem(true);
    GenerateBlock *gb =
        dynamic_cast <
        GenerateBlock * >(OMCreator::instance()->getCurrGenItem());
	if(gb){	
        gb->gitems = *($5);
	}	
     OMCreator::instance()->removeGenItem();
     $$ = gb;
	 OMCreator::instance()->scopeEndAction();
} |VLOG_begin {
    OMCreator::instance()->createGenBlock("");
}

generate_item_list VLOG_end {
    OMCreator::instance()->removeGenItem(true);
    GenerateBlock *gb =
        dynamic_cast <
        GenerateBlock * >(OMCreator::instance()->getCurrGenItem());
	if(gb){	
        gb->gitems = *($3);
	}	
     OMCreator::instance()->removeGenItem();
     $$ = gb;
	 OMCreator::instance()->scopeEndAction();
};

 module_item_list:module_item_list module_item | module_item;

 module_item_list_opt:module_item_list |;


 net_decl_assign:IDENTIFIER '=' expression {
    $$ = new std::pair < char *, Expr * >($1, $3);
}

;

 net_decl_assigns:net_decl_assigns ',' net_decl_assign {
    $1->push_back(*$3);
    $$ = $1;
    delete $3;
}

|net_decl_assign {
    $$ = new std::vector < std::pair < char *, Expr * > >();
    $$->push_back(*$1);
    delete $1;
}

;

 net_type:VLOG_wire {
    $$ = WIRE;
}

|VLOG_tri {
    $$ = TRI;
}

|VLOG_tri1 {
    $$ = TRI1;
}

|VLOG_supply0 {
    $$ = SUPPLY0;
}

|VLOG_wand {
    $$ = WAND;
}

|VLOG_triand {
    $$ = TRIAND;
}

|VLOG_tri0 {
    $$ = TRI0;
}

|VLOG_supply1 {
    $$ = SUPPLY1;
}

|VLOG_wor {
    $$ = WOR;
}

|VLOG_trior {
    $$ = TRIOR;
}

;

 var_type:VLOG_reg {
}

;

 parameter_assign:IDENTIFIER '=' expression {
    $$ = new ParseInfo();
    $$->name = string($1);
    $$->exprs.push_back($3);
}

;

 parameter_assign_decl:parameter_assign_list {
    $$ = OMCreator::instance()->createParams < Param, VecParam > ($1);
}

|range parameter_assign_list {
    (*$2)[0]->range = $1;
    (*$2)[0]->isSigned = false;
    $$ = $1 ? OMCreator::instance()->createVecParams($2) : 0;
}

|VLOG_signed range parameter_assign_list {
    (*$3)[0]->range = $2;
    (*$3)[0]->isSigned = true;
    $$ = $2 ? OMCreator::instance()->createVecParams($3) : 0;
}

|task_func_param_type parameter_assign_list {
    (*$2)[0]->isSigned = false;
    $$ = OMCreator::instance()->createParams < Param, VecParam > ($2, $1);
}

;

 parameter_assign_list:parameter_assign {
    $$ = new std::vector < ParseInfo * >();
    $$->push_back($1);
}

|parameter_assign_list ',' parameter_assign {
    $1->push_back($3);
}

;


 localparam_assign:IDENTIFIER '=' expression {
    $$ = new ParseInfo();
    $$->name = string($1);
    $$->exprs.push_back($3);
}

;

 localparam_assign_decl:localparam_assign_list {
    $$ = OMCreator::instance()->createParams < LocalParam, VecLocalParam > ($1);
}

|range localparam_assign_list {
    (*$2)[0]->range = $1;
    $$ = OMCreator::instance()->createVecLocalParams($2);
}

|VLOG_signed range localparam_assign_list {
    (*$3)[0]->range = $2;
    (*$3)[0]->isSigned = true;
    $$ = OMCreator::instance()->createVecLocalParams($3);
}

|task_func_param_type localparam_assign_list {
    $$ = OMCreator::instance()->createParams < LocalParam, VecLocalParam > ($2, $1);
}

;

 localparam_assign_list:localparam_assign {
    $$ = new std::vector < ParseInfo * >();
    $$->push_back($1);
}

|localparam_assign_list ',' localparam_assign {
    $1->push_back($3);
}

;


 parameter_value_opt:'#' '(' expression_list ')' {
    $$ = new std::vector < NameConn * >();
    const int size = $3 ? $3->size() : 0;
    for (int i = 0; i < size; ++i) 
	{
	    Expr *expr = (*$3)[i] ? OMCreator::instance()->createIdUse((*$3)[i]->toString(), true) : 0;
		if(expr && !expr->isConstExpr()) 
		{
		    MsgHandler::instance()->print(47, OMCreator::instance()->getLine());
			continue;
		}	
        $$->push_back(new NameConn(std::string(),
		(*$3)[i]));
    }
}

|'#' '(' parameter_value_byname_list ')' {
    $$ = $3;
}

|'#' DEC_NUMBER {
    $$ = new std::vector < NameConn * >();
    $$->push_back(new NameConn(std::string(), new Num(atol(yytext))));
}

|'#' error {
    $$ = NULL;
}

| {
    $$ = NULL;
}

;

 parameter_value_byname:'.' IDENTIFIER '(' expression ')' {
    $$ = new NameConn($2, $4);
}

|'.' IDENTIFIER '(' ')' {
    $$ = new NameConn($2, NULL);
}

;

 parameter_value_byname_list:parameter_value_byname {
    $$ = new std::vector < NameConn * >();
    $$->push_back($1);
}

|parameter_value_byname_list ',' parameter_value_byname {
    $$ = $1;
    $$->push_back($3);

}

;


 port:port_reference {
    $$ = $1;
}


|'.' IDENTIFIER '(' port_reference ')' {
    MsgHandler::instance()->print(52, OMCreator::instance()->getLine(), "complex port declaration");
	return 1;
    //$$ = $4;
}


|'{' port_reference_list '}' {
    MsgHandler::instance()->print(52, OMCreator::instance()->getLine(), "complex port declaration");
	return 1;
    //$$ = NULL;
}


|'.' IDENTIFIER '(' '{' port_reference_list '}' ')' {
    MsgHandler::instance()->print(52, OMCreator::instance()->getLine(), "complex port declaration");
	return 1;
    //$$ = NULL;
}

;

 port_opt:port {
    $$ = $1;
}

| {
    $$ = NULL;
}

;

 port_reference:IDENTIFIER {
    $$ = new ParseInfo();
    $$->name = std::string($1);
    $$->range = NULL;

}

|IDENTIFIER '[' expression ':' expression ']' {
    Range *range = new Range($3, $5);
    $$ = new ParseInfo();
    $$->name = std::string($1);
    $$->range = range;
}

|IDENTIFIER '[' expression ']' {
    $$ = NULL;
}

|IDENTIFIER '[' error ']' {
    $$ = NULL;
}

;

 port_reference_list:port_reference {
    $$ = new std::vector < ParseInfo * >();
    $$->push_back($1);
}

|port_reference_list ',' port_reference {
    $1->push_back($3);
    $$ = $1;
}

;


 port_name:'.' IDENTIFIER '(' expression ')' {
    $$ = new NameConn($2, $4);
}

|'.' IDENTIFIER '(' error ')' {
$$ = NULL}

|'.' IDENTIFIER '(' ')' {
    $$ = new NameConn($2, NULL);
}

;

 port_name_list:port_name_list ',' port_name {
    $$ = $1;
    $$->push_back($3);
}

|port_name {
    $$ = new std::vector < NameConn * >();
    $$->push_back($1);
}

;

 port_type:VLOG_input {
    $$ = IN;
}

|VLOG_output {
    $$ = OUT;
}

|VLOG_inout {
$$ = INOUT}

;

 range:'[' expression ':' expression ']' {
    //$$ = new Range($2, $4);
	$$ = ($2 && $4) ? OMCreator::instance()->createRange($2, $4) : 0;
}

;

 range_opt:range {
    $$ = $1;
}

| {
    $$ = NULL;
}

;

  /* This is used to express the return type of a function. */
 function_range_or_type_opt:range {
    $$ = new std::pair < int, Range * >(0, $1);
}

|VLOG_integer {
    $$ = new std::pair < int, Range * >(1, NULL);
}

|VLOG_real {
    $$ = new std::pair < int, Range * >(2, NULL);
}

|VLOG_realtime {
    $$ = new std::pair < int, Range * >(3, NULL);
}

|VLOG_time {
    $$ = new std::pair < int, Range * >(4, NULL);
}

| {
    $$ = NULL;
}

;

 task_func_param_type:VLOG_integer {
    $$ = 1;
}

|VLOG_real {
    $$ = 2;
}

|VLOG_realtime {
    $$ = 3;
}

|VLOG_time {
    $$ = 4;
}

;
 register_variable:IDENTIFIER {
    $$ = new RegInfo($1);
}

|IDENTIFIER '=' expression {
    $$ = new RegInfo($1, $3);
}

|IDENTIFIER dimension {
    $$ = new RegInfo($1, NULL, $2);
}

;

 register_variable_list:register_variable {
    $$ = new std::vector < RegInfo * >();
    $$->push_back($1);
}

|register_variable_list ',' register_variable {
    $$ = $1;
    $$->push_back($3);
}

;

 specify_item:VLOG_specparam specparam_list ';' {
    OMCreator::instance()->addSpecparamDecl($2);
}

|specify_simple_path_decl ';' {
    OMCreator::instance()->addSpecPath($1);
}

|specify_edge_path_decl ';' {
    OMCreator::instance()->addSpecPath($1);
}

|VLOG_if '(' expression ')' specify_simple_path_decl ';' {
    OMCreator::instance()->addSpecPath($5);
	if($5){
        $5->setIfExpr($3);
	}	
}

|VLOG_if '(' expression ')' specify_edge_path_decl ';' {
    OMCreator::instance()->addSpecPath($5);
	if($5) {
        $5->setIfExpr($3);
	}	
}

|VLOG_Shold '(' spec_reference_event ',' spec_reference_event
    ',' delay_value spec_notifier_opt ')' ';' {
    //OMCreator::instance()->createSpecTask("$hold", $3, $5, $7, NULL, 
	//    $8 ? *($8) : std::vector<Expr*>());
}

|VLOG_Speriod '(' spec_reference_event ',' delay_value spec_notifier_opt ')' ';' {
}

|VLOG_Srecovery '(' spec_reference_event ',' spec_reference_event
    ',' delay_value spec_notifier_opt ')' ';' {
}

|VLOG_Ssetup '(' spec_reference_event ',' spec_reference_event
    ',' delay_value spec_notifier_opt ')' ';' {
}

|VLOG_Ssetuphold '(' spec_reference_event ',' spec_reference_event
    ',' delay_value ',' delay_value spec_notifier_opt ')' ';' {
}

|VLOG_Srecrem '(' spec_reference_event ',' spec_reference_event
    ',' delay_value ',' delay_value spec_notifier_opt ')' ';' {
}

|VLOG_Swidth '(' spec_reference_event ',' delay_value ',' expression
    spec_notifier_opt ')' ';' {
}

|VLOG_Swidth '(' spec_reference_event ',' delay_value ')' ';' {
}
|VLOG_Sremoval '(' spec_reference_event ',' delay_value ')' ';' {
}
|VLOG_Sskew '(' spec_reference_event ',' delay_value ')' ';' {
}
|VLOG_Stimeskew '(' spec_reference_event ',' delay_value ')' ';' {
}
|VLOG_Sfullskew '(' spec_reference_event ',' delay_value ')' ';' {
}
|VLOG_Snochange '(' spec_reference_event ',' delay_value ')' ';' {
}

;

 specify_delay_value_list:delay_value {
    $$ = new std::vector < DelayVal * >();
    $$->push_back($1);
}

|specify_delay_value_list ',' delay_value {
    $$ = $1;
    $$->push_back($3);
}

;

 specify_item_list:specify_item | specify_item_list specify_item;

 specify_edge_path_decl:specify_edge_path '=' '(' specify_delay_value_list ')' {
    $$ = $1;
    if ($4) {
        $$->setDelays(*($4));
        delete $4;
    }
}

|specify_edge_path '=' delay_value_simple {
    $$ = $1;
    std::vector < DelayVal * >d;
    d.push_back($3);
    $$->setDelays(d);
}

;

 specify_edge_path:'(' VLOG_posedge specify_path_identifiers spec_polarity VLOG_EG IDENTIFIER ')'
{
    $$ = OMCreator::instance()->createSpecifyEdgePath(POSEDGE, $3, $4,
                              PCONN, $6);
}

|'(' VLOG_posedge specify_path_identifiers spec_polarity VLOG_EG '('
    expr_primary polarity_operator expression ')' ')' {
    $$ = OMCreator::instance()->createSpecifyEdgePath(POSEDGE, $3, $4,
                              PCONN,
                              std::string(""), $7,
                              $8, $9);
}

|'(' VLOG_posedge specify_path_identifiers spec_polarity VLOG_SG IDENTIFIER ')' {
    $$ = OMCreator::instance()->createSpecifyEdgePath(POSEDGE, $3, $4,
                              FCONN, $6);
}

|'(' VLOG_posedge specify_path_identifiers spec_polarity VLOG_SG '('
    expr_primary polarity_operator expression ')' ')' {
    $$ = OMCreator::instance()->createSpecifyEdgePath(POSEDGE, $3, $4,
                              FCONN,
                              std::string(""), $7,
                              $8, $9);
}

|'(' VLOG_negedge specify_path_identifiers spec_polarity VLOG_EG IDENTIFIER ')' {
    $$ = OMCreator::instance()->createSpecifyEdgePath(NEGEDGE, $3, $4,
                              PCONN, $6);
}

|'(' VLOG_negedge specify_path_identifiers spec_polarity VLOG_EG '('
    expr_primary polarity_operator expression ')' ')' {
    $$ = OMCreator::instance()->createSpecifyEdgePath(NEGEDGE, $3, $4,
                              PCONN,
                              std::string(""), $7,
                              $8, $9);
}

|'(' VLOG_negedge specify_path_identifiers spec_polarity VLOG_SG IDENTIFIER ')' {
    $$ = OMCreator::instance()->createSpecifyEdgePath(NEGEDGE, $3, $4,
                              FCONN, $6);
}

|'(' VLOG_negedge specify_path_identifiers spec_polarity VLOG_SG '('
    expr_primary polarity_operator expression ')' ')' {
    $$ = OMCreator::instance()->createSpecifyEdgePath(NEGEDGE, $3, $4,
                              FCONN,
                              std::string(""), $7,
                              $8, $9);
}

;

 polarity_operator:VLOG_PO_POS {
    $$ = POPOS;
}

|VLOG_PO_NEG {
    $$ = PONEG;
}

|':' {
    $$ = POCOL;
}

;

 specify_simple_path_decl:specify_simple_path '=' '(' specify_delay_value_list ')'
{
    $$ = $1;
    if ($$ && $4) {
        $$->setDelays(*($4));
    }
}

|specify_simple_path '=' delay_value_simple {
    $$ = $1;
    if ($$ && $3) {
        std::vector < DelayVal * >dv;
        dv.push_back($3);
        $$->setDelays(dv);
    }
}

|specify_simple_path '=' '(' error ')' {
    $$ = NULL;
    yyerrok;
}

;

 specify_simple_path:'(' specify_path_identifiers spec_polarity
    VLOG_EG specify_path_identifiers ')' {
    if ($2 && $5) {
        $$ = new SpecifyPath($3, PCONN, *($2), *($5));
        delete $2;
        delete $5;
    } else {
        $$ = NULL;
    }
}

|'(' specify_path_identifiers spec_polarity VLOG_SG specify_path_identifiers ')' {
    if ($2 && $5) {
        $$ = new SpecifyPath($3, FCONN, *($2), *($5));
        delete $2;
        delete $5;
    } else {
        $$ = NULL;
    }

}

|'(' error ')' {
    $$ = NULL;
    yyerrok;
}

;

 specify_path_identifiers:IDENTIFIER {
    $$ = new std::vector < Expr * >();
    Expr *expr = OMCreator::instance()->createIdUse($1);
    if (expr) {
        $$->push_back(expr);
    }
}

|IDENTIFIER dimension_select {
    $$ = new std::vector < Expr * >();
    $$->push_back(OMCreator::instance()->createBitSelect($1, $2));
}

|specify_path_identifiers ',' IDENTIFIER {
    $$ = $1;
    Expr *expr = OMCreator::instance()->createIdUse($3);
    if (expr) {
        $$->push_back(expr);
    }
}

|specify_path_identifiers ',' IDENTIFIER dimension_select {
    Expr *bitsel = OMCreator::instance()->createBitSelect($3, $4);
    $$ = $1;
    $$->push_back(bitsel);
}

;

 specparam:IDENTIFIER '=' expression {
    std::vector < Expr * >exprv;
    exprv.push_back($3);
    std::string name($1);
    $$ = new Specparam(name, exprv);
}

|IDENTIFIER '=' expression ':' expression ':' expression {
    std::vector < Expr * >exprv;
    exprv.push_back($3);
    exprv.push_back($5);
    exprv.push_back($7);
    std::string name($1);
    $$ = new Specparam(name, exprv);
}

|PATHPULSE_IDENTIFIER '=' expression {
    $$ = NULL;
}

|PATHPULSE_IDENTIFIER '=' '(' expression ',' expression ')' {
    $$ = NULL;
}

;

 specparam_list:specparam {
    $$ = new std::vector < Specparam * >();
    $$->push_back($1);
}

|specparam_list ',' specparam {
    $$ = $1;
    $$->push_back($3);
}

;

 spec_polarity:'+' {
    $$ = '+';
}

|'-' {
    $$ = '-';
}

| {
    $$ = ' ';
}

;

 spec_reference_event:VLOG_posedge expression {
    $$ = new SpecRefEvent(POSEDGE, NULL, $2);
}

|VLOG_negedge expression {
    $$ = new SpecRefEvent(NEGEDGE, NULL, $2);
}

|VLOG_posedge expr_primary VLOG_TAND expression {
    $$ = new SpecRefEvent(POSEDGE, $2, $4);
}

|VLOG_negedge expr_primary VLOG_TAND expression {
    $$ = new SpecRefEvent(NEGEDGE, $2, $4);
}

|expr_primary VLOG_TAND expression {
    $$ = new SpecRefEvent(NOEDGE, $1, $3);
}

|expr_primary {
    $$ = new SpecRefEvent(NOEDGE, $1, NULL);
}

;

 spec_notifier_opt:        /* empty */
{
    $$ = NULL;
}

|spec_notifier {
    $$ = $1;
}

;
 spec_notifier:',' {
    //if (dynamic_cast < std::vector < Expr * >*>($$ - 1)) {
    //    $$ = $$ - 1;
    //} else {
    //    $$ = new std::vector < Expr * >();
    //}
    //$$->push_back(NULL);
	$$ = NULL;
}

|',' identifier {
    //if (dynamic_cast < std::vector < Expr * >*>($$ - 1)) {
    //    $$ = $$ - 1;
    ///} else {
    //    $$ = new std::vector < Expr * >();
    //}
    //$$->push_back(NULL);
    //Expr *expr = OMCreator::instance()->createIdUse($2);
    //$$->push_back(expr);
	$$ = NULL;
}

|spec_notifier ',' {
    //$$ = $1;
    //$$->push_back(NULL);
	$$ = NULL;
}

|spec_notifier ',' identifier {
    //$$ = $1;
    //Expr *expr = OMCreator::instance()->createIdUse($3);
    //$$->push_back(expr);
	$$ = NULL;
}

|IDENTIFIER {
    $$ = new std::vector < Expr * >();
    Expr *expr = OMCreator::instance()->createIdUse($1);
    $$->push_back(expr);
	$$ = NULL;
}

;

statement
 : VLOG_assign lavalue '=' expression ';' {
    $$ = new ProcContAssign($2, $4);
}

|VLOG_deassign lavalue ';' {
    $$ = new DAssign($2);
}


|VLOG_force lavalue '=' expression ';' {
    $$ = new Force($2, $4);
}

|VLOG_release lavalue ';' {
    $$ = new Release($2);
}

|VLOG_begin statement_list VLOG_end {
    Block *blk = new Block();
    const int size = $2->size();
    for (int i = 0; i < size; ++i) {
        blk->addStmt((*$2)[i]);
    }
    $$ = blk;
}

|VLOG_begin ':' IDENTIFIER {
    NamedBlockBase *nb = new NamedBlock($3);
    OMCreator::instance()->addNamedBlock(nb);
}

block_item_decls_opt statement_list {
    NamedBlock *nb =
        dynamic_cast < NamedBlock * >(const_cast <
                      Scope *
                      >(OMCreator::
                        instance()->getCurrScope()));
    const int size = $6->size();
    for (int i = 0; i < size; ++i) {
        static_cast < NamedBlock * >(nb)->addStmt((*$6)[i]);
    }
}

VLOG_end {
    $$ = dynamic_cast < NamedBlock * >(const_cast <
                       Scope *
                       >(OMCreator::
                         instance()->getCurrScope()));
    OMCreator::instance()->scopeEndAction();
}

|VLOG_begin VLOG_end {
    $$ = new Block();
}

|VLOG_begin ':' IDENTIFIER VLOG_end {
    $$ = new NamedBlock($3);
}

|VLOG_begin error VLOG_end {
    $$ = NULL;
}


|VLOG_fork ':' IDENTIFIER {
    Fork *fork = new Fork($3);
    OMCreator::instance()->addNamedBlock(fork);
}

block_item_decls_opt statement_list VLOG_join {
    const int size = $6->size();
	Fork *fork = const_cast<Fork*>(dynamic_cast<const Fork*>(
	OMCreator::instance()->getCurrScope()));
    for (int i = 0; i < size; ++i) {
        (fork)->addStmt((*$6)[i]);
    }
} {
	$$ = const_cast<Fork*>(
		dynamic_cast<const Fork*>(OMCreator::instance()->getCurrScope()));
    OMCreator::instance()->scopeEndAction();
}	

|VLOG_fork VLOG_join {
    $$ = NULL;
}

|VLOG_fork ':' IDENTIFIER VLOG_join {
    $$ = new Fork($3);
    OMCreator::instance()->addNamedBlock(static_cast <
                         NamedBlockBase * >($$));
    OMCreator::instance()->scopeEndAction();
}

|VLOG_disable identifier ';' {
    //$$ = new Disable(OMCreator::instance()->createIdUse($2));
	$$ = new Disable($2);
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_TRIGGER identifier ';' {
    $$ = new Trigger(OMCreator::instance()->createIdUse($2));
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_forever statement {
    $$ = new Forever($2);
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_fork {
    Fork *fork = new Fork("");
    OMCreator::instance()->addNamedBlock(fork);
	}
	statement_list VLOG_join {
    const int size = $3->size();
	Fork *fork = const_cast<Fork*>(dynamic_cast<const Fork*>(
	OMCreator::instance()->getCurrScope()));
    for (int i = 0; i < size; ++i) {
        (fork)->addStmt((*$3)[i]);
    }
    OMCreator::instance()->scopeEndAction();
    $$ = fork;
}

|VLOG_repeat '(' expression ')' statement {
    $$ = new Repeat($3, $5);
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_case '(' expression ')' case_items VLOG_endcase {
    $$ = new Case(*$5, $3, CASE);
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_casex '(' expression ')' case_items VLOG_endcase {
    $$ = new Case(*$5, $3, CASEX);
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_casez '(' expression ')' case_items VLOG_endcase {
    $$ = new Case(*$5, $3, CASEZ);
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_case '(' expression ')' error VLOG_endcase {
    $$ = NULL;
}

|VLOG_casex '(' expression ')' error VLOG_endcase {
    $$ = NULL;
}

|VLOG_casez '(' expression ')' error VLOG_endcase {
    $$ = NULL;
}

|VLOG_if '(' expression ')' statement_opt %prec less_than_VLOG_else {
    $$ = new If($3, $5, NULL);
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_if '(' expression ')' statement_opt VLOG_else statement_opt {
    $$ = new If($3, $5, $7);
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_if '(' error ')' statement_opt %prec less_than_VLOG_else {
    $$ = NULL;
}

|VLOG_if '(' error ')' statement_opt VLOG_else statement_opt {
    $$ = NULL;
}

|VLOG_for '(' lpvalue '=' expression ';' expression ';'
    lpvalue '=' expression ')' statement {
    $$ = new For($3, $5, $7, $11, $13);
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_for '(' lpvalue '=' expression ';' expression ';' error ')' statement {
    $$ = NULL;
}

|VLOG_for '(' lpvalue '=' expression ';' error ';'
    lpvalue '=' expression ')' statement {
    $$ = NULL;
}

|VLOG_for '(' error ')' statement {
    $$ = NULL;
}

|VLOG_while '(' expression ')' statement {
    $$ = new While($3, $5);
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_while '(' error ')' statement {
    $$ = NULL;
}

|delay1 statement_opt {
    $$ = new DelayStmt($1, $2);
	$$->setScope(OMCreator::instance()->getCurrScope());
}

|event_control statement_opt {
    if ($1) {
        $1->setStmt($2);
        $$ = $1;
	    $$->setScope(OMCreator::instance()->getCurrScope());
    } else {
        $$ = NULL;
    }
}

|'@' '*' statement_opt {
    $$ = new EventCntrl(std::vector < std::pair < EdgeType, Expr * > >(),
                $3, true);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|'@' '(' '*' ')' statement_opt {
    $$ = new EventCntrl(std::vector < std::pair < EdgeType, Expr * > >(),
                $5, true);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|lpvalue '=' expression ';' {
    $$ = new ProcAssign(NULL, NULL, $1, $3, BLOCK);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|lpvalue VLOG_LE expression ';' {
    $$ = new ProcAssign(NULL, NULL, $1, $3, NONBLOCK);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|lpvalue '=' delay1 expression ';' {
    $$ = new ProcAssign(NULL, $3, $1, $4, BLOCK);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|lpvalue VLOG_LE delay1 expression ';' {
    $$ = new ProcAssign(NULL, $3, $1, $4, NONBLOCK);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|lpvalue '=' event_control expression ';' {
    $$ = new CompAssign($1, NULL, $3, $4, BLOCK);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|lpvalue '=' VLOG_repeat '(' expression ')' event_control expression ';' {
    $$ = new CompAssign($1, $5, $7, $8, BLOCK);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|lpvalue VLOG_LE event_control expression ';' {
    $$ = new CompAssign($1, NULL, $3, $4, NONBLOCK);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|lpvalue VLOG_LE VLOG_repeat '(' expression ')' event_control expression ';' {
    $$ = new CompAssign($1, $5, $7, $8, NONBLOCK);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|VLOG_wait '(' expression ')' statement_opt {
    $$ = new Wait($3, $5);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|SYSTEM_IDENTIFIER '(' expression_list ')' ';' {
    if($3) {
        $$ = new TaskFuncCall($1, *($3), true);
        $$->setScope(OMCreator::instance()->getCurrScope());
	}
	else {
	    $$ = 0;
	}	
}

|SYSTEM_IDENTIFIER ';' {
    $$ = new TaskFuncCall($1, std::vector < Expr * >(), true);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|identifier '(' expression_list ')' ';' {
    $$ = new TaskFuncCall($1, $3 ? *($3) : std::vector< Expr* >(), false);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|identifier ';' {
    $$ = new TaskFuncCall($1, std::vector < Expr * >(), false);
    $$->setScope(OMCreator::instance()->getCurrScope());
}

|error ';' {
    $$ = NULL;
}

;

 statement_list:statement_list statement {
    $$ = $1;
	if($2) {
        $$->push_back($2);
	}	
}

|statement {
    $$ = new std::vector < Stmt * >();
	if($1) {
        $$->push_back($1);
	}	
}

;

 statement_opt:statement {
    $$ = $1;
}

|';' {
    $$ = NULL;
}

;

 task_port_decl:
task_func_in_port_decl | task_out_port_decl | task_inout_port_decl;

 task_port_decl_list:
task_port_decl | task_port_decl_list ',' task_port_decl {
    $$ = $3;
}

|task_port_decl_list ',' IDENTIFIER dimension_opt {
    $$ = $1;
    OMCreator::instance()->addTaskFuncPort($3, $$->first.first,
                           $$->second.second, $4,
                           $$->first.second,
                           $$->second.first);
}

;

 task_item:block_item_decl {
}

|task_port_decl_list ';';

 task_item_list:task_item_list task_item {
}

|task_item {
}

;

 task_item_list_opt:task_item_list {
}

| {
}

;

 auto_tf_opt:VLOG_automatic {
    $$ = true;
}

| {
    $$ = false;
}

;

 udp_body:VLOG_table {
}

udp_entry_list VLOG_endtable {
}

;

 udp_entry_list:udp_comb_entry_list | udp_sequ_entry_list;

 udp_comb_entry:udp_input_list ':' udp_output_sym ';' {
    OMCreator::instance()->addTableEntry(new TableEntry(*($1), $3));
    delete $1;
}

;

 udp_comb_entry_list:udp_comb_entry {
}

|udp_comb_entry_list udp_comb_entry {
}

;

 udp_sequ_entry_list:udp_sequ_entry {
}

|udp_sequ_entry_list udp_sequ_entry {
}

;

 udp_sequ_entry:udp_input_list ':' udp_input_sym ':' udp_output_sym ';' {
    OMCreator::instance()->addTableEntry(new TableEntry(*($1), $5, $3));
    delete $1;
}

;

 udp_initial:VLOG_initial IDENTIFIER '=' number ';' {
    $$ = $4;
}

;

 udp_init_opt:udp_initial {
    $$ = $1;
}

| {
    $$ = NULL;
}

;

 udp_input_list:udp_input_sym {
    $$ = new std::vector < char >();
    $$->push_back($1);
}

|udp_input_list udp_input_sym {
    $$ = $1;
    $$->push_back($2);
}

;

 udp_input_sym:'0' {
    $$ = '0';
}

|'1' {
    $$ = '1';
}

|'x' {
    $$ = 'x';
}

|'?' {
    $$ = '?';
}

|'b' {
    $$ = 'b';
}

|'*' {
    $$ = '*';
}

|'%' {
    $$ = '%';
}

|'f' {
    $$ = 'f';
}

|'F' {
    $$ = 'F';
}

|'l' {
    $$ = 'l';
}

|'h' {
    $$ = 'h';
}

|'B' {
    $$ = 'B';
}

|'r' {
    $$ = 'r';
}

|'R' {
    $$ = 'R';
}

|'M' {
    $$ = 'M';
}

|'n' {
    $$ = 'n';
}

|'N' {
    $$ = 'N';
}

|'p' {
    $$ = 'p';
}

|'P' {
    $$ = 'P';
}

|'Q' {
    $$ = 'Q';
}

|'q' {
    $$ = 'q';
}

|'_' {
    $$ = '-';
}

|'+' {
    $$ = '+';
}

;

 udp_output_sym:'0' {
    $$ = '0';
}

|'1' {
    $$ = '1';
}

|'x' {
    $$ = 'x';
}

|'-' {
    $$ = '-';
}

;

 udp_port_decl:VLOG_input udp_list_of_identifiers ';' {
    if ($2) {
        $$ = new ParseInfo("", IN, WIRE);
        const int size = $2->size();
        for (int i = 0; i < size; ++i) {
            $$->ids.push_back((*($2))[i].first);
        }
    } else {
        $$ = NULL;
    }

}

|VLOG_output IDENTIFIER ';' {
    $$ = new ParseInfo($2, OUT, WIRE);
}

|VLOG_reg IDENTIFIER ';' {
    $$ = new ParseInfo($2, OUT, REG);
}

|VLOG_reg VLOG_output IDENTIFIER ';' {
    $$ = new ParseInfo($3, OUT, REG);
}

;

 udp_port_decls:udp_port_decl {
    $$ = new std::vector < ParseInfo * >();
    $$->push_back($1);
}

|udp_port_decls udp_port_decl {
    $$ = $1;
    $$->push_back($2);
}

;

 udp_port_list:IDENTIFIER {
    $$ = new std::vector < char *>();
    $$->push_back($1);
}

|udp_port_list ',' IDENTIFIER {
    $$ = $1;
    $$->push_back($3);
}

;

 reg_opt:VLOG_reg {
    $$ = strdup("reg");
}

| {
    $$ = NULL;
}

;

 udp_initial_expr_opt:'=' expression {
    $$ = $2;
}

| {
    $$ = NULL;
}

;

 udp_input_declaration_list:VLOG_input IDENTIFIER {
    $$ = new std::vector < char *>();
    $$->push_back($2);
}

|udp_input_declaration_list ',' VLOG_input IDENTIFIER {
    $$ = $1;
    $$->push_back($4);
}

;

udp_primitive
 : VLOG_primitive IDENTIFIER '(' udp_port_list ')' ';' udp_port_decls {
    OMCreator::instance()->createUDP($2);
    OMCreator::instance()->createUDPPorts($7, $4);
}

udp_init_opt {
    OMCreator::instance()->setUDPInitVal($9);
}

udp_body VLOG_endprimitive {
    OMCreator::instance()->scopeEndAction();
}


|VLOG_primitive IDENTIFIER
    '(' VLOG_output reg_opt IDENTIFIER udp_initial_expr_opt ','
    udp_input_declaration_list ')' ';' {
    OMCreator::instance()->createUDP($2);
    OMCreator::instance()->createUDPPorts($6, $7, $9, $5 != 0);
}

udp_body VLOG_endprimitive {
    OMCreator::instance()->scopeEndAction();
} {
	$$ = const_cast<UDP*>(OMCreator::instance()->getCurrUdp());
}	

;
