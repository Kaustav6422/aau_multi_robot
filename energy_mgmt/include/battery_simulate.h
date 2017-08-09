#ifndef BATTERY_SIMULATE_H
#define BATTERY_SIMULATE_H

#include <ros/ros.h>
#include <std_msgs/Empty.h>

#include <diagnostic_msgs/DiagnosticArray.h>
//#include <energy_mgmt/battery_state.h>
#include <explorer/battery_state.h>
#include "std_msgs/Float32.h"
#include <adhoc_communication/EmRobot.h>
#include <sstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <stdlib.h>
#include <fstream>
#include "time_manager_interface.h"
#include <boost/thread/mutex.hpp>

#include <robot_state/robot_state.h>
#include "robot_state_em.h"
#include <robot_state/GetRobotState.h>
#include <robot_state/SetRobotState.h>

#include "computer.h"

#include "robot_state_manager.h" //TODO this requires to link in CMakeLists.txt... better use a pointer? https://stackoverflow.com/questions/12466055/field-has-incomplete-type-error

#define SSTR(x) static_cast<std::ostringstream &>((std::ostringstream() << std::dec << x)).str()

class RobotStateManager;

class battery_simulate
{
public:

    battery_simulate();
    void logBatteryState();
    void publishBatteryState();   
    void run();
    void createLogDirectory();
    void createLogFiles();
    
    /*************************
     ** Debugging functions **
     *************************/
    void setTimeManager(TimeManagerInterface *time_manager);
    double last_time_secs();
    void set_last_time();
    void initializeSimulationTime();
    double getElapsedTime();
    void spinOnce();
    bool initializing;
    double getConsumedEnergyA();
    double getConsumedEnergyB();
    double getMaximumTravelingDistance();
    double getTotalTraveledDistance();
    double _f1, _f2, _f3, _f4, _f5, _f6;
    double getRemainingDistance();

private:

    ros::NodeHandle nh;
    explorer::battery_state state;

    ros::Publisher pub_battery;

    ros::ServiceClient set_robot_state_sc;
    ros::ServiceClient get_robot_state_sc;
    
    std::string log_path;
    std::string info_file, battery_state_filename;
    std::fstream fs_info, battery_state_fs;
    std::string robot_name;
    std::string robot_prefix;
    int robot_id;
    bool idle_mode;
    ros::Time sim_time_start;
    ros::WallTime wall_time_start;
    TimeManagerInterface *time_manager;
    
    RobotStateManager robot_state_manager;
    Computer2 *battery_state_updater;
    
    void loadParameters();
    void initializeVariables();
    void initializeBatteryState();
    void createServiceClients();
    void advertiseTopics();
    void subscribeToTopics(); 
    void initializeRobotName();
    void updateBatteryState();

    void poseCallback(const geometry_msgs::PoseWithCovarianceStampedConstPtr &pose);

};


#endif //BATTERY_SIMULATE_H
