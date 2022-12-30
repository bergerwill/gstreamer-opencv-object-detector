from signal import signal, SIGINT
from sys import exit
import time


import gi
gi.require_version('Gst', '1.0')
from gi.repository import Gst
Gst.init(None)

import numpy as np


from lib.GstPipeline import GstPipeline 



# config
PIPELINE = '''
            videotestsrc is-live=true ! videoconvert ! x264enc bitrate=1000 tune=zerolatency ! 
            video/x-h264 ! h264parse ! video/x-h264 ! 
            queue ! flvmux name=mux ! 
            rtmpsink location='rtmp://gstreamer-opencv-object-detector_server_1:1935/stream/muz live=1'
	'''



def main():
    print("main() start")
    pipelineDef = PIPELINE

    # make pipeline 
    p = GstPipeline(pipelineDef)


    def exitHandler(sig, frame):
        # Handle any cleanup here
        print('SIGINT or CTRL-C detected. Exiting gracefully')
        p.stop()

    signal(SIGINT, exitHandler)


    try:
        p.start() # start pipepine
        time.sleep(1)
    except Exception as exp:
        print("Failed to start the pipeline. Exception message: {}".format(exp))

    
    while p.isPlaying():
        time.sleep(1)

    print('main exits here')

if __name__ == '__main__':
    main()
    
    # try:
    #     main()
    # except KeyboardInterrupt as exp:
    #     print("Exiting due to KeyboardInterrupt")