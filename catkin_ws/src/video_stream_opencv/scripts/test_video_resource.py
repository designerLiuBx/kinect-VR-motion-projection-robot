#! /usr/bin/env python2
# -*- coding: utf-8 -*-
"""

Copyright (c) 2015 PAL Robotics SL.
Released under the BSD License.

Created on 7/14/15

@author: Sammy Pfeiffer

test_video_resource.py contains
a testing code to see if opencv can open a video stream
useful to debug if video_stream does not work
"""

import sys
import signal
import cv2
import rospy
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image

def quit(signum, frame):
        print''
        print'stop fusion'
        sys.exit()

if __name__=='__main__':
        signal.signal(signal.SIGINT, quit)
        signal.signal(signal.SIGTERM, quit)

        if len(sys.argv) < 2:
                print "You must give an argument to open a video stream."
                print "  It can be a number as video device, e.g.: 0 would be /dev/video0"
                print "  It can be a url of a stream,        e.g.: rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov"
                print "  It can be a video file,             e.g.: myvideo.mkv"
                exit(0)
        resource = sys.argv[1]
        if len(resource) < 3:
                resource_name = "/dev/video" + resource
                resource = int(resource)
        else:
                resource_name = resource
        print "Trying to open resource: " + resource_name
        cap = cv2.VideoCapture(resource)
        if not cap.isOpened():
                print "Error opening resource: " + str(resource)
                print "Maybe opened resource can't open it"
                exit(0)

        bridge = CvBridge()
        publisher = rospy.Publisher('image_topic', Image, queue_size=10)
        #rospy.init_node('web_cam')
        rospy.init_node('video_stream_opencv')
        print "Correctly opened resource, starting to show feed."
	
	rval, frame = cap.read()
        while rval:
		cv2.imshow('Video_RealTime', frame)   
        	image_message = bridge.cv2_to_imgmsg(frame, encoding="bgr8")
        	try:
                        publisher.publish(image_message)
                except CvBridgeError as e:
                        print(e)
                rval, frame = cap.read()
		if cv2.waitKey(1) &0xFF == ord('q'):
			break
	cap.release()
	cv2.destroyAlldows()




