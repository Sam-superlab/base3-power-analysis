set terminal png size 1200,800
set output 'frequency_plot.png'
set title 'Digit Frequencies in Powers of 2 (Base 3)'
set xlabel 'Power of 2'
set ylabel 'Frequency'
set grid
set key outside
set datafile separator ','
set style fill solid
plot 'frequency_data.csv' using 1:2 title '0s' with points pt 7 ps 1.5 lc rgb '#FF0000',\
     'frequency_data.csv' using 1:3 title '1s' with points pt 7 ps 1.5 lc rgb '#0000FF',\
     'frequency_data.csv' using 1:4 title '2s' with points pt 7 ps 1.5 lc rgb '#00FF00'
