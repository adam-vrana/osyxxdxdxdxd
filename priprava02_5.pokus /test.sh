#!/bin/bash

echo "=== GENERATING TEST FILES ==="
echo "Generating integers..."
./gennum/gennum 5 10 > inty.txt

echo "Generating floats..."
./gennum/gennum 3 15 -f > float.txt

echo "Generating hexadecimal numbers..."
./gennum/gennum 5 8 -h > hexa.txt

echo "Generating binary numbers..."
./gennum/gennum 3 12 -b > bin.txt

echo "Test files generated successfully!"
echo
echo "=== STARTING TESTS ==="
echo

export LD_LIBRARY_PATH=dir1
echo "Changing export path to dir1"
echo "Test inty.txt with -d flag"
./vernum/vernum -d < inty.txt
echo
echo "Test float.txt with -d flag"
./vernum/vernum -d < float.txt
echo
echo "Test hexa.txt with -d flag"
./vernum/vernum -d < hexa.txt
echo
echo "Test bin.txt with -d flag"
./vernum/vernum -d < bin.txt
echo "--------------------------------"
export LD_LIBRARY_PATH=dir2
echo "Changing export path to dir2"
echo "Test inty.txt with -d flag"
./vernum/vernum -d < inty.txt
echo
echo "Test float.txt with -d flag"
./vernum/vernum -d < float.txt
echo
echo "Test hexa.txt with -d flag"
./vernum/vernum -d < hexa.txt
echo
echo "Test bin.txt with -d flag"
./vernum/vernum -d < bin.txt
echo "--------------------------------"
export LD_LIBRARY_PATH=dir3
echo "Changing export path to dir3"
echo "Test inty.txt with -d flag"
./vernum/vernum -d < inty.txt
echo
echo "Test float.txt with -d flag"
./vernum/vernum -d < float.txt
echo
echo "Test hexa.txt with -d flag"
./vernum/vernum -d < hexa.txt
echo
echo "Test bin.txt with -d flag"
./vernum/vernum -d < bin.txt
echo "--------------------------------"
export LD_LIBRARY_PATH=dir4
echo "Changing export path to dir4"
echo "Test inty.txt with -d flag"
./vernum/vernum -d < inty.txt
echo
echo "Test float.txt with -d flag"
./vernum/vernum -d < float.txt
echo
echo "Test hexa.txt with -d flag"
./vernum/vernum -d < hexa.txt
echo
echo "Test bin.txt with -d flag"
./vernum/vernum -d < bin.txt