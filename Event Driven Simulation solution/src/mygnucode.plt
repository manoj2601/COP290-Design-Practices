set terminal pdf
set output "output//gnuplot.pdf"
set title "average time spent by a costumer versus number of tellers in Single Queue"
set xlabel "number of tellers"
set ylabel "average spent time(sec) by a customer"
set boxwidth 0.5
set style fill solid
plot "output//gnuinputs.txt" using 1:2 with boxes title "" lt rgb "#000090", "output//gnuinputs.txt" using 1:2 pt 7 ps 0.5 title "points", "output//gnuinputs.txt" using 1:2 smooth csplines title "" lt rgb "#F00000"
