#pragma once

#include "WeightsArchive.h"

namespace forge
{
	class IWeightsArchive : public WeightsArchive
	{
	public:
		template<typename T>
		WeightsArchive & operator&(T & t);
	};

	template<typename T>
	WeightsArchive & IWeightsArchive::operator&(T & t)
	{
		(*this) << t;
		
		return *this;
	}
} // namespace forge

