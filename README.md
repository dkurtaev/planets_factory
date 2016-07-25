### Dependencies
* freeglut

```
sudo apt-get install freeglut3-dev libxmu-dev libxi-dev
```

* OpenCV

```
sudo apt-get install libopencv-dev
```

* Qt5

```
sudo apt-get install qtbase5-dev
```

* Cmake >= 3.0 for glog

```
sudo apt-get install software-properties-common
sudo add-apt-repository ppa:george-edison55/cmake-3.x
sudo apt-get update
sudo apt-get install cmake
```

### How to use it
Clone
```
git clone https://github.com/dkurtaev/planets_factory.git
cd planets_factory/
```

Update submodules
```
git submodule init
git submodule update
```

Build
```
mkdir build && cd build/
cmake .. && make -j4
```

Run
```
./bin/planets_factory
```

### Features
See wiki page [Features](https://github.com/dkurtaev/planets_factory/wiki/Features)


### Testing devices
```
Acer Aspire One 725, Ubuntu 14.04 LTS
CPU: AMD C-60 APU with Radeon(tm) HD Graphics
GPU: Advanced Micro Devices, Inc. [AMD/ATI] Wrestler [Radeon HD 6290]

HP Pavilion 15-ac001ur, Ubuntu 14.04 LTS
CPU: Intel(R) Celeron(R) CPU  N3050  @ 1.60GHz
GPU: Intel Corporation Device 22b1 (rev 21)
```