#pragma once

#include "KingAttacker.h"
#include "Attackers.h"

namespace forge
{
	class KingAttackers : public boost::container::static_vector<KingAttacker, 2>
	{
	public:
		// --- Alias base class ---
		using super = boost::container::static_vector<KingAttacker, 2>;

	public:
		KingAttackers() = default;
		KingAttackers(const KingAttackers &) = default;
		KingAttackers(KingAttackers &&) noexcept = default;
		virtual ~KingAttackers() noexcept = default;
		KingAttackers & operator=(const KingAttackers &) = default;
		KingAttackers & operator=(KingAttackers &&) noexcept = default;

		void print(const Board & board, std::ostream & os = std::cout) const;

		friend std::ostream & operator<<(std::ostream & os, const KingAttackers & pair)
		{
			pair.print(Board(), os);
		}

		// Coordinates of one attacker (if it exists)
		// attacker1.isValid() will return true iff attacker exists
		KingAttacker & attacker0() { return this->at(0); }
		const KingAttacker & attacker0() const { return this->at(0);	}

		// Coordinates of the other attacker (if it exists)
		// attacker2.isValid() will return true iff attacker exists
		KingAttacker & attacker1() { return this->at(1); }
		const KingAttacker & attacker1() const { return this->at(1); }

		size_t nAttackers() const { return this->size(); }

		bool isFull() const { return this->size() >= 2; }

		bool isNotFull() const { return !isFull(); }

		template<typename RAY_DIRECTION_T>
		void push_back_rays(BoardSquare king, BoardSquare attacker);

		template<typename NON_RAY_DIRECTION>
		void push_back_non_rays(BoardSquare attacker);

		// ---------------------------- STATIC METHODS ------------------------
	public:
		// Searches for any opponent pieces that might be attacking 'ourKing'
		// Returns up to 2 BoardSquares that locate what pieces are attacking 'ourKing'
		// Calling this function should be avoided for performance if 'ourKing' stands on a safe square.
		// This can be determined using the threat board.
		static KingAttackers findKingAttackers(
			const Board & board,
			BoardSquare ourKing,
			BitBoard theirs,
			BitBoard ours);
	};
} // namespace forge

#include "KingAttackersDefinitions.h"