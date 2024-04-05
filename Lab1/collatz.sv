// CSEE 4840 Lab 1: Run and Display Collatz Conjecture Iteration Counts
//
// Spring 2024
//
// By: Hongkuan Yu, Yelin Mao, Yuqi Lin
// Uni: hy2819, ym3000, yl5334

module collatz( input logic         clk,   // Clock
		input logic 	    go,    // Load value from n; start iterating
		input logic  [31:0] n,     // Start value; only read when go = 1
		output logic [31:0] dout,  // Iteration value: true after go = 1
		output logic 	    done); // True when dout reaches 1

   always_ff @(posedge clk) begin
      if (go) begin
         dout <= n;  
      end 
      else begin
         if (dout == 32'h1) begin
            dout <= dout;
         end
         else if (dout % 32'h2 == 32'h0) begin
            dout <= dout / 32'h2;
         end
         else begin
            dout <= (32'h3 * dout) + 32'h1;
         end
      end
   end

   always_comb begin
      if (dout == 32'h1) begin
            done = 1;
      end
      else begin
            done = 0;
      end
   end 

endmodule
