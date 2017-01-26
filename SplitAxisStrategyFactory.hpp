/*SplitAxisStrategyFactory.hpp
 *
 *  Created on: Jan 22, 2017
 *      Author: benross
 */

#ifndef INCLUDE_SPLITAXISSTRATEGYFACTORY_HPP_
#define INCLUDE_SPLITAXISSTRATEGYFACTORY_HPP_

#include "SplitAxisStrategy.hpp"
#include "SplitAxisRoundRobinStrategy.hpp"
#include "SplitAxisRangeStrategy.hpp"

#include <unordered_map>
#include <string>
#include <map>

namespace rossb83 {

/*
 * class to create strategy to split axis based on input string decision
 */
template <typename T>
class SplitAxisStrategyFactory {

    public:

	static std::shared_ptr<SplitAxisStrategy<T>> createSplitAxisStrategy(const std::string& strategy) {

            if (strategy == "cycle") {
                return std::make_shared<SplitAxisRoundRobinStrategy<T>>();
            } else if (strategy == "range") {
                return std::make_shared<SplitAxisRangeStrategy<T>>();
            } else {
                return std::make_shared<SplitAxisRoundRobinStrategy<T>>();
            }
	}

}; // class SplitAxisStrategyFactory

} // namespace rossb83

#endif /* INCLUDE_SPLITAXISSTRATEGYFACTORY_HPP_ */
