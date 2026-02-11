// CMMvhcItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhcItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\VehlDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhcItemDlg dialog

CCMMvhcItemDlg::CCMMvhcItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhcItemDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	//{{AFX_DATA_INIT(CCMMvhcItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCMMvhcItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhcItemDlg)
	DDX_Control(pDX, IDC_CMD_MVHC_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_MVHC_UNSEL_LIST, m_wndUnselList);
	DDX_Control(pDX, IDC_CMD_MVHC_SEL_LIST, m_wndSelList);
	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvhcItemDlg::Data2Dlg()
{
	m_wndName.SetWindowText(m_Data.VehicleClassName);

	// Select 된 것, 아닌 것 구분
	int nAllSize = m_aMvhlList.GetSize();
	int nSelSize = m_Data.arVehicleLoad.GetSize();
	m_aSelFlag.SetSize(nAllSize);
	for (int i = 0; i < nAllSize; i++)
	{
		int j = 0;
		for (j = 0; j < nSelSize; j++)
			if (m_aMvhlList[i] == m_Data.arVehicleLoad[j]) break;
		if (j == nSelSize) m_aSelFlag[i] = FALSE;
		else m_aSelFlag[i] = TRUE;
	}

	int nIndex;
	T_MVHL_D mvhl;

	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();

	// Add String
	for (int i = 0; i < nAllSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetMvhl(m_aMvhlList[i], mvhl);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(mvhl.VehicleLoadName);
			m_wndSelList.SetItemData(nIndex, i);
		}
		else
		{
			nIndex = m_wndUnselList.AddString(mvhl.VehicleLoadName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}

BOOL CCMMvhcItemDlg::Dlg2Data()
{
	m_Data.Initialize();

	m_wndName.GetWindowText(m_Data.VehicleClassName);
	m_Data.arVehicleLoad.SetSize(m_wndSelList.GetCount());
	int nSize = m_aMvhlList.GetSize();
	int nCount = 0;
	for (int i = 0; i < nSize; i++)
	{
		if (m_aSelFlag[i]) m_Data.arVehicleLoad[nCount++] = m_aMvhlList[i];
	}
	return TRUE;
}

BOOL CCMMvhcItemDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhc(m_csOldName, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddMvhc(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMMvhcItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhcItemDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_LBN_DBLCLK(IDC_CMD_MVHC_SEL_LIST, OnDblclkCmdMvhcSelList)
	ON_LBN_DBLCLK(IDC_CMD_MVHC_UNSEL_LIST, OnDblclkCmdMvhcUnselList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhcItemDlg message handlers

BOOL CCMMvhcItemDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!m_bModify)
	{
		m_Data.Initialize();
	}

	T_MVCD_D MvcdD;
	if (!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) { ASSERT(0); return FALSE; }

	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(m_aMvhlList);

	// Permit Vehicle 뺀다.
	T_MVHL_D MvhlD;
	for (int i = m_aMvhlList.GetSize()-1; i >= 0; i--)
	{
		if (!m_pDoc->m_pAttrCtrl->GetMvhl(m_aMvhlList[i], MvhlD)) { ASSERT(0); continue; }
		if ( m_pDoc->m_pAttrCtrl->IsPermitVehicle(MvhlD)) m_aMvhlList.RemoveAt(i);

		if (MvcdD.nCodeType == D_MOVE_CODE_BS)
		{
			if (!CVehlDB::IsBelongToVType(MvhlD, CVehlDB::enVehicleType::CS454_LM1)) m_aMvhlList.RemoveAt(i);
		}
	}

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhcItemDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	int nMax = m_aMvhlList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndUnselList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndUnselList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = TRUE;
		m_wndUnselList.DeleteString(aIndex[i]);
	}
	
	T_MVHL_D mvhl;
	m_wndSelList.ResetContent();
	int nAllSize = m_aMvhlList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetMvhl(m_aMvhlList[i], mvhl);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(mvhl.VehicleLoadName);
			m_wndSelList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvhcItemDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	int nMax = m_aMvhlList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_wndSelList.DeleteString(aIndex[i]);
	}

	T_MVHL_D mvhl;
	m_wndUnselList.ResetContent();
	int nAllSize = m_aMvhlList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetMvhl(m_aMvhlList[i], mvhl);
		if (!m_aSelFlag[i]) 
		{
			nIndex = m_wndUnselList.AddString(mvhl.VehicleLoadName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvhcItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvhcItemDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();	
}

void CCMMvhcItemDlg::OnDblclkCmdMvhcSelList() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnDelete();
}

void CCMMvhcItemDlg::OnDblclkCmdMvhcUnselList() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnAdd();		
}
