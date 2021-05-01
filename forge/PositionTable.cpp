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

	void PositionTable::parse(const WeightsArchive & ar)
	{
		// copy array elements from archive
		for_each(this->data(), this->data() + this->size(), [&](const auto & row) {
			copy_n(row.data(), row.size(), back_inserter(ar));
		});
	}
} // namespace forge