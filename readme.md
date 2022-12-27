# Build
```
docker buildx build -t "bergerwill/gstreamer-opencv-object-detector" .
docker buildx build -t "bergerwill/cpp/gstreamer-opencv-object-detector" -f Dockerfile-cpp .
```

**NOTE**: If you don't want to use `buildx` backend, remove it from the command


# Run 
## Dev
```
docker run -it bergerwill/gstreamer-opencv-object-detector bash
docker run -it bergerwill/cpp/gstreamer-opencv-object-detector bash
```