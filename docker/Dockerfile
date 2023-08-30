FROM ubuntu:22.04

ENV DEBIAN_FRONTEND noninteractive

# Some system dependencies
RUN apt-get update && apt-get install -y mlocate less git cmake-curses-gui cmake-gui jq wget unzip debconf-utils iputils-ping

# Get dependencies for metavision
RUN apt-get install -y libcanberra-gtk-module mesa-utils ffmpeg
RUN apt-get install -y cmake libboost-program-options-dev libeigen3-dev
RUN apt-get install -y python3-pip python3-tk
RUN apt-get install -y python3.8-dev
RUN python3 -m pip install pip --upgrade
RUN python3 -m pip install "opencv-python>=4.5.5.64" "sk-video==1.1.10" "fire==0.4.0" "numpy==1.23.4" pandas scipy h5py
RUN python3 -m pip install jupyter jupyterlab matplotlib "ipywidgets==7.6.5"

# Install metavision
ADD metavision.list /etc/apt/sources.list.d/metavision.list
RUN apt-get update && apt-get install -y metavision-sdk

# INstall Sophus
RUN apt-get install -y libfmt-dev
RUN cd ~ && \
    git clone https://github.com/strasdat/Sophus.git && \
    cd Sophus && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j && \
    make install && \
    cd ../.. && \
    rm -r Sophus

# Install Pangolin
RUN apt-get install -y libgl1-mesa-dev libwayland-dev libxkbcommon-dev wayland-protocols libegl1-mesa-dev libc++-dev libglew-dev libeigen3-dev cmake g++ ninja-build libjpeg-dev libpng-dev libavcodec-dev libavutil-dev libavformat-dev libswscale-dev libavdevice-dev
RUN cd ~ && \
    git clone --recursive https://github.com/stevenlovegrove/Pangolin.git && \
    cd Pangolin && \
    cmake -B build && \
    cmake --build build && \
    cmake --install build && \
    cd .. && \
    rm -r Pangolin

# Install ceres and nanoflann
RUN apt-get install -y libunwind-dev libceres-dev libnanoflann-dev
