set terminal pdf
set output "manoj.pdf"
set title "average amounts of time a customer spent in bank versus number of tellers in Single Queue"
set xlabel "number of tellers"
set ylabel "average spent time by a customer"
plot "gnuinputs.txt" using 1:2 with lines title "Time in seconds"
