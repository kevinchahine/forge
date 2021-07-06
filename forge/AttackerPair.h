#pragma once

#include "Board.h"
#include "BoardSquare.h"
#include <boost/container/static_vector.hpp>

namespace forge
{
	class AttackerPair : public boost::container::static_vector<BoardSquare, 2>
	{
	public:
		AttackerPair() = default;
		AttackerPair(const AttackerPair &) = default;
		AttackerPair(AttackerPair &&) noexcept = default;
		virtual ~AttackerPair() noexcept = default;
		AttackerPair & operator=(const AttackerPair &) = default;
		AttackerPair & operator=(AttackerPair &&) noexcept = default;

		void print(const Board & board, std::ostream & os = std::cout) const;

		friend std::ostream & operator<<(std::ostream & os, const AttackerPair & pair)
		{
			pair.print(Board(), os);
		}

		// Coordinates of one attacker (if it exists)
		// attacker1.isValid() will return true iff attacker exists
		BoardSquare & attacker0() { return this->at(0); }
		const BoardSquare & attacker0() const { return this->at(0);	}

		// Coordinates of the other attacker (if it exists)
		// attacker2.isValid() will return true iff attacker exists
		BoardSquare & attacker1() { return this->at(1); }
		const BoardSquare & attacker1() const { return this->at(1); }

		size_t nAttackers() const { return this->size(); }

		bool isFull() const { return this->size() >= 2; }

		bool isNotFull() const { return !isFull(); }
	};
} // namespace forge