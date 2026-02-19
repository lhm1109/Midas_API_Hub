#include "stdafx.h"

#include <utility>
#include <thread>

#include "..\dgnengine\idesign\DGN_def\DgnInput.h"
#include "..\dgnengine\idesign\DGN_link\IDgnDocCtrl.h"
#include "..\dgnengine\idesign\DGN_link\IDgnDoc.h"
#include "..\dgnengine\idesign\DGN_link\IDgnCalc.h"
#include "..\dgnengine\idesign\DGN_link\IDgnQuery.h"
#include "..\dgnengine\idesign\DGN_link\IDgnPerform.h"
#include "..\dgnengine\idesign\DGN_link\CalcType.h"
#include "..\dgnengine\idesign\DGN_def\DgnModeDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnTypeEnumDef.h"
#include "..\dgnengine\idesign\DGN_def\RptSettingsDef.h"

#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\MembCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_base\ArrayUtil.h"

#include "IDgnPerformDataBase.h"
#include "DgnTypeConverter.h"
#include "DgnPerformInputConverter.h"
#include "DgnAnalRsltConverter.h"
#include "DgnAnalRsltCtrl.h"
#include "DgnProgressDlg.h"
#include "DgnAnalRsltTester.h"
#include "MembTypeUtil.h"
#include "TSafeDeleteFunctor.h"
#include "IEngineResultConvert.h"
#include "ProductObjectFactoryCtrl.h"
#include "ProductDBDocHelper.h"
#include "IProductDgnLcomSetter.h"

#include "DgnPerformRunner.h"
#include "DgnCodeTool.h"
#include "DgnCodeCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::lib;
using namespace dgn::def;
using namespace dgn::converter;

CDgnPerformRunner::CDgnPerformRunner(IDgnPerformDataBase* pIDataBase) :
    m_pIDataBase(pIDataBase),
    m_pDBDocHelper(new CProductDBDocHelper(CDBDoc::GetDocPoint())),
	m_bExistBackgroundThread(false)
{
}

CDgnPerformRunner::~CDgnPerformRunner()
{
    TSafeDeleteFunctor()(m_pDBDocHelper);
}

int CDgnPerformRunner::Run(const T_KEY_LIST& aElemK, const T_KEY_LIST* paAllElemK)
{    
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    const bool bReCalc = (paAllElemK == nullptr) ? false : (paAllElemK->GetSize()==0 ? false : true);

    T_KEY_LIST aKeyTarget;
    T_KEY_LIST aKeyExistResult;
    T_KEY_LIST aKeySelected;
    if ( !GetKeyLists(aElemK, bReCalc, aKeyTarget, aKeyExistResult, aKeySelected) )
    {
        ASSERT(0); return 0;
    }

    auto TType = PROGRESS_TYPE_DGNPERFORM_THREAD;
    auto Thread = std::thread(CDgnProgressDlg::ThreadProgress, &TType);
    auto* pSubject = CDgnProgObserver::Instance();
    pSubject->InitData();
    pSubject->SetTotal(2);

    auto l_ExpireProgressDlg = [&Thread, &pSubject]
    {
        if (pSubject) pSubject->SetStop(true);
        Thread.join();
    };

    auto* pIDataBase = GetIDataBase();

    if(bReCalc)
    {
        if(!SetDataPool(*paAllElemK))
        {
            l_ExpireProgressDlg();
            ASSERT(0);
            return 0;
        }
    }
    else
    {
		if(!SetDataPool(aKeySelected))
		{
			l_ExpireProgressDlg();
			ASSERT(0);
			return 0;
		}
    }

    pSubject->StepUpTotal();

    if ( aKeyTarget.IsEmpty() )
    {
        PlayInitDefault(bReCalc, aKeyExistResult, aKeySelected);
        l_ExpireProgressDlg();
        return 0;
    }

    //Result Converter를 미리 만들자
    const auto* pDBDocHelper = GetDBDocHelper();
    const auto& RepDgnTypePair = pDBDocHelper->GetResultDBType(aKeyTarget);
    IEngineResultConvert ConvertInterface(pDBDocHelper, RepDgnTypePair, bReCalc);
    if ( !ConvertInterface.Initialize(aKeyTarget, pIDataBase) )
    {
        l_ExpireProgressDlg();
        ASSERT(0); return 0;
    }
        
    if ( !PlayPerformThread(aKeyTarget) )
    {
        l_ExpireProgressDlg();
        return 0;
    }

    //이거 Lcom만들고 부르는데 또 불러줘야되나?
//     if (!SetDgnLcomList(aKeyTarget))
//     {
//         l_ExpireProgressDlg();
//         ASSERT(0); return 0;
//     }

	if(pSubject->IsStop())
	{
		l_ExpireProgressDlg();
		return 0;
	}

	GDisplayAnalString(_LS(IDS_DGN_TEXT_PERFORM_MSG_END_DGN));

	pSubject->SetCurrentSubject(_LS(IDS_DGN_TEXT_PERFORM_MSG_CONVERT_RESULT));
    pSubject->SetCurrentStep(0);
    
    WaitBackgroundThread();

	pDoc->SetModifiedFlag(FALSE); // Engine 중립DB가 바뀌면서 실제 모델 정보는 바뀐게 없는데 CDocument에 ModifyedFlag가 TRUE로 바뀜.
    const auto nPassCount =
        ConvertInterface.ConvertResults(pSubject, aKeyTarget, pIDataBase);

	GDisplayAnalString(_LS(IDS_DGN_TEXT_PERFORM_MSG_END_CONVERT_RESULT));

    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return nPassCount;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(pDoc);

    pDgnDoc->GetDgnPerform()->ClearResultDBPool();

    pSubject->StepUpTotal();

    if ( !bReCalc )
    {
        //TODO 중간에 STOP 눌렀을때 처리가 어뜨케 되는거지? STOP 누르면 결과 날려줘야되는것 같은디?
        pIDataBase->Count_UseElemPropNum(nPassCount + aKeyExistResult.GetSize());
    }
		
    pIDataBase->WriteSectResult(nPassCount > 0);

    l_ExpireProgressDlg();

    return nPassCount;
}

bool CDgnPerformRunner::RunPerform(const T_KEY_LIST& aElemK) const
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return false;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(pDoc);

    //중립단위계
    CCurUnitSaver Save(TRUE);
    T_UNIT_INDEX ChangeIndex;
    ChangeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
    ChangeIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
    pDoc->m_pUnitCtrl->SetUnitIndexCurrent(ChangeIndex);

	auto pCtrl = GetIDataBase()->GetAnalRsltCtrl();
	if(pCtrl == nullptr)
	{
		ASSERT(0); return false;
	}
	bool bDone = pDgnDoc->GetDgnPerform()->DoPerform(GetIDataBase()->GetDgnMode(), pCtrl);
	/*
    // set force
    T_KEY_LIST aForceElem;
    int nForceElemSize = GetIDataBase()->GetForceElemList(aElemK, aForceElem);
    auto pConverter = new CDgnAnalRsltConverter(pDoc, aForceElem, IsDirectProduct());
    CDgnAnalRsltCtrl Ctrl(pConverter, pDoc);
    if ( !Ctrl.IsValid() )
    {
        ASSERT(0); return false;
    }

    if ( !Ctrl.Initialize() )
    {
        ASSERT(0); return false;
    }

    bool bDone = pDgnDoc->GetDgnPerform()->DoPerform(GetIDataBase()->GetDgnMode(), &Ctrl);

    if ( !Ctrl.Finalize() )
    {
        ASSERT(0); return false;
    }
    pConverter->Finalize();
    _SAFE_DELETE(pConverter);
	*/
    return bDone;
}

bool CDgnPerformRunner::RunDetailReport(const T_KEY_LIST& aElemK, const CArray<int, int&>& aAllElemK, const dgn::def::CRptSettings& RptSettings)
{
    if ( !IsAvailableReport(RptSettings) )
    {
        return false;
    }

    T_KEY_LIST aKeyList;
    for (int i = 0; i < aAllElemK.GetSize(); ++i)
    {
        aKeyList.Add(static_cast<T_KEY>(aAllElemK.GetAt(i)));
    }
    return RunDetailReport(aElemK, aKeyList, RptSettings);
}

bool CDgnPerformRunner::RunDetailReport(const T_KEY_LIST& aElemK, const T_KEY_LIST& aAllElemK, const dgn::def::CRptSettings& RptSettings)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    //중립단위계
    CCurUnitSaver Save(TRUE);
    T_UNIT_INDEX ChangeIndex;
    ChangeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
    ChangeIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
    pDoc->m_pUnitCtrl->SetUnitIndexCurrent(ChangeIndex);

    pDoc->m_pMatlDB->Create_RebarData(D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_FORCE_INDEX_N, true);

    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return false;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(pDoc);

    dgn::link::IDgnQuery* pDgnQuery = pDgnDoc->GetDgnQuery();

    T_KEY_LIST  aMembK;
    for ( int i = 0; i < aElemK.GetSize(); ++i )
    {
        T_ELEM_K ShowKey;
        if ( !CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey) )
        {
            ASSERT(0); continue;
        }
        aMembK.Add(ShowKey);
    }

    T_KEY_LIST  aAllMembK;
    for (int i = 0; i < aAllElemK.GetSize(); ++i)
    {
        T_ELEM_K ShowKey;
        if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aAllElemK[i], ShowKey))
        {
            ASSERT(0); continue;
        }
        aAllMembK.Add(ShowKey);
    }

    CArrayUtil::RemoveDup(aMembK);
    CArrayUtil::RemoveDup(aAllMembK);
    std::vector<dgn::link::CCalcType> vMemb;

    for ( int i = 0; i < aMembK.GetSize(); ++i )
    {
        dgn::link::CCalcType Type(CDgnTypeConverter::GetDgnCodeByElem(aMembK[i]),
            CDgnTypeConverter::GetDgnType(aMembK[i]), CDgnTypeConverter::GetMembType(pDoc->m_pAttrCtrl->GetMemberType(aMembK[i])));

        vMemb.push_back(Type);
    }

    Macro::UniqueSort(vMemb);

    std::vector<dgn::def::DataType> vDataType;
    for ( auto Memb : vMemb )
    {
        // 결과 줄수 있는 Item은 머냐
        std::vector<dgn::item::enDgnResultItem> vResultItem;
        if ( !pDgnQuery->GetOutputItem(Memb, vResultItem) )
        {
            ASSERT(0); return false;
        }

        std::vector<dgn::def::DataType> vTmpType;
        pDgnDoc->GetDgnQuery()->GetInputPerformDataType(Memb, dgn::def::enDgnMode::Reporting, vResultItem, vTmpType);
        for ( const auto&type : vTmpType )
        {
            vDataType.push_back(type);
        }
    }

    Macro::UniqueSort(vDataType);

    CDgnPerformInputConverter Conv(GetIDataBase());
	std::vector<long> vMembK = Macro::CArray2Vec<UINT, long>(aAllMembK);
	std::vector<long> vCriticalMembK = Macro::CArray2Vec<UINT, long>(aMembK);

    if(GetIDataBase()->IsRcsSlab() || GetIDataBase()->IsRcsMeshWall())
    {
        if(!Conv.AddRptCase(vCriticalMembK))
        {
            ASSERT(0); return false;
        }
		if(!Conv.AddRptCriticalInfo(vCriticalMembK))
		{
			ASSERT(0); return false;
		}
    }
    else
    {
        pDgnDoc->GetDgnPerform()->ClearDataPool();

        for(auto DataType : vDataType)
        {
            if(!Conv.AddData(DataType, vMembK, vCriticalMembK))
            {
                continue;
            }
        }
    }

	auto pCtrl = GetIDataBase()->GetAnalRsltCtrl();
	if(pCtrl == nullptr)
	{
		ASSERT(0); return false;
	}
	pDgnDoc->GetDgnPerform()->DoGenerateReport(pCtrl, RptSettings);

	/*
    // set force
    T_KEY_LIST aForceKeyElem;
    GetIDataBase()->GetForceElemList(aElemK, aForceKeyElem);
    auto pConverter = new CDgnAnalRsltConverter(pDoc, aForceKeyElem, IsDirectProduct());
    CDgnAnalRsltCtrl Ctrl(pConverter, pDoc);
    if ( !Ctrl.IsValid() )
    {
        ASSERT(0); return false;
    }

    if ( !Ctrl.Initialize() )
    {
        ASSERT(0); return false;
    }

    pDgnDoc->GetDgnPerform()->DoGenerateReport(&Ctrl, RptSettings);

    if ( !Ctrl.Finalize() )
    {
        ASSERT(0); return false;
    }
    pConverter->Finalize();
    _SAFE_DELETE(pConverter);
	*/
    pDoc->m_pMatlDB->Create_RebarData(GetIDataBase()->GetCodeUnitLength(), GetIDataBase()->GetCodeUnitForce(), true);

    return true;
}

bool CDgnPerformRunner::SetDataPool(const T_KEY_LIST& aElemK)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    auto* pSubject = CDgnProgObserver::Instance();
    pSubject->SetCurrentSubject(_LS(IDS_DGN_TEXT_PERFORM_MSG_PREPARE_DGN));

    //중립단위계
    CCurUnitSaver Save(TRUE);
    T_UNIT_INDEX ChangeIndex;
    ChangeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
    ChangeIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
    pDoc->m_pUnitCtrl->SetUnitIndexCurrent(ChangeIndex);

    pDoc->m_pMatlDB->Create_RebarData(D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_FORCE_INDEX_N, true);

    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return false;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(pDoc);

    dgn::link::IDgnQuery* pDgnQuery = pDgnDoc->GetDgnQuery();

    T_KEY_LIST  aMembK;
    for (int i = 0; i < aElemK.GetSize(); ++i)
    {
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD))
		{
			ASSERT(0); continue;
		}

		T_ELEM_K ShowKey = aElemK[i];
        if ( ElemD.eltyp != WALL_EL )
        {
            if ( !CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey) )
            {
                ASSERT(0); continue;
            }
            T_HCBM_K HcbmK(0);
            if ( pDoc->m_pAttrCtrl->GetHcbmAssigned(ShowKey, HcbmK) )
            {
                T_HCBM_D HcbmD;
                if ( pDoc->m_pAttrCtrl->GetHcbm(HcbmK, HcbmD) )
                {
                    for ( int j = 0; j < 3; ++j )  aMembK.Add(HcbmD.aElemListPart[j].GetAt(0));
                    continue;
                }
            }
        }
        aMembK.Add(ShowKey);
    }

    CArrayUtil::RemoveDup(aMembK);
    std::vector<dgn::link::CCalcType> vMemb;

    for (int i = 0; i < aMembK.GetSize(); ++i)
    {
		auto enCode = CDgnTypeConverter::GetDgnCodeByElem(aMembK[i]);
		if ( enCode == enDgnCode::CODE_NULL )
			continue;

		auto enDgnType = CDgnTypeConverter::GetDgnType(aMembK[i]);
		if ( enDgnType == enDgn::None )
			continue;
        
		auto enMembType = CDgnTypeConverter::GetMembType(pDoc->m_pAttrCtrl->GetMemberType(aMembK[i]));
		if ( enMembType == enMemb::None )
			continue;

		dgn::link::CCalcType Type(enCode, enDgnType, enMembType);
        vMemb.push_back(Type);
    }

    Macro::UniqueSort(vMemb);

    std::vector<dgn::def::DataType> vDataType;
    for (auto Memb : vMemb)
    {
        // 결과 줄수 있는 Item은 머냐
        std::vector<dgn::item::enDgnResultItem> vResultItem;
        if (!pDgnQuery->GetOutputItem(Memb, vResultItem))
        {
            pDoc->m_pMatlDB->Create_RebarData(GetIDataBase()->GetCodeUnitLength(), GetIDataBase()->GetCodeUnitForce(), true);
            ASSERT(0); return false;
        }

        std::vector<dgn::def::DataType> vTmpType;
        pDgnDoc->GetDgnQuery()->GetInputPerformDataType(Memb, GetIDataBase()->GetDgnMode(), vResultItem, vTmpType);
        for (const auto&type : vTmpType)
        {
            vDataType.push_back(type);
        }
    }

    Macro::UniqueSort(vDataType);

    const auto nDataType = vDataType.size();
    pSubject->SetCurrent(nDataType, nDataType);

    auto vMembK = Macro::CArray2Vec<UINT, long>(aMembK);
    pDgnDoc->GetDgnPerform()->ClearDataPool();
    CDgnPerformInputConverter Conv(GetIDataBase());
    int nDone = 0;
    for (auto DataType : vDataType)
    {
        if (!Conv.AddData(DataType, vMembK, vMembK))
        {
            pSubject->SetCurrentStep(++nDone);
            continue;
        }
        pSubject->SetCurrentStep(++nDone);
    }

    pDoc->m_pMatlDB->Create_RebarData(GetIDataBase()->GetCodeUnitLength(), GetIDataBase()->GetCodeUnitForce(), true);

	GDisplayAnalString(_LS(IDS_DGN_TEXT_PERFORM_MSG_END_PREPARE_DGN));
	
    return true;
}

bool CDgnPerformRunner::GetKeyLists(const T_KEY_LIST& aElemK,
                                    bool bReCalc,
                                    T_KEY_LIST& raKeyTarget, 
                                    T_KEY_LIST& raKeyExistResult,
                                    T_KEY_LIST& raKeySelected) const
{
    typedef IDgnPerformDataBase::enResultType RType;

    if ( aElemK.IsEmpty() )
    {
        ASSERT(0); return false;
    }

    raKeyTarget.RemoveAll();
    raKeyExistResult.RemoveAll();
    raKeySelected.RemoveAll();

    auto* const pIDataBase = GetIDataBase();
    const auto nSizeElemK = aElemK.GetSize();
    for ( int nIdx = 0; nIdx < nSizeElemK; ++nIdx )
    {
        const auto& ElemKey = aElemK[nIdx];
        if ( pIDataBase->IsRunMember(ElemKey) )
        {
			if ( m_pIDataBase->GetDgnType() != CDgnTypeConverter::GetDgnType(ElemKey) )
				continue;

            const auto enResultType = pIDataBase->GetResultType(ElemKey);
            if ( bReCalc || enResultType == RType::None )
            {
                raKeyTarget.Add(ElemKey);
            }
            else if ( enResultType == RType::DGN && pIDataBase->IsDesigning() )
            {
                raKeyExistResult.Add(ElemKey);
            }
            else if ( enResultType == RType::CHK && !pIDataBase->IsDesigning() )
            {
                raKeyExistResult.Add(ElemKey);
            }
            else
            {
                raKeyTarget.Add(ElemKey);
            }

            raKeySelected.Add(ElemKey);
        }
    }

    // check warning
    pIDataBase->CheckDgnFlag(raKeySelected);    

    return true;
}

void CDgnPerformRunner::PlayInitDefault(bool bReCalc, 
                                       const T_KEY_LIST& caKeyExistResult,
                                       const T_KEY_LIST& caKeySelected)
{
    const auto* pDBDocHelper = GetDBDocHelper();
    auto* pIDataBase = GetIDataBase();

    //이걸 불러줘야 설계결과 대화상자에 결과가 표시됨. 
    //설계부재, 단면 Counting Update.
    if ( !bReCalc )
    {
        pIDataBase->Count_UseElemPropNum(caKeyExistResult.GetSize());
    }

    pIDataBase->WriteSectResult(caKeyExistResult.GetSize() > 0);

    //Make PL, Lcom ...
    if ( !pDBDocHelper->GetIDgnPerform()->DoInitPerform() )
    {
        ASSERT(0); 
        return;
    }

    if ( !SetDgnLcomList(caKeySelected) )
    {
        ASSERT(0);
    }
}

bool CDgnPerformRunner::PlayPerformThread(const T_KEY_LIST& caKeyTarget) const
{
    return RunPerform(caKeyTarget);
}

bool CDgnPerformRunner::SetDgnLcomList(const T_KEY_LIST& caKey)
{
    const auto* pDBDocHelper = GetDBDocHelper();
    const auto& ResultDBType = pDBDocHelper->GetResultDBType(caKey);

    std::unique_ptr<const IProductDgnLcomSetter>
        pDgnLcomSetter(CProductObjectFactoryCtrl::CreateDgnLcomSetter(ResultDBType));

    auto* pIDataBase = GetIDataBase();
    if ( !pDgnLcomSetter->SetDgnLcomList(pIDataBase) )
    {
        ASSERT(0); return false;
    }

    return true;
}

IDgnPerformDataBase* CDgnPerformRunner::GetIDataBase() const
{
    return m_pIDataBase;
}

const CProductDBDocHelper* CDgnPerformRunner::GetDBDocHelper() const
{
    return m_pDBDocHelper;
}

void CDgnPerformRunner::FillReportCompanySetting(dgn::def::CRptSettings& Settings)
{
	const std::wstring strPN        = CT2W(CDBLib::GetProductNameVer());
    const std::wstring strName      = CT2W(D_COMPANY_FULL_NAME);
    const std::wstring strNameShort;// = CT2W(D_COMPANY_ABB_NAME);
    const std::wstring strAdd;// = CT2W(D_MIDAS_ADDRESS);
    const std::wstring strPostcode;// = CT2W(_T("\n"));
    const std::wstring strURL;// = CT2W(D_MIDAS_URL);
    CString str;
    str.Format(_T("Tel. : %s"), D_MIDAS_PHONE);
    const std::wstring strTel;// = CT2W(str);
    str.Format(_T("Fax. : %s"), D_MIDAS_FAX);
    const std::wstring strFax;// = CT2W(str);

	const auto DgnMode = [&]()
	{
		if (GetIDataBase()->IsDesigning())
		{
#ifdef _CIVIL
			return enDgnMode::AsDesign;
#else
			return enDgnMode::RebarDesign;
#endif
		}
		else
		{
			return enDgnMode::Checking;
		}
	}();

    Settings.SetUserPref(true);
    Settings.SetProductName(strPN);
    Settings.SetCompanyInfof(strName, strNameShort, strAdd, strPostcode, strURL, strTel, strFax);
	Settings.SetDgnMode(DgnMode);
}

// Get해서 Handle을 가져온다음에 제품에서 Handle의 값을 바꿔줄꺼야.
HANDLE* CDgnPerformRunner::GetPtrBackgroundThread()
{
    return &m_hBackgroundThread;
}

void CDgnPerformRunner::SetBackGroundThread(bool bFlag)
{
	m_bExistBackgroundThread = bFlag;
}

void CDgnPerformRunner::WaitBackgroundThread()
{
    if ( m_bExistBackgroundThread && m_hBackgroundThread )
    {
		int iWait = 0;
		CString strMsg = _LS(IDS_DGN_TEXT_PERFORM_MSG_CREATE_RES_FILE);
		GDisplayAnalString(strMsg);
        DWORD dwExitCode = 0;
        ::GetExitCodeThread(m_hBackgroundThread, &dwExitCode);
        if ( dwExitCode == STILL_ACTIVE )
        {
            while ( TRUE )
            {
                ::GetExitCodeThread(m_hBackgroundThread, &dwExitCode);
                if ( dwExitCode != STILL_ACTIVE )
                {
                    break;
                }

				iWait++;
				
				const int iIdx = iWait%8;
				switch (iIdx)
				{
				case 0:	GDisplayAnalStringSamePos(strMsg); break;
				case 2:	GDisplayAnalStringSamePos(strMsg+_T(".")); break;
				case 4:	GDisplayAnalStringSamePos(strMsg+_T("..")); break;
				case 6:	GDisplayAnalStringSamePos(strMsg+_T("...")); break;
				default:
					break;
				}

				::Sleep(100);
            }
        }
		GDisplayAnalStringSamePos(strMsg+_T("..."));
		GDisplayAnalString(_LS(IDS_DGN_TEXT_PERFORM_MSG_END_RES_FILE));
    }
}

bool CDgnPerformRunner::IsDirectProduct() const
{
    CString csDirectProduct = CProduct::GetTestEnvValue(_ULS(DirectProduct));
    return csDirectProduct.CompareNoCase(_ULS(no)) == 0 ? false : true;
}

bool CDgnPerformRunner::IsAvailableReport(const dgn::def::CRptSettings& Settings) const
{
#if defined (_US)
    if ( CDgnCodeTool::IsEurocode2(Settings.GetDgnCode()) )
    {
        auto enLanguage = CDgnCodeCtrl::GetConLocalePref().GetLanguage();
        switch ( enLanguage )
        {
            case enLanguage::pol:
                {
                    if ( !CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_RUS_RCPSC_DGN) )
                    {
                        AfxMessageBox(_LS(IDS_MAIN_NO_LICENSE_RUS_RCPSC_DGN), MB_OK);
                        return false;
                    }
                }
                break;
            case enLanguage::cze:
            case enLanguage::tur:
                {   
                    if ( !CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_AASHTO_STL_COMP) )//아닌것 같지만 이게 맞다....
                    {
                        AfxMessageBox(_LS(IDS_MAIN_NO_LICENSE_AASHTO_STL_COMP), MB_OK);
                        return false;
                    }
                }
                break;
            default:
                break;
        }
    }
#endif
    return true;
}
