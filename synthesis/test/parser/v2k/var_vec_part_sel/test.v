module test();
reg [7:0] byte;
reg [127:0] vector;
always begin : abc
	integer i;
	for(i = 0; i <=15; i = i +1)
		byte = vector[(i*8) +: 8];
	end

endmodule

