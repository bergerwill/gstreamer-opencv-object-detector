
# Build
```
cd cpp
rm -rf build/ && mkdir -p build && cd build && cmake .. && make && cd ..
```

### Add custom gstreamer element(s) to path
```
export GST_PLUGIN_PATH=$GST_PLUGIN_PATH:$(pwd)/cpp/build
```

### Inspect element 
gst-inspect-1.0 imageblur

**NOTE:** if above command reports "No such element", then make sure `libgstimageblur.so` exists in  `GST_PLUGIN_PATH`.

# Run
## Run samples
**NOTE:** All commands are expected to run from `cpp` directory. 

```
./build/ObjectDetector
```
