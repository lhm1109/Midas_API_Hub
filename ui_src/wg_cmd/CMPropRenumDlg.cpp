// CMPropRenumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMPropRenumDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMPropRenumDlg dialog


CCMPropRenumDlg::CCMPropRenumDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMPropRenumDlg::IDD, pParent)
{
	m_pDoc = NULL;
	//{{AFX_DATA_INIT(CCMPropRenumDlg)
	m_bElemRenum = FALSE;
	m_nStart = 0;
	//}}AFX_DATA_INIT
}


void CCMPropRenumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMPropRenumDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//DDX_Control(pDX, IDC_CMD_SPIN, m_Spin);
	DDX_Check(pDX, IDC_CMD_CHK_ELEM_RENUM, m_bElemRenum);
	DDX_Control(pDX, IDC_CMD_EDT_INCR, m_edtIncrement);
	DDX_Text(pDX, IDC_CMD_EDT_INCR, m_nIncrement);
	DDX_Text(pDX, IDC_CMD_EDT_START, m_nStart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMPropRenumDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMPropRenumDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_BN_CLICKED(IDC_CMD_BTN_RENUM, OnCmdBtnRenum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMPropRenumDlg message handlers

BOOL CCMPropRenumDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	CDialogMove::OnInitDialog();

	//Dialog caption setting
	if      (m_nPropNum == 0) this -> SetWindowText(_LS(IDS_CMD_RENUM_WND_MATL));
	else if (m_nPropNum == 1) this -> SetWindowText(_LS(IDS_CMD_RENUM_WND_SECT));
	else                      this -> SetWindowText(_LS(IDS_CMD_RENUM_WND_THIK));

	//Check box caption setting
	CWnd* pWnd = GetDlgItem(IDC_CMD_CHK_ELEM_RENUM);
	if      (m_nPropNum == 0) pWnd -> SetWindowText(_LS(IDS_CMD_RENUM_CHK_MATL));
	else if (m_nPropNum == 1) pWnd -> SetWindowText(_LS(IDS_CMD_RENUM_CHK_SECT));
	else                      pWnd -> SetWindowText(_LS(IDS_CMD_RENUM_CHK_THIK));

	m_nStart = 1;
	//m_Spin.SetRange(1, 100);
	m_edtIncrement.SetRange(1, 100);
	m_nIncrement = 1;

	UpdateData(FALSE);

	InitList();

	return TRUE;  
}

void CCMPropRenumDlg::OnCmdBtnClose() 
{
	CDialogMove::OnOK();
}

void CCMPropRenumDlg::OnCmdBtnRenum() 
{
	UpdateData(TRUE);

	CArray<int,int> arSelList;
	int nCount = m_List.GetSelCount();
	arSelList.SetSize(nCount);
	m_List.GetSelItems(nCount,arSelList.GetData());

	if(m_nPropNum == 0) // Material Number
	{ 
		CArray<T_MATL_K,T_MATL_K> arMatlKey;
		T_MATL_K MatlKey;
	
		for( int i = 0 ; i < nCount ; i++)
		{
			MatlKey = m_List.GetItemData(arSelList[i]);
			if(MatlKey) arMatlKey.Add(MatlKey);
		}
		m_pDoc->m_pDataCtrl->RenumberMatlNo(arMatlKey, m_nStart, m_nIncrement, m_bElemRenum);
	}
	else if(m_nPropNum == 1 ) // Section Number
	{
		CArray<T_SECT_K,T_SECT_K> arSectKey;
		T_SECT_K SectKey;
	
		for( int i = 0 ; i < nCount ; i++)
		{
			SectKey = m_List.GetItemData(arSelList[i]);
			if(SectKey) arSectKey.Add(SectKey);
		}
		m_pDoc->m_pDataCtrl->RenumberSectNo(arSectKey, m_nStart, m_nIncrement, m_bElemRenum);
	}
	else // Thickness Number
	{
		CArray<T_THIK_K,T_THIK_K> arThikKey;
		T_THIK_K ThikKey;
	
		for( int i = 0 ; i < nCount ; i++)
		{
			ThikKey = m_List.GetItemData(arSelList[i]);
			if(ThikKey) arThikKey.Add(ThikKey);
		}
		m_pDoc->m_pDataCtrl->RenumberThikNo(arThikKey, m_nStart, m_nIncrement, m_bElemRenum);
	}

	InitList();
	
}

void CCMPropRenumDlg::InitList()
{
	m_List.ResetContent();

	CString csItem;
	int nIndex = 0;

	if(m_nPropNum == 0) // Material
	{
		T_MATL_K MatlKey;
		T_MATL_D MatlData;
		CArray<T_MATL_K, T_MATL_K> arMatlKey;
		MatlData.Initialize();

		m_pDoc->m_pAttrCtrl->GetMatlKeyList(arMatlKey);
		
		int nSize = arMatlKey.GetSize();
		for(int i=0 ; i < nSize ; i++)
		{
			MatlKey = arMatlKey[i];
		  if(m_pDoc->m_pAttrCtrl->GetMatl(MatlKey, MatlData))
				csItem.Format(_T("%d : %s"), MatlKey, MatlData.Name);
			else
				csItem.Format(_T("%d : "), MatlKey);
			nIndex = m_List.AddString(csItem);
			m_List.SetItemData(nIndex, (DWORD)MatlKey);
		}
	}
	else if(m_nPropNum == 1) // Section
	{
		T_SECT_K SectKey;
		T_SECT_D SectData;
		CArray<T_SECT_K, T_SECT_K> arSectKey;
		SectData.Initialize();

		m_pDoc->m_pAttrCtrl->GetSectKeyList(arSectKey);
		
		int nSize = arSectKey.GetSize();
		for(int i=0 ; i < nSize ; i++)
		{
			SectKey = arSectKey[i];
		  if(m_pDoc->m_pAttrCtrl->GetSect(SectKey, SectData))
				csItem.Format(_T("%d : %s"), SectKey, SectData.SName);
			else
				csItem.Format(_T("%d : "), SectKey);
			nIndex = m_List.AddString(csItem);
			m_List.SetItemData(nIndex, (DWORD)SectKey);
		}
	}
	else // Thickness
	{
		T_THIK_K ThikKey;
		T_THIK_D ThikData;
		CArray<T_THIK_K, T_THIK_K> arThikKey;
		ThikData.Initialize();

		m_pDoc->m_pAttrCtrl->GetThikKeyList(arThikKey);
		
		int nSize = arThikKey.GetSize();

		for(int i=0 ; i < nSize ; i++)
		{
			ThikKey = arThikKey[i];
		  if(m_pDoc->m_pAttrCtrl->GetThik(ThikKey, ThikData))
				csItem.Format(_T("%d : %g"), ThikKey, ThikData.Value.ThickIn);
			else
				csItem.Format(_T("%d : "), ThikKey);
			nIndex = m_List.AddString(csItem);
			m_List.SetItemData(nIndex, (DWORD)ThikKey);
		}
	}
}
	

