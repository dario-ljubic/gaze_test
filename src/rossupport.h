#pragma once

#include <string>

#include "ros/ros.h"
#include "gazehyps.h"
#include "gaze_test/GazeHyps.h"

// todo: For image acquisition add another class RosImageSubscriber to subscribe to a message

class RosPublisher {

private:
    gaze_test::GazeHyps msg;
    ros::Publisher pub;

public:
    RosPublisher();
    RosPublisher(std::string rosTopic);
    void publishGazeHypotheses(GazeHypsPtr gazehyps);
    ~RosPublisher();

};
