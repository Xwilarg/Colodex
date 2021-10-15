# Colodex
Library to use Holodex in C

## Dependencies
[curl](https://github.com/curl/curl) 7.78.0 \
[cJSON](https://github.com/DaveGamble/cJSON) 1.7.15

## Install and build
```
mkdir build && cd build
conan install .. --build=missing
cmake ..
cmake --build .
```
