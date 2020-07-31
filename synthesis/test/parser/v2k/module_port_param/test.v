module adder #(parameter MSB = 32, LSB = 0)
	(	output 	reg		[MSB:LSB]	sum,
		output 	reg					co,
		input	wire	[MSB:LSB]	a, b,
		input	wire				ci);
endmodule
