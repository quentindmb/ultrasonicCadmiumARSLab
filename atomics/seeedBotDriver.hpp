/**
* By: Ben Earle & Quentin de Montblanc
* ARSLab - Carleton University
*
* Seeed Bot Driver:
* This model will do simple line following using a Seed Bot Shield.
* Its purpose is to demonstrate how to use all of the port IO models in ECADMIUM.
*
* Note: The 'SCARED_OF_THE_DARK' macro will read from a Grove light sensor on 
*   analog port A5 and stop the car if the reading is less then 0.3.
* It must be defined here and in main if being used.
*/
#ifndef BOOST_SIMULATION_PDEVS_BLINKY_HPP
#define BOOST_SIMULATION_PDEVS_BLINKY_HPP

#include "../drivers/ultrasonic_driver.hpp"
#include "../atomics/ultrasonic.hpp"
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include <limits>
#include <math.h> 
#include <assert.h>
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <limits>
#include <random>

 #define USE_ULTRA_SONICS





using namespace cadmium;
using namespace std;
enum DriveState {right = 0, straight = 1, left = 2, stop = 3};
//Port definition
    struct seeedBotDriver_defs {
        //Output ports
        struct rightMotor1 : public out_port<float> { };
        struct rightMotor2 : public out_port<bool> { };
        struct leftMotor1 : public out_port<float> { };
        struct leftMotor2 : public out_port<bool> { };
        //Input ports
        struct rightIR : public in_port<bool> { };
        struct centerIR : public in_port<bool> { };
        struct leftIR : public in_port<bool> { };
        
        #ifdef USE_ULTRA_SONICS
        //struct lightSensor : public in_port<float> { };
        struct ultasoncicsensor : public in_port<bool> { };
        #endif
    };

    template<typename TIME>
    class SeeedBotDriver {
        using defs=seeedBotDriver_defs; // putting definitions in context
        public:
            //Parameters to be overwriten when instantiating the atomic model
            TIME   slowToggleTime;
            TIME   fastToggleTime;
            // default constructor
            SeeedBotDriver() noexcept{
              state.dir = straight;
              // creat the state for the shieldbot can stop with the ultrasonicsensor
              state.too_close = false;
            }
            
            // state definition
            struct state_type{
              bool leftIR;
              bool centerIR;
              bool rightIR;
              bool ultasoncicsensor;
              DriveState dir;
              bool prop;
              // definition type of too_close
              bool too_close;
            }; 
            state_type state;
            // ports definition
            #ifdef USE_ULTRA_SONICS
            using input_ports=std::tuple<typename defs::rightIR, typename defs::ultasoncicsensor, typename defs::centerIR, typename defs::leftIR>;
            #else
            using input_ports=std::tuple<typename defs::rightIR, typename defs::ultasoncicsensor, typename defs::centerIR, typename defs::leftIR>;
            #endif
            using output_ports=std::tuple<typename defs::rightMotor1, typename defs::rightMotor2, typename defs::leftMotor1, typename defs::leftMotor2>;

            // internal transition
            void internal_transition() {
              state.prop = false;
              //Do nothing... 
            }

            // external transition
            void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
             


              for(const auto &x : get_messages<typename defs::rightIR>(mbs)){
                state.rightIR = !x;
              }
              for(const auto &x : get_messages<typename defs::centerIR>(mbs)){
                state.centerIR = !x;
              }
              for(const auto &x : get_messages<typename defs::leftIR>(mbs)){
                state.leftIR = !x;
              }
              #ifdef USE_ULTRA_SONICS
              for(const auto &x : get_messages<typename defs::ultasoncicsensor>(mbs)){
                state.too_close = x; // this is the place where too_close is actualised
              }
              #endif
              if((!(state.rightIR ^ state.leftIR ^ state.centerIR) && !(!state.rightIR && !state.leftIR && !state.centerIR)) || (state.rightIR && state.leftIR && state.centerIR)) {
                //This happens when no IR sensors see the line and if two or more IR sensors see the line.
                state.dir = DriveState::stop;
                 printf(" a l arret pas de sol ");

              

              } else if (state.rightIR) {
                state.dir = DriveState::left;
                 printf(" a gauche ");
              } else if (state.leftIR) {
                state.dir = DriveState::right;
                 printf(" a droite ");
              } else {
                state.dir = DriveState::straight;
                 printf(" tout droit " );
              }
              #ifdef USE_ULTRA_SONICS // this happens when ultrasonic sensor is activate and become true
              printf(" too_close est : %d \n",state.too_close );
              if (state.too_close) {
                state.dir = DriveState::stop;
                 printf(" too_close est vrai on s arrete: %d \n",state.too_close );
              }
             #endif
              
              state.prop = true;
            }

            // confluence transition
            void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
              internal_transition();
              external_transition(TIME(), std::move(mbs));
            }

            // output function
            typename make_message_bags<output_ports>::type output() const {
              typename make_message_bags<output_ports>::type bags;
              float rightMotorOut1;
              bool rightMotorOut2;
              float leftMotorOut1;
              bool leftMotorOut2;  

              switch(state.dir){
                case DriveState::right:
                  rightMotorOut1 = 0.5;
                  rightMotorOut2 = 0;
                  leftMotorOut1 = 1;
                  leftMotorOut2 = 1;                
                break;

                case DriveState::left:
                  rightMotorOut1 = 1;
                  rightMotorOut2 = 1;
                  leftMotorOut1 = 0.5;
                  leftMotorOut2 = 0;
                break;

                case DriveState::straight:
                  rightMotorOut1 = 0.5;
                  rightMotorOut2 = 0;
                  leftMotorOut1 = 0.5;
                  leftMotorOut2 = 0;
                break;

                case DriveState::stop:
                default:
                  rightMotorOut1 = 0;
                  rightMotorOut2 = 0;
                  leftMotorOut1 = 0;
                  leftMotorOut2 = 0;
                break;
              }

              get_messages<typename defs::rightMotor1>(bags).push_back(rightMotorOut1);
              get_messages<typename defs::rightMotor2>(bags).push_back(rightMotorOut2);
              get_messages<typename defs::leftMotor1>(bags).push_back(leftMotorOut1);
              get_messages<typename defs::leftMotor2>(bags).push_back(leftMotorOut2);
                
              return bags;
            }

            // time_advance function
            TIME time_advance() const { 
              if(state.prop)
                return TIME("00:00:00");
              return std::numeric_limits<TIME>::infinity();
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename SeeedBotDriver<TIME>::state_type& i) {
              os << "Current state: " << i.dir; 
              return os;
            }
        };     


#endif // BOOST_SIMULATION_PDEVS_BLINKY_HPP