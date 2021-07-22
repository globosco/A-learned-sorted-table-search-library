# A Learned Sorted Table Search Library

## Introduction

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

In order to include search procedure in a C++ application you can copy and paste the include directory in your project folder.
Then, you can include the following files:

- StandardLayout.hpp in order to use BBS, BFS, K-BBS and K-BFS
- EytzeingerLayout.hpp in order to create an Eytzinger Layout and use BFE
- VamEmdeBoaLayout.hpp in order to create a Van Emde Boas Layout and use BFV
- BTreesLayout.hpp in order to create a B+-Tree Layout and use BFT
- LrLearnedDS.hpp, QrLearnedDS.hpp and CrLearnedDS.hpp in order to use Linear, Quadratic or Cubic regression respectively

### Execute Test

### Execute Python Scripts
