// CMSectItemPSCnCellRight.cpp : implementation file
//
// 2004. 02. 22    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCnCellRight.h"

#include "CMSectItemPSCnCell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_SIZE_PRE_NUM    1+29

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCellRight dialog


CCMSectItemPSCnCellRight::CCMSectItemPSCnCellRight(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMSectItemPSCnCellRight::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCnCellRight)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = NULL;
	ASSERT(pParent != NULL);
	m_pParent = (CCMSectItemPSCnCell *)pParent;

	for(int i=0; i<29; i++) m_Size[i] = 0.0;
}


void CCMSectItemPSCnCellRight::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCnCellRight)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	UINT aEditID[29] = {
		IDC_CMD_HO1_EDIT, IDC_CMD_HO2_EDIT, IDC_CMD_HO21_EDIT,
		IDC_CMD_HO3_EDIT, IDC_CMD_HO4_EDIT, IDC_CMD_BO1_EDIT,
		IDC_CMD_BO11_EDIT, IDC_CMD_BO2_EDIT, IDC_CMD_BO3_EDIT,
		IDC_CMD_BO4_EDIT, IDC_CMD_BO5_EDIT,     // outer
		IDC_CMD_HI1_EDIT, IDC_CMD_HI2_EDIT, IDC_CMD_HI21_EDIT,
		IDC_CMD_HI3_EDIT, IDC_CMD_HI4_EDIT1, IDC_CMD_HI5_EDIT1, 
		IDC_CMD_HI6_EDIT, IDC_CMD_BI1_EDIT, IDC_CMD_BI2_EDIT, 
		IDC_CMD_BI21_EDIT1, IDC_CMD_BI3_EDIT1, IDC_CMD_BI4_EDIT1, 
		IDC_CMD_BI5_EDIT, IDC_CMD_BI6_EDIT,     // inner
		IDC_CMD_BI7_EDIT1, IDC_CMD_BI8_EDIT1,   // mid
		IDC_CMD_R1_EDIT, IDC_CMD_R2_EDIT        
	};
	for (int i=0; i<29; i++)
	{
	  DDX_Control(pDX, aEditID[i], m_aCtrlEdit[i]);
	}

	UINT aUnitID[29] = {
		IDC_CMD_HO1_UNIT, IDC_CMD_HO2_UNIT, IDC_CMD_HO21_UNIT,
		IDC_CMD_HO3_UNIT, IDC_CMD_HO4_UNIT, IDC_CMD_BO1_UNIT,
		IDC_CMD_BO11_UNIT, IDC_CMD_BO2_UNIT, IDC_CMD_BO3_UNIT,
		IDC_CMD_BO4_UNIT, IDC_CMD_BO5_UNIT,     // outer
		IDC_CMD_HI1_UNIT1, IDC_CMD_HI2_UNIT, IDC_CMD_HI21_UNIT,
		IDC_CMD_HI3_UNIT, IDC_CMD_HI4_UNIT, IDC_CMD_HI5_UNIT,
		IDC_CMD_HI6_UNIT, IDC_CMD_BI1_UNIT, IDC_CMD_BI2_UNIT, 
		IDC_CMD_BI21_UNIT, IDC_CMD_BI3_UNIT, IDC_CMD_BI4_UNIT, 
		IDC_CMD_BI5_UNIT, IDC_CMD_BI6_UNIT,     // inner
		IDC_CMD_BI7_UNIT, IDC_CMD_BI8_UNIT,     // mid
		IDC_CMD_R1_UNIT, IDC_CMD_R2_UNIT
	};
	for(int i=0; i<29; i++)
	{
	  DDX_Control(pDX, aUnitID[i], m_aCtrlUnit[i]);
	}
}


BEGIN_MESSAGE_MAP(CCMSectItemPSCnCellRight, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectItemPSCnCellRight)
	ON_EN_CHANGE(IDC_CMD_BI1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI21_EDIT1, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI3_EDIT1, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI4_EDIT1, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI5_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI6_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO11_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO3_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO4_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO5_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI21_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI3_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI4_EDIT1, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI5_EDIT1, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO21_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO3_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO4_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_R1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_R2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI7_EDIT1, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI8_EDIT1, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI6_EDIT, OnChangeCmdEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// External Data Control Function
BOOL CCMSectItemPSCnCellRight::GetDlgData(T_SECT_D* pData)
{
	 T_SECT_D * pSect = (T_SECT_D*) pData;  

	 // 이 다이얼로그가 현재탭이 아닐 경우, 
	 // m_Edits에서 값을 받아올 수 없기 때문에 
	 // destroy 될때 m_Size에 저장해놨다가 받아온다.
	 for(int i = 0; i < 29 ; i++) 
	 {
		 if(m_aCtrlEdit[i].GetSafeHwnd()) // 현재 탭일 경우
			 pSect->SectBefore.SectI.Size[D_SIZE_PRE_NUM+i] = m_aCtrlEdit[i].GetEditValue();
		 else   // 현재 텝이 아닐 경우
			 pSect->SectBefore.SectI.Size[D_SIZE_PRE_NUM+i] = m_Size[i];
	 }
	 return TRUE;
}

// update enable/disable
// case 0. symmetric  
// case 1. cell type    
// case 2. cell number
// case 3. joint
// case 4. side hole 
void CCMSectItemPSCnCellRight::ChangeStatus(BOOL bSym, int nCellType, int CellNum, int nJoint, BOOL bSideHole)
{
	// if it's current tab
	HWND hWnd = GetSafeHwnd();
	if (hWnd == NULL || !(::IsWindow(hWnd))) return;

	ASSERT(m_pParent);
	
	T_SECT_D* pSectD = m_pParent->GetDataPointer();
	for (int i=0; i<29; i++)
	{
		m_aCtrlEdit[i].SetEditUnit(pSectD->SectBefore.SectI.Size[((i==25||i==26)?1:D_SIZE_PRE_NUM)+i]);
		m_aCtrlEdit[i].EnableWindow(!bSym);
	}  
	if (bSym) return;

	BOOL bJO = (nJoint & 0x0001) ? TRUE : FALSE;
	BOOL bJI = (nJoint & 0x0002) ? TRUE : FALSE;

	GetDlgItem(IDC_CMD_HO21_EDIT)->EnableWindow(bJO);
	GetDlgItem(IDC_CMD_BO11_EDIT)->EnableWindow(bJO);

	if (nCellType == 0)   // polygon
	{
		GetDlgItem(IDC_CMD_HI21_EDIT)->EnableWindow(bJI);
		GetDlgItem(IDC_CMD_BI21_EDIT1)->EnableWindow(bJI);

		GetDlgItem(IDC_CMD_HI3_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HI4_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HI5_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_HI6_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_BI1_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_BI2_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_BI3_EDIT1)->EnableWindow(CellNum > 1);
		GetDlgItem(IDC_CMD_BI4_EDIT1)->EnableWindow(CellNum > 1);
		GetDlgItem(IDC_CMD_BI5_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_BI6_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_BI7_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_BI8_EDIT1)->EnableWindow(FALSE);

		GetDlgItem(IDC_CMD_HO4_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HI1_EDIT)->EnableWindow(FALSE);
 }
	else if (nCellType == 1)    // circle
	{
		GetDlgItem(IDC_CMD_HI21_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_BI21_EDIT1)->EnableWindow(FALSE);

		GetDlgItem(IDC_CMD_HI3_EDIT)->EnableWindow(bSideHole);
		GetDlgItem(IDC_CMD_HI4_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HI5_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HI6_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_BI1_EDIT)->EnableWindow(bSideHole);
		GetDlgItem(IDC_CMD_BI2_EDIT)->EnableWindow(bSideHole);
		GetDlgItem(IDC_CMD_BI3_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_BI4_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_BI5_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_BI6_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_BI7_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_BI8_EDIT1)->EnableWindow(FALSE);

		GetDlgItem(IDC_CMD_HI1_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HI2_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HO4_EDIT)->EnableWindow(FALSE);
	}
	else ASSERT(0);
}

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCellRight message handlers

BOOL CCMSectItemPSCnCellRight::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for( int i = 0 ;  i < 29 ; i++)
	{
		m_aCtrlEdit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);  
		m_aCtrlUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}

	SetData2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMSectItemPSCnCellRight::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	SetDlg2Data();
	
	return CChildDialog::DestroyWindow();
}

void CCMSectItemPSCnCellRight::OnChangeCmdEdit() 
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

void CCMSectItemPSCnCellRight::SetData2Dlg()
{
	ASSERT(m_pParent);
	T_SECT_D* pSectD = m_pParent->GetDataPointer();

	for(int i=0; i<29; i++)
		m_aCtrlEdit[i].SetEditUnit(pSectD->SectBefore.SectI.Size[((i==25||i==26)?1:D_SIZE_PRE_NUM)+i]);

	ChangeStatus(pSectD->SectBefore.bSymmetric, pSectD->SectBefore.nCellType, 
		pSectD->SectBefore.nCellShape, pSectD->SectBefore.nJoint, pSectD->SectBefore.bSmallHole);
}

void CCMSectItemPSCnCellRight::SetDlg2Data()
{
	ASSERT(m_pParent);
	T_SECT_D* pSectD = m_pParent->GetDataPointer();

	for(int i=0; i<29; i++)
	{
		if(i==25||i==26)
			continue;
		pSectD->SectBefore.SectI.Size[D_SIZE_PRE_NUM+i] = m_Size[i] = m_aCtrlEdit[i].GetEditValue();
		//if (pSectD->SectBefore.bSymmetric)
			//pSectD->SectBefore.SectI.Size[D_SIZE_PRE_NUM-29+i] = pSectD->SectBefore.SectI.Size[D_SIZE_PRE_NUM+i];
	}
}
