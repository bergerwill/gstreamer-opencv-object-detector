import gi

gi.require_version('GstApp', '1.0')
from gi.repository import GObject, Gst, GstApp

import multiprocessing
import numpy

class GstPipeline(object):
	"""runs input gstreamer pipeline"""
	def __init__(self, pipelineStr):
		super(GstPipeline, self).__init__()
		self.pipelineStr = pipelineStr

		# make pipeline object from string
		self.pipeline = Gst.parse_launch(self.pipelineStr)


		# get and configure bus
		self.bus = self.pipeline.get_bus()
		self.bus.add_signal_watch()
		self.bus.connect("message", self.onMessage)

		# create main event loop object
		self.mainloop = GObject.MainLoop()

		# thread to make pipeline non-blocking
		threadFunc = self.mainloop.run
		self.thread = multiprocessing.Process(target=threadFunc, daemon=True)

	def start(self):
		print('starting pipeline')
		self.pipeline.set_state(Gst.State.PLAYING)
		self.thread.start()
				

	def stop(self):
		print('stopping pipeline')
		self.pipeline.set_state(Gst.State.NULL)
		self.thread.join()
		print('stopped pipeline')


	def isPlaying(self):
		change, state, pending = self.pipeline.get_state(1) 
		output = (state == Gst.State.PLAYING)
		return output	

	def onMessage(self, bus, message):
		'''
		handles the messages posted on bus
		'''

		structure = message.get_structure()
		if structure is None:
			return


		if message.type == Gst.MessageType.EOS:
			self.pipeline.set_state(Gst.State.NULL)
			print('End of stream: {}'.format(message))

		elif message.type == Gst.MessageType.ERROR:
			self.pipeline.set_state(Gst.State.NULL)
			err, debug = message.parse_error()
			print("Error message: {}".format(err))
			print("Debug message: {}".format(debug))
		else:
			print("unhandled message received on bus")        