import sys
import rospy
import serial
from std_msgs.msg import String

global ser
global by_uart
def uart_init(resource):
	global ser
	resource = int (resource)
	if resource > 0:
		ser = serial.Serial('/dev/ttyUSB0', resource, timeout=0.5) 
		print"Your option is %d" %resource
	else:
		print"Your enter is ERROR!"
		exit(0)
	if not ser.isOpen():
		print"ERROR::::Uart is closed!"
		exit(0)
	print"uart is opened!!!"

	rospy.init_node('usart', anonymous=True)
	rospy.Publisher('hand_data', String, queue_size=10)
        rospy.Subscriber('uart_send01', String, callback01)
	rospy.Subscriber('uart_send02', String, callback02)
	rate = rospy.Rate(10) #10hz
	while not rospy.is_shutdown():
		hand_data = message01
		rospy.loginfo(hand_data)
		pub.publisk(hand_data)
	rospy.spin()
	

def callback01(data):
	global ser
	rospy.loginfo(rospy.get_caller_id() + ' I hear %s', data.data)
        send = data.data 
	while(1):
        	ser.write("P%sS"%send)
           	if(ser.read(2) == "ok"):
   	      		break
		else:
			print"Value is sended, but is not accepted!!!"
       	rospy.loginfo(rospy.get_caller_id() + 'Send Succussed', data.data)

def callback02(data):
        global ser
        rospy.loginfo(rospy.get_caller_id() + ' I hear %s', data.data)
        send = data.data
        while(1):
  	        ser.write("P%sS"%send)
                if(ser.read(2) == "ok"):
                        break
                else:
                        print"Value is sended, but is not accepted!!!"
        rospy.loginfo(rospy.get_caller_id() + 'Send Succussed', data.data)

#def usart_receive_and_send():	
#	pub = rospy.Publisher('message01', String, queue_size=200)
#	rate = rospy.Rate(10)
#	while not rospy.is_shutdown():
		#receive = ser.readline()
#               	if receive.startswith('Ax0'):
#                       	del receive[0:2]
#                       	message01 = receive
#               	elif receive.startswith('Bx0'):
#                       	del receive[0:2]
#                       	message02 = receive
#               	receive = 0   	
		
#		rospy.loginfo(message01)
#		pub.publish(message01)
		
#		rospy.Subscriber('uart_send01', String, callback)
#		rate.sleep()
	
#	def usart_send():
if __name__ == '__main__':
	global by_uart
        if len(sys.argv) < 2:
	        print"Please enter bt_uart."
                exit(0)
	print " bt_uart yes"
       	by_uart = sys.argv[1]
	uart_init(by_uart)
#	usart_receive_and_send()
