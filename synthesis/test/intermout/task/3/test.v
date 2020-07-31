module test();

reg [1:0] A, B;
reg [1:0] AB_AND;
reg ab_and;

always @( A or B)
begin
    bitwise_oper(AB_AND, A, B);
end

task bitwise_oper;
output [1:0] ab_and;
input  [1:0] a, b;
begin
    ab_and = a&b;
end
endtask
endmodule
