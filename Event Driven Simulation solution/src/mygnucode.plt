set terminal pdf
set output "output//gnuplot.pdf"
set title "average time spent by a costumer versus number of tellers in Single Queue"
set xlabel "number of tellers"
set ylabel "average spent time by a customer"
plot "output//gnuinputs.txt" using 1:2 with lines title "Time in seconds"
