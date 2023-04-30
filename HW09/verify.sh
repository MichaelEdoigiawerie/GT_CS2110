#!/bin/bash

if [ $(sha512sum list.h | cut -d " " -f 1) != c4b0c127967d8b6820bc4be54d997774cebb3634a6f81c4452cfca58e65712c1c9e6b19fc2a457def291ffcefcc7ca6a42c8e97fc8d5000cfb500000423fc0c2 ]; then
  >&2 echo "error: list.h was modified! re-download it and try again"
  exit 1
fi
