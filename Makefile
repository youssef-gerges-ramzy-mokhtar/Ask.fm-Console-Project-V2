ask.exe: ask.cpp
	g++ ask.cpp -o ask
	ask.exe
	
clean:
	del *.exe