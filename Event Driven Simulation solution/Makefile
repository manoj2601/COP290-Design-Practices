all:
	gcc -fPIC -c tellerline.c
	gcc -fPIC -c EventQueue.c
	gcc -shared -o tellerline.so tellerline.o 
	gcc -shared -o EventQueue.so EventQueue.o
	gcc -c ass6.c -lm
	gcc -o qSim ass6.o tellerline.so EventQueue.so -lm
	sudo mv tellerline.so EventQueue.so /usr/lib
	sudo ldconfig
	rm -rf *.o
	
clean:
	rm -rf *.o
	rm -f qSim
	rm -f *.txt

run:
	rm -f gnuinputs.txt
	touch gnuinputs.txt
	@while read -r file; do \
		./qSim $$file; \
	done < testcase1
	gnuplot mygnucode.plt
# 	while read -r file; do
# 	echo ./qSim $$file && ./bin/qSim $$file;
# 	done <./testcase1
