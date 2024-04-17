module vga_ball(input logic        clk,
	        input logic 	   reset,
		input logic [15:0] writedata,
		input logic 	   write,
		input 		   chipselect,
		input logic [4:0]  address,
                   
                //For Audio
		input L_READY,
		input R_READY,
		output logic [15:0] L_DATA,
		output logic [15:0] R_DATA,
		output logic L_VALID,
		output logic R_VALID,

		output logic [7:0] VGA_R, VGA_G, VGA_B,
		output logic 	   VGA_CLK, VGA_HS, VGA_VS,
		                   VGA_BLANK_n,
		output logic 	   VGA_SYNC_n);

   logic [10:0]	   hcount;
   logic [9:0]     vcount;

   logic [7:0] 	   background_r, background_g, background_b;

   logic [15:0]    fix_coordinate, misc, p1_coordinate, p1_state, p1_bomb, p1_firecenter, p1_fireup, p1_firedown, p1_fireleft, p1_fireright, p2_coordinate, p2_state, p2_bomb, p2_firecenter, p2_fireup, p2_firedown, p2_fireleft, p2_fireright;
	
   vga_counters counters(.clk50(clk), .*);

   always_ff @(posedge clk)
     if (reset) begin
      // set the background to green
      background_r <= 8'h44;
      background_g <= 8'h89;
      background_b <= 8'h1A;
     end 
     else if (chipselect && write)
       case (address)
        5'b00000 : fix_coordinate <= writedata;  // 0 in decimal
        5'b00001 : misc <= writedata;            // 1 in decimal

        5'b00010 : p1_coordinate <= writedata;   // 2 in decimal
        //5'b00011 : p1_die_coordinate <= writedata;// 3 in decimal
        5'b00100 : p1_state <= writedata;        // 4 in decimal
        5'b00101 : p1_bomb <= writedata;         // 5 in decimal
        5'b00110 : p1_firecenter <= writedata;   // 6 in decimal
        5'b00111 : p1_fireup <= writedata;       // 7 in decimal
        5'b01000 : p1_firedown <= writedata;     // 8 in decimal
        5'b01001 : p1_fireleft <= writedata;     // 9 in decimal
        5'b01010 : p1_fireright <= writedata;    // 10 in decimal

        5'b01011 : p2_coordinate <= writedata;   // 11 in decimal
        //5'b01100 : p2_die_coordinate <= writedata;// 12 in decimal
        5'b01101 : p2_state <= writedata;        // 13 in decimal
        5'b01110 : p2_bomb <= writedata;         // 14 in decimal
        5'b01111 : p2_firecenter <= writedata;   // 15 in decimal
        5'b10000 : p2_fireup <= writedata;       // 16 in decimal
        5'b10001 : p2_firedown <= writedata;     // 17 in decimal
        5'b10010 : p2_fireleft <= writedata;     // 18 in decimal
        5'b10011 : p2_fireright <= writedata;    // 19 in decimal

       endcase

    //p1
    logic [9:0] p1_address;
    logic [7:0] p1_output;
    soc_system_p1_unit p1_unit(.address(p1_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_output));
    logic [1:0] p1_en;

    logic [9:0] p1_x;
    logic [9:0] p1_y;
    logic [1:0] p1_dir;

    logic [7:0] p1_die_address;
    logic [7:0] p1_die_output;
    soc_system_p1_die die1(.address(p1_die_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_die_output));
    logic [1:0] p1_die_en;

    logic p1_die_on;

    //logic [9:0] p1die_x;
    //logic [9:0] p1die_y;

    //p2
    logic [9:0] p2_address;
    logic [7:0] p2_output;
    soc_system_p2_unit p2_unit(.address(p2_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_output));
    logic [1:0] p2_en;

    logic [9:0] p2_x;
    logic [9:0] p2_y;
    logic [1:0] p2_dir;

    logic [7:0] p2_die_address;
    logic [7:0] p2_die_output;
    soc_system_p2_die die2(.address(p2_die_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_die_output));
    logic [1:0] p2_die_en;

    logic p2_die_on;

    //logic [9:0] p2die_x;
    //logic [9:0] p2die_y;

    //fixed wall
    logic [7:0] fix_address;
    logic [7:0] fix_output;
    soc_system_fix fix(.address(fix_address),.clk(clk),.clken(1),.reset_req(0),.readdata(fix_output));
    logic [1:0] fix_en;
    
    logic fix_on;

    //p1_bomb
    logic [7:0] p1_bomb_address;
    logic [7:0] p1_bomb_output;
    soc_system_bomb bomb1(.address(p1_bomb_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_bomb_output));
    logic [1:0] p1_bomb_en;
    
    logic p1_bomb_on;

    logic [9:0] p1bomb_x;
    logic [9:0] p1bomb_y;

    //p2_bomb
    logic [7:0] p2_bomb_address;
    logic [7:0] p2_bomb_output;
    soc_system_bomb bomb2(.address(p2_bomb_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_bomb_output));
    logic [1:0] p2_bomb_en;

    logic p2_bomb_on;

    logic [9:0] p2bomb_x;
    logic [9:0] p2bomb_y;

    //p1_fire
    logic [7:0] p1_firecenter_address;
    logic [7:0] p1_firecenter_output;
    soc_system_firecenter firecenter1(.address(p1_firecenter_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_firecenter_output));
    logic [1:0] p1_firecenter_en;

    logic p1_firecenter_on;

    logic [7:0] p1_fireup_address;
    logic [7:0] p1_fireup_output;
    soc_system_fireverti fireup1(.address(p1_fireup_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_fireup_output));
    logic [1:0] p1_fireup_en;

    logic p1_fireup_on;

    logic [7:0] p1_firedown_address;
    logic [7:0] p1_firedown_output;
    soc_system_fireverti firedown1(.address(p1_firedown_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_firedown_output));
    logic [1:0] p1_firedown_en;

    logic p1_firedown_on;

    logic [7:0] p1_fireleft_address;
    logic [7:0] p1_fireleft_output;
    soc_system_firehori fireleft1(.address(p1_fireleft_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_fireleft_output));
    logic [1:0] p1_fireleft_en;
    
    logic p1_fireleft_on;

    logic [7:0] p1_fireright_address;
    logic [7:0] p1_fireright_output;
    soc_system_firehori fireright1(.address(p1_fireright_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_fireright_output));
    logic [1:0] p1_fireright_en;
    
    logic p1_fireright_on;
    
    logic [9:0] p1firecenter_x;
    logic [9:0] p1firecenter_y;
    logic [9:0] p1fireup_x;
    logic [9:0] p1fireup_y;
    logic [9:0] p1firedown_x;
    logic [9:0] p1firedown_y;
    logic [9:0] p1fireleft_x;
    logic [9:0] p1fireleft_y;
    logic [9:0] p1fireright_x;
    logic [9:0] p1fireright_y;

    //p2_fire
    logic [7:0] p2_firecenter_address;
    logic [7:0] p2_firecenter_output;
    soc_system_firecenter firecenter2(.address(p2_firecenter_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_firecenter_output));
    logic [1:0] p2_firecenter_en;

    logic p2_firecenter_on;

    logic [7:0] p2_fireup_address;
    logic [7:0] p2_fireup_output;
    soc_system_fireverti fireup2(.address(p2_fireup_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_fireup_output));
    logic [1:0] p2_fireup_en;

    logic p2_fireup_on;

    logic [7:0] p2_firedown_address;
    logic [7:0] p2_firedown_output;
    soc_system_fireverti firedown2(.address(p2_firedown_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_firedown_output));
    logic [1:0] p2_firedown_en;

    logic p2_firedown_on;

    logic [7:0] p2_fireleft_address;
    logic [7:0] p2_fireleft_output;
    soc_system_firehori fireleft2(.address(p2_fireleft_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_fireleft_output));
    logic [1:0] p2_fireleft_en;
    
    logic p2_fireleft_on;

    logic [7:0] p2_fireright_address;
    logic [7:0] p2_fireright_output;
    soc_system_firehori fireright2(.address(p2_fireright_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_fireright_output));
    logic [1:0] p2_fireright_en;
    
    logic p2_fireright_on;

    logic [9:0] p2firecenter_x;
    logic [9:0] p2firecenter_y;
    logic [9:0] p2fireup_x;
    logic [9:0] p2fireup_y;
    logic [9:0] p2firedown_x;
    logic [9:0] p2firedown_y;
    logic [9:0] p2fireleft_x;
    logic [9:0] p2fireleft_y;
    logic [9:0] p2fireright_x;
    logic [9:0] p2fireright_y;

    //map
    logic [10:0] map_address;
    logic [7:0] map_output;
    soc_system_map_unit map_unit(.address(map_address),.clk(clk),.clken(1),.reset_req(0),.readdata(map_output));
    logic [1:0] map_en;

    //tile
    logic [9:0] tile16_x; //[5:0] should be enough, just in case so match with hcount[10:1]
    logic [9:0] tile16_y; //[5:0] should be enough, just in case so match with vcount[9:0]
    

    // dir:
    // 2'b00 --> up
    // 2'b01 --> down
    // 2'b10 --> left
    // 2'b11 --> right

    always_comb begin
      //about p1	 
      p1_x = p1_coordinate[15:8] << 1; //Still consider whether need shift or not.
      p1_y = p1_coordinate[7:0] << 1;

      //p1die_x = p1_die_coordinate[15:8] << 1;
      //p1die_y = p1_die_coordinate[7:0] << 1;
      
      //state of p1
      p1_dir = p1_state[1:0];
      p1_die_on = p1_state[2];
      p1_bomb_on = p1_state[3];
      p1_firecenter_on = p1_state[4];
      p1_fireup_on = p1_state[5];
      p1_firedown_on = p1_state[6];
      p1_fireleft_on = p1_state[7];
      p1_fireright_on = p1_state[8];
      

      //about p2
      p2_x = p2_coordinate[15:8] << 1;
      p2_y = p2_coordinate[7:0] << 1;
      
      //p2die_x = p2_die_coordinate[15:8] << 1;
      //p2die_y = p2_die_coordinate[7:0] << 1;

      //state of p2
      p2_dir = p2_state[1:0];
      p2_die_on = p2_state[2];
      p2_bomb_on = p2_state[3];
      p2_firecenter_on = p2_state[4];
      p2_fireup_on = p2_state[5];
      p2_firedown_on = p2_state[6];
      p2_fireleft_on = p2_state[7];
      p2_fireright_on = p2_state[8];

      //tile
      tile16_x = hcount[10:1] >> 4; //divide by 16 to get the x coordinate. 640/16 = 40
      tile16_y = vcount[9:0] >> 4; //divide by 16 to get the y coordinate. 480/16 = 30

      //p1_bomb
      p1bomb_x = p1_bomb[15:8] << 1;
      p1bomb_y = p1_bomb[7:0] << 1;

      //p1_fire
      p1firecenter_x = p1_firecenter[15:8] << 1;
      p1firecenter_y = p1_firecenter[7:0] << 1;
      p1fireup_x = p1_fireup[15:8] << 1;
      p1fireup_y = p1_fireup[7:0] << 1;
      p1firedown_x = p1_firedown[15:8] << 1;
      p1firedown_y = p1_firedown[7:0] << 1;
      p1fireleft_x = p1_fireleft[15:8] << 1;
      p1fireleft_y = p1_fireleft[7:0] << 1;
      p1fireright_x = p1_fireright[15:8] << 1;
      p1fireright_y = p1_fireright[7:0] << 1;

      //p2_bomb
      p2bomb_x = p2_bomb[15:8] << 1;
      p2bomb_y = p2_bomb[7:0] << 1;

      //p2_fire
      p2firecenter_x = p2_firecenter[15:8] << 1;
      p2firecenter_y = p2_firecenter[7:0] << 1;
      p2fireup_x = p2_fireup[15:8] << 1;
      p2fireup_y = p2_fireup[7:0] << 1;
      p2firedown_x = p2_firedown[15:8] << 1;
      p2firedown_y = p2_firedown[7:0] << 1;
      p2fireleft_x = p2_fireleft[15:8] << 1;
      p2fireleft_y = p2_fireleft[7:0] << 1;
      p2fireright_x = p2_fireright[15:8] << 1;
      p2fireright_y = p2_fireright[7:0] << 1;

      //map
      map_en = 2'b1;
      map_address = tile16_x + tile16_y * 40;
      fix_on = 1'b1;

    // dir:
    // 2'b00 --> up
    // 2'b01 --> down
    // 2'b10 --> left
    // 2'b11 --> right
    end

    //show p1
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1_x && hcount[10:1] <= (p1_x + 10'd15) && vcount[9:0] >= p1_y && vcount[9:0] <= (p1_y + 10'd15) && p1_die_on == 1'b0) begin
        
        p1_en <= 2'b1;
        p1_die_en <= 2'b0;
	
	case(p1_dir)
	 2'b00 : p1_address <= hcount[10:1] - p1_x + (vcount[9:0] - p1_y) * 16;
         2'b01 : p1_address <= hcount[10:1] - p1_x + (vcount[9:0] - p1_y) * 16 + 256;
         2'b10 : p1_address <= hcount[10:1] - p1_x + (vcount[9:0] - p1_y) * 16 + 512;
         2'b11 : p1_address <= hcount[10:1] - p1_x + (vcount[9:0] - p1_y) * 16 + 768;
	endcase
      
      end

      else if (hcount[10:1] >= p1_x && hcount[10:1] <= (p1_x + 10'd15) && vcount[9:0] >= p1_y && vcount[9:0] <= (p1_y + 10'd15) && p1_die_on == 1'b1) begin

        p1_en <= 2'b0;
        p1_die_en <= 2'b1;
        
        p1_die_address <= hcount[10:1] - p1_x + (vcount[9:0] - p1_y) * 16;

      end

    end

    //show p2
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2_x && hcount[10:1] <= (p2_x + 10'd15) && vcount[9:0] >= p2_y && vcount[9:0] <= (p2_y + 10'd15) && p2_die_on == 1'b0) begin
        
        p2_en <= 2'b1;
        p2_die_en <= 2'b0;
	
	case(p2_dir)
	 2'b00 : p2_address <= hcount[10:1] - p2_x + (vcount[9:0] - p2_y) * 16;
         2'b01 : p2_address <= hcount[10:1] - p2_x + (vcount[9:0] - p2_y) * 16 + 256;
         2'b10 : p2_address <= hcount[10:1] - p2_x + (vcount[9:0] - p2_y) * 16 + 512;
         2'b11 : p2_address <= hcount[10:1] - p2_x + (vcount[9:0] - p2_y) * 16 + 768;
	endcase
      
      end

      else if (hcount[10:1] >= p2_x && hcount[10:1] <= (p2_x + 10'd15) && vcount[9:0] >= p2_y && vcount[9:0] <= (p2_y + 10'd15) && p2_die_on == 1'b1) begin

        p2_en <= 2'b0;
        p2_die_en <= 2'b1;
        
        p2_die_address <= hcount[10:1] - p2_x + (vcount[9:0] - p2_y) * 16;

      end

    end

    //show p1 bomb
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1bomb_x && hcount[10:1] <= (p1bomb_x + 10'd15) && vcount[9:0] >= p1bomb_y && vcount[9:0] <= (p1bomb_y + 10'd15) && p1_bomb_on == 1'b1) begin
        
        p1_bomb_en <= 2'b1;

        p1_bomb_address <= hcount[10:1] - p1bomb_x + (vcount[9:0] - p1bomb_y) * 16;
      
      end

      else begin

        p1_bomb_en <= 2'b0;

      end

    end

    //show p2 bomb
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2bomb_x && hcount[10:1] <= (p2bomb_x + 10'd15) && vcount[9:0] >= p2bomb_y && vcount[9:0] <= (p2bomb_y + 10'd15) && p2_bomb_on == 1'b1) begin
        
        p2_bomb_en <= 2'b1;

        p2_bomb_address <= hcount[10:1] - p2bomb_x + (vcount[9:0] - p2bomb_y) * 16;
      
      end

      else begin

        p2_bomb_en <= 2'b0;

      end

    end

    //show p1 bomb fire after explosion
    //p1 fire center
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1firecenter_x && hcount[10:1] <= (p1firecenter_x + 10'd15) && vcount[9:0] >= p1firecenter_y && vcount[9:0] <= (p1firecenter_y + 10'd15) && p1_firecenter_on == 1'b1) begin
        
        p1_firecenter_en <= 2'b1;

        p1_firecenter_address <= hcount[10:1] - p1firecenter_x + (vcount[9:0] - p1firecenter_y) * 16;
      
      end

      else begin

        p1_firecenter_en <= 2'b0;

      end

    end


    //p1 fire up
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1fireup_x && hcount[10:1] <= (p1fireup_x + 10'd15) && vcount[9:0] >= p1fireup_y && vcount[9:0] <= (p1fireup_y + 10'd15) && p1_fireup_on == 1'b1) begin
        
        p1_fireup_en <= 2'b1;

        p1_fireup_address <= hcount[10:1] - p1fireup_x + (vcount[9:0] - p1fireup_y) * 16;
      
      end

      else begin

        p1_fireup_en <= 2'b0;

      end

    end

    //p1 fire down
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1firedown_x && hcount[10:1] <= (p1firedown_x + 10'd15) && vcount[9:0] >= p1firedown_y && vcount[9:0] <= (p1firedown_y + 10'd15) && p1_firedown_on == 1'b1) begin
        
        p1_firedown_en <= 2'b1;

        p1_firedown_address <= hcount[10:1] - p1firedown_x + (vcount[9:0] - p1firedown_y) * 16;
      
      end

      else begin

        p1_firedown_en <= 2'b0;

      end

    end

    //p1 fire left
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1fireleft_x && hcount[10:1] <= (p1fireleft_x + 10'd15) && vcount[9:0] >= p1fireleft_y && vcount[9:0] <= (p1fireleft_y + 10'd15) && p1_fireleft_on == 1'b1) begin
        
        p1_fireleft_en <= 2'b1;

        p1_fireleft_address <= hcount[10:1] - p1fireleft_x + (vcount[9:0] - p1fireleft_y) * 16;
      
      end

      else begin

        p1_fireleft_en <= 2'b0;

      end

    end

    //p1 fire right
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1fireright_x && hcount[10:1] <= (p1fireright_x + 10'd15) && vcount[9:0] >= p1fireright_y && vcount[9:0] <= (p1fireright_y + 10'd15) && p1_fireright_on == 1'b1) begin
        
        p1_fireright_en <= 2'b1;

        p1_fireright_address <= hcount[10:1] - p1fireright_x + (vcount[9:0] - p1fireright_y) * 16;
      
      end

      else begin

        p1_fireright_en <= 2'b0;

      end

    end

    //show p2 bomb fire after explosion
    //p2 fire center
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2firecenter_x && hcount[10:1] <= (p2firecenter_x + 10'd15) && vcount[9:0] >= p2firecenter_y && vcount[9:0] <= (p2firecenter_y + 10'd15) && p2_firecenter_on == 1'b1) begin
        
        p2_firecenter_en <= 2'b1;

        p2_firecenter_address <= hcount[10:1] - p2firecenter_x + (vcount[9:0] - p2firecenter_y) * 16;
      
      end

      else begin

        p2_firecenter_en <= 2'b0;

      end

    end


    //p2 fire up
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2fireup_x && hcount[10:1] <= (p2fireup_x + 10'd15) && vcount[9:0] >= p2fireup_y && vcount[9:0] <= (p2fireup_y + 10'd15) && p2_fireup_on == 1'b1) begin
        
        p2_fireup_en <= 2'b1;

        p2_fireup_address <= hcount[10:1] - p2fireup_x + (vcount[9:0] - p2fireup_y) * 16;
      
      end

      else begin

        p2_fireup_en <= 2'b0;

      end

    end

    //p2 fire down
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2firedown_x && hcount[10:1] <= (p2firedown_x + 10'd15) && vcount[9:0] >= p2firedown_y && vcount[9:0] <= (p2firedown_y + 10'd15) && p2_firedown_on == 1'b1) begin
        
        p2_firedown_en <= 2'b1;

        p2_firedown_address <= hcount[10:1] - p2firedown_x + (vcount[9:0] - p2firedown_y) * 16;
      
      end

      else begin

        p2_firedown_en <= 2'b0;

      end

    end

    //p2 fire left
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2fireleft_x && hcount[10:1] <= (p2fireleft_x + 10'd15) && vcount[9:0] >= p2fireleft_y && vcount[9:0] <= (p2fireleft_y + 10'd15) && p2_fireleft_on == 1'b1) begin
        
        p2_fireleft_en <= 2'b1;

        p2_fireleft_address <= hcount[10:1] - p2fireleft_x + (vcount[9:0] - p2fireleft_y) * 16;
      
      end

      else begin

        p2_fireleft_en <= 2'b0;

      end

    end

    //p2 fire right
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2fireright_x && hcount[10:1] <= (p2fireright_x + 10'd15) && vcount[9:0] >= p2fireright_y && vcount[9:0] <= (p2fireright_y + 10'd15) && p2_fireright_on == 1'b1) begin
        
        p2_fireright_en <= 2'b1;

        p2_fireright_address <= hcount[10:1] - p2fireright_x + (vcount[9:0] - p2fireright_y) * 16;
      
      end

      else begin

        p2_fireright_en <= 2'b0;

      end

    end

    //map    
    always_ff @(posedge clk) begin

      if (map_en && map_output && fix_on == 1'b1) begin //still considering the condition

        fix_en <= 2'b1;
        fix_address <= hcount[4:1] + vcount[3:0] * 16;

      end

      else begin

        fix_en <= 2'b0;

      end

    end


   //Color Mapping
   always_comb begin
      {VGA_R, VGA_G, VGA_B} = {background_r, background_g, background_b};
      if (VGA_BLANK_n) begin
	if (p1_en) begin
          case (p1_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h9D, 8'h9D, 8'h9D}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'hBE, 8'h26, 8'h33}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hE0, 8'h6F, 8'h8B}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'h49, 8'h3C, 8'h2B}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hA4, 8'h64, 8'h22}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hEB, 8'h89, 8'h31}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'hF7, 8'hE2, 8'h6B}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h2F, 8'h48, 8'h4E}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h44, 8'h89, 8'h1A}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'hA3, 8'hCE, 8'h27}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'h1B, 8'h26, 8'h32}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h57, 8'h84}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'h31, 8'hA2, 8'hF2}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hB2, 8'hDC, 8'hEF}; // White
          endcase
        end
        else if (p1_die_en) begin
          case (p1_die_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p2_en) begin
          case (p1_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
	else if (p2_die_en) begin
          case (p2_die_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (fix_en) begin
          case (fix_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h9D, 8'h9D, 8'h9D}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'hBE, 8'h26, 8'h33}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hE0, 8'h6F, 8'h8B}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'h49, 8'h3C, 8'h2B}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hA4, 8'h64, 8'h22}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hEB, 8'h89, 8'h31}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'hF7, 8'hE2, 8'h6B}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h2F, 8'h48, 8'h4E}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h44, 8'h89, 8'h1A}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'hA3, 8'hCE, 8'h27}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'h1B, 8'h26, 8'h32}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h57, 8'h84}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'h31, 8'hA2, 8'hF2}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hB2, 8'hDC, 8'hEF}; // White
          endcase
        end
        else if (p1_bomb_en) begin
          case (p1_bomb_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p2_bomb_en) begin
          case (p2_bomb_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p1_firecenter_en) begin
          case (p1_firecenter_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p1_fireup_en) begin
          case (p1_fireup_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p1_firedown_en) begin
          case (p1_firedown_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p1_fireleft_en) begin
          case (p1_fireleft_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p1_fireright_en) begin
          case (p1_fireright_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p2_firecenter_en) begin
          case (p2_firecenter_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p2_fireup_en) begin
          case (p2_fireup_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p2_firedown_en) begin
          case (p2_firedown_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p2_fireleft_en) begin
          case (p2_fireleft_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end
        else if (p2_fireright_en) begin
          case (p2_fireright_output)
            8'h00: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; // Black
            8'h01: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hAA}; // Blue
            8'h02: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'h00}; // Green
            8'h03: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hAA, 8'hAA}; // Cyan
            8'h04: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'h00}; // Red
            8'h05: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h00, 8'hAA}; // Magenta
            8'h06: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'h55, 8'h00}; // Brown
            8'h07: {VGA_R, VGA_G, VGA_B} = {8'hAA, 8'hAA, 8'hAA}; // Light Gray
            8'h08: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'h55}; // Dark Gray
            8'h09: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'h55, 8'hFF}; // Light Blue
            8'h0A: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'h55}; // Light Green
            8'h0B: {VGA_R, VGA_G, VGA_B} = {8'h55, 8'hFF, 8'hFF}; // Light Cyan
            8'h0C: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'h55}; // Light Red
            8'h0D: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'h55, 8'hFF}; // Light Magenta
            8'h0E: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'h55}; // Yellow
            8'h0F: {VGA_R, VGA_G, VGA_B} = {8'hFF, 8'hFF, 8'hFF}; // White
          endcase
        end

      end
   end
       
endmodule

module vga_counters(
 input logic 	     clk50, reset,
 output logic [10:0] hcount,  // hcount[10:1] is pixel column
 output logic [9:0]  vcount,  // vcount[9:0] is pixel row
 output logic 	     VGA_CLK, VGA_HS, VGA_VS, VGA_BLANK_n, VGA_SYNC_n);

/*
 * 640 X 480 VGA timing for a 50 MHz clock: one pixel every other cycle
 * 
 * HCOUNT 1599 0             1279       1599 0
 *             _______________              ________
 * ___________|    Video      |____________|  Video
 * 
 * 
 * |SYNC| BP |<-- HACTIVE -->|FP|SYNC| BP |<-- HACTIVE
 *       _______________________      _____________
 * |____|       VGA_HS          |____|
 */
   // Parameters for hcount
   parameter HACTIVE      = 11'd 1280,
             HFRONT_PORCH = 11'd 32,
             HSYNC        = 11'd 192,
             HBACK_PORCH  = 11'd 96,   
             HTOTAL       = HACTIVE + HFRONT_PORCH + HSYNC +
                            HBACK_PORCH; // 1600
   
   // Parameters for vcount
   parameter VACTIVE      = 10'd 480,
             VFRONT_PORCH = 10'd 10,
             VSYNC        = 10'd 2,
             VBACK_PORCH  = 10'd 33,
             VTOTAL       = VACTIVE + VFRONT_PORCH + VSYNC +
                            VBACK_PORCH; // 525

   logic endOfLine;
   
   always_ff @(posedge clk50 or posedge reset)
     if (reset)          hcount <= 0;
     else if (endOfLine) hcount <= 0;
     else  	         hcount <= hcount + 11'd 1;

   assign endOfLine = hcount == HTOTAL - 1;
       
   logic endOfField;
   
   always_ff @(posedge clk50 or posedge reset)
     if (reset)          vcount <= 0;
     else if (endOfLine)
       if (endOfField)   vcount <= 0;
       else              vcount <= vcount + 10'd 1;

   assign endOfField = vcount == VTOTAL - 1;

   // Horizontal sync: from 0x520 to 0x5DF (0x57F)
   // 101 0010 0000 to 101 1101 1111
   assign VGA_HS = !( (hcount[10:8] == 3'b101) &
		      !(hcount[7:5] == 3'b111));
   assign VGA_VS = !( vcount[9:1] == (VACTIVE + VFRONT_PORCH) / 2);

   assign VGA_SYNC_n = 1'b0; // For putting sync on the green signal; unused
   
   // Horizontal active: 0 to 1279     Vertical active: 0 to 479
   // 101 0000 0000  1280	       01 1110 0000  480
   // 110 0011 1111  1599	       10 0000 1100  524
   assign VGA_BLANK_n = !( hcount[10] & (hcount[9] | hcount[8]) ) &
			!( vcount[9] | (vcount[8:5] == 4'b1111) );

   /* VGA_CLK is 25 MHz
    *             __    __    __
    * clk50    __|  |__|  |__|
    *        
    *             _____       __
    * hcount[0]__|     |_____|
    */
   assign VGA_CLK = hcount[0]; // 25 MHz clock: rising edge sensitive
   
endmodule
