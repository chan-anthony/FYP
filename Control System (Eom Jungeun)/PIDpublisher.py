#!/usr/bin/env python
# license removed for brevity
import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import String

msg = "Control"

e = "Communications Failed"

def PIDpublisher():
    pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)
    rospy.init_node('PIDpublisher', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    
    # BlueROV2 should stop at the target to be stabilized
    try:
        print(msg)
        while not rospy.is_shutdown():
            twist = Twist()
            twist.linear.x = 0.0; twist.linear.y = 0.0; twist.linear.z = 0.0
            twist.angular.z = 0.0
            pub.publish(twist)

    except:
        print(e)
    
    finally:
        twist = Twist()
        twist.linear.x = 0.0; twist.linear.y = 0.0; twist.linear.z = 0.0
        twist.angular.z = 0.0
        pub.publish(twist)

if __name__ == '__main__':
    try:
        PIDpublisher()
    except rospy.ROSInterruptException:
        pass