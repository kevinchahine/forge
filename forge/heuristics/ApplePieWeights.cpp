#include "ApplePieWeights.h"

namespace forge
{
	bool ApplePieWeights::operator==(const ApplePieWeights & rhs) const
	{
		return static_cast<const WeightsBase&>(*this) == static_cast<const WeightsBase&>(rhs) &&
			// --- Material ---
			queenMaterial == rhs.queenMaterial &&
			rookMaterial == rhs.rookMaterial &&
			bishopMaterial == rhs.bishopMaterial &&
			knightMaterial == rhs.knightMaterial &&
			pawnMaterial == rhs.pawnMaterial &&
			bishopPair == rhs.bishopPair &&
			oppositeBishop == rhs.oppositeBishop &&

			// --- Mobility ---
			queenMobility == rhs.queenMobility &&
			rookMobility == rhs.rookMobility &&
			bishopMobility == rhs.bishopMobility &&
			knightMobility == rhs.knightMobility &&
			pawnMobility == rhs.pawnMobility &&
			kingMobility == rhs.kingMobility &&

			// --- Piece Square Table Bonus ---
			queenPSTB == rhs.queenPSTB &&
			rookPSTB == rhs.rookPSTB &&
			bishopPSTB == rhs.bishopPSTB &&
			knightPSTB == rhs.knightPSTB &&
			pawnPSTB == rhs.pawnPSTB &&
			kingPSTB == rhs.kingPSTB &&

			// --- Defended Pieces ---
			queensDefended == rhs.queensDefended &&
			rooksDefended == rhs.rooksDefended &&
			bishopsDefended == rhs.bishopsDefended &&
			knightsDefended == rhs.knightsDefended &&
			pawnsDefended == rhs.pawnsDefended &&
			kingsDefended == rhs.kingsDefended &&

			true;
	}

	void ApplePieWeights::serialize(WeightsArchive & ar) const
	{
		// --- Material ---
		ar.push_back(queenMaterial);
		ar.push_back(rookMaterial);
		ar.push_back(bishopMaterial);
		ar.push_back(knightMaterial);
		ar.push_back(pawnMaterial);
		ar.push_back(bishopPair);
		ar.push_back(oppositeBishop);

		// --- Mobility ---
		ar.push_back(queenMobility);
		ar.push_back(rookMobility);
		ar.push_back(bishopMobility);
		ar.push_back(knightMobility);
		ar.push_back(pawnMobility);
		ar.push_back(kingMobility);

		// --- Piece Square Table Bonus ---
		queenPSTB.serialize(ar);
		rookPSTB.serialize(ar);
		bishopPSTB.serialize(ar);
		knightPSTB.serialize(ar);
		pawnPSTB.serialize(ar);
		kingPSTB.serialize(ar);

		// --- Defended Pieces ---
		ar.push_back(queensDefended);
		ar.push_back(rooksDefended);
		ar.push_back(bishopsDefended);
		ar.push_back(knightsDefended);
		ar.push_back(pawnsDefended);
		ar.push_back(kingsDefended);
	}

	void ApplePieWeights::parse(WeightsArchive & ar)
	{
		// --- Material ---
		queenMaterial = ar.front();		ar.pop_front();
		rookMaterial = ar.front();		ar.pop_front();
		bishopMaterial = ar.front();		ar.pop_front();
		knightMaterial = ar.front();		ar.pop_front();
		pawnMaterial = ar.front();		ar.pop_front();
		bishopPair = ar.front();		ar.pop_front();
		oppositeBishop = ar.front();	ar.pop_front();

		// --- Mobility ---
		queenMobility = ar.front();		ar.pop_front();
		rookMobility = ar.front();		ar.pop_front();
		bishopMobility = ar.front();	ar.pop_front();
		knightMobility = ar.front();	ar.pop_front();
		pawnMobility = ar.front();		ar.pop_front();
		kingMobility = ar.front();		ar.pop_front();

		// --- Piece Square Table Bonus ---
		//queenPSTB.parse(ar);
		//rookPSTB.parse(ar);
		//bishopPSTB.parse(ar);
		//knightPSTB.parse(ar);
		//pawnPSTB.parse(ar);
		//kingPSTB.parse(ar);

		// --- Defended Pieces ---
		queensDefended = ar.front();	ar.pop_front();
		rooksDefended = ar.front();		ar.pop_front();
		bishopsDefended = ar.front();	ar.pop_front();
		knightsDefended = ar.front();	ar.pop_front();
		pawnsDefended = ar.front();		ar.pop_front();
		kingsDefended = ar.front();		ar.pop_front();
	}
} // namespace forge