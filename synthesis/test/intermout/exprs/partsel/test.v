module test(input [7:0] in, output [7:0] out);
wire sel;
reg [255:0] data1;
reg [0:255] data2;

reg [7:0] byte1;
wire [7:0] byte0, byte2, byte3, byte4;
integer i;

assign out[5:2] = in[3:0];
assign byte0 = data1[31-:8];
assign byte2 = data1[24+:8];
assign byte3 = data2[31-:8];
assign byte4 = data2[24+:8];

always @(sel ) begin
	for(i = 0; i <= 1; i = i+1)
	    byte1 = data1[(i*8) +:8];
end

endmodule
