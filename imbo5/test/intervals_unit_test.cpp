#include "IntervalMap.h"
#include <gtest/gtest.h>
#include <vector>

using IntervalVector = std::vector<imbo5::Interval<int>>;
using IntervalInt = IntervalVector::value_type;

void expect_eq_intervals(const IntervalVector& expected, const imbo5::IntervalMap<int>& im)
{
    IntervalVector actual;
    im.copy_intervals(std::back_inserter(actual));
    auto size = expected.size();
    EXPECT_EQ(size, actual.size()) << "Test correct number of intervals";

    for (std::size_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(expected[i].min, actual[i].min) << "Test interval min equality";
        EXPECT_EQ(expected[i].sup, actual[i].sup) << "Test interval sup equality";
    }
}

TEST(IntervalMap, Add)
{
    imbo5::IntervalMap<int> interval_map;


    // 1) Add disjoint intervals to the interval map in different positions

    interval_map.add(13, 21);   // to an empty interval map
    interval_map.add(6, 9);     // at the beginning
    interval_map.add(1,3);      // at the beginning again
    interval_map.add(131,292);  // at the end
    interval_map.add(45, 49);   // in the middle of the map
    interval_map.add(40, 44);   // in the middle of the map again

    IntervalVector expected_intervals = 
    {
        IntervalInt(1, 3),
        IntervalInt(6, 9),
        IntervalInt(13, 21),
        IntervalInt(40, 44),
        IntervalInt(45, 49),
        IntervalInt(131, 292)
    };
 
    expect_eq_intervals(expected_intervals, interval_map);

    // 2) Add overlapping intervals at the beginning
    interval_map.add(-10, 1);   // join the first interval removing [1, 3)
    interval_map.add(-11, 16);  // remove [6, 9) and [13, 21)
    
    expected_intervals = 
    {
        IntervalInt(-11, 21),
        IntervalInt(40, 44),
        IntervalInt(45, 49),
        IntervalInt(131, 292)
    };

    expect_eq_intervals(expected_intervals, interval_map);

    interval_map.add(-11, 300);
    expected_intervals =
    {
        IntervalInt(-11, 300)
    };
    
    expect_eq_intervals(expected_intervals, interval_map);

    interval_map.add(301, 302);
    interval_map.add(307, 314);
    interval_map.add(-12, 308);

    expected_intervals =
    {
        IntervalInt(-12, 314)
    };

    expect_eq_intervals(expected_intervals, interval_map);

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
