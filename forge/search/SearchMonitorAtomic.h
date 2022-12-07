#pragma once

#include "forge/search/SearchMonitorTemplate.h"

#include <boost/atomic/atomic.hpp>

namespace forge
{
	class SearchMonitorAtomic : public SearchMonitorTemplate<boost::atomic> {
	public:
	};
} // namespace forge