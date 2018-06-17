PLEASE NOTE THAT THE GITHUB VERSION OF SLiM IS PRERELEASE SOFTWARE THAT IS UNDER ACTIVE DEVELOPMENT!
---------------------------------------------------------------------------------------------
It is strongly recommended that all end users of SLiM use the current release version, available at [http://messerlab.org/slim/](http://messerlab.org/slim/).
---------------------------------------------------------------------------------------------
 

License
----------

Copyright (c) 2016 Philipp Messer.  All rights reserved.

SLiM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SLiM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SLiM.  If not, see [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/).


Development & Feedback
-----------------------------------
SLiM is under active development, and our goal is to make it as broadly useful as possible.  If you have feedback or feature requests, or if you are interested in contributing to SLiM, please contact Philipp Messer at [messer@cornell.edu](mailto:messer@cornell.edu). Please note that Philipp is also looking for graduate students and postdocs.

Installation from source
========================

We use CMake, with an out-of-source build, as described here:
    https://gitlab.kitware.com/cmake/community/wikis/FAQ#out-of-source-build-trees
CMake, available by `brew install cmake` (on OSX) or `aptitude install cmake` (on debian).

To compile the development version from this repository, do:
```
git clone https://github.com/MesserLab/SLiM.git
mkdir SLiM_build  # the directory that SLiM will be built in (name can be anything)
cd SLiM_build
cmake -D CMAKE_BUILD_TYPE=Release ../SLiM
make
```
The resulting binary will be called `slim`, within the `SLiM_build` directory.

To update the build, in the `SLiM_build` directory run
```
cmake -D CMAKE_BUILD_TYPE=Release ../SLiM
make
```

To build the code with debug flags turned on, run instead
```
cmake -D CMAKE_BUILD_TYPE=Debug ../SLiM
make
```
This could be done in a separate directory to retain both versions of the build.
