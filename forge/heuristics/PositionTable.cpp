#include "PositionTable.h"

#include <algorithm>

using namespace std;

namespace forge
{
	void PositionTable::serialize(WeightsArchive & ar) const
	{
		// copy array elements to archive
		for_each(this->data(), this->data() + this->size(), [&](const auto & row) {
			copy_n(row.data(), row.size(), back_inserter(ar));
		});
	}

	void PositionTable::parse(WeightsArchive & ar)
	{
		// copy array elements from archive
		// Iterate each row of array.
		for_each(this->data(), this->data() + this->size(), [&](auto & row) {
			// Iterate each element "column" of the current row
			for_each(row.begin(), row.end(), [&](auto & elem) {
				// Copy from archive to Table Element
				elem = ar.front();
			
				// Pop element from archive so it doesn't get reused 
				ar.pop_front();
			});
		});
	} 
} // namespace forge