// CMSectItemPSCImport.cpp : implementation file
//
// PSC-Value 단면에서 import 버튼 클릭시 본 다이얼로그가 뜨며 이 다이얼로그에서는 
// CCMSectImportSPC 클래스에서 import한 단면목록을 리스트에 띄워 유저로보터 선택할 수 있게 한다.

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCImport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCImport dialog


CCMSectItemPSCImport::CCMSectItemPSCImport(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemPSCImport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCImport)
	//}}AFX_DATA_INIT

	m_nCurID = 0;
	m_aAllID.RemoveAll();
	m_aAllData.RemoveAll();
	m_bCompositeMode = FALSE;
}


void CCMSectItemPSCImport::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCImport)
	DDX_Control(pDX, IDC_CMD_ALL_LIST, m_wndList);
	//}}AFX_DATA_MAP
}

void CCMSectItemPSCImport::SetData(const CArray<UINT, UINT> &raID, const CArray<T_SECT_D, T_SECT_D&> &raData, int nCurID)
{
	ASSERT(raID.GetSize() == raData.GetSize());
	m_aAllID.Copy(raID);
	m_aAllData.Copy(raData);
	m_nCurID = nCurID;
}

BOOL CCMSectItemPSCImport::GetData(UINT &rCurID, T_SECT_D& rData)
{
	rCurID = m_nCurID;
	int i = 0;
	for (i = 0; i < m_aAllID.GetSize(); i++)
		if (m_aAllID[i] == m_nCurID) break;
	if (i == m_aAllID.GetSize()) return FALSE;
	rData = m_aAllData[i];
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMSectItemPSCImport, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemPSCImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCImport message handlers

BOOL CCMSectItemPSCImport::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemPSCImport::OnOK() 
{
	// TODO: Add extra validation here

	if (!SaveData()) return;

	CDialogMove::OnOK();
}

void CCMSectItemPSCImport::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

//-----------------------------------------------------------------
void CCMSectItemPSCImport::SetList()
{
	m_wndList.ResetContent();
	
	int nCount = m_aAllID.GetSize();
	if (nCount < 1) return ;
	
	CString str;
	int ix, curix = 0;
	BOOL bWrongTypeFound = FALSE;
	for (int i = 0 ; i < nCount; i++)
	{
		// 20080223 mylee
		if(m_bCompositeMode)
		{
			if(m_aAllData[i].nStype != D_SECT_TYPE_COMPO_G)
			{
				bWrongTypeFound = TRUE;
				continue;
			}
		}
		else
		{
			if(m_aAllData[i].nStype == D_SECT_TYPE_COMPO_G)
			{
				bWrongTypeFound = TRUE;
				continue;
			}
		}
		
		str.Format(_T("%d:%s"), m_aAllID[i], m_aAllData[i].SName);
		ix = m_wndList.AddString(str);
		m_wndList.SetItemData(ix, m_aAllID[i]);
		if (m_aAllID[i] == m_nCurID) curix = ix;
	}
	
	m_wndList.SetCurSel(curix);
	
	if(bWrongTypeFound)
	{
		if(m_bCompositeMode)
			AfxMessageBox(_LS(IDS_CMD_CON_IMPORT_WRONG_TYPE1));
		else
		{
#if defined(_MGEN)
			AfxMessageBox(_LS(IDS_CMD_CON_IMPORT_WRONG_TYPE2_GEN));
#else
			AfxMessageBox(_LS(IDS_CMD_CON_IMPORT_WRONG_TYPE2));
#endif
		}
	}
}

BOOL CCMSectItemPSCImport::SaveData()
{
	int ix = m_wndList.GetCurSel();
	if (ix == -1)
	{
		//AfxMessageBox(_T("[오류] 단면이 선택되지 않았습니다."));
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SECTION));
		return FALSE;
	}
	m_nCurID = m_wndList.GetItemData(ix);
	return TRUE;
}


