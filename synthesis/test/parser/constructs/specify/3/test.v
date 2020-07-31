module M(out, a, b, c, d);
output out;
input a, b, c, d;

wire e, f;

specify
	if(a) (a => out) = 9;
	if(~a) (a => out ) = 10;
	
	if(b & c) ( b => out) = 9;
	if(~(b&c)) (b => out) = 19;
	
	if({c, d} == 2'b01)
		(c, d *> out) = 11;
	if({c, d} != 2'b01)
		(c, d *> out) = 13;
endspecify
endmodule

module DFF_1 (input d, clk, output reg q);

always @ (posedge clk) 
begin
	q <= d;
end

specify
	specparam t_delay = 11;
	(clk => q) = t_delay;
	
	specparam t_rise = 9, t_fall = 13;
	(clk => q) = (t_rise, t_fall);
	
	specparam t_01 = 9, t_10 = 13, t_0z = 11;
	specparam t_z1 = 9, t_1z = 11, t_z0 = 13;
	(clk => q) = (t_01, t_10, t_0z, t_z1, t_1z, t_z0);
endspecify
endmodule

module DFF_2 (input d, clk, output reg q);

always @( posedge clk) 
begin
	q <= d;
end

specify
	specparam t_rise = 8:9:10, t_fall = 12:13:14, t_turnoff = 10:11:12;
	(clk => q) = (t_rise, t_fall, t_turnoff);
endspecify
endmodule
