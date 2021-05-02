#pragma once

#include "HeuristicBase.h"

#include <deque>
#include <vector>
#include <algorithm>

namespace forge
{
	// Purpose: Stores weights of any Weights class in a single vector 
	// making it ideal for training algorithms, logging and saving weights
	// in a file. Weight classes can serialize/parse their data into/from a WeightsArchive
	// object. The weights can be stored in any data structure as long as the
	// weights are of type 'heuristic_t'.
	// ex: vector<heuristic_t> can be serialized into a WeightsArchive but
	//	vector<float> or vector<uint8_t> can't (Unless ofcourse someone changes the type
	//	of heuristic_t to float or uint8_t).
	class WeightsArchive : public std::deque<heuristic_t>
	{
	public:
		// TODO: Add the & operator overload. Match it up with Boost::Serialization

		// Copies weights from WeightsArchive into a container of type
		// CONTAINER_T.
		// Typically CONTAINER_T would be a vector<heuristic_t>
		template<typename CONTAINER_T>
		CONTAINER_T to() const;

		// Copies weights from a container of type
		// CONTAINER_T to WeightsArchive. Contents of this
		// WeightsArchive are deleted before the copy is made
		// Typically CONTAINER_T would be a vector<heuristic_t>
		template<typename CONTAINER_T>
		void from(const CONTAINER_T & data);

		// TODO: Still need stream insersion and extraction operators. 
		// Usefull for storing weights in a file or transfering through IPC.
		// Use Boost to convert the deque into and from text data.
		// Maybe we should just make a serialize method thats compatible with boost::serialization
		///friend std::ostream & operator<<(std::ostream & os, const WeightsArchive & ar);
		///friend std::istream & operator>>(std::istream & is, WeightsArchive & ar);
	};

	template<typename CONTAINER_T>
	CONTAINER_T WeightsArchive::to() const
	{
		// Make sure CONTAINER_T has a push_back and reserve method
		static_assert(std::is_invocable<CONTAINER_T::push_back, int>(), "CONTAINER_T must to have a push_back method");;
		static_assert(std::is_invocable<CONTAINER_T::reserve>(), "CONTAINER_T must have a reserve method");

		CONTAINER_T cont;
		cont.resize(this->size());

		copy(this->begin(), this->end(), back_inserter(cont));

		return cont;
	}

	template<typename CONTAINER_T>
	void WeightsArchive::from(const CONTAINER_T & data)
	{
		// Make sure CONTAINER_T has a push_back and reserve method
		static_assert(std::is_invocable<CONTAINER_T::begin>(), "CONTAINER_T must have a begin method");
		static_assert(std::is_invocable<CONTAINER_T::end>(), "CONTAINER_T must have an end method");

		this->clear();

		copy(data.begin(), data.end(), back_inserter(*this));// static_cast<&std::deque<heuristic_t>>(*this)));
	}
} // namespace forge