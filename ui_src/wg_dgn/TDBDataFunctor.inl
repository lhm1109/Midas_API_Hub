#pragma once

namespace dgn
{
    namespace converter
    {
        template <typename Type>
        TDBDataFunctor<Type>::TDBDataFunctor()
        {
        }

        template <typename Type>
        const Type* TDBDataFunctor<Type>::operator() (FIterator ItrRefer) const
        {
            const auto* pTDBData =
                FITR_TO_DATA_PTR(TDBData<Type>, ItrRefer);
            if ( !pTDBData )
            {
                ASSERT(0); return nullptr;
            }

            const auto* pDBData = pTDBData->GetData();
            if ( !pDBData )
            {
                ASSERT(0); return nullptr;
            }

            return pDBData;
        }
    }
}