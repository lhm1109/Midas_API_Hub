// DgnConCodeNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewDlg.h"
#include "DconChildDialog.h"

#include "DgnConCodeNewChild_1_Dlg.h"
#include "DgnConCodeNewChild_2_Dlg.h"
#include "DgnConCodeNewChild_3_Dlg.h"
#include "DgnConCodeNewChild_4_Dlg.h"
#include "DgnConCodeNewChild_5_Dlg.h"
#include "DgnConCodeNewChild_6_Dlg.h"
#include "DgnConCodeNewChild_7_Dlg.h"
#include "DgnConCodeNewChild_8_Dlg.h"
#include "DgnConCodeNewChild_9_Dlg.h"
#include "DgnConCodeNewChild_10_Dlg.h"
#include "DgnConCodeNewChild_11_Dlg.h"
#include "DgnConCodeNewChild_12_Dlg.h"
#include "DgnConCodeNewChild_13_Dlg.h"
#include "DgnConCodeNewChild_14_Dlg.h"
#include "DgnConCodeNewChild_15_Dlg.h"
#include "DgnConCodeNewChild_16_Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewDlg dialog


CDgnConCodeNewDlg::CDgnConCodeNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConCodeNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConCodeNewDlg)
	m_aCodeEnable.RemoveAll();

	m_mapChildDlg.RemoveAll();
	m_mapChildDlg.InitHashTable(37);

	m_aCtrlMethod.Add(IDOK);
	m_aCtrlMethod.Add(IDCANCEL);
	
}

CDgnConCodeNewDlg::~CDgnConCodeNewDlg()
{
	CDconChildDialog* dlg;
	int key;

	if(m_mapChildDlg.Lookup(m_nCurrentDlgIndex, dlg))
		if(dlg->IsCreated())   dlg->End();

	POSITION pos = m_mapChildDlg.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapChildDlg.GetNextAssoc(pos,key,dlg);
		delete dlg;
	}
	m_mapChildDlg.RemoveAll();
}


void CDgnConCodeNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeNewDlg)
	DDX_Control(pDX, IDC_DGN_CTRLDATA_DESIGNCODE, m_DesignCode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnConCodeNewDlg)
	ON_CBN_SELCHANGE(IDC_DGN_CTRLDATA_DESIGNCODE, OnSelchangeDgnCtrldataDesigncode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewDlg message handlers

BOOL CDgnConCodeNewDlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	CDialogMove::OnInitDialog();

	BOOL bExistData=TRUE;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetDcon(m_Data))
	{
		bExistData=FALSE;
		m_Data.Initialize();
	}  

	m_mapChildDlg.SetAt(EN_CHILD_KCI_ACI_1, new CDgnConCodeNewChild1Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_KCI_ACI_2, new CDgnConCodeNewChild2Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_ACI_3    , new CDgnConCodeNewChild3Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_NSR10_4  , new CDgnConCodeNewChild4Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_TWN_USD_5, new CDgnConCodeNewChild5Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_IS456_6  , new CDgnConCodeNewChild6Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_AIJ_WSD_7, new CDgnConCodeNewChild7Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_GB50010_8, new CDgnConCodeNewChild8Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_EURO204_9, new CDgnConCodeNewChild9Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_ACI318_M_10, new CDgnConCodeNewChild10Dlg(this, &m_Data, bExistData));
    m_mapChildDlg.SetAt(EN_CHILD_TWN_USD_112, new CDgnConCodeNewChild11Dlg(this, &m_Data, bExistData));
    m_mapChildDlg.SetAt(EN_CHILD_ACI318_12, new CDgnConCodeNewChild12Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_KDS2022_13, new CDgnConCodeNewChild13Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_NTC_DCEC_2017, new CDgnConCodeNewChild14Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_SP_63_2018, new CDgnConCodeNewChild15Dlg(this, &m_Data, bExistData));
	m_mapChildDlg.SetAt(EN_CHILD_NTC_DCEC_2023, new CDgnConCodeNewChild16Dlg(this, &m_Data, bExistData));
	InitCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewDlg::InitCtrl()
{
	m_DesignCode.ResetContent();

	CStringArray ConCodeName;
	CDBLib::GetConCodeNameList(ConCodeName, &m_aCodeEnable);
	
	// Change, Jaeoh('07.12.26)
	int iCodeSize = ConCodeName.GetSize();
	for(int i=0; i<iCodeSize; i++)	
	{
		//(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
#if defined(_CIVIL)
		CString strConCodeName = CDBLib::ViewConvertCodeName(ConCodeName[i]);
#elif defined(_MGEN)
		CString strConCodeName = ConCodeName[i];
#endif
		if (strConCodeName != _T("GB50068-2018"))
		{
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(strConCodeName);
#endif
			m_DesignCode.AddString(strConCodeName);
		}
	}

	CDBDoc *pDoc = CDBDoc::GetDocPoint();

	CString strDesignCode;
	if(pDoc->m_pAttrCtrl->ExistDcon()) strDesignCode = CDBLib::ViewConvertCodeName(m_Data.DesignCode);
	else
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		pDoc->m_pInitCtrl->GetPreference(rPref);
		//(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
		
		
		strDesignCode = CDBLib::ViewConvertCodeName(rPref.DgnCode.ConcCode);
		m_Data.DesignCode = strDesignCode;
		m_Data.nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
		if(strDesignCode == CONCODE_NTC_DCEC_2023) m_Data.nSCWBDgnMethod = 1; // nominal
	}

	CString CodeName;
	CodeName= strDesignCode;
#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(CodeName);
#endif
	int Index=0;
	if(strDesignCode!=_T("")) Index = m_DesignCode.FindStringExact(-1, CodeName);
	if(Index!=-1)	m_DesignCode.SetCurSel(Index);

	m_DesignCode.GetWindowText(strDesignCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strDesignCode);
#endif
	m_nCurrentDlgIndex = GetChildDlgIndex(strDesignCode);

	CDconChildDialog *ChildDlg=NULL;
	if(!m_mapChildDlg.Lookup(m_nCurrentDlgIndex, ChildDlg)) { ASSERT(0); return; }

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_CODE_PLACEHOLDER);
	ChildDlg->CreateInit(pPlaceHolder);  
//   ChildDlg->SetDesignCode(strDesignCode);

	ResizeDialog();
	ChildDlg->ShowWindow(SW_SHOW);
}

BOOL CDgnConCodeNewDlg::Dlg2Data()
{
	CDconChildDialog *ChildDlg=NULL;
	if(!m_mapChildDlg.Lookup(m_nCurrentDlgIndex, ChildDlg)) { ASSERT(0); return FALSE; }

	if(ChildDlg==NULL) return FALSE;
	return ChildDlg->Dlg2Data();
}

void CDgnConCodeNewDlg::OnOK() 
{
	if(!Dlg2Data()) return;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pDataCtrl->AddDcon(m_Data)) 
	{ 
		//ASSERT(0); 
		return;
	}
	pDoc->m_pDataCtrl->AddDconLog(m_Data);

	CDialogMove::OnOK();
}

void CDgnConCodeNewDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}
												
void CDgnConCodeNewDlg::OnSelchangeDgnCtrldataDesigncode() 
{
	CString strDesignCode;
	m_DesignCode.GetWindowText(strDesignCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strDesignCode);
#endif
	int nChildDlgIndex = GetChildDlgIndex(strDesignCode);
	strDesignCode = CDBLib::ViewConvertCodeName(strDesignCode);

	CDconChildDialog *ChildDlg=NULL;
	if(!m_mapChildDlg.Lookup(nChildDlgIndex, ChildDlg)) { ASSERT(0); return; }
	
	CDconChildDialog *ChildDlgOld=NULL;
	if(!m_mapChildDlg.Lookup(m_nCurrentDlgIndex, ChildDlgOld)) { ASSERT(0); return; }
	ChildDlgOld->Dlg2Data();

	if(m_nCurrentDlgIndex != nChildDlgIndex)
	{
		ChildDlgOld->ShowWindow(SW_HIDE);
		m_Data.DesignCode = strDesignCode;
		CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_CODE_PLACEHOLDER);
		ChildDlg->CreateInit(pPlaceHolder);  
		ChildDlg->UpdateChildData();

		m_nCurrentDlgIndex = nChildDlgIndex;
		ChildDlg->ShowWindow(SW_SHOW);
	}
	else m_Data.DesignCode = strDesignCode;
	
	ChildDlg->ChangeDesignCode();
	//ChildDlg->SetDesignCode(strDesignCode);
//   ChildDlg->CalcDlgSize();

	
}

int CDgnConCodeNewDlg::GetChildDlgIndex(CString& strDesignCode)
{
    if ( strDesignCode == CONCODE_KDS_41_20_2022 ) { return EN_CHILD_KDS2022_13; }
    else if ( strDesignCode == CONCODE_KDS_41_30_2018 ) { return EN_CHILD_KCI_ACI_1; }
    else if ( strDesignCode == CONCODE_KCI_USD12 )  { return EN_CHILD_KCI_ACI_1; }
    else if ( strDesignCode == CONCODE_KCI_USD07 )  { return EN_CHILD_KCI_ACI_1; }
    else if ( strDesignCode == CONCODE_KCI_USD03 )  { return EN_CHILD_KCI_ACI_2; }
    else if ( strDesignCode == CONCODE_KCI_USD99 )  { return EN_CHILD_KCI_ACI_2; }
    else if ( strDesignCode == CONCODE_KSCE_USD96 ) { return EN_CHILD_KCI_ACI_2; }
    else if ( strDesignCode == CONCODE_AIK_USD94 )  { return EN_CHILD_KCI_ACI_2; }
    else if ( strDesignCode == CONCODE_AIK_WSD2K )  { return EN_CHILD_KCI_ACI_2; }
    else if ( strDesignCode == CONCODE_ACI318_14 )  { return EN_CHILD_ACI318_M_10; }
    else if ( strDesignCode == CONCODE_ACI318M_14)  { return EN_CHILD_ACI318_M_10; }
    else if ( strDesignCode == CONCODE_ACI318_19 )  { return EN_CHILD_ACI318_12; }
    else if ( strDesignCode == CONCODE_ACI318M_19)  { return EN_CHILD_ACI318_12; }
	else if ( strDesignCode == CONCODE_ACI318_25       ) { return EN_CHILD_ACI318_12;     }
    else if ( strDesignCode == CONCODE_ACI318M_25      ) { return EN_CHILD_ACI318_12;     }
    else if ( strDesignCode == CONCODE_NSCP_2015 )  { return EN_CHILD_ACI318_M_10; }
	else if ( strDesignCode == CONCODE_NTC_DCEC_2017 ) { return EN_CHILD_NTC_DCEC_2017; }
	else if ( strDesignCode == CONCODE_NTC_DCEC_2023 ) { return EN_CHILD_NTC_DCEC_2023; }
    else if ( strDesignCode == CONCODE_ACI318_11 )  { return EN_CHILD_KCI_ACI_1; }
    else if ( strDesignCode == CONCODE_ACI318_08 )  { return EN_CHILD_KCI_ACI_1; }
    else if ( strDesignCode == CONCODE_ACI318_05 )  { return EN_CHILD_ACI_3; }
    else if ( strDesignCode == CONCODE_ACI318_02 )  { return EN_CHILD_ACI_3; }
    else if ( strDesignCode == CONCODE_ACI318_99 )  { return EN_CHILD_ACI_3; }
    else if ( strDesignCode == CONCODE_ACI318_95 )  { return EN_CHILD_ACI_3; }
    else if ( strDesignCode == CONCODE_ACI318_89 )  { return EN_CHILD_ACI_3; }
    else if ( strDesignCode == CONCODE_GB50010_19 ) { return EN_CHILD_GB50010_8; }
    else if ( strDesignCode == CONCODE_GB50010_10 ) { return EN_CHILD_GB50010_8; }
    else if ( strDesignCode == CONCODE_GB50010_02 ) { return EN_CHILD_GB50010_8; }
    else if ( strDesignCode == CONCODE_BS8110_97 )  { return EN_CHILD_KCI_ACI_2; }
    else if ( strDesignCode == CONCODE_EC2_04 )     { return EN_CHILD_EURO204_9; }
    else if ( strDesignCode == CONCODE_EC2 )        { return EN_CHILD_KCI_ACI_2; }
    else if ( strDesignCode == CONCODE_NSR_10 )     { return EN_CHILD_NSR10_4; }
    else if ( strDesignCode == CONCODE_CSA_A23_3_94 ) { return EN_CHILD_KCI_ACI_2; }
    else if ( strDesignCode == CONCODE_AIJ_WSD99 )    { return EN_CHILD_AIJ_WSD_7; }
    else if ( strDesignCode == CONCODE_IS456_2000 )   { return EN_CHILD_IS456_6; }
    else if ( strDesignCode == CONCODE_TWN_USD112 )   { return EN_CHILD_TWN_USD_112; }
    else if ( strDesignCode == CONCODE_TWN_USD100 )   { return EN_CHILD_TWN_USD_5; }
    else if ( strDesignCode == CONCODE_TWN_USD92 )    { return EN_CHILD_TWN_USD_5; }
	else if (strDesignCode == CONCODE_SP_63_13330_2018) { return EN_CHILD_SP_63_2018; }
    ASSERT(0);
	return -1;
}

void CDgnConCodeNewDlg::ResizeDialog(CDconChildDialog* ChildDlg)
{
	if(ChildDlg==nullptr) return;

	CRect rRef, rMove, rChild, rDlg;

	ChildDlg->GetWindowRect(rChild);

	CWnd* pWnd = GetDlgItem(IDC_DGN_CODE_PLACEHOLDER);
	if(pWnd==nullptr) return;

	pWnd->GetWindowRect(rMove);
	rMove.bottom = rMove.bottom - (rMove.Height() - rChild.Height());
// 	rMove.right = rMove.right - (rMove.Width() - rChild.Width()); 
	ScreenToClient(rMove);
	pWnd->MoveWindow(rMove);

	this->GetWindowRect(rRef);
	rRef.bottom -= (rMove.Height() - rChild.Height());

	SetWindowPos(NULL, rRef.left, rRef.top, rRef.Width()/*+20*/, rRef.Height(), SWP_NOMOVE);

	// IDOK, ID_CANCEL
	GetDlgItem(IDOK)->GetWindowRect(rMove);
	GetDlgItem(IDC_DGN_CODE_PLACEHOLDER)->GetWindowRect(rRef);
 
	//int nDistX = rRef.right - rMove.right;
	int nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(16);
	//CDlgUtil::CtrlMoveDistX(this, m_aCtrlMethod, nDistX);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlMethod, nDistY);

	GetDlgItem(IDOK)->GetWindowRect(rRef);
	GetWindowRect(rMove);

	rMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(rMove, TRUE);

	//ChildDlg->ShowWindow(SW_SHOW);
}

BOOL CDgnConCodeNewDlg::GetDataFromDlg(T_DCON_D& rData)
{
    m_Data = rData;

    if ( !Dlg2Data() )
    {
        ASSERT(0); return FALSE;
    }

    rData = m_Data;

    return TRUE;
}