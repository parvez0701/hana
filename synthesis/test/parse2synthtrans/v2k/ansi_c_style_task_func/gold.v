module test();

reg myreg;
reg signed [31:0] myvar;
reg myreg_0;
reg myvar_0;
reg signed [31:0] fp_alu_0;
task check_result(result, expected, OK);

input  [63:0]result;
input  [63:0]expected;
output  OK;
    begin
end
endtask
function signed [31:0] fp_alu(r1);

    begin
    myvar_0 = ({myvar_0}+-1);
    end
endfunction
endmodule
