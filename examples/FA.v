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
	assign Co = t2 | t3;
endmodule
	
