#pragma once

#include <map>

#include "../wg_dgn/TKeyData.h"

#include "HeaderPre.h"

namespace dgn
{
    namespace converter
    {
        enum class enProductResultSaveLcomUpdateFlag
        {
            Failure,
            Current,
            Previous,
            Number
        };

        enum class enProductResultSaveLcomUpdater
        {
            PM,
            PM_Y,
            PM_Z,
            Shear_Y,
            Shear_Z,
            Shear_End_Y,
            Shear_Mid_Y,
            Shear_End_Z,
            Shear_Mid_Z,
            Joint_Y,
            Joint_Z,
            Cutline_Moment,
            Cutline_Shear,
            Column_2Way,
            Number
        };

        struct __MY_EXT_CLASS__ TProductResultSaveLcomUpdater final
        {
            typedef enProductResultSaveLcomUpdateFlag enUpdateFlag;
            typedef enProductResultSaveLcomUpdater enUpdateType;
            typedef TKeyData<T_KEY, double> TKeyRatio;

            template <int _nSize>
            TProductResultSaveLcomUpdater(const std::array<enUpdateType, _nSize>& caType);

            template <int _nSize>
            void Init(const std::array<enUpdateType, _nSize>& caType);

            enUpdateFlag UpdateKeyRatio(enUpdateType Type, T_KEY EDgnLcomKey, double dCurRatio);

            T_KEY  GetMaxKey(enUpdateType Type) const;
            double GetMaxRatio(enUpdateType Type) const;

        private:
            void SetKeyRatio(enUpdateType Type, T_KEY EDgnLcomKey, double dCurRatio);
            const TKeyRatio& GetKeyRatio(enUpdateType Type) const;

        private:
            std::map<enUpdateType, bool> m_mbTypeInit;
            std::map<enUpdateType, TKeyRatio> m_mTypeKeyRatio;
        };

        struct __MY_EXT_CLASS__ TProductResultSaveMembUpdater final
        {
            std::map<T_KEY, TProductResultSaveLcomUpdater*> mappLcomUpdater;

            ~TProductResultSaveMembUpdater()
            {
                Init();
            }

            TProductResultSaveLcomUpdater* GetLcomUpdater(T_KEY MembKey)
            {
                auto itr = mappLcomUpdater.find(MembKey);
                if(itr != mappLcomUpdater.end())
                {
					return itr->second;
				}

                return nullptr;
            }
            
            void SetLcomUpdater(T_KEY MembKey, TProductResultSaveLcomUpdater* pLcomUpdater)
            {
				auto itr = mappLcomUpdater.find(MembKey);
				if(itr != mappLcomUpdater.end()) 
				{
                    ASSERT(0);
                    return;
				}
                mappLcomUpdater[MembKey] = pLcomUpdater;
            }

            void Init()
            {
				for(auto& rPair : mappLcomUpdater)
				{
					if(rPair.second)
					{
						delete rPair.second;
						rPair.second = nullptr;
					}
				}
				mappLcomUpdater.clear();
            }
        };
    }
}

#include "HeaderPost.h"

#include "TProductResultSaveLcomUpdater.inl"
