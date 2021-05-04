#pragma once

#include "WeightsArchive.h"

namespace forge
{
	class OWeightsArchive : public WeightsArchive
	{
	public:
		template<typename T>
		WeightsArchive & operator&(T & t);
	};

	template<typename T>
	WeightsArchive & OWeightsArchive::operator&(T & t)
	{
		(*this) << t;

		return *this;
	}
} // namespace forge