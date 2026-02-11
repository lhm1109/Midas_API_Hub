// CMSectItemImportReg.cpp : implementation file
//


#include "stdafx.h"
#include "wg_cmd.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_attrctrl.h"
#include "CMSectItemImportReg.h"
#include "..\wg_db\DB_ST_DT_SECT.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemImportReg dialog


CCMSectItemImportReg::CCMSectItemImportReg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemImportReg::IDD, pParent)
{
	m_Data.Initialize();
	//{{AFX_DATA_INIT(CCMSectItemImportReg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectItemImportReg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemImportReg)
	DDX_Control(pDX, IDC_CMD_LIST_SECTION, m_ltbxSection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemImportReg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemImportReg)
	ON_BN_CLICKED(IDC_CMD_IMPORT, OnCmdImport)
	ON_LBN_SELCHANGE(IDC_CMD_LIST_SECTION, OnSelchangeCmdListSection)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemImportReg message handlers

void CCMSectItemImportReg::OnCmdImport() 
{
	// TODO: Add your control notification handler code here
	int index = m_ltbxSection.GetCurSel();
	if(index >=0)
	{
		m_SectKey = m_ltbxSection.GetItemData(index);
	}
	else
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_section__nChoose_one_));
		return;
	}
	//CDialogMove::OnOK(); OnSelchangeCmdListSection쪽으로 이동
	
}

void CCMSectItemImportReg::OnSelchangeCmdListSection() 
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
	
	CDialogMove::OnOK();
}

void CCMSectItemImportReg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

BOOL CCMSectItemImportReg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	m_wndSecView.Init(GetDlgItem(IDC_CMD_WND_PREVIEW));
	T_SECT_D sect_d;
	CArray<T_SECT_K,T_SECT_K> aSectKeys;
	CString csItem;
	pDoc->m_pAttrCtrl->GetSectKeyList(aSectKeys);
	for(int i=0; i<aSectKeys.GetSize(); i++)
	{
		BOOL bAddSect = FALSE;
		pDoc->m_pAttrCtrl->GetSect(aSectKeys[i],sect_d);
		if(m_csTargetShape==D_SECT_SHAPE_COMPO_G)
		{
			if(sect_d.nStype == D_SECT_TYPE_COMPO_G)
				bAddSect = TRUE;
		}
		else // 기존 - D_SECT_SHAPE_REG_GEN
		{
			if(sect_d.nStype == D_SECT_TYPE_USER && sect_d.SectBefore.Shape == m_csTargetShape)
				bAddSect = TRUE;
		}

		if(bAddSect)
		{
			csItem.Format(_T("%d:%s"), aSectKeys[i], sect_d.SName);
			m_ltbxSection.SetItemData(m_ltbxSection.AddString(csItem), aSectKeys[i]);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
