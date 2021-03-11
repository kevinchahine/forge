#include "MoveGenerator.h"

using namespace std;

namespace forge
{
	MoveList MoveGenerator::generatePseudoMoves(const Position & position)
	{
		if (position.moveCounter().isWhitesTurn()) {
			return generatePseudoMovesWhite(position);
		}
		else {
			return generatePseudoMovesBlack(position);
		}
	}

	MoveList MoveGenerator::generatePseudoMovesWhite(const Position & position)
	{
		MoveList moves;
		moves.reserve(35);	// At any point in a chess game there will probably be about 35 - 38 legal moves
		
		// --- Alias some objects ---
		const Board & board = position.board();
		BitBoard isOccupied = board.occupied();
		BitBoard isEmpty = board.empty();
		BitBoard whites = board.whites();
		BitBoard blacks = board.blacks();

		// traverse each cell and look for white pieces
		for (uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				BoardSquare square(row, col);
		
				// Does this cell contain a white piece?
				if (whites[square] == 1) {
					// Yes this is a white piece.
					if (board.isPawn(square)) {
						generatePawnMovesWhite(position, square, moves);
					}
					else if (board.isRook(square)) {
						generateRookMoves(position, square, true, moves);						
					}
					else if (board.isKnight(square)) {
					}
					else if (board.isBishop(square)) {
					}
					else if (board.isQueen(square)) {
					}
					else if (board.isKing(square)) {
					}
					else {
						// It must be an error.
						// May cause problems with UCI
						cout << "Uhhoo" << __FUNCTION__ << " line " << __LINE__ << '\n';
					}
				} // if (m_whites
			} // for (col
		} // for (row

		moves.shrink_to_fit();
		return  moves;
	}

	MoveList MoveGenerator::generatePseudoMovesBlack(const Position & position)
	{
		MoveList moves;
		moves.reserve(35);	// At any point in a chess game there will probably be about 35 - 38 legal moves

		// --- Alias some objects ---
		const Board & board = position.board();
		BitBoard isOccupied = board.occupied();
		BitBoard isEmpty = board.empty();
		BitBoard whites = board.whites();
		BitBoard blacks = board.blacks();

		// traverse each cell and look for white pieces
		for (uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				BoardSquare square(row, col);

				// Does this cell contain a white piece?
				if (blacks[square] == 1) {
					// Yes this is a black piece.
					if (board.isPawn(square)) {
						generatePawnMovesBlack(position, square, moves);
					}
					else if (board.isRook(square)) {
						generateRookMoves(position, square, false, moves);
					}
					else if (board.isKnight(square)) {
					}
					else if (board.isBishop(square)) {
					}
					else if (board.isQueen(square)) {
					}
					else if (board.isKing(square)) {
					}
					else {
						// It must be an error.
						// May cause problems with UCI
						cout << "Uhhoo" << __FUNCTION__ << " line " << __LINE__ << '\n';
					}
				} // if (m_whites
			} // for (col
		} // for (row

		moves.shrink_to_fit();
		return  moves;
	}

	MoveList MoveGenerator::generatePseudoMovesCurr(const Position & position)
	{
		if (position.moveCounter().isWhitesTurn()) {
			return generatePseudoMovesWhite(position);
		}
		else {
			return generatePseudoMovesBlack(position);
		}
	}

	void MoveGenerator::generatePseudoMovesBoth(const Position & position, MoveList & moves)
	{

	}
	
} // namespace forge
