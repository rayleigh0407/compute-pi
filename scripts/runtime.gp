reset
set ylabel 'time(sec)'
set xlabel 'N(thousands)'
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'performance_statistics.png'
set key left

plot \
'result_clock_gettime.csv' using 1:2 with lines linewidth 2 title 'Baseline', \
'result_clock_gettime.csv' using 1:3 with lines linewidth 2 title 'OpenMP(2thread)'  , \
'result_clock_gettime.csv' using 1:4 with lines linewidth 2 title 'OpenMP(4thread)'  , \
'result_clock_gettime.csv' using 1:5 with lines linewidth 2 title 'AVX'  , \
'result_clock_gettime.csv' using 1:6 with lines linewidth 2 title 'AVX+unrolling'
