# gstreamer-opencv-object-detector
Checkout `tutorials/simple-streaming.md` for initial instructions/tutorial. 


# Sample pipelines
### stream mp4 video
```
gst-launch-1.0 filesrc location=/app/tutorials/videos/ww2-warbirds-in-formation.mp4 ! decodebin name=dec ! videoconvert ! video/x-raw,format=RGBA  ! imageblur ! videoconvert ! x264enc bitrate=1000 tune=zerolatency ! video/x-h264 ! h264parse ! video/x-h264 ! queue ! flvmux name=mux ! rtmpsink location='rtmp://rtmp.docker:1935/stream/test live=1'
```
