#pragma once

#include "Exception.h"
#include <map>

namespace imbo5
{
    /*!
     *  Represent an interval in the form [min, sup)
     */
    template<class T>
    struct Interval
    {
        Interval(const T& min, const T& sup)
        : min(min)
        , sup(sup)
        {}

        T min;
        T sup;
    };

    /*!
     *  Represents a set of intervals in the form [).
     *  S = U_{i = 0}^n [a_i, b_i) with a_i < b_i.
     *  Template parameters T must be copyable, comparable by means of operators < and ==
     */
    template<class T>
    class IntervalMap
    {
    public:

        using Map = std::map<T, T>;
        typedef typename Map::value_type MapEntry;

        /*!
         *  Add interval [min, sup) to the interval map. min must be less than sup
         */
        void add(const T& min, const T& sup)
        {
            if (!(min < sup))
            {
               throw Exception("Invalid arguments adding an interval to the IntervalMap");
            }

            if (_intervals_map.empty())
            {
                _intervals_map.insert(MapEntry(min, sup));
            }
            else
            {
                auto min_lb_it = _intervals_map.lower_bound(min);

                if (min_lb_it == _intervals_map.end())
                {
                    // [7, 15) [21, 45) end
                    //                   ^
                    //               min_lb_it
                    //
                    // min > 21

                    --min_lb_it;
                    if (!(min_lb_it->second < min))
                    {
                        if (min_lb_it->second < sup)
                            min_lb_it->second = sup;
                    }
                    else
                    {
                        _intervals_map.insert(_intervals_map.end(), MapEntry(min, sup));
                    }
                } 
                else
                {
                    auto sup_lb_it = _intervals_map.lower_bound(sup);
                    if (min_lb_it == _intervals_map.begin())
                    {
                        if (min_lb_it == sup_lb_it)
                        {
                            // [7, 15) ...
                            //   ^
                            // min_lb_it
                            // sup_lb_it
                            //
                            // min <= 7, sup <= 7

                            if (sup == min_lb_it->first)
                            {                        
                                T new_sup = min_lb_it->second;
                                _intervals_map.erase(min_lb_it);
                                _intervals_map.insert(_intervals_map.begin(), MapEntry(min, new_sup));
                            }
                            else
                            {
                                _intervals_map.insert(_intervals_map.begin(), MapEntry(min, sup));
                            }
                        }
                        else if (sup_lb_it == _intervals_map.end())
                        {
                            // [7, 15) ... [18, 34) end
                            //   ^                   ^
                            // min_lb_it         sup_lb_it
                            //
                            // min <= 7, sup > 18
                            
                            --sup_lb_it;
                            T new_sup = std::max(sup, sup_lb_it->second);
                            if (min == min_lb_it->first)
                            {
                                min_lb_it->second = new_sup;
                                ++min_lb_it;
                                _intervals_map.erase(min_lb_it, _intervals_map.end());
                            }
                            else
                            {
                                _intervals_map.clear();
                                _intervals_map.insert(MapEntry(min, new_sup));
                            } 
                        }
                        else
                        {
                            // [7, 15) ... [18, 34) [45, 77) ...
                            //   ^                     ^
                            // min_lb_it           sup_lb_it
                            //
                            // min <= 7, sup in (18, 45]
                            T new_sup;
                            if (sup < sup_lb_it->first)
                            {
                                --sup_lb_it;
                                new_sup = std::max(sup, sup_lb_it->second);
                            }
                            else
                            {
                                new_sup = sup_lb_it->second;
                            }
 
                            if (min == min_lb_it->first)
                            {
                                min_lb_it->second = new_sup;
                                _intervals_map.erase(++min_lb_it, ++sup_lb_it);
                            }
                            else
                            {
                                _intervals_map.erase(min_lb_it, ++sup_lb_it);
                                _intervals_map.insert(_intervals_map.begin(), MapEntry(min, new_sup));
                            }
                        }
                    }
                    else
                    {
                        // at least 2 elements in the map
                        if (min_lb_it == sup_lb_it)
                        {
                            // .... [21, 25) [32, 44) ....
                            //                  ^
                            //              min_lb_it  
                            //              sup_lb_it
                            //
                            //  min and sup are in (21, 32]
    
                            --min_lb_it;
                            if (min_lb_it->second < sup)
                            {
                                if (sup < sup_lb_it->first)
                                {
                                    if (min_lb_it->second < min)
                                        _intervals_map.insert(sup_lb_it, MapEntry(min, sup));
                                    else
                                        min_lb_it->second = sup;
                                }
                                else if (!(min_lb_it->second < min))
                                {
                                    min_lb_it->second = sup_lb_it->second;
                                    _intervals_map.erase(sup_lb_it);
                                }
                                else
                                {
                                    T new_sup = sup_lb_it->second;
                                    _intervals_map.erase(sup_lb_it);
                                    _intervals_map.insert(MapEntry(min, new_sup));
                                }
                           }
                        }
                        else if (sup_lb_it != _intervals_map.end())
                        {
                            // .... [21, 25) [32, 44) [50, 54) [72, 78) [81, 101) ....
                            //                  ^                          ^
                            //              min_lb_it                  sup_lb_it
                            //
                            // min in (21, 32], sup in (72, 81]

                            if (sup < sup_lb_it->first)
                            {
                                --min_lb_it;
                                --sup_lb_it;
                                T new_sup = std::max(sup, sup_lb_it->second);
                                if (!(min_lb_it->second < min))
                                {
                                    min_lb_it->second = new_sup;
                                    _intervals_map.erase(++min_lb_it, ++sup_lb_it);
                                }
                                else
                                {
                                    ++min_lb_it;
                                    if (min < min_lb_it->first)
                                    {
                                        _intervals_map.erase(min_lb_it, ++sup_lb_it);
                                        _intervals_map.insert(MapEntry(min, new_sup));
                                    }
                                    else
                                    {
                                        min_lb_it->second = new_sup;
                                        _intervals_map.erase(++min_lb_it, ++sup_lb_it);
                                    }
                                }
                            }
                            else
                            {
                                --min_lb_it;
                                T new_sup = sup_lb_it->second;
                                if (!(min_lb_it->second < min))
                                {
                                    min_lb_it->second = new_sup;
                                    _intervals_map.erase(++min_lb_it, ++sup_lb_it);
                                }
                                else
                                {
                                    ++min_lb_it;    
                                    if (min < min_lb_it->first)
                                    {
                                        _intervals_map.erase(min_lb_it, ++sup_lb_it);
                                        _intervals_map.insert(MapEntry(min, new_sup));
                                    }
                                    else
                                    {
                                        min_lb_it->second = new_sup;
                                        _intervals_map.erase(++min_lb_it, ++sup_lb_it);
                                    }
                                }
                            }
                        }
                        else
                        {
                            // [21, 25) [32, 44) .... [116, 121) end
                            //             ^                      ^
                            //         min_lb_it              sup_lb_it
                            //
                            // min in (21, 32], sup > 116
                            --sup_lb_it;
                            T new_sup = std::max(sup_lb_it->second, sup);
                            --min_lb_it;
                            if (!(min_lb_it->second < min))
                            {
                                min_lb_it->second = new_sup;
                                _intervals_map.erase(++min_lb_it, _intervals_map.end());
                            }
                            else
                            {
                                ++min_lb_it;
                                if (min < min_lb_it->first)
                                {
                                    _intervals_map.erase(min_lb_it, _intervals_map.end());
                                    _intervals_map.insert(MapEntry(min, new_sup));
                                }
                                else
                                {
                                    min_lb_it->second = new_sup;
                                    _intervals_map.erase(++min_lb_it, _intervals_map.end());
                                }
                            }
                        }
                    }
                }   
            }
        }

        /*!
         *  Copy the intervals (of type Interval)  in the range starting at iterator it
         */
        template<class OutputIterator>
        void copy_intervals(OutputIterator out) const
        {
            for (const auto& interval_pair : _intervals_map)
            {
                *out++ = Interval<T>(interval_pair.first, interval_pair.second);
            }
        }

        /*!
         *  Return true if t is present in the interval set
         */
        bool contains(const T& t) const
        {
            return get_interval_iterator(t) != _intervals_map.end();
        }

        /*!
         *  Return the interval which contains the specified element. Throw an exception if the
         *  element is not present in the interval map.
         */
        MapEntry get_interval(const T& element) const
        {
            auto it = get_interval_iterator(element);
            if (it == _intervals_map.end())
                throw std::runtime_error("Element not present in the interval map");
            return *it;
        }

    protected:

        /*!
         *  Return an iterator to the interval containing t. Return an iterator
         *  to the map end if t is not in the interval map
         */
        typename Map::const_iterator get_interval_iterator(const T& t) const
        {
            if (_intervals_map.empty())
                return _intervals_map.end();

            auto it = _intervals_map.lower_bound(t);
            
            // t = 15
            // ... [13, x)
            if (it == _intervals_map.end())
            {
                --it;
                if (t < it->second)
                    return it;
                else
                    return _intervals_map.end();
            }

            if (it == _intervals_map.begin())
            {
                if (t == it->first)
                    return it;
                else
                    return _intervals_map.end();
            }

            // ... [x, y) [15, z)
            if (t == it->first)
                return it;

            // ... [10, x) [22, 34) ...
            --it;
            if (t < it->second)
                return it;
            else
                return _intervals_map.end();

            // avoid warning on some compilers. This line is unreachable
            return _intervals_map.end();
        }

        /*!
         *  The key is the interval minimum and the value the interval sup. Remember that each interval is in the form [)
         *  The map must be canonical (it should contain the least possible information to store the intervals correctly):
         *  -   intervals don't overlap;
         *  -   there should be a gap between two consecutive intervals, otherwise they can be merged.
         *
         *  These requirements are satisfied if the sup of an interval is less than the minimum of the next interval
         */
         Map _intervals_map;
    };
}
