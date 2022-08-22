#!/bin/bash
g++ -o encode encode.cc ../base64_encoder.cc
g++ -o decode decode.cc ../base64_decoder.cc

for file in $(find raw-files -type f -print); do
  echo "file: $file"
  echo "*** encode test ***"
  ./encode <$file >x0 
  openssl enc -e -base64 <$file >x1
  if ! cmp x0 x1; then
    exit 1
  fi
  echo "*** encode test end ***"

  echo "*** decode test ***"
  ./decode <x0 >dx0
  openssl enc -d -base64 <x1 >dx1
  cmp dx0 dx1
  if ! cmp x0 x1; then 
    exit 1
  fi
  echo "*** decode test end ***"
done

echo 
echo "*** All tests pass ***"
echo 

echo "Remove files used for tests"
rm x0 x1 dx0 dx1
echo "Remove successfully"
