#!/usr/bin/env python
# license removed for brevity
import rospy
from std_msgs.msg import String

pub = rospy.Publisher('chatter', String, queue_size=10)
class talker():
    def __init__(self):
        send_flag = 0
        rospy.init_node('talker', anonymous=True)
        rospy.Subscriber("hand_data", String, self.callback)
 
    def callback(self,data):
        rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)
        hand_data_stc = data.data
        send_flag = 1
        rate = rospy.Rate(10) # 10hz
        while not rospy.is_shutdown():
            if(send_flag == 1):
                data_package = hand_data_stc
                rospy.loginfo(data_package)
                pub.publish(data_package)
                rate.sleep()

if __name__ == '__main__':
    talker()
    rospy.spin()
#if __name__ == '__main__':
#    try:
#        talker()
#    except rospy.ROSInterruptException:
#        pass
