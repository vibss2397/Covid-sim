all: mac

linux: 
	g++ -fopenmp Simulator.cpp Graph.cpp Person.cpp TestingDatabase.cpp Helper_Func.cpp ChartWriter.cpp -o simulator
mac: 
	g++ -std=c++17 Simulator.cpp Graph.cpp Person.cpp TestingDatabase.cpp Helper_Func.cpp ChartWriter.cpp -o simulator
windows: 
	g++ -fopenmp Simulator.cpp Graph.cpp Person.cpp TestingDatabase.cpp Helper_Func.cpp ChartWriter.cpp -o simulator.exe
clean: 
	rm -rf *.dat simulator simulator.exe