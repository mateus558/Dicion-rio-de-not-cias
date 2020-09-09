BUILD_DIR="./build"
[[ -d "${BUILD_DIR}" ]] || mkdir ${BUILD_DIR}
cd ${BUILD_DIR}

cmake -DCMAKE_BUILD_TYPE=Release ..
echo
make -j 4
echo
echo "Copying executables to project main folder..."
echo
cp Experiments/exp ..
cd ..

./exp hash
mv *hash.svg Plots/Hash
mv *plt Plots/Hash

./exp trie
mv *plt Plots/TRIE
mv *trie.svg Plots/TRIE

echo
echo "Generating plots on Plots folder..."
cd Plots
gnuplot plot_mem.p
gnuplot plot_in_time.p
gnuplot plot_queries1_time.p
gnuplot plot_queries2_time.p
echo
echo "Done!"