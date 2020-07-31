module test(input in1, in2, clk, output out);
reg q1, q2, q3;

assign out = q3;
always @(posedge clk) begin
    q1 <= in1;
	q2 <= in2;
end	
always @(posedge clk) begin
    q3 <= q1 | q2;
end	
endmodule
