set terminal svg
set output 'insertion_memory.svg'
set xlabel 'Number of documents'
set ylabel 'Time (ms)'
plot 'Hash/mem_hash.plt' using 1:2 with lines title 'Hash', \
     'TRIE/mem_trie.plt' using 1:2 with lines title 'TRIE'