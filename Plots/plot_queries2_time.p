set terminal svg
set output 'queries_2_terms_time.svg'
set xlabel 'Number of documents'
set ylabel 'Time (ms)'
plot 'Hash/time_queries_2_hash.plt' using 1:2 with lines title 'Hash', \
     'TRIE/time_queries_2_trie.plt' using 1:2 with lines title 'TRIE'