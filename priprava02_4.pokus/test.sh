#!/bin/bash

echo "=== GENERATING TEST FILES ==="
echo "Generating integers..."
./gennum 5 10 > inty.txt

echo "Generating floats..."
./gennum 3 15 -f > float.txt

echo "Generating hexadecimal numbers..."
./gennum 5 8 -h > hexa.txt

echo "Generating binary numbers..."
./gennum 3 12 -b > bin.txt

echo "Test files generated successfully!"
echo
echo "=== STARTING TESTS ==="
echo

# Změň do adresáře vernum
cd vernum

export LD_LIBRARY_PATH=dir1
echo "Zmena dir1"
echo "Test inty.txt"
./vernum < ../inty.txt
echo
echo "Test float.txt"
./vernum < ../float.txt
echo
echo "Test hexa.txt"
./vernum < ../hexa.txt
echo
echo "Test bin.txt"
./vernum < ../bin.txt
echo "--------------------------------"
export LD_LIBRARY_PATH=dir2
echo "Zmena dir2"
echo "Test inty.txt"
./vernum < ../inty.txt
echo 
echo "Test float.txt"
./vernum < ../float.txt
echo
echo "Test hexa.txt"
./vernum < ../hexa.txt
echo
echo "Test bin.txt"
./vernum < ../bin.txt
echo "--------------------------------"
export LD_LIBRARY_PATH=dir3
echo "Zmena dir3"
echo "Test inty.txt"
./vernum < ../inty.txt
echo
echo "Test float.txt"
./vernum < ../float.txt
echo
echo "Test hexa.txt"
./vernum < ../hexa.txt
echo
echo "Test bin.txt"
./vernum < ../bin.txt
echo "--------------------------------"
export LD_LIBRARY_PATH=dir4
echo "Zmena dir4"
echo "Test inty.txt"
./vernum < ../inty.txt
echo
echo "Test float.txt"
./vernum < ../float.txt
echo
echo "Test hexa.txt"
./vernum < ../hexa.txt
echo
echo "Test bin.txt"
./vernum < ../bin.txt