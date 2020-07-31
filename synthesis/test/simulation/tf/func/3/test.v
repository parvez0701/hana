module test(input in1, in2, output out1, out2);
     	 
assign out1 =  func(in1);
assign out2 =  func(in2);

function  func;
   input in; 
   begin
     func = in;
   end
endfunction

endmodule	
