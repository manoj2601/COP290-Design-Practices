all:
	gcc -o plagChecker src/main.c -lm
	cp plagChecker bin/plagChecker

clean:
	rm -f plagChecker bin/plagChecker

run:
	./plagChecker corpus_files/catchmeifyoucan.txt corpus_files
