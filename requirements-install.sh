sudo apt-get update -y
sudo apt install curl
sudo apt-get install -y libcurlpp-dev
sudo apt update
sudo apt install libpqxx-dev
cd ../..
git clone https://github.com/cpp-redis/cpp_redis.git
cd cpp_redis
git submodule init && git submodule update
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
make install