rm ../upload/*
cp ./*.cpp ../upload/
cp ./*.hpp ../upload/
cp ./CMakeLists.txt ../upload/
cp ./CodeCraft_zip.sh ../upload/
../upload/CodeCraft_zip.sh
mv ../upload/CodeCraft-2023.zip ../
