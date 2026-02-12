// CMTHLoadCombItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTHLoadCombItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\IUsageCounter.h"
#include "..\wg_base\wg_base_ArrayUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMTHLoadCombItemDlg dialog
CCMTHLoadCombItemDlg::CCMTHLoadCombItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMTHLoadCombItemDlg::IDD, pParent)
{
	m_bModify = FALSE;
	m_Key = 0;
	m_Data.Initialize();
	m_nType = D_THLC_TYPE_ENV;
}

void CCMTHLoadCombItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CMD_NAME_EDT, m_edtName);
	DDX_Control(pDX, IDC_CMD_DESC_EDT, m_edtDesc);
	DDX_Control(pDX, IDC_CMD_CASE_DEF_LIST, m_lstDefined );
	DDX_Control(pDX, IDC_CMD_CASE_SEL_LIST, m_lstSelected);
	DDX_Radio  (pDX, IDC_CMD_TYPE_ENV_RDO, m_nType);
}

BEGIN_MESSAGE_MAP(CCMTHLoadCombItemDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_RIGHT_BTN      , OnSelectBtn)
	ON_BN_CLICKED(IDC_CMD_LEFT_BTN       , OnUnselectBtn)
	ON_BN_CLICKED(IDC_CMD_APPLY_BTN      , OnApply)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTHLoadCombItemDlg message handlers
BOOL CCMTHLoadCombItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	Data2Dlg();

	return TRUE;
}

void CCMTHLoadCombItemDlg::SetData(T_THLC_K Key, T_THLC_D &Data)
{
	m_Key = Key;
	m_Data = Data;
}

BOOL CCMTHLoadCombItemDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.Initialize();
	m_edtName.GetWindowText(m_Data.strName);
	m_edtDesc.GetWindowText(m_Data.strDesc);
	m_Data.nType = m_nType;

	for(int i=0; i<m_lstSelected.GetCount(); i++)
	{
		T_THIS_K ThisK = m_lstSelected.GetItemData(i);
		m_Data.aThisK.Add(ThisK);
	}

	return TRUE;
}

BOOL CCMTHLoadCombItemDlg::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);
	m_edtDesc.SetWindowText(m_Data.strDesc);
	m_nType = m_Data.nType;

	CArray<T_THIS_K,T_THIS_K> aThisK;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisK, FALSE);
	for(int i=0; i<aThisK.GetSize(); i++)
	{
		T_THIS_D ThisD;
		m_pDoc->m_pAttrCtrl->GetThis(aThisK[i], ThisD);
		const BOOL bSorted=TRUE;
		if(CArrayUtil::IsExist(m_Data.aThisK, aThisK[i], bSorted))
		{
			int nIdx = m_lstSelected.AddString(ThisD.LoadCaseName);
			m_lstSelected.SetItemData(nIdx, aThisK[i]);
		}
		else
		{
			int nIdx = m_lstDefined.AddString(ThisD.LoadCaseName);
			m_lstDefined.SetItemData(nIdx, aThisK[i]);
		}
	}

	UpdateData(FALSE);

	return TRUE;
}

void CCMTHLoadCombItemDlg::OnOK() 
{
	if (!AddToDB())  return;

	CDialogMove::OnOK();  
}

void CCMTHLoadCombItemDlg::OnApply()
{
	AddToDB();
}

BOOL CCMTHLoadCombItemDlg::AddToDB()
{
	if (!Dlg2Data()) return FALSE;

	BOOL bSuccess = FALSE;
	if (m_bModify)  bSuccess = m_pDoc->m_pDataCtrl->ModifyThlc(m_Key, m_Key, m_Data);  
	else            bSuccess = m_pDoc->m_pDataCtrl->AddThlc(m_Data);

	IUsageCounter::Use(_T("TH_LCOM"), FALSE);

	return bSuccess;
}

void CCMTHLoadCombItemDlg::OnSelectBtn()
{
	CArray<T_THIS_K,T_THIS_K> aThisK_selected;
	for(int i=m_lstDefined.GetCount()-1; i>=0; --i)
	{
		if(m_lstDefined.GetSel(i)>0)
		{
			T_THIS_K ThisK = m_lstDefined.GetItemData(i);
			aThisK_selected.Add(ThisK);
			m_lstDefined.DeleteString(i);
		}
	}
	for(int i=0; i<m_lstSelected.GetCount(); i++)
	{
		T_THIS_K ThisK = m_lstSelected.GetItemData(i);
		aThisK_selected.Add(ThisK);
	}
	CArrayUtil::Sort(aThisK_selected);

	m_lstSelected.ResetContent();
	for(int i=0; i<aThisK_selected.GetSize(); i++)
	{
		T_THIS_D ThisD;
		m_pDoc->m_pAttrCtrl->GetThis(aThisK_selected[i], ThisD);
		m_lstSelected.AddString(ThisD.LoadCaseName);
		m_lstSelected.SetItemData(i, aThisK_selected[i]);
	}
}

void CCMTHLoadCombItemDlg::OnUnselectBtn()
{
	CArray<T_THIS_K,T_THIS_K> aThisK_unselected;
	for(int i=m_lstSelected.GetCount()-1; i>=0; --i)
	{
		if(m_lstSelected.GetSel(i)>0)
		{
			T_THIS_K ThisK = m_lstSelected.GetItemData(i);
			aThisK_unselected.Add(ThisK);
			m_lstSelected.DeleteString(i);
		}
	}
	for(int i=0; i<m_lstDefined.GetCount(); i++)
	{
		T_THIS_K ThisK = m_lstDefined.GetItemData(i);
		aThisK_unselected.Add(ThisK);
	}
	CArrayUtil::Sort(aThisK_unselected);

	m_lstDefined.ResetContent();
	for(int i=0; i<aThisK_unselected.GetSize(); i++)
	{
		T_THIS_D ThisD;
		m_pDoc->m_pAttrCtrl->GetThis(aThisK_unselected[i], ThisD);
		m_lstDefined.AddString(ThisD.LoadCaseName);
		m_lstDefined.SetItemData(i, aThisK_unselected[i]);
	}
}