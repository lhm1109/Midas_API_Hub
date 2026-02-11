// CMSectSODDeckStiffenerCopyDlg.cpp : implementation file
//
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectSODDeckStiffenerCopyDlg.h"
#include "CMSectSODDeckStiffenerDlgUtil.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectSODDeckStiffenerCopyDlg dialog

CCMSectSODDeckStiffenerCopyDlg::CCMSectSODDeckStiffenerCopyDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectSODDeckStiffenerCopyDlg::IDD, pParent)
{
	m_Data.Initialize();
	m_nPos=0;
	m_nPart=0;
	m_nDeckPartCnt=0;
	m_bCopyMirror = FALSE;
	m_bWeb2 = FALSE;
	m_arDeckPart.RemoveAll();
	m_arDeckPartSel.RemoveAll();
}


void CCMSectSODDeckStiffenerCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectSODDeckStiffenerCopyDlg)
	DDX_Control(pDX, IDC_CMD_SECT_SOD_DECK_STIFFENER_COPY_LIST,      m_lstStiff);
	DDX_Control(pDX, IDC_CMD_SECT_SOD_DECK_STIFFENER_COPY_SEL_LIST,  m_lstSelStiff);
	DDX_Control(pDX, IDC_CMD_SECT_SOD_DECK_STIFFENER_COPY_MIRROR_CHK, m_chkCopyMirror);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectSODDeckStiffenerCopyDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectSODDeckStiffenerCopyDlg)
	ON_BN_CLICKED(IDC_CMD_SECT_SOD_DECK_STIFFENER_COPY_SEL_BTN,        OnSODSMCopySelBtn)
	ON_BN_CLICKED(IDC_CMD_SECT_SOD_DECK_STIFFENER_COPY_UNSEL_BTN,      OnSODSMCopyUnSelBtn)
	ON_BN_CLICKED(IDC_CMD_SECT_SOD_DECK_STIFFENER_COPY_ALL_BTN,        OnSODSMCopyAllBtn)
	ON_BN_CLICKED(IDC_CMD_SECT_SOD_DECK_STIFFENER_COPY_NONE_BTN,       OnSODSMCopyNoneBtn)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMSectSODDeckStiffenerCopyDlg::SetData(T_SECT_D& Data, int nDeckPos, int nDeckPart, CArray<int, int>& arDeckPart)
{
	m_Data = Data;
	m_nPos = nDeckPos;
	m_nPart = nDeckPart;
	m_arDeckPart.Copy(arDeckPart);
	m_nDeckPartCnt = m_arDeckPart.GetSize();
	
	if(m_Data.nStype == D_SECT_TYPE_STLG_B || 
		m_Data.nStype == D_SECT_TYPE_COMPO_STLG_B ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_B) ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B))
	{
		if(m_nPos==1 || m_nPos==2) m_bWeb2 = TRUE;
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB))
	{
		if(m_nPos==1 || m_nPos==2) m_bWeb2 = TRUE;
	}

}

void CCMSectSODDeckStiffenerCopyDlg::GetData(CArray<int, int>& arDeckPartSel)
{
	arDeckPartSel.RemoveAll();
	arDeckPartSel.Copy(m_arDeckPartSel);
}

void CCMSectSODDeckStiffenerCopyDlg::SetInitList()
{
	int nSize = m_arDeckPart.GetSize();
	if(nSize == 0) return;

	CString strName = _T("");
	if(m_bWeb2)
	{
		for(int i=0; i<nSize; i++) 
		{
			int nDeckPos = (m_nPos==1)? 2 :1;

			CString strDeckName;
			CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, nDeckPos, m_nPart, m_nDeckPartCnt, &strDeckName);

			strName.Format(_T("%s"), strDeckName);
			m_lstStiff.SetItemData(m_lstStiff.AddString(strName), nDeckPos);
		}
	}
	else
	{
		for(int i=0; i<nSize; i++) 
		{
			int nDeckPart = m_arDeckPart[i];
			if(m_nPart==nDeckPart) continue;

			CString strDeckName;
			CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, m_nPos, nDeckPart, m_nDeckPartCnt, &strDeckName);

			strName.Format(_T("%s"), strDeckName);
			m_lstStiff.SetItemData(m_lstStiff.AddString(strName), nDeckPart);
		}
	}
}

void CCMSectSODDeckStiffenerCopyDlg::InitCtrls()
{
	BOOL bWeb = FALSE;
	if(m_Data.nStype == D_SECT_TYPE_STLG_B || 
		 m_Data.nStype == D_SECT_TYPE_COMPO_STLG_B ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_B) ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B))
	{
		if(m_nPos==1 || m_nPos==2) bWeb = TRUE;
	}
	else if(m_Data.nStype == D_SECT_TYPE_STLG_I || m_Data.nStype == D_SECT_TYPE_COMPO_STLG_I ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_I) ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I))
	{
		if(m_nPos==1) bWeb = TRUE;
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB))
	{
		if(m_nPos==1 || m_nPos==2) bWeb = TRUE;
	}
	else if(m_Data.nStype == D_SECT_TYPE_STLG_MCELL ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_MCELL))
	{
		if(m_nPos==1) bWeb = TRUE;
	}
	else
	{
		ASSERT(0);
	}

	GetDlgItem(IDC_CMD_SECT_SOD_DECK_STIFFENER_COPY_MIRROR_CHK)->ShowWindow(!bWeb);
}

BOOL CCMSectSODDeckStiffenerCopyDlg::GetUnSelData(CArray<int, int&>& aKey, BOOL bAll)
{
	int nLcomCount = m_lstSelStiff.GetCount();
	if(nLcomCount<1) return FALSE;

	aKey.RemoveAll();

	// 기존 Lcomion list에 존재하는 items
	int nSelCount = m_lstStiff.GetCount();
	for(int i=0; i<nSelCount; i++)
	{
		int Key;
		Key = m_lstStiff.GetItemData(i);
		aKey.Add(Key);
	}

	// Selected Lcomion list에서 선택된 items
	CArray<int, int> aSelItem;
	if(!bAll)
	{
		nLcomCount = m_lstSelStiff.GetSelCount();
		aSelItem.SetSize(nLcomCount);
		m_lstSelStiff.GetSelItems(nLcomCount, aSelItem.GetData()); 
	}

	for(int i=0; i<nLcomCount; i++)
	{
		int Key;
		int nIndex = bAll ? i : aSelItem[i];
		Key = m_lstSelStiff.GetItemData(nIndex);
		aKey.Add(Key);
	}

	if(bAll) m_lstSelStiff.ResetContent();
	else
	{
		for(int i=0; i<nLcomCount; i++)
			m_lstSelStiff.DeleteString(aSelItem[i]-i);
	}

	return TRUE;
}

BOOL CCMSectSODDeckStiffenerCopyDlg::GetSelData(CArray<int, int&>& aKey, BOOL bAll)
{
	int nLcomCount = m_lstStiff.GetCount();
	if(nLcomCount<1) return FALSE;

	aKey.RemoveAll();

	// 기존 Selected Lcomion list 에 존재하는 items
	int nSelCount = m_lstSelStiff.GetCount();
	for(int i=0; i<nSelCount; i++)
	{
		int Key;
		Key = m_lstSelStiff.GetItemData(i);
		aKey.Add(Key);
	}

	// Lcomion list에서 선택된 items
	CArray<int, int> aSelItem;
	if(!bAll)
	{
		nLcomCount = m_lstStiff.GetSelCount();
		aSelItem.SetSize(nLcomCount);
		m_lstStiff.GetSelItems(nLcomCount, aSelItem.GetData()); 
	}

	for(int i=0; i<nLcomCount; i++)
	{
		int Key;
		int nIndex = bAll ? i : aSelItem[i];
		Key = m_lstStiff.GetItemData(nIndex);
		aKey.Add(Key);
	}

	if(bAll) m_lstStiff.ResetContent();
	else
	{
		for(int i=0; i<nLcomCount; i++)
			m_lstStiff.DeleteString(aSelItem[i]-i);
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CCMSectSODDeckStiffenerCopyDlg message handlers

BOOL CCMSectSODDeckStiffenerCopyDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitCtrls();
	SetInitList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectSODDeckStiffenerCopyDlg::OnOK() 
{
	int nCount = m_lstSelStiff.GetCount();
	if(nCount>0)
	{
		m_arDeckPartSel.RemoveAll();
		m_arDeckPartSel.SetSize(nCount);

		for(int i=0; i<nCount; ++i)
		{
			m_arDeckPartSel[i] = m_lstSelStiff.GetItemData(i);
		}
	}

	m_bCopyMirror = m_chkCopyMirror.GetCheck();


	CDialogMove::OnOK();
}

void CCMSectSODDeckStiffenerCopyDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CCMSectSODDeckStiffenerCopyDlg::OnSODSMCopySelBtn() 
{
	CArray<int, int&> aKey;
	if(!GetSelData(aKey, FALSE)) return;

	int nSize = aKey.GetSize();
	qsort((void*)aKey.GetData(), nSize, sizeof(int), CCompFunc::UINTAsc);

	m_lstSelStiff.ResetContent();

	CString strName = _T("");
	if(m_bWeb2)
	{
		for(int i=0; i<nSize; i++) 
		{
			int nDeckPos = aKey[i];

			CString strDeckName;
			CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, nDeckPos, m_nPart, m_nDeckPartCnt, &strDeckName);

			strName.Format(_T("%s"), strDeckName);
			m_lstSelStiff.SetItemData(m_lstSelStiff.AddString(strName), nDeckPos);
		}
	}
	else
	{
		for(int i=0; i<nSize; i++) 
		{
			int nDeckPart = aKey[i];
			if(m_nPart==nDeckPart) continue;

			CString strDeckName;
			CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, m_nPos, nDeckPart, m_nDeckPartCnt, &strDeckName);

			strName.Format(_T("%s"), strDeckName);
			m_lstSelStiff.SetItemData(m_lstSelStiff.AddString(strName), nDeckPart);
		}
	}
}

void CCMSectSODDeckStiffenerCopyDlg::OnSODSMCopyUnSelBtn() 
{
	CArray<int, int&> aKey;
	if(!GetUnSelData(aKey, FALSE)) return;

	int nSize = aKey.GetSize();
	qsort((void*)aKey.GetData(), nSize, sizeof(int), CCompFunc::UINTAsc);

	m_lstStiff.ResetContent();

	CString strName = _T("");
	if(m_bWeb2)
	{
		for(int i=0; i<nSize; i++) 
		{
			int nDeckPos = aKey[i];

			CString strDeckName;
			CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, nDeckPos, m_nPart, m_nDeckPartCnt, &strDeckName);

			strName.Format(_T("%s"), strDeckName);
			m_lstStiff.SetItemData(m_lstStiff.AddString(strName), nDeckPos);
		}
	}
	else
	{
		for(int i=0; i<nSize; i++) 
		{
			int nDeckPart = aKey[i];
			if(m_nPart==nDeckPart) continue;

			CString strDeckName;
			CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, m_nPos, nDeckPart, m_nDeckPartCnt, &strDeckName);

			strName.Format(_T("%s"), strDeckName);
			m_lstStiff.SetItemData(m_lstStiff.AddString(strName), nDeckPart);
		}
	}

}

void CCMSectSODDeckStiffenerCopyDlg::OnSODSMCopyAllBtn() 
{
	CArray<int, int&> aKey;
	if(!GetSelData(aKey, TRUE)) return;

	int nSize = aKey.GetSize();
	qsort((void*)aKey.GetData(), nSize, sizeof(int), CCompFunc::UINTAsc);

	m_lstSelStiff.ResetContent();

	CString strName = _T("");
	if(m_bWeb2)
	{
		for(int i=0; i<nSize; i++) 
		{
			int nDeckPos = aKey[i];
 
			CString strDeckName;
			CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, nDeckPos, m_nPart, m_nDeckPartCnt, &strDeckName);

			strName.Format(_T("%s"), strDeckName);
			m_lstSelStiff.SetItemData(m_lstSelStiff.AddString(strName), nDeckPos);
		}
	}
	else
	{
		for(int i=0; i<nSize; i++) 
		{
			int nDeckPart = aKey[i];
			if(m_nPart==nDeckPart) continue;

			CString strDeckName;
			CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, m_nPos, nDeckPart, m_nDeckPartCnt, &strDeckName);

			strName.Format(_T("%s"), strDeckName);
			m_lstSelStiff.SetItemData(m_lstSelStiff.AddString(strName), nDeckPart);
		}
	}
}

void CCMSectSODDeckStiffenerCopyDlg::OnSODSMCopyNoneBtn() 
{
	CArray<int, int&> aKey;
	if(!GetUnSelData(aKey, TRUE)) return;

	int nSize = aKey.GetSize();
	qsort((void*)aKey.GetData(), nSize, sizeof(int), CCompFunc::UINTAsc);

	m_lstStiff.ResetContent();

	CString strName = _T("");
	if(m_bWeb2)
	{
		for(int i=0; i<nSize; i++) 
		{
			int nDeckPos = aKey[i];

			CString strDeckName;
			CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, nDeckPos, m_nPart, m_nDeckPartCnt, &strDeckName);

			strName.Format(_T("%s"), strDeckName);
			m_lstStiff.SetItemData(m_lstStiff.AddString(strName), nDeckPos);
		}
	}
	else
	{
		for(int i=0; i<nSize; i++) 
		{
			int nDeckPart = aKey[i];
			if(m_nPart==nDeckPart) continue;

			CString strDeckName;
			CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, m_nPos, nDeckPart, m_nDeckPartCnt, &strDeckName);

			strName.Format(_T("%s"), strDeckName);
			m_lstStiff.SetItemData(m_lstStiff.AddString(strName), nDeckPart);
		}
	}
}
