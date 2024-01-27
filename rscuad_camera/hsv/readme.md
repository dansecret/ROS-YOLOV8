# under maintenance

# note
- we update to opencv4 in preprocessor
- just type 

        $ make cv4

- memory leaks cannot be removed as this is the default in OpenCV, make sure the memory does not increase significantly.

## install 3.4
> ubuntu > 2018 and fedora

step1:

ubuntu

        $ sudo apt install build-essential cmake git pkg-config libgtk-3-dev \
                libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
                libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
                gfortran openexr libatlas-base-dev python3-dev python3-numpy \
                libtbb2 libtbb-dev libdc1394-22-dev libopenexr-dev \
                libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev
fedora

        $ sudo dnf install cmake python-devel numpy gcc gcc-c++ gtk2-devel libdc1394-devel ffmpeg-devel gstreamer1-plugins-base-devel

step2: 
        
        $ mkdir ~/opencv_build && cd ~/opencv_build
        $ git clone https://github.com/opencv/opencv.git
        $ git clone https://github.com/opencv/opencv_contrib.git
        $ cd opencv
        $ git checkout 3.4
        $ mkdir -p build && cd build

step3:

        $ cmake -D CMAKE_BUILD_TYPE=RELEASE \
                -D CMAKE_INSTALL_PREFIX=/usr/local \
                -D INSTALL_C_EXAMPLES=ON \
                -D INSTALL_PYTHON_EXAMPLES=ON \
                -D OPENCV_GENERATE_PKGCONFIG=ON \
                -D OPENCV_EXTRA_MODULES_PATH= ../../opencv_contrib/modules \
                -D BUILD_EXAMPLES=ON ..
step4:

        $ make -j8

step5:
        
        $ sudo make install
        $ pkg-config --modversion opencv

step6:

ubuntu

        $ echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib" >> ~/.bashrc
        $ source ~/.bashrc
fedora

        $ echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64" >> ~/.bashrc
        $ source ~/.bashrc


> using ubuntu <= 2018
        
        $ sudo apt install lib-opencv-dev

> remove opencv

        $ sudo apt-get purge '*opencv*'
        $ sudo find / -name "*opencv*" -exec rm -rf {} \;
        
        check
        $ sudo find / -name "*opencv*";

        https://stackoverflow.com/questions/9276169/removing-all-installed-opencv-libs

## example in test.cpp