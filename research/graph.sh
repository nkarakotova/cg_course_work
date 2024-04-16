#!/opt/homebrew/bin/gnuplot -persist

reset
NO_ANIMATION = 1

set xlabel "Глубина трассировки"
set ylabel "Время, мс"
set grid

set key left box
set size 1,1

f(x) = a * x**5 + b * x**4 + c * x**3 + d * x**2 + e * x + f
fit f(x) 'research/result/result_1.txt' via a,b,c,d,e,f

g(x) = g * x**5 + h * x**4 + i * x**3 + j * x**2 + k * x + l
fit g(x) 'research/result/result_2.txt' via g,h,i,j,k,l

h(x) = m * x**5 + n * x**4 + o * x**3 + p * x**2 + q * x + r
fit h(x) 'research/result/result_3.txt' via m,n,o,p,q,r


plot f(x) title '' lt rgb 'navy', 'research/result/result_1.txt' pt 6  lt rgb 'navy' title 'Один пузырь', \
     g(x) title '' lt rgb 'coral', 'research/result/result_2.txt' pt 7 lt rgb 'coral' title 'Два пузыря' , \
     h(x) title '' lt rgb 'forest-green', 'research/result/result_3.txt' pt 8 lt rgb 'forest-green' title 'Три пузыря'