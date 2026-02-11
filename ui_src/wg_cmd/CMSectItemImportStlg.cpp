// CMSectItemImportStlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_attrctrl.h"
#include "CMSectItemImportStlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemImportStlg dialog


CCMSectItemImportStlg::CCMSectItemImportStlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemImportStlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemImportStlg)
	//}}AFX_DATA_INIT
	m_bSetCellTypeShape = FALSE;
	m_bInitialized = FALSE;
	m_nFlangeShape = 0;
	m_nCellNum = 0;
	m_nCellType = 0;
	m_Data.Initialize();
}


void CCMSectItemImportStlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemImportStlg)
	DDX_Control(pDX, IDC_CMD_LIST_SECTION, m_ltbxSection);
	DDX_Control(pDX, IDC_CMD_CHK_INC_OPTION, m_chkIncludeOption);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSectItemImportStlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemImportStlg)
	ON_BN_CLICKED(IDC_CMD_IMPORT, OnCmdImport)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_LBN_SELCHANGE(IDC_CMD_LIST_SECTION, OnSelchangeCmdListSection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemImportStlg message handlers

void CCMSectItemImportStlg::EnableDisableOption()
{
	if (m_bInitialized)
	{
		BOOL bOption = m_csTargetShape!=D_SECT_SHAPE_STLG_B && m_csTargetShape!=D_SECT_SHAPE_STLG_I && m_csTargetShape!=D_SECT_SHAPE_STLG_MCELL;
		if (!bOption) m_chkIncludeOption.SetCheck(FALSE);
		m_chkIncludeOption.EnableWindow(bOption);
	}
}

BOOL CCMSectItemImportStlg::OnInitDialog() 
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

			if(sect_d.nStype == D_SECT_TYPE_STLG_B || sect_d.nStype == D_SECT_TYPE_STLG_I)
			{
				if(sect_d.SectBefore.Shape == m_csTargetShape)
				{
					csItem.Format(_T("%d:%s"), aSectKeys[i], sect_d.SName);
					m_ltbxSection.SetItemData(m_ltbxSection.AddString(csItem), aSectKeys[i]);
				}
			}
			else if(sect_d.nStype == D_SECT_TYPE_STLG_MCELL)
			{
				if(sect_d.SectBefore.Shape == m_csTargetShape)
				{
					if(m_nCellNum == 0)
					{
						if(sect_d.SectBefore.nFlangeShape == m_nFlangeShape &&
							sect_d.SectBefore.nCellNum     == m_nCellNum &&
							sect_d.SectBefore.nCellType    == m_nCellType)
						{
							csItem.Format(_T("%d:%s"), aSectKeys[i], sect_d.SName);
							m_ltbxSection.SetItemData(m_ltbxSection.AddString(csItem), aSectKeys[i]);
						}
					}
					else
					{
						if(sect_d.SectBefore.nFlangeShape == m_nFlangeShape &&
							sect_d.SectBefore.nCellNum     == m_nCellNum)
						{
							csItem.Format(_T("%d:%s"), aSectKeys[i], sect_d.SName);
							m_ltbxSection.SetItemData(m_ltbxSection.AddString(csItem), aSectKeys[i]);
						}
					}
				}				
			}
		}
	//}

	m_chkIncludeOption.SetCheck(m_bIncludeOption[0]);

	// Option Enable/Disable
	m_bInitialized = TRUE;
	EnableDisableOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemImportStlg::OnCmdImport() 
{
	int index = m_ltbxSection.GetCurSel();
	if(index >=0)
	{
		m_SectKey = m_ltbxSection.GetItemData(index);
		m_bIncludeOption[0] = m_chkIncludeOption.GetCheck();
	}
	else
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_section__nChoose_one_));
		return;
	}

	CDialogMove::OnOK();
}

void CCMSectItemImportStlg::OnCancel() 
{
	
	CDialogMove::OnCancel();
}

void CCMSectItemImportStlg::OnSelchangeCmdListSection() 
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
