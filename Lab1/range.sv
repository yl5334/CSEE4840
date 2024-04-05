// CSEE 4840 Lab 1: Run and Display Collatz Conjecture Iteration Counts
//
// Spring 2024
//
// By: Hongkuan Yu, Yelin Mao, Yuqi Lin
// Uni: hy2819, ym3000, yl5334

module range
   #(parameter
     RAM_WORDS = 16,            // Number of counts to store in RAM
     RAM_ADDR_BITS = 4)         // Number of RAM address bits
   (input logic         clk,    // Clock
    input logic 	go,     // Read start and start testing
    input logic [31:0] 	start,  // Number to start from or count to read
    output logic 	done,   // True once memory is filled
    output logic [15:0] count); // Iteration count once finished

   logic 		cgo;    // "go" for the Collatz iterator
   logic                cdone;  // "done" from the Collatz iterator
   logic [31:0] 	n;      // number to start the Collatz iterator

// verilator lint_off PINCONNECTEMPTY
   
   // Instantiate the Collatz iterator
   collatz c1(.clk(clk),
	      .go(cgo),
	      .n(n),
	      .done(cdone),
	      .dout());

   logic [RAM_ADDR_BITS - 1:0] 	 num;         // The RAM address to write
   logic 			 running = 0; // True during the iterations

   /* Replace this comment and the code below with your solution,
      which should generate running, done, cgo, n, num, we, and din */
   always_ff @(posedge clk) begin
      if (go) begin
         running <= 1;
         n <= start;
         num <= {RAM_ADDR_BITS{1'b0}};
         cgo <= 1;
         din <= 16'b1;
      end
      else begin
         if (num != {RAM_ADDR_BITS{1'b1}}) begin
            if (cgo) begin
               running <= 1;
               cgo <= 0;
               din <= 16'b1;               
            end
            else begin
               running <= 1;
               din <= din + 16'b1; 
               if (cdone) begin
                  cgo <= 1;
                  n <= n + 32'h1;
                  num <= num + 1;
                  din <= din;
               end
            end
         end
         else begin
            if (cgo) begin
               running <= 1;
               cgo <= 0;
               din <= 16'b1;               
            end
            else begin
               din <= din + 16'b1;
               running <= 1;
               if (cdone) begin
                  din <= din;                 
                  running <= 0;
                  if (running == 1) begin
                     done <= 1;
                     n <= n + 32'h1;
                  end
                  else begin
                     done <= 0;
                     n <= n;
                  end
               end
            end
         end
      end
   end
   
   always_comb begin
      if (cdone && running) begin
         we = 1;
         if (cgo || done) begin
            we = 0;
         end
      end
      else begin
         we = 0;
      end
   end
   /* Replace this comment and the code above with your solution */

   logic 			 we;                    // Write din to addr
   logic [15:0] 		 din;                   // Data to write
   logic [15:0] 		 mem[RAM_WORDS - 1:0];  // The RAM itself
   logic [RAM_ADDR_BITS - 1:0] 	 addr;                  // Address to read/write

   assign addr = we ? num : start[RAM_ADDR_BITS-1:0];
   
   always_ff @(posedge clk) begin
      if (we) mem[addr] <= din;
      count <= mem[addr];      
   end

endmodule
