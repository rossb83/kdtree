/*SplitPointStrategyFactory.hpp
 *
 *  Created on: Jan 22, 2017
 *      Author: benross
 */

#ifndef INCLUDE_SPLITPOINTSTRATEGYFACTORY_HPP_
#define INCLUDE_SPLITPOINTSTRATEGYFACTORY_HPP_

#include "SplitPointStrategy.hpp"
#include "SplitPointSortStrategy.hpp"
#include "SplitPointSelectStrategy.hpp"

#include <unordered_map>
#include <string>
#include <map>

namespace rossb83 {

/*
 * class to create strategy to split point at median based on input string decision
 */
template <typename T>
class SplitPointStrategyFactory {

    public:

	static std::shared_ptr<SplitPointStrategy<T>> createSplitPointStrategy(const std::string& strategy) {

            if (strategy == "sort") {
                return std::make_shared<SplitPointSortStrategy<T>>();
            } else if (strategy == "select") {
                return std::make_shared<SplitPointSelectStrategy<T>>();
            } else {
                return std::make_shared<SplitPointSortStrategy<T>>();
            }
	}

}; // class SplitPointStrategyFactory

} // namespace rossb83

#endif /* INCLUDE_SPLITPOINTSTRATEGYFACTORY_HPP_ */
