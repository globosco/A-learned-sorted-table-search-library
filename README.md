# A Learned Sorted Table Search Library

## Introduction

This library includes a collection of methods for performing element search in ordered tables, starting from textbook implementations to more complex algorithms. We also provide some real Benchmarking datasets ranging in size from a few kilobytes to a few Gigabytes.
For more information, we refer to the paper [1].

## Requirements

All Software has been tested on Ubuntu 20.04 LTS with the following features:

- gcc version 9.3.0
- Python 3.8.10
- [Intel Math Kernel Libary](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/onemkl.html)

In order to execute Python script, you need the to install dependencies using:

`pip install -r requirements.txt`

## Datasets

The datasets can be downloaded ...(Inserire repository)

## Usage

This Software can be compiled and execute as standalone software or can be included as library inside a C++ application.

### Include library

In order to include search procedure in a C++ application you have to copy and paste the include directory in your project folder.
Then, you can include the following files:

- StandardLayout.hpp in order to use BBS, BFS, K-BBS and K-BFS
- EytzeingerLayout.hpp in order to create an Eytzinger Layout [2] and use BFE
- VamEmdeBoaLayout.hpp in order to create a Van Emde Boas Layout [2] and use BFV
- BTreesLayout.hpp in order to create a B+-Tree Layout [2] and use BFT
- LrLearnedDS.hpp, QrLearnedDS.hpp and CrLearnedDS.hpp in order to use Linear, Quadratic or Cubic regression respectively

### Execute Test

### Execute Python Scripts

## Cite

If you use this library on your work, please cite us:

## Bibliography

[1] Nostro Articolo
[2] Morin



