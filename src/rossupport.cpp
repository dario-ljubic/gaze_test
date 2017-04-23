#include "rossupport.h"

RosPublisher::RosPublisher(){
}

RosPublisher::~RosPublisher(){

}

RosPublisher::RosPublisher(std::string rosTopic) {
    ros::NodeHandle nh;
    pub = nh.advertise<gaze_test::GazeHyps>(rosTopic, 10);
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
