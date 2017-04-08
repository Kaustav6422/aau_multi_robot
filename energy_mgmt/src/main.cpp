#include <ros/ros.h>
#include <ros/console.h>
#include <battery.h>
#include <battery_simulate.h>
#include <docking.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "energy_mgmt");

    // handle battery management for different robot platforms
    string platform;
    
    /*
    ros::get_environment_variable(platform, "ROBOT_PLATFORM");
    if(platform.compare("turtlebot") == 0){
        battery_turtle bat;
    }
    else if(platform.compare("pioneer3dx") == 0 || platform.compare("pioneer3at") == 0){
        battery_pioneer bat;
    }
    else{
        battery_simulate bat;
    }
    */
    
    battery_simulate bat;
    
    
    // coordinate docking of robots for recharging
    docking doc;

    // Frequency of loop
    double rate = 0.5; // Hz
    ros::Rate loop_rate(rate);

    while(ros::ok()){
        // get updates from subscriptions
        ros::spinOnce();

        // compute new battery state
        bat.compute();

        // output battery state to console
        bat.output();

        // publish battery state
        bat.publish();

        // send broadcast message with positions of all (known) docking stations
        
        // 
        doc.discover_docking_stations();
        
        //
        doc.compute_optimal_ds();
        
        doc.update_robot_state();
        
        doc.map_info();

        // sleep for 1/rate seconds
        loop_rate.sleep();
    }

    return 0;
}
