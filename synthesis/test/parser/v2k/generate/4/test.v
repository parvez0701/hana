module adder(co, sum, a0, a1, ci);
parameter N = 4;
output [N-1:0] sum;
output co;
input [N-1:0] a0, a1;
input ci;

generate
case (N)
	1: adder_1bit adder1(co, sum, a0, a1, ci);
	2: adder_2bit adder2(co, sum, a0, a1, ci);
	default: adder_cla #(N) adder3 (co, sum, a0, a1, ci);
endcase
endgenerate
endmodule
