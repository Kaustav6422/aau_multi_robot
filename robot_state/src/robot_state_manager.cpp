#include <robot_state_manager.h>

//TODO use a separate executable???
RobotStateManager::RobotStateManager()
{
    ROS_INFO_COND_NAMED(LOG_TRUE, "robot_state", "Creating instance of RobotStateManager class..."); //TODO use COND_NAMED everywhere
    loadParameters();
    initializeRobotState();
    createServices();
    fillRobotStateStringsVector();
    dt.createLogFile(); //TODO
    ROS_INFO("Instance correctly created");
}

void RobotStateManager::loadParameters() {
    
}

void RobotStateManager::initializeRobotState() {
    robot_state = robot_state::INITIALIZING;
}

void RobotStateManager::createServices() {
    ros::NodeHandle nh;
    ss_get_robot_state = nh.advertiseService("robot_state/get_robot_state", &RobotStateManager::get_robot_state_callback, this);
    ss_set_robot_state = nh.advertiseService("robot_state/set_robot_state", &RobotStateManager::set_robot_state_callback, this);
    //TODO use lock and unlock???
    //ss_lock_robot_state = nh.advertiseService("robot_state/lock_robot_state", &RobotStateManager::lock_robot_state_callback, this);
    //ss_unlock_robot_state = nh.advertiseService("robot_state/unlock_robot_state", &RobotStateManager::unlock_robot_state_callback, this);
}

//TODO not very robust to changes... try using enumerations or other ways...
void RobotStateManager::fillRobotStateStringsVector() {
    std::vector<std::string>::iterator it = robot_state_strings.begin();
    robot_state_strings.push_back("INITIALIZING");
    robot_state_strings.push_back("CHOOSING_ACTION");
    robot_state_strings.push_back("COMPUTING_NEXT_GOAL");
    robot_state_strings.push_back("MOVING_TO_FRONTIER");
    robot_state_strings.push_back("GOING_CHECKING_VACANCY");
    robot_state_strings.push_back("CHECKING_VACANCY");
    robot_state_strings.push_back("GOING_CHARGING");
    robot_state_strings.push_back("CHARGING");
    robot_state_strings.push_back("CHARGING_COMPLETED");
    robot_state_strings.push_back("CHARGING_ABORTED");
    robot_state_strings.push_back("LEAVING_DS");
    robot_state_strings.push_back("GOING_IN_QUEUE");
    robot_state_strings.push_back("IN_QUEUE");
    robot_state_strings.push_back("AUCTIONING");
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool RobotStateManager::get_robot_state_callback(robot_state::GetRobotState::Request &req, robot_state::GetRobotState::Response &res) {
    mutex.lock();
    ROS_INFO("get_robot_state service required");    
    res.robot_state = robot_state;
    ROS_INFO("Service message successfully sent");
    mutex.unlock();
    return true;
}
#pragma GCC diagnostic pop

bool RobotStateManager::set_robot_state_callback(robot_state::SetRobotState::Request &req, robot_state::SetRobotState::Response &res) {
    mutex.lock();
    ROS_INFO("set_robot_state service required");
    if(isNewStateValid(req.robot_state)) {
        ROS_DEBUG("Robot state transiction: %s -> %s", robotStateEnumToString(robot_state).c_str(), robotStateEnumToString(req.robot_state).c_str());
        dt.updateLogFile(); //TODO
        robot_state = req.robot_state;
        res.set_succeeded = true;
    }
    else {
        ROS_ERROR("The next required robot state is invalid! Keeping current state (%s)", robotStateEnumToString(robot_state).c_str());
        res.set_succeeded = false;
    }
    ROS_INFO("Service message successfully sent");
    mutex.unlock();
    return true;
}

bool RobotStateManager::isNewStateValid(int new_state) {
    return new_state >= 0 && (unsigned int)new_state < robot_state_strings.size();
}

std::string RobotStateManager::robotStateEnumToString(unsigned int enum_value) {
    // Sanity check
    if(enum_value >= robot_state_strings.size())
        ROS_FATAL("Invalid robot state! It will cause a segmentation fault!");
    return robot_state_strings.at(enum_value);
}

