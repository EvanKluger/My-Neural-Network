Net.exe: useNeuralNet.o neural_network.o
	g++ -o Net.exe useNeuralNet.o neural_network.o

useNeuralNet.o: useNeuralNet.cpp neural_network.h
	g++ -c useNeuralNet.cpp

neural_network.o: neural_network.cpp neural_network.h
	g++ -c neural_network.cpp

debug:
	g++ -g -o NetDebug.exe useNeuralNet.cpp neural_network.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups