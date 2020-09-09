set terminal svg
set output 'insertion_time.svg'
set xlabel 'Number of documents'
set ylabel 'Time (ms)'
plot 'Hash/time_hash.plt' using 1:2 with lines title 'Hash', \
     'TRIE/time_trie.plt' using 1:2 with lines title 'TRIE'