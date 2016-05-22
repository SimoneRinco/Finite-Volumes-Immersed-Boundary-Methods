#pragma once

#include "Size.h"
#include "Point.h"
#include "IntervalMap.h"
#include <vector>
#include <limits>
#include <sstream>

#define IMBO_5_MESH_CELL_NOT_FOUND_STRING(x, y) \
    "Cell (" << (x) << ", " << (y) << ") not found\n" 

namespace imbo5{

template<class T>
class Mesh
{
public:

    using Index = unsigned int;
    using CellLabel = Point<Index, 2>;

    /*!
     *  Build a rectangular mesh of the specified size. Assigns the value t to each cell
     */
    Mesh(const Size<2>& size, const T& t)
    :   _cells(size.x() * size.y(), t)
    ,   _raws_info(size.y())
    {
        for (auto& row_info :_raws_info) 
            row_info.column_indexes.add(0, size.x());
    }    

    /*!
     *  Build a rectangular mesh of the specified size. Assign the value f(x, y) to each cell
     */
    template<class Function>
    Mesh(const Size<2>& size, Function f)
    :   _cells(size.x() * size.y())
    ,   _raws_info(size.y())
    {
        for (size_t x = 0; x < size.x(); ++x)
            for (size_t y = 0; y < size.y(); ++y)
                _cells[size.x() * y + x] = f(x, y);

        for (std::size_t i = 0; i < _raws_info.size(); ++i)
        {
            _raws_info[i].column_indexes.add(0, size.x());
            _raws_info[i].cumul_total_cells = size.x() * (i + 1);
        }
    }

    /*!
     *  Set the mesh rectangle [pos_x, pos_x + size_x) x [pos_y, pos_y + size_y) to the value t.
     *  If a mesh cell does not exist it is created.
     */
    void set(const CellLabel& pos, const Size<2>& size, const T& t); 

    /*!
     *  Return a reference to the object at position pos. Throws an exception if the cell does not exist
     */
    T& operator[](const CellLabel& pos)
    {
        auto iy = pos.y();
        if (_raws_info.size() < iy)
        {
            std::stringstream ss;
            ss << IMBO_5_MESH_CELL_NOT_FOUND_STRING(pos.x(), iy);
            throw Exception(ss.str());
        }

        auto ix = pos.x();
        const auto& raw_info = _raws_info[iy];
        auto cell_pos_in_raw = raw_info.column_indexes.get_cells_count(ix);
        if (cell_pos_in_raw == IndexesIntervalMap::max_index)
        {
            std::stringstream ss;
            ss << IMBO_5_MESH_CELL_NOT_FOUND_STRING(ix, iy);
            throw Exception(ss.str());
        }
        
        std::size_t cell_absolute_index = cell_pos_in_raw;
        if (iy > 0)
            cell_absolute_index += _raws_info[iy - 1].cumul_total_cells;
        return _cells[cell_absolute_index];
    }

private:

    /*!
     *  An interval map suitable to keep all the column indexes of the cells for a particular raw.
     */
    class IndexesIntervalMap : private IntervalMap<Index>
    {
    public:

        using Base = IntervalMap<Index>;
        static const auto max_index = std::numeric_limits<Index>::max();
        void add(Index min, Index sup)
        {
            Base::add(min, sup);
            
            // recalculate this from scratch. TODO improve
            _interval_widths_cs.resize(_intervals_map.size());
            auto map_it = _intervals_map.begin();
            _interval_widths_cs[0] = map_it->second - map_it->first;
            while(++map_it != _intervals_map.end())
            {
                _interval_widths_cs.push_back(_interval_widths_cs.back() + map_it->second - map_it->first);
            }
        }
    
        /*!
         *  Return how many cells precede the one with index i in the interval map.
         *  Return max_index if the cell with index i does not exist.
         */
        std::size_t get_cells_count(Index i) const
        {
            auto it = get_interval_iterator(i);
            if (it == _intervals_map.end())
                return max_index;
            auto dist = std::distance(it, _intervals_map.begin());
            auto ret = i - it->first;
            if (dist != 0)
                return ret += _interval_widths_cs[dist - 1];
            return ret;
        }

        /*!
         *  Return the total number of cells
         */
        std::size_t get_cells_count() const
        {
            if (_interval_widths_cs.empty())
                return 0;
            return _interval_widths_cs.back();
        }
        

    private:
        
        /*!
         *  component j is the sum of the intervals width for indexes [0, j]
         */
        std::vector<unsigned int> _interval_widths_cs; /*<! intervals widths cumulative sum */
    };

 
    std::vector<T> _cells; /*<! stores all the mesh elements */

    struct RawInfo
    {
        IndexesIntervalMap column_indexes;  /*<! all the column indexes of the current raw */
        std::size_t cumul_total_cells = 0;    /*<! total number of cells from raw 0 to this raw included */
    };

    std::vector<RawInfo> _raws_info; /*<! component r represents the info of row r */
};

}
