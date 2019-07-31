/**
* Quentin & Kyle
* ARSLab - Carleton University
*
*/

#ifndef BOOST_SIMULATION_PDEVS_ULTRA_HPP
#define BOOST_SIMULATION_PDEVS_ULTRA_HPP

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

#include "../drivers/ultrasonic_driver.hpp"

using namespace cadmium;
using namespace std;

//Port definition
    struct ultra_defs {
        struct ultrasonic_out : public out_port<bool> { };
        struct ultrasonic_in : public in_port<bool> { };
    };

    template<typename TIME>
    class Ultrasonic {
        using defs=ultra_defs; // putting definitions in context

        public:
            //Parameters to be overwriten when instantiating the atomic model
            TIME pollingTime = TIME("00:00:001:000");
            
        
            // default constructor
            Ultrasonic() noexcept{
              
             
             
              //state.lightOn = false;
              //state.fastToggle = false;
            }
            
            // state definition
            struct state_type{
              bool maybe_we_need_this;
              //bool fastToggle;
            }; 
            state_type state;
            // ports definition

            using input_ports=std::tuple<typename defs::ultrasonic_in>;
            using output_ports=std::tuple<typename defs::ultrasonic_out>;

            // internal transition
            void internal_transition() {
            }

            // external transition
            void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 

            }

            // confluence transition
            void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
              internal_transition();
              external_transition(TIME(), std::move(mbs));
            }

            // output function
            typename make_message_bags<output_ports>::type output() const {
              typename make_message_bags<output_ports>::type bags;
              
               UltrasonicDriver Ultrasonicsensor(D5, D6); 
               
                
               long distance = Ultrasonicsensor.distance();   
               // add condition to distance because use a long and return a bool --> distance < xx cm 
                get_messages<typename defs::ultrasonic_out>(bags).push_back(distance<10);
                wait(1.0); // 1 sec  
        
              

            
                
              return bags;
            }

            // time_advance function
            TIME time_advance() const {  
                return pollingTime;
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename Ultrasonic<TIME>::state_type& i) {
              os << "Output: " << (1); 
              return os;
            }
        };     


#endif // BOOST_SIMULATION_PDEVS_ULTRA_HPP