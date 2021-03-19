#!/bin/sh

url=$1
name=${1##*/}
name=${name%%.*}

curl -Lo $name.cc "$url?raw=true"
