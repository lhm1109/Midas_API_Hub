// CMSectItemPSC_CMPRight.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC_CMPRight.h"

#include "CMSectItemPSC_CMP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_CMP_SIZE_NUM      32
#define D_SIZE_PRE_NUM      32

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_CMPRight dialog


CCMSectItemPSC_CMPRight::CCMSectItemPSC_CMPRight(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMSectItemPSC_CMPRight::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC_CMPRight)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	ASSERT(pParent != NULL);
	m_pParent = (CCMSectItemPSC_CMP*)pParent;

	memset(m_Size, 0, sizeof(m_Size));
}


void CCMSectItemPSC_CMPRight::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC_CMPRight)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	UINT aEditID[] = {
		IDC_CMD_HO1_EDIT,   IDC_CMD_HO2_EDIT,   IDC_CMD_HO3_EDIT,
		IDC_CMD_HO4_EDIT,   IDC_CMD_HO5_EDIT,   IDC_CMD_HO6_EDIT,   
		IDC_CMD_HO61_EDIT,  IDC_CMD_BO1_EDIT,   IDC_CMD_BO2_EDIT,   
		IDC_CMD_BO3_EDIT,   IDC_CMD_BO4_EDIT,   IDC_CMD_BO5_EDIT,   
		IDC_CMD_BO6_EDIT,   IDC_CMD_BO7_EDIT,   IDC_CMD_BO8_EDIT,   
		IDC_CMD_BO9_EDIT,   IDC_CMD_BO10_EDIT,  IDC_CMD_BO11_EDIT1,
		IDC_CMD_HI1_EDIT,   IDC_CMD_HI2_EDIT,   IDC_CMD_HI3_EDIT_,  
		IDC_CMD_HI4_EDIT_,  IDC_CMD_HI5_EDIT_,  IDC_CMD_HI6_EDIT,   
		IDC_CMD_HI7_EDIT,   IDC_CMD_HI8_EDIT,   IDC_CMD_BI1_EDIT_,  
		IDC_CMD_BI2_EDIT_,  IDC_CMD_BI3_EDIT_,  IDC_CMD_BI4_EDIT_,  
		IDC_CMD_BI5_EDIT,   IDC_CMD_BI6_EDIT_,
	};
	for (int i = 0; i < D_CMP_SIZE_NUM; i++)
	{
	  DDX_Control(pDX, aEditID[i], m_aCtrlEdit[i]);
	}

	UINT aUnitID[] = {
		IDC_CMD_HO1_UNIT,   IDC_CMD_HO2_UNIT,   IDC_CMD_HO3_UNIT,
		IDC_CMD_HO4_UNIT,   IDC_CMD_HO5_UNIT,   IDC_CMD_HO6_UNIT,   
		IDC_CMD_HO61_UNIT,  IDC_CMD_BO1_UNIT,   IDC_CMD_BO2_UNIT,   
		IDC_CMD_BO3_UNIT,   IDC_CMD_BO4_UNIT,   IDC_CMD_BO5_UNIT,   
		IDC_CMD_BO6_UNIT,   IDC_CMD_BO7_UNIT,   IDC_CMD_BO8_UNIT,   
		IDC_CMD_BO9_UNIT,   IDC_CMD_BO10_UNIT,  IDC_CMD_BO11_UNIT1,  
		IDC_CMD_HI1_UNIT,   IDC_CMD_HI2_UNIT,   IDC_CMD_HI3_UNIT_,  
		IDC_CMD_HI4_UNIT_,  IDC_CMD_HI5_UNIT_,  IDC_CMD_HI6_UNIT,   
		IDC_CMD_HI7_UNIT,   IDC_CMD_HI8_UNIT,   IDC_CMD_BI1_UNIT_,  
		IDC_CMD_BI2_UNIT_,  IDC_CMD_BI3_UNIT_,  IDC_CMD_BI4_UNIT_,  
		IDC_CMD_BI5_UNIT_,  IDC_CMD_BI6_UNIT,
	};
	for (int i = 0; i < D_CMP_SIZE_NUM; i++)
	{
	  DDX_Control(pDX, aUnitID[i], m_aCtrlUnit[i]);
	}
}


BEGIN_MESSAGE_MAP(CCMSectItemPSC_CMPRight, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectItemPSC_CMPRight)
	ON_EN_CHANGE(IDC_CMD_HO1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO3_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO4_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO5_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO6_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO61_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO3_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO4_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO5_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO6_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO7_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO11_EDIT1, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO8_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO9_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO10_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI3_EDIT_, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI4_EDIT_, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI5_EDIT_, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI6_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI7_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI8_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI1_EDIT_, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI2_EDIT_, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI3_EDIT_, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI4_EDIT_, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI5_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI6_EDIT_, OnChangeCmdEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// External Data Control Function
BOOL CCMSectItemPSC_CMPRight::GetDlgData(T_SECT_D* pData)
{
	 // 좌우 모두 저장할 경우 size 갯수가 60개가 넘게 되므로
	 // 여기서는 BO1, BO2, BO10만 저장한다. (index : 32,33,34)
	 T_SECT_D * pSect = (T_SECT_D*) pData;  

	 // 이 다이얼로그가 현재탭이 아닐 경우, 
	 // m_Edits에서 값을 받아올 수 없기 때문에 
	 // destroy 될때 m_Size에 저장해놨다가 받아온다.
	 if (m_aCtrlEdit[7].GetSafeHwnd())
	 {
		 pSect->SectBefore.SectI.Size[32] = m_aCtrlEdit[7].GetEditValue();
		 pSect->SectBefore.SectI.Size[33] = m_aCtrlEdit[8].GetEditValue();
		 pSect->SectBefore.SectI.Size[34] = m_aCtrlEdit[17].GetEditValue();
	 }
	 else
	 {
		 pSect->SectBefore.SectI.Size[32] = m_Size[7];
		 pSect->SectBefore.SectI.Size[33] = m_Size[8];
		 pSect->SectBefore.SectI.Size[34] = m_Size[17];
	 }
	 /*
	 for(int i = 0; i < D_CMP_SIZE_NUM ; i++) 
	 {
		 if (m_aCtrlEdit[i].GetSafeHwnd()) // 현재 탭일 경우
			pSect->SectBefore.SectI.Size[i] = m_aCtrlEdit[i].GetEditValue();
		 else   // 현재 텝이 아닐 경우
			 pSect->SectBefore.SectI.Size[i] = m_Size[i];
	 }
	 */
	 return TRUE;
}

// update enable/disable
// case 0. symmetric(Left X)  : BO10, BO1, BO2
// case 1. hunch              : HO4, HO61, BO4, BO5, BO9, BO10, HI4, HI5 
void CCMSectItemPSC_CMPRight::ChangeStatus(BOOL bSymmetric, BOOL bHunch)
{
	for (int i = 0; i < D_CMP_SIZE_NUM; i++)
	{
		if(i==7 || i==8 || i==17)
			m_aCtrlEdit[i].EnableWindow(!bSymmetric);
		else
			m_aCtrlEdit[i].EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_CMPRight message handlers

BOOL CCMSectItemPSC_CMPRight::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitControls(); 
	SetData2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMSectItemPSC_CMPRight::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	SetDlg2Data();
	
	return CChildDialog::DestroyWindow();
}

void CCMSectItemPSC_CMPRight::OnChangeCmdEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CChildDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetDlg2Data();
	m_pParent->RedrawSection();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CCMSectItemPSC_CMPRight::InitControls()
{
	for ( int i = 0 ;  i < D_CMP_SIZE_NUM ; i++)
	{
		m_aCtrlEdit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);  
		m_aCtrlUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
}

void CCMSectItemPSC_CMPRight::SetData2Dlg()
{
	ASSERT(m_pParent);
	T_SECT_D* pSectD = m_pParent->GetDataPointer();


	for (int i = 0; i < D_CMP_SIZE_NUM; i++)
	{
		m_aCtrlEdit[i].SetEditUnit(pSectD->SectBefore.SectI.Size[i]);
	}
	if (!pSectD->SectBefore.bSymmetric)
	{
		m_aCtrlEdit[7].SetEditUnit(pSectD->SectBefore.SectI.Size[32]);
		m_aCtrlEdit[8].SetEditUnit(pSectD->SectBefore.SectI.Size[33]);
		m_aCtrlEdit[17].SetEditUnit(pSectD->SectBefore.SectI.Size[34]);
	}

	ChangeStatus(pSectD->SectBefore.bSymmetric, pSectD->SectBefore.bSmallHole);
}

void CCMSectItemPSC_CMPRight::SetDlg2Data()
{
	ASSERT(m_pParent);
	T_SECT_D* pSectD = m_pParent->GetDataPointer();

	pSectD->SectBefore.SectI.Size[32] = m_Size[7] = m_aCtrlEdit[7].GetEditValue();
	pSectD->SectBefore.SectI.Size[33] = m_Size[8] = m_aCtrlEdit[8].GetEditValue();
	pSectD->SectBefore.SectI.Size[34] = m_Size[17] = m_aCtrlEdit[17].GetEditValue();

	if (pSectD->SectBefore.bSymmetric)
	{

		pSectD->SectBefore.SectI.Size[7]  = pSectD->SectBefore.SectI.Size[32];
		pSectD->SectBefore.SectI.Size[8]  = pSectD->SectBefore.SectI.Size[33];
		pSectD->SectBefore.SectI.Size[17] = pSectD->SectBefore.SectI.Size[34];
	}
}
