`include "delay.v"

//--------------------------------------------------------------
// mips.v
// David_Harris@hmc.edu and Sarah_Harris@hmc.edu 23 October 2005
// Single-cycle MIPS processor
//--------------------------------------------------------------

//---------------------------------//
// Author : Kim Ki Hoon				  //
//	E-mail : skywhite15@korea.ac.kr //
//	Date   : 2013. 11. 19			  //
//---------------------------------//

// files needed for simulation:
//  mipsttest.v
//  mipstop.v
//  mipsmem.v
//  mips.v
//  mipsparts.v

// single-cycle MIPS processor
module mips(input         clk, reset,
            output [31:0] pc,
            input  [31:0] instr,
            output 	cm_memwrite,				//kkh
            output [31:0] memaddr,
            output [31:0] memwritedata,
            input  [31:0] memreaddata,
				output [3:0]  membyteen);
			

  wire        signext, shiftl16, memtoreg, branch;  //modified beq -> branch 
  wire        alusrc, regdst, regwrite, jump, bne, jal, jr, memwrite;  //add new instruction controllers
  		  
  wire 		  [3:0]aluop;				
  wire		  [31:0]tocontrol;		//send instruction to ControlUnit from Datapath
  assign membyteen = 4'b1111;	
  
  // Instantiate Controller
  controller c(.op         (tocontrol[31:26]), 
			      .funct      (tocontrol[5:0]), 
					.signext    (signext), 
               .shiftl16   (shiftl16), 
               .memtoreg   (memtoreg), 
					.memwrite   (memwrite), 
					.branch     (branch),				//name modified beq -> branch
					.bne			(bne),
					.alusrc		(alusrc),				
               .aluop      (aluop), 
					.regdst     (regdst), 
					.regwrite   (regwrite), 
					.jump       (jump),
					.jal        (jal),
					.jr         (jr));

  // Instantiate Datapath
  datapath dp( .clk        (clk), 
			      .reset      (reset), 
					.signext    (signext), 
					.shiftl16   (shiftl16), 
					.memtoreg   (memtoreg),
					.memwrite   (memwrite),
					.cm_memwrite(cm_memwrite),			//send memwrite after execution Flipflop
					.branch    	(branch),
					.bne			(bne),
               .alusrc     (alusrc),					
					.regdst     (regdst), 
					.regwrite   (regwrite), 
					.jump       (jump),
					.jal			(jal),
					.jr	 	   (jr),
               .aluop 		(aluop),					//receive aluop from ControlUnit  
					.pc         (pc), 
					.instr      (instr),
					
               .m_aluout     (memaddr), 	//alu output after excuting
					.m_writedata  (memwritedata),	//data which will be written in memory
					.readdata   (memreaddata),			//read data from memory
					.tocontrol	(tocontrol));			//send instruction to ControlUnit from Datapath
 
endmodule

module controller(input  [5:0] op, funct,
                  output       signext,
                  output       shiftl16,
                  output       memtoreg, memwrite,
                  output       branch, bne,
                  output       regdst, regwrite,
                  output       jump, jal, alusrc, jr,
                  output [3:0] aluop);

  reg [14:0] controls;  //so reg controls' size should be 14 bit
  
  assign {signext, shiftl16, regwrite, regdst, 
			 alusrc, branch, bne, memwrite,
          memtoreg, jump, jal, aluop} = controls;

  always @(*)
    case(op)
      6'b000000: controls <= #`mydelay 15'b00110000000_0011; // Rtype
      6'b100011: controls <= #`mydelay 15'b10101000100_0000; // LW
      6'b101011: controls <= #`mydelay 15'b10001001000_0000; // SW
      6'b000100: controls <= #`mydelay 15'b10000100000_0001; // branch
		6'b000101: controls <= #`mydelay 15'b10000110000_0001; // BNE
      6'b001000, 
      6'b001001: controls <= #`mydelay 15'b10101000000_0000; // ADDI, ADDIU: only difference is exception
      6'b001101: controls <= #`mydelay 15'b00101000000_0010; // ORI
      6'b001111: controls <= #`mydelay 15'b01101000000_0000; // LUI
      6'b000010: controls <= #`mydelay 15'b00000000010_0000; // J
		6'b000011: controls <= #`mydelay 15'b00100000011_0000; // JAL
		6'b001010: controls <= #`mydelay 15'b10101000000_0100; //SLTi
      default:   controls <= #`mydelay 15'bxxxxxxxxxx_xxxx; // ???
    endcase

		assign jr = (op==6'b000000 && funct==6'b001000);	//jr controller operates


endmodule



module datapath(input         clk, reset,
                input         signext,
                input         shiftl16,
                input         memtoreg, branch, bne,
                input         alusrc, regdst,
                input         regwrite, jump, jal, jr, 
					 input  			memwrite,
					 
					 output 			cm_memwrite,
					 input  [3:0]  aluop, 						//receive aluop from Control Unit
                
                output [31:0] pc,
                input  [31:0] instr,
					 
                output [31:0] m_aluout,m_writedata,
                input  [31:0] readdata,
					 
					 output [31:0]	tocontrol
					 );

					 
  wire [31:0] writedata;
  wire [4:0]  writereg;
  wire [31:0] pcnext, pcnextbr, pcplus4, pcbranch;
  wire [31:0] signimm, signimmsh, shiftedimm;
  wire [31:0] srca, srcb;
  
  wire [31:0] result;
  wire        shift;
  wire		  zero;
  wire		  pcsrc;

  wire [31:0] regresult;			//jal
  wire [4:0]  jalwritereg;
  wire [31:0] pcnextjr;
  wire [3:0] alucontrol; 
  wire [31:0] aluout;
  
  //	decoding wire
  wire [31:0] d_pcout;
  wire [31:0] d_instrout;
  
  //	excuting wire
  wire [31:0] e_instrout;
  wire [31:0] e_pcout;
  wire [31:0] e_srca;
  wire [31:0] e_writedata;
  wire [31:0] e_shiftedimm;
  wire [4:0]  e_regdst_rs, e_regdst_rd, e_regdst_rt;
  wire [4:0]  e_regdst;
  
  //	memory access wire
  wire [31:0] 	m_pcbranch;
  wire [4:0] 	m_regdst;
  wire [31:0]  m_pcout;
  
  //	writeback wire
  wire [31:0] w_readdata;
  wire [31:0] w_aluout;
  wire [4:0]  w_regdst;
  wire [31:0] w_pcout;
  
  
  //  excute control wire
  wire ce_regwrite, ce_memtoreg, ce_branch, ce_bne, ce_memwrite, ce_regdst, ce_alusrc, ce_jal, ce_jump, ce_jr;
  wire [3:0] ce_aluop;
  
  //	memory access control wire
  wire cm_regwrite, cm_memtoreg, cm_jal;
  
  //	writeback wire
  wire cw_regwrite, cw_memtoreg, cw_jal;
  
  //	forwarding wire
  wire [1:0]srcacontrol, srcbcontrol;
  wire [31:0]forward_srca,forward_srcb;
  wire [31:0]forwardregdata_srca, forwardregdata_srcb;
  wire [1:0]forwardregdata_srca_control, forwardregdata_srcb_control;
  
  //	Stalling wire
  wire enable_pc, enable_fetch, enable_nopmux;
  
  //  Control hazard
  wire e_bneresult;
  wire flush;
	
  assign pcsrc = ce_branch && e_bneresult; //use bneresult which is made from bnemux
  assign tocontrol = d_instrout;   	//send instruction to control 
	

	
	mux2 #(1) e_bnemux	(.d0 (zero),
								 .d1 (~zero),
								 .s  (ce_bne),
								 .y  (e_bneresult));
	  
  
  //============================================================//
  //========================Hazard_unit=========================//
  //============================================================//
  hazard_unit	hazard_unit(.memtoreg		(ce_memtoreg),
									.e_rt				(e_regdst_rt),
									.d_rs				(d_instrout[25:21]),
									.d_rt				(d_instrout[20:16]),
									.pcsrc			(pcsrc),
									.jump				(ce_jump),
									.jr				(ce_jr),
									.enable_pc		(enable_pc),
									.enable_fetch	(enable_fetch),
									.enable_nopmux	(enable_nopmux),
									.flush			(flush));

							
  fowarding_Unit  fowarding_Unit(.w_rd		(w_regdst),
							  				.m_rd		(m_regdst),
											.e_rd    (e_regdst),
											.mem_regwrite  (cm_regwrite),
											.wb_regwrite   (cw_regwrite),
											.ex_regwrite	(ce_regwrite),
											.d_rs		(d_instrout[25:21]),
											.d_rt		(d_instrout[20:16]),
											.e_rs		(e_regdst_rs),
											.e_rt		(e_regdst_rt),
											.srcacontrol	(srcacontrol),
											.srcbcontrol	(srcbcontrol),
											.forwardregdata_srca_control	(forwardregdata_srca_control),
											.forwardregdata_srcb_control	(forwardregdata_srcb_control));
  //============================================================//
  
  //============================================================//
  //=======================Control Flipflops====================//
  //============================================================//
  flopclr #(14) flopr_condtrol_decode_execution ( 	.clk 		(clk),
																	.reset		(reset),
																	.clr		(enable_nopmux),
																	.d	    ({regwrite, memtoreg, branch, bne, memwrite,regdst,alusrc,aluop, jal, jump, jr}),
																	.q		({ce_regwrite, ce_memtoreg, ce_branch, ce_bne, ce_memwrite, ce_regdst, ce_alusrc, ce_aluop, ce_jal, ce_jump, ce_jr}));								
  
  flopr_control_excute_memory		flopr_condtrol_excute_memory
																(.clk (clk),
																 .reset (reset),
																 .d1 (ce_regwrite), .d2 (ce_memtoreg), .d3 (ce_memwrite), .d4  (ce_jal),
																 .q1 (cm_regwrite), .q2 (cm_memtoreg), .q3 (cm_memwrite), .q4  (cm_jal));
																 
  flopr_control_memory_wb			flopr_condtrol_memory_wb  (.clk  (clk),
																				.reset  (reset),
																				.d1	(cm_regwrite),
																				.d2	(cm_memtoreg),
																				.d3	(cm_jal),
																				.q1	(cw_regwrite),
																				.q2	(cw_memtoreg),
																				.q3	(cw_jal));
  //===========================================================================================//
  
  //============================================================//
  //======================PipeLine Flipflops====================//
  //============================================================//
  
  flopr_pipeline_fetch_decode #(32) flopr_pipeline_fetch_decode(.clk  (clk),
																					 .reset (reset),
																					 .clr		(flush),
 																					 .en (enable_fetch),
																					 .d1 (pcplus4),
																					 .d2 (instr),
																					 .q1 (d_pcout),
																					 .q2 (d_instrout));
	
  
  flopr_pipeline_decode_excution		flopr_pipeline_decode_excution(	.clk	(clk),
																							.reset (reset),
																							.d1  	(d_pcout),
																							.d2	(forwardregdata_srca),
																							.d3	(forwardregdata_srcb),
																							.d4	(shiftedimm[31:0]),
																							.d5	(d_instrout[20:16]),
																							.d6	(d_instrout[15:11]),
																							.d7	(d_instrout[25:21]),
																							.d8	(d_instrout),
																							.q1	(e_pcout),
																							.q2	(e_srca),
																							.q3	(e_writedata),
																							.q4	(e_shiftedimm), 
																							.q5	(e_regdst_rt),
																							.q6	(e_regdst_rd),
																							.q7	(e_regdst_rs),
																							.q8	(e_instrout));	

	flopr_pipeline_excution_memory   flopr_pipeline_excution_memory(	.clk		(clk),
																							.reset	(reset),
																							.d1		(pcbranch),
																							.d2		(aluout),
																							.d3		(srcb),			
																							.d4		(e_regdst),
																							.d5		(e_pcout),
																							.q1		(m_pcbranch),
																							.q2		(m_aluout),
																							.q3		(m_writedata),
																							.q4      (m_regdst),
																							.q5		(m_pcout));						 
										
  flopr_pipeline_memory_wb    flopr_pipeline_memory_wb (	.clk   (clk),
																			.reset  (reset),
																			.d1  (readdata),
																			.d2  (m_aluout),
																			.d3  (m_regdst),
																			.d4  (m_pcout),
																			.q1  (w_readdata),
																			.q2  (w_aluout),
																			.q3  (w_regdst),
																			.q4  (w_pcout));

  //============================================================//										 
 
  //================================================================//
  //==========================DataPath==============================//
  //================================================================//
  // next PC logic
  flopenr #(32) pcreg ( .clk   (clk),
								.en		(enable_pc),
								.reset (reset), 
								.d     (pcnext), 
								.q     (pc));

  adder       pcadd1 (.a (pc), 
			             .b (32'b100), 
						 	 .y (pcplus4));
	
  									
  sl2         immsh (.a (e_shiftedimm), 
			            .y (signimmsh));
				 
  adder       pcadd2 (.a (e_pcout), 
			             .b (signimmsh), 
							 .y (pcbranch));

  mux2 #(32)  pcbrmux(.d0  (pcplus4), 
			             .d1  (pcbranch), 
							 .s   (pcsrc), 
							 .y   (pcnextbr));
  	
  mux2 #(32)  pcmux (.d0   (pcnextbr), 
			            .d1   ({e_pcout[31:28], e_instrout[25:0], 2'b00}), 
                     .s    (ce_jump), 
							.y    (pcnextjr));
 
 //Connection of ALU Logic and PC for jr instruction
  mux2 #(32)  jrmux (.d0  (pcnextjr),
							.d1  (e_srca),   //$ra
							.s	  (ce_jr),
							.y   (pcnext));
							
  // register file logic
  regfile     rf(.clk     (clk), 
			        .we      (cw_regwrite), 
					  .ra1     (d_instrout[25:21]),
					  .ra2     (d_instrout[20:16]), 
					  .wa      (jalwritereg),      //jal instruction
                 .wd      (regresult), 		 //jal instruction
					  .rd1     (srca), 
					  .rd2     (writedata));
 
  mux4 #(32)  forwardsrca_mux(.d0 (srca), 				// on writeback stage.
										.d1 (aluout), 
										.d2 (m_aluout),
										.d3 (result),
										.s  (forwardregdata_srca_control), 
										.y  (forwardregdata_srca));

  mux4 #(32)  forwardsrc2_mux(.d0 (writedata), 	
										.d1 (aluout), 
										.d2 (m_aluout),
										.d3 (result),
										.s  (forwardregdata_srcb_control), 
										.y  (forwardregdata_srcb));
  
  aludec  ad( .funct      (e_shiftedimm[5:0]), 
			     .aluop      (ce_aluop), 
				  .alucontrol (alucontrol));
  
  mux2 #(5)   wrmux(.d0  (e_regdst_rt), 
			           .d1  (e_regdst_rd), 
						  .s   (ce_regdst), 
						  .y   (e_regdst));

						  
  mux2 #(32)  resmux(.d0 (w_aluout), 	
			            .d1 (w_readdata), 
							.s  (cw_memtoreg), 
							.y  (result));
							
  mux2 # (32)  jalmux1(.d0 (result),         //register data
							  .d1 (w_pcout),			//$ra <= pcplus4
							  .s  (cw_jal),
							  .y  (regresult));
							  
  mux2 # (5)  jalmux2 (.d0 (w_regdst),			//register number
							  .d1 (5'b11111),			//$ra
							  .s  (cw_jal),
							  .y  (jalwritereg));
							  
  sign_zero_ext  sze(.a       (d_instrout[15:0]), 
			            .signext (signext),
			            .y       (signimm[31:0]));

  shift_left_16 sl16(.a         (signimm[31:0]), 
			            .shiftl16  (shiftl16),
			            .y         (shiftedimm[31:0]));

  // ALU logic
  mux2 #(32)  srcbmux(.d0 (srcb), 
			             .d1 (e_shiftedimm[31:0]), 
							 .s  (ce_alusrc), 
							 .y  (forward_srcb));
							 
  mux3 #(32)  forwardsrca(.d0 (e_srca),
								  .d1 (m_aluout),
								  .d2 (result),
								  .s  (srcacontrol),
								  .y  (forward_srca));
								 
  mux3 #(32)  forwardsrcb(.d0 (e_writedata),
								  .d1 (m_aluout),
								  .d2 (result),
								  .s  (srcbcontrol),
								  .y  (srcb));
								  
  alu         alu( .a       (forward_srca), 
			          .b       (forward_srcb), 
						 .alucont (alucontrol),
						 .result  (aluout), 
						 .zero    (zero));						 
						 
endmodule


module aludec(input      [5:0] funct,
              input      [3:0] aluop,
              output reg [3:0] alucontrol
				  );
  always @(*) 
    case(aluop)
      4'b0000: alucontrol <= #`mydelay 4'b0010; // add
      4'b0001: alucontrol <= #`mydelay 4'b1010; // sub
      4'b0010: alucontrol <= #`mydelay 4'b0001; // or
		4'b0100: alucontrol <= #`mydelay 4'b1011; // slti
      default: case(funct)          // RTYPE instructions
			 6'b100000,
          6'b100001: alucontrol <= #`mydelay 4'b0010;// ADD, ADDU: only difference is exception
          6'b100010,
          6'b100011: alucontrol <= #`mydelay 4'b1010;// SUB, SUBU: only difference is exception
          6'b100100: alucontrol <= #`mydelay 4'b0000;// AND
          6'b100101: alucontrol <= #`mydelay 4'b0001;// OR
          6'b101010: alucontrol <= #`mydelay 4'b1011;// SLT
			 6'b101011: alucontrol <= #`mydelay 4'bx100;// SLTU
          default:   alucontrol <= #`mydelay 4'bxxxx;// ???
        endcase
    endcase
	 
endmodule	

module fowarding_Unit(input [4:0]w_rd,
							 input [4:0]m_rd,
							 input [4:0]e_rd,
							 input [4:0]e_rs,
							 input [4:0]e_rt,
							 input [4:0]d_rs,
							 input [4:0]d_rt,
							 input wb_regwrite,
							 input mem_regwrite,
							 input ex_regwrite,
							 output reg [1:0]srcacontrol,
							 output reg [1:0]srcbcontrol,
							 output reg [1:0]forwardregdata_srca_control,
							 output reg [1:0]forwardregdata_srcb_control);
			
	always @(*)
		begin
			if((m_rd == e_rs) && mem_regwrite)
				srcacontrol = 2'b01;
			else if((w_rd == e_rs) && wb_regwrite)
				srcacontrol = 2'b10;
			else
				srcacontrol = 2'b00;
		end
		
	always @(*)
		begin
			if((m_rd==e_rt)&&mem_regwrite)
				srcbcontrol = 2'b01;
			else if((w_rd==e_rt)&&wb_regwrite)
				srcbcontrol = 2'b10;
			else
				srcbcontrol = 2'b00;
		end
		
	always @(*)
		begin
			if((d_rs==e_rd)&&ex_regwrite)
				forwardregdata_srca_control=2'b01;
			else if((d_rs==m_rd)&&mem_regwrite)
				forwardregdata_srca_control=2'b10;
			else if((d_rs==w_rd)&&wb_regwrite)
				forwardregdata_srca_control=2'b11;
			else
				forwardregdata_srca_control=2'b00;
		end
	always @(*)
		begin
			if((d_rt==e_rd)&&ex_regwrite)
				forwardregdata_srcb_control=2'b01;
			else if((d_rt==m_rd)&&mem_regwrite)
				forwardregdata_srcb_control=2'b10;
			else if((d_rt==w_rd)&&wb_regwrite)
				forwardregdata_srcb_control=2'b11;
			else
				forwardregdata_srcb_control=2'b00; 
		end
		
endmodule

module hazard_unit(input memtoreg,
						 input [4:0]e_rt,
						 input [4:0]d_rs,
						 input [4:0]d_rt,
						 input pcsrc,jump,jr,
						 output reg enable_pc,
						 output reg enable_fetch,
						 output reg enable_nopmux,
						 output reg flush);

	always @(*)
		if(((e_rt==d_rs)||(e_rt==d_rt))&&memtoreg)
				begin	enable_pc= 1'b0; enable_fetch= 1'b0; enable_nopmux=1'b1;end
		else
				begin	enable_pc= 1'b1; enable_fetch= 1'b1; enable_nopmux=1'b0;end
	always @(*)
		if(pcsrc||jump||jr)
				flush = 1'b1;
		else
				flush = 1'b0;
endmodule