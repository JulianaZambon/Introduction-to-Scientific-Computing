#!/bin/bash

if [ -z "$1" ]
then
  echo "uso: $0 login.tgz";
  exit 1;
fi

make verificaEP02 > /dev/null;

FILE=$1;
DIR=$(echo $FILE | rev | cut -d '/' -f 1 | rev | cut -d '.' -f 1)

echo "----------";
echo $DIR;

tar -xzf $FILE;
cd $DIR;
make > /dev/null;

echo "----------";
cat ../edo.dat | ./resolveEDO | ../verificaEP02;
echo "----------";

make clean > /dev/null;
cd ..;
make clean > /dev/null;
