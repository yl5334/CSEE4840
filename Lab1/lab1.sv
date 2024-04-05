// CSEE 4840 Lab 1: Run and Display Collatz Conjecture Iteration Counts
//
// Spring 2024
//
// By: Hongkuan Yu, Yelin Mao, Yuqi Lin
// Uni: hy2819, ym3000, yl5334

module lab1( input logic        CLOCK_50,  // 50 MHz Clock input
	     
	     input logic [3:0] 	KEY, // Pushbuttons; KEY[0] is rightmost

	     input logic [9:0] 	SW, // Switches; SW[0] is rightmost

	     // 7-segment LED displays; HEX0 is rightmost
	     output logic [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5,

	     output logic [9:0] LEDR // LEDs above the switches; LED[0] on right
	     );

   logic 			clk, go, done;   
   logic [31:0] 		start;
   logic [15:0] 		count;

   logic [11:0] 		n;
   
   assign clk = CLOCK_50;
 
   range #(256, 8) // RAM_WORDS = 256, RAM_ADDR_BITS = 8)
         r ( .* ); // Connect everything with matching names

   logic [22:0] counter;

    assign LEDR = SW;
 
    // Replace this comment and the code below it with your own code;
    always_ff @(posedge CLOCK_50) begin
        if (KEY == 4'b1110) begin
            if (counter == 0) begin
		if(n < SW + 256)begin
			n <= n + 1;
			start <= start + 1;
		end
		end
        end else if (KEY == 4'b1101) begin
            if (counter == 0) begin
		if (n > SW)begin
                	n <= n - 1;
			start <= start - 1;
		end
		end
        end

        if (KEY == 4'b1011) begin
            n <= SW;
	    start <= 0;
        end

        if (KEY == 4'b0111) begin
            go <= 1;
            start <= SW;
	    n <= SW;
        end else go <= 0;
	
	if(done) begin
	    start <= 0;
	end

        counter <= counter + 1;
   end
	/*
        // Ensure n stays within bounds
        if (n < SW) begin
		start <= 0;
                n <= SW_temp;
	end
        else if (n > SW_temp + 256) begin
		n <= SW_temp + 256;
		start <= 255;
	end
    end
       */
    
    hex7seg h0(.a(count[3:0]), .y(HEX0));
    hex7seg h1(.a(count[7:4]), .y(HEX1));
    hex7seg h2(.a(count[11:8]), .y(HEX2));
    hex7seg h3(.a(n[3:0]), .y(HEX3));
    hex7seg h4(.a(n[7:4]), .y(HEX4));
    hex7seg h5(.a(n[11:8]), .y(HEX5));

endmodule
