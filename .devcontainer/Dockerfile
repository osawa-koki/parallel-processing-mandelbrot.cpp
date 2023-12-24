FROM ubuntu:22.04
WORKDIR /work/
RUN apt -qq -y update && apt -qq -y install git dos2unix cmake build-essential libpng-dev
RUN git clone https://github.com/google/benchmark.git && \
    cd benchmark && \
    cmake -E make_directory "build" && \
    cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../ && \
    cmake --build "build" --config Release && \
    cmake -E chdir "build" ctest --build-config Release && \
    cmake --build "build" --config Release --target install
