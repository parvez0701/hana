module test(input in, sel, output out);
     	 
assign out =  func(in, sel);

function  func;
   input in; 
   input sel; 
   begin
     if(sel)
         func = in;
		 
   end
endfunction

endmodule	
