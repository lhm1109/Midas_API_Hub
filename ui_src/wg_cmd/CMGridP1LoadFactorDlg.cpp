// CMGridP1LoadFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridP1LoadFactorDlg.h"
#include "CMGridP1LoadFactorTypeDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "ExportFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGridP1LoadFactorDlg dialog


CCMGridP1LoadFactorDlg::CCMGridP1LoadFactorDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMGridP1LoadFactorDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMGridP1LoadFactorDlg)
	m_nFactorType = 0;
	//}}AFX_DATA_INIT
		
	m_bModify = FALSE;
}


void CCMGridP1LoadFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridP1LoadFactorDlg)
	DDX_Control(pDX, IDC_LOADTYPE_COMBO, m_comboLoadType);
	DDX_Control(pDX, IDC_LOAD_TYPE_EDT, m_wndLoadItem);
	DDX_Radio(pDX, IDC_CMD_CONS_ADD_RDO, m_nFactorType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridP1LoadFactorDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMGridP1LoadFactorDlg)
	ON_BN_CLICKED(IDC_CMD_MORE_BUTTON, OnCmdMoreButton)
	ON_CBN_SELCHANGE(IDC_LOADTYPE_COMBO, OnSelchangeLoadtypeCombo)
	ON_BN_CLICKED(IDC_CMD_CONS_ADD_RDO, OnTmConsAddDelRdo)
	ON_BN_CLICKED(IDC_CMD_CONS_DELETE_RDO, OnTmConsAddDelRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridP1LoadFactorDlg message handlers

void CCMGridP1LoadFactorDlg::OnTmExecute() 
{
	UpdateData();

	T_P1LT_K P1ltK;
	T_P1LT_D P1ltD;
	m_comboLoadType.GetSelectedP1lt(P1ltK);
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl->GetP1lt(P1ltK, P1ltD))
		return;

	CArray<UINT, UINT> aSelKey;
	I_GENModelBase * pIGM = I_GENModelBase::GetCurMySelfST();
	
	if (P1ltD.nLoadItem == 0 || P1ltD.nLoadItem == 1)
		pIGM->GetSelectedNodeKeyList(aSelKey);
	else
		pIGM->GetSelectedElemKeyList(aSelKey);
	
	int nSize = aSelKey.GetSize();
	if (nSize < 1)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD__ADDD__Error___IMPACT_FACTOR_NO_SELECTED));
		return;
	}

	if (m_nFactorType == 0)   // add/modify
	{
		T_P1LA_D P1laD;
		CArray<T_P1LA_D, T_P1LA_D&> aP1laD;
		for (int i=0; i<nSize; i++)
		{
			P1laD.Initialize();
			P1laD.TypeKey = P1ltK;
			P1laD.nLoadItem = P1ltD.nLoadItem;
			P1laD.NodeElemK = aSelKey[i];
			aP1laD.Add(P1laD);
		}
		if (pDoc->m_pDataCtrl->AddP1la(aP1laD))
		{
			pIGM->UnselectAllNode();
			pIGM->UnselectAllElem();
		}
	}
	else    // delete
	{
		T_P1LA_K P1laK;
		CArray<T_P1LA_K, T_P1LA_K> aP1laK;
		for (int i=0; i<nSize; i++)
		{
			P1laK = pDoc->m_pAttrCtrl->GetP1laKey(P1ltD.nLoadItem, aSelKey[i]);
			if (P1laK > 0) aP1laK.Add(P1laK);
		}
		if (aP1laK.GetSize() > 0)
		{
			if (pDoc->m_pDataCtrl->DelP1la(aP1laK))
			{
				pIGM->UnselectAllNode();
				pIGM->UnselectAllElem();
			}
		}
	}

	/*
	if (m_nFactorType == 0)
	{
		//Add/Modify
		T_P1LA_D P1laD;
		T_P1LA_K P1laK;
		for (int i=0; i<nSize; i++)
		{
			P1laD.Initialize();
			P1laD.TypeKey = P1ltK;
			P1laD.nLoadItem = P1ltD.nLoadItem;
			P1laD.NodeElemK = aSelKey[i];

			P1laK = pDoc->m_pAttrCtrl->GetP1laKey(P1ltD.nLoadItem, aSelKey[i]);
			if (P1laK > 0)
			{
				if (!pDoc->m_pDataCtrl->ModifyP1la(P1laK, P1laD))
					continue;
			}
			else
			{
				if (!pDoc->m_pDataCtrl->AddP1la(P1laD))
					continue;
			}
		}
	}
	else
	{
		//Delete
		T_P1LA_K P1laK;
		for (int i=0; i<nSize; i++)
		{
			P1laK = pDoc->m_pAttrCtrl->GetP1laKey(P1ltD.nLoadItem, aSelKey[i]);
			if (P1laK > 0)
			{
				if (!pDoc->m_pDataCtrl->DelP1la(P1laK))
					continue;
			}
		}
	}
	*/
}

void CCMGridP1LoadFactorDlg::OnTmClose() 
{
	CloseDlg();
}

void CCMGridP1LoadFactorDlg::OnCmdMoreButton() 
{
	CCMGridP1LoadFactorTypeDlg dlg;
	dlg.DoModal();

	//CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//CreateOrActivateDlg(pDoc, CCMGridP1LoadFactorTypeDlg::IDD, this);
}

void CCMGridP1LoadFactorDlg::OnSelchangeLoadtypeCombo() 
{
	T_P1LT_K P1ltK;
	T_P1LT_D P1ltD;
	m_comboLoadType.GetSelectedP1lt(P1ltK);
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl->GetP1lt(P1ltK, P1ltD))
		return;

	if (P1ltD.nLoadItem == 0)
		GetDlgItem(IDC_CMD_LOAD_ITEM)->SetWindowText(_LS(IDS_CMD_IMPACTNP1_REACTION));
	else if (P1ltD.nLoadItem == 1)
		GetDlgItem(IDC_CMD_LOAD_ITEM)->SetWindowText(_LS(IDS_CMD_IMPACTNP1_DISP));
	else if (P1ltD.nLoadItem == 2)
		GetDlgItem(IDC_CMD_LOAD_ITEM)->SetWindowText(_LS(IDS_CMD_IMPACTNP1_MEMBFORCE));

	CString strLoadType=_T("");
	for (int i=0; i<6; i++)
	{
		if (P1ltD.nLoadType[i] == 0)
			strLoadType += _T("B");
		else if (P1ltD.nLoadType[i] == 1)
			strLoadType += _T("S");
	}
	m_wndLoadItem.SetWindowText(strLoadType);
}

void CCMGridP1LoadFactorDlg::Execute()
{
}

void CCMGridP1LoadFactorDlg::Data2Dlg(T_P1LA_D* pData)
{
	m_comboLoadType.ChangeSelect(pData->TypeKey);
	OnSelchangeLoadtypeCombo();
}

BOOL CCMGridP1LoadFactorDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) { ASSERT(0); return FALSE; }

	T_P1LA_D data;
	if (!pDoc->m_pAttrCtrl->GetP1la(key, data))
		data.Initialize();

	m_bModify = TRUE;
	Data2Dlg(&data);

	return TRUE;
}

BOOL CCMGridP1LoadFactorDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
	
	GetDlgItem(IDC_LOAD_TYPE_EDT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_EXECUTE)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_CLOSE)->ShowWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGridP1LoadFactorDlg::OnTmConsAddDelRdo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_comboLoadType.EnableWindow(m_nFactorType == 0);
	GetDlgItem(IDC_CMD_MORE_BUTTON)->EnableWindow(m_nFactorType == 0);
}
