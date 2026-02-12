// CMThrdStepGnlkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdStepGnlkDlg.h"
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
// CCMThrdStepGnlkDlg dialog


CCMThrdStepGnlkDlg::CCMThrdStepGnlkDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdStepGnlkDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdStepGnlkDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CCMThrdStepGnlkDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdStepGnlkDlg)
	DDX_Control(pDX, IDC_CMD_BAR_THNLLK_CBX, m_cbxNllk);
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cmbLoadCase);
	DDX_Control(pDX, IDC_CMD_BAR_THVALUE2, m_cmbValue);
	DDX_Control(pDX, IDC_CMD_BAR_THCOMP, m_cmbComponents);
	DDX_Control(pDX, IDC_CMD_BAR_THPOINT, m_cmbPoint);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdStepGnlkDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdStepGnlkDlg)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepGnlkDlg message handlers

void CCMThrdStepGnlkDlg::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 5);
}

void CCMThrdStepGnlkDlg::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 5);
	}
}

void CCMThrdStepGnlkDlg::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
	}
}

void CCMThrdStepGnlkDlg::DataInit()
{
	m_pDlgData.Initialize();
	m_pDlgData.nPosition = 1;
	m_pDlgData.nComponent = 1;
	
	m_bModify = FALSE;
	strOldName = _T("");
}

void CCMThrdStepGnlkDlg::Data2Dlg()
{
	m_strName = m_pDlgData.Name;
	m_cmbPoint.SetCurSel(m_pDlgData.nPosition-1);
	m_cmbComponents.SetCurSel(m_pDlgData.nComponent-1);
	m_cmbValue.SetCurSel(m_pDlgData.nValueType);

	int nSize = m_cbxNllk.GetCount();
	for (int i=0; i<nSize; i++)
	{
		if (m_cbxNllk.GetItemData(i) == m_pDlgData.nEntity)
		{
			m_cbxNllk.SetCurSel(i);
			break;
		}
	}
	m_cmbLoadCase.ChangeSelect(D_LOADCASE_HISTORY, m_pDlgData.KeyThis);

	UpdateData(FALSE);
}

BOOL CCMThrdStepGnlkDlg::Dlg2Data()
{
	UpdateData();

	m_pDlgData.nGraphStep = 1;
	m_pDlgData.nStepFunction = 6;
	m_pDlgData.Name = m_strName;
	m_pDlgData.nPosition = m_cmbPoint.GetCurSel() + 1;
	m_pDlgData.nComponent = m_cmbComponents.GetCurSel()+1;
	m_pDlgData.nValueType = m_cmbValue.GetCurSel();
	
	int nSelIdx=m_cbxNllk.GetCurSel();
	m_pDlgData.nEntity = m_cbxNllk.GetItemData(nSelIdx);

	UINT nLType,nLKey,nMnMxType;
	if(!m_cmbLoadCase.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return FALSE;
	}
	m_pDlgData.KeyThis = nLKey; 
	return TRUE;
}

BOOL CCMThrdStepGnlkDlg::OnExecute()
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

BOOL CCMThrdStepGnlkDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();

	m_bModify = FALSE;
	if (!m_bModify)
		DataInit();
	
	m_cmbLoadCase.SetLoadType(D_SELECTLOAD_HISTORY);

	MakeCombo();
	MakeNllkKeyList();
	Data2Dlg();
	return TRUE;
}

void CCMThrdStepGnlkDlg::MakeCombo()
{
	m_cmbPoint.AddString(_LS(IDS_WG_CMD_I_NODE));
	m_cmbPoint.AddString(_LS(IDS_WG_CMD_J_NODE));
	//m_cmbPoint.AddString(_LS(IDS_WG_CMD_M_NODE));
	m_cmbPoint.SetCurSel(0);

	/**
	CString sCompo[]={_T("Fx"), _T("Fy"), _T("Fz"), _T("Mx"), _T("My"), _T("Mz")};
	for (int i=0; i<6; i++)
		m_cmbComponents.AddString(sCompo[i]);
		**/
	m_cmbComponents.AddString(_LS(IDS_WG_CMD_AXIAL));
	m_cmbComponents.AddString(_LS(IDS_WG_CMD_SHEAR_Y));
	m_cmbComponents.AddString(_LS(IDS_WG_CMD_SHEAR_Z));
	m_cmbComponents.AddString(_LS(IDS_WG_CMD_TORSION));
	m_cmbComponents.AddString(_LS(IDS_WG_CMD_MOMENT_Y));
	m_cmbComponents.AddString(_LS(IDS_WG_CMD_MOMENT_Z));
	m_cmbComponents.SetCurSel(0);

	CString sValue[] = { _LS(IDS_WG_CMD_MIN), _LS(IDS_WG_CMD_MAX), _LS(IDS_WG_CMD_ABSMAX) };
	for (int i=0; i<3; i++)
		m_cmbValue.AddString(sValue[i]);
	m_cmbValue.SetCurSel(0);
}

BOOL CCMThrdStepGnlkDlg::ExternalInit(UINT key)
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

void CCMThrdStepGnlkDlg::MakeNllkKeyList()
{
	CDBDoc *pDoc=CDBDoc::GetDocPoint();

	CArray<T_NLNK_K,T_NLNK_K>m_arNllk;
	pDoc->m_pAttrCtrl->GetNlnkKeyList(m_arNllk);
	if(!m_arNllk.GetSize()) return;

	CString   sNllk;
	T_NLNK_D  Data;
	m_cbxNllk.ResetContent();
	for (int i=0; i<m_arNllk.GetSize(); i++)
	{
		if(pDoc->m_pAttrCtrl->GetNlnk(m_arNllk[i], Data))
		{
			sNllk.Format(_T("%d (n1:%d,n2:%d)"),m_arNllk[i],Data.Node1,Data.Node2);
			m_cbxNllk.AddString(sNllk);
			m_cbxNllk.SetItemData(i, m_arNllk[i]);
		}
	}
	
	if (m_cbxNllk.GetCount() > 0)
		m_cbxNllk.SetCurSel(0);
}
