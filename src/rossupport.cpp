#include "rossupport.h"

RosSubscriber::RosSubscriber() : it(nh)
{
    sub = it.subscribe("camera/image", 1, &RosSubscriber::Callback, this);
}

void RosSubscriber::Callback(const sensor_msgs::ImageConstPtr& msg)
  {
    try
    {
      cvImagePtr = cv_bridge::toCvCopy(msg);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
  }

bool RosSubscriber::get(cv::Mat& frame){
    //todo:add some conditions if the topic is not created etc. (like in yarpsupport)
    ros::spinOnce();
    if (cvImagePtr && ros::ok()) {
        //http://answers.opencv.org/question/7682/copyto-and-clone-functions/
        //http://stackoverflow.com/questions/184710/what-is-the-difference-between-a-deep-copy-and-a-shallow-copy
        cv::Mat rgbframe(cvImagePtr->image);
        frame = rgbframe.clone();
        return true;
    }
    //std::cout << "There is no cvImagePtr!" << std::endl;
    return false;
}

std::string RosSubscriber::getLabel()
{
    return "";
}

std::string RosSubscriber::getId()
{
    return "";
}

RosSubscriber::~RosSubscriber(){

}


RosPublisher::RosPublisher(){
}

RosPublisher::RosPublisher(std::string rosTopicPub) {
    ros::NodeHandle nh;
    pub = nh.advertise<gaze_test::GazeHyps>(rosTopicPub, 10);
}

void RosPublisher::publishGazeHypotheses(GazeHypsPtr gazehyps) {

//    msg.header.seq = ; //uint32
//    msg.header.stamp = ; //time
//    msg.header.frame_id = ; //string

    double lid = std::nan("not set"); //todo: find a default number instead of nan!
    double horest = std::nan("not set");
    double vertest = std::nan("not set");
    bool mutgaze = false;
    if (gazehyps->size()) {
        GazeHyp& ghyp = gazehyps->hyps(0);
        lid = ghyp.eyeLidClassification.get_value_or(lid);
        horest = ghyp.horizontalGazeEstimation.get_value_or(horest);
        vertest = ghyp.verticalGazeEstimation.get_value_or(vertest);
        mutgaze = ghyp.isMutualGaze.get_value_or(false);
    }
    msg.frame = gazehyps->frameCounter;
//    msg.id = gazehyps->id;
//    msg.label = gazehyps->label;
//    msg.lid = lid;
    msg.horGaze = horest;
    msg.verGaze = vertest;
    msg.mutGaze = mutgaze;

    pub.publish(msg);
    ros::spinOnce();
}

RosPublisher::~RosPublisher(){

}
