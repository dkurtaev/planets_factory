### Dependencies
* freeglut

```
sudo apt-get install freeglut3-dev libxmu-dev libxi-dev
```

### How to use it
Clone and build.
```
git clone https://github.com/dkurtaev/planets_factory.git
cd planets_factory/
mkdir build && cd build
cmake .. && make
```
Run
```
./planets_factory
```

### Features
* Basic icosphere drawing.  
* Use mouse with pressed left button for moving camera around icosphere.  
* Press ```Ctrl``` key for rotating around icosphere's normal.
