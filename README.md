# cpp-rwnmr

Open-source terminal Linux (Ubuntu) application for image based random walk (RW) simulations of nuclear magnetic resonance (NMR) response. It explores GPUs for parallel computing and performance accelearation.

# Features
**cpp-rwnmr** includes simulations for some well-known NMR experiments:

+ CPMG
+ PFGSE
+ MultiTau

# Requirements
## Hardware
**cpp-rwnmr** requires a NVIDIA Graphical Processing Unit (GPU).

## Software
The following software and libraries are required:
+ [CMake (3.17 or greater)](https://cmake.org/);
+ [OpenMPI](https://www.open-mpi.org/);
+ [Eigen3](https://eigen.tuxfamily.org/);
+ [OpenCV](https://opencv.org/);
+ [CUDA Toolkit](https://developer.nvidia.com/cuda-toolkit);

# Installation
**cpp-rwnmr** execution requires two steps: C++ dependencies installation and Project build/compilation.

## C++ requirements

### **OpenMPI** (message protocol for multi process applications) 
``` 
sudo apt update && sudo apt-get install libopenmpi-dev openmpi-bin
```

### **Eigen3** (linear algebra and matrix operations) 
```
sudo apt update && sudo apt install libeigen3-dev
```

### **OpenCV** (image manipulation)
```
sudo apt update && sudo apt install libopencv-dev python3-opencv
```

### **CUDA Toolkit** (GPU programming)
A complete step-by-step installation guide of CUDA Toolkit can be found [here](https://developer.nvidia.com/cuda-downloads/).
After installation is completed, CUDA paths must be added to the enviroment variables in order to compile the project.
This action must be repeated every time a new terminal window is open. In order to automatize this process, we recommend creating a file in ``/etc/profile.d/`` such as:
``` 
touch ~/cuda.sh
echo "export PATH=$PATH:/usr/local/cuda/bin" >> ~/cuda.sh
echo "export CUDADIR=/usr/local/cuda" >> ~/cuda.sh
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib64" >> ~/cuda.sh
sudo mv ~/cuda.sh /etc/profile.d/cuda.sh
```

## Project build 
**cpp-rwnmr** uses CMake (3.17 or greater) to build its project.

### CMake (latest version)
If current version of CMake is 3.16 or lower, we recommend uninstalling it
```
sudo apt remove --purge --auto-remove cmake
```

Install build tools and libs CMake depends on
```
sudo apt update && sudo apt install -y software-properties-common lsb-release && sudo apt clean all
```

Obtain a copy of kitware's signing key
```
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
```

Add kitware's repository to source list
```
sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
```

As an optional step, is recommended that we also install the kitware-archive-keyring package to ensure that Kitware's keyring stays up to date as they rotate their keys
```
sudo apt update && apt install -y kitware-archive-keyring 
sudo rm /etc/apt/trusted.gpg.d/kitware.gpg
```

Note: If running ``sudo apt update`` gets the following error:
```
Err:7 https://apt.kitware.com/ubuntu bionic InRelease
The following signatures couldn't be verified because the public key is not available: NO_PUBKEY 6AF7F09730B3F0A4
```
Then manually add the public key:
```
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 6AF7F09730B3F0A4
```

Update and install CMake package:
```
sudo apt update && apt install -y cmake
```

### Build
To build **cpp-rwnmr**, run  ``build.sh`` from the project's root directory:
```
./build.sh
```

# Dockerized
Due to its dependencies, we recommend using [Docker](https://www.docker.com/) for an isolated environment.
For this purpose, Docker and [NVIDIA Container Toolkit](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/install-guide.html) must be installed in the host machine.

## NVIDIA Container Toolkit installation
Setting up NVIDIA Container Toolkit
```
distribution=$(. /etc/os-release;echo $ID$VERSION_ID) \
      && curl -fsSL https://nvidia.github.io/libnvidia-container/gpgkey | sudo gpg --dearmor -o /usr/share/keyrings/nvidia-container-toolkit-keyring.gpg \
      && curl -s -L https://nvidia.github.io/libnvidia-container/$distribution/libnvidia-container.list | \
            sed 's#deb https://#deb [signed-by=/usr/share/keyrings/nvidia-container-toolkit-keyring.gpg] https://#g' | \
            sudo tee /etc/apt/sources.list.d/nvidia-container-toolkit.list
```

Install nvidia-docker2 package via apt:
```
sudo apt update && sudo apt install -y nvidia-docker2
```

Restart the Docker daemon to complete the installation:
```
sudo systemctl restart docker
```

## Build cpp-rwnmr Docker Image
```
docker build -t rwnmr .
```

## Run Docker Container:
```
docker run -it --rm --gpus all -v $(pwd)/db:/app/db -v $(pwd)/config:/app/config -v $(pwd)/input:/app/input rwnmr
```