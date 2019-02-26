//
//    FILE: Statistic.cpp
//  AUTHOR: Ignacio Gonzalez. igonzalez@aindustriosa.com 
//                  Based on Statistic code by Rob dot Tillaart at gmail dot com
// VERSION: 0.1
// PURPOSE: Simple statistical library for Arduino
//
// NOTE: 2011-01-07 Gill Ross
// -------------
//
// HISTORY:
// 0.1 - 2019-02-25 initial version
//
// Released to the public domain
//

#include "Statistic.h"

Statistic_int16t::Statistic_int16t()
{
    clear();
    
}

// resets all counters
void Statistic_int16t::clear()
{
    _cnt = 0;
    _sum = 0;
    _min = 0;
    _max = 0;
#ifdef STAT_USE_STDEV
    _ssqdif = 0.0;  // not _ssq but sum of square differences
    // which is SUM(from i = 1 to N) of
    // (f(i)-_ave_N)**2
#endif
}

// adds a new value to the data-set
void Statistic_int16t::add(const int16_t value)
{
    if (_cnt == 0)
    {
        _min = value;
        _max = value;
    } else {
        if (value < _min) _min = value;
        else if (value > _max) _max = value;
    }
    _sum += value;
    _cnt++;

#ifdef STAT_USE_STDEV
    if (_cnt > 1)
    {
        float _store = ((float)_sum / _cnt - (float)value);
        _ssqdif = _ssqdif + _cnt * _store * _store / (_cnt-1);
        // ~10% faster but limits the amount of samples to 65K as _cnt*_cnt overflows
        // float _store = _sum - _cnt * value;
        // _ssqdif = _ssqdif + _store * _store / (_cnt*_cnt - _cnt);
    }
#endif
}

// returns the average of the data-set added sofar
int16_t Statistic_int16t::average() const
{
    if (_cnt == 0) return NAN16bit; // original code returned 0
    return (int16_t)(_sum / (int32_t)_cnt);
}

// Population standard deviation = s = sqrt [ S ( Xi - � )2 / N ]
// http://www.suite101.com/content/how-is-standard-deviation-used-a99084
#ifdef STAT_USE_STDEV

int16_t Statistic_int16t::variance() const
{
    if (_cnt == 0) return NAN16bit; // otherwise DIV0 error
    return (int16_t) (_ssqdif / _cnt);
}

int16_t Statistic_int16t::pop_stdev() const
{
    if (_cnt == 0) return NAN16bit; // otherwise DIV0 error
    return (int16_t)sqrt( _ssqdif / _cnt);
}

int16_t Statistic_int16t::unbiased_stdev() const
{
    if (_cnt < 2) return NAN16bit; // otherwise DIV0 error
    return (int16_t)sqrt( _ssqdif / (_cnt - 1));
}

#endif
// END OF FILE
