#include "MCTS_Sequential.h"

#include <forge/core/GameState.h>
#include <forge/search/UCB.h>

using namespace std;

namespace forge
{
	void MCTS_Sequential::solve() {	
		m_nodeTree.root().expand();// This code is not redundant
		
		while (true) {
			MCTS_Node::iterator curr = m_nodeTree.root();
		
			// --- Selection ---
			curr = select(curr);
			
			// --- Expand ---
			expand(curr);
			
			// --- Evaluate ---
			EvalVisits ev = evaluate(curr);
			
			// --- BackPropagate ---
			backPropagate(curr, m_nodeTree.root(), ev);
			
			// --- Exit Condition ---
			auto & sm = m_searchMonitor;
			sm.nodeCount.add(ev.visits);
			if (sm.exitConditionReached()) {
				sm.stop();	// stop the clock so we can record exact search time.
				break;
			}
		} // while (true)
	}
} // namespace forge