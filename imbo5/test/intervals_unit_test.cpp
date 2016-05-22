#include "IntervalMap.h"
#include <gtest/gtest.h>
#include <vector>
#include <array>
#include <cstdlib>

using IntervalVector = std::vector<imbo5::Interval<int>>;
using IntervalInt = IntervalVector::value_type;


std::ostream& operator <<(std::ostream& os, const IntervalVector& iv)
{
    for (const auto i : iv)
        os << "[" << i.min << ", " << i.sup << ") ";
    return os;
}

void expect_eq_intervals(
    const IntervalVector& expected,
    const imbo5::IntervalMap<int>& im,
    const IntervalVector& sequence
)
{
    IntervalVector actual;
    im.copy_intervals(std::back_inserter(actual));
    auto size = expected.size();
    EXPECT_EQ(size, actual.size()) << "Test correct number of intervals";

    for (std::size_t i = 0; i < size; ++i)
    {
        int expected_values[2] = {expected[i].min, expected[i].sup};
        int actual_values[2] = {actual[i].min,actual[i].sup};
        for (int j = 0; j < 2; ++j)
        {
            EXPECT_EQ(expected_values[j], actual_values[j]) << "Test interval bound equality: Added sequence\n"
                 << sequence << "\nExpected: " << expected << "\nActual: " << actual << "\n" ;
        }
    }
}

TEST(IntervalMap, Add)
{
    int n_tests = 10000;
    const int sup = 1001;

    srand(time(nullptr));

    for(; n_tests > 0; --n_tests)
    {
        imbo5::IntervalMap<int> interval_map;
        std::vector<int> data(sup, 0); // data[i] = 1 if i is present the map, 0 otherwise
        IntervalVector sequence;
        int n_intervals = rand() % 30 + 5; // random number of intervals to add in range [5, 35)
        for (int intervals = 0; intervals < n_intervals;)
        {
            int min_i = rand() % sup;
            int sup_i = rand() % sup;  

            if (min_i < sup_i)
            {
                interval_map.add(min_i, sup_i);
                sequence.push_back(IntervalInt(min_i, sup_i));
                for (int i = min_i; i < sup_i; ++i)
                    data[i] = 1;
                ++intervals;
            }
        }
    
        IntervalVector expected_intervals;
        for (int i = 0; i < sup;)
        {
            if (data[i] == 1)
            {
                int j = i;
                for (; data[j] == 1; ++j);
                expected_intervals.push_back(IntervalInt(i, j));
                i = j + 1;
            }
            else
                ++i;
        }

        expect_eq_intervals(expected_intervals, interval_map, sequence);
    }
}

TEST(IntervalMap, GetIterator)
{
    class TestGetIterator : public imbo5::IntervalMap<int>
    {
        public:
           int dist_from_begin(int i) const
           {
                auto it = get_interval_iterator(i);
                if (it == _intervals_map.end())
                    return -1;
                return std::distance(_intervals_map.begin(), it);
           }
    };

    std::vector<std::array<int, 2>> intervals
    {
        {1, 3},
        {5, 6},
        {11, 14},
        {31, 45}
    };
    
    std::vector<int> expected_results(50, -1); // the value is the expected distance from begin
    int expected_dist_from_begin = 0;
    for(const auto& interval : intervals)
    {
        for (int i = interval[0];i < interval[1]; ++i)
        {
            expected_results[i] = expected_dist_from_begin;
        }

        ++expected_dist_from_begin; 
    }

    TestGetIterator test_map;
    
    for(const auto& interval : intervals)
    {
       test_map.add(interval[0], interval[1]);
    }

    for(auto i = 0; i < expected_results.size(); ++i)
    {
        auto expected_distance = expected_results[i];
        auto actual_distance = test_map.dist_from_begin(i);
        EXPECT_EQ(expected_distance, actual_distance) << "Element: " << i << "\n ";
    }
} 
            
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
