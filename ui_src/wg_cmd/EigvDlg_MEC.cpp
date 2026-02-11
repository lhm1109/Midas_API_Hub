// EigvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EigvDlg_MEC.h"
#include "CMGroupDampingElementDlg.h"
#include "EigvComplexDlg.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\MIT_lib\MProduct.h"
using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL __bIsNotOnInitialUpdate_MEC = TRUE;
/////////////////////////////////////////////////////////////////////////////
// CEigvDlg_MEC dialog


CEigvDlg_MEC::CEigvDlg_MEC(CWnd* pParent /*=NULL*/)
	: CDialogMove(CEigvDlg_MEC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEigvDlg_MEC)
	m_bNLLinkVect = FALSE;
	m_nEigenRitz = 0;
// 	m_nSubLanc = 0;
	m_bSTRUM=0;
	m_bUseMinMax=0;
	//}}AFX_DATA_INIT
	m_nSTLC = 0; // static load case 갯수 
	m_nAnalType = 1;
// 	m_nSubComp = 0;
	m_bComp = FALSE;
}


void CEigvDlg_MEC::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEigvDlg_MEC)
	DDX_Control(pDX, IDC_NUMBER_RITZ_EDIT, m_wndTotalNumOfRitz2);
	DDX_Control(pDX, IDC_TOTAL_NUMBER_RITZ_EDIT, m_wndTotalNumOfRitz);
	DDX_Control(pDX, IDC_NL_LINK_FVECTOR_EDIT, m_wndNLFVector);
	DDX_Control(pDX, IDC_NL_LINK_VNUMBER_EDIT, m_wndENLForceVectors);
	DDX_Control(pDX, IDC_CMD_NUM_OF_GEN_EDIT, m_wndNOGSpin);
	DDX_Control(pDX, IDC_LC_LIST, m_wndLCList);
	DDX_Control(pDX, IDC_CMD_LC_COMBO, m_wndLCCombo);
	DDX_Control(pDX, IDC_EIGV_FSHIFT_UNIT, m_wndFreqShiftUnit);
	DDX_Control(pDX, IDC_EIGV_UPF_UNIT, m_wndUpperFreqUnit);
	DDX_Control(pDX, IDC_EIGV_LOF_UNIT, m_wndLowerFreqUnit);
	DDX_Control(pDX, IDC_EIGV_NOF_EDIT, m_wndNOFSpin);
	DDX_Control(pDX, IDC_EIGV_NOI_EDIT, m_wndNOISpin);
	DDX_Control(pDX, IDC_EIGV_SUBDIM_EDIT, m_wndSubDimSpin);
	DDX_Text(pDX, IDC_EIGV_LOF_EDIT, m_dblLowerFreq);
	DDX_Text(pDX, IDC_EIGV_UPF_EDIT, m_dblUpperFreq);
	DDX_Text(pDX, IDC_EIGV_CONVTOL_EDIT, m_dblConvTol);
	//DDX_Text(pDX, IDC_EIGV_FSHIFT_EDIT, m_dblFreqShift);
	DDX_Check(pDX, IDC_INC_NL_LINK_FVECTOR_CHECK, m_bNLLinkVect);
	DDX_Check(pDX, IDC_EIGV_FRI_CHK, m_bUseMinMax);
	DDX_Check(pDX, IDC_EIGV_SSC_CHK, m_bSTRUM);
	DDX_Radio(pDX, IDC_CMD_EIGEN_RDO, m_nEigenRitz);
// 	DDX_Radio(pDX, IDC_ATYPE_EIGEN_RADIO, m_nSubLanc);
// 	DDX_Radio(pDX, IDC_COMP_MASS_RADIO, m_nSubComp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEigvDlg_MEC, CDialogMove)
	//{{AFX_MSG_MAP(CEigvDlg_MEC)
	ON_BN_CLICKED(IDC_EIGV_REMOVE_BTN, OnEigvRemoveBtn)
	ON_BN_CLICKED(IDC_LC_ADD_BUTTON, OnLcAddButton)
	ON_BN_CLICKED(IDC_LC_DELETE_BUTTON, OnLcDeleteButton)
	ON_BN_CLICKED(IDC_LC_MODIFY_BUTTON, OnLcModifyButton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LC_LIST, OnItemchangedLcList)
	ON_NOTIFY(NM_CLICK, IDC_LC_LIST, OnClickLcList)
	ON_BN_CLICKED(IDC_INC_NL_LINK_FVECTOR_CHECK, OnIncNlLinkFvectorCheck)
	ON_BN_CLICKED(IDC_EIGV_FRI_CHK,OnUseFRICheck)
	ON_EN_CHANGE(IDC_NL_LINK_VNUMBER_EDIT, OnChangeNlLinkVnumberEdit)
	ON_BN_CLICKED(IDC_ATYPE_RITZ_RADIO, OnAnaltypeRadio)
	ON_BN_CLICKED(IDC_CMD_EIGEN_RDO, OnAnaltypeRadio)
// 	ON_BN_CLICKED(IDC_ATYPE_EIGEN_RADIO, OnAnaltypeRadio)
// 	ON_BN_CLICKED(IDC_ATYPE_LANC_RADIO, OnAnaltypeRadio)
// 	ON_BN_CLICKED(IDC_ATYPE_COMP_RADIO, OnAnaltypeRadio)
// 	ON_BN_CLICKED(IDC_COMP_MASS_RADIO, OnEIGVComplexRadio)
// 	ON_BN_CLICKED(IDC_COMP_ELEM_RADIO, OnEIGVComplexRadio)
	ON_BN_CLICKED(IDC_COMP_BUTTON, OnEigvComplexBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEigvDlg_MEC message handlers
BOOL CEigvDlg_MEC::OnInitDialog() 
{
	__bIsNotOnInitialUpdate_MEC = FALSE;

	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	m_EigvData.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetEigv(m_EigvData))
		GetDlgItem(IDC_EIGV_REMOVE_BTN)->EnableWindow(FALSE);
	else
		m_bComp = TRUE;

	m_wndNOGSpin.SetRange(1, SHRT_MAX);
	m_wndNOGSpin.SetInteger(TRUE);
	m_wndENLForceVectors.SetRange(1, SHRT_MAX);
	m_wndENLForceVectors.SetInteger(TRUE);
	m_wndNOFSpin.SetRange(0, 1000);
	m_wndNOFSpin.SetInteger(TRUE);
	m_wndNOISpin.SetRange(0, 1000);
	m_wndNOISpin.SetInteger(TRUE);
	m_wndSubDimSpin.SetRange(0, 1000);
	m_wndSubDimSpin.SetInteger(TRUE);
	m_wndFreqShiftUnit.SetUnitType(CUnitCtrl::m_EIGV_UNIT.FShift);
	m_wndUpperFreqUnit.SetUnitType(CUnitCtrl::m_EIGV_UNIT.FrMax);
	m_wndLowerFreqUnit.SetUnitType(CUnitCtrl::m_EIGV_UNIT.FrMin);

	MakeLCCombo();
	m_wndLCCombo.SetCurSel(0);

	Eigv2DlgVar();

	//m_wndNLFVector.SetWindowText(_T("1"));
	if (m_EigvData.nType == 0) m_wndENLForceVectors.SetWindowText(_T("1"));
	m_wndNOGSpin.SetWindowText(_T("1"));

	MakeCaseListHeader();
	MakeCaseList();
	
	//CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStldKeyList(
	
	UpdateData(FALSE);
 
	CDlgUtil::GetCtrlIDByIncRect(this,m_EigvCtrls,IDC_CMD_EIGEN_TITLE_STATIC, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_RitzCtrls,IDC_CMD_RITZ_TITLE_STATIC , TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_EigvSub,IDC_CMD_EIGV_CONTROL_PROP , TRUE);
	

	CtrlMan();
	
	__bIsNotOnInitialUpdate_MEC = TRUE;

// 	CArray<UINT, UINT> CompCtrls;	
// 	CompCtrls.Add(IDC_CMD_STATIC10);
// 	CompCtrls.Add(IDC_ATYPE_COMP_RADIO);
// 	CompCtrls.Add(IDC_COMP_MASS_RADIO);
// 	CompCtrls.Add(IDC_COMP_ELEM_RADIO);
// 	CompCtrls.Add(IDC_COMP_BUTTON);
// 
// 	if (CProduct::GetTestEnvValue(_T("UseComplex")) == _T("yes"))
// 	{
// 		CDlgUtil::CtrlShowHide(this, CompCtrls, TRUE);
// 	}
// 	else
// 	{
// 		CDlgUtil::CtrlShowHide(this, CompCtrls, FALSE);
// 	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEigvDlg_MEC::MakeLCCombo()
{
	//(CDBDoc::GetDocPoint())->m_pAttrCtrl->
	this->m_wndLCCombo.ResetContent();
	T_STLD_D StldD;
	CArray<T_STLD_K, T_STLD_K> StldKeys;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStldKeyList(StldKeys);

	// MNET:XXXX 20070724 mylee - 정부장님 요청
	// MNET:XXXX 20080610 mylee - 버그가 있어 다시 고침
	for(int j=StldKeys.GetSize()-1 ; j>=0 ; --j)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStld(StldKeys[j],StldD))
		{
			if(StldD.LoadCaseType == _T("CS") || StldD.LoadCaseType == _T("TCS"))
				StldKeys.RemoveAt(j);      
		}
	}

	int nStld = StldKeys.GetSize();
	for(int i = 0; i< nStld; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStld(StldKeys[i],StldD))
		{
			m_wndLCCombo.AddString(StldD.LoadCaseName);
			m_wndLCCombo.SetItemData(i,(DWORD)StldKeys[i]);
		}
	}

	m_nSTLC = nStld;

	m_wndLCCombo.AddString(_LS(IDS_CMD_0529_Ground_Acc_X));  
	m_wndLCCombo.SetItemData(m_nSTLC  ,1);
	m_wndLCCombo.AddString(_LS(IDS_CMD_0529_Ground_Acc_Y));
	m_wndLCCombo.SetItemData(m_nSTLC+1,2);
	m_wndLCCombo.AddString(_LS(IDS_CMD_0529_Ground_Acc_Z));
	m_wndLCCombo.SetItemData(m_nSTLC+2,3);
}

void CEigvDlg_MEC::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE)) return;
	DlgVar2Eigv();

// 	if (m_nSubLanc == 2)
// 	{
// 		if (m_nSubComp == 0)
// 		{
// 			if (!m_bComp)
// 			{
// 				if (AfxMessageBox(_LS(IDS_CMD_EIGV_COMPLEX_MASS_TIP), MB_OKCANCEL) == IDOK)
// 					OpenComplexMassDlg();
// 				else
// 					return;
// 			}
// 		}
// 		else
// 		{
// 			T_GRDP_D GrdpD;	GrdpD.Initialize();
// 			if (!m_pDoc->m_pAttrCtrl->GetGrdp(GrdpD))
// 			{
// 				if (AfxMessageBox(_LS(IDS_CMD_EIGV_COMPLEX_ELEM_TIP), MB_OKCANCEL) == IDOK)
// 					OpenComplexElemDlg();
// 				else
// 					return;
// 			}
// 		}
// 	}
	
	if (!m_pDoc->m_pDataCtrl->AddEigv(m_EigvData)) return;

	if (m_EigvData.nType == 3 || m_EigvData.nType == 4)
	{
		T_STLD_K SplcK;
		T_SPLC_D SplcD;
		T_KEY_LIST lstSplc;
		m_pDoc->m_pAttrCtrl->GetSplcKeyList(lstSplc);
		for (int i = 0; i < lstSplc.GetCount(); i++)
		{
			SplcD.Initialize();
			SplcK = lstSplc[i];
			if (!m_pDoc->m_pAttrCtrl->GetSplc(SplcK, SplcD)) { ASSERT(0); continue; }
			SplcD.bAutoSearchAngle = FALSE;
			SplcD.nAutoType = 0;
			if (!m_pDoc->m_pDataCtrl->ModifySplc(SplcD.LoadCaseName, SplcD)) { ASSERT(0); continue; }
		}
	}
	
	CDialogMove::OnOK();
}

void CEigvDlg_MEC::Eigv2DlgVar()
{
	m_wndNOFSpin.SetValue(m_EigvData.nNumMode);
	m_dblLowerFreq = m_EigvData.FrMin;
	m_dblUpperFreq = m_EigvData.FrMax;
	m_wndNOISpin.SetValue(m_EigvData.nNumItr);
	m_wndSubDimSpin.SetValue(m_EigvData.nNPA);
	m_dblConvTol   = m_EigvData.SimTol;
	//m_dblFreqShift = m_EigvData.FShift;
	m_bSTRUM       = m_EigvData.Ritz.bSTRUM;

	m_nAnalType = m_EigvData.nType;
	if(m_nAnalType==0)
	{
		m_nEigenRitz=0;
// 		m_nSubLanc=0;
	}
	else if(m_nAnalType == 1)
	{
		m_nEigenRitz=1;
	}
	else if(m_nAnalType == 2)
	{
		m_nEigenRitz=0;
// 		m_nSubLanc=1;
	}
	else if (m_nAnalType == 3)
	{
		m_nEigenRitz = 0;
// 		m_nSubLanc = 2;
// 		if (m_EigvData.bElement)
// 			m_nSubComp = 1;
// 		else
// 			m_nSubComp = 0;
// 		ChangeComplexBtnText(m_nSubComp);
		
	}

	CString StrTemp;
	StrTemp.Format(_T("%d"),m_EigvData.Ritz.nGenerationNum);
	m_wndENLForceVectors.SetWindowText(StrTemp);
	this->m_bNLLinkVect = m_EigvData.Ritz.bIncludeForceVectors;
	//StrTemp.Format(_T("%g"),m_EigvData.Ritz.nForceVectorsNum);
	this->m_wndNLFVector.SetWindowText(StrTemp);
	//StrTemp.Format(_T("%d"),m_EigvData.Ritz.nTotalNum);
	this->m_wndTotalNumOfRitz.SetWindowText(StrTemp);
	
	/*
	StrTemp.Format(_T("%g"),m_EigvData.Ritz.dDependency);
	this->m_wndDepOfRitz.SetWindowText(StrTemp);
	StrTemp.Format(_T("%g"),m_EigvData.Ritz.dJacobiIteration);
	this->m_wndJacobiIter.SetWindowText(StrTemp);
	*/
	this->m_bUseMinMax = m_EigvData.bMinMax;
	UpdateData(FALSE);
}

int CEigvDlg_MEC::FindEndLCIndex()
{
	int nEndLC = 0;
	for(int i = 0; i < D_EIGV_NUMLC ; i++)
	{
		if(m_EigvData.Ritz.LC[i].LoadCaseKey == 0) break;
		nEndLC++;
	}
	return nEndLC;
}

void CEigvDlg_MEC::DlgVar2Eigv()
{
	UpdateData(TRUE);
	m_EigvData.nNumMode  = m_wndNOFSpin.GetEditValueInt();
	m_EigvData.FrMin     = m_dblLowerFreq;
	m_EigvData.FrMax     = m_dblUpperFreq;
	m_EigvData.nNumItr   = m_wndNOISpin.GetEditValueInt();
	m_EigvData.nNPA      = m_wndSubDimSpin.GetEditValueInt();
	m_EigvData.SimTol    = m_dblConvTol;
	//m_EigvData.FShift    = m_dblFreqShift;
	m_EigvData.Ritz.bSTRUM           = m_bSTRUM;

	if(m_nEigenRitz==0)
	{
		m_nAnalType = 2;
// 		if(m_nSubLanc==0) m_nAnalType = 0;
// 		else if (m_nSubLanc == 2)
// 		{
// 			m_nAnalType = 3;
// 			if (m_nSubComp == 0)	m_EigvData.bElement = FALSE;
// 			else  m_EigvData.bElement = TRUE;
// 		}
// 		else   m_nAnalType = 2;
	}
	else m_nAnalType = 1;

	m_EigvData.nType = m_nAnalType;
	//////////////////////////////////////////////////////////////////////////
	// Ritz Load Case Data는 기설정된 상태...
	CString StrTemp;
	
	//this->m_wnd
	m_wndENLForceVectors.GetWindowText(StrTemp);
	if(!CStrParser::GetINumber(StrTemp,m_EigvData.Ritz.nGenerationNum))
		m_EigvData.Ritz.nGenerationNum = 0;
	
	m_EigvData.Ritz.bIncludeForceVectors = m_bNLLinkVect ;
	m_EigvData.bMinMax = m_bUseMinMax ;
	

	
//  this->m_wndNLFVector.GetWindowText(StrTemp);
//  if(!CStrParser::GetINumber(StrTemp,m_EigvData.Ritz.nForceVectorsNum))
//    m_EigvData.Ritz.nForceVectorsNum = 0;
	
//  this->m_wndTotalNumOfRitz.GetWindowText(StrTemp);
//  if(!CStrParser::GetINumber(StrTemp,m_EigvData.Ritz.nTotalNum))
//    m_EigvData.Ritz.nTotalNum = 0;
	/*
	this->m_wndDepOfRitz.GetWindowText(StrTemp);
	if(!CStrParser::GetFloatNumber(StrTemp,m_EigvData.Ritz.dDependency))
		m_EigvData.Ritz.dDependency = 0;
	
	this->m_wndJacobiIter.GetWindowText(StrTemp);
	if(!CStrParser::GetFloatNumber(StrTemp,m_EigvData.Ritz.dJacobiIteration))
		m_EigvData.Ritz.dJacobiIteration = 0;
		*/
}

void CEigvDlg_MEC::OnEigvRemoveBtn() 
{
	// TODO: Add your control notification handler code here
	if (m_pDoc->m_pDataCtrl->DelEigv())
	{
		// *^^* Remove하면 Dialog 닫는다.
		/*
		m_EigvData.Initialize();
		Eigv2DlgVar();
		GetDlgItem(IDC_EIGV_REMOVE_BTN)->EnableWindow(FALSE);
		UpdateData(FALSE);
		*/
		CDialogMove::OnOK();
	}
}

void CEigvDlg_MEC::CtrlMan()
{
	//IDOK,IDCANCEL,IDC_EIGV_REMOVE_BTN

	UpdateData();

	CWnd* pEIwnd = GetDlgItem(IDC_CMD_EIGEN_TITLE_STATIC);
	CWnd* pRTwnd = GetDlgItem(IDC_CMD_RITZ_TITLE_STATIC);
	CWnd* pRRwnd = GetDlgItem(IDC_EIGV_REMOVE_BTN);
	CWnd* pRBwnd = GetDlgItem(IDOK);
	CRect EIRect,RTRect,DlgRect,BtnRect, RRRect;

	GetWindowRect(DlgRect);
	pEIwnd->GetWindowRect(EIRect);
	pRTwnd->GetWindowRect(RTRect);
	pRRwnd->GetWindowRect(RRRect);
	pRBwnd->GetWindowRect(BtnRect);
	int DY;

	CArray<UINT,UINT> Buttons;
	Buttons.Add(IDOK);
	Buttons.Add(IDCANCEL);
	Buttons.Add(IDC_EIGV_REMOVE_BTN);

	CArray<UINT,UINT> arSTRUM;
	arSTRUM.Add(IDC_EIGV_SSC_CHK);

	CArray<UINT,UINT> aSpin1, aSpin2;
	aSpin1.Add(IDC_EIGV_NOF_EDIT);
	aSpin2.Add(IDC_EIGV_NOI_EDIT);
	aSpin2.Add(IDC_EIGV_SUBDIM_EDIT);

	CRect NofRect, NoiRect, NofStcRect, ConRect;
	GetDlgItem(IDC_EIGV_NOF_EDIT)->GetWindowRect(NofRect);
	GetDlgItem(IDC_EIGV_NOI_EDIT)->GetWindowRect(NoiRect);
	GetDlgItem(IDC_CMD_STATIC13)->GetWindowRect(NofStcRect);
	GetDlgItem(IDC_EIGV_CONVTOL_EDIT)->GetWindowRect(ConRect);
	//CDlgUtil::CtrlMoveDistXY(this,aSpin1,DlgRect.right-NofRect.left,0,TRUE);
	CDlgUtil::CtrlMoveDistXY(this,aSpin2,DlgRect.right-NoiRect.left,0,TRUE);

	GetDlgItem(IDC_EIGV_NOF_EDIT)->GetWindowRect(NofRect);
	GetDlgItem(IDC_EIGV_NOI_EDIT)->GetWindowRect(NoiRect);

	if(m_nEigenRitz == 1)
	{
		int RDX = EIRect.left - RTRect.left;
		int RDY = EIRect.top  - RTRect.top ;
		
		CDlgUtil::CtrlShowHide  (this,this->m_EigvCtrls,FALSE );
		CDlgUtil::CtrlShowHide  (this,this->m_RitzCtrls,TRUE  );
		CDlgUtil::CtrlShowHide  (this,arSTRUM,FALSE );
		CDlgUtil::CtrlMoveDistXY(this,m_RitzCtrls,RDX,RDY,TRUE);
	 
		DisplayRitzVectorNumber();

		pRTwnd->GetWindowRect(RTRect);
		DY = RTRect.bottom + globalUtils.ScaleByDPI(8) - RRRect.top;
		
// 		CtrlComplex(FALSE);
// 		CDlgUtil::CtrlEnableDisable(this, IDC_ATYPE_COMP_RADIO, FALSE);
	}
	else if(m_nEigenRitz == 0)
	{
		pEIwnd->GetWindowRect(EIRect);
		DY = EIRect.bottom + globalUtils.ScaleByDPI(8) - RRRect.top;
		CArray<UINT,UINT> arTempCtrls;
		arTempCtrls.Add(IDC_EIGV_FRI_CHK);
		arTempCtrls.Add(IDC_EIGV_LOF_EDIT);
		arTempCtrls.Add(IDC_EIGV_UPF_EDIT);
		CDlgUtil::CtrlShowHide(this,this->m_EigvCtrls,TRUE);
		CDlgUtil::CtrlShowHide(this,this->m_RitzCtrls,FALSE);
// 		if(m_nSubLanc==0)
// 		{
// 			CDlgUtil::CtrlEnableDisable(this,arTempCtrls,FALSE);
// 			CDlgUtil::CtrlShowHide  (this,arSTRUM,FALSE );
// 			CDlgUtil::CtrlShowHide  (this,m_EigvSub,TRUE);
// 			//CDlgUtil::CtrlMoveDistXY(this,aSpin1,NofStcRect.right + globalUtils.ScaleByDPI(2) - NofRect.left,0,TRUE);
// 			CDlgUtil::CtrlMoveDistXY(this,aSpin2,ConRect.left-NoiRect.left,0,TRUE);
// // 			CtrlComplex(FALSE);
// // 			CDlgUtil::CtrlEnableDisable(this, IDC_ATYPE_COMP_RADIO, TRUE);
// 		}
// 		else if(m_nSubLanc==1)
// 		{
// 			CDlgUtil::CtrlEnableDisable(this,arTempCtrls,TRUE);
// 			OnUseFRICheck();
// 			CRect ETRect,RIRect;
// 			CWnd* pETwnd = GetDlgItem(IDC_EIGV_SSC_CHK);
// 			CWnd* pRIwnd = GetDlgItem(IDC_CMD_EIGV_NUM_ITER );
// 			pETwnd->GetWindowRect(ETRect);
// 			pRIwnd->GetWindowRect(RIRect);
// 			int RDX =  RIRect.left-ETRect.left;
// 			int RDY = 0 ;
// 			CDlgUtil::CtrlShowHide  (this,m_EigvSub,FALSE);
// 			CDlgUtil::CtrlShowHide  (this,arSTRUM,TRUE );
// 			CDlgUtil::CtrlMoveDistXY(this,arSTRUM,RDX,RDY,TRUE);
// 			//CDlgUtil::CtrlMoveDistXY(this,aSpin1,NofStcRect.right + globalUtils.ScaleByDPI(2) - NofRect.left,0,TRUE);
// // 			CtrlComplex(FALSE);
// // 			CDlgUtil::CtrlEnableDisable(this, IDC_ATYPE_COMP_RADIO, TRUE);
// 		}
// 		else if (m_nSubLanc == 2)
// 		{
			CDlgUtil::CtrlEnableDisable(this, arTempCtrls, TRUE);
			OnUseFRICheck();
			CRect ETRect, RIRect;
			CWnd* pETwnd = GetDlgItem(IDC_EIGV_SSC_CHK);
			CWnd* pRIwnd = GetDlgItem(IDC_CMD_EIGV_NUM_ITER);
			pETwnd->GetWindowRect(ETRect);
			pRIwnd->GetWindowRect(RIRect);
			int RDX = RIRect.left - ETRect.left;
			int RDY = RIRect.top - ETRect.top;
			CDlgUtil::CtrlShowHide(this, m_EigvSub, FALSE);
			CDlgUtil::CtrlShowHide(this, arSTRUM, TRUE);
			CDlgUtil::CtrlMoveDistXY(this, arSTRUM, RDX, RDY, TRUE);
			//CDlgUtil::CtrlMoveDistXY(this, aSpin1, NofStcRect.right + globalUtils.ScaleByDPI(2) - NofRect.left, 0, TRUE);
// 			CtrlComplex(TRUE);
// 			CDlgUtil::CtrlEnableDisable(this, IDC_ATYPE_COMP_RADIO, TRUE);
// 		}
// 		else ASSERT(0);

	}
	else ASSERT(0);

	CDlgUtil::CtrlMoveDistY(this,Buttons,DY,TRUE);

	UpdateData(FALSE);

// 	GetDlgItem(IDC_ATYPE_EIGEN_RADIO)->EnableWindow(m_nEigenRitz==0);
// 	GetDlgItem(IDC_ATYPE_LANC_RADIO)->EnableWindow(m_nEigenRitz==0);

	pRBwnd->GetWindowRect(BtnRect);

	DlgRect.bottom = BtnRect.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(DlgRect.left,DlgRect.top,DlgRect.Width(),DlgRect.Height(),TRUE);

	//////////////////////////////////////////////////////////////////////////
// 	CArray<UINT, UINT> aCtrlComplex;
// 	aCtrlComplex.Add(IDC_ATYPE_COMP_RADIO);
// 	aCtrlComplex.Add(IDC_COMP_MASS_RADIO);
// 	aCtrlComplex.Add(IDC_COMP_ELEM_RADIO);
// 	aCtrlComplex.Add(IDC_COMP_BUTTON);
// 
// 	BOOL bHideComblex = TRUE;
// 	if (CProduct::GetTestEnvValue(_T("UseComplex")) == _T("yes")) bHideComblex = FALSE;
// #ifdef _MGEN_CH
// 	bHideComblex = FALSE;
// #endif
// 	if (bHideComblex) CDlgUtil::CtrlShowHide(this, aCtrlComplex, FALSE);

	NLLVCtrlMan();
}

void CEigvDlg_MEC::NLLVCtrlMan()
{
	if(m_nEigenRitz == 1)
	{
		CArray<UINT, UINT> Ctrls;
		Ctrls.Add(IDC_NL_LINK_VNUMBER_EDIT);
		CDlgUtil::CtrlEnableDisable(this,Ctrls,m_bNLLinkVect);
	}
}

void CEigvDlg_MEC::OnAnaltypeRadio() 
{
	UpdateData(TRUE);
	CtrlMan();
}

void CEigvDlg_MEC::MakeCaseListHeader()
{
	CStringArray HTitles;
	CArray<float, float>HRatio;
	HTitles.Add(_LS(IDS_WG_EIGV__Load_Case));
	HTitles.Add(_LS(IDS_WG_EIGV__Count));
	HRatio.Add(0.5f);
	HRatio.Add(0.5f);

	CDlgUtil::_SetListCtrlHeader(&m_wndLCList,HTitles,&HRatio,NULL);
}

void CEigvDlg_MEC::MakeCaseList()
{
	// Load Case List를 설정한다. 
	T_STLD_D StldD;
	m_wndLCList.DeleteAllItems();
	CStringArray Contents;
	CString StrTemp;
	for(int i = 0; i< D_EIGV_NUMLC ; i++)
	{
		if(m_EigvData.Ritz.LC[i].LoadCaseKey == 0) break;
		
		Contents.RemoveAll();
		if(m_EigvData.Ritz.LC[i].nType == 0) // Static LoadCase
		{ 
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStld(m_EigvData.Ritz.LC[i].LoadCaseKey, StldD))
				Contents.Add(StldD.LoadCaseName);
			else 
				Contents.Add(_T(" "));
		}
		else //1:Ground Acc
		{
			switch(m_EigvData.Ritz.LC[i].LoadCaseKey)
			{
			case 1: Contents.Add(_LS(IDS_CMD_EIGV_ACC_X));
				break;
			case 2: Contents.Add(_LS(IDS_CMD_EIGV_ACC_Y));
				break; 
			case 3: Contents.Add(_LS(IDS_CMD_EIGV_ACC_Z));
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		StrTemp.Format(_T("%d"),m_EigvData.Ritz.LC[i].nCount);
		Contents.Add(StrTemp);

		CDlgUtil::SetListItem(&m_wndLCList,i,Contents,(LONG_PTR)(m_EigvData.Ritz.LC + i));
	}
}

void CEigvDlg_MEC::OnLcAddButton() 
{
	// TODO: Add your control notification handler code here
	
	this->m_nSTLC;
	int nCurLCI = m_wndLCCombo.GetCurSel();
	int nEndLCI = FindEndLCIndex();

	if(D_EIGV_NUMLC == nEndLCI)
	{
		AfxMessageBox(_LS(IDS_CMD_EIGV_EXCEED_MAX));
		return;
	}
	
	int nLCKey;
	int nType ;
	nLCKey = (int)(m_wndLCCombo.GetItemData(nCurLCI));

	if(nCurLCI >= m_nSTLC) // Ground Acc
		nType = 1;
	else                   // Static Load Case
		nType = 0;

	for(int i = 0; i<= nEndLCI ; i++)
	{
		if(m_EigvData.Ritz.LC[i].nType == nType && 
			 m_EigvData.Ritz.LC[i].LoadCaseKey == nLCKey)
		{
			AfxMessageBox(_LS(IDS_CMD_EIGV_LC_DUP_NOT_ALLOWED));
			return;
		}
	}

	m_EigvData.Ritz.LC[nEndLCI].nType = nType;
	m_EigvData.Ritz.LC[nEndLCI].LoadCaseKey = nLCKey;

	CString StrTemp;
	int nNumOfGen;
	this->m_wndNOGSpin.GetWindowText(StrTemp);
	if(!CStrParser::GetINumber(StrTemp,nNumOfGen))
		nNumOfGen = 0;
	
	m_EigvData.Ritz.LC[nEndLCI].nCount = nNumOfGen;

	MakeCaseList();
	DisplayRitzVectorNumber();
}

void CEigvDlg_MEC::OnLcDeleteButton() 
{
	// TODO: Add your control notification handler code here
	T_EIGV_RITZ_LC TempLC[D_EIGV_NUMLC];
	
	CMap<int,int,int,int> IndexMap;
	CArray<int, int> arSIndex;
	int TempI;
	if(CDlgUtil::GetSelectedListItemIndex(&m_wndLCList,arSIndex))
	{
		int nSel = arSIndex.GetSize();
		for(int i = 0; i < nSel ; i++)
		{ 
			IndexMap.SetAt(arSIndex[i],arSIndex[i]);
		}

		for (int i = 0; i < D_EIGV_NUMLC; i++)
		{
			TempLC[i].nType = 0; 
			TempLC[i].LoadCaseKey = 0; 
			TempLC[i].nCount = 0;
		}

		int CurI = 0;
		for(int i = 0; i < D_EIGV_NUMLC; i++)
		{
			if(!IndexMap.Lookup(i,TempI))
			{
				TempLC[CurI].LoadCaseKey = this->m_EigvData.Ritz.LC[i].LoadCaseKey;
				TempLC[CurI].nCount      = this->m_EigvData.Ritz.LC[i].nCount     ;
				TempLC[CurI].nType       = this->m_EigvData.Ritz.LC[i].nType      ;
				CurI++;  
			}
		}
	}

	memcpy(m_EigvData.Ritz.LC,TempLC,sizeof(T_EIGV_RITZ_LC)*D_EIGV_NUMLC);
	MakeCaseList();
	DisplayRitzVectorNumber();
}

void CEigvDlg_MEC::OnLcModifyButton() 
{
	// TODO: Add your control notification handler code here
	//this->m_wndLCList.GetSelectedList
	CArray<int,int> arRet;
	if(!CDlgUtil::GetSelectedListItemIndex(&m_wndLCList,arRet)) return; 
	if(arRet.GetSize() != 1) return;

	int nEndLCI = arRet[0];
	int nCurLCI = m_wndLCCombo.GetCurSel();
	int nType ;
	int nLCKey = (int)(m_wndLCCombo.GetItemData(nCurLCI));

	if(nCurLCI >= m_nSTLC) // Ground Acc
		nType = 1;
	else                   // Static Load Case
		nType = 0;

	CString StrTemp;
	int nNumOfGen;
	this->m_wndNOGSpin.GetWindowText(StrTemp);
	if(!CStrParser::GetINumber(StrTemp,nNumOfGen))
		nNumOfGen = 0;
	
	struct T_EIGV_RITZ_LC SaveRitzLC;
	
	SaveRitzLC = m_EigvData.Ritz.LC[nEndLCI];
	
	
	if(SaveRitzLC.LoadCaseKey != nLCKey || 
		 SaveRitzLC.nType       != nType)
	{
		m_EigvData.Ritz.LC[nEndLCI].nType = -1;
		m_EigvData.Ritz.LC[nEndLCI].LoadCaseKey = -1;
		m_EigvData.Ritz.LC[nEndLCI].nCount = -1;

		
		for(int i = 0; i<= D_EIGV_NUMLC ; i++)
		{
			if(m_EigvData.Ritz.LC[i].nType == nType && 
				 m_EigvData.Ritz.LC[i].LoadCaseKey == nLCKey )
			{
				AfxMessageBox(_LS(IDS_CMD_EIGV_LC_DUP_NOT_ALLOWED));
				m_EigvData.Ritz.LC[nEndLCI] = SaveRitzLC;
				return;
			}
		}
	}

	m_EigvData.Ritz.LC[nEndLCI].nType = nType;
	m_EigvData.Ritz.LC[nEndLCI].LoadCaseKey = nLCKey;
	m_EigvData.Ritz.LC[nEndLCI].nCount = nNumOfGen;
	
	MakeCaseList();
	DisplayRitzVectorNumber();
}

//////////////////////////////////////////////////////////////////////
// Load Case List에서 선택 항목 바뀔때 내용 반영 할것..
// Load Case
// Number Of Generation .
// Load Type... ^^

void CEigvDlg_MEC::ShowCurLoadCaseData()
{
	
	CArray<int,int> arSel;
	CDlgUtil::GetSelectedListItemIndex(&m_wndLCList,arSel);
	if(arSel.GetSize() != 1) return;

	if(m_EigvData.Ritz.LC[arSel[0]].nType     == 0)
	{
		/*
		unsigned int nLoadCaseType, nLoadCaseKey;
		m_wndLCCombo.GetSelectedLoad(nLoadCaseType,nLoadCaseKey);
		m_wndLCCombo.ChangeSelect(nLoadCaseType,m_EigvData.Ritz.LC[arSel[0]].LoadCaseKey);
		*/
		int nStld = m_wndLCCombo.GetCount() - 3;
		ASSERT(nStld >= 0);
		for( int i = 0; i< nStld; i++)
		{
			if(m_wndLCCombo.GetItemData(i) == m_EigvData.Ritz.LC[arSel[0]].LoadCaseKey)
			{
				m_wndLCCombo.SetCurSel(i);
				break;
			}
		}
	}
	else
	{ 
		//m_EigvData.Ritz.LC[arSel[0]].LoadCaseKey;
		int nSel = m_wndLCCombo.GetCount() - (4 - m_EigvData.Ritz.LC[arSel[0]].LoadCaseKey);
		m_wndLCCombo.SetCurSel(nSel);
	}

	CString TempStr;
	TempStr.Format(_T("%d"),m_EigvData.Ritz.LC[arSel[0]].nCount);

	m_wndNOGSpin.SetWindowText(TempStr);
	
}

void CEigvDlg_MEC::OnItemchangedLcList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowCurLoadCaseData();	
	*pResult = 0;
}

void CEigvDlg_MEC::OnClickLcList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowCurLoadCaseData();	
	*pResult = 0;
}

void CEigvDlg_MEC::OnIncNlLinkFvectorCheck() 
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	NLLVCtrlMan();	
	if(m_wndLCList.GetItemCount()>0) DisplayRitzVectorNumber();
}

void CEigvDlg_MEC::DisplayRitzVectorNumber()
{
	int nLC = m_wndLCList.GetItemCount();
	
	int nDB;
	
	if(m_bNLLinkVect)
		nDB = CDBDoc::GetDocPoint()->m_pAttrCtrl->CalcEigvNlnkForceVectors();
	else
		nDB = 0;
	
	CString StrTemp;
	StrTemp.Format(_T("%d"),nDB);
	m_wndNLFVector.SetWindowText(StrTemp);
	StrTemp.Format(_T("%d"),nLC + nDB);
	m_wndTotalNumOfRitz.SetWindowText(StrTemp);

	DlgVar2Eigv();

	int nTR = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNumEigv(m_EigvData);
	StrTemp.Format(_T("%d"),nTR);
	m_wndTotalNumOfRitz2.SetWindowText(StrTemp);

	//StrTemp.Format(_T("%d"),nLC);
	//m_wndTotalNumOfRitz2.SetWindowText(StrTemp);
}


void CEigvDlg_MEC::OnChangeNlLinkVnumberEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogMove::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(__bIsNotOnInitialUpdate_MEC)
		DisplayRitzVectorNumber();
	
}
void CEigvDlg_MEC::OnUseFRICheck()
{
	CArray<UINT,UINT> arTempCtrls;
	arTempCtrls.Add(IDC_EIGV_LOF_EDIT);
	arTempCtrls.Add(IDC_EIGV_UPF_EDIT);
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this,arTempCtrls,m_bUseMinMax);
}


void CEigvDlg_MEC::OnEIGVComplexRadio()
{
// 	m_nSubComp = GetCheckedRadioButton(IDC_COMP_MASS_RADIO, IDC_COMP_ELEM_RADIO);
// 	ChangeComplexBtnText(m_nSubComp);

	/*if (m_nSubComp == IDC_COMP_ELEM_RADIO)
	{
		T_STYP_D DataStyp;
		if (!m_pDoc->m_pAttrCtrl->GetStyp(DataStyp))
			DataStyp.Initialize();
		DataStyp.nMassType = 1;
		DataStyp.bConsiderOffset = TRUE;
	}*/
	
}

void CEigvDlg_MEC::CtrlComplex(BOOL bShow)
{
	CDlgUtil::CtrlEnableDisable(this, IDC_COMP_MASS_RADIO, bShow);
	CDlgUtil::CtrlEnableDisable(this, IDC_COMP_ELEM_RADIO, bShow);
	CDlgUtil::CtrlEnableDisable(this, IDC_COMP_BUTTON, bShow);
}

void CEigvDlg_MEC::ChangeComplexBtnText(int nType)
{
// 	if (nType == IDC_COMP_MASS_RADIO)
// 		GetDlgItem(IDC_COMP_BUTTON)->SetWindowText(_LS(IDS_IDD_ETC_EIGV_IDC_COMP_BUTTON));
// 	else
// 		GetDlgItem(IDC_COMP_BUTTON)->SetWindowText(_LS(IDS_IDD_ETC_EIGV_IDC_COMP_BUTTON1));
}

void CEigvDlg_MEC::OnEigvComplexBtn()
{
	UpdateData();

// 	if (m_nSubComp == 0)
// 		OpenComplexMassDlg();
// 	else
// 		OpenComplexElemDlg();
}

void CEigvDlg_MEC::OpenComplexMassDlg()
{
	CArray<T_THIS_K, T_THIS_K> rKeyList;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(rKeyList);
	int nThis = rKeyList.GetCount();

	if (nThis > 0)
		GSaveHistoryFormatNF(_LS(IDS_CMD_EIGV_COMPLEX_TIP));

	CEigvComplexDlg dlg;
	dlg.SetDlgData(m_EigvData.CompMass);
	dlg.SetInitPos(D_INIT_POS_LT, 0, 50);
	dlg.DoModal();

	//m_EigvData.CompMass.Initialize();
	if (!dlg.GetDlgData(m_EigvData.CompMass))
		return;

	m_bComp = TRUE;
}

void CEigvDlg_MEC::OpenComplexElemDlg()
{
	CCMGroupDampingElementDlg dlg;
	dlg.SetInitPos(D_INIT_POS_LT, 0, 50);
	dlg.DoModal();
}