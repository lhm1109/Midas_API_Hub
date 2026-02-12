// CMSelectSect.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSelectSect.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSelectSect dialog


CCMSelectSect::CCMSelectSect(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSelectSect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSelectSect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_mSectType.RemoveAll();
	m_mSectType.InitHashTable(101);
	m_aPSCShape.RemoveAll();
	m_Key = 0;
}


void CCMSelectSect::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSelectSect)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSelectSect, CDialogMove)
	//{{AFX_MSG_MAP(CCMSelectSect)
	ON_LBN_DBLCLK(IDC_CMD_LIST, OnDblclkCmdList)
	ON_LBN_SELCHANGE(IDC_CMD_LIST, OnSelchangeCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMSelectSect::SetSectType(const CArray<int, int> &aType)
{
	m_mSectType.RemoveAll();
	int nSize = aType.GetSize();
	for(int i=0; i<nSize; ++i)
	{
		m_mSectType.SetAt(aType[i], TRUE);
	}
}

void CCMSelectSect::SetPSCShape(const CArray<CString, CString&> &aShape)
{
	m_mSectType.RemoveAll();
	m_mSectType.SetAt(D_SECT_TYPE_PSC, TRUE);
	m_aPSCShape.Copy(aShape);
}

void CCMSelectSect::ResetList()
{
	m_List.ResetContent();

	CAttrCtrl *pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	
	CArray<CString, CString&>strSectList;
	CArray<T_SECT_K, T_SECT_K>SectKeyList;
	pAttrCtrl->GetSectKeyList(SectKeyList);
	pAttrCtrl->GetSectList(strSectList);

	int nSect=strSectList.GetSize();
	CString strSectName = _T("");

	int nType = m_mSectType.GetCount();
	if(nType<=0)
	{
		for(int i=0; i<nSect; i++)
		{
			strSectName.Format(_T("%d: "),SectKeyList[i]);
			m_List.SetItemData(m_List.AddString(strSectName+strSectList[i]), SectKeyList[i]);
		}
	}
	else  // Type이 설정되었을 때 
	{
		BOOL bTmp = TRUE;
		int  nShapeSize = m_aPSCShape.GetSize();
		T_SECT_D SectD;
		for(int i=0; i<nSect; i++)
		{
			if(!pAttrCtrl->GetSect(SectKeyList[i], SectD)) continue;
			if(!m_mSectType.Lookup(SectD.nStype, bTmp)) continue;

			if(nShapeSize>0)
			{
				BOOL bFind = FALSE;
				for(int j=0; j<nShapeSize; ++j)
				{
					if(SectD.SectBefore.Shape!=m_aPSCShape[j]) continue;
					bFind = TRUE;
					break;
				}
				if(!bFind) continue;
			}

			strSectName.Format(_T("%d: "),SectKeyList[i]);
			m_List.SetItemData(m_List.AddString(strSectName+strSectList[i]), SectKeyList[i]);
		}
	}
	if(nSect>0)
	{
		m_List.SetCurSel(0);
		m_Key = m_List.GetItemData(0);
		m_List.Invalidate();
	}
}

UINT CCMSelectSect::GetSelectedSectK()
{
	return m_Key;
}
/////////////////////////////////////////////////////////////////////////////
// CCMSelectSect message handlers

BOOL CCMSelectSect::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	ResetList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSelectSect::OnDblclkCmdList() 
{
	m_Key = m_List.GetItemData(m_List.GetCurSel());
	CDialogMove::OnOK();
}

void CCMSelectSect::OnSelchangeCmdList() 
{
	m_Key = m_List.GetItemData(m_List.GetCurSel());
}
