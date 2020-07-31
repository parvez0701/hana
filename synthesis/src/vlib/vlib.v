// Generic synthesis library.
//Author: Parvez Ahmad


module RtlBuf(o, i);
output o;
input i;
parameter AREA = 1;
parameter DELAY = 1;
buf(o, i);
endmodule

module RtlNot(o, i);
output o;
input i;
parameter AREA = 1;
parameter DELAY = 1;
not(o, i);
endmodule

module RtlOr2(o, i0, i1);
output o;
input i0;
input i1;
parameter AREA = 2;
parameter DELAY = 2;
or(o, i0, i1);

endmodule

module RtlOr3(o, i0, i1, i2);
output o;
input i0;
input i1;
input i2;
parameter AREA = 3;
parameter DELAY = 3;

or(o, i0, i1, i2);
endmodule

module RtlOr4(o, i0, i1, i2, i3);
output o;
input i0;
input i1;
input i2;
input i3;
parameter AREA = 4;
parameter DELAY = 4;

or(o, i0, i1, i2, i3);
endmodule

module RtlAnd2(o, i0, i1);
output o;
input i0;
input i1;
parameter AREA = 2;
parameter DELAY = 2;

and(o, i0, i1);
endmodule

module RtlAnd3(o, i0, i1, i2);
output o;
input i0;
input i1;
input i2;
parameter AREA = 3;
parameter DELAY = 3;

and(o, i0, i1, i2);
endmodule

module RtlAnd4(o, i0, i1, i2, i3);
output o;
input i0;
input i1;
input i2;
input i3;
parameter AREA = 4;
parameter DELAY = 4;

and(o, i0, i1, i2, i3);
endmodule

module RtlGnd (o);
output o;
parameter AREA = 1;
parameter DELAY = 1;
assign o = 1'b0;
endmodule

module RtlVcc(o);
output o;
parameter AREA = 1;
parameter DELAY = 1;
assign o = 1'b1;
endmodule

module RtlNand2(o, i0, i1);
output o;
input i0, i1;
parameter AREA = 3;
parameter DELAY = 3;
nand(o, i0, i1);
endmodule

module RtlNand3(o, i0, i1, i2);
output o;
input i0, i1, i2;
parameter AREA = 4;
parameter DELAY = 4;

nand(o, i0, i1, i2);
endmodule

module RtlNand4(o, i0, i1, i2, i3);
output o;
input i0, i1, i2, i3;
parameter AREA = 5;
parameter DELAY = 5;

nand(o, i0, i1, i2, i3);
endmodule


module RtlNor2(o, i0, i1);
output o;
input i0;
input i1;
parameter AREA = 3;
parameter DELAY = 3;

nor(o, i0, i1);

endmodule

module RtlNor3(o, i0, i1, i2);
output o;
input i0;
input i1;
input i2;
parameter AREA = 4;
parameter DELAY = 4;

nor(o, i0, i1, i2);
endmodule

module RtlNor4(o, i0, i1, i2, i3);
output o;
input i0;
input i1;
input i2;
input i3;
parameter AREA = 5;
parameter DELAY = 5;

nor(o, i0, i1, i2, i3);
endmodule

module RtlXor2(o, i0, i1);
output o;
input i0;
input i1;
parameter AREA = 3;
parameter DELAY = 3;

xor(o, i0, i1);
endmodule
module RtlXor3(o, i0, i1, i2);
output o;
input i0;
input i1;
input i2;
parameter AREA = 4;
parameter DELAY = 4;

xor(o, i0, i1, i2);
endmodule

module RtlXor4(o, i0, i1, i2, i3);
output o;
input i0;
input i1;
input i2;
input i3;
parameter AREA = 5;
parameter DELAY = 5;
xor(o, i0, i1, i2, i3);
endmodule


module RtlXnor2(o, i0, i1);
output o;
input i0;
input i1;
parameter AREA = 3;
parameter DELAY = 3;

xnor(o, i0, i1);
endmodule
module RtlXnor3(o, i0, i1, i2);
output o;
input i0;
input i1;
input i2;
parameter AREA = 4;
parameter DELAY = 4;

xnor(o, i0, i1, i2);
endmodule

module RtlXnor4(o, i0, i1, i2, i3);
output o;
input i0;
input i1;
input i2;
input i3;
parameter AREA = 5;
parameter DELAY = 5;
xnor(o, i0, i1, i2, i3);
endmodule

module RtlMux2(o, i0, i1, sel0);
output o;
input i0;
input i1;
input sel0;
wire w0;
wire w1;
wire w2;
parameter AREA = 7;
parameter DELAY = 7;

and(w1, i1, sel0);
not(w0, sel0);
and(w2, i0, w0);
or(o, w1, w2);

endmodule
module RtlMux4(o, i0, i1, i2, i3, sel0, sel1);
output reg o;
input i0;
input i1;
input i2;
input i3;
input sel0;
input sel1;
wire w0;
wire w1;
wire w2;
parameter AREA = 7;
parameter DELAY = 7;

endmodule

module RtlFF(q, d, clk);
input d, clk;
output reg q;
parameter AREA = 4;
parameter DELAY = 4;
endmodule

module RtlFFR(q, d, clk, reset);
input d, clk, reset;
output reg q;
parameter AREA = 4;
parameter DELAY = 4;
endmodule

module RtlLD(q, d, enable);
input d, enable;
output reg q;
parameter AREA = 4;
parameter DELAY = 4;
endmodule
module RtlLDR(q, d, enable, reset);
input d, enable, reset;
output reg q;
parameter AREA = 4;
parameter DELAY = 4;
endmodule

module RtlAdd#(parameter SIZE = 1)
		(	input [SIZE-1:0]in, output reg [SIZE-1:0]out,
			input cin, output reg cout);
parameter AREA = SIZE * 2;
parameter DELAY = SIZE * 2;
endmodule			


module RtlSub#(parameter SIZE = 1)
		(	input [SIZE-1:0]in, output reg [SIZE-1:0]out,
			input bin, output reg bout);
parameter AREA = SIZE * 2;
parameter DELAY = SIZE * 2;
endmodule			

module RtlMul#(parameter SIZE1 = 1, parameter SIZE2 = 1)
		(	input [SIZE1-1:0]in1, input [SIZE2-1:0]in2,
			output reg [SIZE1+SIZE2-1:0]out);
parameter AREA = SIZE1 * 2;
parameter DELAY = SIZE1 * 2;
endmodule			

module RtlDiv#(parameter SIZE1 = 1, parameter SIZE2 = 1)
		(	input [SIZE1-1:0]in1, input [SIZE2-1:0]in2,
			output reg [(SIZE1 > SIZE2 ? SIZE1 : SIZE2 ):0]out);
parameter AREA = SIZE1 * 2;
parameter DELAY = SIZE1 * 2;
endmodule			

module RtlCmp #(parameter SIZE = 1)
		(	input [SIZE-1:0]in1, input [SIZE-1:0]in2,
			output reg out);
parameter AREA = SIZE1 * 2;
parameter DELAY = SIZE1 * 2;
endmodule			

module RtlLsh #(parameter SIZE = 1)
		(	input [SIZE-1:0]in, output reg [SIZE-1:0] out);
parameter AREA = SIZE * 2;
parameter DELAY = SIZE * 2;
endmodule			

module RtlRsh #(parameter SIZE = 1)
		(	input [SIZE-1:0]in, output reg [SIZE-1:0] out);
parameter AREA = SIZE * 2;
parameter DELAY = SIZE * 2;
endmodule			

module RtlEnc #(parameter SIZE = 1)
		(	input [2 ** SIZE-1:0] in, input enable, 
			output reg [SIZE-1:0] out );
parameter AREA = SIZE * 2;
parameter DELAY = SIZE * 2;
endmodule			

module RtlDec #(parameter SIZE = 1)
		(	input [SIZE-1:0]in, output reg [2**SIZE-1:0]out, input enable);
parameter AREA = SIZE * 2;
parameter DELAY = SIZE * 2;
endmodule	

module RtlInc #(parameter SIZE = 1)
		(	input [SIZE-1:0] in, output reg [SIZE-1:0] out, output reg cout);
parameter AREA = SIZE * 2;
parameter DELAY = SIZE * 2;
endmodule			
