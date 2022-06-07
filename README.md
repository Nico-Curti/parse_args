| **Authors**  | **Project** |
|:------------:|:-----------:|
| [**N. Curti**](https://github.com/Nico-Curti) |  **parse args**  |

| **Windows CI** | **MacOS CI** | **Linux CI** |
|:--------------:|:------------:|:------------:|
| [![Windows CI](https://github.com/Nico-Curti/parse_args/actions/workflows/windows.yml/badge.svg)](https://github.com/Nico-Curti/parse_args/actions/workflows/windows.yml) | [![MacOS CI](https://github.com/Nico-Curti/parse_args/actions/workflows/macos.yml/badge.svg)](https://github.com/Nico-Curti/parse_args/actions/workflows/macos.yml) | [![Linux CI](https://github.com/Nico-Curti/parse_args/actions/workflows/linux.yml/badge.svg)](https://github.com/Nico-Curti/parse_args/actions/workflows/linux.yml) |

[![GitHub pull-requests](https://img.shields.io/github/issues-pr/Nico-Curti/parse_args.svg?style=plastic)](https://github.com/Nico-Curti/parse_args/pulls)
[![GitHub issues](https://img.shields.io/github/issues/Nico-Curti/parse_args.svg?style=plastic)](https://github.com/Nico-Curti/parse_args/issues)

[![GitHub stars](https://img.shields.io/github/stars/Nico-Curti/parse_args.svg?label=Stars&style=social)](https://github.com/Nico-Curti/parse_args/stargazers)
[![GitHub watchers](https://img.shields.io/github/watchers/Nico-Curti/parse_args.svg?label=Watch&style=social)](https://github.com/Nico-Curti/parse_args/watchers)

**C supported compilers:**

![gcc version](https://img.shields.io/badge/gcc-4.9.*|5.*|6.*|7.*|8.*|9.*|10.*-yellow.svg)

![clang version](https://img.shields.io/badge/clang-3.*|4.*|5.*|6.*|7.*|8.*|9.*|10.*-red.svg)

![msvc version](https://img.shields.io/badge/msvc-vs2017%20x86%20|%20vs2017%20x64|%20vs2019%20x86%20|%20vs2019%20x64-blue.svg)

<a href="https://github.com/UniboDIFABiophysics">
  <div class="image">
    <img src="https://cdn.rawgit.com/physycom/templates/697b327d/logo_unibo.png" width="90" height="90">
  </div>
</a>

# Parse Command Line

## Simple command line parser in C++

This project implements a simple command line parser for C++ softwares inspired to the [argparse](https://docs.python.org/3/library/argparse.html) Python package.

* [Installation](#installation)
* [Contribution](#contribution)
* [Authors](#authors)
* [License](#license)
* [Acknowledgments](#acknowledgments)
* [Citation](#citation)

## Installation

First of all download the project or the latest release:

```bash
git clone https://github.com/Nico-Curti/parse_args
cd parse_args
git submodule update --init --recursive
```

The you can manually build the library using the [CMake](https://github.com/Nico-Curti/parse_args/blob/main/CMakeLists.txt) script or you can use the *build* scripts of the project.
Example:

```bash
./build.sh
```

Note: The `build.sh` script works on Linux and macOS while the `build.ps1` is reserved for the Windows users.

### Import the library into other CMake project

The CMake script installation automatically creates the files required for the import of the library into other CMakeLists.

Into your CMake file you can just use the commands

```bash
find_package(ParseArgs REQUIRED)
include_directories (${ParseArgs_INCLUDE_DIR})
list (APPEND linked_libs ParseArgs::parseargs)
```

If your system doesn't automatically find the correct include path you can provided it in the CMake command line using the `-DPARSE_ARGS_DIR:FILEPATH="/path/to/parseargs/share/parseargs/"` variable.

### Import the library using pkg-config

The CMake script installation automatically creates the pkg file required for the import of the library.

First of all install the `pkg-config` if it is not yet installed:

```bash
sudo apt install pkg-config
```

Then you can move the `parseargs.pc` file using

```bash
sudo mkdir -p /usr/local/lib/pkgconfig
sudo mv parseargs.pc /usr/local/lib/pkgconfig/
```

## Contribution

Any contribution is more than welcome :heart:. Just fill an [issue](https://github.com/Nico-Curti/parse_args/blob/main/.github/ISSUE_TEMPLATE/ISSUE_TEMPLATE.md) or a [pull request](https://github.com/Nico-Curti/parse_args/blob/main/.github/PULL_REQUEST_TEMPLATE/PULL_REQUEST_TEMPLATE.md) and we will check ASAP!

See [here](https://github.com/Nico-Curti/parse_args/blob/main/.github/CONTRIBUTING.md) for further informations about how to contribute with this project.

## Authors

* <img src="https://avatars0.githubusercontent.com/u/24650975?s=400&v=4" width="25px"> **Nico Curti** [git](https://github.com/Nico-Curti), [unibo](https://www.unibo.it/sitoweb/nico.curti2)

See also the list of [contributors](https://github.com/Nico-Curti/parse_args/contributors) [![GitHub contributors](https://img.shields.io/github/contributors/Nico-Curti/parse_args.svg?style=plastic)](https://github.com/Nico-Curti/parse_args/graphs/contributors/) who participated in this project.

## License

The `parse_args` package is licensed under the MIT "Expat" License. [![License](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/Nico-Curti/parse_args/blob/main/LICENSE)

## Acknowledgments

Thanks goes to all contributors of this project.

## Citation

If you have found `parse_args` helpful in your research, please consider citing this project repository

```BibTeX
@misc{ParseArgs,
  author = {Curti, Nico},
  title = {ParseArgs - Simple command line parser in C++},
  year = {2021},
  publisher = {GitHub},
  howpublished = {\url{https://github.com/Nico-Curti/parse_args}},
}
```

