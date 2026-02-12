// CMThrdBeam.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdBeam.h"
#include "CMThMainDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define THIS_MODE_METHOD  1
#define THIS_MSPT_METHOD  2
#define THIS_DIRE_METHOD  3
/////////////////////////////////////////////////////////////////////////////
// CCMThrdBeam dialog


CCMThrdBeam::CCMThrdBeam(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdBeam::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdBeam)
	m_Name = _T("");
	m_KeyNdEl = _T("");
	m_ModNum = 0;
	m_Type = 0;
	m_bCominedAxial = FALSE;
	//}}AFX_DATA_INIT
}


void CCMThrdBeam::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdBeam)
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cbxLComb);
	DDX_Control(pDX, IDC_CMD_BAR_THPOINT, m_PointCb);
	DDX_Control(pDX, IDC_CMD_BAR_THMODNUM, m_ModNumCb);
	DDX_Control(pDX, IDC_CMD_BAR_THKYNDEL, m_ElemPickEd);
	DDX_Control(pDX, IDC_CMD_BAR_THCOMP, m_CompCb);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_Name);
	DDX_Text(pDX, IDC_CMD_BAR_THKYNDEL, m_KeyNdEl);
	DDX_Radio(pDX, IDC_CMD_BAR_THMODNUM_ALL, m_ModNum);
	DDX_Radio(pDX, IDC_CMD_BAR_THTYPE_FORCE, m_Type);
	DDX_Check(pDX, IDC_CMD_BAR_THCOMBINE, m_bCominedAxial);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdBeam, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdBeam)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_FORCE,		OnGpsBarThType)
	ON_BN_CLICKED(IDC_CMD_BAR_THMODNUM_ALL, OnGpsBarThmodnum)
	ON_CBN_SELCHANGE(IDC_CMD_BAR_THCOMP, OnSelchangeGpsBarThcomp)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_STRESS,	OnGpsBarThType)
	ON_BN_CLICKED(IDC_CMD_BAR_THMODNUM_ONE, OnGpsBarThmodnum)
	ON_CBN_SELCHANGE(IDC_CMD_BAR_THLOADCASE, OnSelchangeGpsBarThloadcase)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdBeam message handlers

BOOL CCMThrdBeam::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if ((pMsg->message==WM_KEYDOWN) && (pMsg->wParam==13)) {
		pMsg->wParam = 9;
	}

	return CCMChildBarBase::PreTranslateMessage(pMsg);
}

BOOL CCMThrdBeam::OnExecute()
{
	if (!Dlg2Data())
		return FALSE;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (m_bModify)
	{
		if (!pDoc->m_pDataCtrl->ModifyThrd(strOldName, m_pDlgData))
			return FALSE;
	}
	else
	{
		if (!pDoc->m_pDataCtrl->AddThrd(m_pDlgData))
			return FALSE;
	}
	return TRUE;
}

void CCMThrdBeam::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 2);
}

void CCMThrdBeam::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 2);
	}
}

void CCMThrdBeam::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
	}
}

void CCMThrdBeam::DataInit()
{
	m_pDlgData.Initialize();
	m_pDlgData.nSubType = 1;
	m_pDlgData.nPosition = 1;
	m_pDlgData.nComponent = 1;
	m_pDlgData.bAllMode = TRUE;

	m_bModify = FALSE;
	strOldName = _T("");
}

BOOL CCMThrdBeam::Dlg2Data()
{
	UpdateData(TRUE);

	m_pDlgData.nGraphStep = 0;
	m_pDlgData.nFunctionType = 4;
	m_pDlgData.Name = m_Name;
	_stscanf(m_KeyNdEl.operator LPCTSTR(), _T("%d"), &m_pDlgData.nEntity);
	m_pDlgData.nSubType    = m_Type + 1;
	m_pDlgData.nPosition   = m_PointCb.GetCurSel() + 1;
	m_pDlgData.nComponent  = m_CompCb.GetCurSel()  + 1;
	if(m_ModNum == 0) 
	{
		m_pDlgData.bAllMode = TRUE;
		m_pDlgData.nSelectedMode = 0;
	}
	else 
	{
		m_pDlgData.bAllMode = FALSE;
		m_pDlgData.nSelectedMode = m_ModNumCb.GetCurSel() + 1;
	}

	if(m_Type != 1 || m_CompCb.GetCurSel()<=2)
		m_pDlgData.nOption1 = 0;
	else // bend stress +y,-y,+z,-z;
		m_pDlgData.nOption1 = m_bCominedAxial;

	UINT nLType,nLKey,nMnMxType;
	if(!m_cbxLComb.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return FALSE;
	}
	m_pDlgData.KeyThis     = nLKey; 
	return TRUE;
}

void CCMThrdBeam::Data2Dlg()
{
	m_Name = m_pDlgData.Name;
	
	if (m_pDlgData.nEntity == 0)
		m_KeyNdEl.Format(_T(""));
	else
	  m_KeyNdEl.Format(_T("%d"), m_pDlgData.nEntity);
	
	m_Type = m_pDlgData.nSubType - 1;
	m_bCominedAxial = m_pDlgData.nOption1;

	m_PointCb.SetCurSel(m_pDlgData.nPosition-1);
	m_cbxLComb.ChangeSelect(D_LOADCASE_HISTORY, m_pDlgData.KeyThis);

	MakeMenuCombo(m_Type);
	m_CompCb.SetCurSel(m_pDlgData.nComponent-1);

	if(m_pDlgData.bAllMode) 
	{
		m_ModNum = 0;
		m_ModNumCb.EnableWindow(FALSE);
		m_ModNumCb.SetCurSel(0);
	}
	else 
	{
		m_ModNum = 1;
		m_ModNumCb.EnableWindow(TRUE);
		m_ModNumCb.SetCurSel(m_pDlgData.nSelectedMode-1);
	}
	UpdateModeNumCtrl();
	UpdateData(FALSE);
	OnSelchangeGpsBarThcomp();
}

BOOL CCMThrdBeam::OnInitDialog()
{
	CCMChildBarBase::OnInitDialog();

	CDBDoc *pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;
	if (!m_bModify)
		DataInit();

	m_cbxLComb.SetLoadType(D_SELECTLOAD_HISTORY); 

	m_PointCb.AddString(_LS(IDS_WG_CMD_I_NODE));
	m_PointCb.AddString(_LS(IDS_WG_CMD_J_NODE));

	int		i, mod_num=pDoc->m_pAttrCtrl->GetNumEigv();
	TCHAR	ch[10];
	for (i=0;i<mod_num;i++) 
	{
		_stprintf(ch, _LS(IDS_WG_CMD_MODE__D), i+1);
		m_ModNumCb.AddString(ch);
	}

	m_ElemPickEd.SetAttNodeList();
	m_ElemPickEd.SetLButtonDownNotifyWindow(this);
	m_ElemPickEd.SetEnterNotifyWindow(this);
	m_ElemPickEd.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_ElemPickEd.SetMaxElemKeyNum(1);
	GotoDlgCtrl(&m_ElemPickEd);

	Data2Dlg();
	OnSelchangeGpsBarThcomp();
	
	return FALSE;
//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThrdBeam::OnGpsBarThType()
{
	// TODO: Add your control notification handler code here

	int		cur_type=m_Type;

	UpdateData(TRUE);

	if(cur_type == m_Type)return;

	MakeMenuCombo(m_Type);
}

void CCMThrdBeam::MakeMenuCombo(int nType)
{
	m_CompCb.ResetContent();
	if (nType == 0) 
	{
		m_CompCb.AddString(_LS(IDS_WG_CMD_AXIAL));
		m_CompCb.AddString(_LS(IDS_WG_CMD_SHEAR_Y));
		m_CompCb.AddString(_LS(IDS_WG_CMD_SHEAR_Z));
		m_CompCb.AddString(_LS(IDS_WG_CMD_TORSION));
		m_CompCb.AddString(_LS(IDS_WG_CMD_MOMENT_Y));
		m_CompCb.AddString(_LS(IDS_WG_CMD_MOMENT_Z));
		m_CompCb.SetCurSel(2);
	}
	else 
	{
		m_CompCb.AddString(_LS(IDS_WG_CMD_AXIAL));
		m_CompCb.AddString(_LS(IDS_WG_CMD_SHEAR_STRESS_Y));
		m_CompCb.AddString(_LS(IDS_WG_CMD_SHEAR_STRESS_Z));
		m_CompCb.AddString(_LS(IDS_WG_CMD_BEND__Y_));
		m_CompCb.AddString(_LS(IDS_WG_CMD_BEND__Y__MSG1));
		m_CompCb.AddString(_LS(IDS_WG_CMD_BEND__Z_));
		m_CompCb.AddString(_LS(IDS_WG_CMD_BEND__Z__MSG2));
		m_CompCb.SetCurSel(2);
	}
}

void CCMThrdBeam::OnGpsBarThmodnum()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	m_ModNumCb.EnableWindow(m_ModNum);
}

void CCMThrdBeam::OnSelchangeGpsBarThcomp() 
{
	UpdateData(TRUE);	
	
	if(m_Type != 1 || m_CompCb.GetCurSel()<=2)
		GetDlgItem(IDC_CMD_BAR_THCOMBINE)->EnableWindow(FALSE);
	else // bend stress +y,-y,+z,-z;
		GetDlgItem(IDC_CMD_BAR_THCOMBINE)->EnableWindow(TRUE);
}

void CCMThrdBeam::OnSelchangeGpsBarThloadcase() 
{
	UpdateModeNumCtrl();
}

void CCMThrdBeam::UpdateModeNumCtrl(void)
{
	UINT nLType,nLKey,nMnMxType;
	if(!m_cbxLComb.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		EnableModeNumCtrl(FALSE);
		return; 
	}
	switch(GetThisLdcType(nLKey))
	{
	case 1: EnableModeNumCtrl(TRUE);    break;  // Mode Super
	case 2: EnableModeNumCtrl(FALSE);   break;  // Direct Intergration
	case 3: EnableModeNumCtrl(FALSE);   break;  // Direct Intergration
	}
}

// 1:Mode Superposition  2:Direct Intergration
int  CCMThrdBeam::GetThisLdcType(UINT ThisK)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();

	int nRtnVal;
	T_THIS_D ThisD;
	pDoc->m_pAttrCtrl->GetThis(ThisK,ThisD);

	// Mode Superposition
	if(ThisD.nAnalMethod==1)
	{
		T_THMS_K ThmsK;
		T_THMS_D ThmsD;
		BOOL bDirect = FALSE;
		POSITION posThms=pDoc->m_pAttrCtrl->GetStartThms();
		while(posThms)
		{
			pDoc->m_pAttrCtrl->GetNextThms(posThms, ThmsK, ThmsD);
			if(ThmsD.ThisKey == ThisK)
			{
				bDirect=TRUE;
				break;
			}
		}

		if (bDirect)
			nRtnVal = THIS_MSPT_METHOD;
		else
			nRtnVal = THIS_MODE_METHOD;
	}
	// Direct Intergration
	else if(ThisD.nAnalMethod==2)
	{
		nRtnVal = THIS_DIRE_METHOD;
	}
	else if(ThisD.nAnalMethod==3)
	{
		nRtnVal = THIS_DIRE_METHOD;  
	}
	else
		ASSERT(0);
	return nRtnVal;
}

void  CCMThrdBeam::EnableModeNumCtrl(BOOL bEnable)
{
	GetDlgItem(IDC_CMD_BAR_THMODNUM_ALL)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_BAR_THMODNUM_ONE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_BAR_THMODNUM)->EnableWindow(bEnable && m_ModNum);
}

BOOL CCMThrdBeam::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_THRD_D ThrdD;
	pDoc->m_pAttrCtrl->GetThrd(key, ThrdD);
	m_pDlgData = ThrdD;
	strOldName = ThrdD.Name;
	m_bModify = TRUE;
	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
	
	Data2Dlg();
	return TRUE;
}
