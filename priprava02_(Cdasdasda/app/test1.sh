#!/bin/bash
export LD_LIBRARY_PATH=../dir1
echo "Zmena dir1"
echo "Test inty.txt"
./vernum/vernum < vernum/inty.txt
echo
echo "Test float.txt"
./vernum/vernum < vernum/float.txt
echo
echo "Test hexa.txt"
./vernum/vernum < vernum/hexa.txt
echo
echo "Test bin.txt"
./vernum/vernum < vernum/bin.txt
echo "--------------------------------"
export LD_LIBRARY_PATH=../dir2
echo "Zmena dir2"
echo "Test inty.txt"
./vernum/vernum < vernum/inty.txt
echo 
echo "Test float.txt"
./vernum/vernum < vernum/float.txt
echo
echo "Test hexa.txt"
./vernum/vernum < vernum/hexa.txt
echo
echo "Test bin.txt"
./vernum/vernum < vernum/bin.txt
echo "--------------------------------"
export LD_LIBRARY_PATH=../dir3
echo "Zmena dir3"
echo "Test inty.txt"
./vernum/vernum < vernum/inty.txt
echo
echo "Test float.txt"
./vernum/vernum < vernum/float.txt
echo
echo "Test hexa.txt"
./vernum/vernum < vernum/hexa.txt
echo
echo "Test bin.txt"
./vernum/vernum < vernum/bin.txt
echo "--------------------------------"
export LD_LIBRARY_PATH=../dir4
echo "Zmena dir4"
echo "Test inty.txt"
./vernum/vernum < vernum/inty.txt
echo
echo "Test float.txt"
./vernum/vernum < vernum/float.txt
echo
echo "Test hexa.txt"
./vernum/vernum < vernum/hexa.txt
echo
echo "Test bin.txt"
./vernum/vernum < vernum/bin.txt