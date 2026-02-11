// CMTendonAddDlg.cpp : implementation file
//
// Old Version : 5.1.0부터 CMTendonProfileDlg.cpp로 변경

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTendonAddDlg.h"
#include "CMTendonTypeDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_I_GenModelBase.h"
#include "..\wg_gr\LGLtype.h"

#include "CMTendonDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTendonAddDlg dialog
void CCMTendonAddDlg::SetParentDlg(CDialog *pParentDlg)
{
	m_pMyParentDlg = pParentDlg;
}

#define CDialog CCMDlgBase

CCMTendonAddDlg::CCMTendonAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMTendonAddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTendonAddDlg)
	m_strTendonName = _T("");
	m_bDY = FALSE;
	m_bDZ = FALSE;
	m_nPropileOpt = 0;
	m_nDistFromIOpt = 0;
	m_bXVector = FALSE;
	m_bYVector = FALSE;
	m_bZVector = FALSE;
	//}}AFX_DATA_INIT
	m_TdnaKey  = 0;
	m_TdnaData.Initialize();
	m_pMyParentDlg = NULL;
	m_bDirVector = FALSE;
}

void CCMTendonAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTendonAddDlg)
	DDX_Control(pDX, IDC_CMD_ABS_REL_DIST_UNIT9, m_RELDistUnit);
	DDX_Control(pDX, IDC_CMD_DIST_UNIT1, m_wndDistUnit1);
	DDX_Control(pDX, IDC_CMD_DIST_UNIT2, m_wndDistUnit2);
	DDX_Control(pDX, IDC_CMD_DIST_UNIT3, m_wndDistUnit3);
	DDX_Control(pDX, IDC_CMD_DIST_UNIT4, m_wndDistUnit4);
	DDX_Control(pDX, IDC_CMD_DIST_UNIT5, m_wndDistUnit5);
	DDX_Control(pDX, IDC_CMD_DIST_UNIT6, m_wndDistUnit6);
	DDX_Control(pDX, IDC_CMD_DZ_EDIT, m_wndDZ);
	DDX_Control(pDX, IDC_CMD_DY_EDIT, m_wndDY);
	DDX_Control(pDX, IDC_CMD_EZ_EDIT, m_wndEZ);
	DDX_Control(pDX, IDC_CMD_EY_EDIT, m_wndEY);
	DDX_Control(pDX, IDC_CMD_REF_ELEM_EDIT, m_wndRefElem);
	DDX_Control(pDX, IDC_CMD_DIST_FROM_I_NODE_EDIT, m_wndDistFromINode);
	DDX_Control(pDX, IDC_CMD_END_LENGTH_EDIT, m_wndEndLength);
	DDX_Control(pDX, IDC_CMD_BEGIN_LENGTH_EDIT, m_wndBeginLength);
	DDX_Control(pDX, IDC_CMD_ABS_DIRVECTOR_EDIT, m_wndABSDVector);
	DDX_Control(pDX, IDC_CMD_ABS_COORD_EDIT, m_wndABSCoord);
	DDX_Control(pDX, IDC_CMD_ASSIGNED_ELEM_EDIT, m_wndAssignElem);
	DDX_Control(pDX, IDC_CMD_TENDON_DATA_LIST, m_wndTendonDataList);
	DDX_Control(pDX, IDC_CMD_TENDON_PROP_COMBO, m_wndTdntCobx);
	DDX_Text(pDX, IDC_CMD_TENDON_NAME_EDIT, m_strTendonName);
	DDX_Check(pDX, IDC_CMD_DY_CHECK, m_bDY);
	DDX_Check(pDX, IDC_CMD_DZ_CHECK, m_bDZ);
	DDX_Radio(pDX, IDC_CMD_POPT_ABS_RADIO, m_nPropileOpt);
	DDX_Radio(pDX, IDC_CMD_RELATIVE_ABS_RADIO, m_nDistFromIOpt);
	DDX_Check(pDX, IDC_CMD_DIRVECT_X_CHECK, m_bXVector);
	DDX_Check(pDX, IDC_CMD_DIRVECT_Y_CHECK, m_bYVector);
	DDX_Check(pDX, IDC_CMD_DIRVECT_Z_CHECK, m_bZVector);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTendonAddDlg, CDialog)
	//{{AFX_MSG_MAP(CCMTendonAddDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOk)
	ON_BN_CLICKED(IDC_CMD_POPT_ABS_RADIO, OnCmdProfileOptionRadio)
	ON_BN_CLICKED(IDC_CMD_TENDON_PROP_BUTTON, OnCmdTendonPropButton)
	ON_BN_CLICKED(IDC_CMD_ADD_BUTTON, OnCmdAddButton)
	ON_BN_CLICKED(IDC_CMD_DELETE_BUTTON, OnCmdDeleteButton)
	ON_BN_CLICKED(IDC_CMD_MODIFY_BUTTON, OnCmdModifyButton)
	ON_BN_CLICKED(IDC_CMD_RELATIVE_ABS_RADIO, OnDistOption)
	ON_BN_CLICKED(IDC_CMD_DY_CHECK, OnCmdAzAyCheck)
	ON_NOTIFY(NM_CLICK, IDC_CMD_TENDON_DATA_LIST, OnClickCmdTendonDataList)
	ON_NOTIFY(HDN_ENDDRAG, IDC_CMD_TENDON_DATA_LIST, OnEnddragCmdTendonDataList)
	ON_BN_CLICKED(IDC_CMD_PRIO_UP_BUTTON, OnCmdPrioUpButton)
	ON_BN_CLICKED(IDC_CMD_PRIO_DOWN_BUTTON, OnCmdPrioDownButton)
	ON_NOTIFY(LVN_ODSTATECHANGED, IDC_CMD_TENDON_DATA_LIST, OnOdstatechangedCmdTendonDataList)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_REL_ALPHAY_SPIN, OnDeltaposCmdRelAlphaYSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_REL_ALPHAZ_SPIN, OnDeltaposCmdRelAlphaZSpin)
	ON_BN_CLICKED(IDC_CMD_POPT_REL_RADIO, OnCmdProfileOptionRadio)
	ON_BN_CLICKED(IDC_CMD_RELATIVE_REL_RADIO, OnDistOption)
	ON_BN_CLICKED(IDC_CMD_DZ_CHECK, OnCmdAzAyCheck)
	ON_BN_CLICKED(IDC_CMD_DIRVECT_X_CHECK, OnCmdDirvectCheck)
	ON_BN_CLICKED(IDC_CMD_DIRVECT_Y_CHECK, OnCmdDirvectCheck)
	ON_BN_CLICKED(IDC_CMD_DIRVECT_Z_CHECK, OnCmdDirvectCheck)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMyListCtrl::NLV_KEYUP,OnListCtrlKeyUp)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnMouseEditEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CCMTendonAddDlg message handlers
BOOL CCMTendonAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DlgSizeMan();
	//I_GENModelBase::GetCurMySelfST()->UnselectAllNode();
	//I_GENModelBase::GetCurMySelfST()->UnselectAllElem(TRUE);

	m_wndDistUnit1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistUnit3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistUnit4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistUnit5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistUnit6.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_RELDistUnit.SetUnitType (D_UNITSYS_BASE_LENGTH);
	/*
	m_wndDistUnit7.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistUnit8.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistUnit9.SetUnitType(D_UNITSYS_BASE_LENGTH);
	*/

	m_wndRefElem.SetAttElemList();
	m_wndRefElem.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndRefElem.SetMaxElemKeyNum(1);
	
	m_wndDistFromINode.SetAttWcsDistance();
	m_wndDistFromINode.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndDistFromINode.SetDistValueMode(TRUE);

	m_wndBeginLength.SetAttWcsDistance();
	m_wndBeginLength.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndBeginLength.SetDistValueMode(TRUE);
	
	m_wndEndLength.SetAttWcsDistance();
	m_wndEndLength.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndEndLength.SetDistValueMode(TRUE);

 
	m_wndABSCoord.SetAttWcsPos();
	m_wndABSCoord.SetModeToUse(MOUSEEDIT_USE_GET_POS_CONTINUE);

	m_wndABSDVector.SetAttWcsDistance();
	m_wndABSDVector.SetModeToUse(MOUSEEDIT_USE_SET_DIST);

	m_wndEY.SetAttWcsDistance();
	m_wndEY.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndEY.SetDistValueMode(TRUE);

	m_wndEZ.SetAttWcsDistance();
	m_wndEZ.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndEZ.SetDistValueMode(TRUE);

	
	m_wndDistFromINode.SetWindowText(_T("0"));
	m_wndEY.SetWindowText(_T("0"));
	m_wndEZ.SetWindowText(_T("0"));
	m_wndDY.SetWindowText(_T("0"));
	m_wndDZ.SetWindowText(_T("0"));
	m_wndBeginLength.SetWindowText(_T("0"));
	m_wndEndLength  .SetWindowText(_T("0"));
	m_wndABSCoord   .SetWindowText(_T("0, 0, 0"));
	//m_wndABSDVector .SetWindowText(_T("0, 0, 0"));
	m_wndABSDVector.MySetWindowText(_T("0, 0, 0"));

	m_wndABSCoord.SetEnterNotifyWindow(this);
	m_wndABSCoord.SetLButtonDownNotifyWindow(this);



	

	/*
	
	
	m_wndEndLength;
	m_wndBeginLength;
	m_wndABSDVector;
	m_wndABSCoord;
	*/

	m_wndAssignElem.Connect(SC_ID_ELEM,&m_wndAssignElem);
		
	MakeListHeader();
	SetData2Dlg();
	
	SetDirVectorMode();
	AbsRelCtrlMan();

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMTendonAddDlg::MakeListHeader()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;
	HTitles.Add(_T("No"));        HRatio.Add(0.1f);
	HTitles.Add(_T("Option"));    HRatio.Add(0.2f);
	HTitles.Add(_T("X"));         HRatio.Add(0.23f);
	HTitles.Add(_T("Y"));         HRatio.Add(0.23f); 
	HTitles.Add(_T("Z"));         HRatio.Add(0.23f);
	HTitles.Add(_T("DX"));        HRatio.Add(0.2f);
	HTitles.Add(_T("DY"));        HRatio.Add(0.2f);
	HTitles.Add(_T("DZ"));        HRatio.Add(0.2f);
	HTitles.Add(_T("Ref. El"));   HRatio.Add(0.4f);

	CDlgUtil::_SetListCtrlHeader(&m_wndTendonDataList,HTitles,&HRatio,NULL);
}

void CCMTendonAddDlg::SetTDNA(T_TDNA_K TdnaKey)
{
	m_TdnaKey  = TdnaKey;
	m_TdnaData.Initialize();
	m_OriTdnaName = m_TdnaData.TendonName;
	if(TdnaKey != 0) // Modify
	{
		//*^^* 수정됨 by jbseon
		//if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdna(TdnaKey,m_TdnaData))
		{
			m_TdnaData.Initialize();
		}
		m_OriTdnaName = m_TdnaData.TendonName;
	}

	if(GetSafeHwnd())
	{
		SetData2Dlg();
	}
}

void CCMTendonAddDlg::SetData2Dlg()
{
	m_strTendonName =  m_TdnaData.TendonName;
	m_wndTdntCobx.ChangeSelect(m_TdnaData.TendonTypeKey);
	CArray<T_NODE_K,T_NODE_K> rNodes;
	CDBDoc::GetDocPoint()->m_pViewCtrl->Select(NULL,rNodes,m_TdnaData.aElemList,TRUE);
	
	CString StrTemp;
	StrTemp.Format(_T("%.8g"),m_TdnaData.dBeginLength);
	m_wndBeginLength.SetWindowText(StrTemp);
	StrTemp.Format(_T("%.8g"),m_TdnaData.dEndLength);
	m_wndEndLength.SetWindowText(StrTemp);

	UpdateData(FALSE);

	UpdateProfileList();
	
}

void CCMTendonAddDlg::UpdateProfileList()
{
	m_wndTendonDataList.DeleteAllItems();
	CStringArray Contents;
	CString StrTemp;
	int nProfile = m_TdnaData.aProfile.GetSize();

	T_TDNA_BASE_501 TTdnaBase;

	for(int i= 0; i < nProfile; i++)
	{
		Contents.RemoveAll();
		
		TTdnaBase = m_TdnaData.aProfile[i];
	 
		if(ConvREL2ABS(TTdnaBase))
		{
			StrTemp.Format(_T("%d"),i+1);
			Contents.Add(StrTemp);
			
			if(TTdnaBase.nInputOption == 0)
				StrTemp.Format(_T("REL"));
			else
				StrTemp.Format(_T("ABS"));


			Contents.Add(StrTemp);
			StrTemp.Format(_T("%.8g"),TTdnaBase.dCoord[0]);
			Contents.Add(StrTemp);
			StrTemp.Format(_T("%.8g"),TTdnaBase.dCoord[1]);
			Contents.Add(StrTemp);
			StrTemp.Format(_T("%.8g"),TTdnaBase.dCoord[2]);
			Contents.Add(StrTemp);
			StrTemp.Format(_T("%.8g"),TTdnaBase.dDirVec[0]);
			Contents.Add(StrTemp);
			StrTemp.Format(_T("%.8g"),TTdnaBase.dDirVec[1]);
			Contents.Add(StrTemp);
			StrTemp.Format(_T("%.8g"),TTdnaBase.dDirVec[2]);
			Contents.Add(StrTemp);
			StrTemp.Format(_T("%d"),TTdnaBase.RefElemKey);
			Contents.Add(StrTemp);
		}
		else
		{
			Contents.RemoveAll();
			StrTemp.Format(_T("%d"),i+1);
			Contents.Add(StrTemp);
			if(TTdnaBase.nInputOption == 0)
				StrTemp.Format(_T("REL"));
			else
				StrTemp.Format(_T("ABS"));
			Contents.Add(StrTemp);
			
			StrTemp.Format(_T(" "));
			Contents.Add(StrTemp);
			StrTemp.Format(_T(" "));
			Contents.Add(StrTemp);
			Contents.Add(StrTemp);
			Contents.Add(StrTemp);
			Contents.Add(StrTemp);
			Contents.Add(StrTemp);
			StrTemp.Format(_T("0"));
			Contents.Add(StrTemp);
		}

		CDlgUtil::SetListItem(&m_wndTendonDataList,i,Contents,(LONG_PTR)&(m_TdnaData.aProfile[i]));
	}
}

void CCMTendonAddDlg::OnCmdAddButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	T_TDNA_BASE_501 TdnaB;
	TdnaB.Initiaize();
	
	if(m_nPropileOpt == 0) // Absolute
	{
		double PX,PY,PZ;
		double VX,VY,VZ;
		BOOL B1,B2;
		B1 = m_wndABSCoord  .GetCoordData(&PX,&PY,&PZ);
		B2 = m_wndABSDVector.GetCoordData(&VX,&VY,&VZ);
		if(!B1 || !B2) return;

		//TdnaB.

		if(m_nPropileOpt == 0)
			TdnaB.nInputOption = 1;
		else
			TdnaB.nInputOption = 0;

		if(m_nDistFromIOpt == 0)
			TdnaB.nDistOpt = 1;
		else
			TdnaB.nDistOpt = 0;

		
		
		TdnaB.dCoord [0] = PX; TdnaB.dCoord [1] = PY; TdnaB.dCoord [2] = PZ;
		TdnaB.bXYZ[0]=m_bXVector;
		TdnaB.bXYZ[1]=m_bYVector;
		TdnaB.bXYZ[2]=m_bZVector;

		if(m_bXVector == TRUE && m_bYVector == FALSE && m_bZVector == FALSE)  
		{ 
			TdnaB.dDirVec[0] = VX; TdnaB.dDirVec[1] = 0 ; TdnaB.dDirVec[2] = 0 ; 
		}
		else if(m_bXVector == FALSE && m_bYVector == TRUE && m_bZVector == FALSE)
		{
			TdnaB.dDirVec[0] = 0 ; TdnaB.dDirVec[1] = VY; TdnaB.dDirVec[2] = 0 ; 
		} 
		else if(m_bXVector == FALSE && m_bYVector == FALSE && m_bZVector == TRUE)
		{ 
			TdnaB.dDirVec[0] = 0 ; TdnaB.dDirVec[1] = 0 ; TdnaB.dDirVec[2] = VZ; 
		}   
		else if(m_bXVector == TRUE && m_bYVector == TRUE && m_bZVector == FALSE)
		{
			TdnaB.dDirVec[0] = VX; TdnaB.dDirVec[1] = VY ; TdnaB.dDirVec[2] = 0 ; 
		}  
		else if(m_bXVector == FALSE && m_bYVector == TRUE && m_bZVector == TRUE)
		{
			TdnaB.dDirVec[0] = 0 ; TdnaB.dDirVec[1] = VY ; TdnaB.dDirVec[2] = VZ ; 
		}  
		else if(m_bXVector == TRUE && m_bYVector == FALSE && m_bZVector == TRUE)
		{ 
			TdnaB.dDirVec[0] = VX; TdnaB.dDirVec[1] = 0 ; TdnaB.dDirVec[2] = VZ ; 
		}  
		else if(m_bXVector == TRUE && m_bYVector == TRUE && m_bZVector == TRUE)
		{ 
			TdnaB.dDirVec[0] = VX; TdnaB.dDirVec[1] = VY ; TdnaB.dDirVec[2] = VZ ;
		}

		
	}
	else  // Relative
	{
		
		CString strText;
		m_wndRefElem.GetWindowText(strText);

		int INum;
		if(!CStrParser::GetINumber(strText,INum)) return;
		if(INum<0) return;
		TdnaB.RefElemKey = INum;

		if(m_nDistFromIOpt == 0)
			TdnaB.nDistOpt = 1;
		else
			TdnaB.nDistOpt = 0;

		if(!m_wndDistFromINode.GetCoordData(&(TdnaB.dDistI),NULL,NULL)) return;

		TdnaB.bAlpha = m_bDZ;
		//TdnaB.bAlphaY = m_bDY;
				
		if(m_bDZ)
		{
			m_wndDZ.GetWindowText(strText);
			if(!CStrParser::GetFloatNumber(strText,TdnaB.dAlphaZ)) return;
		}

		//if(m_bDY)
		if(m_bDZ)
		{
			m_wndDY.GetWindowText(strText);
			if(!CStrParser::GetFloatNumber(strText,TdnaB.dAlphaY)) return;
		}

		
		
		if(m_nPropileOpt == 0)
			TdnaB.nInputOption = 1;
		else
			TdnaB.nInputOption = 0;
		
		if(!m_wndEY.GetCoordData(&(TdnaB.dEccY),NULL,NULL)) return ;
		if(!m_wndEZ.GetCoordData(&(TdnaB.dEccZ),NULL,NULL)) return ;
	}

	m_TdnaData.aProfile.Add(TdnaB);
	UpdateProfileList();
}


/*
struct T_TDNA_BASE_501
{
	int nInputOption;   // 0:Relative, 1:Absolute
	// Relative
	T_ELEM_K RefElemKey;  // Frame Type 요소만 가능
	double dDistI;       // 단위:없음(0.0 ~ 1.0)
	double dEccY, dEccZ;
	BOOL   bAlphaY, bAlphaZ;
	double dAlphaY, dAlphaZ;
	// Absolute
	double dCoord[3];   // x, y, z
	double dDirVec[3];  // dx, dy, dz

	void Initiaize()
	{
		nInputOption = 0;
		RefElemKey = 0;
		dDistI = 0.0;
		dEccY = dEccZ = 0.0;
		bAlphaY = bAlphaZ = FALSE;
		dAlphaY = dAlphaZ = 0.0;
		for (int i = 0; i < 3; i++) dCoord[i] = dDirVec[i] = 0.0;
	}
};
*/

BOOL CCMTendonAddDlg::ConvREL2ABS(T_TDNA_BASE_501& TdnaB ,BOOL bBeforeAdd /*= FALSE*/ )
{
	if(TdnaB.nInputOption != 0) return TRUE;
	if(TdnaB.RefElemKey  == 0 ) return FALSE;

	int nInputOption = TdnaB.nInputOption;

	T_TDNA_D_501 TdnaD;
	TdnaD.aProfile.Add(TdnaB);
	
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->calcTdnaRelativeToAbsolute(TdnaD)) return FALSE;

	TdnaB = TdnaD.aProfile[0];

	if(bBeforeAdd)
		TdnaB.nDistOpt = 0;

	TdnaB.nInputOption = nInputOption;

	return TRUE;
	

	
	
	double EXVect[3]; // I --> J
	double ELocVect[3][3];
	double ELength;

	T_NODE_D NI,NJ;
	T_ELEM_D ElemD;

	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(TdnaB.RefElemKey,ElemD))  return FALSE;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsFrameType(ElemD.eltyp))         return FALSE;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNode(ElemD.elnod[0],NI))       return FALSE;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNode(ElemD.elnod[1],NJ))       return FALSE;
	if(!CDBDoc::GetDocPoint()->calcElemLocalVector(TdnaB.RefElemKey,ELocVect))return FALSE;

	EXVect[0] = NJ.x - NI.x;
	EXVect[1] = NJ.y - NI.y;
	EXVect[2] = NJ.z - NI.z;

	ELength = sqrt(EXVect[0]*EXVect[0] + EXVect[1]*EXVect[1] + EXVect[2]*EXVect[2]);

	if(I_GENModelBase::GetCurMySelfST()->IsZero(ELength)) return FALSE;

	EXVect[0] = EXVect[0] / ELength;
	EXVect[1] = EXVect[1] / ELength;
	EXVect[2] = EXVect[2] / ELength;

	double DistFromI;
	if(TdnaB.nDistOpt == 1 )// ABS
	{
		DistFromI    = TdnaB.dDistI;
		TdnaB.dDistI = TdnaB.dDistI / ELength;
	}
	else
	{
		DistFromI = ELength * TdnaB.dDistI;  
	}

	if(bBeforeAdd)
		TdnaB.nDistOpt = 0;

	double XPos[3],YPos[3],OPos[3] ;
		
	OPos[0] = NI.x + EXVect[0]*DistFromI; 
	OPos[1] = NI.y + EXVect[1]*DistFromI + TdnaB.dEccY ; 
	OPos[2] = NI.z + EXVect[2]*DistFromI + TdnaB.dEccZ ;
	
	TdnaB.dCoord[0] = OPos[0];
	TdnaB.dCoord[1] = OPos[1];
	TdnaB.dCoord[2] = OPos[2];
	
	
	if( TdnaB.bAlpha)
	{
		// Ratio로 설정된 상태.
		XPos[0] = ELocVect[0][0] + OPos[0];
		XPos[1] = ELocVect[0][1] + OPos[1];
		XPos[2] = ELocVect[0][2] + OPos[2];

		YPos[0] = ELocVect[1][0] + OPos[0];
		YPos[1] = ELocVect[1][1] + OPos[1];
		YPos[2] = ELocVect[1][2] + OPos[2];

		if(!I_GENModelBase::GetCurMySelfST()->UTIL_SetCoordMapper(OPos,XPos,YPos)) return FALSE;
		
		double DCosZ[3]={0,0,0};
		double DCosY[3]={0,0,0};
		double SrcDir[3],DestDir[3];

		if(TdnaB.bAlpha)
		{
			double CosA,SinA;
			CosA = cos(RAD(TdnaB.dAlphaZ));
			SinA = sin(RAD(TdnaB.dAlphaZ));
			DCosZ[0] = CosA;
			DCosZ[1] = SinA;
		}

		if(TdnaB.bAlpha)
		{
			double CosA,SinA;
			CosA = cos(RAD(TdnaB.dAlphaY));
			SinA = sin(RAD(TdnaB.dAlphaY));
			DCosY[0] = CosA;
			DCosY[2] = -SinA;
		}

		SrcDir[0] = DCosY[0] + DCosZ[0];
		SrcDir[1] = DCosY[1] + DCosZ[1];
		SrcDir[2] = DCosY[2] + DCosZ[2];

		I_GENModelBase::GetCurMySelfST()->UTIL_DoPlaneToWcs(SrcDir,DestDir);

		double DirVect[3];
		DirVect[0] = DestDir[0] - OPos[0];
		DirVect[1] = DestDir[1] - OPos[1];
		DirVect[2] = DestDir[2] - OPos[2];

		double DirLength;
		DirLength = sqrt( DirVect[0]*DirVect[0] + DirVect[1]*DirVect[1] + DirVect[2]*DirVect[2]);
		if(I_GENModelBase::GetCurMySelfST()->IsZero(DirLength)) return FALSE;

		DirVect[0] = DirVect[0] / DirLength;
		DirVect[1] = DirVect[1] / DirLength;
		DirVect[2] = DirVect[2] / DirLength;

		TdnaB.dDirVec[0] = DirVect[0];
		TdnaB.dDirVec[1] = DirVect[1];
		TdnaB.dDirVec[2] = DirVect[2];
	}
	else
	{
		TdnaB.dDirVec[0] = 0; 
		TdnaB.dDirVec[1] = 0;
		TdnaB.dDirVec[2] = 0;
	}
	
	return TRUE;
}


BOOL CCMTendonAddDlg::ApplyDlg()
{
	UpdateData(TRUE);
	m_TdnaData.TendonName  = this->m_strTendonName;
	m_wndTdntCobx.GetSelectedTdnt(m_TdnaData.TendonTypeKey);
	
	CString StrText;
	m_wndAssignElem.GetWindowText(StrText);

	CArray<int,int> aElem;
	if(!CStrParser::ParsingListByTo(StrText,aElem)) return FALSE;

	int nElem = aElem.GetSize();
	m_TdnaData.aElemList.RemoveAll();
	for(int i = 0; i< nElem; i++)
	{
		m_TdnaData.aElemList.Add(aElem[i]);
	}

	if(!m_wndBeginLength.GetCoordData(&(m_TdnaData.dBeginLength),NULL,NULL)) return FALSE;
	if(!m_wndEndLength.GetCoordData  (&(m_TdnaData.dEndLength  ),NULL,NULL)) return FALSE;
	
	int nProfile = m_TdnaData.aProfile.GetSize();
	/*
	for(  i = 0 ; i < nProfile ; i++)
	{
		if(!ConvREL2ABS(m_TdnaData.aProfile[i],TRUE)) return FALSE;
	}
	*/

	if(m_TdnaKey == 0) // Add
	{
		T_TDNT_K Tdnt = m_TdnaData.TendonTypeKey;
		//*^^* 수정됨 by jbseon
		//if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddTdna(m_TdnaData)) return FALSE;
		// DialogData 초기화 !!!
		m_TdnaData.Initialize();
		m_TdnaData.TendonTypeKey = Tdnt;
		SetData2Dlg();
		CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	}
	else               // Modify !!!
	{
		//*^^* 수정됨 by jbseon
		//if(!CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyTdna(m_OriTdnaName,m_TdnaData)) return FALSE;
		
		m_OriTdnaName = m_TdnaData.TendonName;

		if(m_pMyParentDlg) 
			((CCMTendonDlg*)m_pMyParentDlg)->DisplayCurrentTendon(m_TdnaKey);
	} 
	
	return TRUE;

}

void CCMTendonAddDlg::DlgSizeMan()
{
	CRect RectDlg, RectApply;
	
	GetWindowRect(&RectDlg);
	CWnd * pWnd = GetDlgItem(IDC_CMD_APPLY);
	ASSERT(pWnd);
	pWnd->GetWindowRect(&RectApply);

	int DeltaY = RectDlg.bottom - RectApply.bottom;

	int Wx = RectDlg.Width();
	int Wy = RectDlg.Height() - DeltaY+10;

	SetWindowPos(NULL, 0,0,Wx,Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}


void CCMTendonAddDlg::AbsRelCtrlMan()
{
	CArray<UINT,UINT> AbsCtrls;
	CArray<UINT,UINT> RelCtrls;

	AbsCtrls.Add(IDC_CMD_STATIC10              );
	AbsCtrls.Add(IDC_CMD_STATIC15              );
	AbsCtrls.Add(IDC_CMD_ABS_COORD_EDIT        );
	AbsCtrls.Add(IDC_CMD_DIST_UNIT1            );
	//AbsCtrls.Add(IDC_CMD_DIRVECT_CHECK         );
	AbsCtrls.Add(IDC_CMD_STATIC34              );
	AbsCtrls.Add(IDC_CMD_DIRVECT_X_CHECK       );
	AbsCtrls.Add(IDC_CMD_DIRVECT_Y_CHECK       );
	AbsCtrls.Add(IDC_CMD_DIRVECT_Z_CHECK       );
	AbsCtrls.Add(IDC_CMD_ABS_DIRVECTOR_EDIT    );
	AbsCtrls.Add(IDC_CMD_DIST_UNIT2            );
						 
	RelCtrls.Add(IDC_CMD_STATIC12              );  
	RelCtrls.Add(IDC_CMD_STATIC23              );
	RelCtrls.Add(IDC_CMD_REF_ELEM_EDIT         );
	RelCtrls.Add(IDC_CMD_STATIC24              );
	RelCtrls.Add(IDC_CMD_RELATIVE_ABS_RADIO    );
	RelCtrls.Add(IDC_CMD_RELATIVE_REL_RADIO    );
	RelCtrls.Add(IDC_CMD_DIST_FROM_I_NODE_EDIT );
	RelCtrls.Add(IDC_CMD_ABS_REL_DIST_UNIT9    );
	RelCtrls.Add(IDC_CMD_STATIC19              );
	RelCtrls.Add(IDC_CMD_STATIC25              );
	RelCtrls.Add(IDC_CMD_STATIC40              );
	RelCtrls.Add(IDC_CMD_EY_EDIT               );
	RelCtrls.Add(IDC_CMD_EZ_EDIT               );
	RelCtrls.Add(IDC_CMD_DIST_UNIT5            );
	RelCtrls.Add(IDC_CMD_DIST_UNIT6            );
	RelCtrls.Add(IDC_CMD_DZ_CHECK              );
	RelCtrls.Add(IDC_CMD_DZ_EDIT               );
	RelCtrls.Add(IDC_CMD_STATIC43              );
	RelCtrls.Add(IDC_CMD_DY_EDIT               );
	RelCtrls.Add(IDC_CMD_REL_ALPHAZ_SPIN       );
	RelCtrls.Add(IDC_CMD_REL_ALPHAY_SPIN       );
	RelCtrls.Add(IDC_CMD_DIST_UNIT7            );
	RelCtrls.Add(IDC_CMD_DIST_UNIT8            );

	BOOL bIsMoved = FALSE;  

	CWnd *pAbs,*pRel;
	pAbs   = GetDlgItem(IDC_CMD_STATIC10       );
	pRel   = GetDlgItem(IDC_CMD_STATIC12       );

	CRect RectAbs,RectRel;

	pAbs->GetWindowRect(RectAbs);
	pRel->GetWindowRect(RectRel);

	ScreenToClient(&RectAbs);
	ScreenToClient(&RectRel);

	int DeltaY;
	DeltaY = RectAbs.bottom - RectRel.bottom;

	if(DeltaY != 0)
	{
		CDlgUtil::CtrlMoveDistY(this,RelCtrls,DeltaY);
	}
	
	if(m_nPropileOpt == 0)
	{
		CDlgUtil::CtrlShowHide(this,AbsCtrls,TRUE);
		CDlgUtil::CtrlShowHide(this,RelCtrls,FALSE);
		m_wndABSDVector.EnableWindow(m_bDirVector);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this,AbsCtrls,FALSE);
		CDlgUtil::CtrlShowHide(this,RelCtrls,TRUE );
		CDlgUtil::CtrlEnableDisable(this,IDC_CMD_REL_ALPHAY_SPIN,m_bDZ);
		CDlgUtil::CtrlEnableDisable(this,IDC_CMD_REL_ALPHAZ_SPIN,m_bDZ);
		m_wndDY.EnableWindow(m_bDZ);
		m_wndDZ.EnableWindow(m_bDZ);
		if(m_nDistFromIOpt == 0)// Distance 
		{
			CMouseEdit::ImAssocWindowST();
			m_wndDistFromINode.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
			m_wndDistFromINode.SetAttWcsDistance();
			m_wndDistFromINode.SetKeyPressedFlag(TRUE);
			m_RELDistUnit.ShowWindow(SW_SHOW);
		}
		else // Ratio
		{ 
			CMouseEdit::ImNotAssocWindowST();
			m_wndDistFromINode.SetModeToUse(MOUSEEDIT_USE_NOTHING);
			m_wndDistFromINode.SetAttNothing();
			m_RELDistUnit.ShowWindow(SW_HIDE);
		}
	}
}

void CCMTendonAddDlg::OnCmdApply() 
{
	ApplyDlg();

}
void CCMTendonAddDlg::OnCmdCancel() 
{
	if(m_pMyParentDlg)
		if(m_pMyParentDlg->GetSafeHwnd())
			 m_pMyParentDlg->ShowWindow(SW_SHOW);
	
	CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	DestroyWindow();	
}

void CCMTendonAddDlg::OnCmdOk() 
{
	if(ApplyDlg())
	{
		if(m_pMyParentDlg)
			if(m_pMyParentDlg->GetSafeHwnd())
				m_pMyParentDlg->ShowWindow(SW_SHOW);
		DestroyWindow();
	}
}

void CCMTendonAddDlg::OnCmdProfileOptionRadio() 
{
	UpdateData();
	AbsRelCtrlMan();
}

void CCMTendonAddDlg::OnCmdTendonPropButton() 
{
	// TODO: Add your control notification handler code here
	CCMTendonTypeDlg Dlg;
	Dlg.DoModal();
}


void CCMTendonAddDlg::SetDirVectorMode()
{
	int nMEMode;

	this->m_bDirVector = TRUE;

	if(m_bXVector == TRUE && m_bYVector == FALSE && m_bZVector == FALSE)  
		nMEMode = 1;
	else if(m_bXVector == FALSE && m_bYVector == TRUE && m_bZVector == FALSE)
		nMEMode = 2;
	else if(m_bXVector == FALSE && m_bYVector == FALSE && m_bZVector == TRUE)
		nMEMode = 3;
	else if(m_bXVector == TRUE && m_bYVector == TRUE && m_bZVector == FALSE)
		nMEMode = 4;
	else if(m_bXVector == FALSE && m_bYVector == TRUE && m_bZVector == TRUE)
		nMEMode = 5;
	else if(m_bXVector == TRUE && m_bYVector == FALSE && m_bZVector == TRUE)
		nMEMode = 6;
	else if(m_bXVector == TRUE && m_bYVector == TRUE && m_bZVector == TRUE)
		nMEMode = 0;
	else
	{
		this->m_bDirVector = FALSE;
	}
 
	m_wndABSDVector.SetDistComponent(nMEMode);

}


void CCMTendonAddDlg::OnCmdDirvectCheck() 
{
	UpdateData(TRUE);
	
	if(m_bXVector || m_bYVector || m_bZVector) 
		m_bDirVector = TRUE;
	else
		m_bDirVector = FALSE;

	SetDirVectorMode();
	
	m_wndABSDVector.EnableWindow(m_bDirVector);
	if(!m_bDirVector)
	{

		//m_wndABSDVector.SetWindowText(_T("0, 0, 0"));
		m_wndABSDVector.MySetWindowText(_T("0, 0, 0"));
	}
	m_wndABSDVector.SetKeyPressedFlag(TRUE);
}

void CCMTendonAddDlg::OnCmdDeleteButton() 
{
	CArray<T_TDNA_BASE_501,T_TDNA_BASE_501&> DListBackup;
	CMap<int,int,int,int> RetMap;
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndTendonDataList,arRet,LVIS_SELECTED);

	int nRet = arRet.GetSize();
	if(nRet == 0 ) return;

	for( int i= 0; i < nRet; i++)
	{
		RetMap.SetAt(arRet[i],arRet[i]);
	}

	int nProf = m_TdnaData.aProfile.GetSize();
	int nTemp;
	for(int i = 0; i < nProf ;i++)
	{
		if(!RetMap.Lookup(i,nTemp))
		{
			DListBackup.Add(m_TdnaData.aProfile[i]);
		}
	}
	m_TdnaData.aProfile.RemoveAll();
	m_TdnaData.aProfile.Copy(DListBackup);
	UpdateProfileList();
}

void CCMTendonAddDlg::OnCmdModifyButton() 
{
	UpdateData();
	
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndTendonDataList,arRet,LVIS_SELECTED);
	int nRet = arRet.GetSize();

	if(nRet != 1) return;

	T_TDNA_BASE_501 TdnaB;
	TdnaB.Initiaize();

	if(m_nPropileOpt == 0)
		TdnaB.nInputOption = 1;
	else
		TdnaB.nInputOption = 0;

	if(m_nPropileOpt == 0) // Absolute
	{
		double PX,PY,PZ;
		double VX,VY,VZ;
		BOOL B1,B2;
		B1 = m_wndABSCoord  .GetCoordData(&PX,&PY,&PZ);
		B2 = m_wndABSDVector.GetCoordData(&VX,&VY,&VZ);
		if(!B1 || !B2) return;
		TdnaB.dCoord[0] = PX; TdnaB.dCoord [1] = PY; TdnaB.dCoord[2]  = PZ;

		TdnaB.bXYZ[0] = this->m_bXVector;
		TdnaB.bXYZ[1] = this->m_bYVector;
		TdnaB.bXYZ[2] = this->m_bZVector;
		
		if(m_bXVector == TRUE && m_bYVector == FALSE && m_bZVector == FALSE)  
		{ 
			TdnaB.dDirVec[0] = VX; TdnaB.dDirVec[1] = 0 ; TdnaB.dDirVec[2] = 0 ; 
		}
		else if(m_bXVector == FALSE && m_bYVector == TRUE && m_bZVector == FALSE)
		{
			TdnaB.dDirVec[0] = 0 ; TdnaB.dDirVec[1] = VY; TdnaB.dDirVec[2] = 0 ; 
		} 
		else if(m_bXVector == FALSE && m_bYVector == FALSE && m_bZVector == TRUE)
		{
			TdnaB.dDirVec[0] = 0 ; TdnaB.dDirVec[1] = 0 ; TdnaB.dDirVec[2] = VZ; 
		}   
		else if(m_bXVector == TRUE && m_bYVector == TRUE && m_bZVector == FALSE)
		{
			TdnaB.dDirVec[0] = VX; TdnaB.dDirVec[1] = VY ; TdnaB.dDirVec[2] = 0 ; 
		}  
		else if(m_bXVector == FALSE && m_bYVector == TRUE && m_bZVector == TRUE)
		{ 
			TdnaB.dDirVec[0] = 0 ; TdnaB.dDirVec[1] = VY ; TdnaB.dDirVec[2] = VZ ; 
		}  
		else if(m_bXVector == TRUE && m_bYVector == FALSE && m_bZVector == TRUE)
		{ 
			TdnaB.dDirVec[0] = VX; TdnaB.dDirVec[1] = 0 ; TdnaB.dDirVec[2] = VZ ; 
		}
		else if(m_bXVector == TRUE && m_bYVector == TRUE && m_bZVector == TRUE)
		{ 
			TdnaB.dDirVec[0] = VX; TdnaB.dDirVec[1] = VY ; TdnaB.dDirVec[2] = VZ ; 
		}

	}
	else  // Relative
	{
		CString strText;
		this->m_wndRefElem.GetWindowText(strText);

		int INum;
		if(!CStrParser::GetINumber(strText,INum)) return;
		if(INum<0) return;
		TdnaB.RefElemKey = INum;

		if(m_nDistFromIOpt == 0)
			TdnaB.nDistOpt = 1;
		else
			TdnaB.nDistOpt = 0;
		
		if(!m_wndDistFromINode.GetCoordData(&(TdnaB.dDistI),NULL,NULL)) return;

		
		TdnaB.bAlpha = m_bDZ;

		//if(m_bDY)
		if(m_bDZ)
		{
			m_wndDY.GetWindowText(strText);
			if(!CStrParser::GetFloatNumber(strText,TdnaB.dAlphaY)) return;
		}

		if(m_bDZ)
		{
			m_wndDZ.GetWindowText(strText);
			if(!CStrParser::GetFloatNumber(strText,TdnaB.dAlphaZ)) return;
		}
		
		if(m_nPropileOpt == 0)
			TdnaB.nInputOption = 1;
		else
			TdnaB.nInputOption = 0;
		
		if(!m_wndEY.GetCoordData(&(TdnaB.dEccY),NULL,NULL)) return ;
		if(!m_wndEZ.GetCoordData(&(TdnaB.dEccZ),NULL,NULL)) return ;
		
		
	}

	m_TdnaData.aProfile.SetAt(arRet[0],TdnaB);
	UpdateProfileList();
}

void CCMTendonAddDlg::OnDistOption() 
{
	UpdateData(TRUE);
	/*
	m_wndDY.EnableWindow(m_bDY);
	m_wndDZ.EnableWindow(m_bDZ);
	*/
	if(m_nDistFromIOpt == 0)
		m_RELDistUnit.ShowWindow(SW_SHOW);
	else
		m_RELDistUnit.ShowWindow(SW_HIDE);
}

void CCMTendonAddDlg::OnCmdAzAyCheck() 
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this,IDC_CMD_REL_ALPHAY_SPIN,m_bDZ);
	CDlgUtil::CtrlEnableDisable(this,IDC_CMD_REL_ALPHAZ_SPIN,m_bDZ);
	m_wndDZ.EnableWindow(m_bDZ);
	m_wndDY.EnableWindow(m_bDZ);
}

void CCMTendonAddDlg::ListSelOperation() 
{
	UpdateData(TRUE);
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndTendonDataList,arRet,LVIS_SELECTED);
	
	T_TDNA_BASE_501 * pCurTdna;

	int nRet = arRet.GetSize();

	if(nRet > 1) // Multi Selected
	{

	}
	else if(nRet == 1) // Single Selected
	{
		pCurTdna = (T_TDNA_BASE_501*)m_wndTendonDataList.GetItemData(arRet[0]);
		if(pCurTdna)
		{
			CString strTemp,strTemp1;
			
			if(pCurTdna->nInputOption == 1) // ABS
			{
				m_nPropileOpt = 0;
				strTemp1.Format(_T("%.8g"),pCurTdna->dCoord[0]);
				strTemp += strTemp1;
				strTemp1.Format(_T(", %.8g"),pCurTdna->dCoord[1]);
				strTemp += strTemp1;
				strTemp1.Format(_T(", %.8g"),pCurTdna->dCoord[2]);
				strTemp += strTemp1;
								
				m_wndABSCoord.SetWindowText(strTemp);

				m_bXVector = pCurTdna->bXYZ[0];
				m_bYVector = pCurTdna->bXYZ[1];
				m_bZVector = pCurTdna->bXYZ[2];

				if(m_bXVector == TRUE && m_bYVector == FALSE && m_bZVector == FALSE)  
					strTemp1.Format(_T("%.8g"),pCurTdna->dDirVec[0]);
				else if(m_bXVector == FALSE && m_bYVector == TRUE && m_bZVector == FALSE)
					strTemp1.Format(_T("%.8g"),pCurTdna->dDirVec[1]);
				else if(m_bXVector == FALSE && m_bYVector == FALSE && m_bZVector == TRUE)
					strTemp1.Format(_T("%.8g"),pCurTdna->dDirVec[2]);
				else if(m_bXVector == TRUE && m_bYVector == TRUE && m_bZVector == FALSE)
					strTemp1.Format(_T("%.8g, %.8g"),pCurTdna->dDirVec[0],pCurTdna->dDirVec[1]);
				else if(m_bXVector == FALSE && m_bYVector == TRUE && m_bZVector == TRUE)
					strTemp1.Format(_T("%.8g, %.8g"),pCurTdna->dDirVec[1],pCurTdna->dDirVec[2]);
				else if(m_bXVector == TRUE && m_bYVector == FALSE && m_bZVector == TRUE)
					strTemp1.Format(_T("%.8g, %.8g"),pCurTdna->dDirVec[0],pCurTdna->dDirVec[2]);
				else if(m_bXVector == TRUE && m_bYVector == TRUE && m_bZVector == TRUE)
					strTemp1.Format(_T("%.8g, %.8g, %.8g"),pCurTdna->dDirVec[0],pCurTdna->dDirVec[1],pCurTdna->dDirVec[2]);
				else strTemp1.Empty();

				strTemp = strTemp1;

				if(pCurTdna->bXYZ[0] || pCurTdna->bXYZ[1] || pCurTdna->bXYZ[2])
				{
					m_bDirVector=TRUE;
				}
				else
				{
					m_bDirVector=FALSE;
				}

				SetDirVectorMode();
				// ## 순서에 유의할것.....
				//m_wndABSDVector.SetWindowText(strTemp);
				//m_wndABSDVector.SetKeyPressedFlag(TRUE);
				m_wndABSDVector.MySetWindowText(strTemp);
			}
			else // REL
			{
				m_nPropileOpt = 1;

				CString strTemp;
				
				strTemp.Format(_T("%d"),pCurTdna->RefElemKey);
				m_wndRefElem.SetWindowText(strTemp);

						
				
				if(pCurTdna->nDistOpt == 0)
					this->m_nDistFromIOpt = 1;
				else
					this->m_nDistFromIOpt = 0;

				strTemp.Format(_T("%.8g"),pCurTdna->dDistI);
				m_wndDistFromINode.SetWindowText(strTemp);


				strTemp.Format(_T("%.8g"),pCurTdna->dEccY);
				m_wndEY.SetWindowText(strTemp);
				strTemp.Format(_T("%.8g"),pCurTdna->dEccZ);
				m_wndEZ.SetWindowText(strTemp);

				m_bDY = pCurTdna->bAlpha;
				m_bDZ = pCurTdna->bAlpha;

				if(m_bDY)
				{
					strTemp.Format(_T("%.8g"),pCurTdna->dAlphaY);
					m_wndDY.SetWindowText(strTemp);
				}
				else
				{
					m_wndDY.SetWindowText(_T("0"));
				}
				
				if(m_bDZ)
				{
					strTemp.Format(_T("%.8g"),pCurTdna->dAlphaZ);
					m_wndDZ.SetWindowText(strTemp);
				}
				else
				{
					m_wndDZ.SetWindowText(_T("0"));
				}
			}
			UpdateData(FALSE);
			AbsRelCtrlMan();
		}
	}
	else
	{
	
	}
}

void CCMTendonAddDlg::OnClickCmdTendonDataList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ListSelOperation();
	
	*pResult = 0;
}

void CCMTendonAddDlg::OnEnddragCmdTendonDataList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ListSelOperation();
}

/*
void CCMTendonAddDlg::OnKeydownCmdTendonDataList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	
	ListSelOperation();
	
	*pResult = 0;
}
*/

void CCMTendonAddDlg::OnOdstatechangedCmdTendonDataList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVODSTATECHANGE* pStateChanged = (NMLVODSTATECHANGE*)pNMHDR;
	
	ListSelOperation();
	
	*pResult = 0;
}


LRESULT CCMTendonAddDlg::OnListCtrlKeyUp(WPARAM wParam, LPARAM lParam)
{
	ListSelOperation();
	return 0L;
}


void CCMTendonAddDlg::OnCmdPrioUpButton() 
{
	CArray<int,int> arRet;
	CArray<int,int> arSet;
	CDlgUtil::GetListItemByMask(&m_wndTendonDataList,arRet,LVIS_SELECTED);
	if(arRet.GetSize() > 1) return;

	T_TDNA_BASE_501 TdnaB1,TdnaB2;
	
	if(arRet.GetSize() == 0) return;

	int nItem = m_wndTendonDataList.GetItemCount();
	if(nItem == 1) return;

	if(arRet[0] >= 1)
	{
		TdnaB1 = m_TdnaData.aProfile[arRet[0]];
		TdnaB2 = m_TdnaData.aProfile[arRet[0]-1];
		m_TdnaData.aProfile.SetAt(arRet[0]-1,TdnaB1);
		m_TdnaData.aProfile.SetAt(arRet[0],TdnaB2);
		UpdateProfileList();
		
		m_wndTendonDataList.SetItemState(arRet[0]  ,0,LVIS_SELECTED);
		m_wndTendonDataList.SetItemState(arRet[0]  ,0,LVIS_FOCUSED);
		m_wndTendonDataList.SetItemState(arRet[0]-1  ,LVIS_FOCUSED ,LVIS_FOCUSED );
		m_wndTendonDataList.SetItemState(arRet[0]-1  ,LVIS_SELECTED,LVIS_SELECTED);
	}
}

void CCMTendonAddDlg::OnCmdPrioDownButton() 
{
	CArray<int,int> arRet;
	CArray<int,int> arSet;
	CDlgUtil::GetListItemByMask(&m_wndTendonDataList,arRet,LVIS_SELECTED);
	
	if(arRet.GetSize() == 0) return;
	
	T_TDNA_BASE_501 TdnaB1,TdnaB2;
	
	int nItem = m_wndTendonDataList.GetItemCount();

	if(nItem == 1) return;
	if(arRet[0] < nItem-1)
	{
		TdnaB1 = m_TdnaData.aProfile[arRet[0]];
		TdnaB2 = m_TdnaData.aProfile[arRet[0]+1];
		m_TdnaData.aProfile.SetAt(arRet[0]+1,TdnaB1);
		m_TdnaData.aProfile.SetAt(arRet[0]  ,TdnaB2);
		UpdateProfileList();
	 
		m_wndTendonDataList.SetItemState(arRet[0]  ,0,LVIS_SELECTED);
		m_wndTendonDataList.SetItemState(arRet[0]  ,0,LVIS_FOCUSED);
		m_wndTendonDataList.SetItemState(arRet[0]+1,LVIS_FOCUSED,LVIS_FOCUSED);
		m_wndTendonDataList.SetItemState(arRet[0]+1,LVIS_SELECTED,LVIS_SELECTED);
	}
}



void CCMTendonAddDlg::OnDeltaposCmdRelAlphaYSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetFloatSpinValue(this,IDC_CMD_DY_EDIT,_T("U"),_T("U"),1., pNMHDR,pResult);
}

void CCMTendonAddDlg::OnDeltaposCmdRelAlphaZSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetFloatSpinValue(this,IDC_CMD_DZ_EDIT,_T("U"),_T("U"),1., pNMHDR,pResult);
}

BOOL __bBefore = FALSE;
void CCMTendonAddDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		__bBefore = TRUE;
//    UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_AFTER:
		__bBefore = FALSE;
		UpdateBuffer();
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
		DoStageChange();
		break;
	case D_UPDATE_UNIT:
		OnUnitChange();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}

}


void CCMTendonAddDlg::DoStageChange()
{
	if(m_TdnaKey != 0)
	{
		T_TDNA_D_501 TdnaData;
		
		//*^^* 수정됨 by jbseon
		//if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdna(m_TdnaKey,TdnaData))
		{
			OnCmdCancel();
		}
	}
	else
	{
		/*
		T_TDNA_BASE_501 TdnaB;
		T_TDNA_BASE_501::nInputOption; // 0:Relative, 1:Absolute(dialog에서만 사용)    
		
		int nProfile = m_TdnaData.aProfile.GetSize();
		
		for(int i = 0; i < nProfile ; i++)
		{
			TdnaB = m_TdnaData.aProfile[i];
		}
		
		UpdateProfileList(); 
		*/
	}
}


void CCMTendonAddDlg::UpdateBuffer()
{
	CString StrText;
	m_wndAssignElem.GetWindowText(StrText);
	CArray<int,int> aElem;
	if(!CStrParser::ParsingListByTo(StrText,aElem))
	{
		;
	}
	int nProfile = m_TdnaData.aProfile.GetSize();

	CMap<T_ELEM_K,T_ELEM_K,T_ELEM_K,T_ELEM_K> ElemSel;
	ElemSel.InitHashTable(50);

	int nSel = aElem.GetSize();
	for(int i = 0; i < nSel; i++)
	{
		ElemSel.SetAt(aElem[i],aElem[i]);
	}

	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	T_ELEM_K MfdElemK,MfsElemK,DelElemK;//AddElemK
	T_ELEM_D ElemD;
	T_NODE_K MfsNodeK;
	T_NODE_D NodeD;
	BOOL bElemModify = FALSE;

	T_TDNA_K DelTdnaK = 0;
	T_TDNA_D_501 DelTdnaD;
	T_TDNA_K AddTdnaK = 0;
	T_TDNA_D_501 AddTdnaD;

	BOOL bCurTdnaDeleted = FALSE;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_TDNA_DEL):
			{
				if(m_TdnaKey != 0)
				{
					//*^^* 수정됨 by jbseon
					//pViewBuff->GetTdna(nKey,DelTdnaK,DelTdnaD);
					
					if(m_TdnaKey == DelTdnaK)
						bCurTdnaDeleted = TRUE;
				}
			}
			break;
		case(UR_TDNA_ADD):
			{
				if(m_TdnaKey != 0)
				{
					//*^^* 수정됨 by jbseon
					//pViewBuff->GetTdna(nKey,AddTdnaK,AddTdnaD);

					if(bCurTdnaDeleted)
					{
						if(m_TdnaKey == AddTdnaK)
							bCurTdnaDeleted = FALSE;
					}
				}

			}
			break;
		case(UR_ELEM_MFD): // 뭐로 
			{
				//if(m_TdnaKey == 0)
				{
					pViewBuff->GetElem(nKey,MfdElemK,ElemD);
					//GDebugOut(_T("UR_ELEM_MFD %d"),MfdElemK);
				}
				/*
				else
				{
					bElemModify = TRUE;
					goto End;
				}
				*/
			}
			break; 
		case(UR_NODE_MFS):
			{
				pViewBuff->GetNode(nKey,MfsNodeK,NodeD);
				CArray<T_ELEM_K, T_ELEM_K> TElemList;
				CDBDoc::GetDocPoint()->m_pAttrCtrl->GetConnectedElem(MfsNodeK,TElemList);
				int nElem = TElemList.GetSize();
				if(nElem)
				{
					for(int i = 0; i < nProfile; i++)
					{
						if(m_TdnaData.aProfile[i].nInputOption == 0) // Relative 
						{
							for(int J = 0; J < nElem; J++)
							{
								if(m_TdnaData.aProfile[i].RefElemKey == TElemList[J]) 
								{
									bElemModify = TRUE;
								}
							}
						}
					}
				}
			}
			break;
		case(UR_ELEM_MFS): // 뭐가 
			{
				//if(m_TdnaKey == 0)
				{
					pViewBuff->GetElem(nKey,MfsElemK,ElemD);
					//GDebugOut(_T("UR_ELEM_MFS %d"),MfsElemK);
					if(MfdElemK != MfsElemK)
					{
						T_ELEM_K TEKey;
						if(ElemSel.Lookup(MfsElemK,TEKey))
						{
							ElemSel.RemoveKey(MfsElemK);
							ElemSel.SetAt(MfdElemK,MfdElemK);
							bElemModify = TRUE;
						}
						for(int  i = 0; i < nProfile; i++)
						{
							if(m_TdnaData.aProfile[i].nInputOption == 0) // Relative 
							{
								if(m_TdnaData.aProfile[i].RefElemKey == MfsElemK) 
								{
									m_TdnaData.aProfile[i].RefElemKey = MfdElemK;
									bElemModify = TRUE;
								}
							}
						}
					}
				}
				/*
				else
				{
					bElemModify = TRUE;
					goto End;
				}
				*/
			}
			break;
		case(UR_ELEM_ADD):
			{
				/*
				if(m_TdnaKey == 0)
				{
					pViewBuff->GetElem(nKey,AddElemK,ElemD);
					//GDebugOut(_T("UR_ELEM_ADD %d"),AddElemK);
				}
				else
				{
					bElemModify = TRUE;
					goto End;
				}
				*/
			}
			break;
		case(UR_ELEM_DEL):
			{
				//if(m_TdnaKey == 0)
				{
					T_ELEM_K TEKey;
					pViewBuff->GetElem(nKey,DelElemK,ElemD);
					//GDebugOut(_T("UR_ELEM_DEL %d"),DelElemK);
					if(ElemSel.Lookup(DelElemK,TEKey))
					{
						ElemSel.RemoveKey(DelElemK);
						bElemModify = TRUE;
					}
					
					for(int  i = 0; i < nProfile; i++)
					{
						if(m_TdnaData.aProfile[i].nInputOption == 0) // Relative 
						{
							if(m_TdnaData.aProfile[i].RefElemKey == DelElemK) 
							{
								m_TdnaData.aProfile[i].RefElemKey = 0;
								bElemModify = TRUE;
							}  
						}
					}
				}
				/*
				else
				{
					bElemModify = TRUE;
					goto End;
				}
				*/
			}
			break;
		default:
			break;
		}
	} 
//End:
	
	if(bCurTdnaDeleted)
	{
		OnCmdCancel(); 
	}
	else
	{
		if(bElemModify)
		{
			//if(m_TdnaKey == 0)
			{
				POSITION Pos = ElemSel.GetStartPosition();
				CString StrSel,StrTemp;
				T_ELEM_K rKey,rValue;
				int nIndex = 0;
				CArray<T_ELEM_K,T_ELEM_K> arSel;
				while(Pos)
				{
					ElemSel.GetNextAssoc(Pos,rKey,rValue);
					arSel.Add(rKey);
					if(nIndex == 0)
						StrTemp.Format(_T("%d"),rKey);
					else
						StrTemp.Format(_T(",%d"),rKey);
					StrSel += StrTemp;
				}
				
				if(arSel.GetSize())
					CDBDoc::GetDocPoint()->m_pViewCtrl->SelectElem(NULL,arSel,FALSE,TRUE);
			}
			/*
			else
			{
				//CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
				SetTDNA(m_TdnaKey);
				CDBDoc::GetDocPoint()->m_pViewCtrl->SelectElem(NULL,m_TdnaData.aElemList,FALSE,TRUE);
				
			}
			*/
			UpdateProfileList();
			//m_wndTendonDataList.SetItemState(arRet[0]  ,0,LVIS_SELECTED);
			//m_wndTendonDataList.SetItemState(arRet[0]  ,0,LVIS_FOCUSED);
			m_wndTendonDataList.SetItemState(0  ,LVIS_FOCUSED ,LVIS_FOCUSED );
			m_wndTendonDataList.SetItemState(0  ,LVIS_SELECTED,LVIS_SELECTED);
			ListSelOperation(); 
		}
	}
}

void CCMTendonAddDlg::OnUnitChange()
{
#define CONVERT(X) X=CDBDoc::GetDocPoint()->ConvertUnitLengthPrevious(X)
	T_TDNA_BASE_501 TdnaB;
	
	CONVERT(m_TdnaData.dBeginLength);
	CONVERT(m_TdnaData.dEndLength);
	
	int nProfile = m_TdnaData.aProfile.GetSize();
	
	for(int i = 0; i < nProfile; i++)
	{
		TdnaB = m_TdnaData.aProfile[i];
		CONVERT(TdnaB.dCoord[0]);
		CONVERT(TdnaB.dCoord[1]);
		CONVERT(TdnaB.dCoord[2]);
		if(TdnaB.nDistOpt==1) // 0:Relative, 1:Absolute(dialog에서만 사용)
		{
			CONVERT(TdnaB.dDistI);
		}
		
		CONVERT(TdnaB.dEccY );
		CONVERT(TdnaB.dEccZ );
		
		CONVERT(TdnaB.dDirVec[0]);
		CONVERT(TdnaB.dDirVec[1]);
		CONVERT(TdnaB.dDirVec[2]);

		m_TdnaData.aProfile.SetAt(i,TdnaB);
	}
	UpdateProfileList();
#undef CONVERT
}

LRESULT CCMTendonAddDlg::OnMouseEditEnter(WPARAM wParam, LPARAM lParam)
{
	/*
	// lParam == HWND ==> Message를 보낸 CMouseEdit의 HWND ...
	if(m_wndCoordXYZ.GetSafeHwnd() == (HWND)lParam)
	{
		_NodeCreate CS;
		
		if(GetParameter(CS))
		{
			CButton* pExecButton = GetExecButton();
			if(pExecButton)
			{
				//Execute Button의 Push 효과를 낸다. 
			}
			
			//if(AddNode(CS))  ;
				//InitOption();
			AddNode(CS);
		}
	}
	*/
	if(m_wndABSCoord.GetSafeHwnd() == (HWND)lParam)
	{
		OnCmdAddButton();
	}
	return 0L;
}

LRESULT CCMTendonAddDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	/*
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(m_wndCoordXYZ.GetSafeHwnd() == (HWND)lParam)
	{
		if(pIGM->GetModelingModeST() == IG_STATE_NODE_ADD)
		{
			_NodeCreate CS;
			if(GetParameter(CS))
			{
				CButton* pExecButton = GetExecButton();
				if(pExecButton)
				{
				// Execute Button의 Push 효과를 낸다. 
				}
				pIGM->CursorHide();
				AddNode(CS);
				pIGM->CursorCurPointDraw();
				
				pIGM->SetModelingModeST(IG_STATE_INITALL);

				pIGM->SetModelingModeST(IG_STATE_NODE_ADD);
				//m_wndCoordXYZ.SetFocus();
			}
		}
	}
	*/
	if(m_wndABSCoord.GetSafeHwnd() == (HWND)lParam)
	{
		if(m_nPropileOpt == 0)
		{
			CArray<int,int> arRet;
			CDlgUtil::GetListItemByMask(&m_wndTendonDataList,arRet,LVIS_SELECTED);
			int nRet = arRet.GetSize();

			if(nRet ==0)
				OnCmdAddButton();
			else
				OnCmdModifyButton();
		}
		
		m_wndABSCoord.SetFocus();
	}
	return 0L;
}

void CCMTendonAddDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();

	//I_GENModelBase::GetCurMySelfST()->UnselectAllNode();
	//I_GENModelBase::GetCurMySelfST()->UnselectAllElem();
	if(CDBDoc::GetDocPoint())
		CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
}
