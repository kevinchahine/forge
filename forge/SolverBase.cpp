#include "SolverBase.h"

namespace forge
{
    SolverBase::SolverBase(const SolverBase& solver) :
        ControllerBase(solver),
        m_heuristicPtr(solver.m_heuristicPtr->clone()),
        m_searchMonitor(solver.m_searchMonitor)  
    {
    }

    SolverBase& SolverBase::operator=(const SolverBase& solver)
    {
        static_cast<ControllerBase&>(*this) = solver;
        m_heuristicPtr = solver.m_heuristicPtr->clone();
        m_searchMonitor = solver.m_searchMonitor;

        return *this;
    }
} // namespace forge