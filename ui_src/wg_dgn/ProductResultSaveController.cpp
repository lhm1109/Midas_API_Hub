#include "StdAfx.h"

#include "../dgnengine/idesign/DGN_link/IDgnPerform.h"
#include "../dgnengine/idesign/DGN_def/DesignCaseDef.h"
#include "../dgnengine/idesign/DGN_link/IDgnResultController.h"

#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl.h"
#include "../wg_db/PostCtrl.h"
#include "../wg_db/UnitCtrl.h"

#include "IProductSaveObjectBuilder.h"
#include "ProductDBDocHelper.h"
#include "IProductDataBaseHelper.h"
#include "IProductResultSaver.h"
#include "TEngineResult.h"
#include "TSafeDeleteFunctor.h"
#include "DgnTypeConverter.h"
#include "IDgnPerformDataBase.h"
#include "IProductResultTracker.h"

#include "ProductResultSaveController.h"
#include "IProductDgnLcomSetter.h"
#include "ProductObjectFactoryCtrl.h"
#include <mutex>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::link;
using namespace dgn::def;
using namespace dgn::converter;

CProductResultSaveController::CProductResultSaveController(const CProductDBDocHelper* pDBDocHelper,
                                                           const IProductSaveObjectBuilder* pBuilder) :
    m_pDBDocHelper(pDBDocHelper),
    m_pBuilder(pBuilder),
    m_pTracker(pBuilder->CreateResultTracker(pDBDocHelper))
{
    if ( !m_pDBDocHelper || !m_pBuilder )
    {
        ASSERT(0);
    }

    m_mTypeDataBaseHelper.clear();
    m_mTypeSaver.clear();
}

CProductResultSaveController::~CProductResultSaveController()
{
    for ( auto& p : m_mTypeDataBaseHelper )
    {
        TSafeDeleteFunctor()(p.second);
    }

    for ( auto& p : m_mTypeSaver )
    {
        TSafeDeleteFunctor()(p.second);
    }

    TSafeDeleteFunctor()(m_pTracker);
}

bool dgn::converter::CProductResultSaveController::Initialize()
{
    ASSERT(m_mTypeDataBaseHelper.size() == 1); //이게 2개인 경우가 있나?
    for ( auto itrHelper : m_mTypeDataBaseHelper )
    {
        for ( auto itrSaver : m_mTypeSaver )
        {
            enResultDatabaseType ResultDBType = CResultDataBaseTypeUtil::GetResultDataBaseType(itrHelper.first.GetEDgnType(), itrSaver.first.GetEMembType());
            std::unique_ptr<const IProductDgnLcomSetter>
                pDgnLcomSetter(CProductObjectFactoryCtrl::CreateDgnLcomSetter(ResultDBType));

            if ( !pDgnLcomSetter->SetDgnLcomList(((IProductDataBaseHelper*)itrHelper.second)->GetPerformDataBase()) )
            {
                ASSERT(0); return false;
            }

            itrSaver.second->MakePDgnLcomKeyMap();
        }
    }

    return true;
}

bool dgn::converter::CProductResultSaveController::GetEngineResultRepInfo(const UINT& iMemb, dgn::link::TEngineResultInfo& rInfo) const
{
    auto* const pAttrCtrl = GetDBDocHelper()->GetAttrCtrl();
    const auto PMembType = pAttrCtrl->GetMemberType(iMemb);
    const auto EMembType = CDgnTypeConverter::GetMembTypeByElem(iMemb);
    //PSubType이 있는 경우 처리를 어떻게 해야되나..
    TProductMembType TMembType(PMembType, enProductSubType::None, EMembType);
    auto itr = m_mTypeSaver.find(TMembType);
    if ( itr == m_mTypeSaver.end() )
    {
        TEngineResultInfo Dummy;
        Dummy.Set(1U, iMemb, TEngineResultInfo::Direct);//slab... 아놔. 이게 최선인가..
        rInfo = Dummy;
        return true;
    }
    return itr->second->GetEngineResultRepInfo(rInfo);
}

bool dgn::converter::CProductResultSaveController::SetLcomResult(const UINT& iMemb, idgn::FIterator EDgnLcomItr, idgn::FIterator itrRefer, const dgn::item::enDgnResultItem& EItem, const std::vector<double>& crData)
{
	static std::mutex g_LcomResult;
	std::lock_guard<std::mutex> guard(g_LcomResult);

    if ( m_mTypeSaver.size() == 1 ) 
    {
        auto itr = m_mTypeSaver.begin();
        itr->second->DoInitialize(iMemb);
        return itr->second->SetLcomResult(EDgnLcomItr, itrRefer, EItem, crData);
    }
    else
    {
        for ( auto Saver : m_mTypeSaver )
        {
            if ( FITR_TO_DATATYPE(itrRefer) == CDataTypeUtil::GetDPoolType(dgn::def::DataType::PLevel2DForce) )
            {
                if ( Saver.first.GetPSubType() == dgn::converter::enProductSubType::Flexural )
                {
                    Saver.second->DoInitialize(iMemb);
                    return Saver.second->SetLcomResult(EDgnLcomItr, itrRefer, EItem, crData);
                }
            }
            else if ( FITR_TO_DATATYPE(itrRefer) == CDataTypeUtil::GetDPoolType(dgn::def::DataType::PLevelCutLine) )
            {
                if ( Saver.first.GetPSubType() == dgn::converter::enProductSubType::OneWayFlexural ||
                    Saver.first.GetPSubType() == dgn::converter::enProductSubType::OneWayShear )
                {
                    Saver.second->DoInitialize(iMemb);
                    return Saver.second->SetLcomResult(EDgnLcomItr, itrRefer, EItem, crData);
                }
            }
            else if ( FITR_TO_DATATYPE(itrRefer) == CDataTypeUtil::GetDPoolType(dgn::def::DataType::PLevelPoint) )
            {
                if ( Saver.first.GetPSubType() == dgn::converter::enProductSubType::PunchingShear )
                {
                    Saver.second->DoInitialize(iMemb);
                    return Saver.second->SetLcomResult(EDgnLcomItr, itrRefer, EItem, crData);
                }
            }
            else
            {
                ASSERT(0);
            }
        }
    }
    return false;
}

bool dgn::converter::CProductResultSaveController::SetLcomResult(const UINT& iMemb, const dgn::def::CDgnLesultBundle* pLesultBundle)
{
    static std::mutex g_LcomResult;
    std::lock_guard<std::mutex> guard(g_LcomResult);

    const auto* pDBDocHelper = GetDBDocHelper();
    if ( pDBDocHelper == nullptr )
    {
        ASSERT(0); return false;
    }
    const auto TDgnType = pDBDocHelper->GetTDgnType(iMemb);
    auto* pHelper = GetIDataBaseHelper(TDgnType);
    
    auto* const pAttrCtrl = GetDBDocHelper()->GetAttrCtrl();
    const auto PMembType = pAttrCtrl->GetMemberType(iMemb);
    const auto EMembType = CDgnTypeConverter::GetMembTypeByElem(iMemb);
    //PSubType이 있는 경우 처리를 어떻게 해야되나..
    TProductMembType TMembType(PMembType, enProductSubType::None, EMembType);
    auto itr = m_mTypeSaver.find(TMembType);
    return itr->second->SetLcomResult(iMemb, pLesultBundle, pHelper->GetPerformDataBase());
}

bool CProductResultSaveController::Play(T_KEY MembKey,
                                        IDgnPerformDataBase* pIDataBase) const
{
    CCurUnitSaver UnitRestoreObject(TRUE);

    if ( !Initialize(MembKey, pIDataBase) )
    {
        ASSERT(0); return false;
    }

    if ( !SaveData_WriteFile(MembKey, pIDataBase) )
    {
        ASSERT(0); return false;
    }

    return true;
}

bool CProductResultSaveController::MakeISaverList(T_KEY MembKey, IDgnPerformDataBase* pIDataBase) const
{
    typedef std::map<TProductMembType, IProductResultSaver*> mapTypeSaver;

    if ( !m_pBuilder )
    {
        ASSERT(0); return 0;
    }

    std::vector<TProductMembType> vTType;
    if ( !GetTMembTypeList(MembKey, pIDataBase, vTType) )
    {
        ASSERT(0); return false;
    }

    for ( const auto& TType : vTType )
    {
        if ( !HasISaver(TType) )
        {
            auto& InsertResult =
                const_cast<mapTypeSaver&>(m_mTypeSaver).insert(std::make_pair(TType, nullptr));
            if ( !InsertResult.second )
            {
                ASSERT(0); return 0;
            }

            const auto* pDBDocHelper = GetDBDocHelper();
            const auto* pIDataBaseHelper = GetIDataBaseHelper(MembKey, pIDataBase);

            auto* pSaver = m_pBuilder->CreateSaver(TType, pDBDocHelper, pIDataBaseHelper);
            InsertResult.first->second = pSaver;
        }
    }

    return true;
}

bool CProductResultSaveController::Initialize(T_KEY MembKey,
                                              IDgnPerformDataBase* pIDataBase) const
{
    const auto* pDBDocHelper = GetDBDocHelper();
    const auto* pIDataBaseHelper = GetIDataBaseHelper(MembKey, pIDataBase);
    
    pIDataBaseHelper->SetDgnLcomList(MembKey);

    if ( !pIDataBaseHelper->HasDgnLcomList() )
    {
        ASSERT(0); return false;
    }

    if ( !pDBDocHelper->CreateRebarData(MembKey) )
    {
        ASSERT(0); return false;
    }

    if ( !pDBDocHelper->ChangeCurUnitIdx(MembKey) )
    {
        ASSERT(0); return false;
    }

    return true;
}

bool CProductResultSaveController::SaveData_WriteFile(T_KEY MembKey,
                                                      IDgnPerformDataBase* pIDataBase) const
{
    std::vector<IProductResultSaver*> vpISaver;
    if ( !GetISaverList(MembKey, pIDataBase, vpISaver) )
    {
        ASSERT(0); return false;
    }

    auto* pTracker = GetTracker();

    Macro::Filter(vpISaver, 
        [&pTracker, &MembKey] (IProductResultSaver* pISaver)
    {
        //Filter가 false조건이면 지워짐. Skip조건이면 false.
        return !pTracker->IsSkipCondition(MembKey, pISaver);
    });

    for ( auto& pISaver : vpISaver )
    {
        if ( !pISaver->Initialize(MembKey) )
        {
            ASSERT(0); return false;
        }

        TEngineResultInfo EResultInfo;
        TEngineResult EResultAll;
        
        if ( !pISaver->GetEngineResultRepInfo(EResultInfo) )
        {
            ASSERT(0); return false;
        }

        if ( !GetEngineResult(EResultInfo, EResultAll) )
        {
            ASSERT(0); return false;
        }

        if ( EResultInfo.GetFlag() & TEngineResultInfo::Result )
        {
            if ( !pISaver->SaveDgnResult(EResultAll, pIDataBase) )
            {
                ASSERT(0); return false;
            }
        }

        if ( !pISaver->SetResultFlag() )
        {
            ASSERT(0); return false;
        }

        if ( !pISaver->WriteFileDgnResult(pIDataBase) )
        {
            ASSERT(0); return false;
        }

        if ( !pTracker->SetDone(MembKey, pISaver) )
        {
            ASSERT(0); return false;
        }
    }

    return true;
}

bool CProductResultSaveController::GetEngineResult(const dgn::link::TEngineResultInfo& Info,
                                                   TEngineResult& rData) const
{
    const auto& RepDcasKey = Info.GetRepDcasKey();
    const auto& RepMembKey = Info.GetRepMembKey();
    const auto& Flag       = Info.GetFlag();

    const auto* pDBDocHelper = GetDBDocHelper();
    auto* const pDgnPerform = pDBDocHelper->GetIDgnPerform();

    if ( Flag & TEngineResultInfo::Result )
    {
        rData.pEResultAll = pDgnPerform->GetDesignResult(RepDcasKey, RepMembKey);
        if ( rData.pEResultAll == nullptr ) 
        {
            ASSERT(0); return false;
        }
    }

//     if ( Flag & TEngineResultInfo::Lesult )
//     {
//         if ( !pDgnPerform->GetDesignLesult(RepDcasKey, RepMembKey, rData.pELesultAll) )
//         {
//             ASSERT(0); return false;
//         }
//     }

    return true;
}

int CProductResultSaveController::GetTMembTypeList(T_KEY MembKey, 
                                                   IDgnPerformDataBase* pIDataBase,
                                                   std::vector<TProductMembType>& rvType) const
{
    auto* const pAttrCtrl = GetDBDocHelper()->GetAttrCtrl();
    const auto* pIDataBaseHelper = GetIDataBaseHelper(MembKey, pIDataBase);

    const auto PMembType = pAttrCtrl->GetMemberType(MembKey);
    const auto EMembType = CDgnTypeConverter::GetMembTypeByElem(MembKey);

    rvType.clear();

    std::vector<enProductSubType> vPSubType;
    if ( !pIDataBaseHelper->GetPSubTypeList(std::make_pair(PMembType, EMembType), vPSubType) )
    {
        ASSERT(0); return 0;
    }

    for ( const auto& PSubType : vPSubType )
    {
        rvType.push_back(TProductMembType(PMembType, PSubType, EMembType));
    }

    return Macro::GetSize(rvType);
}

const IProductDataBaseHelper* CProductResultSaveController::GetIDataBaseHelper(T_KEY MembKey,
                                                                               IDgnPerformDataBase* pIDataBase) const
{
    typedef std::map<TProductDgnType, const IProductDataBaseHelper*> mapTypeHelper;

    if ( !m_pBuilder )
    {
        ASSERT(0); return nullptr;
    }

    const auto* pDBDocHelper = GetDBDocHelper();
    const auto TDgnType = pDBDocHelper->GetTDgnType(MembKey);

    if ( !HasIDataBaseHelper(TDgnType) )
    {
        auto& InsertResult =
            const_cast<mapTypeHelper&>(m_mTypeDataBaseHelper).insert(std::make_pair(TDgnType, nullptr));
        if ( !InsertResult.second )
        {
            ASSERT(0); return nullptr;
        }
        
        InsertResult.first->second = m_pBuilder->CreateDataBaseHelper(pIDataBase);
    }

    return m_mTypeDataBaseHelper.at(TDgnType);
}

IProductDataBaseHelper* dgn::converter::CProductResultSaveController::GetIDataBaseHelper(TProductDgnType DgnType) const
{
    const auto& itr = m_mTypeDataBaseHelper.find(DgnType);
    if (itr == m_mTypeDataBaseHelper.end())
        return nullptr;

    return (IProductDataBaseHelper*)itr->second;
}

int CProductResultSaveController::GetISaverList(T_KEY MembKey,
                                                IDgnPerformDataBase* pIDataBase,
                                                std::vector<IProductResultSaver*>& rvpISaver) const
{
    typedef std::map<TProductMembType, IProductResultSaver*> mapTypeSaver;

    if ( !m_pBuilder )
    {
        ASSERT(0); return 0;
    }

    std::vector<TProductMembType> vTType;
    if ( !GetTMembTypeList(MembKey, pIDataBase, vTType) )
    {
        ASSERT(0); return 0;
    }

    for ( const auto& TType : vTType )
    {
        if ( !HasISaver(TType) )
        {
            MakeISaverList(MembKey, pIDataBase);
        }
        else
        {
            const auto& itr = m_mTypeSaver.find(TType);
            rvpISaver.push_back(itr->second);
        }
    }

    return Macro::GetSize(rvpISaver);
}

IProductResultTracker* CProductResultSaveController::GetTracker() const
{
    return m_pTracker;
}

bool CProductResultSaveController::HasIDataBaseHelper(const TProductDgnType& TDgnType) const
{
    const auto& itr = m_mTypeDataBaseHelper.find(TDgnType);
    return (itr != m_mTypeDataBaseHelper.end());
}

bool CProductResultSaveController::HasISaver(const TProductMembType& TMembType) const
{
    const auto& itr = m_mTypeSaver.find(TMembType);
    return (itr != m_mTypeSaver.end());
}

const CProductDBDocHelper* CProductResultSaveController::GetDBDocHelper() const
{
    return m_pDBDocHelper;
}

const IProductSaveObjectBuilder* CProductResultSaveController::GetBuilder() const
{
    return m_pBuilder;
}