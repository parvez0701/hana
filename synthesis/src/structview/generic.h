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

static const char *GenericLib = "\
Cell    BUF {\
    pin:        OUTPUT    out;\
    pin:        INPUT    in;\
    function:    BUF;\
    area:        1.0;\
    delay:        1.0;\
}\
Cell    TRIBUF {\
    pin:        OUTPUT    out;\
    pin:        INPUT    in;\
    pin:        INPUT    enable;\
    function:    TRIBUF;\
    area:        2.0;\
    delay:        1.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    INV {\
    pin:        OUTPUT    out;\
    pin:        INPUT    in;\
    function:    NOT;\
    area:        1.0;\
    delay:        1.0;\
}\
Cell    AND2 {\
    pin:        OUTPUT     out;\
    pin:        INPUT    [1:0] in;\
    function:    AND;\
    area:        2.0;\
    delay:        2.0;\
}\
Cell    AND3 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [2:0] in;\
    function:    AND;\
    area:        3.0;\
    delay:        3.0;\
}\
Cell    AND4 {\
    pin:        OUTPUT     out;\
    pin:        INPUT    [3:0] in;\
    function:    AND;\
    area:        3.0;\
    delay:        3.0;\
}\
Cell    OR2 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [1:0] in;\
    function:    OR;\
    area:        2.0;\
    delay:        2.0;\
}\
Cell    OR3 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [2:0] in;\
    function:    OR;\
    area:        3.0;\
    delay:        3.0;\
}\
Cell    OR4 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [3:0] in;\
    function:    OR;\
    area:        4.0;\
    delay:        4.0;\
}\
Cell    NAND2 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [1:0] in;\
    function:    NAND;\
    area:        2.0;\
    delay:        2.0;\
}\
Cell    NAND3 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [2:0] in;\
    function:    NAND;\
    area:        3.0;\
    delay:        3.0;\
}\
Cell    NAND4 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [3:0] in;\
    function:    NAND;\
    area:        4.0;\
    delay:        4.0;\
}\
Cell    NOR2 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [1:0] in;\
    function:    NOR;\
    area:        2.0;\
    delay:        2.0;\
}\
Cell    NOR3 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [2:0] in;\
    function:    NOR;\
    area:        3.0;\
    delay:        3.0;\
}\
Cell    NOR4 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [3:0] in;\
    function:    NOR;\
    area:        4.0;\
    delay:        4.0;\
}\
Cell    XOR2 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [1:0] in;\
    function:    XOR;\
    area:        2.0;\
    delay:        2.0;\
}\
Cell    XOR3 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [2:0] in;\
    function:    XOR;\
    area:        3.0;\
    delay:        3.0;\
}\
Cell    XOR4 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [3:0] in;\
    function:    XOR;\
    area:        4.0;\
    delay:        4.0;\
}\
Cell    XNOR2 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [1:0] in;\
    function:    XNOR;\
    area:        2.0;\
    delay:        2.0;\
}\
Cell    XNOR3 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [2:0] in;\
    function:    XNOR;\
    area:        3.0;\
    delay:        3.0;\
}\
Cell    XNOR4 {\
    pin:        OUTPUT    out;\
    pin:        INPUT    [3:0] in;\
    function:    XNOR;\
    area:        4.0;\
    delay:        4.0;\
}\
Cell    ENC2 {\
    pin:        INPUT    [1:0] in;\
    pin:        OUTPUT    out;\
    pin:        INPUT    enable;\
    function:    ENC;\
    area:        3.0;\
    delay:        3.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    ENC4 {\
    pin:        INPUT    [3:0] in;\
    pin:        OUTPUT    [1:0] out;\
    pin:        INPUT    enable;\
    function:    ENC;\
    area:        5.0;\
    delay:        5.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    ENC8 {\
    pin:        INPUT    [7:0] in;\
    pin:        OUTPUT    [2:0] out;\
    pin:        INPUT    enable;\
    function:    ENC;\
    area:        9.0;\
    delay:        9.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    ENC16 {\
    pin:        INPUT    [15:0] in;\
    pin:        OUTPUT    [3:0] out;\
    pin:        INPUT    enable;\
    function:    ENC;\
    area:        17.0;\
    delay:        17.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    ENC32 {\
    pin:        INPUT    [31:0] in;\
    pin:        OUTPUT    [4:0] out;\
    pin:        INPUT    enable;\
    function:    ENC;\
    area:        33.0;\
    delay:        33.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    ENC64 {\
    pin:        INPUT    [63:0] in;\
    pin:        OUTPUT    [5:0] out;\
    pin:        INPUT    enable;\
    function:    ENC;\
    area:        65.0;\
    delay:        65.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    DEC1 {\
    pin:        INPUT        in;\
    pin:        OUTPUT    [1:0] out;\
    pin:        INPUT    enable;\
    function:    DEC;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    DEC2 {\
    pin:        INPUT    [1:0] in;\
    pin:        OUTPUT    [3:0] out;\
    pin:        INPUT    enable;\
    function:    DEC;\
    area:        3.0;\
    delay:        1.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    DEC3 {\
    pin:        INPUT    [2:0] in;\
    pin:        OUTPUT    [7:0] out;\
    pin:        INPUT    enable;\
    function:    DEC;\
    area:        4.0;\
    delay:        4.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    DEC4 {\
    pin:        INPUT    [3:0] in;\
    pin:        OUTPUT    [15:0] out;\
    pin:        INPUT    enable;\
    function:    DEC;\
    area:        5.0;\
    delay:        5.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    DEC5 {\
    pin:        INPUT    [4:0] in;\
    pin:        OUTPUT    [31:0] out;\
    pin:        INPUT    enable;\
    function:    DEC;\
    area:        6.0;\
    delay:        6.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    DEC6 {\
    pin:        INPUT    [5:0] in;\
    pin:        OUTPUT    [63:0] out;\
    pin:        INPUT    enable;\
    function:    DEC;\
    area:        7.0;\
    delay:        7.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    MUX2 {\
    pin:        INPUT    [1:0] in;\
    pin:        OUTPUT     out;\
    pin:        INPUT    select;\
    function:    MUX;\
    area:        3.0;\
    delay:        3.0;\
    attribute:    (select, SELECT, 1);\
}\
Cell    MUX4 {\
    pin:        INPUT    [3:0] in;\
    pin:        OUTPUT     out;\
    pin:        INPUT    [1:0] select;\
    function:    MUX;\
    area:        6.0;\
    delay:        6.0;\
    attribute:    (select, SELECT, 1);\
}\
Cell    MUX8 {\
    pin:        INPUT    [7:0] in;\
    pin:        OUTPUT     out;\
    pin:        INPUT    [2:0] select;\
    function:    MUX;\
    area:        11.0;\
    delay:        11.0;\
    attribute:    (select, SELECT, 1);\
}\
Cell    MUX16 {\
    pin:        INPUT    [15:0] in;\
    pin:        OUTPUT     out;\
    pin:        INPUT    [3:0] select;\
    function:    MUX;\
    area:        20.0;\
    delay:        20.0;\
    attribute:    (select, SELECT, 1);\
}\
Cell    MUX32 {\
    pin:        INPUT    [31:0] in;\
    pin:        OUTPUT     out;\
    pin:        INPUT    [4:0] select;\
    function:    MUX;\
    area:        37.0;\
    delay:        37.0;\
    attribute:    (select, SELECT, 1);\
}\
Cell    MUX64 {\
    pin:        INPUT    [63:0] in;\
    pin:        OUTPUT     out;\
    pin:        INPUT    [5:0] select;\
    function:    MUX;\
    area:        70.0;\
    delay:        70.0;\
    attribute:    (select, SELECT, 1);\
}\
Cell    ADD1 {\
    pin:        INPUT     in1;\
    pin:        INPUT     in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     out;\
    function:    ADD;\
    area:        3.0;\
    delay:        3.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
    attribute:    (in1, DATAIN, 1);\
    attribute:    (in2, DATAIN, 1);\
    attribute:    (out, DATAOUT, 1);\
}\
Cell    ADD2 {\
    pin:        INPUT     [1:0] in1;\
    pin:        INPUT     [1:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [1:0] out;\
    function:    ADD;\
    area:        5.0;\
    delay:        5.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    ADD4 {\
    pin:        INPUT     [3:0] in1;\
    pin:        INPUT     [3:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [3:0] out;\
    function:    ADD;\
    area:        9.0;\
    delay:        9.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    ADD8 {\
    pin:        INPUT     [7:0] in1;\
    pin:        INPUT     [7:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [7:0] out;\
    function:    ADD;\
    area:        17.0;\
    delay:        17.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    ADD16 {\
    pin:        INPUT     [15:0] in1;\
    pin:        INPUT     [15:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [15:0] out;\
    function:    ADD;\
    area:        33.0;\
    delay:        33.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    ADD32 {\
    pin:        INPUT     [31:0] in1;\
    pin:        INPUT     [31:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [31:0] out;\
    function:    ADD;\
    area:        65.0;\
    delay:        65.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    ADD64 {\
    pin:        INPUT     [63:0] in1;\
    pin:        INPUT     [63:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [63:0] out;\
    function:    ADD;\
    area:        129.0;\
    delay:        129.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    SUB1 {\
    pin:        INPUT     in1;\
    pin:        INPUT     in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     out;\
    function:    SUB;\
    area:        3.0;\
    delay:        3.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    SUB2 {\
    pin:        INPUT     [1:0] in1;\
    pin:        INPUT     [1:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [1:0] out;\
    function:    SUB;\
    area:        5.0;\
    delay:        5.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    SUB4 {\
    pin:        INPUT     [3:0] in1;\
    pin:        INPUT     [3:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [3:0] out;\
    function:    SUB;\
    area:        9.0;\
    delay:        9.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    SUB8 {\
    pin:        INPUT     [7:0] in1;\
    pin:        INPUT     [7:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [7:0] out;\
    function:    SUB;\
    area:        17.0;\
    delay:        17.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    SUB16 {\
    pin:        INPUT     [15:0] in1;\
    pin:        INPUT     [15:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [15:0] out;\
    function:    SUB;\
    area:        33.0;\
    delay:        33.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    SUB32 {\
    pin:        INPUT     [31:0] in1;\
    pin:        INPUT     [31:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [31:0] out;\
    function:    SUB;\
    area:        33.0;\
    delay:        33.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    SUB64 {\
    pin:        INPUT     [63:0] in1;\
    pin:        INPUT     [63:0] in2;\
    pin:        INPUT     cin;\
    pin:        OUTPUT     cout;\
    pin:        OUTPUT     [63:0] out;\
    function:    SUB;\
    area:        129.0;\
    delay:        129.0;\
    attribute:    (cin, CIN, 1);\
    attribute:    (cout, COUT, 1);\
}\
Cell    MUL1 {\
    pin:        INPUT     in1;\
    pin:        INPUT     in2;\
    pin:        OUTPUT [1:0] out;\
    function:    MUL;\
    area:        2.0;\
    delay:        2.0;\
}\
Cell    MUL2 {\
    pin:        INPUT     [1:0] in1;\
    pin:        INPUT     [1:0] in2;\
    pin:        OUTPUT     [3:0] out;\
    function:    MUL;\
    area:        4.0;\
    delay:        4.0;\
}\
Cell    MUL4 {\
    pin:        INPUT     [3:0] in1;\
    pin:        INPUT     [3:0] in2;\
    pin:        OUTPUT     [7:0] out;\
    function:    MUL;\
    area:        8.0;\
    delay:        8.0;\
}\
Cell    MUL8 {\
    pin:        INPUT     [7:0] in1;\
    pin:        INPUT     [7:0] in2;\
    pin:        OUTPUT     [15:0] out;\
    function:    MUL;\
    area:        16.0;\
    delay:        16.0;\
}\
Cell    MUL16 {\
    pin:        INPUT     [15:0] in1;\
    pin:        INPUT     [15:0] in2;\
    pin:        OUTPUT     [31:0] out;\
    function:    MUL;\
    area:        32.0;\
    delay:        32.0;\
}\
Cell    MUL32 {\
    pin:        INPUT     [31:0] in1;\
    pin:        INPUT     [31:0] in2;\
    pin:        OUTPUT     [63:0] out;\
    function:    MUL;\
    area:        32.0;\
    delay:        32.0;\
}\
Cell    MUL64 {\
    pin:        INPUT     [63:0] in1;\
    pin:        INPUT     [63:0] in2;\
    pin:        OUTPUT     [127:0] out;\
    function:    MUL;\
    area:        64.0;\
    delay:        64.0;\
}\
Cell    DIV1 {\
    pin:        INPUT     in1;\
    pin:        INPUT     in2;\
    pin:        OUTPUT     out;\
    pin:        OUTPUT     rem;\
    function:    DIV;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in1, divisor, 1);\
    attribute:    (in2, dividend, 1);\
}\
Cell    DIV2 {\
    pin:        INPUT     [1:0] in1;\
    pin:        INPUT     [1:0] in2;\
    pin:        OUTPUT     [1:0] out;\
    pin:        OUTPUT     [1:0] rem;\
    function:    DIV;\
    area:        4.0;\
    delay:        4.0;\
    attribute:    (in1, divisor, 1);\
    attribute:    (in2, dividend, 1);\
}\
Cell    DIV4 {\
    pin:        INPUT     [3:0] in1;\
    pin:        INPUT     [3:0] in2;\
    pin:        OUTPUT     [3:0] out;\
    pin:        OUTPUT     [3:0] rem;\
    function:    DIV;\
    area:        8.0;\
    delay:        8.0;\
    attribute:    (in1, divisor, 1);\
    attribute:    (in2, dividend, 1);\
}\
Cell    DIV8 {\
    pin:        INPUT     [7:0] in1;\
    pin:        INPUT     [7:0] in2;\
    pin:        OUTPUT     [7:0] out;\
    pin:        OUTPUT     [7:0] rem;\
    function:    DIV;\
    area:        16.0;\
    delay:        16.0;\
    attribute:    (in1, divisor, 1);\
    attribute:    (in2, dividend, 1);\
}\
Cell    DIV16 {\
    pin:        INPUT     [15:0] in1;\
    pin:        INPUT     [15:0] in2;\
    pin:        OUTPUT     [15:0] out;\
    pin:        OUTPUT     [15:0] rem;\
    function:    DIV;\
    area:        32.0;\
    delay:        32.0;\
    attribute:    (in1, divisor, 1);\
    attribute:    (in2, dividend, 1);\
}\
Cell    DIV32 {\
    pin:        INPUT     [31:0] in1;\
    pin:        INPUT     [31:0] in2;\
    pin:        OUTPUT     [31:0] out;\
    pin:        OUTPUT     [31:0] rem;\
    function:    DIV;\
    area:        64.0;\
    delay:        64.0;\
    attribute:    (in1, divisor, 1);\
    attribute:    (in2, dividend, 1);\
}\
Cell    DIV64 {\
    pin:        INPUT     [63:0] in1;\
    pin:        INPUT     [63:0] in2;\
    pin:        OUTPUT     [63:0] out;\
    pin:        OUTPUT     [63:0] rem;\
    function:    DIV;\
    area:        128.0;\
    delay:        128.0;\
    attribute:    (in1, divisor, 1);\
    attribute:    (in2, dividend, 1);\
}\
Cell    FF {\
    pin:        INPUT     d;\
    pin:        INPUT     clk;\
    pin:        OUTPUT     q;\
    function:    FF;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (clk, CLOCK, 1);\
}\
Cell    RFF {\
    pin:        INPUT     d;\
    pin:        INPUT     clk;\
    pin:        INPUT     reset;\
    pin:        OUTPUT     q;\
    function:    RFF;\
    area:        3.0;\
    delay:        3.0;\
    attribute:    (clk, CLOCK, 1);\
    attribute:    (reset, RESET, 0);\
}\
Cell    SFF {\
    pin:        INPUT     d;\
    pin:        INPUT     clk;\
    pin:        INPUT     set;\
    pin:        OUTPUT     q;\
    function:    SFF;\
    area:        3.0;\
    delay:        3.0;\
    attribute:    (clk, CLOCK, 1);\
    attribute:    (set, SET, 0);\
}\
Cell    RSFF {\
    pin:        INPUT     d;\
    pin:        INPUT     clk;\
    pin:        INPUT     reset;\
    pin:        INPUT     set;\
    pin:        OUTPUT     q;\
    function:    RSFF;\
    area:        4.0;\
    delay:        4.0;\
    attribute:    (clk, CLOCK, 1);\
    attribute:    (reset, RESET, 0);\
    attribute:    (set, SET, 1);\
}\
Cell    SRFF {\
    pin:        INPUT     d;\
    pin:        INPUT     clk;\
    pin:        INPUT     reset;\
    pin:        INPUT     set;\
    pin:        OUTPUT     q;\
    function:    SRFF;\
    area:        4.0;\
    delay:        4.0;\
    attribute:    (clk, CLOCK, 1);\
    attribute:    (reset, RESET, 0);\
    attribute:    (set, SET, 1);\
}\
Cell    LATCH {\
    pin:        INPUT     d;\
    pin:        INPUT     enable;\
    pin:        OUTPUT     q;\
    function:    LD;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (enable, ENABLE, 1);\
}\
Cell    RLATCH {\
    pin:        INPUT     d;\
    pin:        INPUT     enable;\
    pin:        INPUT     reset;\
    pin:        OUTPUT     q;\
    function:    RLD;\
    area:        3.0;\
    delay:        3.0;\
    attribute:    (enable, ENABLE, 1);\
    attribute:    (reset, RESET, 0);\
}\
Cell    LSHIFT1 {\
    pin:        INPUT    in;\
    pin:        INPUT    shift;\
    pin:        OUTPUT    out;\
    pin:        INPUT    val;\
    function:    LSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    LSHIFT2 {\
    pin:        INPUT    [1:0]    in;\
    pin:        INPUT    [1:0]    shift;\
    pin:        OUTPUT    [1:0]    out;\
    pin:        INPUT    val;\
    function:    LSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    LSHIFT4 {\
    pin:        INPUT    [3:0]    in;\
    pin:        INPUT    [2:0]    shift;\
    pin:        OUTPUT    [3:0]    out;\
    pin:        INPUT    val;\
    function:    LSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    LSHIFT8 {\
    pin:        INPUT    [7:0]    in;\
    pin:        INPUT    [3:0]    shift;\
    pin:        OUTPUT    [7:0]    out;\
    pin:        INPUT    val;\
    function:    LSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    LSHIFT16 {\
    pin:        INPUT    [15:0]    in;\
    pin:        INPUT    [4:0]    shift;\
    pin:        OUTPUT    [15:0]    out;\
    pin:        INPUT    val;\
    function:    LSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    LSHIFT32 {\
    pin:        INPUT    [31:0]    in;\
    pin:        INPUT    [5:0]    shift;\
    pin:        OUTPUT    [31:0]    out;\
    pin:        INPUT    val;\
    function:    LSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    LSHIFT64 {\
    pin:        INPUT    [63:0]    in;\
    pin:        INPUT    [6:0]    shift;\
    pin:        OUTPUT    [63:0]    out;\
    pin:        INPUT    val;\
    function:    LSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    RSHIFT1 {\
    pin:        INPUT    in;\
    pin:        INPUT    shift;\
    pin:        OUTPUT    out;\
    pin:        INPUT    val;\
    function:    RSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    RSHIFT2 {\
    pin:        INPUT    [1:0]    in;\
    pin:        INPUT    [1:0]    shift;\
    pin:        OUTPUT    [1:0]    out;\
    pin:        INPUT    val;\
    function:    RSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    RSHIFT4 {\
    pin:        INPUT    [3:0]    in;\
    pin:        INPUT    [2:0]    shift;\
    pin:        OUTPUT    [3:0]    out;\
    pin:        INPUT    val;\
    function:    RSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    RSHIFT8 {\
    pin:        INPUT    [7:0]    in;\
    pin:        INPUT    [3:0]    shift;\
    pin:        OUTPUT    [7:0]    out;\
    pin:        INPUT    val;\
    function:    RSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    RSHIFT16 {\
    pin:        INPUT    [15:0]    in;\
    pin:        INPUT    [4:0]    shift;\
    pin:        OUTPUT    [15:0]    out;\
    pin:        INPUT    val;\
    function:    RSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    RSHIFT32 {\
    pin:        INPUT    [31:0]    in;\
    pin:        INPUT    [5:0]    shift;\
    pin:        OUTPUT    [31:0]    out;\
    pin:        INPUT    val;\
    function:    RSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    RSHIFT64 {\
    pin:        INPUT    [63:0]    in;\
    pin:        INPUT    [6:0]    shift;\
    pin:        OUTPUT    [63:0]    out;\
    pin:        INPUT    val;\
    function:    RSH;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (in, DATAIN, 1);\
    attribute:    (shift, SHIFT, 0);\
    attribute:    (val, SHIFTVAL, 0);\
}\
Cell    CMP1 {\
    pin:        INPUT    in1;\
    pin:        INPUT    in2;\
    pin:        OUTPUT    equal;\
    pin:        OUTPUT    unequal;\
    pin:        OUTPUT    greater;\
    pin:        OUTPUT    lesser;\
    function:    CMP;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (equal, EQ, 1);\
    attribute:    (unequal, UEQ, 1);\
    attribute:    (greater, GT, 1);\
    attribute:    (lesser, LT, 1);\
}\
Cell    CMP2 {\
    pin:        INPUT    [1:0]    in1;\
    pin:        INPUT    [1:0]    in2;\
    pin:        OUTPUT    equal;\
    pin:        OUTPUT    unequal;\
    pin:        OUTPUT    greater;\
    pin:        OUTPUT    lesser;\
    function:    CMP;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (equal, EQ, 1);\
    attribute:    (unequal, UEQ, 1);\
    attribute:    (greater, GT, 1);\
    attribute:    (lesser, LT, 1);\
}\
Cell    CMP4 {\
    pin:        INPUT    [3:0]    in1;\
    pin:        INPUT    [3:0]    in2;\
    pin:        OUTPUT    equal;\
    pin:        OUTPUT    unequal;\
    pin:        OUTPUT    greater;\
    pin:        OUTPUT    lesser;\
    function:    CMP;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (equal, EQ, 1);\
    attribute:    (unequal, UEQ, 1);\
    attribute:    (greater, GT, 1);\
    attribute:    (lesser, LT, 1);\
}\
Cell    CMP8 {\
    pin:        INPUT    [7:0]    in1;\
    pin:        INPUT    [7:0]    in2;\
    pin:        OUTPUT    equal;\
    pin:        OUTPUT    unequal;\
    pin:        OUTPUT    greater;\
    pin:        OUTPUT    lesser;\
    function:    CMP;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (equal, EQ, 1);\
    attribute:    (unequal, UEQ, 1);\
    attribute:    (greater, GT, 1);\
    attribute:    (lesser, LT, 1);\
}\
Cell    CMP8 {\
    pin:        INPUT    [7:0]    in1;\
    pin:        INPUT    [7:0]    in2;\
    pin:        OUTPUT    equal;\
    pin:        OUTPUT    unequal;\
    pin:        OUTPUT    greater;\
    pin:        OUTPUT    lesser;\
    function:    CMP;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (equal, EQ, 1);\
    attribute:    (unequal, UEQ, 1);\
    attribute:    (greater, GT, 1);\
    attribute:    (lesser, LT, 1);\
}\
Cell    CMP16 {\
    pin:        INPUT    [15:0]    in1;\
    pin:        INPUT    [15:0]    in2;\
    pin:        OUTPUT    equal;\
    pin:        OUTPUT    unequal;\
    pin:        OUTPUT    greater;\
    pin:        OUTPUT    lesser;\
    function:    CMP;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (equal, EQ, 1);\
    attribute:    (unequal, UEQ, 1);\
    attribute:    (greater, GT, 1);\
    attribute:    (lesser, LT, 1);\
}\
Cell    CMP32 {\
    pin:        INPUT    [31:0]    in1;\
    pin:        INPUT    [31:0]    in2;\
    pin:        OUTPUT    equal;\
    pin:        OUTPUT    unequal;\
    pin:        OUTPUT    greater;\
    pin:        OUTPUT    lesser;\
    function:    CMP;\
    area:        2.0;\
    delay:        2.0;\
    attribute:    (equal, EQ, 1);\
    attribute:    (unequal, UEQ, 1);\
    attribute:    (greater, GT, 1);\
    attribute:    (lesser, LT, 1);\
}\
Cell    CMP64 {\
    pin:        INPUT    [63:0]    in1;\
    pin:        INPUT    [63:0]    in2;\
    pin:        OUTPUT    equal;\
    pin:        OUTPUT    unequal;\
    pin:        OUTPUT    greater;\
    pin:        OUTPUT    lesser;\
    function:    CMP;\
    attribute:    (equal, EQ, 1);\
    attribute:    (unequal, UEQ, 1);\
    attribute:    (greater, GT, 1);\
    attribute:    (lesser, LT, 1);\
}\
Cell    VCC {\
    pin:    OUTPUT    out;\
    function:    VCC;\
    area:        1.0;\
    delay:        1.0;\
}\
Cell    GND {\
    pin:    OUTPUT out;\
    function:    GND;\
    area:        1.0;\
    delay:        1.0;\
}\
Cell    INC1 {\
    pin:    INPUT in;\
    pin:    OUTPUT [1:0] out;\
    function:    INC;\
    area:        1.0;\
    delay:        1.0;\
}\
Cell    INC2 {\
    pin:    INPUT [1:0] in;\
    pin:    OUTPUT [2:0] out;\
    function:    INC;\
    area:        2.0;\
    delay:        1.0;\
}\
Cell    INC4 {\
    pin:    INPUT [3:0] in;\
    pin:    OUTPUT [4:0] out;\
    function:    INC;\
    area:        4.0;\
    delay:        1.0;\
}\
Cell    INC8 {\
    pin:    INPUT [7:0] in;\
    pin:    OUTPUT [8:0] out;\
    function:    INC;\
    area:        8.0;\
    delay:        1.0;\
}\
Cell    INC16 {\
    pin:    INPUT [15:0] in;\
    pin:    OUTPUT [16:0] out;\
    function:    INC;\
    area:        16.0;\
    delay:        1.0;\
}\
Cell    INC32 {\
    pin:    INPUT [31:0] in;\
    pin:    OUTPUT [32:0] out;\
    function:    INC;\
    area:        32.0;\
    delay:        1.0;\
}\
Cell    INC64 {\
    pin:    INPUT [63:0] in;\
    pin:    OUTPUT [64:0] out;\
    function:    INC;\
    area:        64.0;\
    delay:        1.0;\
}    \0";
