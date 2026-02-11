// CMTendonChangePropDlg.cpp : implementation file
//
// 2003. 09. 26    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTendonChangePropDlg.h"

#include "CMTendonTypeDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTendonChangePropDlg dialog


CCMTendonChangePropDlg::CCMTendonChangePropDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMTendonChangePropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTendonChangePropDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_aSelectedProfiles.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

}

void CCMTendonChangePropDlg::SetSelectedProfiles(CArray<UINT, UINT> &aSelectedK)
{
	m_aSelectedProfiles.Copy(aSelectedK);
}

void CCMTendonChangePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTendonChangePropDlg)
	DDX_Control(pDX, IDC_CMD_PROPERTY_COMBO, m_cobxProperty);
	DDX_Control(pDX, IDC_CMD_SELECTED_LIST, m_listSelProfiles);
	DDX_Control(pDX, IDC_CMD_ALL_LIST, m_listAllProfiles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTendonChangePropDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMTendonChangePropDlg)
	ON_BN_CLICKED(IDC_CMD_SEL_BUTTON, OnCmdSelButton)
	ON_BN_CLICKED(IDC_CMD_UNSEL_BUTTON, OnCmdUnselButton)
	ON_BN_CLICKED(IDC_CMD_PROPERTY_BUTTON, OnCmdPropertyButton)
	ON_LBN_DBLCLK(IDC_CMD_ALL_LIST, OnDblclkCmdAllList)
	ON_LBN_DBLCLK(IDC_CMD_SELECTED_LIST, OnDblclkCmdSelectedList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTendonChangePropDlg message handlers

BOOL CCMTendonChangePropDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitAllList();
	InitSelList();
	InitPropCombo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTendonChangePropDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!DoChangeProperty()) return;

	CDialogMove::OnOK();
}

void CCMTendonChangePropDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMTendonChangePropDlg::OnCmdSelButton() 
{
	// TODO: Add your control notification handler code here
	DoSelectItem();
}

void CCMTendonChangePropDlg::OnCmdUnselButton() 
{
	// TODO: Add your control notification handler code here
	DoUnselectItem();
}

void CCMTendonChangePropDlg::OnDblclkCmdAllList() 
{
	// TODO: Add your control notification handler code here
	DoSelectItem();
}

void CCMTendonChangePropDlg::OnDblclkCmdSelectedList() 
{
	// TODO: Add your control notification handler code here
	DoUnselectItem();
}

void CCMTendonChangePropDlg::OnCmdPropertyButton() 
{
	// TODO: Add your control notification handler code here
	CCMTendonTypeDlg dlg;
	dlg.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMTendonChangePropDlg::InitAllList()
{
	m_listAllProfiles.ResetContent();

	int ix;
	T_TDNA_D data;
	CArray<T_TDNA_K, T_TDNA_K> aKey;
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(aKey);
	int nNum = aKey.GetSize();
	for (int i=0; i<nNum; i++)
	{
		int k = 0;
		for (k=0; k<m_aSelectedProfiles.GetSize(); k++)
			if (m_aSelectedProfiles[k] == aKey[i]) break;
		if (k == m_aSelectedProfiles.GetSize())
		{
			m_pDoc->m_pAttrCtrl->GetTdna(aKey[i], data);
			ix = m_listAllProfiles.AddString(data.TendonName);
			m_listAllProfiles.SetItemData(ix, aKey[i]);
		}
	}
	if (m_listAllProfiles.GetCount() > 0)
		m_listAllProfiles.SetCurSel(0);
}

void CCMTendonChangePropDlg::InitSelList()
{
	m_listSelProfiles.ResetContent();

	int ix;
	T_TDNA_K key;
	T_TDNA_D data;

	int nNum = m_aSelectedProfiles.GetSize();
	for (int i=0; i<nNum; i++)
	{
		key = m_aSelectedProfiles.GetAt(i);
		m_pDoc->m_pAttrCtrl->GetTdna(key, data);

		ix = m_listSelProfiles.AddString(data.TendonName);
		m_listSelProfiles.SetItemData(ix, key);
	}
	if (nNum > 0) m_listSelProfiles.SetCurSel(0);
}

void CCMTendonChangePropDlg::InitPropCombo()
{
	m_cobxProperty.ResetContent();

	int ix;
	T_TDNT_D data;
	CArray<T_TDNT_K, T_TDNT_K> aKey;
	m_pDoc->m_pAttrCtrl->GetTdntKeyList(aKey);
	int nNum = aKey.GetSize();
	for (int i=0; i<nNum; i++)
	{
		m_pDoc->m_pAttrCtrl->GetTdnt(aKey[i], data);
		ix = m_cobxProperty.AddString(data.TendonTypeName);
		m_cobxProperty.SetItemData(ix, aKey[i]);
	}
	if (nNum > 0) m_cobxProperty.SetCurSel(0);
}

void CCMTendonChangePropDlg::DoSelectItem()
{
	int nCount = m_listAllProfiles.GetSelCount();
	if (nCount <= 0) return;

	T_TDNA_K key;
	CArray<int, int> aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listAllProfiles.GetSelItems(nCount, aSelectedItem.GetData()); 

	for (int i=0; i<nCount; i++)
	{
		key = m_listAllProfiles.GetItemData(aSelectedItem[i]);
		m_aSelectedProfiles.Add(key);
	}

	InitAllList();
	InitSelList();
}

void CCMTendonChangePropDlg::DoUnselectItem()
{
	int nCount = m_listSelProfiles.GetSelCount();
	if (nCount <= 0) return;

	T_TDNA_K key;
	CArray<int, int> aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listSelProfiles.GetSelItems(nCount, aSelectedItem.GetData()); 

	for (int i=m_aSelectedProfiles.GetSize()-1; i>=0; i--)
	{
		int k = 0;
		for (k=0; k<nCount; k++)
		{
			key = m_listSelProfiles.GetItemData(aSelectedItem[k]);
			if (key == m_aSelectedProfiles[i]) break;
		}
		if (k != nCount) m_aSelectedProfiles.RemoveAt(i);
	}

	InitAllList();
	InitSelList();
}

BOOL CCMTendonChangePropDlg::DoChangeProperty()
{
	int ix = m_cobxProperty.GetCurSel();
	if (ix == CB_ERR)
	{
		AfxMessageBox(_LS(IDS_CMD_ERR_Select_Tendon_Property));
		return FALSE;
	}

	int nNum = m_aSelectedProfiles.GetSize();
	if (nNum <= 0) return TRUE;

	T_TDNA_K key;
	T_TDNA_D data;
	CArray<T_TDNA_D, T_TDNA_D&> aModifyData;
	T_TDNT_K PropKey = m_cobxProperty.GetItemData(ix);
	for (int i=0; i<nNum; i++)
	{
		key = m_aSelectedProfiles.GetAt(i);
		m_pDoc->m_pAttrCtrl->GetTdna(key, data);
		data.TendonTypeKey = PropKey;
		aModifyData.Add(data);
	}

	if (!m_pDoc->m_pDataCtrl->ModifyTdna(m_aSelectedProfiles, m_aSelectedProfiles, aModifyData)) return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// override 

void CCMTendonChangePropDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
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

void CCMTendonChangePropDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_TDNT_ADD):
		case(UR_TDNT_DEL):
		case(UR_TDNT_MFD):
		case(UR_TDNT_MFS):
			bChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if (bChanged)
	{
		T_TDNT_K oldKey;
		int ix = m_cobxProperty.GetCurSel();
		if (ix != CB_ERR)
			oldKey = m_cobxProperty.GetItemData(ix);

		InitPropCombo();

		if (ix != CB_ERR)
		{
			int nNum = m_cobxProperty.GetCount();
			int i = 0;
			for (i=0; i<nNum; i++)
				if (m_cobxProperty.GetItemData(i) == oldKey) break;
			if (i != nNum) m_cobxProperty.SetCurSel(i);
		}
	}
}



