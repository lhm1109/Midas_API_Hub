// CMThrdDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdDisp.h"
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
// CCMThrdDisp dialog


CCMThrdDisp::CCMThrdDisp(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdDisp::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdDisp)
	m_KeyNdEl = _T("");
	m_Name = _T("");
	m_Type = -1;
	m_ModNum = -1;
	m_nRefType = -1;
	m_sRefNode = _T("");
	//}}AFX_DATA_INIT
}


void CCMThrdDisp::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdDisp)
	DDX_Control(pDX, IDC_CMD_BAR_LOADCASE, m_cbxLComb);
	DDX_Control(pDX, IDC_CMD_BAR_THMODNUM, m_ModNumCb);
	DDX_Control(pDX, IDC_CMD_BAR_THCOMP, m_CompCb);
	DDX_Control(pDX, IDC_CMD_BAR_THKYNDEL, m_NodePickEd);
	DDX_Text(pDX, IDC_CMD_BAR_THKYNDEL, m_KeyNdEl);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_Name);
	DDX_Radio(pDX, IDC_CMD_BAR_THTYPE_DISP, m_Type);
	DDX_Radio(pDX, IDC_CMD_BAR_THMODNUM_ALL, m_ModNum);
	DDX_Radio(pDX, IDC_CMD_BAR_THTYPE_REFGROUND, m_nRefType);
	DDX_Text(pDX, IDC_CMD_BAR_THREF_NODE, m_sRefNode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdDisp, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdDisp)
	ON_BN_CLICKED(IDC_CMD_BAR_THMODNUM_ALL, OnGpsBarThmodnum)
	ON_CBN_SELCHANGE(IDC_CMD_BAR_LOADCASE, OnSelchangeGpsBarThloadcase)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_ACCEL, OnGpsBarThtypeofResult)
	ON_BN_CLICKED(IDC_CMD_BAR_THMODNUM_ONE, OnGpsBarThmodnum)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_DISP, OnGpsBarThtypeofResult)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_VEL, OnGpsBarThtypeofResult)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_REFADDGROUND, OnGpsBarThtypeRef)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_REFGROUND, OnGpsBarThtypeRef)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_REFNODE, OnGpsBarThtypeRef)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdDisp message handlers

BOOL CCMThrdDisp::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message==WM_KEYDOWN) && (pMsg->wParam==13))
		pMsg->wParam = 9;
	return CCMChildBarBase::PreTranslateMessage(pMsg);
}

BOOL CCMThrdDisp::OnExecute()
{
	UpdateData(TRUE);

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

void CCMThrdDisp::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 0);
}

void CCMThrdDisp::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 0);
	}
}

void CCMThrdDisp::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
	}
}

BOOL CCMThrdDisp::OnInitDialog()
{
	CCMChildBarBase::OnInitDialog();
	
	m_bModify = FALSE;  
	if (!m_bModify)
		DataInit();

	CString sComp[]={ _T("DX"), _T("DY"), _T("DZ"), _T("RX"), _T("RY"), _T("RZ") };
	for(int i=0; i<6; i++)m_CompCb.AddString(sComp[i]);
	
	m_cbxLComb.SetLoadType(D_SELECTLOAD_HISTORY); 
	
	m_NodePickEd.SetAttNodeList();
	m_NodePickEd.SetLButtonDownNotifyWindow(this);
	m_NodePickEd.SetEnterNotifyWindow(this);
	m_NodePickEd.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_NodePickEd.SetMaxNodeKeyNum(1);
	GotoDlgCtrl(&m_NodePickEd);

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	int	mod_num=pDoc->m_pAttrCtrl->GetNumEigv();
	TCHAR	ch[10];
	m_ModNumCb.ResetContent();
	
	for(int i=0;i<mod_num;i++)
	{
		_stprintf(ch, _LS(IDS_WG_CMD_MODE__D), i+1);
		m_ModNumCb.AddString(ch);
	}

	Data2Dlg();

	return FALSE;
}

void CCMThrdDisp::DataInit()
{
	m_pDlgData.Initialize();
	m_pDlgData.nComponent = 1;
	m_pDlgData.nSubType = 1;
	m_pDlgData.bAllMode = TRUE;
	
	m_bModify = FALSE;
	strOldName = _T("");
}

void CCMThrdDisp::Data2Dlg()
{
	m_CompCb.SetCurSel(m_pDlgData.nComponent-1);
	m_Name = m_pDlgData.Name;
	
	if (m_pDlgData.nEntity == 0)
		m_KeyNdEl.Format(_T(""));
	else
		m_KeyNdEl.Format(_T("%d"), m_pDlgData.nEntity);
	
	m_Type = m_pDlgData.nSubType - 1;
	m_cbxLComb.ChangeSelect(D_LOADCASE_HISTORY, m_pDlgData.KeyThis);
	m_nRefType = m_pDlgData.nRefPoint-1;

	if(m_pDlgData.KeyRefNode)
		m_sRefNode.Format(_T("%d"), m_pDlgData.KeyRefNode);

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

	UpdateAntNodeCtrl(m_nRefType);
	UpdateModeNumCtrl();
	UpdateRefNodeCtrl();

	UpdateData(FALSE);
}

BOOL CCMThrdDisp::Dlg2Data()
{
	m_pDlgData.nGraphStep = 0;
	m_pDlgData.nFunctionType = 2;
	m_pDlgData.Name = m_Name;
	_stscanf(m_KeyNdEl.operator LPCTSTR(), _T("%d"), &m_pDlgData.nEntity);
	m_pDlgData.nSubType    = m_Type + 1;
	m_pDlgData.nComponent  = m_CompCb.GetCurSel() + 1;
	if (m_ModNum == 0) 
	{
		m_pDlgData.bAllMode = TRUE;
		m_pDlgData.nSelectedMode = 0;
	}
	else 
	{
		m_pDlgData.bAllMode = FALSE;
		m_pDlgData.nSelectedMode = m_ModNumCb.GetCurSel() + 1;
	}

	UINT nLType,nLKey,nMnMxType;
	if(!m_cbxLComb.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return FALSE; 
	}
	m_pDlgData.KeyThis     = nLKey; 
	m_pDlgData.nRefPoint   = m_nRefType+1;
	m_pDlgData.KeyRefNode  = _ttoi(m_sRefNode);
	return TRUE;
}

void CCMThrdDisp::OnGpsBarThmodnum()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ModNumCb.EnableWindow(m_ModNum);
}

void CCMThrdDisp::OnSelchangeGpsBarThloadcase() 
{
	UpdateData();
	UpdateModeNumCtrl();
	UpdateRefNodeCtrl();
}

void CCMThrdDisp::OnGpsBarThtypeofResult() 
{
	//UpdateRefPtCbx();
	UpdateData(TRUE);

	if (m_Type == 2)
		m_nRefType = 1;

	UpdateAntNodeCtrl(m_nRefType);
	UpdateData(FALSE);
}

void CCMThrdDisp::UpdateModeNumCtrl(void)
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
void CCMThrdDisp::UpdateRefNodeCtrl(void)
{
	UINT nLType,nLKey,nMnMxType;
	
	if(!m_cbxLComb.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		GetDlgItem(IDC_CMD_BAR_THTYPE_REFGROUND)->EnableWindow(TRUE);
		return; 
	}
	
	if(IsMutiSupportThis(nLKey)) 
	{
		if(m_nRefType==0){  m_nRefType=1; UpdateData(FALSE);  }
		GetDlgItem(IDC_CMD_BAR_THTYPE_REFGROUND)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_CMD_BAR_THTYPE_REFGROUND)->EnableWindow(TRUE);
	}
}

BOOL CCMThrdDisp::IsMutiSupportThis(UINT nLKey)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	T_THMS_K ThmsK;
	T_THMS_D ThmsD;
	POSITION nPos = pDoc->m_pAttrCtrl->GetStartThms();
	while(nPos)
	{
		pDoc->m_pAttrCtrl->GetNextThms(nPos,ThmsK,ThmsD);
		if(ThmsD.ThisKey == nLKey)
			return TRUE;
	}
	return FALSE; 
}

// 1:Mode Superposition  2:Direct Intergration
int  CCMThrdDisp::GetThisLdcType(UINT ThisK)
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

void  CCMThrdDisp::EnableModeNumCtrl(BOOL bEnable)
{
	GetDlgItem(IDC_CMD_BAR_THMODNUM_ALL)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_BAR_THMODNUM_ONE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_BAR_THMODNUM)->EnableWindow(bEnable && m_ModNum);
}

void  CCMThrdDisp::UpdateAntNodeCtrl(int nType)
{
	switch(nType)
	{
	case 0:
	case 1:
		GetDlgItem(IDC_CMD_BAR_THREF_NODE)->EnableWindow(FALSE);
		break; 
	case 2:
		GetDlgItem(IDC_CMD_BAR_THREF_NODE)->EnableWindow(TRUE);
		break; 
	}
}

void CCMThrdDisp::OnGpsBarThtypeRef() 
{
	UpdateData(TRUE);
	UpdateAntNodeCtrl(m_nRefType);
}

BOOL CCMThrdDisp::ExternalInit(UINT key)
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
