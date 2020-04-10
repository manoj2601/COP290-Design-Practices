all: ass6.c vector.c vector.c tellerline.h tellerline.c EventQueue.h EventQueue.c allstructs.h
# 	gcc -fPIC -c ass6.c
	gcc -fPIC -c vector.c
	gcc -fPIC -c tellerline.c
	gcc -fPIC -c EventQueue.c
# 	gcc -fPIC -c mult.c
	gcc -shared -o tellerline.so tellerline.o # extra.o divi.o
	gcc -shared -o vector.so vector.o
	gcc -shared -o EventQueue.so EventQueue.o
	gcc -c ass6.c -lm
	gcc -o main ass6.o vector.so tellerline.so EventQueue.so -lm
	sudo mv tellerline.so vector.so EventQueue.so /usr/lib
# 	sudo mv myheader.so /usr/lib/
	sudo ldconfig
	rm -rf *.o
	
clean:
	rm -rf *.o
	rm -f main
	rm -f costumers.txt
	rm -f tellers.txt
	rm -f output.txt
	rm -f output2.txt

run:
	./main 12 12 12 12
