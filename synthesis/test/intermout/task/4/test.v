module test();

reg  A, B, C, D, Z;
reg  AB_AND;
reg ab_and;

always @( A or B)
begin
    bitwise_oper(AB_AND, A, B);
    bitwise_oper(Z, C, D);
end

task bitwise_oper;
output  ab_and;
input   a, b;
begin
    ab_and = a&b;
end
endtask
endmodule
