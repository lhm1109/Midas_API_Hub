// CMSectItemImportPSC.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_attrctrl.h"
#include "CMSectItemImportPSC.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemImportPSC dialog


CCMSectItemImportPSC::CCMSectItemImportPSC(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemImportPSC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemImportPSC)
	//}}AFX_DATA_INIT
	m_bSetCellTypeShape = FALSE;
	m_Data.Initialize();
}


void CCMSectItemImportPSC::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemImportPSC)
	DDX_Control(pDX, IDC_CMD_LIST_SECTION, m_ltbxSection);
	DDX_Control(pDX, IDC_CMD_CHK_INC_OPTION, m_chkIncludeOption);
	DDX_Control(pDX, IDC_CMD_CHK_INC_OPTION2, m_chkIncludeOption2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSectItemImportPSC, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemImportPSC)
	ON_BN_CLICKED(IDC_CMD_IMPORT, OnCmdImport)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_LBN_SELCHANGE(IDC_CMD_LIST_SECTION, OnSelchangeCmdListSection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemImportPSC message handlers

BOOL CCMSectItemImportPSC::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	m_wndSecView.Init(GetDlgItem(IDC_CMD_WND_PREVIEW));

	T_SECT_D sect_d;
	CArray<T_SECT_K,T_SECT_K> aSectKeys;
		
	CString csItem;
	pDoc->m_pAttrCtrl->GetSectKeyList(aSectKeys);

	/*
	if(m_bSetCellTypeShape)
	{
		for(int i=0; i<aSectKeys.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetSect(aSectKeys[i],sect_d);		
			if(sect_d.nStype == D_SECT_TYPE_PSC && sect_d.SectBefore.Shape == m_csTargetShape &&
				sect_d.SectBefore.nCellType == m_nCellType && sect_d.SectBefore.nCellShape == m_nCellShape)
			{
				csItem.Format(_T("%d:%s"), aSectKeys[i], sect_d.SName);
				m_ltbxSection.SetItemData(m_ltbxSection.AddString(csItem), aSectKeys[i]);
			}
		}
	}
	else 
	{
	*/
		for(int i=0; i<aSectKeys.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetSect(aSectKeys[i],sect_d);		
			if(sect_d.nStype == D_SECT_TYPE_PSC && sect_d.SectBefore.Shape == m_csTargetShape)
			{
				csItem.Format(_T("%d:%s"), aSectKeys[i], sect_d.SName);
				m_ltbxSection.SetItemData(m_ltbxSection.AddString(csItem), aSectKeys[i]);
			}
		}

	//}

	m_chkIncludeOption.SetCheck(m_bIncludeOption[0]);
	m_chkIncludeOption2.SetCheck(m_bIncludeOption[1]);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemImportPSC::OnCmdImport() 
{
	int index = m_ltbxSection.GetCurSel();
	if(index >=0)
	{
		m_SectKey = m_ltbxSection.GetItemData(index);
		m_bIncludeOption[0] = m_chkIncludeOption.GetCheck();
		m_bIncludeOption[1] = m_chkIncludeOption2.GetCheck();
	}
	else
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_section__nChoose_one_));
		return;
	}

	CDialogMove::OnOK();
}

void CCMSectItemImportPSC::OnCancel() 
{
	
	CDialogMove::OnCancel();
}

void CCMSectItemImportPSC::OnSelchangeCmdListSection() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ltbxSection.GetCurSel();
	if (nIndex < 0) return;
	m_SectKey = m_ltbxSection.GetItemData(nIndex);
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetSect(m_SectKey, m_Data);
	
	m_wndSecView.SetDataSource(&m_Data);
	m_wndSecView.SetCentroidFlag(TRUE);
	m_wndSecView.Invalidate();
}
