module test(byteIn, clockfa, byteout);
input [3:0] byteIn;
input clockfa;
output [3:0] byteout;

task reversebyte;
    input [3:0] a;
	output [3:0] z;
	integer j;
	begin
	    for(j = 3; j >= 0; j = j -1)
		    z[j] = a[3-j];
	end
endtask

always @ (negedge clockfa)
    reversebyte(byteIn, byteout);

endmodule
