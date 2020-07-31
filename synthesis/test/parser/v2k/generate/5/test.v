module test(q, d, clk, rst);
input d, clk, rst;
output reg q;
wire w;
reg r1, r2;

genvar i;
generate 
for(i = 0; i < 1; i = i + 1) begin
    always @* begin
	    if(rst)
		    q <= 1'b1;
		else
		    q <= d;
	end
end
endgenerate

generate
if (1) begin : xxx
reg r;
assign w = r2;
end	
if(1)
   assign  w = r1;
endgenerate	


endmodule

/*
module test(out, in);
parameter p = 0;
output [1:0] out;
input [1:0] in;

generate 
if(p) begin
    assign out[1] = in[1];
end
else begin
    Dummy dummy();
    //assign out[0] = in[0];
end
endgenerate
endmodule

module Dummy();
endmodule
 
*/
