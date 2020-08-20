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

./exp trie
mv *trie.svg Plots/TRIE
