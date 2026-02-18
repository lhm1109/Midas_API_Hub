// ConcSectionCreateBeamRebar.cpp : implementation file

#include "stdafx.h"
#include "wg_dgn.h"
#include "ConcSectionCreateBeamRebar.h"
#include "ConcSectionMainBeamRebar.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_MsgDll.h"

#ifdef _DEBUG

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateBeamRebar dialog


CConcSectionCreateBeamRebar::CConcSectionCreateBeamRebar(CWnd* pParent /*=NULL*/, int nCurTab /*= 0*/)
	: CChildDialog(CConcSectionCreateBeamRebar::IDD, pParent)
{
	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	//{{AFX_DATA_INIT(CConcSectionCreateBeamRebar)
	m_nTypeofCreation = 0;
	m_strPointTypeCent = _T("0,0");
	m_strLineTypeStartP = _T("0,0");	
	m_strLineTypeEndP = _T("0,0");
	m_strCirTypeStartP = _T("0,0");	
	m_strCirTypeCent = _T("0,0");
	m_strCirTypeAngle = _T("360");
	m_strDiaP = _T("");
	m_strDiaL = _T("");
	m_strDiaC = _T("");
	//}}AFX_DATA_INIT

	m_pParent = (CConcSectionMainBeamRebar*)pParent;
	m_pRebarGrid = new CDgnCreateBeamRebarGrid();

	m_nCurTab = nCurTab;

	m_pIDVP = 0;

	m_wndPointTypeCentP  = 0;
	m_wndLineTypeEndP    = 0;
	m_wndLineTypeStartP  = 0;
	m_wndCirTypeCent     = 0;
	m_wndCirTypeStart    = 0;
	m_As.SetEditUnit(0.0);
}

void CConcSectionCreateBeamRebar::SetDlgViewPanel(I_DlgViewPanel* pIDVP)
{
	this->m_pIDVP = pIDVP;
}

CConcSectionCreateBeamRebar::~CConcSectionCreateBeamRebar()
{
	if(m_pRebarGrid != NULL)
	{
		delete m_pRebarGrid;
		m_pRebarGrid = NULL;
	}
}


void CConcSectionCreateBeamRebar::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcSectionCreateBeamRebar)
	DDX_Control(pDX,IDC_DGN_REBARDB_COMBO_P,m_CobxAreaDBP);
	DDX_Control(pDX,IDC_DGN_REBARDB_COMBO_L,m_CobxAreaDBL);
	DDX_Control(pDX,IDC_DGN_REBARDB_COMBO_C,m_CobxAreaDBC);
	DDX_Control(pDX,IDC_DGN_LENGTH_UNIT_STATIC5, m_wndLengthUnit5);
	DDX_Control(pDX,IDC_DGN_LENGTH_UNIT_STATIC4, m_wndLengthUnit4);
	DDX_Control(pDX,IDC_DGN_LENGTH_UNIT_STATIC3, m_wndLengthUnit3);
	DDX_Control(pDX,IDC_DGN_LENGTH_UNIT_STATIC2, m_wndLengthUnit2);
	DDX_Control(pDX,IDC_DGN_LENGTH_UNIT_STATIC, m_wndLengthUnit1);
	DDX_Control(pDX, IDC_DGN_LINE_TYPE_NUM_OF_REBAR_EDIT, m_edtLineTypeNumOfRebar);
	DDX_Control(pDX, IDC_DGN_CIR_TYPE_NUM_OF_REBAR_EDIT, m_editCirTypeNum);
	DDX_Radio(pDX,IDC_DGN_TYPE_POINT_RADIO, m_nTypeofCreation);
	DDX_Text(pDX,IDC_DGN_POINT_TYPE_CENT_EDIT, m_strPointTypeCent);
	DDX_Text(pDX,IDC_DGN_LINE_TYPE_STARTP_EDIT, m_strLineTypeStartP);
	DDX_Text(pDX,IDC_DGN_LINE_TYPE_ENDP_EDIT, m_strLineTypeEndP);
	DDX_Text(pDX,IDC_DGN_CIR_TYPE_STARTP_EDIT, m_strCirTypeStartP);
	DDX_Text(pDX,IDC_DGN_CIR_TYPE_CENT_EDIT, m_strCirTypeCent);
	DDX_Text(pDX,IDC_DGN_CIR_TYPE_ANGLE_EDIT, m_strCirTypeAngle);
	DDX_CBString(pDX, IDC_DGN_REBARDB_COMBO_P, m_strDiaP);
	DDX_CBString(pDX, IDC_DGN_REBARDB_COMBO_L, m_strDiaL);
	DDX_CBString(pDX, IDC_DGN_REBARDB_COMBO_C, m_strDiaC);
	DDX_Control(pDX, IDC_DGN_CREATE_I_AS_UNIT, m_AsUT);
	DDX_Control(pDX, IDC_DGN_CREATE_I_AS_EDIT, m_As);
	DDX_Control(pDX, IDC_DGN_POINT_TYPE_BETA_EDT, m_edtPointBeta);
	DDX_Control(pDX, IDC_DGN_LINE_TYPE_BETA_EDT, m_edtLineBeta);
	DDX_Control(pDX, IDC_DGN_CIR_TYPE_BETA_EDT, m_edtCirBeta);
	DDX_Control(pDX, IDC_DGN_REBAR_GRID, *m_pRebarGrid);

	//DDX_Control(pDX,IDC_DGN_POINT_TYPE_CENT_EDIT, m_wndPointTypeCentP);
	//DDX_Control(pDX,IDC_DGN_LINE_TYPE_ENDP_EDIT, m_wndLineTypeEndP);
	//DDX_Control(pDX,IDC_DGN_LINE_TYPE_STARTP_EDIT, m_wndLineTypeStartP);
	//DDX_Control(pDX,IDC_DGN_CIR_TYPE_CENT_EDIT, m_wndCirTypeCent);
	//DDX_Control(pDX,IDC_DGN_CIR_TYPE_STARTP_EDIT,m_wndCirTypeStart);
	//}}AFX_DATA_MAP
}

BOOL CConcSectionCreateBeamRebar::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	T_UNIT_INDEX CurUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurUnit);
	m_pDoc->m_pMatlDB->Create_RebarData(CurUnit.nBase_Length,CurUnit.nBase_Force,TRUE);

	m_arCtrlPType.RemoveAll();
	m_arCtrlLType.RemoveAll();
	m_arCtrlCType.RemoveAll();

	CDlgUtil::GetCtrlIDByIncRect(this,m_arCtrlPType,IDC_DGN_TYPE_POINT_CTRL_HOLDER,FALSE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_arCtrlLType,IDC_DGN_TYPE_LINEAR_CTRL_HOLDER,FALSE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_arCtrlCType,IDC_DGN_TYPE_CIRCULAR_CTRL_HOLDER,FALSE);

	m_edtLineTypeNumOfRebar.SetRange(0, 100);
	m_edtLineTypeNumOfRebar.SetInteger(TRUE);
	m_editCirTypeNum.SetRange(0, 100);
	m_editCirTypeNum.SetInteger(TRUE);

	ControlMan(TRUE);
	SetInitUnit();

	m_pRebarGrid->Initialize(&m_arData,this);
	
	m_pRebarGrid->MakeItemEx();
	
	CDBLib::GetRebarNameAtComboBox(&m_CobxAreaDBP, TRUE); // Concrete=TRUE, SRC=FASLE.
	CDBLib::GetRebarNameAtComboBox(&m_CobxAreaDBL, TRUE);
	CDBLib::GetRebarNameAtComboBox(&m_CobxAreaDBC, TRUE);

	if(m_pIDVP)
	{
		CArray<I_DlgViewPanel_MouseEdit*, I_DlgViewPanel_MouseEdit*> arWnd;

		if(m_pIDVP->AttachRebarEdit_Point(this, IDC_DGN_POINT_TYPE_CENT_EDIT,arWnd))
		{
			m_wndPointTypeCentP = arWnd[0];
		}
		
		if(m_pIDVP->AttachRebarEdit_Circle(this, IDC_DGN_CIR_TYPE_CENT_EDIT,IDC_DGN_CIR_TYPE_STARTP_EDIT,arWnd))
		{
			m_wndCirTypeCent  = arWnd[0];
			m_wndCirTypeStart = arWnd[1];
		}
		
		if(m_pIDVP->AttachRebarEdit_Line(this, IDC_DGN_LINE_TYPE_STARTP_EDIT,IDC_DGN_LINE_TYPE_ENDP_EDIT,arWnd))
		{
			m_wndLineTypeEndP    = arWnd[0];
	    m_wndLineTypeStartP  = arWnd[1];
		}

		m_pIDVP->PushRebarEventHandler(); // Interaction 기능에 대한 연결 
	}

	SetCreationType(); // Point, Line, Circle이냐 ...
	
	if     (m_nCurTab==0) RebarKey = m_pParent->m_pParent->GetMainRebarI_LastNum();
	else if(m_nCurTab==1) RebarKey = m_pParent->m_pParent->GetMainRebarM_LastNum();
	else                  RebarKey = m_pParent->m_pParent->GetMainRebarJ_LastNum();
	
	SetAsRebarArea();

	EnableDisableControls(m_pParent->m_bSameIMJ);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CConcSectionCreateBeamRebar, CChildDialog)
	//{{AFX_MSG_MAP(CConcSectionCreateBeamRebar)
	ON_BN_CLICKED(IDC_DGN_TYPE_POINT_RADIO, OnClickTypeRadioBtn)
	ON_EN_CHANGE(IDC_DGN_POINT_TYPE_CENT_EDIT, OnChangePointTypeParam)
	ON_EN_CHANGE(IDC_DGN_LINE_TYPE_STARTP_EDIT, OnChangeLineTypeParam)
	ON_EN_CHANGE(IDC_DGN_CIR_TYPE_CENT_EDIT, OnChangeCirTypeParam)	
	ON_BN_CLICKED(IDC_DGN_CREATE_BUTTON, OnDgnCreateButton)
	ON_BN_CLICKED(IDC_DGN_DELETE_BUTTON, OnDgnDeleteButton)
	ON_CBN_SELCHANGE(IDC_DGN_REBARDB_COMBO_P, OnSelchangeDgnRebardbCombo)
	ON_EN_CHANGE(IDC_DGN_LINE_TYPE_NUM_OF_REBAR_EDIT, OnChangeLineTypeDivNum)
	ON_EN_CHANGE(IDC_DGN_CIR_TYPE_NUM_OF_REBAR_EDIT, OnOnChangeCirTypeDivNum)
	ON_EN_CHANGE(IDC_DGN_CIR_TYPE_ANGLE_EDIT, OnChangeCirTypeAngle)
	ON_BN_CLICKED(IDC_DGN_TYPE_LINEAR_RADIO, OnClickTypeRadioBtn)
	ON_BN_CLICKED(IDC_DGN_TYPE_CIRCULAR_RADIO, OnClickTypeRadioBtn)
	ON_EN_CHANGE(IDC_DGN_LINE_TYPE_ENDP_EDIT, OnChangeLineTypeParam)
	ON_EN_CHANGE(IDC_DGN_CIR_TYPE_STARTP_EDIT, OnChangeCirTypeParam)
	ON_CBN_SELCHANGE(IDC_DGN_REBARDB_COMBO_L, OnSelchangeDgnRebardbCombo)
	ON_CBN_SELCHANGE(IDC_DGN_REBARDB_COMBO_C, OnSelchangeDgnRebardbCombo)
	ON_BN_CLICKED(IDC_DGN_REDRAW_BUTTON, OnDgnRedrawButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateBeamRebar message handlers

void CConcSectionCreateBeamRebar::ControlMan(BOOL bInit)
{
	CArray<UINT, UINT> aControls;
	CString strCodeName = CDBLib::GetConCodeName();
		
	if(bInit)
	{
		CWnd* pPHolder = GetDlgItem(IDC_DGN_TYPE_POINT_CTRL_HOLDER);
		CWnd* pLHolder = GetDlgItem(IDC_DGN_TYPE_LINEAR_CTRL_HOLDER);
		CWnd* pCHolder = GetDlgItem(IDC_DGN_TYPE_CIRCULAR_CTRL_HOLDER);
		CRect PHRect,LHRect,CHRect;
		pPHolder->GetWindowRect(PHRect);
		pLHolder->GetWindowRect(LHRect);
		pCHolder->GetWindowRect(CHRect);

		int DY_L = PHRect.top - LHRect.top;
		int DY_C = PHRect.top - CHRect.top;

		CDlgUtil::CtrlMoveDistY(this,m_arCtrlLType,DY_L,TRUE);
		CDlgUtil::CtrlMoveDistY(this,m_arCtrlCType,DY_C,TRUE);
	}

	if(m_nTypeofCreation == 0) // Point Type
	{
		CDlgUtil::CtrlShowHide(this,m_arCtrlPType,TRUE);
		CDlgUtil::CtrlShowHide(this,m_arCtrlLType,FALSE);
		CDlgUtil::CtrlShowHide(this,m_arCtrlCType,FALSE);

		aControls.Add(IDC_DGN_POINT_TYPE_BETA_STC);
		aControls.Add(IDC_DGN_POINT_TYPE_BETA_STC2);
		aControls.Add(IDC_DGN_POINT_TYPE_BETA_EDT);

		if(strCodeName == CONCODE_SNIP_2_05_03_84 || strCodeName == CONCODE_SP_35_13330_2011 || strCodeName == CONCODE_SNIP_2_05_03_84_MKS || strCodeName == CONCODE_SP_35_13330_2011_MKS)
		{
			CDlgUtil::CtrlShowHide(this, aControls, SW_SHOW);
		}
		else
		{
			CDlgUtil::CtrlShowHide(this, aControls, SW_HIDE);
		}
	}
	else if(m_nTypeofCreation == 1) // Line Type
	{
		CDlgUtil::CtrlShowHide(this,m_arCtrlPType,FALSE);
		CDlgUtil::CtrlShowHide(this,m_arCtrlLType,TRUE);
		CDlgUtil::CtrlShowHide(this,m_arCtrlCType,FALSE);

		aControls.Add(IDC_DGN_LINE_TYPE_BETA_STC);
		aControls.Add(IDC_DGN_LINE_TYPE_BETA_STC2);
		aControls.Add(IDC_DGN_LINE_TYPE_BETA_EDT);
		
		if(strCodeName == CONCODE_SNIP_2_05_03_84 || strCodeName == CONCODE_SP_35_13330_2011 || strCodeName == CONCODE_SNIP_2_05_03_84_MKS || strCodeName == CONCODE_SP_35_13330_2011_MKS)
		{
			CDlgUtil::CtrlShowHide(this, aControls, SW_SHOW);
		}
		else
		{
			CDlgUtil::CtrlShowHide(this, aControls, SW_HIDE);
		}
	}
	else if(m_nTypeofCreation == 2) // Area Type
	{
		CDlgUtil::CtrlShowHide(this,m_arCtrlPType,FALSE);
		CDlgUtil::CtrlShowHide(this,m_arCtrlLType,FALSE);
		CDlgUtil::CtrlShowHide(this,m_arCtrlCType,TRUE);

		aControls.Add(IDC_DGN_CIR_TYPE_BETA_STC);
		aControls.Add(IDC_DGN_CIR_TYPE_BETA_STC2);
		aControls.Add(IDC_DGN_CIR_TYPE_BETA_EDT);
		
		if(strCodeName == CONCODE_SNIP_2_05_03_84 || strCodeName == CONCODE_SP_35_13330_2011 || strCodeName == CONCODE_SNIP_2_05_03_84_MKS || strCodeName == CONCODE_SP_35_13330_2011_MKS)
		{
			CDlgUtil::CtrlShowHide(this, aControls, SW_SHOW);
		}
		else
		{
			CDlgUtil::CtrlShowHide(this, aControls, SW_HIDE);
		}
	}	
}

void CConcSectionCreateBeamRebar::OnClickTypeRadioBtn() 
{
	//데이타를 해당 타입에 맞게 세팅한다.
	//코드 작성해야함
	UpdateData(TRUE);

	ControlMan(FALSE);
 
	SetCreationType();

	EnableDisableControls(m_pParent->m_bSameIMJ);
}

void CConcSectionCreateBeamRebar::SetCreationType()
{
	if(m_nTypeofCreation == 0)
	{
		m_pIDVP->SetRebarFunc_Point();
	}
	else if(m_nTypeofCreation == 1)
	{
		m_pIDVP->SetRebarFunc_Line();
	}
	else if(m_nTypeofCreation == 2)
	{
		m_pIDVP->SetRebarFunc_Circle();
	}

	OnSelchangeDgnRebardbCombo();
}

BOOL CConcSectionCreateBeamRebar::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//Dlg2Data(&m_Data);
	return CChildDialog::DestroyWindow();
}

void CConcSectionCreateBeamRebar::SetInitUnit()
{
	m_wndLengthUnit5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndLengthUnit4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndLengthUnit3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndLengthUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndLengthUnit1.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_As.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsUT.SetUnitType(D_UNITSYS_BASE_AREA);

	m_edtPointBeta.SetUnitType(D_UNITSYS_NONE);
	m_edtLineBeta.SetUnitType(D_UNITSYS_NONE);
	m_edtCirBeta.SetUnitType(D_UNITSYS_NONE);
}

void CConcSectionCreateBeamRebar::OnDgnCreateButton() 
{	
	addDelMod = 0;

	if (!Dlg2Data()) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Input_data_error_));
		return;
	}

	UpdateBuffer();
	AddRebarDrawData();

	REBAR_KEY RebarK;
	T_RBGC_MABR MabrD;
	POSITION Pos = m_MabrDataList.GetStartPosition();
	BOOL bOldLock = m_pRebarGrid->LockUpdate(TRUE);
	while(Pos)
	{
		m_MabrDataList.GetNextAssoc(Pos,RebarK,MabrD);
		/*
		if(m_pDoc->m_pDataCtrl->AddRcbe(ElemK,RcbeD))
		{
			m_BeamGrid->SetValue
		} */
		
		m_pRebarGrid->SetCurrentMaRb(RebarK, MabrD);
	}
	m_pRebarGrid->LockUpdate(bOldLock);
	if (!bOldLock) m_pRebarGrid->Redraw();

	SetAsRebarArea();
}

void CConcSectionCreateBeamRebar::OnDgnDeleteButton() 
{
	CArray<long, long> selectKeyList;
	int selectKeyNum = 0;

	//그리드의 선택되어진 Row의 키들을 넘겨받음
	selectKeyNum = m_pRebarGrid->GetSelectedKeys(selectKeyList);

	if(selectKeyNum == 0)
		return;
	
	BOOL bOldLock = m_pRebarGrid->LockUpdate(TRUE);
	//그리드에 키에 해당하는 Row를 지운다.
	for(int i=0 ; i<selectKeyList.GetSize() ; i++)
	{
		//그리드의 해당 Row를 지운다
		m_pRebarGrid->DeleteRowByKey(selectKeyList[i]);
		//판넬뷰에서 해당 철근 그림을 지운다
		m_pParent->DeleteRebarData(selectKeyList[i], m_nCurTab);
	}
	m_pRebarGrid->LockUpdate(bOldLock);
	if (!bOldLock) m_pRebarGrid->Redraw();

	SetAsRebarArea();
}

void CConcSectionCreateBeamRebar::OnChangeLineTypeDivNum() //분할 개수 변경 되었을 때  
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CChildDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here	
	OnSelchangeDgnRebardbCombo();

}

void CConcSectionCreateBeamRebar::OnOnChangeCirTypeDivNum() //분할 개수 변경 되었을 때 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CChildDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	OnSelchangeDgnRebardbCombo();
}

void CConcSectionCreateBeamRebar::OnChangeCirTypeAngle() // Circle Type의 Arc Angle이 변경되었을때 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CChildDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	OnSelchangeDgnRebardbCombo();
}


void CConcSectionCreateBeamRebar::OnChangePointTypeParam() //Edit Control의 내용이 변경되었을 때 
{
	//---------------------------------------------------
	//KeyPressed 상태이면 화면을 Update하고 그렇지 않으면 
	//화면을 갱신하지 않는다. ...........................

	/*
	CWnd* m_wndPointTypeCentP ;
	CWnd*	m_wndLineTypeEndP   ;
	CWnd* m_wndLineTypeStartP ;
	CWnd*	m_wndCirTypeCent    ;
	CWnd* m_wndCirTypeStart   ;
	*/
	/*
	if(!m_wndPointTypeCentP->IsKeyPressed())
	{
		m_pIDVP->UpdateView();
		return;
	}
	*/
	
	OnSelchangeDgnRebardbCombo();
	
}
																											
void CConcSectionCreateBeamRebar::OnChangeLineTypeParam() //Edit Control의 내용이 변경되었을 때 
{
	//---------------------------------------------------
	//KeyPressed 상태이면 화면을 Update하고 그렇지 않으면 
	//화면을 갱신하지 않는다. ........................... 
	/*
	if(!(m_wndLineTypeStartP->IsKeyPressed() || m_wndLineTypeEndP->IsKeyPressed()))
	{
		m_pIDVP->UpdateView();
		return;
	}
	*/

	OnSelchangeDgnRebardbCombo();
}                  
																										 
void CConcSectionCreateBeamRebar::OnChangeCirTypeParam() //Edit Control의 내용이 변경되었을 때 
{
	/*
	//---------------------------------------------------
	//KeyPressed 상태이면 화면을 Update하고 그렇지 않으면 
	//화면을 갱신하지 않는다. ...........................

	if(!(m_wndCirTypeCent->IsKeyPressed() || m_wndCirTypeStart->IsKeyPressed()))
	{
		m_pIDVP->UpdateView();
		return;
	}
	*/
	
	OnSelchangeDgnRebardbCombo();
}

void CConcSectionCreateBeamRebar::OnSelchangeDgnRebardbCombo()  //Diameter Combo box 변경 
{	
	UpdateData(TRUE);
	int Index;
	_DGN_REBAR_DATA RebarData;
	CString RebarName;
	if(m_nTypeofCreation == 0) // Point Type
	{
		Index = m_CobxAreaDBP.GetCurSel();
		if(Index!=-1)
			m_CobxAreaDBP.GetLBText(Index, RebarName);
		else 
			return;
		RebarData = m_pDoc->m_pMatlDB->Get_RebarData(RebarName);
		double RebarArea = 3.14159265358979323846 * (RebarData.dDia/2) * (RebarData.dDia/2);

		m_pIDVP->SetRebarParam_Point(RebarArea);
	}
	else if (m_nTypeofCreation == 1) // Line Type
	{
		int nRNum = m_edtLineTypeNumOfRebar.GetEditValueInt();
		
		Index = m_CobxAreaDBL.GetCurSel();
		if(Index!=-1)
			m_CobxAreaDBL.GetLBText(Index, RebarName);
		else 
			return;
		RebarData = m_pDoc->m_pMatlDB->Get_RebarData(RebarName);
		double RebarArea = 3.14159265358979323846 * (RebarData.dDia/2) * (RebarData.dDia/2);

		m_pIDVP->SetRebarParam_Line(RebarArea,nRNum);
	}
	else if (m_nTypeofCreation == 2) // Circle Type
	{
		int nRNum = m_editCirTypeNum.GetEditValueInt();

		Index = m_CobxAreaDBC.GetCurSel();
		if(Index!=-1)
			m_CobxAreaDBC.GetLBText(Index, RebarName);
		else 
			return;
		RebarData = m_pDoc->m_pMatlDB->Get_RebarData(RebarName);
		double RebarArea = 3.14159265358979323846 * (RebarData.dDia/2) * (RebarData.dDia/2);

		double AngleArc= _ttoi(m_strCirTypeAngle);

		m_pIDVP->SetRebarParam_Circle(RebarArea,AngleArc,nRNum);
	}
	//
	//Type별로 Diameter 변경 을 반영한다.  
	//
	// PI*R*R 로 계산 해서 넣어줄것. 
	

}


BOOL CConcSectionCreateBeamRebar::Data2Dlg()
{
	/*
	m_Dt.SetEditUnit(0.);
	m_Db.SetEditUnit(0.);

	m_iRbarRatio = m_Data.iRhoStand;
	m_cboCrackWidth.SetCurSel(m_Data.iServCond);
	m_bIncludeSbar = m_Data.bStirrup;
	m_Dt.SetEditUnit(m_Data.dDt);
	m_Db.SetEditUnit(m_Data.dDb);
	m_strRemark = m_Data.strRemark;

	return TRUE;

	UpdateData(FALSE);
	*/

	return TRUE;
}

BOOL CConcSectionCreateBeamRebar::Dlg2Data()
{	
	UpdateData(TRUE);

	int Index;
	m_MabrData.Initialize();

	RebarKey = m_pRebarGrid->GetLastRebarKey()+1;
	
	if(m_nTypeofCreation == 0) // Point Type
	{
		//m_MabrDataList.InitHashTable(max(1, 1));
		m_MabrDataList.RemoveAll();

		double X = 0.0;
		double Y = 0.0;
		double Z = 0.0;

		//if(!m_wndPointTypeCentP.GetCoordData(&Y, &Z, &X)) return FALSE;
		DV_3dp Pos;
		if(!m_pIDVP->GetRebarResultPos_Point(Pos)) return FALSE;
		
		X = Pos.x(); Y = Pos.y(); Z = Pos.z();
		

		m_MabrData.dCenterY = X;
		m_MabrData.dCenterZ = Y;
		m_MabrData.nLabel = RebarKey;
		Index = m_CobxAreaDBP.GetCurSel();
		if(Index!=-1) m_CobxAreaDBP.GetLBText(Index,m_MabrData.strRebarName);
		m_MabrData.dBeta = m_edtPointBeta.GetEditValue();
		m_MabrDataList.SetAt(RebarKey++, m_MabrData);
	}
	else if(m_nTypeofCreation == 1) // Line Type
	{
		m_MabrDataList.RemoveAll();    
		
		CArray<DV_3dp, DV_3dp&> arPoses;
		if(!m_pIDVP->GetRebarResultPos_Line(arPoses)) return FALSE;

		for(int i=0 ; i<arPoses.GetSize() ; i++)
		{
			m_MabrData.dCenterY = arPoses[i].x();
			m_MabrData.dCenterZ = arPoses[i].y();
			m_MabrData.nLabel = RebarKey;
			Index = m_CobxAreaDBL.GetCurSel();
			if(Index!=-1) m_CobxAreaDBL.GetLBText(Index,m_MabrData.strRebarName);
			m_MabrData.dBeta = m_edtLineBeta.GetEditValue();
			m_MabrDataList.SetAt(RebarKey++, m_MabrData);
		}		
	}
	else if(m_nTypeofCreation == 2) // Area Type (Circle)
	{
		m_MabrDataList.RemoveAll();

		CArray<DV_3dp, DV_3dp&> arPoses;
		if(!m_pIDVP->GetRebarResultPos_Circle(arPoses)) return FALSE;

		for(int i=0 ; i<arPoses.GetSize() ; i++)
		{
			m_MabrData.dCenterY = arPoses[i].x();
			m_MabrData.dCenterZ = arPoses[i].y();
			m_MabrData.nLabel = RebarKey;
			Index = m_CobxAreaDBC.GetCurSel();
			if(Index!=-1) m_CobxAreaDBC.GetLBText(Index,m_MabrData.strRebarName);
			m_MabrData.dBeta = m_edtCirBeta.GetEditValue();
			m_MabrDataList.SetAt(RebarKey++, m_MabrData);
		}
	}

	return TRUE;
}

void CConcSectionCreateBeamRebar::UpdateBuffer()
{
	REBAR_KEY    Key;
	T_RBGC_MABR    Data;

	BOOL bOldLock = m_pRebarGrid->LockUpdate(TRUE);

	POSITION Pos = m_MabrDataList.GetStartPosition();
	while(Pos)
	{
		if(addDelMod == 0)
		{
			m_MabrDataList.GetNextAssoc(Pos,Key,Data);	
			m_pRebarGrid->InsertRecordByKey(Key, Data);
		}
		else if(addDelMod == 1)
		{
			m_MabrDataList.GetNextAssoc(Pos,Key,Data);	
			m_pRebarGrid->DeleteRecordByKey(Key, Data);
			m_pRebarGrid->InsertRecordByKey(Key, Data);
		}
		else if(addDelMod == 2)
		{
			m_MabrDataList.GetNextAssoc(Pos,Key,Data);	
			m_pRebarGrid->DeleteRecordByKey(Key, Data);
		}
	}

	//int row = m_pRebarGrid->GetRowCount();
	//m_pRebarGrid->SetRowCount(row-1);
	
	/*
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_STLD_K cKey;
	m_BeamGrid->GetCurrentCell(ncRow, ncCol);
	m_BeamGrid->GetColumnNoFor(m_BeamGrid->GetKeyColID(), nkCol);
	cKey = _ttol(m_BeamGrid->GetValueRowCol(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_ELEM_K    Key, KeyBak;
	T_RCBE_D    Data, DataBak;

	BOOL bOldLock = m_BeamGrid->LockUpdate(TRUE);

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STLD_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				//pViewBuff->GetStld(nKey, Key, Data);
				m_BeamGrid->InsertRecordByKey(Key, Data);
				break;
		case(UR_STLD_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				//pViewBuff->GetStld(nKey, Key, Data);
				m_BeamGrid->DeleteRecordByKey(Key, Data);
				break;
		case(UR_STLD_MFD):
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				//pViewBuff->GetStld(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_STLD_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				//pViewBuff->GetStld(nKey, Key, Data);
				m_BeamGrid->DeleteRecordByKey(Key, Data);
				m_BeamGrid->InsertRecordByKey(KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

	*/

	ROWCOL ncRow, ncCol, nkCol;
	REBAR_KEY cKey;
	m_pRebarGrid->GetCurrentCell(ncRow, ncCol);
	//m_pRebarGrid->SetCurrentCell(--ncRow, ncCol);
	m_pRebarGrid->GetColumnNoFor(m_pRebarGrid->GetKeyColID(), nkCol);
	cKey = _ttol(m_pRebarGrid->GetValueRowCol(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	// 원래 row, col을 복구하려고 시도
	unsigned int nOldMode = m_pRebarGrid->m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_pRebarGrid->m_nEditMode = CTBBrowserWnd::noMode;
		if (m_pRebarGrid->GetRowCount() < ncRow) ncRow = m_pRebarGrid->GetRowCount();
		BOOL bSet = m_pRebarGrid->SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = m_pRebarGrid->SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = m_pRebarGrid->SetLeftValidCell(ncRow, ncCol);
		m_pRebarGrid->m_nEditMode = nOldMode;
	}

	m_pRebarGrid->GetCurrentCell(ncRow, ncCol);
	m_pRebarGrid->GetColumnNoFor(m_pRebarGrid->GetKeyColID(), nkCol);
	T_STLD_K newKey = _ttol(m_pRebarGrid->GetValueRowCol(ncRow, nkCol));
	if (newKey != cKey) m_pRebarGrid->OnChangeCurrentRecord(ncRow);

	m_pRebarGrid->AdjustAppendRowNo();
	m_pRebarGrid->LockUpdate(bOldLock);
	if (!bOldLock) m_pRebarGrid->Redraw();  
}


void CConcSectionCreateBeamRebar::AddRebarDrawData() 
{
	REBAR_KEY    Key;
	T_RBGC_MABR    Data;

	POSITION Pos = m_MabrDataList.GetStartPosition();
	while(Pos)
	{
		m_MabrDataList.GetNextAssoc(Pos,Key,Data);	
		m_pParent->AddRebarData(Key, Data, m_nCurTab);		
	}
}

double CConcSectionCreateBeamRebar::GetZ(double Y, double a, double b, double c, double d) 
{
	//기울기
	double m;
	double n;

	m = (d-b)/(c-a);
	n = b - (((d-b)/(c-a))*a);

	return (m*Y) + n;
}
void CConcSectionCreateBeamRebar::SetInitData(CArray<T_RBGC_MABR,T_RBGC_MABR&>& MainRebarDataList)
{
	m_MabrDataList.RemoveAll();
	RebarKey = 0;
	for(int i=0 ; i<MainRebarDataList.GetSize() ; i++)
	{
		m_MabrDataList.SetAt(i, MainRebarDataList[i]);
		RebarKey++;
	}

	addDelMod = 0;
	UpdateBuffer();
	AddRebarDrawData();

	Data2Dlg();
}

void CConcSectionCreateBeamRebar::SaveDlgData()
{
	Dlg2Data();
}

int CConcSectionCreateBeamRebar::GetComboRebarIndex(CString strRebarname)
{
	CStringArray arRebarNa;
	CDBLib::GetRebarNameListByCode(arRebarNa, TRUE);

	int i = 0;
	for(i=0 ; i<arRebarNa.GetSize() ; i++)
	{
		if(strRebarname == arRebarNa[i])
			break;
	}

	return i;
}

void CConcSectionCreateBeamRebar::RemoveMainRebarDataList(REBAR_KEY Key, T_RBGC_MABR Data)
{
	m_pRebarGrid->DeleteRecordByKey(Key, Data);
		
	m_MabrDataList.RemoveAll();	
}

void CConcSectionCreateBeamRebar::SetRebarListData(CMap<REBAR_KEY,REBAR_KEY,T_RBGC_MABR,T_RBGC_MABR>& m_MDataList)
{
	m_MabrDataList.RemoveAll();

	REBAR_KEY    Key;
	T_RBGC_MABR    Data;

	POSITION Pos = m_MDataList.GetStartPosition();
	while(Pos)
	{
		m_MDataList.GetNextAssoc(Pos,Key,Data);	
		m_MabrDataList.SetAt(Key, Data);
	}

	addDelMod = 0;
	UpdateBuffer();
	AddRebarDrawData();
}


void CConcSectionCreateBeamRebar::OnDgnRedrawButton() 
{
	if(!m_pRebarGrid->SaveGridData())
		return;
	
	REBAR_KEY Key;
	T_RBGC_MABR ReberData;
	POSITION Pos = m_pRebarGrid->m_MabrDataList.GetStartPosition();
	while(Pos)
	{
		m_pRebarGrid->m_MabrDataList.GetNextAssoc(Pos,Key,ReberData);
		m_pParent->m_pParent->ReDrawRebar(Key, ReberData, m_nCurTab);			
	}

	SetAsRebarArea();
}

void CConcSectionCreateBeamRebar::SetAsRebarArea() 
{
	double RebarArea = 0.;
	
	REBAR_KEY RKey;
	T_RBGC_MABR ReberData;

	if(m_nCurTab==0)
	{
		POSITION Pos = m_pParent->m_pParent->m_MabrDataListI.GetStartPosition();
		while(Pos)
		{
			m_pParent->m_pParent->m_MabrDataListI.GetNextAssoc(Pos,RKey,ReberData);
			RebarArea += m_pDoc->m_pMatlDB->Get_RebarArea(ReberData.strRebarName);			
		}
	}
	else if(m_nCurTab==1)
	{
		POSITION Pos = m_pParent->m_pParent->m_MabrDataListM.GetStartPosition();
		while(Pos)
		{
			m_pParent->m_pParent->m_MabrDataListM.GetNextAssoc(Pos,RKey,ReberData);
			RebarArea += m_pDoc->m_pMatlDB->Get_RebarArea(ReberData.strRebarName);			
		}
	}
	else
	{
		POSITION Pos = m_pParent->m_pParent->m_MabrDataListJ.GetStartPosition();
		while(Pos)
		{
			m_pParent->m_pParent->m_MabrDataListJ.GetNextAssoc(Pos,RKey,ReberData);
			RebarArea += m_pDoc->m_pMatlDB->Get_RebarArea(ReberData.strRebarName);			
		}
	}	
	
	m_As.SetEditUnit(RebarArea);
	UpdateData(FALSE);
}

void CConcSectionCreateBeamRebar::EnableDisableControls(BOOL bSameIMJ)
{
	if((m_nCurTab==1 || m_nCurTab==2) && bSameIMJ)
	{
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_STATIC,                    FALSE, TRUE);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_TYPE_POINT_CTRL_HOLDER2,   FALSE, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arCtrlPType, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arCtrlLType, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arCtrlCType, FALSE);

		GetDlgItem(IDC_DGN_REDRAW_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CREATE_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DELETE_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STATIC30        )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CREATE_I_AS_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CREATE_I_AS_UNIT)->EnableWindow(FALSE);			
	}
	else
	{
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_STATIC,                    TRUE, TRUE);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_TYPE_POINT_CTRL_HOLDER2,   TRUE, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arCtrlPType, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_arCtrlLType, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_arCtrlCType, TRUE);
		
		GetDlgItem(IDC_DGN_REDRAW_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CREATE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DELETE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STATIC30        )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CREATE_I_AS_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CREATE_I_AS_UNIT)->EnableWindow(TRUE);			
	}

	GetDlgItem(IDC_DGN_TYPE_POINT_CTRL_HOLDER   )->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_TYPE_LINEAR_CTRL_HOLDER  )->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_TYPE_CIRCULAR_CTRL_HOLDER)->ShowWindow(SW_HIDE);
}