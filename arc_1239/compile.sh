#!/usr/bin/env sh

./configure --prefix=/home/abcwow/abcwow_server --enable-optimize
make | tee compile_out.log
make install


