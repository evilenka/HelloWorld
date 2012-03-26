#!/bin/bash

mkfifo pipe;
./task2  1> pipe;
read fbpid < pipe;
sleep 10s;
kill -INT -$fbpid;
rm pipe;
