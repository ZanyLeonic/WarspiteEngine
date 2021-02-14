#!/bin/bash
sudo apt update
sudo apt install -y libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-gfx-dev libvorbis-dev rapidjson-dev zlib1g-dev libopenal-dev python3-dev pybind11-dev libspdlog-dev libfmt-dev
cmake -DCMAKE_BUILD_TYPE="Release" -Bbuild .