/* splitAxisRangeStrategy.hpp
 *
 *  Created on: Jan 22, 2017
 *      Author: benross
 */

#ifndef INCLUDE_SPLITAXISRANGESTRATEGY_HPP_
#define INCLUDE_SPLITAXISRANGESTRATEGY_HPP_

#include <vector>
#include <iostream>

#include "SplitAxisStrategy.hpp"

namespace rossb83 {

template<typename T>
class SplitAxisRangeStrategy : public SplitAxisStrategy<T> {

public:

    // split axis based on largest range
    std::size_t splitAxis(const std::vector<Point<T>>& points, const std::size_t& begin, const std::size_t& end) {

	// point dimensions in input point set
    	std::size_t dims = points[0].dims();

        // state variables to keep track of max range and its corresponding dimension
        std::size_t splitaxis = 0;
        std::size_t maxrangesofar = 0;

	for (std::size_t axis = 0; axis < dims; axis++) {

            Point<T> min = *(std::min_element(points.begin() + begin, points.begin() + end + 1,
                [&axis](Point<T> p1, Point<T> p2) {return p1[axis] < p2[axis];}));

            Point<T> max = *(std::max_element(points.begin() + begin, points.begin() + end + 1,
                [&axis](Point<T> p1, Point<T> p2) {return p1[axis] < p2[axis];}));

            std::size_t range = max[axis] - min[axis];

            if (range > maxrangesofar) {

                splitaxis = axis;
                maxrangesofar = range;
            }
	}

        return splitaxis;
    }

};// class Split

} // namespace rossb83

#endif /* INCLUDE_SPLITAXISROUNDROBINSTRATEGY_HPP_ */
