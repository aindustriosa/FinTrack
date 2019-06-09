#ifndef Statistic_h
#define Statistic_h
//
//    FILE: Statistic.h
//  AUTHOR: Ignacio Gonzalez. igonzalez@aindustriosa.com 
//                  Based on Statistic code by Rob dot Tillaart at gmail dot com
// VERSION: 0.1
// PURPOSE: Simple Statistical library for Arduino
//
// Released to the public domain
//

// the standard deviation increases the lib (<100 bytes)
// it can be in/excluded by un/commenting next line  (compile time)
#define STAT_USE_STDEV 1

#include <Arduino.h>
#include <math.h>

//#define NAN16bit -32768
#define NAN16bit -2048
 
#define STATISTIC_LIB_VERSION "0.1"

class Statistic_int16t
{
public:
    Statistic_int16t();             // "switches on/off" stdev run time
    void clear();            // "switches on/off" stdev run time
    void add(const int16_t);

    // returns the number of values added
    uint32_t count() const { return _cnt; }; // zero if empty
    int32_t sum() const      { return _sum; }; // zero if empty
    int16_t minimum() const  { return _min; }; // zero if empty
    int16_t maximum() const  { return _max; }; // zero if empty
    int16_t average() const;                   // NAN if empty

#ifdef STAT_USE_STDEV
    int16_t variance() const;                  // NAN if empty
    int16_t pop_stdev() const; // population stdev  // NAN if empty
    int16_t unbiased_stdev() const;            // NAN if empty
#endif

protected:
    uint32_t _cnt;
    int32_t    _sum;
    int16_t    _min;
    int16_t    _max;
#ifdef STAT_USE_STDEV
    float    _ssqdif;		    // sum of squares difference
#endif
};


#endif
// END OF FILE
