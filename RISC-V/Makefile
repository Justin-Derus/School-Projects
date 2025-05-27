#
# PROGRAM: RV32I Simulator
# AUTHOR:  Justin Derus
#

rv32i: cpu_single_hart.cpp cpu_single_hart.h hex.cpp main.cpp hex.h memory.cpp memory.h registerfile.cpp registerfile.h rv32i_decode.cpp rv32i_decode.h rv32i_hart.cpp rv32i_hart.h
	g++ -std=c++11 -o rv32i cpu_single_hart.cpp hex.cpp main.cpp memory.cpp registerfile.cpp rv32i_decode.cpp rv32i_hart.cpp

clean:
		rm rv32i