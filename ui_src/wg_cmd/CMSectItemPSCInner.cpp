// CMSectItemPSCInner.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCInner.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCInner dialog
#define CDialog CChildDialog


CCMSectItemPSCInner::CCMSectItemPSCInner(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemPSCInner::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCInner)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSectData = NULL;
}


void CCMSectItemPSCInner::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCInner)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	CArray<UINT , UINT > UnitIDs;
	UnitIDs.Add(IDC_CMD_UNIT_STATIC1) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC2) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC3);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC4) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC5) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC6);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC7) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC8) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC9);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC10);UnitIDs.Add(IDC_CMD_UNIT_STATIC11);UnitIDs.Add(IDC_CMD_UNIT_STATIC12);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC13);UnitIDs.Add(IDC_CMD_UNIT_STATIC14);UnitIDs.Add(IDC_CMD_UNIT_STATIC15);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC16);UnitIDs.Add(IDC_CMD_UNIT_STATIC17);UnitIDs.Add(IDC_CMD_UNIT_STATIC18);

	for( int i = 0 ; i < 18;  i++)
	{
		 DDX_Control(pDX,UnitIDs[i] ,m_Units[i]);  
	}

	CArray<UINT , UINT> EditIDs;
	EditIDs.Add(IDC_CMD_HI1_EDIT) ;EditIDs.Add(IDC_CMD_HI2_EDIT) ;
	EditIDs.Add(IDC_CMD_HI21_EDIT);EditIDs.Add(IDC_CMD_HI22_EDIT);
	EditIDs.Add(IDC_CMD_HI3_EDIT) ;EditIDs.Add(IDC_CMD_HI31_EDIT);
	EditIDs.Add(IDC_CMD_HI4_EDIT) ;EditIDs.Add(IDC_CMD_HI41_EDIT);
	EditIDs.Add(IDC_CMD_HI42_EDIT);EditIDs.Add(IDC_CMD_HI5_EDIT) ;
	EditIDs.Add(IDC_CMD_BI1_EDIT) ;EditIDs.Add(IDC_CMD_BI11_EDIT);
	EditIDs.Add(IDC_CMD_BI12_EDIT);EditIDs.Add(IDC_CMD_BI21_EDIT);
	EditIDs.Add(IDC_CMD_BI3_EDIT) ;EditIDs.Add(IDC_CMD_BI31_EDIT);
	EditIDs.Add(IDC_CMD_BI32_EDIT);EditIDs.Add(IDC_CMD_BI4_EDIT) ;

	for(   i = 0 ; i < 18 ;i ++)
	{
		DDX_Control(pDX,EditIDs[i],m_Edits[i]);
	}
}


BEGIN_MESSAGE_MAP(CCMSectItemPSCInner, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemPSCInner)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCInner message handlers
																//T_SECT_D  
BOOL CCMSectItemPSCInner::GetDlgData(void* pData)
{
	T_SECT_D * pSect = (T_SECT_D*) pData;  
	for(int i = 12; i < 30 ; i++)  // GetEditValue 값과 m_Size[...]의 값이 일치하지 않으면 
	{                              // m_Size[...]의 값에 우선한다. 
		if(m_Edits[i-12].GetSafeHwnd())
			pSect->SectBefore.SectI.Size[i] =  m_Edits[i-12].GetEditValue();
		else
			pSect->SectBefore.SectI.Size[i] =  m_Size[i -12];
	}
	return TRUE;
}

void  CCMSectItemPSCInner::SetCurDlgData(void* pData)
{
	m_pSectData = pData;
	
	T_SECT_D * pSect = (T_SECT_D*) pData;  
	for(int i = 12; i < 30 ; i++)
		m_Size[i-12] =  pSect->SectBefore.SectI.Size[i];
}

BOOL CCMSectItemPSCInner::OnInitDialog() 
{
	CDialog::OnInitDialog();
	  
	for( int i = 0 ;  i < 18 ; i++)
	{
		m_Edits[i].SetUnitType(D_UNITSYS_BASE_LENGTH);    
	}
	
	UpdateCtrls();
	UpdateCurDatas();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemPSCInner::UpdateCtrls()
{
	ASSERT(m_pSectData);

	T_SECT_D *pSectD = (T_SECT_D*)m_pSectData;

	int JI1 = pSectD->SectBefore.SectI.BuiltUpFlag & 0x00000008;
	int JI2 = pSectD->SectBefore.SectI.BuiltUpFlag & 0x00000010;
	int JI3 = pSectD->SectBefore.SectI.BuiltUpFlag & 0x00000020;
	int JI4 = pSectD->SectBefore.SectI.BuiltUpFlag & 0x00000040;
	int JI5 = pSectD->SectBefore.SectI.BuiltUpFlag & 0x00000080;

	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_CMD_BI11_EDIT);
	pWnd->EnableWindow(JI1);
	pWnd = GetDlgItem(IDC_CMD_HI21_EDIT);
	pWnd->EnableWindow(JI1);


	pWnd = GetDlgItem(IDC_CMD_BI12_EDIT);
	pWnd->EnableWindow(JI2);
	pWnd = GetDlgItem(IDC_CMD_HI22_EDIT);
	pWnd->EnableWindow(JI2);


	pWnd = GetDlgItem(IDC_CMD_BI21_EDIT);
	pWnd->EnableWindow(JI3);
	pWnd = GetDlgItem(IDC_CMD_HI31_EDIT);
	pWnd->EnableWindow(JI3);


	pWnd = GetDlgItem(IDC_CMD_BI32_EDIT);
	pWnd->EnableWindow(JI4);
	pWnd = GetDlgItem(IDC_CMD_HI42_EDIT);
	pWnd->EnableWindow(JI4);


	pWnd = GetDlgItem(IDC_CMD_BI31_EDIT);
	pWnd->EnableWindow(JI5);
	pWnd = GetDlgItem(IDC_CMD_HI41_EDIT);
	pWnd->EnableWindow(JI5);

	//IDC_CMD_BI4_EDIT

	pWnd = GetDlgItem(IDC_CMD_BI4_EDIT);

	if(pSectD->SectBefore.Shape  == D_SECT_SHAPE_PSC_1CELL)
	{
		pWnd->EnableWindow(FALSE);
	} 
	else
	{
		pWnd->EnableWindow(TRUE);
	}
	
}

void CCMSectItemPSCInner::UpdateCurDatas()
{
	T_SECT_D * pSect = (T_SECT_D*) m_pSectData;  
	CString TStr;
	for(int i = 12; i < 30 ; i++)
	{
		/*
		TStr.Format("%g",pSect->SectBefore.SectI.Size[i]);
		 
		m_Edits[i-12].SetWindowText(TStr);
		*/
		//m_Edits[i-12].SetEditUnit(pSect->SectBefore.SectI.Size[i]);
		m_Edits[i-12].SetEditUnit(m_Size[i-12]);
	}
}

BOOL CCMSectItemPSCInner::DestroyWindow() 
{
	//GetDlgData(m_pSectData);
	for( int i = 12 ; i< 30; i++)
	{
		((T_SECT_D*)m_pSectData)->SectBefore.SectI.Size[i] =  m_Size[i -12] = m_Edits[i-12].GetEditValue();
	}
	return CDialog::DestroyWindow();
}
