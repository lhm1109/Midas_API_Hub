// CMSectItemPSC3CellDef.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC3CellDef.h"
#include "CMSectItemPSC3Cell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC3CellDef dialog


CCMSectItemPSC3CellDef::CCMSectItemPSC3CellDef(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMSectItemPSC3CellDef::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC3CellDef)
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMSectItemPSC3Cell *)pParent;

	for(int i=0; i<22; i++) m_Size[i] = 0.;
}


void CCMSectItemPSC3CellDef::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC3CellDef)
	//}}AFX_DATA_MAP
	CArray<UINT , UINT > UnitIDs;
	UnitIDs.Add(IDC_CMD_UNIT_STATIC1) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC2) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC3);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC4) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC5) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC6);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC7) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC8) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC9);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC10);UnitIDs.Add(IDC_CMD_UNIT_STATIC11);UnitIDs.Add(IDC_CMD_UNIT_STATIC12);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC13);UnitIDs.Add(IDC_CMD_UNIT_STATIC14);UnitIDs.Add(IDC_CMD_UNIT_STATIC15);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC16);UnitIDs.Add(IDC_CMD_UNIT_STATIC17);UnitIDs.Add(IDC_CMD_UNIT_STATIC18);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC19);UnitIDs.Add(IDC_CMD_UNIT_STATIC20);UnitIDs.Add(IDC_CMD_UNIT_STATIC21);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC222);

	for( int i = 0 ; i < 22;  i++)
	{
		 DDX_Control(pDX,UnitIDs[i] ,m_Units[i]);  
	}
 

	CArray<UINT , UINT> EditIDs;
	EditIDs.Add(IDC_CMD_HI1_EDIT) ;EditIDs.Add(IDC_CMD_HI2_EDIT) ;
	EditIDs.Add(IDC_CMD_HI3_EDIT);EditIDs.Add(IDC_CMD_HI4_EDIT);
	EditIDs.Add(IDC_CMD_HI5_EDIT) ;EditIDs.Add(IDC_CMD_HI6_EDIT);
	EditIDs.Add(IDC_CMD_HI7_EDIT) ;EditIDs.Add(IDC_CMD_HI8_EDIT);
	EditIDs.Add(IDC_CMD_HI9_EDIT);EditIDs.Add(IDC_CMD_HI10_EDIT) ;
	EditIDs.Add(IDC_CMD_BI1_EDIT) ;EditIDs.Add(IDC_CMD_BI2_EDIT);
	EditIDs.Add(IDC_CMD_BI3_EDIT);EditIDs.Add(IDC_CMD_BI4_EDIT);
	EditIDs.Add(IDC_CMD_BI5_EDIT) ;EditIDs.Add(IDC_CMD_BI6_EDIT);
	EditIDs.Add(IDC_CMD_BI7_EDIT);EditIDs.Add(IDC_CMD_HO1_EDIT) ;
	EditIDs.Add(IDC_CMD_HO2_EDIT2);EditIDs.Add(IDC_CMD_BO1_EDIT2) ;
	EditIDs.Add(IDC_CMD_BO2_EDIT2);EditIDs.Add(IDC_CMD_BO3_EDIT) ;

	for(int i = 0 ; i < 22;i ++)
	{
		DDX_Control(pDX,EditIDs[i],m_Edits[i]);
	}

}

BEGIN_MESSAGE_MAP(CCMSectItemPSC3CellDef, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectItemPSC3CellDef)
	ON_EN_CHANGE(IDC_CMD_BI1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI3_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI4_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI5_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI6_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI7_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO1_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO2_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO3_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI10_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI3_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI4_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI5_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI6_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI7_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI8_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI9_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO2_EDIT2, OnChangeCmdEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
//
// External Data Control Function
//
BOOL CCMSectItemPSC3CellDef::GetDlgData(T_SECT_D* pData)
{
	 T_SECT_D * pSect = (T_SECT_D*) pData;  

	 // 이 다이얼로그가 현재탭이 아닐 경우, 
	 // m_Edits에서 값을 받아올 수 없기 때문에 
	 // destroy 될때 m_Size에 저장해놨다가 받아온다.
	 for(int i = 0; i < 22 ; i++) 
	 {
		 if(m_Edits[i].GetSafeHwnd())
			 pSect->SectBefore.SectI.Size[i] = m_Edits[i].GetEditValue();
		 else
			 pSect->SectBefore.SectI.Size[i] =  m_Size[i];
	 }
	 return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Initialize
//
void CCMSectItemPSC3CellDef::SetData2Dlg()
{
	ASSERT(m_pParent);
	T_SECT_D* pSectD = m_pParent->GetDataPointer();

	for(int i=0; i<22; i++)
		m_Edits[i].SetEditUnit(pSectD->SectBefore.SectI.Size[i]);
}


BOOL CCMSectItemPSC3CellDef::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for( int i = 0 ;  i < 22 ; i++)
	{
		m_Edits[i].SetUnitType(D_UNITSYS_BASE_LENGTH);  
		m_Units[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}

	SetData2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//
// Destroy
//
void CCMSectItemPSC3CellDef::SetDlg2Data()
{
	ASSERT(m_pParent);
	T_SECT_D* pSectD = m_pParent->GetDataPointer();

	for(int i=0; i<22; i++)
		pSectD->SectBefore.SectI.Size[i] = m_Size[i] = m_Edits[i].GetEditValue();
}

BOOL CCMSectItemPSC3CellDef::DestroyWindow() 
{
	SetDlg2Data();

	return CDialog::DestroyWindow();
}


/////////////////////////////////////////////////////////////////////////////////////////
//
// View Control 관련
//

void CCMSectItemPSC3CellDef::OnChangeCmdEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CChildDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetDlg2Data();

	m_pParent->RedrawSection();
}
