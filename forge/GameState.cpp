#include "GameState.h"

using namespace std;

namespace forge
{
	std::ostream & forge::operator<<(std::ostream & os, const GameState & gs)
	{
		if (gs.state != GameState::STATE::DRAW) {
			switch (gs.player)
			{
			case GameState::PLAYER::WHITE:	os << "White";	break;
			case GameState::PLAYER::BLACK:	os << "Black";	break;
			}
		}

		switch (gs.state)
		{
		case GameState::STATE::CONTINUE:	os << "'s turn";	break;
		case GameState::STATE::WIN:			os << " won";		break;
		case GameState::STATE::DRAW:		os << "Draw";		break;
		}

		switch (gs.reason)
		{
		case GameState::REASON::NOTHING:												break;
		case GameState::REASON::AGREEMENT:					os << " by aggreement";		break;
		case GameState::REASON::RESIGNATION:				os << " by resignation";	break;
		case GameState::REASON::CHECKMATE:					os << " by checkmate";		break;
		case GameState::REASON::CANT_MOVE:					os << " by not moving";		break;
		case GameState::REASON::REPETITION:					os << " by repetition";		break;
		case GameState::REASON::FIFTY_MOVE_RULE:			os << " by 50 Move Rule";	break;
		case GameState::REASON::INSUFFICIENT_MATERIAL_ONLY: os << " by insufficient material";	break;
		case GameState::REASON::TIMEOUT_ONLY:				os << " by timeout";		break;
		case GameState::REASON::TIMEOUT_AND_INSUFFICIENT_MATERIAL:	os << " by timeout with insufficient material";  break;
		}

		return os;
	}

	void GameState::operator()(const Position & position)
	{
	}

} // namespace forge