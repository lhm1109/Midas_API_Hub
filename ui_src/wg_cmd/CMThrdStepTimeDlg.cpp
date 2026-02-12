// CMThrdStepTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdStepTimeDlg.h"
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
// CCMThrdStepTimeDlg dialog


CCMThrdStepTimeDlg::CCMThrdStepTimeDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdStepTimeDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdStepTimeDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CCMThrdStepTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdStepTimeDlg)
	DDX_Control(pDX, IDC_CMD_BAR_TIME_STEP, m_cmbTimeStep);
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cmbLoadCase);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdStepTimeDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdStepTimeDlg)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_CBN_SELCHANGE(IDC_CMD_BAR_THLOADCASE, OnSelchangeCmdBarThloadcase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepTimeDlg message handlers

void CCMThrdStepTimeDlg::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 0);
}

void CCMThrdStepTimeDlg::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 0);
	}
}

void CCMThrdStepTimeDlg::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
	}
}

void CCMThrdStepTimeDlg::DataInit()
{
	m_pDlgData.Initialize();
	
	m_bModify = FALSE;
	strOldName = _T("");
}

void CCMThrdStepTimeDlg::Data2Dlg()
{
	m_strName = m_pDlgData.Name;
	m_cmbLoadCase.ChangeSelect(D_LOADCASE_HISTORY,m_pDlgData.KeyThis);
	OnSelchangeCmdBarThloadcase();

	int nSize = m_cmbTimeStep.GetCount();
	double dTimeStep;
	CString strTimeStep;
	for (int i=0; i<nSize; i++)
	{
		m_cmbTimeStep.GetLBText(i, strTimeStep);
		dTimeStep = _tstof(strTimeStep);
		if (dTimeStep == m_pDlgData.dTimeStep)
		{
			m_cmbTimeStep.SetCurSel(i);
			break;
		}
	}
	UpdateData(FALSE);
}

BOOL CCMThrdStepTimeDlg::Dlg2Data()
{
	UpdateData();

	m_pDlgData.nGraphStep = 1;
	m_pDlgData.nStepFunction = 1;
	m_pDlgData.Name = m_strName;

	UINT nLType,nLKey,nMnMxType;
	if(!m_cmbLoadCase.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return FALSE;
	}
	m_pDlgData.KeyThis = nLKey;
	
	int nSel = m_cmbTimeStep.GetCurSel();
	if (nSel > -1)
	{
		CString strTimeStep;
		m_cmbTimeStep.GetLBText(nSel, strTimeStep);
		m_pDlgData.dTimeStep = _tstof(strTimeStep);
	}
	return TRUE;
}

BOOL CCMThrdStepTimeDlg::OnExecute()
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

BOOL CCMThrdStepTimeDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();

	m_bModify = FALSE;
	if (!m_bModify)
		DataInit();
	
	m_cmbLoadCase.SetLoadType(D_SELECTLOAD_HISTORY);

	Data2Dlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThrdStepTimeDlg::OnSelchangeCmdBarThloadcase() 
{
	m_cmbTimeStep.ResetContent();

	UINT nLType,nLKey,nMnMxType;
	if(!m_cmbLoadCase.GetSelectedLoad(nLType, nLKey, &nMnMxType))
		return;

	CArray<double, double> aTimeStep;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetTimeStep(nLKey, aTimeStep);

	int nSize = aTimeStep.GetSize();
	if (nSize < 1) return;
	CString str;
	for (int i=0; i<nSize; i++)
	{
		str.Format(_T("%g"), aTimeStep[i]);
		m_cmbTimeStep.AddString(str);
	}
	m_cmbTimeStep.SetCurSel(0);
}

BOOL CCMThrdStepTimeDlg::ExternalInit(UINT key)
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
