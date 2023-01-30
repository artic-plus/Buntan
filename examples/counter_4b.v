module HA(A, B, S, C);
	input A,B;
	output S,C;
	assign C = A & B;
	assign S = A ^ B;
endmodule


module FA(A,B,Ci,S,Co);
	input A,B,Ci;
	output S,Co;
	wire t1,t2,t3;
	HA HA1(A,B,t1,t2);
	HA HA2(t1,Ci,S,t3);
	assign Co = t2 & t3;
endmodule
	

module adder_4b(A,B,S,overflow);
	input [3:0] A,B;
	output [3:0] S;
	output overflow;
	wire [4:0] C;
	FA FA0(A[0],B[0],C[0],S[0],C[1]); 
	FA FA1(A[1],B[1],C[1],S[1],C[2]);
	FA FA2(A[2],B[2],C[2],S[2],C[3]);
	FA FA3(A[3],B[3],C[3],S[3],C[4]);
	assign overflow = C[3];
	assign C[0] = 0;
endmodule

module counter_4b(clk,out,overflow);
	input clk;
	output reg[3:0] out;
	output overflow;
	wire[3:0] count;
	adder_4b adder(4'b0001,out,count,overflow);
	always @(posedge clk)
	begin
		out <= count;
	end
endmodule
	
