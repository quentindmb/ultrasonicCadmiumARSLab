/**
* By: Quentin de Montblanc
* ARSLab - Carleton University
*
*/
#ifndef ULTRASONIC_DRIVER_HPP
#define ULTRASONIC_DRIVER_HPP

#include "mbed.h"


class UltrasonicDriver
{
  public:
    
    UltrasonicDriver(PinName t, PinName e) : trig(t), echo(e) {}

    long echo_duration() {
        
        timer.reset();  //reset timer
        trig=0;   // trigger low 
        wait_us(2); //  wait 
        trig=1;   //  trigger high
        wait_us(10);
        trig=0;  // trigger low
             while(!echo); // start pulseIN
          timer.start();
         while(echo);
          timer.stop();
         return timer.read_us(); 
 
    }
 
    //return distance in cm 
    long distance(){
        duration = echo_duration();
      distance_cm = (duration/2)/29.1  ;
            return distance_cm;
     
    }


    
    DigitalOut trig;
    DigitalIn echo;
    Timer timer;   
    long duration,distance_cm;
 
    
    

    private:

};





// other possibility to use ultrasonic sensor /!\ you need the library of hc-sr04 available in top model//
   /* public:
       
    
        DigitalOut _trig;
        InterruptIn _echo;
        Timer _t;
        Timeout _tout;
        int _distance;
        float _updateSpeed;
        int start;
        int end;
        volatile int done;
        void (*_onUpdateMethod)(int);
        
        float _timeout;
        int d;

 
    UltrasonicDriver(PinName trigPin, PinName echoPin, float updateSpeed, float timeout):_trig(trigPin), _echo(echoPin)
  {
    _updateSpeed = updateSpeed;
    _timeout = timeout;
  }
    
    UltrasonicDriver(PinName trigPin, PinName echoPin, float updateSpeed, float timeout, void onUpdate(int))
    :_trig(trigPin), _echo(echoPin)
    {
        _onUpdateMethod=onUpdate;
        _updateSpeed = updateSpeed;
        _timeout = timeout;
        _t.start ();
    }
    void _startT()
    { 
        if(_t.read()>600)
        {
            _t.reset ();
        }
        start = _t.read_us ();
    }
        
    void _updateDist()
    {
        end = _t.read_us ();
        done = 1;
        _distance = (end - start)/6;       
        _tout.detach();
        _tout.attach(this,&UltrasonicDriver::_startTrig, _updateSpeed);   
    }
    void _startTrig(void)
    {
            _tout.detach();
            _trig=1;             
            wait_us(10);        
            done = 0;            
            _echo.rise(this,&UltrasonicDriver::_startT);   
            _echo.fall(this,&UltrasonicDriver::_updateDist);
            _echo.enable_irq ();
            _tout.attach(this,&UltrasonicDriver::_startTrig,_timeout);
            _trig=0;                     
    }
    
    int getCurrentDistance(void)
    {
        return _distance;
    }
    void pauseUpdates(void)
    {
        _tout.detach();
        _echo.rise(NULL);
        _echo.fall(NULL);
    }
    void startUpdates(void)
    {
        _startTrig();
    }
    void attachOnUpdate(void method(int))
    {
        _onUpdateMethod = method;
    }
    void changeUpdateSpeed(float updateSpeed)
    {
        _updateSpeed = updateSpeed;
    }
    float getUpdateSpeed()
    {
        return _updateSpeed;
    }
    int isUpdated(void)
    {
        //printf("%d", done);
        d=done;
        done = 0;
        return d;
    }
    void checkDistance(void)
    {
        if(isUpdated())
        {
            (*_onUpdateMethod)(_distance);
        }
    }
private:
};*/
//using namespace drivers;

#endif//ULTRASONIC_DRIVER_HPP