/* 
Copyright (C) 2009-2010 Parvez Ahmad
Written by Parvez Ahmad <parvez_ahmad@yahoo.co.uk>.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* C code produced by gperf version 2.5 (GNU C++ version) */
/* Command-line: gperf -o -i 1 -C -k 1-3,$ -L C -H hash_keyword -N lookup_keyword -tT keywords_list  */
#include <string>
#include <string.h>
#include "parser.tab.hh"
#include<iostream>


#define TOTAL_KEYWORDS 103
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 12
#define MIN_HASH_VALUE 14
#define MAX_HASH_VALUE 341
/* maximum key range = 328, duplicates = 0 */
struct lexor_keyword { const char*name; int type; };
static unsigned int
hash_keyword (register char *str, register int unsigned len)
{
  static const unsigned short asso_values[] =
    {
     342, 342, 342, 342, 342, 342, 342, 342, 342, 342,
     342, 342, 342, 342, 342, 342, 342, 342, 342, 342,
     342, 342, 342, 342, 342, 342, 342, 342, 342, 342,
     342, 342, 342, 342, 342, 342, 342, 342, 342, 342,
     342, 342, 342, 342, 342, 342, 342, 342,  61,  66,
     342, 342, 342, 342, 342, 342, 342, 342, 342, 342,
     342, 342, 342, 342, 342, 342, 342, 342, 342, 342,
     342, 342, 342, 342, 342, 342, 342, 342, 342, 342,
     342, 342, 342, 342, 342, 342, 342, 342, 342, 342,
     342, 342, 342, 342, 342, 342, 342,  11,  46,  11,
      21,   1,  21,  86,   1, 111,   6,  21, 111,  36,
       1,  11,  46, 342,   1,   1,   6,  36,  51, 106,
      56,  16,   1, 342, 342, 342, 342, 342,
    };
  register int hval = len;

  switch (hval)
    {
      default:
      case 3:
        hval += asso_values[str[2]];
      case 2:
        hval += asso_values[str[1]];
      case 1:
        hval += asso_values[str[0]];
        break;
    }
  return hval + asso_values[str[len - 1]];
}

const struct lexor_keyword*
lookup_keyword ( register char *str, register unsigned int len)
{
  static const struct lexor_keyword wordlist[] =
    {
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"rtran",         VLOG_rtran},
      {"or",         VLOG_or},
      {"",}, 
      {"nor",         VLOG_nor},
      {"",}, {"",}, 
      {"assign",         VLOG_assign},
      {"",}, 
      {"realtime",     VLOG_realtime},
      {"tran",         VLOG_tran},
      {"",}, {"",}, {"",}, 
      {"not",         VLOG_not},
      {"case",         VLOG_case},
      {"casez",         VLOG_casez},
      {"",}, 
      {"endcase",     VLOG_endcase},
      {"endtable",     VLOG_endtable},
      {"endmodule",     VLOG_endmodule},
      {"",}, 
      {"endfunction",     VLOG_endfunction},
      {"endprimitive",     VLOG_endprimitive},
      {"for",         VLOG_for},
      {"nand",         VLOG_nand},
      {"force",         VLOG_force},
      {"",}, 
      {"forever",     VLOG_forever},
      {"deassign",     VLOG_deassign},
      {"task",         VLOG_task},
      {"rnmos",         VLOG_rnmos},
      {"",}, {"",}, 
      {"end",         VLOG_end},
      {"",}, 
      {"endspecify",     VLOG_endspecify},
      {"",}, 
      {"endtask",     VLOG_endtask},
      {"scalared",     VLOG_scalared},
      {"nmos",         VLOG_nmos},
      {"rcmos",         VLOG_rcmos},
      {"",}, 
      {"default",     VLOG_default},
      {"and",         VLOG_and},
      {"fork",         VLOG_fork},
      {"",}, 
      {"repeat",         VLOG_repeat},
      {"",}, {"",}, 
      {"cmos",         VLOG_cmos},
      {"event",         VLOG_event},
      {"output",         VLOG_output},
      {"posedge",     VLOG_posedge},
      {"function",     VLOG_function},
      {"parameter",     VLOG_parameter},
      {"table",         VLOG_table},
      {"macromodule",     VLOG_macromodule},
      {"specify",     VLOG_specify},
      {"xor",         VLOG_xor},
      {"xnor",         VLOG_xnor},
      {"",}, 
      {"module",         VLOG_module},
      {"strong0",     VLOG_strong0},
      {"rtranif0",     VLOG_rtranif0},
      {"",}, {"",}, {"",}, 
      {"strong1",     VLOG_strong1},
      {"rtranif1",     VLOG_rtranif1},
      {"",}, 
      {"casex",         VLOG_casex},
      {"notif0",         VLOG_notif0},
      {"tranif0",     VLOG_tranif0},
      {"defparam",     VLOG_defparam},
      {"",}, 
      {"rpmos",         VLOG_rpmos},
      {"notif1",         VLOG_notif1},
      {"tranif1",     VLOG_tranif1},
      {"vectored",     VLOG_vectored},
      {"specparam",     VLOG_specparam},
      {"",}, {"",}, 
      {"negedge",     VLOG_negedge},
      {"",}, 
      {"pmos",         VLOG_pmos},
      {"",}, 
      {"medium",         VLOG_medium},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, 
      {"edge",         VLOG_edge},
      {"",}, {"",}, {"",}, {"",}, 
      {"else",         VLOG_else},
      {"",}, {"",}, 
      {"release",     VLOG_release},
      {"wor",         VLOG_wor},
      {"",}, 
      {"trior",         VLOG_trior},
      {"",}, 
      {"integer",     VLOG_integer},
      {"buf",         VLOG_buf},
      {"real",         VLOG_real},
      {"large",         VLOG_large},
      {"",}, {"",}, {"",}, 
      {"join",         VLOG_join},
      {"inout",         VLOG_inout},
      {"",}, {"",}, {"",}, {"",}, 
      {"begin",         VLOG_begin},
      {"",}, 
      {"disable",     VLOG_disable},
      {"",}, 
      {"wand",         VLOG_wand},
      {"",}, 
      {"triand",         VLOG_triand},
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"supply0",     VLOG_supply0},
      {"",}, {"",}, {"",}, 
      {"if",         VLOG_if},
      {"supply1",     VLOG_supply1},
      {"",}, 
      {"time",         VLOG_time},
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"small",         VLOG_small},
      {"",}, {"",}, {"",}, 
      {"primitive",     VLOG_primitive},
      {"input",         VLOG_input},
      {"bufif0",         VLOG_bufif0},
      {"",}, {"",}, {"",}, {"",}, 
      {"bufif1",         VLOG_bufif1},
      {"",}, 
      {"reg",         VLOG_reg},
      {"",}, 
      {"localparam",     VLOG_localparam},
      {"",}, {"",}, {"",}, 
      {"tri0",         VLOG_tri0},
      {"weak0",         VLOG_weak0},
      {"",}, {"",}, {"",}, 
      {"tri1",         VLOG_tri1},
      {"weak1",         VLOG_weak1},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, 
      {"pulldown",     VLOG_pulldown},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"trireg",         VLOG_trireg},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, 
      {"wire",         VLOG_wire},
      {"while",         VLOG_while},
      {"signed",         VLOG_signed},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"tri",         VLOG_tri},
      {"",}, {"",}, 
      {"always",         VLOG_always},
      {"",}, {"",}, 
      {"wait",         VLOG_wait},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"pullup",         VLOG_pullup},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, 
      {"pull0",         VLOG_pull0},
      {"",}, {"",}, {"",}, {"",}, 
      {"pull1",         VLOG_pull1},
      {"highz0",         VLOG_highz0},
      {"",}, {"",}, {"",}, {"",}, 
      {"highz1",         VLOG_highz1},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"initial",     VLOG_initial},
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash_keyword (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*s == *str && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}

int lexor_keyword_handler(const char*str, unsigned nstr)
{
      const struct lexor_keyword*lk = lookup_keyword((char*)str, nstr);
      if (lk == 0)
      return IDENTIFIER;
      else
      return lk->type;
}
