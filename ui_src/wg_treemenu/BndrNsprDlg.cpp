// BndrNsprDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrNsprDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\ViewBuff.h"
#include "..\wg_cmd2\MlfcListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBndrNsprDlg dialog


CBndrNsprDlg::CBndrNsprDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrNsprDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrNsprDlg)
	m_nOption = 0; 
	//}}AFX_DATA_INIT
	m_arGen.Add(IDC_TM_NSPR_SDX_TITLE);
	m_arGen.Add(IDC_TM_NSPR_SDX);
	m_arGen.Add(IDC_TM_NSPR_SDX_UNIT);

	m_arGen.Add(IDC_TM_NSPR_SDY_TITLE);
	m_arGen.Add(IDC_TM_NSPR_SDY);
	m_arGen.Add(IDC_TM_NSPR_SDY_UNIT);

	m_arGen.Add(IDC_TM_NSPR_SDZ_TITLE);
	m_arGen.Add(IDC_TM_NSPR_SDZ);
	m_arGen.Add(IDC_TM_NSPR_SDZ_UNIT);

	m_arGen.Add(IDC_TM_NSPR_SRX_TITLE);
	m_arGen.Add(IDC_TM_NSPR_SRX);
	m_arGen.Add(IDC_TM_NSPR_SRX_UNIT);

	m_arGen.Add(IDC_TM_NSPR_SRY_TITLE);
	m_arGen.Add(IDC_TM_NSPR_SRY);
	m_arGen.Add(IDC_TM_NSPR_SRY_UNIT);

	m_arGen.Add(IDC_TM_NSPR_SRZ_TITLE);
	m_arGen.Add(IDC_TM_NSPR_SRZ);
	m_arGen.Add(IDC_TM_NSPR_SRZ_UNIT);

	m_arGen.Add(IDC_TM_NSPR_POINT_SPRING_FIXED_GRUP);
	m_arGen.Add(IDC_TM_NSPR_SDX_CHK);
	m_arGen.Add(IDC_TM_NSPR_SDY_CHK);
	m_arGen.Add(IDC_TM_NSPR_SDZ_CHK);
	m_arGen.Add(IDC_TM_NSPR_SRX_CHK);
	m_arGen.Add(IDC_TM_NSPR_SRY_CHK);
	m_arGen.Add(IDC_TM_NSPR_SRZ_CHK);

	m_arGen.Add(IDC_TM_NSPR_DAMPING_CHK);
	m_arGen.Add(IDC_TM_NSPR_CX_TITLE);
	m_arGen.Add(IDC_TM_NSPR_CX);
	m_arGen.Add(IDC_TM_NSPR_CX_UNIT);
	m_arGen.Add(IDC_TM_NSPR_CY_TITLE);
	m_arGen.Add(IDC_TM_NSPR_CY);
	m_arGen.Add(IDC_TM_NSPR_CY_UNIT);
	m_arGen.Add(IDC_TM_NSPR_CZ_TITLE);
	m_arGen.Add(IDC_TM_NSPR_CZ);
	m_arGen.Add(IDC_TM_NSPR_CZ_UNIT);
	m_arGen.Add(IDC_TM_NSPR_RCX_TITLE);
	m_arGen.Add(IDC_TM_NSPR_RCX);
	m_arGen.Add(IDC_TM_NSPR_RCX_UNIT);
	m_arGen.Add(IDC_TM_NSPR_RCY_TITLE);
	m_arGen.Add(IDC_TM_NSPR_RCY);
	m_arGen.Add(IDC_TM_NSPR_RCY_UNIT);
	m_arGen.Add(IDC_TM_NSPR_RCZ_TITLE);
	m_arGen.Add(IDC_TM_NSPR_RCZ);
	m_arGen.Add(IDC_TM_NSPR_RCZ_UNIT);

	m_aDamping.Add(IDC_TM_NSPR_CX);
	m_aDamping.Add(IDC_TM_NSPR_CX_UNIT);
	m_aDamping.Add(IDC_TM_NSPR_CY);
	m_aDamping.Add(IDC_TM_NSPR_CY_UNIT);
	m_aDamping.Add(IDC_TM_NSPR_CZ);
	m_aDamping.Add(IDC_TM_NSPR_CZ_UNIT);
	m_aDamping.Add(IDC_TM_NSPR_RCX);
	m_aDamping.Add(IDC_TM_NSPR_RCX_UNIT);
	m_aDamping.Add(IDC_TM_NSPR_RCY);
	m_aDamping.Add(IDC_TM_NSPR_RCY_UNIT);
	m_aDamping.Add(IDC_TM_NSPR_RCZ);
	m_aDamping.Add(IDC_TM_NSPR_RCZ_UNIT);

	m_arCompTens.Add(IDC_TM_NSPR_STIFF_TITLE);
	m_arCompTens.Add(IDC_TM_NSPR_STIFF);
	m_arCompTens.Add(IDC_TM_NSPR_STIFF_UNIT);
	m_arCompTens.Add(IDC_TM_NSPR_DIRECT_TITLE);
	m_arCompTens.Add(IDC_TM_NSPR_DIRECT_CMB);
	m_arCompTens.Add(IDC_TM_NSPR_VECTOR);
	
	m_arMulti.Add(IDC_TM_ELNK_MLFC_STC);
	m_arMulti.Add(IDC_TM_ELNK_MLFC_COMBO);
	m_arMulti.Add(IDC_TM_ELNK_MLFC_BUTTON);
	m_arMulti.Add(IDC_TM_MLDIR_CMB);
	m_arMulti.Add(IDC_TM_MLDIR_STC);
	m_arMulti.Add(IDC_TM_MLVECTOR_EDT);

	// [PMS 5075] 파일강성계수 계산 편의기능
#if defined(_MGEN) && defined(_ORG)
	m_arGen.Add(IDC_TM_NSPR_SDZ_BTN);
	m_arCompTens.Add(IDC_TM_NSPR_STIFF_BTN);
#endif
	m_PsidD.Initialize();
}


void CBndrNsprDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrNsprDlg)
	DDX_Control(pDX, IDC_TM_TYPE_CMB, m_cmbType);
	DDX_Radio(pDX, IDC_TM_NSPR_OPT_ADD, m_nOption);
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Control(pDX, IDC_TM_NSPR_DIRECT_CMB, m_wndStiffCombo);
	DDX_Control(pDX, IDC_TM_NSPR_STIFF, m_wndStiffEdit);
	DDX_Control(pDX, IDC_TM_NSPR_STIFF_UNIT, m_wndStiffUnit);
	DDX_Control(pDX, IDC_TM_NSPR_VECTOR, m_wndVectorEdit);
	DDX_Control(pDX, IDC_TM_MLDIR_CMB, m_cmbMultiDir);
	DDX_Control(pDX, IDC_TM_MLVECTOR_EDT, m_edtMultiVector);  
	DDX_Check(pDX, IDC_TM_NSPR_SDX_CHK, m_chkSDx);
	DDX_Check(pDX, IDC_TM_NSPR_SDY_CHK, m_chkSDy);
	DDX_Check(pDX, IDC_TM_NSPR_SDZ_CHK, m_chkSDz);
	DDX_Check(pDX, IDC_TM_NSPR_SRX_CHK, m_chkSRx);
	DDX_Check(pDX, IDC_TM_NSPR_SRY_CHK, m_chkSRy);
	DDX_Check(pDX, IDC_TM_NSPR_SRZ_CHK, m_chkSRz);
	DDX_Check(pDX, IDC_TM_NSPR_DAMPING_CHK, m_chkDamping);

	DDX_Control(pDX, IDC_TM_ELNK_MLFC_COMBO, m_wndMlfccmbx);  

	//}}AFX_DATA_MAP
	UINT nID[6][2] = { 
		{IDC_TM_NSPR_SDX, IDC_TM_NSPR_SDX_UNIT},
		{IDC_TM_NSPR_SDY, IDC_TM_NSPR_SDY_UNIT},
		{IDC_TM_NSPR_SDZ, IDC_TM_NSPR_SDZ_UNIT},
		{IDC_TM_NSPR_SRX, IDC_TM_NSPR_SRX_UNIT},
		{IDC_TM_NSPR_SRY, IDC_TM_NSPR_SRY_UNIT},
		{IDC_TM_NSPR_SRZ, IDC_TM_NSPR_SRZ_UNIT},
	};
	for (int i = 0; i < 6; i++)
	{
	  DDX_Control(pDX, nID[i][0], m_Value[i]);
	  DDX_Control(pDX, nID[i][1], m_Unit[i]);
	}

	UINT nIDDamping[6][2] = { 
		{IDC_TM_NSPR_CX  , IDC_TM_NSPR_CX_UNIT},
		{IDC_TM_NSPR_CY  , IDC_TM_NSPR_CY_UNIT},
		{IDC_TM_NSPR_CZ  , IDC_TM_NSPR_CZ_UNIT},
		{IDC_TM_NSPR_RCX , IDC_TM_NSPR_RCX_UNIT},
		{IDC_TM_NSPR_RCY , IDC_TM_NSPR_RCY_UNIT},
		{IDC_TM_NSPR_RCZ , IDC_TM_NSPR_RCZ_UNIT},
	};
	for (int i = 0; i < 6; i++)
	{
		DDX_Control(pDX, nIDDamping[i][0], m_DampingValue[i]);
		DDX_Control(pDX, nIDDamping[i][1], m_DampingUnit[i]);
	}

	DDX_Control(pDX, IDC_TM_PICTURE, m_wndPicture);
}

CBndrNsprDlg::~CBndrNsprDlg()
{
}

void CBndrNsprDlg::ChangeBitmap()
{
	CString aBitmapPath[] = { 
		_T("SVG\\illustration\\Dialog\\tm_nspr.svg"),
		_T("SVG\\illustration\\Dialog\\tm_nspr_comp.svg"),
		_T("SVG\\illustration\\Dialog\\tm_nspr_vec.svg"),
		_T("SVG\\illustration\\Dialog\\tm_nspr_unsym.svg"),
		_T("SVG\\illustration\\Dialog\\tm_nspr_sym.svg")
	}; // need change - mylee

	int nBitmap = 0;
	int nType = m_cmbType.GetCurSel();
	if(nType == 0) nBitmap = 0; // linear
	else
	{
		if(nType==1 || nType==2)
		{
			int nDir = m_wndStiffCombo.GetCurSel();
			nBitmap = (nDir==6) ? 2 : 1; // comp, tens / vector
		}
		else
		{
			//int nMultiType = m_cmbMultiType.GetCurSel();
			//nBitmap = (nMultiType==0) ? 3 : 4; // unsymmetric / symmetric
		}
	}
	
	m_wndPicture.SetImage(aBitmapPath[nBitmap]);

	SetFlag();
}

BEGIN_MESSAGE_MAP(CBndrNsprDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrNsprDlg)
	ON_BN_CLICKED(IDC_TM_NSPR_OPT_ADD    , OnChangeOption)
	ON_CBN_SELCHANGE(IDC_TM_NSPR_DIRECT_CMB, OnSelchangeTmBndrStiffCbo)  
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_CBN_SELCHANGE(IDC_TM_MLDIR_CMB, OnSelchangeMultiDir)
	ON_BN_CLICKED(IDC_TM_NSPR_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_NSPR_OPT_DELETE , OnChangeOption)
	ON_CBN_SELCHANGE(IDC_TM_TYPE_CMB, OnSelchangeType)
	ON_BN_CLICKED(IDC_TM_NSPR_SDX_CHK, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_NSPR_SDY_CHK, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_NSPR_SDZ_CHK, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_NSPR_SRX_CHK, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_NSPR_SRY_CHK, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_NSPR_SRZ_CHK, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_NSPR_DAMPING_CHK, OnClickDamping)
	ON_BN_CLICKED(IDC_TM_NSPR_SDZ_BTN   , OnClickCalcBtn_SDZ)
	ON_BN_CLICKED(IDC_TM_NSPR_STIFF_BTN , OnClickCalcBtn_Stiff)
	ON_BN_CLICKED(IDC_TM_ELNK_MLFC_BUTTON, OnMlfcButton)  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrNsprDlg message handlers

BOOL CBndrNsprDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
#if !defined(_MGEN) || !defined(_ORG)
	GetDlgItem(IDC_TM_NSPR_SDZ_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_NSPR_STIFF_BTN)->ShowWindow(SW_HIDE);
#endif

	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	m_nOption = 0;
	
	// Init SubControls
	for (int i = 0; i < 6; i++)
	{
		m_Unit[i].SetUnitType(CUnitCtrl::m_NSPR_UNIT.Spring[i]);
		m_Value[i].SetUnitType(CUnitCtrl::m_NSPR_UNIT.Spring[i]);
	}
	
	for (int i = 0; i < 6; i++)
	{
		m_DampingUnit[i].SetUnitType(CUnitCtrl::m_NSPR_UNIT.dDamping[i]);
		m_DampingValue[i].SetUnitType(CUnitCtrl::m_NSPR_UNIT.dDamping[i]);
	}
			
	m_cmbType.ResetContent(); 
	m_cmbType.AddString(_LS(IDS_TM_WORKTREE_Linear));
	m_cmbType.AddString(_LS(IDS_WG_TREEMENU_Comp_Only));
	m_cmbType.AddString(_LS(IDS_WG_TREEMENU_Tens_Only));
	m_cmbType.AddString(_LS(IDS_TM_MULTI_LINEAR));
	m_cmbType.SetCurSel(0);
		
	m_cmbMultiDir.ResetContent();
	m_cmbMultiDir.AddString(_LS(IDS_TM_NSPR_Local_Plus_x));
	m_cmbMultiDir.AddString(_LS(IDS_TM_NSPR_Local_Minus_x));
	m_cmbMultiDir.AddString(_LS(IDS_TM_NSPR_Local_Plus_y));
	m_cmbMultiDir.AddString(_LS(IDS_TM_NSPR_Local_Minus_y));
	m_cmbMultiDir.AddString(_LS(IDS_TM_NSPR_Local_Plus_z));
	m_cmbMultiDir.AddString(_LS(IDS_TM_NSPR_Local_Minus_z));
	m_cmbMultiDir.AddString(_LS(IDS_TM_NSPR_Vector));
	m_cmbMultiDir.SetCurSel(0); 

	m_edtMultiVector.SetAttUcsDistance();
	m_edtMultiVector.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_edtMultiVector.EnableWindow(FALSE);
	// MNET:2458 20061110 mylee END

	m_wndStiffCombo.ResetContent();
	m_wndStiffCombo.AddString(_LS(IDS_TM_NSPR_Local_Plus_x));
	m_wndStiffCombo.AddString(_LS(IDS_TM_NSPR_Local_Minus_x));
	m_wndStiffCombo.AddString(_LS(IDS_TM_NSPR_Local_Plus_y));
	m_wndStiffCombo.AddString(_LS(IDS_TM_NSPR_Local_Minus_y));
	m_wndStiffCombo.AddString(_LS(IDS_TM_NSPR_Local_Plus_z));
	m_wndStiffCombo.AddString(_LS(IDS_TM_NSPR_Local_Minus_z));
	m_wndStiffCombo.AddString(_LS(IDS_TM_NSPR_Vector));
	int nCur =0;
	m_wndStiffCombo.SetCurSel(nCur);
	m_wndStiffEdit.SetUnitType(CUnitCtrl::m_NSPR_UNIT.dStiffness);
	m_wndStiffUnit.SetUnitType(CUnitCtrl::m_NSPR_UNIT.dStiffness);

	m_wndVectorEdit.SetAttUcsDistance();
	m_wndVectorEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndVectorEdit.EnableWindow(FALSE);
	
	RestoreMlfcCbx();

	//CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, nMethod);
	 
	UpdateData(FALSE);		
	
	// Align Controls
#if defined(_CIVIL) || defined(_MGEN) // MNET:2866 20070628 mylee
	CRect DestRect, SrcRect;
	int XDelta, YDelta;
	GetDlgItem(IDC_TM_NSPR_SDX_TITLE)->GetWindowRect(&DestRect);
	GetDlgItem(IDC_TM_NSPR_STIFF_TITLE)->GetWindowRect(&SrcRect);
	XDelta = DestRect.left - SrcRect.left;
	CDlgUtil::CtrlMoveDistX(this, m_arCompTens, XDelta);
	
	GetDlgItem(IDC_TM_PICTURE)->GetWindowRect(&DestRect);
	GetDlgItem(IDC_TM_ELNK_MLFC_STC)->GetWindowRect(&SrcRect);
	XDelta = DestRect.left - SrcRect.left;
	YDelta = DestRect.top - SrcRect.top;
	CDlgUtil::CtrlMoveDistXY(this, m_arMulti, XDelta, YDelta);

	// ShowHide Controls
	CDlgUtil::CtrlShowHide(this,m_arGen, TRUE);
	CDlgUtil::CtrlShowHide(this,m_arCompTens, FALSE);
	CDlgUtil::CtrlShowHide(this,m_arMulti, FALSE); // MNET:2458 20061110 mylee

#else
	GetDlgItem(IDC_TM_TYPE_STC)->ShowWindow(FALSE);
	GetDlgItem(IDC_TM_TYPE_CMB)->ShowWindow(FALSE);

	// ShowHide Controls
	CDlgUtil::CtrlShowHide(this,m_arGen, FALSE);
	CDlgUtil::CtrlShowHide(this,m_arCompTens, FALSE);
	CDlgUtil::CtrlShowHide(this,m_arMulti, FALSE); // MNET:2458 20061110 mylee

#endif
	
	// EnableDisable Controls
	OnSelchangeType(); 
	OnSelchangeTmBndrStiffCbo();
	OnSelchangeMultiDir();
	OnClickDamping();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrNsprDlg::Execute() 
{
	UpdateData(TRUE);
	CArray<T_NODE_K, T_NODE_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedNodeKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return;
	}
	T_NSPR_D data;
	data.Initialize();

	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	data.GroupKey = BngrK;
	int nType = m_cmbType.GetCurSel();
	data.nGenCompTen = nType;
	if(nType == 0) // Linear
	{
		data.nDirection = 0;
		for (int i = 0; i < 6; i++)
		{
			data.Spring[i] = m_Value[i].GetEditValue();
		}

		data.bFixed[0] = m_chkSDx;
		data.bFixed[1] = m_chkSDy;
		data.bFixed[2] = m_chkSDz;
		data.bFixed[3] = m_chkSRx;
		data.bFixed[4] = m_chkSRy;
		data.bFixed[5] = m_chkSRz;

		data.bDamping = m_chkDamping;
		for (int i = 0; i < 6; i++)
		{
			data.dDamping[i] = m_DampingValue[i].GetEditValue();
		}
	}
	else if (nType == 1 || nType == 2) // Comp and Tens
	{
		data.nDirection = m_wndStiffCombo.GetCurSel();  
		double dSelected;
		dSelected = m_wndStiffEdit.GetEditValue();
		data.dStiffness = dSelected; 
		CString csVector;
		double ux, uy, uz;
		if(data.nDirection == 6)
		{
			m_wndVectorEdit.GetWindowText(csVector);
			if (!GetPosition(csVector, ux, uy, uz))
			{
				 GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_vector_));
				 return;
			}
			pIGM->GetWcsVector(ux, uy, uz, data.dVx, data.dVy, data.dVz);
		}
	} 
	// MNET:2458 20061110 mylee
	else // MultiLinear 
	{ 
		// 사용 안하는 Data들은 기본값으로 채워 넣음
		data.nMultiLinearType = 0;    
		data.ax = 0;
		data.bx = 0;
		data.cx = 0;
		data.dx = 0;
		data.ex = 0;
		data.fx = 0;
		data.ay = 0;
		data.by = 0;
		data.cy = 0;
		data.dy = 0;
		data.ey = 0;
		data.fy = 0;    

		data.nDirection = m_cmbMultiDir.GetCurSel();
		if (data.nDirection == 6) // Vector
		{
			CString csVector;
			double ux, uy, uz;
			m_edtMultiVector.GetWindowText(csVector);
			if (!GetPosition(csVector, ux, uy, uz))
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_vector_));
				return;
			}
			pIGM->GetWcsVector(ux, uy, uz, data.dVx, data.dVy, data.dVz);
		}

		data.MlfcK = CDlgUtil::CobxGetCurSelItemData(m_wndMlfccmbx, m_wndMlfccmbx.GetCurSel());    
	}

	BOOL bSuccess=FALSE;
	if (m_nOption == 0 || m_nOption == 1) // Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddNspr(aSelKey, data, (m_nOption==0)?FALSE:TRUE);	
	else if (m_nOption == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelNspr(aSelKey,BngrK);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CBndrNsprDlg::RestoreMlfcCbx()
{
	T_MLFC_K MlfcKLast;
	MlfcKLast = CDlgUtil::CobxGetCurSelItemData(m_wndMlfccmbx, m_wndMlfccmbx.GetCurSel());

	m_wndMlfccmbx.ResetContent();

	CArray<T_MLFC_K, T_MLFC_K> aMlfcK;
	m_pDoc->m_pAttrCtrl->GetMlfcKeyList(aMlfcK);
	for (int i = 0; i < aMlfcK.GetSize(); i++)
	{
		T_MLFC_D MlfcD;
		MlfcD.Initialize();
		m_pDoc->m_pAttrCtrl->GetMlfc(aMlfcK[i], MlfcD);

		if(MlfcD.nFuncType == 0)
		{
			CDlgUtil::CobxAddItem(m_wndMlfccmbx, MlfcD.FuncName, aMlfcK[i]);
		}
	}  
	if(CDlgUtil::CobxSetCurSelItemData(m_wndMlfccmbx, MlfcKLast) == CB_ERR)
	{
		m_wndMlfccmbx.SetCurSel(0);
	}
}

void CBndrNsprDlg::OnTmClose() 
{
	CloseDlg();
}

void CBndrNsprDlg::OnChangeOption() 
{
	UpdateData(TRUE);
	if (m_nOption == 2) // delete
	{
		EnableValueEdit(FALSE); 
		this->m_wndGroupCombo.ShowHideAll(TRUE);
	}
	else
	{
		EnableValueEdit(TRUE);	
		this->m_wndGroupCombo.ShowHideAll(FALSE);
	}
	
}

void CBndrNsprDlg::OnSelchangeType() // added by mylee 061121
{
	int nType = m_cmbType.GetCurSel();

	CDlgUtil::CtrlShowHide(this,m_arGen,nType == 0);
	CDlgUtil::CtrlShowHide(this,m_arCompTens,(nType == 1 || nType == 2));
	CDlgUtil::CtrlShowHide(this,m_arMulti,nType == 3);

	GetDlgItem(IDC_TM_PICTURE)->ShowWindow(nType != 3);

	if(nType == 0) ChangeBitmap();
	else if (nType == 1 || nType == 2) OnSelchangeTmBndrStiffCbo();
	else if (nType == 3) OnSelchangeMultiDir();
}

void CBndrNsprDlg::EnableValueEdit(BOOL bEnable)
{
	/*
	for (int i = 0; i < 6; i++)
	{
		m_Value[i].EnableWindow(bEnable);
	}
	*/
	CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_NSPR_POINT_SPRING_GRUP,bEnable,TRUE);
	if(bEnable)
	{
		OnSelchangeTmBndrStiffCbo();
		OnSelchangeMultiDir();
		OnClickDamping();
	}
}

BOOL CBndrNsprDlg::ExternalInit_by_SelectLabel(UINT key)
{
	return ExternalInit(key);
}

BOOL CBndrNsprDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_NSPR_K KeyNspr;
	T_NSPR_D data;
	KeyNspr.keymap=key;
	m_pDoc->m_pAttrCtrl->GetNspr(KeyNspr, data);
	int nType = 0;
	
//#if defined(_MGEN) // MNET:2866 20070628 mylee
//  data.nGenCompTen = 0;
//#endif
	nType = data.nGenCompTen;

	m_nOption = 1;
	
	T_BNGR_D  bngrD; // Group Setting
	m_pDoc->m_pAttrCtrl->GetBngr(data.GroupKey, bngrD);
	
	for(int i=0; i<m_wndGroupCombo.GetCount(); i++)
	{
		CString str;
		m_wndGroupCombo.GetLBText(i, str);
		if(str.CompareNoCase(bngrD.GroupName)==0) { m_wndGroupCombo.SetCurSel(i); break; }
	}
	if(nType == 0) // Linear
	{
		for (int i = 0; i < 6; i++)
		{
		  m_Value[i].SetEditUnit(data.Spring[i]);
		}

// 		m_chkSDx = m_chkSDy = m_chkSDz = m_chkSRx = m_chkSRy = m_chkSRz = FALSE;
// 		if( fabs(data.Spring[0] - m_pDoc->m_pAttrCtrl2->GetCurFixed_D()) < 1e-6 )  m_chkSDx = TRUE;
// 		if( fabs(data.Spring[1] - m_pDoc->m_pAttrCtrl2->GetCurFixed_D()) < 1e-6 )  m_chkSDy = TRUE;
// 		if( fabs(data.Spring[2] - m_pDoc->m_pAttrCtrl2->GetCurFixed_D()) < 1e-6 )  m_chkSDz = TRUE;
// 		if( fabs(data.Spring[3] - m_pDoc->m_pAttrCtrl2->GetCurFixed_R()) < 1e-6 )  m_chkSRx = TRUE;
// 		if( fabs(data.Spring[4] - m_pDoc->m_pAttrCtrl2->GetCurFixed_R()) < 1e-6 )  m_chkSRy = TRUE;
// 		if( fabs(data.Spring[5] - m_pDoc->m_pAttrCtrl2->GetCurFixed_R()) < 1e-6 )  m_chkSRz = TRUE;

		m_chkSDx = data.bFixed[0];
		m_chkSDy = data.bFixed[1];
		m_chkSDz = data.bFixed[2];
		m_chkSRx = data.bFixed[3];
		m_chkSRy = data.bFixed[4];
		m_chkSRz = data.bFixed[5];

		m_chkDamping = data.bDamping;
		for (int i = 0; i < 6; i++)
		{
			m_DampingValue[i].SetEditUnit(data.dDamping[i]);
		}
		

	}
	else if (nType == 1 || nType == 2) // Comp and Tens
	{
		m_wndStiffCombo.SetCurSel(data.nDirection);
		CString strValue;
		strValue.Format(_T("%g"),data.dStiffness);
		m_wndStiffEdit.SetWindowText(strValue);
		if(data.nDirection == 6)
		{
			CString csVector;
			csVector.Format(_LS(IDS_WG_TREEMENU___2lf____2lf____2lf), data.dVx, data.dVy, data.dVz);
			m_wndVectorEdit.SetWindowText(csVector);
		}
	}
	// MNET:2458 20061110 mylee
	else // MultiLinear 
	{
		//m_cmbMultiType.SetCurSel(data.nMultiLinearType);
		m_cmbMultiDir.SetCurSel(data.nDirection);
		CDlgUtil::CobxSetCurSelItemData(m_wndMlfccmbx, data.MlfcK);    
		OnSelchangeMultiDir();
		if(data.nDirection == 6)
		{
			CString csVector;
			csVector.Format(_LS(IDS_WG_TREEMENU___2lf____2lf____2lf), data.dVx, data.dVy, data.dVz);
			m_edtMultiVector.SetWindowText(csVector);
		}
	}

	UpdateData(FALSE); // nOption update
	m_cmbType.SetCurSel(nType); // Type Combo update
	OnSelchangeType(); 
	OnClickDamping();
	return TRUE;
}

void CBndrNsprDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);
}
void CBndrNsprDlg::OnSelchangeTmBndrStiffCbo()
{
	int nSelected = m_wndStiffCombo.GetCurSel();
	m_wndVectorEdit.EnableWindow(nSelected == 6);

	ChangeBitmap();
}
void CBndrNsprDlg::OnSelchangeMultiDir() // MNET:2458 20061116 mylee
{
	int nSelected = m_cmbMultiDir.GetCurSel();
	m_edtMultiVector.EnableWindow(nSelected == 6);

	ChangeBitmap();
}

void CBndrNsprDlg::OnClickDamping()
{
	UpdateData(TRUE);  
	CDlgUtil::CtrlEnableDisable(this, m_aDamping, m_chkDamping);
}
void CBndrNsprDlg::OnToggleFlag()
{
	const MSG* pMsg = GetCurrentMessage();
	
	UpdateData();  
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_NSPR_SDX_CHK:
		{
			m_Value[0].EnableWindow(!m_chkSDx);
// 			if( m_chkSDx )  m_Value[0].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_D());
// 			else            m_Value[0].SetEditUnit(0);
		}
		break;
	case IDC_TM_NSPR_SDY_CHK:
		{
			m_Value[1].EnableWindow(!m_chkSDy);
// 			if( m_chkSDy )  m_Value[1].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_D());
// 			else            m_Value[1].SetEditUnit(0);
		}
		break;
	case IDC_TM_NSPR_SDZ_CHK:
		{
			m_Value[2].EnableWindow(!m_chkSDz);
// 			if( m_chkSDz )  m_Value[2].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_D());
// 			else            m_Value[2].SetEditUnit(0);
		}
		break;
	case IDC_TM_NSPR_SRX_CHK:
		{
			m_Value[3].EnableWindow(!m_chkSRx);
// 			if( m_chkSRx )  m_Value[3].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_R());
// 			else            m_Value[3].SetEditUnit(0);
		}
		break;
	case IDC_TM_NSPR_SRY_CHK:
		{
			m_Value[4].EnableWindow(!m_chkSRy);
// 			if( m_chkSRy )  m_Value[4].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_R());
// 			else            m_Value[4].SetEditUnit(0);
		}
		break;
	case IDC_TM_NSPR_SRZ_CHK:
		{
			m_Value[5].EnableWindow(!m_chkSRz);
// 			if( m_chkSRz )  m_Value[5].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_R());
// 			else            m_Value[5].SetEditUnit(0);
		}
		break;
	}
	UpdateData(FALSE);
}

void CBndrNsprDlg::SetFlag()
{
	m_Value[0].EnableWindow(!m_chkSDx);
	//if( m_chkSDx )  m_Value[0].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_D());
	m_Value[1].EnableWindow(!m_chkSDy);
	//if( m_chkSDy )  m_Value[1].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_D());
	m_Value[2].EnableWindow(!m_chkSDz);
	//if( m_chkSDz )  m_Value[2].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_D());
	m_Value[3].EnableWindow(!m_chkSRx);
	//if( m_chkSRx )  m_Value[3].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_R());
	m_Value[4].EnableWindow(!m_chkSRy);
	//if( m_chkSRy )  m_Value[4].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_R());
	m_Value[5].EnableWindow(!m_chkSRz);
	//if( m_chkSRz )  m_Value[5].SetEditUnit(m_pDoc->m_pAttrCtrl2->GetCurFixed_R());
}

void CBndrNsprDlg::OnClickCalcBtn_SDZ()
{
	int nType = m_cmbType.GetCurSel();
	CBndrNsprCalcDlg dlg;
	dlg.SetPisdData(m_PsidD);
	dlg.SetSpringType(nType);
	if(dlg.DoModal()==IDOK)
	{
		double dVal = dlg.GetCalcValue();
		m_Value[2].SetEditUnit(dVal);
	}
}

void CBndrNsprDlg::OnClickCalcBtn_Stiff()
{
	int nType = m_cmbType.GetCurSel();
	CBndrNsprCalcDlg dlg;
	dlg.SetPisdData(m_PsidD);
	dlg.SetSpringType(nType);
	if(dlg.DoModal()==IDOK)
	{
		double dVal = dlg.GetCalcValue();
		m_wndStiffEdit.SetEditUnit(dVal);    
	}
}


void CBndrNsprDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break; 
	case D_UPDATE_UNIT:
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

void CBndrNsprDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount ==0 ) return;

	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsTdmtCommand = FALSE;
	BOOL bIsTmatCommand = FALSE;
	BOOL bIsMatlCommand = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_MLFC_ADD):
		case(UR_MLFC_DEL):
		case(UR_MLFC_MFD):
		case(UR_MLFC_MFS):
			RestoreMlfcCbx();
			break;
		default:
			break;
		}
	}
}

void CBndrNsprDlg::OnMlfcButton()
{
	CMlfcListDlg Dlg;
	Dlg.DoModal();
}

