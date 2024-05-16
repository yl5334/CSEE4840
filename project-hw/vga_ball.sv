module vga_ball(input logic        clk,
	        input logic 	   reset,
		input logic [31:0] writedata,
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

   logic [31:0]    p1_coordinate, p1_state, p1_bomb, p1_firecenter, p1_fireup, p1_firedown, p1_fireleft, p1_fireright, p2_coordinate, p2_state, p2_bomb, p2_firecenter, p2_fireup, p2_firedown, p2_fireleft, p2_fireright, map_info;
	
   vga_counters counters(.clk50(clk), .*);

// Audio ////////////////////////////////////////////////////////////////////////

	logic [13:0] explode_address;
	logic [15:0] explode_data;
	soc_system_explode_sound(.address(explode_address),.clk(clk),.clken(1),.reset_req(0),.readdata(explode_data));

        logic [13:0] jingle_address;
	logic [15:0] jingle_data;
	soc_system_jingle_sound(.address(jingle_address),.clk(clk),.clken(1),.reset_req(0),.readdata(jingle_data));

	reg [11:0] counter;
	
	logic playing_explode1;
        logic playing_explode2;
        logic playing_jingle1;
        logic playing_jingle2;

        logic explode_ready1;
        logic explode_ready2;
        logic jingle_ready1;
        logic jingle_ready2;

	always_ff @(posedge clk) begin

		if (reset) begin

			counter <= 0;
			L_VALID <= 0;
			R_VALID <= 0;

			playing_explode1 <= 0;
                        playing_explode2 <= 0;
                        playing_jingle1 <= 0;
                        playing_jingle2 <= 0;

			explode_address <= 0;
                        jingle_address <= 0;

                        explode_ready1 <= 1;
                        explode_ready2 <= 1;
                        jingle_ready1 <= 1;
                        jingle_ready2 <= 1;

		end
		else if (L_READY == 1 && R_READY == 1 && counter < 3125) begin

			counter <= counter + 1;
			L_VALID <= 0;
			R_VALID <= 0;

		end
		else if (L_READY == 1 && R_READY == 1 && counter >= 3125) begin

			counter <= 0;
			L_VALID <= 1;
			R_VALID <= 1;


		        //Flags
		        if (p1_firecenter_on == 1 && explode_ready1 == 1) begin

				if (playing_explode2 == 0 && playing_jingle1 == 0 && playing_jingle2 == 0) begin

					
					playing_explode1 <= 1;
					playing_explode2 <= 0;
                                        playing_jingle1 <= 0;
                                        playing_jingle2 <= 0;	

					explode_ready1 <= 0;

				end

			end
			else if (p2_firecenter_on == 1 && explode_ready2 == 1) begin

				if (playing_explode1 == 0 && playing_jingle1 == 0 && playing_jingle2 == 0) begin	
				
					playing_explode1 <= 0;
					playing_explode2 <= 1;
                                        playing_jingle1 <= 0;
                                        playing_jingle2 <= 0;	

					explode_ready2 <= 0;

				end

			end
	                else if (p1_bomb_on == 1 && jingle_ready1 == 1) begin

				if (playing_explode1 == 0 && playing_explode2 == 0 && playing_jingle2 == 0) begin	
				
					playing_explode1 <= 0;
					playing_explode2 <= 0;
                                        playing_jingle1 <= 1;
                                        playing_jingle2 <= 0;	

					jingle_ready1 <= 0;

				end

			end
                        else if (p2_bomb_on == 1 && jingle_ready2 == 1) begin

				if (playing_explode1 == 0 && playing_explode2 == 0 && playing_jingle1 == 0) begin	
				
					playing_explode1 <= 0;
					playing_explode2 <= 0;
                                        playing_jingle1 <= 0;
                                        playing_jingle2 <= 1;	

					jingle_ready2 <= 0;

				end

			end

                        //Play audio once per instant
			if (p1_firecenter_on == 0) begin

				explode_ready1 <= 1;

			end

			if (p2_firecenter_on == 0) begin

				explode_ready2 <= 1;

			end
                        
                        if (p1_bomb_on == 0) begin

				jingle_ready1 <= 1;

			end

			if (p2_bomb_on == 0) begin

				jingle_ready2 <= 1;

			end
				

			// Playing Audio
			if (playing_explode1 == 1) begin
				if (explode_address > 15000) begin
					explode_address <= 0;
					playing_explode1 <= 0;
				end
				else begin
					explode_address <= explode_address + 1;
				end
				L_DATA <= explode_data;
				R_DATA <= explode_data;
			end
			else if (playing_explode2 == 1) begin
				if (explode_address > 15000) begin
					explode_address <= 0;
					playing_explode2 <= 0;
				end
				else begin
					explode_address <= explode_address + 1;
				end
				L_DATA <= explode_data;
				R_DATA <= explode_data;
			end
                        else if (playing_jingle1 == 1) begin
				if (jingle_address > 15000) begin
					jingle_address <= 0;
					playing_jingle1 <= 0;
				end
				else begin
					jingle_address <= jingle_address + 1;
				end
				L_DATA <= jingle_data;
				R_DATA <= jingle_data;
			end
			else if (playing_jingle2 == 1) begin
				if (jingle_address > 15000) begin
					jingle_address <= 0;
					playing_jingle2 <= 0;
				end
				else begin
					jingle_address <= jingle_address + 1;
				end
				L_DATA <= jingle_data;
				R_DATA <= jingle_data;
			end
			

		end
		else begin

			L_VALID <= 0;
			R_VALID <= 0;

		end
	end

// Display //////////////////////////////////////////////////////////////////////
   always_ff @(posedge clk) begin
     if (reset) begin
      // set the background to green
      background_r <= 8'h30;
      background_g <= 8'h80;
      background_b <= 8'h00;

      //initialization
      p1_coordinate <= 32'd0;   
      p1_state <= 32'd0;        
      p1_bomb <= 32'd0;         
      p1_firecenter <= 32'd0;   
      p1_fireup <= 32'd0;       
      p1_firedown <= 32'd0;     
      p1_fireleft <= 32'd0;     
      p1_fireright <= 32'd0;    

      p2_coordinate <= 32'd0;   
      p2_state <= 32'd0;        
      p2_bomb <= 32'd0;         
      p2_firecenter <= 32'd0;   
      p2_fireup <= 32'd0;       
      p2_firedown <= 32'd0;     
      p2_fireleft <= 32'd0;     
      p2_fireright <= 32'd0;
     end 
     else if (chipselect && write) begin
       case (address)
        5'b00001 : p1_coordinate <= writedata;   
        5'b00010 : p1_state <= writedata;        
        5'b00011 : p1_bomb <= writedata;         
        5'b00100 : p1_firecenter <= writedata;   
        5'b00101 : p1_fireup <= writedata;       
        5'b00110 : p1_firedown <= writedata;     
        5'b00111 : p1_fireleft <= writedata;     
        5'b01000 : p1_fireright <= writedata;    

        5'b01001 : p2_coordinate <= writedata;   
        5'b01010 : p2_state <= writedata;        
        5'b01011 : p2_bomb <= writedata;         
        5'b01100 : p2_firecenter <= writedata;   
        5'b01101 : p2_fireup <= writedata;       
        5'b01110 : p2_firedown <= writedata;     
        5'b01111 : p2_fireleft <= writedata;     
        5'b10000 : p2_fireright <= writedata;

        5'b10001 : map_info <= writedata;    
       endcase
     end
   end

    //p1
    logic [9:0] p1_address;
    logic [15:0] p1_output;
    soc_system_p1_unit p1_unit(.address(p1_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_output));
    logic p1_en;

    logic [9:0] p1_x;
    logic [9:0] p1_y;
    logic [1:0] p1_dir;

    logic [7:0] p1_die_address;
    logic [15:0] p1_die_output;
    soc_system_p1_die die1(.address(p1_die_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_die_output));
    logic p1_die_en;

    logic p1_die_on;


    //p2
    logic [9:0] p2_address;
    logic [15:0] p2_output;
    soc_system_p2_unit p2_unit(.address(p2_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_output));
    logic p2_en;

    logic [9:0] p2_x;
    logic [9:0] p2_y;
    logic [1:0] p2_dir;

    logic [7:0] p2_die_address;
    logic [15:0] p2_die_output;
    soc_system_p2_die die2(.address(p2_die_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_die_output));
    logic p2_die_en;

    logic p2_die_on;

    //fixed wall
    logic [7:0] fix_address;
    logic [15:0] fix_output;
    soc_system_fix fix(.address(fix_address),.clk(clk),.clken(1),.reset_req(0),.readdata(fix_output));
    logic fix_en;

    //Destroyable wall
    logic [7:0] wall_address;
    logic [15:0] wall_output;
    soc_system_wall wall(.address(wall_address),.clk(clk),.clken(1),.reset_req(0),.readdata(wall_output));
    logic wall_en;
    

    //p1_bomb
    logic [7:0] p1_bomb_address;
    logic [15:0] p1_bomb_output;
    soc_system_bomb bomb1(.address(p1_bomb_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_bomb_output));
    logic p1_bomb_en;
    
    logic p1_bomb_on;

    logic [9:0] p1bomb_x;
    logic [9:0] p1bomb_y;

    //p2_bomb
    logic [7:0] p2_bomb_address;
    logic [15:0] p2_bomb_output;
    soc_system_bomb bomb2(.address(p2_bomb_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_bomb_output));
    logic p2_bomb_en;

    logic p2_bomb_on;

    logic [9:0] p2bomb_x;
    logic [9:0] p2bomb_y;

    //p1_fire
    logic [7:0] p1_firecenter_address;write
    logic [15:0] p1_firecenter_output;
    soc_system_firecenter firecenter1(.address(p1_firecenter_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_firecenter_output));
    logic p1_firecenter_en;

    logic p1_firecenter_on;

    logic [7:0] p1_fireup_address;
    logic [15:0] p1_fireup_output;
    soc_system_fireverti fireup1(.address(p1_fireup_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_fireup_output));
    logic p1_fireup_en;

    logic p1_fireup_on;

    logic [7:0] p1_firedown_address;
    logic [15:0] p1_firedown_output;
    soc_system_fireverti firedown1(.address(p1_firedown_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_firedown_output));
    logic p1_firedown_en;

    logic p1_firedown_on;

    logic [7:0] p1_fireleft_address;
    logic [15:0] p1_fireleft_output;
    soc_system_firehori fireleft1(.address(p1_fireleft_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_fireleft_output));
    logic p1_fireleft_en;
    
    logic p1_fireleft_on;

    logic [7:0] p1_fireright_address;
    logic [15:0] p1_fireright_output;
    soc_system_firehori fireright1(.address(p1_fireright_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_fireright_output));
    logic p1_fireright_en;
    
    logic p1_fireright_on;
    
    logic [9:0] p1firecenter_x;
    logic [9:0] p1firecenter_y;
    logic [9:0] p1fireup_x;
    logic [9:0] p1fireup_y;
    logic [9:0] p1firedown_x;
    logic [9:0] p1firedown_y;write
    logic [9:0] p1fireleft_x;
    logic [9:0] p1fireleft_y;
    logic [9:0] p1fireright_x;
    logic [9:0] p1fireright_y;

    //p2_fire
    logic [7:0] p2_firecenter_address;
    logic [15:0] p2_firecenter_output;
    soc_system_firecenter firecenter2(.address(p2_firecenter_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_firecenter_output));
    logic p2_firecenter_en;

    logic p2_firecenter_on;

    logic [7:0] p2_fireup_address;
    logic [15:0] p2_fireup_output;
    soc_system_fireverti fireup2(.address(p2_fireup_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_fireup_output));
    logic p2_fireup_en;

    logic p2_fireup_on;

    logic [7:0] p2_firedown_address;
    logic [15:0] p2_firedown_output;
    soc_system_fireverti firedown2(.address(p2_firedown_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_firedown_output));
    logic p2_firedown_en;

    logic p2_firedown_on;

    logic [7:0] p2_fireleft_address;
    logic [15:0] p2_fireleft_output;
    soc_system_firehori fireleft2(.address(p2_fireleft_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_fireleft_output));
    logic p2_fireleft_en;
    
    logic p2_fireleft_on;

    logic [7:0] p2_fireright_address;
    logic [15:0] p2_fireright_output;
    soc_system_firehori fireright2(.address(p2_fireright_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_fireright_output));
    logic p2_fireright_en;
    
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
    logic map_chipselect;
    logic map_write;
    logic [7:0] map_input;
    logic [7:0] map_output;
    soc_system_map_unit map_unit(.address(map_address),.clk(clk),.clken(1),.reset_req(0),.chipselect(map_chipselect),.write(map_write),.writedata(map_input),.readdata(map_output));
    logic map_en;

    //p1_win
    logic [10:0] p1_win_address;
    logic [15:0] p1_win_output;
    soc_system_p1_win p1_win(.address(p1_win_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p1_win_output));
    logic p1_win_en;

    //p2_win
    logic [10:0] p2_win_address;
    logic [15:0] p2_win_output;
    soc_system_p2_win p2_win(.address(p2_win_address),.clk(clk),.clken(1),.reset_req(0),.readdata(p2_win_output));
    logic p2_win_en;

    //start
    logic [11:0] start_address;
    logic [15:0] start_output;
    soc_system_start start(.address(start_address),.clk(clk),.clken(1),.reset_req(0),.readdata(start_output));
    logic start_en;

    logic start_on;

    //tile
    logic [5:0] tile16_x;
    logic [5:0] tile16_y; 
    

    always_comb begin
      //about p1	 
      p1_x = p1_coordinate[9:0];
      p1_y = p1_coordinate[25:16];
      
      //state of p1
      p1_dir = p1_state[1:0];
      p1_die_on = ~p1_state[2];
      p1_bomb_on = p1_state[3];
      p1_firecenter_on = p1_state[4];
      p1_fireup_on = p1_state[5];
      p1_firedown_on = p1_state[6];
      p1_fireleft_on = p1_state[7];
      p1_fireright_on = p1_state[8];
      

      //about p2
      p2_x = p2_coordinate[9:0];
      p2_y = p2_coordinate[25:16];
      

      //state of p2
      p2_dir = p2_state[1:0];
      p2_die_on = ~p2_state[2];
      p2_bomb_on = p2_state[3];
      p2_firecenter_on = p2_state[4];
      p2_fireup_on = p2_state[5];
      p2_firedown_on = p2_state[6];
      p2_fireleft_on = p2_state[7];
      p2_fireright_on = p2_state[8];


      //p1_bomb
      p1bomb_x = p1_bomb[9:0];
      p1bomb_y = p1_bomb[19:10];

      //p1_fire
      p1firecenter_x = p1_firecenter[9:0];
      p1firecenter_y = p1_firecenter[19:10];
      p1fireup_x = p1_fireup[9:0];
      p1fireup_y = p1_fireup[19:10];
      p1firedown_x = p1_firedown[9:0];
      p1firedown_y = p1_firedown[19:10];
      p1fireleft_x = p1_fireleft[9:0];
      p1fireleft_y = p1_fireleft[19:10];
      p1fireright_x = p1_fireright[9:0];
      p1fireright_y = p1_fireright[19:10];

      //p2_bomb
      p2bomb_x = p2_bomb[9:0];
      p2bomb_y = p2_bomb[19:10];

      //p2_fire
      p2firecenter_x = p2_firecenter[9:0];
      p2firecenter_y = p2_firecenter[19:10];
      p2fireup_x = p2_fireup[9:0];
      p2fireup_y = p2_fireup[19:10];
      p2firedown_x = p2_firedown[9:0];
      p2firedown_y = p2_firedown[19:10];
      p2fireleft_x = p2_fireleft[9:0];
      p2fireleft_y = p2_fireleft[19:10];
      p2fireright_x = p2_fireright[9:0];
      p2fireright_y = p2_fireright[19:10];

      //tile
      tile16_x = hcount[10:1] >> 4; //divide by 16 to get the x coordinate. 640/16 = 40
      tile16_y = vcount[9:0] >> 4; //divide by 16 to get the y coordinate. 480/16 = 30

      //map
      map_chipselect = map_info[31];
      map_write = map_info[30];
      if (map_chipselect == 1 & map_write == 1) begin
        map_en = 0;
	map_address = map_info[29:19];
        map_input = map_info[7:0];
      end
      else begin
      	map_en = 1;
	map_address = tile16_x + tile16_y * 40;
	map_input = 0;
      end

      //start
      start_on = p1_state[31] & p2_state[31];
      
    // dir:
    // 2'b00 --> down
    // 2'b01 --> up
    // 2'b10 --> left
    // 2'b11 --> right
    end
  
    //show p1
    always_ff @(posedge clk) begin
      if (hcount[10:1] >= p1_x && hcount[10:1] <= (p1_x + 10'd15) && vcount[9:0] >= p1_y && vcount[9:0] <= (p1_y + 10'd15) && p1_die_on == 0) begin
        
        p1_en <= 1;
	
	case(p1_dir)
	 2'b00 : p1_address <= hcount[10:1] - p1_x + (vcount[9:0] - p1_y) * 16;
         2'b01 : p1_address <= hcount[10:1] - p1_x + (vcount[9:0] - p1_y) * 16 + 256;
         2'b10 : p1_address <= hcount[10:1] - p1_x + (vcount[9:0] - p1_y) * 16 + 512;
         2'b11 : p1_address <= hcount[10:1] - p1_x + (vcount[9:0] - p1_y) * 16 + 768;
	endcase
      
      end

      else begin

        p1_en <= 0;

      end

    end

    //p1 die
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1_x && hcount[10:1] <= (p1_x + 10'd15) && vcount[9:0] >= p1_y && vcount[9:0] <= (p1_y + 10'd15) && p1_die_on == 1) begin
        
        p1_die_en <= 1;

        p1_die_address <= hcount[10:1] - p1_x + (vcount[9:0] - p1_y) * 16;
      
      end

      else begin

        p1_die_en <= 0;

      end

    end

    
    //show p2
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2_x && hcount[10:1] <= (p2_x + 10'd15) && vcount[9:0] >= p2_y && vcount[9:0] <= (p2_y + 10'd15) && p2_die_on == 0) begin
        
        p2_en <= 1;
	
	case(p2_dir)
	 2'b00 : p2_address <= hcount[10:1] - p2_x + (vcount[9:0] - p2_y) * 16;
         2'b01 : p2_address <= hcount[10:1] - p2_x + (vcount[9:0] - p2_y) * 16 + 256;
         2'b10 : p2_address <= hcount[10:1] - p2_x + (vcount[9:0] - p2_y) * 16 + 512;
         2'b11 : p2_address <= hcount[10:1] - p2_x + (vcount[9:0] - p2_y) * 16 + 768;
	endcase
      
      end

      else begin

        p2_en <= 0;

      end

    end
    
    //p2 die
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2_x && hcount[10:1] <= (p2_x + 10'd15) && vcount[9:0] >= p2_y && vcount[9:0] <= (p2_y + 10'd15) && p2_die_on == 1) begin
        
        p2_die_en <= 1;

        p2_die_address <= hcount[10:1] - p2_x + (vcount[9:0] - p2_y) * 16;
      
      end

      else begin

        p2_die_en <= 0;

      end

    end

    //show p1 bomb
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1bomb_x && hcount[10:1] <= (p1bomb_x + 10'd15) && vcount[9:0] >= p1bomb_y && vcount[9:0] <= (p1bomb_y + 10'd15) && p1_bomb_on == 1) begin
        
        p1_bomb_en <= 1;

        p1_bomb_address <= hcount[10:1] - p1bomb_x + (vcount[9:0] - p1bomb_y) * 16;
      
      end

      else begin

        p1_bomb_en <= 0;

      end

    end

    //show p2 bomb
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2bomb_x && hcount[10:1] <= (p2bomb_x + 10'd15) && vcount[9:0] >= p2bomb_y && vcount[9:0] <= (p2bomb_y + 10'd15) && p2_bomb_on == 1) begin
        
        p2_bomb_en <= 1;

        p2_bomb_address <= hcount[10:1] - p2bomb_x + (vcount[9:0] - p2bomb_y) * 16;
      
      end

      else begin

        p2_bomb_en <= 0;

      end

    end

    //show p1 bomb fire after explosion
    //p1 fire center
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1firecenter_x && hcount[10:1] <= (p1firecenter_x + 10'd15) && vcount[9:0] >= p1firecenter_y && vcount[9:0] <= (p1firecenter_y + 10'd15) && p1_firecenter_on == 1) begin
        
        p1_firecenter_en <= 1;

        p1_firecenter_address <= hcount[10:1] - p1firecenter_x + (vcount[9:0] - p1firecenter_y) * 16;
      
      end

      else begin

        p1_firecenter_en <= 0;

      end

    end


    //p1 fire up
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1fireup_x && hcount[10:1] <= (p1fireup_x + 10'd15) && vcount[9:0] >= p1fireup_y && vcount[9:0] <= (p1fireup_y + 10'd15) && p1_fireup_on == 1) begin
        
        p1_fireup_en <= 1;

        p1_fireup_address <= hcount[10:1] - p1fireup_x + (vcount[9:0] - p1fireup_y) * 16;
      
      end

      else begin

        p1_fireup_en <= 0;

      end

    end

    //p1 fire down
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1firedown_x && hcount[10:1] <= (p1firedown_x + 10'd15) && vcount[9:0] >= p1firedown_y && vcount[9:0] <= (p1firedown_y + 10'd15) && p1_firedown_on == 1) begin
        
        p1_firedown_en <= 1;

        p1_firedown_address <= hcount[10:1] - p1firedown_x + (vcount[9:0] - p1firedown_y) * 16;
      
      end

      else begin

        p1_firedown_en <= 0;

      end

    end

    //p1 fire left
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1fireleft_x && hcount[10:1] <= (p1fireleft_x + 10'd15) && vcount[9:0] >= p1fireleft_y && vcount[9:0] <= (p1fireleft_y + 10'd15) && p1_fireleft_on == 1) begin
        
        p1_fireleft_en <= 1;

        p1_fireleft_address <= hcount[10:1] - p1fireleft_x + (vcount[9:0] - p1fireleft_y) * 16;
      
      end

      else begin

        p1_fireleft_en <= 0;

      end

    end

    //p1 fire right
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p1fireright_x && hcount[10:1] <= (p1fireright_x + 10'd15) && vcount[9:0] >= p1fireright_y && vcount[9:0] <= (p1fireright_y + 10'd15) && p1_fireright_on == 1) begin
        
        p1_fireright_en <= 1;

        p1_fireright_address <= hcount[10:1] - p1fireright_x + (vcount[9:0] - p1fireright_y) * 16;
      
      end

      else begin

        p1_fireright_en <= 0;

      end

    end

    //show p2 bomb fire after explosion
    //p2 fire center
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2firecenter_x && hcount[10:1] <= (p2firecenter_x + 10'd15) && vcount[9:0] >= p2firecenter_y && vcount[9:0] <= (p2firecenter_y + 10'd15) && p2_firecenter_on == 1) begin
        
        p2_firecenter_en <= 1;

        p2_firecenter_address <= hcount[10:1] - p2firecenter_x + (vcount[9:0] - p2firecenter_y) * 16;
      
      end

      else begin

        p2_firecenter_en <= 0;

      end

    end


    //p2 fire up
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2fireup_x && hcount[10:1] <= (p2fireup_x + 10'd15) && vcount[9:0] >= p2fireup_y && vcount[9:0] <= (p2fireup_y + 10'd15) && p2_fireup_on == 1) begin
        
        p2_fireup_en <= 1;

        p2_fireup_address <= hcount[10:1] - p2fireup_x + (vcount[9:0] - p2fireup_y) * 16;
      
      end

      else begin

        p2_fireup_en <= 0;

      end

    end

    //p2 fire down
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2firedown_x && hcount[10:1] <= (p2firedown_x + 10'd15) && vcount[9:0] >= p2firedown_y && vcount[9:0] <= (p2firedown_y + 10'd15) && p2_firedown_on == 1) begin
        
        p2_firedown_en <= 1;

        p2_firedown_address <= hcount[10:1] - p2firedown_x + (vcount[9:0] - p2firedown_y) * 16;
      
      end

      else begin

        p2_firedown_en <= 0;

      end
    end

    //p2 fire left
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2fireleft_x && hcount[10:1] <= (p2fireleft_x + 10'd15) && vcount[9:0] >= p2fireleft_y && vcount[9:0] <= (p2fireleft_y + 10'd15) && p2_fireleft_on == 1) begin
        
        p2_fireleft_en <= 1;

        p2_fireleft_address <= hcount[10:1] - p2fireleft_x + (vcount[9:0] - p2fireleft_y) * 16;
      
      end

      else begin

        p2_fireleft_en <= 0;

      end

    end

    //p2 fire right
    always_ff @(posedge clk) begin

      if (hcount[10:1] >= p2fireright_x && hcount[10:1] <= (p2fireright_x + 10'd15) && vcount[9:0] >= p2fireright_y && vcount[9:0] <= (p2fireright_y + 10'd15) && p2_fireright_on == 1) begin
        
        p2_fireright_en <= 1;

        p2_fireright_address <= hcount[10:1] - p2fireright_x + (vcount[9:0] - p2fireright_y) * 16;
      
      end

      else begin

        p2_fireright_en <= 0;

      end

    end

    //map
    always_ff @(posedge clk) begin

      if (map_en == 1 && map_output == 1) begin

        fix_en <= 1;
        fix_address <= hcount[4:1] + vcount[3:0] * 16;

      end

      else begin

        fix_en <= 0;

      end

    end

    always_ff @(posedge clk) begin

      if (map_en == 1 && map_output == 2) begin

        wall_en <= 1;
        wall_address <= hcount[4:1] + vcount[3:0] * 16;

      end

      else begin

        wall_en <= 0;

      end

    end

    //start
    always_ff @(posedge clk) begin

      if (tile16_x >= 6'd12 && tile16_x <= 6'd27 && tile16_y == 6'd14 && start_on == 0) begin

        start_en <= 1;
        start_address <= hcount[4:1] + vcount[3:0] * 16 + (tile16_x - 6'd12) * 256;

      end

      else begin

        start_en <= 0;

      end

    end

    //p1_win
    always_ff @(posedge clk) begin

      if (tile16_x >= 6'd17 && tile16_x <= 6'd22 && tile16_y == 6'd14 && p2_die_on == 1) begin

        p1_win_en <= 1;
        p1_win_address <= hcount[4:1] + vcount[3:0] * 16 + (tile16_x - 6'd17) * 256;

      end

      else begin

        p1_win_en <= 0;

      end

    end

   //p2_win
    always_ff @(posedge clk) begin

      if (tile16_x >= 6'd17 && tile16_x <= 6'd22 && tile16_y == 6'd14 && p1_die_on == 1) begin

        p2_win_en <= 1;
        p2_win_address <= hcount[4:1] + vcount[3:0] * 16 + (tile16_x - 6'd17) * 256;

      end

      else begin

        p2_win_en <= 0;

      end

    end

   //Color Mapping
   always_comb begin
      {VGA_R, VGA_G, VGA_B} = {background_r, background_g, background_b};
      if (VGA_BLANK_n) begin
	if (p1_die_en) begin
	  {VGA_R, VGA_G, VGA_B} = {p1_die_output[15:12], 4'b0000, p1_die_output[11:8], 4'b0000, p1_die_output[7:4], 4'b0000};
        end
        else if (p2_win_en) begin
          {VGA_R, VGA_G, VGA_B} = {p2_win_output[15:12], 4'b0000, p2_win_output[11:8], 4'b0000, p2_win_output[7:4], 4'b0000};
        end
        else if (p2_die_en) begin
          {VGA_R, VGA_G, VGA_B} = {p2_die_output[15:12], 4'b0000, p2_die_output[11:8], 4'b0000, p2_die_output[7:4], 4'b0000};
        end
        else if (p1_win_en) begin
          {VGA_R, VGA_G, VGA_B} = {p1_win_output[15:12], 4'b0000, p1_win_output[11:8], 4'b0000, p1_win_output[7:4], 4'b0000};
        end
        else if (start_en) begin
          {VGA_R, VGA_G, VGA_B} = {start_output[15:12], 4'b0000, start_output[11:8], 4'b0000, start_output[7:4], 4'b0000};
        end
        else if (fix_en) begin
          {VGA_R, VGA_G, VGA_B} = {fix_output[15:12], 4'b0000, fix_output[11:8], 4'b0000, fix_output[7:4], 4'b0000};
        end
        else if (p1_en) begin
          {VGA_R, VGA_G, VGA_B} = {p1_output[15:12], 4'b0000, p1_output[11:8], 4'b0000, p1_output[7:4], 4'b0000};
        end
	else if (p2_en) begin
          {VGA_R, VGA_G, VGA_B} = {p2_output[15:12], 4'b0000, p2_output[11:8], 4'b0000, p2_output[7:4], 4'b0000};
        end
        else if (p1_firecenter_en) begin
	  {VGA_R, VGA_G, VGA_B} = {p1_firecenter_output[15:12], 4'b0000, p1_firecenter_output[11:8], 4'b0000, p1_firecenter_output[7:4], 4'b0000};
        end
        else if (p1_fireup_en) begin
	  {VGA_R, VGA_G, VGA_B} = {p1_fireup_output[15:12], 4'b0000, p1_fireup_output[11:8], 4'b0000, p1_fireup_output[7:4], 4'b0000};
        end
        else if (p1_firedown_en) begin
     	  {VGA_R, VGA_G, VGA_B} = {p1_firedown_output[15:12], 4'b0000, p1_firedown_output[11:8], 4'b0000, p1_firedown_output[7:4], 4'b0000};
        end
        else if (p1_fireleft_en) begin
	  {VGA_R, VGA_G, VGA_B} = {p1_fireleft_output[15:12], 4'b0000, p1_fireleft_output[11:8], 4'b0000, p1_fireleft_output[7:4], 4'b0000};
        end
        else if (p1_fireright_en) begin
          {VGA_R, VGA_G, VGA_B} = {p1_fireright_output[15:12], 4'b0000, p1_fireright_output[11:8], 4'b0000, p1_fireright_output[7:4], 4'b0000};
        end
        else if (p2_firecenter_en) begin
	  {VGA_R, VGA_G, VGA_B} = {p2_firecenter_output[15:12], 4'b0000, p2_firecenter_output[11:8], 4'b0000, p2_firecenter_output[7:4], 4'b0000};
        end
        else if (p2_fireup_en) begin
	  {VGA_R, VGA_G, VGA_B} = {p2_fireup_output[15:12], 4'b0000, p2_fireup_output[11:8], 4'b0000, p2_fireup_output[7:4], 4'b0000};
        end
        else if (p2_firedown_en) begin
     	  {VGA_R, VGA_G, VGA_B} = {p2_firedown_output[15:12], 4'b0000, p2_firedown_output[11:8], 4'b0000, p2_firedown_output[7:4], 4'b0000};
        end
        else if (p2_fireleft_en) begin
	  {VGA_R, VGA_G, VGA_B} = {p2_fireleft_output[15:12], 4'b0000, p2_fireleft_output[11:8], 4'b0000, p2_fireleft_output[7:4], 4'b0000};
        end
        else if (p2_fireright_en) begin
          {VGA_R, VGA_G, VGA_B} = {p2_fireright_output[15:12], 4'b0000, p2_fireright_output[11:8], 4'b0000, p2_fireright_output[7:4], 4'b0000};
        end
        else if (wall_en) begin
	  {VGA_R, VGA_G, VGA_B} = {wall_output[15:12], 4'b0000, wall_output[11:8], 4'b0000, wall_output[7:4], 4'b0000};
        end
        else if (p1_bomb_en) begin
	  {VGA_R, VGA_G, VGA_B} = {p1_bomb_output[15:12], 4'b0000, p1_bomb_output[11:8], 4'b0000, p1_bomb_output[7:4], 4'b0000};
        end
        else if (p2_bomb_en) begin
  	  {VGA_R, VGA_G, VGA_B} = {p2_bomb_output[15:12], 4'b0000, p2_bomb_output[11:8], 4'b0000, p2_bomb_output[7:4], 4'b0000};
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
