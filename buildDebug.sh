BUILD_DIR="./build"
[[ -d "${BUILD_DIR}" ]] || mkdir ${BUILD_DIR}
cd ${BUILD_DIR}

cmake -DCMAKE_BUILD_TYPE=Debug ..
echo
make -j 4

echo
echo "Copying executables to project main folder..."
echo
cp Experiments/exp ..
cp CLI/news_index ..
cp teste ..
cd ..
gdb ./news_index