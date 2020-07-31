module stimulus;

reg IN0, IN1, IN2, IN3;
reg S1, S0;

wire OUTPUT;

mux4_to_1 mux(OUTPUT, IN0, IN1, IN2, IN3, S1, S0);

initial
begin
	IN0 = 1;
	IN1 = 0;
	IN2 = 1;
	IN3 = 0;
	
	S1 = 0; S0 = 0;
	#1 $display("IN0 = %b, IN1 = %b, IN2 = %b, IN3 = %b\n", 
		IN0, IN1, IN2, IN3);
end
endmodule
