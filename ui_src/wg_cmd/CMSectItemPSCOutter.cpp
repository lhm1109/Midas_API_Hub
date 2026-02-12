// CMSectItemPSCOutter.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCOutter.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCOutter dialog
#define CDialog CChildDialog

CCMSectItemPSCOutter::CCMSectItemPSCOutter(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemPSCOutter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCOutter)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSectData = NULL;
}


void CCMSectItemPSCOutter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCOutter)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	CArray<UINT , UINT > UnitIDs;
	UnitIDs.Add(IDC_CMD_UNIT_STATIC1);UnitIDs.Add(IDC_CMD_UNIT_STATIC2);UnitIDs.Add(IDC_CMD_UNIT_STATIC3);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC4);UnitIDs.Add(IDC_CMD_UNIT_STATIC5);UnitIDs.Add(IDC_CMD_UNIT_STATIC6);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC11);UnitIDs.Add(IDC_CMD_UNIT_STATIC12);UnitIDs.Add(IDC_CMD_UNIT_STATIC13);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC14);UnitIDs.Add(IDC_CMD_UNIT_STATIC15);UnitIDs.Add(IDC_CMD_UNIT_STATIC16);

	for(int i = 0 ;i < 12 ;i ++)
	{
		DDX_Control(pDX,UnitIDs[i],m_Units[i]);
	}

	
	CArray<UINT , UINT> EditIDs;
	EditIDs.Add(IDC_CMD_HO1_EDIT);EditIDs.Add(IDC_CMD_HO2_EDIT);
	EditIDs.Add(IDC_CMD_HO21_EDIT);EditIDs.Add(IDC_CMD_HO22_EDIT);
	EditIDs.Add(IDC_CMD_HO3_EDIT);EditIDs.Add(IDC_CMD_HO31_EDIT);
	EditIDs.Add(IDC_CMD_BO1_EDIT);EditIDs.Add(IDC_CMD_BO11_EDIT);
	EditIDs.Add(IDC_CMD_BO12_EDIT);EditIDs.Add(IDC_CMD_BO2_EDIT);
	EditIDs.Add(IDC_CMD_BO21_EDIT);EditIDs.Add(IDC_CMD_BO3_EDIT);
	
	for(   i = 0 ; i < 12 ;i ++)
	{
		DDX_Control(pDX,EditIDs[i],m_Edits[i]);
	}
	
}


BEGIN_MESSAGE_MAP(CCMSectItemPSCOutter, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemPSCOutter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCOutter message handlers 
																		 //T_SECT_D
BOOL CCMSectItemPSCOutter::GetDlgData(void* pData)
{
	 T_SECT_D * pSect = (T_SECT_D*) pData;  
	 for(int i = 0; i < 12 ; i++) // GetEditValue()의 값과 m_Size[...]의 값이 
	 {                            // 다를때에는 m_Size[i]값에 우선한다. 
		 if(m_Edits[i].GetSafeHwnd())
			 pSect->SectBefore.SectI.Size[i] = m_Edits[i].GetEditValue();
		 else
			 pSect->SectBefore.SectI.Size[i] =  m_Size [i];
	 }
	 return TRUE;
}

void  CCMSectItemPSCOutter::SetCurDlgData(void* pData)
{
	this->m_pSectData = pData;
	T_SECT_D * pSect = (T_SECT_D*) pData;  
	 for(int i = 0; i < 12 ; i++)
		 m_Size[i] = pSect->SectBefore.SectI.Size[i];
}

BOOL CCMSectItemPSCOutter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateCtrls();
	UpdateCurDatas();
	for( int i = 0 ;  i < 12 ; i++)
	{
		m_Edits[i].SetUnitType(D_UNITSYS_BASE_LENGTH);    
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemPSCOutter::UpdateCtrls()
{
	ASSERT(m_pSectData);

	T_SECT_D *pSectD = (T_SECT_D*)m_pSectData;

	int JO1 = pSectD->SectBefore.SectI.BuiltUpFlag & 0x00000001;
	int JO2 = pSectD->SectBefore.SectI.BuiltUpFlag & 0x00000002;
	int JO3 = pSectD->SectBefore.SectI.BuiltUpFlag & 0x00000004;

	CWnd * pWnd;

	pWnd = GetDlgItem(IDC_CMD_HO21_EDIT);
	pWnd->EnableWindow(JO1);
	pWnd = GetDlgItem(IDC_CMD_BO11_EDIT);
	pWnd->EnableWindow(JO1);
	
	pWnd = GetDlgItem(IDC_CMD_HO22_EDIT);
	pWnd->EnableWindow(JO2);
	pWnd = GetDlgItem(IDC_CMD_BO12_EDIT);
	pWnd->EnableWindow(JO2);
	
	pWnd = GetDlgItem(IDC_CMD_HO31_EDIT);
	pWnd->EnableWindow(JO3);
	pWnd = GetDlgItem(IDC_CMD_BO21_EDIT);
	pWnd->EnableWindow(JO3);

	
}

void CCMSectItemPSCOutter::UpdateCurDatas()
{
	T_SECT_D * pSect = (T_SECT_D*) this->m_pSectData;
	CString TStr;
	for(int i = 0; i < 12 ; i++)
	{
		/*
		TStr.Format("%g", pSect->SectBefore.SectI.Size[i]);
		m_Edits[i].SetWindowText(TStr);
		*/
		m_Edits[i].SetEditUnit(pSect->SectBefore.SectI.Size[i]);
	}
}

BOOL CCMSectItemPSCOutter::DestroyWindow() 
{
	//GetDlgData(m_pSectData);	
	for( int i = 0 ; i< 12; i++)
	{
		((T_SECT_D*)m_pSectData)->SectBefore.SectI.Size[i]  = m_Size[i] = m_Edits[i].GetEditValue();
	}
	return CDialog::DestroyWindow();
}

