module top(clk, A, B, out);
    input clk, A, B;
    output reg out;
    wire temp;
    assign temp = out;
	always @(posedge clk)
	begin
		out <= A | (B & temp);
	end
endmodule