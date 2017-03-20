# Dependencies
This code has been tested on ubuntu and os x with g++ and clang++.

The c++ part of this code depends on gsl.
The python part of this code used to generate the figures depends on numpy, scipy, matplotlib, basemap, and astropy.

## *c++* dependencies
To install gsl execute the following:

* ubuntu 16.04
```sh
sudo apt-get update
sudo apt-get install libgsl-dev
```
* ubuntu 12.04 or ubuntu 14.04
```sh
sudo apt-get update
sudo apt-get install libgsl0-dev
```
* mac os x
```sh
brew update
brew install gsl
```

## *python* dependencies
For the python installations use the python installer of your choice (such as pip). For a package installation execute the following:
* ubuntu
```sh
sudo apt-get update
sudo apt-get install python-numpy python-scipy python-matplotlib python-mpltoolkits.basemap python-astropy
```
* mac os x
```sh
brew update
brew install numpy scipy matplotlib matplotlib-basemap
pip install astropoy
```

## Support
If you have questions or encounter any problems when running *ANA*, please use github's [issue tracker](https://github.com/PeterDenton/ANA/issues).

This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code [DOI:10.5281/zenodo.x](zenodo.org/record/x) and the paper [arXiv:17xx.xxxxx](https://arxiv.org/abs/17xx.xxxxx).
