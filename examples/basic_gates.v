module dff (CK,Q,D);
input CK,D;
output Q;

  always @(posedge CK) Q <= D;

endmodule


module NA2(Y,A,B);
	output Y;
	input A,B;
	assign Y = !(A & B);
endmodule

module NA3(Y,A,B,C);
	output Y;
	input A,B,C;
	assign Y = !(A & B & C);
endmodule

module NA4(Y,A,B,C,D);
	output Y;
	input A,B,C,D;
	assign Y = !(A & B & C & D);
endmodule


module nand2(Y,A,B);
	output Y;
	input A,B;
	assign Y = !(A & B);
endmodule

module nand3(Y,A,B,C);
	output Y;
	input A,B,C;
	assign Y = !(A & B & C);
endmodule

module nand4(Y,A,B,C,D);
	output Y;
	input A,B,C,D;
	assign Y = !(A & B & C & D);
endmodule

module AND2(Y,A,B);
	output Y;
	input A,B;
	assign Y = A & B;
endmodule

module AND3(Y,A,B,C);
	output Y;
	input A,B,C;
	assign Y = A & B & C;
endmodule


module and2(Y,A,B);
	output Y;
	input A,B;
	assign Y = A & B;
endmodule

module and3(Y,A,B,C);
	output Y;
	input A,B,C;
	assign Y = A & B & C;
endmodule

module OR2(Y,A,B);
	output Y;
	input A,B;
	assign Y = A | B;
endmodule

module or2(Y,A,B);
	output Y;
	input A,B;
	assign Y = A | B;
endmodule

module NO2(Y,A,B);
	output Y;
	input A,B;
	assign Y = !(A | B);
endmodule

module nor2(Y,A,B);
	output Y;
	input A,B;
	assign Y = !(A | B);
endmodule


module IN1(Y,A);
	output Y;
	input A;
	assign Y = !A;
endmodule

module inv(Y,A);
	output Y;
	input A;
	assign Y = !A;
endmodule

module BU1(Q,D);
	output Q;
	input D;
	buf buf1(Q,D);
endmodule


module buffer(Q,D);
	output Q;
	input D;
	buf buf1(Q,D);
endmodule
