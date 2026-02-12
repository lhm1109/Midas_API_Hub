// CMThrdPnStrn.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdPnStrn.h"
#include "CMThMainDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThrdPnStrn dialog


CCMThrdPnStrn::CCMThrdPnStrn(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdPnStrn::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdPnStrn)
	m_Name = _T("");
	m_KeyNdEl = _T("");
	m_ModNum = 0;
	m_Type = 0;
	m_nGlobal = 0;
	//}}AFX_DATA_INIT
}


void CCMThrdPnStrn::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdPnStrn)
	DDX_Control(pDX, IDC_CMD_BAR_THPOINT, m_PointCb);
	DDX_Control(pDX, IDC_CMD_BAR_THMODNUM, m_ModNumCb);
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cbxLComb);
	DDX_Control(pDX, IDC_CMD_BAR_THKYNDEL, m_ElemPickEd);
	DDX_Control(pDX, IDC_CMD_BAR_THCOMP, m_CompCb);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_Name);
	DDX_Text(pDX, IDC_CMD_BAR_THKYNDEL, m_KeyNdEl);
	DDX_Radio(pDX, IDC_CMD_BAR_THMODNUM_ALL, m_ModNum);
	DDX_Radio(pDX, IDC_CMD_BAR_THTYPE_FORCE, m_Type);
	DDX_Radio(pDX, IDC_CMD_THRD_PART_RDO1, m_nGlobal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdPnStrn, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdPnStrn)
	ON_BN_CLICKED(IDC_CMD_BAR_THMODNUM_ALL, OnGpsBarThmodnum)
	ON_BN_CLICKED(IDC_CMD_BAR_THMODNUM_ONE, OnGpsBarThmodnum)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_FORCE, OnGpsBarThType)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_STRESS, OnGpsBarThType)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdPnStrn message handlers

BOOL CCMThrdPnStrn::OnExecute()
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

void CCMThrdPnStrn::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 4);
}

void CCMThrdPnStrn::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 4);
	}
}

void CCMThrdPnStrn::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
	}
}

void CCMThrdPnStrn::DataInit()
{
	m_pDlgData.Initialize();
	m_pDlgData.nSubType = 1;
	m_pDlgData.nPosition = 1;
	m_pDlgData.nComponent = 1;
	m_pDlgData.bAllMode = TRUE;

	m_bModify = FALSE;
	strOldName = _T("");
}

BOOL CCMThrdPnStrn::Dlg2Data()
{
	UpdateData(TRUE);
	m_pDlgData.nGraphStep = 0;
	m_pDlgData.nFunctionType = 10;
	m_pDlgData.Name = m_Name;
	_stscanf(m_KeyNdEl.operator LPCTSTR(), _T("%d"), &m_pDlgData.nEntity);
	m_pDlgData.nSubType    = m_Type + 1;
	m_pDlgData.nPosition   = m_PointCb.GetCurSel() + 1;
	m_pDlgData.nComponent  = m_CompCb.GetCurSel() + 1;
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

	UINT nLType,nLKey,nMnMxType;
	if(!m_cbxLComb.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return FALSE;
	}
	m_pDlgData.KeyThis     = nLKey; 
	m_pDlgData.nGlobalLocal = m_nGlobal;
	return TRUE;
}

void CCMThrdPnStrn::Data2Dlg()
{
	m_Name = m_pDlgData.Name;
	
	if (m_pDlgData.nEntity == 0)
		m_KeyNdEl.Format(_T(""));
	else
		m_KeyNdEl.Format(_T("%d"), m_pDlgData.nEntity);

	m_Type = m_pDlgData.nSubType - 1;
	SetPosCompList(m_Type);
	m_cbxLComb.ChangeSelect(D_LOADCASE_HISTORY, m_pDlgData.KeyThis);
	m_PointCb.SetCurSel(m_pDlgData.nPosition-1);
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
	m_nGlobal = m_pDlgData.nGlobalLocal;
	UpdateData(FALSE);
}

BOOL CCMThrdPnStrn::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
	
	CDBDoc *pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;
	if (!m_bModify)
		DataInit();

	int		i, mod_num=pDoc->m_pAttrCtrl->GetNumEigv();
	TCHAR	ch[10];
	for(i=0;i<mod_num;i++) 
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

	m_cbxLComb.SetLoadType(D_SELECTLOAD_HISTORY ); 

	Data2Dlg();

	return FALSE;
}

BOOL CCMThrdPnStrn::PreTranslateMessage(MSG* pMsg) 
{
	if((pMsg->message==WM_KEYDOWN) && (pMsg->wParam==13)) 
	{
		pMsg->wParam = 9;
	}
	
	return CCMChildBarBase::PreTranslateMessage(pMsg);
}

void CCMThrdPnStrn::OnGpsBarThmodnum() 
{
	UpdateData(TRUE);
	m_ModNumCb.EnableWindow(m_ModNum);
	
}

void CCMThrdPnStrn::OnGpsBarThType() 
{
	int		cur_type=m_Type;
	UpdateData(TRUE);
	if(cur_type == m_Type)return;
	SetPosCompList(m_Type,TRUE);
}

void CCMThrdPnStrn::SetPosCompList(int nType,BOOL bDefault/*=FALSE*/)
{
	m_PointCb.ResetContent();
	m_CompCb.ResetContent();
	if(nType == 0) 
	{
		m_PointCb.AddString(_LS(IDS_WG_CMD_I_NODE));
		m_PointCb.AddString(_LS(IDS_WG_CMD_J_NODE));
		m_PointCb.AddString(_LS(IDS_WG_CMD_K_NODE));
		m_PointCb.AddString(_LS(IDS_WG_CMD_L_NODE));
		if(bDefault)m_PointCb.SetCurSel(0);

		m_CompCb.AddString(_T("Fx"));
		m_CompCb.AddString(_T("Fy"));
		m_CompCb.AddString(_T("Fz"));
		if(bDefault)m_CompCb.SetCurSel(0);
	}
	else 
	{
		m_PointCb.AddString(_LS(IDS_WG_CMD_CENTER));
		m_PointCb.AddString(_LS(IDS_WG_CMD_I_NODE));
		m_PointCb.AddString(_LS(IDS_WG_CMD_J_NODE));
		m_PointCb.AddString(_LS(IDS_WG_CMD_K_NODE));
		m_PointCb.AddString(_LS(IDS_WG_CMD_L_NODE));
		if(bDefault)m_PointCb.SetCurSel(0);

		m_CompCb.AddString(_LS(IDS_WG_GPS_LEGEND_Sig_xx));
		m_CompCb.AddString(_LS(IDS_WG_GPS_LEGEND_Sig_yy));
		m_CompCb.AddString(_LS(IDS_WG_GPS_LEGEND_Sig_zz));
		m_CompCb.AddString(_LS(IDS_WG_GPS_LEGEND_Sig_xy));
		if(bDefault)m_CompCb.SetCurSel(0);
	}
}

BOOL CCMThrdPnStrn::ExternalInit(UINT key)
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
