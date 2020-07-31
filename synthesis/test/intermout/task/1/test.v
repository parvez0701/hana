module test();

parameter delay = 10;
reg [1:0] A, B;
reg [1:0] AB_AND, AB_OR, AB_XOR;
reg ab_and;

always @( A or B)
begin
    bitwise_oper(AB_AND, AB_OR, AB_XOR, A, B);
end

task bitwise_oper;
output [1:0] ab_and, ab_or, ab_xor;
input [1:0] a, b;
begin
    #delay ab_and = a&b;
	ab_or = a | b;
	ab_xor = a ^ b;
end
endtask
endmodule
