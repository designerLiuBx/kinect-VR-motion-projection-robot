#!/usr/bin/env python
import rospy
from std_msgs.msg import String

def talker():
    pub = rospy.Publisher("hand_data", String, queue_size=10)
    rospy.init_node("usart_handData", anonymous=True)
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        hand_data_str = "123test"

        rospy.loginfo(hand_data_str)
        pub.publish(hand_data_str)
        rate.sleep()

if __name__ == '__main__':
	talker()
    
