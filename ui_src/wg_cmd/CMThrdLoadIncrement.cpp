// CMThrdLoadIncrement.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdLoadIncrement.h"
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
// CCMThrdLoadIncrement dialog


CCMThrdLoadIncrement::CCMThrdLoadIncrement(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdLoadIncrement::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdLoadIncrement)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CCMThrdLoadIncrement::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdLoadIncrement)
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cmbLC);
	DDX_Control(pDX, IDC_CMD_BAR_SCALE, m_edtScaleFactor);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdLoadIncrement, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdLoadIncrement)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOk)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdLoadIncrement message handlers

BOOL CCMThrdLoadIncrement::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
	
	m_bModify = FALSE;
	strOldName = _T("");

	MakeLCCombo();
	T_THRD_D rData;
	rData.Initialize();

	Data2Dlg(rData);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThrdLoadIncrement::MakeLCCombo()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	CArray<T_THIS_K, T_THIS_K> rKeyList;
	pDoc->m_pAttrCtrl->GetThisKeyList(rKeyList, FALSE);
	
	int nSize = rKeyList.GetSize();
	if (nSize < 1) return;

	m_cmbLC.ResetContent();
	T_THIS_D rData;
	int nIndex = -1;
	for (int i=0; i<nSize; i++)
	{
		pDoc->m_pAttrCtrl->GetThis(rKeyList[i], rData);

		if (rData.nAnalType==2 && rData.nAnalMethod==3)
		{
			nIndex = m_cmbLC.AddString(rData.LoadCaseName);
			m_cmbLC.SetItemData(nIndex, rKeyList[i]);
		}
	}

	if (nIndex != -1)
		m_cmbLC.SetCurSel(0);
}

void CCMThrdLoadIncrement::Data2Dlg(T_THRD_D& rData)
{
	m_strName = rData.Name;
	//m_edtScaleFactor.SetEditUnit(rData.dScaleFactor);
	
	MakeLCCombo();

	int nSize = m_cmbLC.GetCount();
	for (int i=0; i<nSize; i++)
	{
		if (rData.KeyThis == m_cmbLC.GetItemData(i))
		{
			m_cmbLC.SetCurSel(i);
			break;
		}
	}
	
	UpdateData(FALSE);
}

void CCMThrdLoadIncrement::Dlg2Data(T_THRD_D& rData)
{
	UpdateData();

	rData.Initialize();
	rData.Name = m_strName;
	rData.nFunctionType = 11;
	//rData.dScaleFactor = m_edtScaleFactor.GetEditValue();
	
	int nSel = m_cmbLC.GetCurSel();
	if (nSel != CB_ERR)
	{
		rData.KeyThis = m_cmbLC.GetItemData(nSel);
	}
}

BOOL CCMThrdLoadIncrement::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_THRD_D ThrdD;
	pDoc->m_pAttrCtrl->GetThrd(key, ThrdD);
	
	m_bModify = TRUE;
	Data2Dlg(ThrdD);
	strOldName = ThrdD.Name;
	return TRUE;
}

void CCMThrdLoadIncrement::OnCmdOk() 
{
	T_THRD_D rData;
	Dlg2Data(rData);

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (m_bModify)
	{
		if (pDoc->m_pDataCtrl->ModifyThrd(strOldName, rData))
			((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 9);//ComboIndex
	}
	else
	{
		if (pDoc->m_pDataCtrl->AddThrd(rData))
			((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 9);//ComboIndex
	}
}

void CCMThrdLoadIncrement::OnCmdCancel() 
{
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 9);//ComboIndex
}
