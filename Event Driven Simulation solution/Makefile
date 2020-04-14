all:
	gcc -fPIC -c ./src/tellerline.c
	gcc -fPIC -c ./src/EventQueue.c
	gcc -shared -o tellerline.so tellerline.o 
	gcc -shared -o EventQueue.so EventQueue.o
	gcc -c ./src/ass6.c -lm
	gcc -o qSim ass6.o tellerline.so EventQueue.so -lm
	#sudo mv tellerline.so EventQueue.so /usr/lib
	#sudo ldconfig
	mv *.o obj
	cp qSim bin
	mv *so obj
	#rm -rf *.o
	
clean:
	rm -rf output
	rm -rf obj
	rm -rf bin
	mkdir bin
	mkdir obj
	mkdir output

run:
	rm -f output/gnuinputs.txt output/output.txt
	touch output/gnuinputs.txt output/output.txt
	@while read -r file; do \
		./bin/qSim $$file; \
	done < testcases/testcase1
	gnuplot src/mygnucode.plt

run_test2:
	rm -f output/gnuinputs.txt output/output.txt
	touch output/gnuinputs.txt output/output.txt
	@while read -r file; do \
		./bin/qSim $$file; \
	done < testcases/testcase2
	gnuplot src/mygnucode.plt

run_test3:
	rm -f output/gnuinputs.txt output/output.txt
	touch output/gnuinputs.txt output/output.txt
	@while read -r file; do \
		./bin/qSim $$file; \
	done < testcases/testcase3
	gnuplot src/mygnucode.plt
# 	while read -r file; do
# 	echo ./qSim $$file && ./bin/qSim $$file;
# 	done <./testcase1
