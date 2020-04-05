# async-rpc

Install the [googletest](https://github.com/google/googletest) library via apt.

```bash
sudo apt install libgtest-dev
cd /usr/src/googletest
sudo mkdir build
cd build
sudo cmake ..
sudo make -j4
sudo make install
```

valgrind --leak-check=yes --track-origins=yes --log-file=valgrind-result.txt ./build/test/arpc-test
