#include <Heisenberg.h++>
#include <LatticeIppl.h++>

enum MoveType
{
    SpinFlip,
    Random,
    SmallStep,
    Addaptive
};

bool metropolis_ippl(       LatticeIppl& latticeIppl,
                            flt const& T /*temperature*/,
                            flt const& J /*interaction Strength*/,
                            flt const& maxTimeSeconds,
                            u64 const& maxSteps,
                            Spin const& h = {0,0,0},
                            Spin const& k = {0,0,0});