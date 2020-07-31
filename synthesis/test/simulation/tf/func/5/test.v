module test(input in, sel, output reg out);

always @(in or sel)
    out =  func(in, sel);

function  func;
   input in; 
   input sel; 
   begin
     if(sel)
         func = in;
		 
   end
endfunction

endmodule	
