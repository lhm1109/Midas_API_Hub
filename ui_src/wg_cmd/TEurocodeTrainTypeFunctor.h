#pragma once

#include "CMTlgtDataDef.h"

struct TEurocodeTrainTypeFunctor final
{
    typedef std::pair<double, double> Paird;
    typedef std::vector<Paird> vPaird;
    typedef std::pair<int, Paird> IndexPaird;
    template <std::size_t N> using aIndexPaird = std::array<IndexPaird, N>;

public:
    TEurocodeTrainTypeFunctor();

    int GetTypeA(TRAIN_TYPE Type, vPaird& rvLF) const;
    int GetTypeB(UINT unForceNumber, double dSpace, vPaird& rvLF) const;

private:
    template <std::size_t N>
    int CopyArr2Vec(const aIndexPaird<N>& caIVF, vPaird& rvLF) const
    {
        if ( caIVF.empty() )
        {
            ASSERT(0); return 0;
        }

        rvLF.clear();
        rvLF.resize(caIVF.size());
        for ( const auto& IVF : caIVF )
        {
            const auto& Index = IVF.first - 1;
            const auto& Data = IVF.second;

            rvLF[Index] = Data;
        }

        return static_cast<int>(rvLF.size());
    }
};