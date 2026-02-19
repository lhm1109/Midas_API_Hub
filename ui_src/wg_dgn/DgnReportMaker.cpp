#include "stdafx.h"

#include "DgnReportMaker.h"

#include "../wg_db/DBDoc.h"
#include "../wg_dbLock/LockOption.h"

#include "../dgnengine/idesign/DGN_def/DesignCaseDef.h"
#include "../dgnengine/idesign/DGN_link/ITmpAASHTOSteelLib.h"
#include "../dgnengine/idesign/DGN_def/DesignCaseDef.h"
#include "../dgnengine/idesign/DGN_def/LocaleDef.h"

#include "../dgnengine/idesign/DGN_link/IDgnDocCtrl.h"
#include "../dgnengine/idesign/DGN_link/IDgnReport.h"
#include "../dgnengine/idesign/DGN_link/IDgnDoc.h"
#include "../dgnengine/idesign/DGN_def/DesignCaseDef.h"
#include "../dgnengine/idesign/DGN_link/IDgnDocCtrl.h"
#include "../dgnengine/idesign/DGN_def/RptSettingsDef.h"
#include "../dgnengine/idesign/DGN_link/IDgnReport.h"
#include "../dgnengine/idesign/DGN_link/IDgnDoc.h"
#include "../dgnengine/src/DGN_Test/SimpleTestPerformerInfo.h"

#include "../wg_db/DBDoc.h"
#include "../wg_db/FileCtrl.h"
#include "../wg_dbLock/LockMgr.h"

#include "DgnInputConverter.h"
#include "IDgnDataBase.h"
#include "DgnReportMaker.h"
#include "DgnReportMaker.h"
#include "DgnCodeTool.h"
#include "DgnCodeCtrl.h"

using namespace dgn::def;
using namespace dgn;

CDgnReportMaker::CDgnReportMaker(IDgnDataBase* pDataBase) : IDgnReportMaker()
{
    m_pDataBase = pDataBase;
}

CDgnReportMaker::~CDgnReportMaker()
{
}

bool CDgnReportMaker::RunSummaryViewReport(const dgn::link::ItemInputList& ItemInput, CScrollVi* pView, CDC* pDC, const dgn::def::enReportType& RptType, const std::wstring& sRtplang, const std::wstring& strPath, const T_DGN_RPT_HEAD_INFO& HeadInfo)
{
    dgn::def::CDgnReport Report;
    auto* pReportType = new dgn::def::CReportType;
    pReportType->RptType = RptType;
    pReportType->pView = pView;
    pReportType->pDC = pDC;
    if ( RptType != dgn::def::enReportType::CDC )
    {
        pReportType->optionDC.bMargin      = TRUE;
        pReportType->optionDC.dMarginX    *= 1.5;
        pReportType->optionDC.dMarginYTop *= 1.2;
    }
    pReportType->strLang = sRtplang;
    pReportType->strPath = strPath;
    Report.AddData(pReportType);

    auto* pReportHeadInfo = new dgn::def::CReportHeadInfo;
    pReportHeadInfo->strTitle       = HeadInfo.sTitle;
    pReportHeadInfo->strProductVer  = HeadInfo.sProductVer;
    pReportHeadInfo->bUserInfor     = HeadInfo.bUserInfor;
    pReportHeadInfo->strCompanyName = HeadInfo.sCompanyName;
    pReportHeadInfo->strWebSite     = HeadInfo.sWebSite;
    pReportHeadInfo->strTelNo       = HeadInfo.sTelNo;
    pReportHeadInfo->strFaxNo       = HeadInfo.sFaxNo;
    Report.AddData(pReportHeadInfo);

    auto* pReportUnitType = new dgn::link::CReportUnitType;
    pReportUnitType->SetUseUser(true);
    pReportUnitType->SetCodeUnitType(m_pDataBase->GetDgnCode());
    pReportUnitType->SetUserUnitType(m_pDataBase->GetCurrentUnit());
    Report.AddData(pReportUnitType);

    return GenerateSummaryReport(ItemInput, Report);
}

bool CDgnReportMaker::RunDetailReport(const dgn::def::enReportType& RptType, const std::wstring& strPath,
    const dgn::link::MItemInputList& vItemInput,
    const dgn::link::MembInfoListLegacy&  vMembInfo)
{
    if ( !CDgnCodeTool::IsSupportCodeIDesignReport(m_pDataBase->GetDgnCode()) )
    {
        ASSERT(0); return false;
    }

    std::wstring F, L, UL;
    m_pDataBase->GetDgnPreferenceUnit(F, L, UL);

    dgn::def::CRptSettings Settings;
    Settings.SetDgnCode(m_pDataBase->GetDgnCode());
    Settings.SetDgnType(m_pDataBase->GetDgnType());
    Settings.SetFileOutPath(strPath);
	Settings.SetRptUnit(enUserUnit, F, L, UL);
    Settings.SetFileOutType(RptType);
    Settings.SetDesignCase(INTEGRATIONDESIGNCASE);

    if ( vItemInput.size() != vMembInfo.size() )
    {
        ASSERT(0); return false;
    }

    if ( !dgn::link::ITmpAASHTOSteelLib::GetInstance()->PrintDetailReport(Settings, vItemInput, vMembInfo) )
    {
        ASSERT(0);
        return false;
    }

    return true;
}

bool CDgnReportMaker::RunDetailReport()
{
    dgn::def::CRptSettings Settings;
	m_pDataBase->GetRptSettings(_T("Memb"), Settings);

    if ( !IsAvailable(Settings) )
    {
        return false;
    }

    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return false;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(pDoc);
    
    link::MItemInputList vMembItemInput;
    link::MembInfoList vMembInfo;

    //저 데이터들 줍줍해서 가져와야되. 
	m_pDataBase->InitializeRpt();
    m_pDataBase->GetReportInput(vMembItemInput, vMembInfo);

    if ( vMembItemInput.size() == 0 )
        return false;

	if ( !CLockMgr::Get_IsRegressionTest() )
	{
		if ( !dgn::link::IDgnDocCtrl::Instance()->OnChangeLocale(Settings.GetLocale()) )
		{
			ASSERT(0); return false;
		}
	}

	if ( !pDgnDoc->GetDgnReport()->PrintDetailReportTest(Settings, vMembItemInput, vMembInfo) )
	{
		ASSERT(0); return false;
	}

	return m_pDataBase->FinalizeRpt();
}

bool CDgnReportMaker::GenerateSummaryReport(const dgn::link::ItemInputList& ItemInput, const dgn::def::CDgnReport& Report)
{
    return dgn::link::ITmpAASHTOSteelLib::GetInstance()->PrintSummaryReport(m_pDataBase->GetDgnCode(), ItemInput, Report);
}

bool CDgnReportMaker::GetReportInfo(const T_DGN_RPT_DATA& RptData, dgn::def::CDgnInput* pInp, dgn::def::CDgnInput* pInpShrY, dgn::def::CDgnInput* pInpShrZ, dgn::link::MItemInputList& vItemInput, dgn::link::MembInfoListLegacy&  vMembInfo)
{
    dgn::converter::CDgnInputConverter Converter;

    dgn::def::CRptMembInfoLegacy MembInfo;
    MembInfo.RptPref = dgn::def::CRptMembInfoLegacy::MemberReport;

    MembInfo.aVal[dgn::def::CRptMembInfoLegacy::DesignCode] = m_pDataBase->GetDgnCodeName();
    MembInfo.aVal[dgn::def::CRptMembInfoLegacy::MemberName] = RptData.sElem;
    MembInfo.MembType = m_pDataBase->GetDgnMembType();
    if ( pInp->IsExist(dgn::def::DataType::Material) )
    {
        auto* pMatl = pInp->GetInput<dgn::def::CMaterial>();
        if ( pMatl == nullptr ) ASSERT(0);
        else MembInfo.aVal[dgn::def::CRptMembInfoLegacy::Material] = pMatl->strName;
    }
    if ( pInp->IsExist(dgn::def::DataType::Section) )
    {
        auto* pSect = pInp->GetInput<dgn::def::CSection>();
        if ( pSect == nullptr ) ASSERT(0);
        else MembInfo.aVal[dgn::def::CRptMembInfoLegacy::Section] = pSect->strSection;
    }

    dgn::link::ItemInputList aInput;
    std::wstring sLcom = RptData.sLcom;
    std::wstring sPosi = RptData.sPosi;
    std::wstring sLcomDesc = _T("");
    dgn::def::ReportCrit ReportCritD(sLcom, sLcomDesc, sPosi);

    aInput.push_back(std::make_pair(dgn::item::enDgnResultItem::COMBINE____P, std::make_pair(pInp, ReportCritD)));

    dgn::def::ReportCrit ShearYReportCritD(RptData.sLcomVy, sLcomDesc, RptData.sPosiVy);
    aInput.push_back(std::make_pair(dgn::item::enDgnResultItem::SHEAR_YF_P, std::make_pair(pInpShrY, ShearYReportCritD)));

    dgn::def::ReportCrit ShearZReportCritD(RptData.sLcomVz, sLcomDesc, RptData.sPosiVz);
    aInput.push_back(std::make_pair(dgn::item::enDgnResultItem::SHEAR_ZF_P, std::make_pair(pInpShrZ, ShearZReportCritD)));

    vItemInput.push_back(aInput);
    vMembInfo.push_back(MembInfo);

    return true;
}

bool CDgnReportMaker::GetReportInfo(const T_DGN_RPT_DATA& RptData, dgn::def::CDgnInput* pInp, dgn::def::CDgnInput* pInpShrY, dgn::def::CDgnInput* pInpShrZ, dgn::link::ItemInputList& aInput)
{
    //STEEL 만 사용
    dgn::converter::CDgnInputConverter Converter;

    std::wstring sLcom = RptData.sLcom;
    std::wstring sPosi = RptData.sPosi;
    std::wstring sLcomDesc = _T("");
    dgn::def::ReportCrit ReportCritD(sLcom, sLcomDesc, sPosi);
    aInput.push_back(std::make_pair(dgn::item::enDgnResultItem::COMBINE____P, std::make_pair(pInp, ReportCritD)));

    if ( !RptData.sLcomVy.empty() )
    {
        dgn::def::ReportCrit ShearReportCritDy(RptData.sLcomVy, sLcomDesc, RptData.sPosiVy);
        aInput.push_back(std::make_pair(dgn::item::enDgnResultItem::SHEAR_YF_P, std::make_pair(pInpShrY, ShearReportCritDy)));
    }

    if ( !RptData.sLcomVz.empty() )
    {
        dgn::def::ReportCrit ShearReportCritDz(RptData.sLcomVz, sLcomDesc, RptData.sPosiVz);
        aInput.push_back(std::make_pair(dgn::item::enDgnResultItem::SHEAR_ZF_P, std::make_pair(pInpShrZ, ShearReportCritDz)));
    }

    return true;
}

bool CDgnReportMaker::IsAvailable(const dgn::def::CRptSettings& Settings)
{
// #if defined (_US)
//     if ( CDgnCodeTool::IsEurocode4(Settings.GetDgnCode()) )
//     {
//         auto Language = CDgnCodeCtrl::GetCpgLocalePref().GetLanguage();
//         if ( Language == enLanguage::cze || Language == enLanguage::tur )
//         {
//             if ( !CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_AASHTO_STL_COMP) )
//             {
//                 AfxMessageBox(_LS(IDS_MAIN_NO_LICENSE_AASHTO_STL_COMP), MB_OK);
//                 return false;
//             }
//         }
//     }
// #endif
    return true;
}
