# Install Guide
Once the [dependcies](DEPENDENCY.md) are met, clone into this repo or download a zip. To compile simply run `make` (or `make -j$(nproc)` for parallel make), and to execute the file run `./main`. Plotting is then done by `python py/Likelihood.py` and so on. Comment out different parts of [*src/main.cpp*](src/main.cpp) to change what is run. To incorporate into your own code, include [*include/Likelihood.h*](include/Likelihood.h) which contains the main likelihood functions.

## Support
If you have questions or encounter any problems when running *ANA*, please use github's [issue tracker](https://github.com/PeterDenton/ANA/issues).

This code is free to use, copy, distribute, and modify.
If you use this code or any modification of this code, we request that you reference both this code [DOI:10.5281/zenodo.438675](https://zenodo.org/record/438675) and the paper [arXiv:17xx.xxxxx](https://arxiv.org/abs/17xx.xxxxx).
