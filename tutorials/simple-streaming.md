# Goal
In this tutorial, we will run test video streaming app. The streaming application is written using gstreamer framework and streams to nginx RTMP server. 


# Steps

## Run nginx-rtmp docker container
```
docker network create bergerwill # make new network
docker run -d --name=rtmp.docker --network=bergerwill -p 1935:1935 -p 8080:80  alfg/nginx-rtmp
```


## Run gstreamer docker container
Clone the repo and checkout to target branch (`muz/add-gstreamer-python-docker` at the time of writing this tutorial). 
```
cd /path/to/repo
docker buildx build -t bergerwill/cpp/gstreamer -f cpp/Dockerfile .
docker run -it --network bergerwill -v /path/to/repo:/app --workdir /app bergerwill/cpp/gstreamer bash
```
At this point you should be inside the container terminal.

## Build application inside docker
```
cd cpp
rm -rf build/ && mkdir -p build && cd build && cmake .. && make && cd .. 
./build/ObjectDetector
```
Above commands will start streaming to nginx-rtmp server. Corresponding application code is in `cpp/src/main.cpp` file.

## Playback stream
```
ffplay rtmp://localhost:1935/stream/test
```

If ffplay is not available, then you can use vlc player as well using the guide [here](https://www.videoconverterfactory.com/tips/rtmp-vlc.html)
