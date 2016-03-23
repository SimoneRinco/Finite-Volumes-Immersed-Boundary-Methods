#include <vector>

namespace imbo5
{
    template<class T>
    class Mesh
    {
    public:
        
        using Index = std::size_t;
        static const Index invalid_index = -1;

        /*!
         *  Return a reference to the mesh element at row i and column j.Throws an exception if the element is not
         *  in the mesh
         */
        const T& operator()(Index i, Index j) const
        {
        }

        T& operator()(Index i, Index j)
        {
            return const_cast<T&>(static_cast<const T&>(*this)(i, j));
        }

        /*!
         *  Add an element to the mesh at position (i, j)
         */
        void add(Index i, Index j, const T& element)
        {

        };   

    private:
        
        using ActualIndex = Index;  /**< This is useful semantically. It represents an actual index in the underlying data structures. */ 

        /*!
         *  Represent the indexes of consecutive elements in a mesh row in the form [inf, sup)
         */
        struct IndexInterval
        {
            std::size_t inf;
            std::size_t sup;
        };        

        /*!
         *  Describe the structure of a row in the mesh
         */
        class RowStructure
        {
        public:

            /*!
             *  Add an element index to the row
             */
            void add(Index i)
            {
            }

            /*!
             * Return the actual index in the row of an element at index i. Return invalid index if the element is not present
             * in the row
             */
            ActualIndex actual_index(Index i)
            {
                // It is very simple: first we find the interval where the index i is with a binary search.
                // Then we sum the position of i in the interval (which is i - the interval inf)
                // to the partial sum of the previous interval.

                if (_index_intervals.empty())
                    return invalid_index;

                std::size_t inf_bound = 0;
                std::size_t sup_bound = _index_intervals.size(-1);
                
                for (std::size_t k = (sup_bound - inf_bound) / 2; inf_bound < sup_bound; k = (sup_bound - inf_bound) / 2)
                {
                    IndexInterval& i_interval = _index_intervals[k];
                    if (i < i_interval.inf)
                    {
                        sup_bound = k;
                    }
                    else if (i > i_interval.sup)
                    {
                        inf_bound = k;
                    }
                    else if (i == i_interval.sup)
                    {
                        // this is because the upper bound is excluded and _index_intervals is canonical
                        return invalid_index;
                    }
                    else
                    {
                        // found!
                        auto pos_in_interval = i - i_interval;
                        return k == 0 ? pos_in_interval : _index_intervals[k - 1].partial_sum + pos_in_interval;
                    }
                }
            }

        private:

            /*!
             * Represent the idexes of consecutive elements in a mesh row in the form [inf, sup).    
             */
            struct IndexesInterval
            {
                Index inf;
                Index sup;
                Index partial_sum;  /**< How many elements are present in the row before this chunk of consecutive elements */
            };

        private:

            std::vector<IndexesInterval> _index_intervals;
        };

        std::vector<RowStructure> _mesh_structure;


        std::vector<T> _elements;   /**< The actual elments in the grid */

    
    };
}
