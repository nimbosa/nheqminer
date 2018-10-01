#pragma once

#include "Solver.h"
#include "SolverStub.h"


#ifdef USE_CPU_TROMP
#include "../cpu_tromp/cpu_tromp.hpp"
#else
CREATE_SOLVER_STUB(cpu_tromp, "cpu_tromp_STUB")
#endif
#ifdef USE_CUDA_TROMP
#include "../cuda_tromp/cuda_tromp.hpp"
#else
CREATE_SOLVER_STUB(cuda_tromp, "cuda_tromp_STUB")
#endif

//namespace AvailableSolvers
//{
//} // AvailableSolvers

// CPU solvers
class CPUSolverTromp : public Solver<cpu_tromp> {
public:
	CPUSolverTromp() : Solver<cpu_tromp>(new cpu_tromp(), SolverType::CPU) {}
	virtual ~CPUSolverTromp() {}
};
// TODO remove platform id for cuda solvers
// CUDA solvers
class CUDASolverTromp : public Solver<cuda_tromp> {
public:
	CUDASolverTromp(int dev_id, int blocks, int threadsperblock) : Solver<cuda_tromp>(new cuda_tromp(0, dev_id), SolverType::CUDA) {
		if (blocks > 0) {
			_context->blocks = blocks;
		}
		if (threadsperblock > 0) {
			_context->threadsperblock = threadsperblock;
		}
	}
	virtual ~CUDASolverTromp() {}
};
