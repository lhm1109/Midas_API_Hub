#include "stdafx.h"
#include "IDgnCodeCheck.h"

#include "..\dgnengine\idesign\DGN_def\DgnInput.h"
#include "..\dgnengine\idesign\DGN_link\IDgnDocCtrl.h"
#include "..\dgnengine\idesign\DGN_link\IDgnDoc.h"
#include "..\dgnengine\idesign\DGN_link\IDgnCalc.h"
#include "..\dgnengine\idesign\DGN_link\IDgnQuery.h"
#include "..\dgnengine\idesign\DGN_link\IDgnPerform.h"
#include "..\dgnengine\idesign\DGN_link\CalcType.h"
#include "..\dgnengine\idesign\DGN_def\DgnCodeDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnTypeEnumDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnOutput.h"
#include "..\dgnengine\idesign\DGN_def\DgnResult.h"
#include "..\dgnengine\idesign\DGN_def\DataTypeDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnModeDef.h"
#include "..\dgnengine\idesign\DGN_def\Force1DDef.h"
#include "..\dgnengine\idesign\DGN_def\BindingDataDef.h"
#include "..\dgnengine\idesign\DGN_link\ITmpAASHTOSteelLib.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\MembCtrl.h"

#include "IDgnDataBase.h"
#include "DgnInputConverter.h"
#include "DgnTypeConverter.h"
#include "DgnPerformInputConverter.h"

using namespace dgn::converter;
using namespace dgn::def;
using namespace dgn::link;

IDgnCodeCheck::IDgnCodeCheck(IDgnDataBase* pDataBase)
{
    ASSERT(pDataBase);
    m_pDataBase = pDataBase;
}

IDgnCodeCheck::~IDgnCodeCheck()
{
}

IDgnDataBase* IDgnCodeCheck::GetDataBase()
{
    return m_pDataBase;
}

bool IDgnCodeCheck::Check_Section(bool bReport)
{
    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return false;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(CDBDoc::GetDocPoint());

    dgn::link::IDgnQuery* pDgnQuery = pDgnDoc->GetDgnQuery();

    dgn::def::enDgnCode enDgnCode = m_pDataBase->GetDgnCode();
    dgn::def::enDgn       enDgnType = m_pDataBase->GetDgnType();
    dgn::def::enMemb      enDgnMembType = m_pDataBase->GetDgnMembType();

    dgn::def::CDgnInput Input;
    GetDgnInput(Input, enDgnMode::Checking);

    dgn::def::CDgnOutput Output;
    dgn::def::CDgnResult Result;

    CCalcType Type(enDgnCode, enDgnType, enDgnMembType);

    if (!bReport)
    {   
        dgn::link::IDgnCalc* pDgnCalc = pDgnDoc->GetDgnCalc();
        CItemList ItemList;
        if ( pDgnCalc->GetPerformSectionItem(Type, ItemList) == 0 )
        {
            ASSERT(0); return false;
        }

        if ( !pDgnCalc->CheckSection(Type, ItemList, Input, Result) )
        {
            ASSERT(0); return false;
        }

        std::vector<dgn::item::enDgnResultItem> aResultItem;
        size_t size = Result.GetItemList(aResultItem);
        for (const auto& Item : aResultItem)
        {
            const dgn::def::IResult* pResult = Result.GetData(Item);
            GetDataBase()->SetResult(pResult);
        }
    }

    return true;
}

bool IDgnCodeCheck::GetDgnInput(dgn::def::CDgnInput& rInput, dgn::def::enDgnMode enMode)
{
    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return false;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(CDBDoc::GetDocPoint());

    dgn::link::IDgnQuery* pDgnQuery = pDgnDoc->GetDgnQuery();

    dgn::def::enDgnCode enDgnCode = m_pDataBase->GetDgnCode();
    dgn::def::enDgn       enDgnType = m_pDataBase->GetDgnType();
    dgn::def::enMemb      enDgnMembType = m_pDataBase->GetDgnMembType();

    // 결과 줄수 있는 Item은 머냐
    std::vector<dgn::item::enDgnResultItem> vResultItem;
    dgn::link::CCalcType CType(enDgnCode, enDgnType, enDgnMembType);
    if ( !pDgnQuery->GetOutputItem(CType, vResultItem) )
    {
        ASSERT(0); return false;
    }

    // 그 결과 줄라면 무슨 Input 넘겨야 하는데
    std::vector<dgn::def::DataType> vDataType;
    for (const auto& item : vResultItem)
    {
        std::vector<dgn::def::DataType> vTmpType;
        if ( !pDgnQuery->GetInputDataType(CType, enMode, item, vTmpType) )
        {
            ASSERT(0); return false;
        }

        for (const auto&type : vTmpType)
        {
            vDataType.push_back(type);
        }
    }
    sort(vDataType.begin(), vDataType.end());
    vDataType.erase(unique(vDataType.begin(), vDataType.end()), vDataType.end());

    // Input 채우는 짓
    dgn::converter::CDgnInputConverter Converter;
    Converter.AddInputMembType(enDgnType, enDgnMembType, rInput);
    for (const auto& Type : vDataType)
    {
        if (!Converter.AddInputData(Type, GetDataBase(), rInput))
        {
            ASSERT(0); return false;
        }
    }

    return true;
}