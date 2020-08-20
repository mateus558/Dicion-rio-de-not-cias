rm -r build
mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release ..
make -j 4

echo "\nCopying executables to project main folder..."
cp Experiments/exp ..
cp CLI/news_index ..
