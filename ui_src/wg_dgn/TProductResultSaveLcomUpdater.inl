#pragma once

namespace dgn
{
    namespace converter
    {
        template <int _nSize>
        TProductResultSaveLcomUpdater::TProductResultSaveLcomUpdater(const std::array<enUpdateType, _nSize>& caType)
        {
            Init(caType);

            return;
        }

        template <int _nSize>
        void TProductResultSaveLcomUpdater::Init(const std::array<enUpdateType, _nSize>& caType)
        {
            m_mTypeKeyRatio.clear();
            for ( const auto& Type : caType )
            {
                m_mbTypeInit[Type] = true;
                m_mTypeKeyRatio[Type] = TKeyRatio(0, 0.0);
            }

            return;
        }
    }
}