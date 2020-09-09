set terminal svg
set output 'queries_1_term_time.svg'
set xlabel 'Number of documents'
set ylabel 'Time (ms)'
plot 'Hash/time_queries_1_hash.plt' using 1:2 with lines title 'Hash', \
     'TRIE/time_queries_1_trie.plt' using 1:2 with lines title 'TRIE'