#include <iostream>

#include <gst/gst.h>

using namespace std;


int main(int argc, char *argv[])
{

  // char pipeline1[] = "playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm";
  char pipeline2[] = "videotestsrc is-live=true ! videoconvert ! x264enc bitrate=1000 tune=zerolatency ! video/x-h264 ! h264parse ! video/x-h264 ! queue ! flvmux name=mux ! rtmpsink location='rtmp://rtmp.docker:1935/stream/test live=1'"; 

  GstElement *pipeline = nullptr;
  GstBus *bus = nullptr;
  GstMessage *msg = nullptr;

  // gstreamer initialization
  gst_init(&argc, &argv);

  // building pipeline
  pipeline = gst_parse_launch(pipeline2, nullptr);

  // start playing
  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  // wait until error or EOS ( End Of Stream )
  bus = gst_element_get_bus(pipeline);
  msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                   static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

  // free memory
  if (msg != nullptr)
    gst_message_unref(msg);
  gst_object_unref(bus);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);

  return 0;
}