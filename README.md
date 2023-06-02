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

Datasets and Query Files can be downloaded [here](https://osf.io/qtyu7/?view_only=b48e6cc6e01b441383b26b81588090ec)

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

In order to use library as standalone application, you can find some C++ example files inside the Test directory. In particular:

- mainStdTest.cpp is used to execute textbook search implementation and layout
- mainRgrTest.cpp is used to execute Regression and search
- mainOptimalSearchTest.cpp is used to execute KO-BFS or KO-BBS as described in [1]
- mainKbfsTest.cpp and mainKbbsTest.cpp is used to execute k-ary search in the branchy or brachfree version respectively, as described in [2]

To compile this files use:

```

gcc [file c++ path and name] -o [file executable path and name] -std=c++17 -fopenmp

```


### Execute Python Scripts

### How to modified PGM Library

In order to use PGM_M_a as described in our work, You have to change the row 92 of the [tuner](https://github.com/gvinciguerra/PGM-index/blob/master/tuner/tuner.cpp) with:

```
int a = 2
auto lo_eps = a * cache_line_size() / sizeof(int64_t);

```

where a is the PGM_M_a parameter

## Cite

If you use this library on your work, please cite us:

```
@Article{data8030056,
 AUTHOR = {Amato, Domenico and Giancarlo, Raffaele and Lo Bosco, Giosué},
 TITLE = {Learned Sorted Table Search and Static Indexes in Small-Space Data Models},
 JOURNAL = {Data},
 VOLUME = {8},
 YEAR = {2023},
 NUMBER = {3},
 ARTICLE-NUMBER = {56},
 URL = {https://www.mdpi.com/2306-5729/8/3/56},
 ISSN = {2306-5729},
 DOI = {10.3390/data8030056}
}

@InProceedings{10.1007/978-3-031-08421-8_32,
 author="Amato, Domenico and Lo Bosco, Giosu{\'e} and Giancarlo, Raffaele",
 title="Learned Sorted Table Search and Static Indexes in Small Model Space",
 booktitle="AIxIA 2021 -- Advances in Artificial Intelligence",
 year="2022",
 publisher="Springer International Publishing",
 address="Cham",
 pages="462--477",
 isbn="978-3-031-08421-8"
}

@article{amato2021learned,
  title={Learned sorted table search and static indexes in small space: Methodological and practical insights via an experimental study},
  author={Amato, Domenico and Giancarlo, Raffaele and Lo Bosco, Giosu{\`e} },
  journal={arXiv preprint arXiv:2107.09480},
  year={2021}
}

```

## Bibliography

[1] Amato, D., Giancarlo, R., & Bosco, G. L. (2021). Learned sorted table search and static indexes in small space: Methodological and practical insights via an experimental study. arXiv preprint arXiv:2107.09480.

[2] Khuong, P. V., & Morin, P. (2017). Array layouts for comparison-based searching. Journal of Experimental Algorithmics (JEA), 22, 1-39.



