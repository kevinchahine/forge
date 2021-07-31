#include "MovePositionPair.h"

#include "HashCombine.h"

namespace forge
{
    bool MovePositionPair::operator==(const MovePositionPair& pair) const
    {
        return this->hash() == pair.hash();
    }

    bool MovePositionPair::operator<(const MovePositionPair& pair) const
    {
        return this->hash() < pair.hash();
    }

    size_t MovePositionPair::hash() const
    {
        size_t hash = 0;

        hash_combine(hash, position.hash());
        //hash_combine(hash, move.hash());

        return hash;
    }
} // namespace forge