// CMStageUDefStepDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageUDefStepDlg.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMStageUDefStepDlg dialog
#define CDialog CChildDialogMove 

CCMStageUDefStepDlg::CCMStageUDefStepDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMStageUDefStepDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStageUDefStepDlg)
	m_strDays = _T("");
	//}}AFX_DATA_INIT
	m_pSteps = NULL;
}


void CCMStageUDefStepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageUDefStepDlg)
	DDX_Control(pDX, IDC_CMD_STEP_LIST, m_wndStepList);
	DDX_Text(pDX, IDC_CMD_DAY_EDIT, m_strDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMStageUDefStepDlg, CDialog)
	//{{AFX_MSG_MAP(CCMStageUDefStepDlg)
	ON_BN_CLICKED(IDC_CMD_ADD_BUTTON, OnCmdAddButton)
	ON_BN_CLICKED(IDC_CMD_CLEAR_BUTTON, OnCmdClearButton)
	ON_BN_CLICKED(IDC_CMD_DEL_BUTTON, OnCmdDelButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageUDefStepDlg message handlers

BOOL CCMStageUDefStepDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetListHeader();
	UpdateCurStepDatas();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStageUDefStepDlg::SetSteps(CArray<double, double> * pSteps)
{
	m_pSteps = pSteps;
}

void CCMStageUDefStepDlg::SetListHeader()
{
	CStringArray HTitles;
	HTitles.Add(_LS(IDS_CMD0417__Step));
	HTitles.Add(_LS(IDS_CMD0417__Day) );

	CArray<float, float> HRatio;
	HRatio.Add(0.5f);HRatio.Add(0.5f);

	CDlgUtil::_SetListCtrlHeader(&m_wndStepList, HTitles, &HRatio, NULL);
}

void CCMStageUDefStepDlg::UpdateCurStepDatas()
{
	ASSERT(m_pSteps);
	this->m_wndStepList.DeleteAllItems();
	int nStep = m_pSteps->GetSize();
	CStringArray Contents;
	CString TStr;
	for( int i = 0 ; i< nStep ; i++)
	{
		Contents.RemoveAll();
		TStr.Format(_T("%d"),i+1);
		Contents.Add(TStr);
		TStr.Format(_T("%d"),(*m_pSteps)[i]);
		Contents.Add(TStr);
		CDlgUtil::SetListItem(&m_wndStepList,i,Contents,(*m_pSteps)[i]);
	}
}

void CCMStageUDefStepDlg::InsertStepData(int nStep)
{
	int nItem = m_wndStepList.GetItemCount();

	CStringArray Contents;
	CString TStr;

	if(!nItem)
	{
		TStr.Format(_T("1"));
		Contents.Add(TStr);
		TStr.Format(_T("%d"),nStep);
		Contents.Add(TStr);
		CDlgUtil::SetListItem(&m_wndStepList,0,Contents,nStep);
	}
	else
	{
		int nTemp;
		DWORD nTempD;
		CDlgUtil::GetListItem(&m_wndStepList,nItem-1,Contents,nTempD);
		nTemp = (int)nTempD;
		if(nTemp >= nStep)
		{
			// Error 
			return;
		}
		else
		{
			TStr.Format(_T("%d"),nItem+1);
			Contents.Add(TStr);
			TStr.Format(_T("%d"),nStep);
			Contents.Add(TStr);
			CDlgUtil::SetListItem(&m_wndStepList,nItem+1,Contents,nStep);
		}
	}
}

void CCMStageUDefStepDlg::DeleteCurSelItems()
{
	CMap<int, int, int, int> ItemMap;
	ItemMap.InitHashTable(50);
	int nItems = m_wndStepList.GetItemCount();
	if(!nItems) return;
	int nTemp;
	for(int i = 0; i < nItems; i++)
	{
		nTemp = m_wndStepList.GetItemData(i);
		ItemMap.SetAt(nTemp,nTemp);
	}

	CArray<int, int> SelItems;
	CDlgUtil::GetListItemByMask(&m_wndStepList,SelItems,LVIS_SELECTED);

	nItems = SelItems.GetSize();
	int nT;
	for(int i = 0; i < nItems ;i ++)
	{
		nTemp = m_wndStepList.GetItemData(SelItems[i]);
		ItemMap.RemoveKey(nTemp);
	}

	m_wndStepList.DeleteAllItems();

	POSITION Pos = ItemMap.GetStartPosition();
	int nI = 0 ;
	CStringArray Contents;
	CString TStr;
	SelItems.RemoveAll();
	while(Pos)
	{
		ItemMap.GetNextAssoc(Pos,nTemp,nT);
		SelItems.Add(nTemp);
	}
	qsort(SelItems.GetData(),SelItems.GetSize(),sizeof(int),CCompFunc::intAsc);

	nItems = SelItems.GetSize();
	for(int  i = 0; i < nItems; i++)
	{
		Contents.RemoveAll();
		TStr.Format(_T("%d"),i+1);
		Contents.Add(TStr);
		TStr.Format(_T("%d"),SelItems[i]);
		Contents.Add(TStr);
		CDlgUtil::SetListItem(&m_wndStepList,i,Contents,nTemp);
	}
}


void CCMStageUDefStepDlg::SaveData()
{
	ASSERT(m_pSteps);
	m_pSteps->RemoveAll();

	int nI = m_wndStepList.GetItemCount();
	int nTemp;
	for( int i = 0 ; i < nI ; i++)
	{
		nTemp = m_wndStepList.GetItemData(i);
		m_pSteps->Add(nTemp);
	}
}

void CCMStageUDefStepDlg::OnCmdAddButton() 
{
	UpdateData(TRUE);

	CArray<int, int> Days;
	CMap<int, int,int, int> DayMap;
	int   Day;
	if(CStrParser::GetNodeList(m_strDays,Days))
	{
		if(Days.GetSize())
		{
			int nItem = m_wndStepList.GetItemCount();
			for( int i = 0 ; i < nItem ; i++)
			{
				Day = (int)m_wndStepList.GetItemData(i);
				DayMap.SetAt(Day,Day);
			}

			nItem = Days.GetSize();
			for(int i = 0 ; i < nItem ; i++)
			{
				DayMap.SetAt(Days[i], Days[i]);
			}

			Days.RemoveAll();

			POSITION Pos = DayMap.GetStartPosition();
			int Key, Data;
			while(Pos)
			{
				DayMap.GetNextAssoc(Pos,Key, Data);
				Days.Add(Key);
			}

			qsort(Days.GetData(),Days.GetSize(),sizeof(int),CCompFunc::intAsc);

			m_wndStepList.DeleteAllItems();

			nItem = Days.GetSize();

			CStringArray Contents;
			CString      TStr;

			for(int i = 0 ; i < nItem ; i++)
			{
				Contents.RemoveAll();
				TStr.Format(_T("%d"), i+1);
				Contents.Add(TStr);
				TStr.Format(_T("%d"), Days[i]);
				Contents.Add(TStr);
				CDlgUtil::SetListItem(&m_wndStepList,i,Contents,Days[i]);
			}
		}
	}
	else
	{
		// Parameter Error!!!
	}

}

void CCMStageUDefStepDlg::OnCmdClearButton() 
{
	m_wndStepList.DeleteAllItems();
	m_pSteps->RemoveAll();
}

void CCMStageUDefStepDlg::OnCmdDelButton() 
{
	ASSERT(m_pSteps);
	DeleteCurSelItems();
	SaveData();
}

void CCMStageUDefStepDlg::OnOK() 
{
	ASSERT(m_pSteps);
	SaveData();
	CDialog::OnOK();
}
