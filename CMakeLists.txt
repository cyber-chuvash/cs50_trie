cmake_minimum_required(VERSION 3.14)
project(cs50_trie C)

set(CMAKE_C_STANDARD 11)

add_executable(speller speller.c dictionary.c)