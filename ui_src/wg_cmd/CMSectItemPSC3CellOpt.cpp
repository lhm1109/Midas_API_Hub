// CMSectItemPSC3CellOpt.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC3CellOpt.h"
#include "CMSectItemPSC3Cell.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC3CellOpt dialog

CCMSectItemPSC3CellOpt::CCMSectItemPSC3CellOpt(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMSectItemPSC3CellOpt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC3CellOpt)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMSectItemPSC3Cell *)pParent;

	for(int i=0; i<26; i++) m_Size[i] = 0.;
}

void CCMSectItemPSC3CellOpt::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC3CellOpt)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	CArray<UINT , UINT > UnitIDs;
	UnitIDs.Add(IDC_CMD_UNIT_STATIC1) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC2) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC3);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC4) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC5) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC6);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC7) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC8) ;UnitIDs.Add(IDC_CMD_UNIT_STATIC9);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC10);UnitIDs.Add(IDC_CMD_UNIT_STATIC11);UnitIDs.Add(IDC_CMD_UNIT_STATIC12);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC13);UnitIDs.Add(IDC_CMD_UNIT_STATIC14);UnitIDs.Add(IDC_CMD_UNIT_STATIC15);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC16);UnitIDs.Add(IDC_CMD_UNIT_STATIC17);UnitIDs.Add(IDC_CMD_UNIT_STATIC18);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC19);UnitIDs.Add(IDC_CMD_UNIT_STATIC20);UnitIDs.Add(IDC_CMD_UNIT_STATIC21);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC222);UnitIDs.Add(IDC_CMD_UNIT_STATIC23);UnitIDs.Add(IDC_CMD_UNIT_STATIC24);
	UnitIDs.Add(IDC_CMD_UNIT_STATIC25);UnitIDs.Add(IDC_CMD_UNIT_STATIC26);

	for( int i = 0 ; i < 26;  i++)
	{
		 DDX_Control(pDX,UnitIDs[i] ,m_Units[i]);  
	}

	CArray<UINT , UINT> EditIDs;
	EditIDs.Add(IDC_CMD_HO21_EDIT2);EditIDs.Add(IDC_CMD_HO22_EDIT2);
	EditIDs.Add(IDC_CMD_HI21_EDIT); EditIDs.Add(IDC_CMD_HI22_EDIT);
	EditIDs.Add(IDC_CMD_HI41_EDIT2);EditIDs.Add(IDC_CMD_HI42_EDIT2);
	EditIDs.Add(IDC_CMD_HI61_EDIT); EditIDs.Add(IDC_CMD_HI62_EDIT);
	EditIDs.Add(IDC_CMD_HI63_EDIT); EditIDs.Add(IDC_CMD_HI91_EDIT2);
	EditIDs.Add(IDC_CMD_HI92_EDIT2);EditIDs.Add(IDC_CMD_HI81_EDIT2);
	EditIDs.Add(IDC_CMD_HI82_EDIT2);
	EditIDs.Add(IDC_CMD_BO21_EDIT2);EditIDs.Add(IDC_CMD_BO22_EDIT);
	EditIDs.Add(IDC_CMD_BI11_EDIT2);EditIDs.Add(IDC_CMD_BI12_EDIT);
	EditIDs.Add(IDC_CMD_BI41_EDIT2);EditIDs.Add(IDC_CMD_BI42_EDIT2);
	EditIDs.Add(IDC_CMD_BI31_EDIT2);EditIDs.Add(IDC_CMD_BI32_EDIT2);
	EditIDs.Add(IDC_CMD_BI33_EDIT2);EditIDs.Add(IDC_CMD_BI71_EDIT2); 
	EditIDs.Add(IDC_CMD_BI72_EDIT2);EditIDs.Add(IDC_CMD_BI61_EDIT2);
	EditIDs.Add(IDC_CMD_BI62_EDIT2); 
	
	for(int i = 0 ; i < 26 ;i ++)
	{
		DDX_Control(pDX,EditIDs[i],m_Edits[i]);
	}
}

BEGIN_MESSAGE_MAP(CCMSectItemPSC3CellOpt, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectItemPSC3CellOpt)
	ON_EN_CHANGE(IDC_CMD_BI11_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI12_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI31_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI32_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI33_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI41_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI42_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI61_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI62_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI71_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BI72_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO21_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BO22_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI21_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI22_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI41_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI42_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI61_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI62_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI63_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI81_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI82_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI91_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HI92_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO21_EDIT2, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HO22_EDIT2, OnChangeCmdEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
//
// External Data Control Function
//
BOOL CCMSectItemPSC3CellOpt::GetDlgData(void* pData)
{
	T_SECT_D * pSect = (T_SECT_D*) pData;  
	for(int i = 22; i < 48 ; i++)  // GetEditValue 값과 m_Size[...]의 값이 일치하지 않으면 
	{                              // m_Size[...]의 값에 우선한다. 
		if(m_Edits[i-22].GetSafeHwnd())
			pSect->SectBefore.SectI.Size[i] =  m_Edits[i-22].GetEditValue();
		else
			pSect->SectBefore.SectI.Size[i] =  m_Size[i -22];
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Initialize
//
void CCMSectItemPSC3CellOpt::SetData2Dlg()
{
	ASSERT(m_pParent);
	T_SECT_D* pSectD = m_pParent->GetDataPointer();

	for(int i=22; i<48; i++)
		m_Edits[i-22].SetEditUnit(pSectD->SectBefore.SectI.Size[i]);
}

BOOL CCMSectItemPSC3CellOpt::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for( int i = 0 ;  i < 26 ; i++)
	{
		m_Edits[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_Edits[i].SetEditUnit(m_Size[i]);
	}
 
	SetData2Dlg();
	UpdateCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//
// Destroy
//
void CCMSectItemPSC3CellOpt::SetDlg2Data()
{
	ASSERT(m_pParent);
	T_SECT_D* pSectD = m_pParent->GetDataPointer();

	for(int i=22; i<48; i++)  
		pSectD->SectBefore.SectI.Size[i] =  m_Size[i-22] = m_Edits[i-22].GetEditValue();  
}

BOOL CCMSectItemPSC3CellOpt::DestroyWindow() 
{
	SetDlg2Data();

	return CDialog::DestroyWindow();
}


/////////////////////////////////////////////////////////////////////////////
//
// UpdateCtrls - Parent Dialog와의 연계
//
void  CCMSectItemPSC3CellOpt::UpdateCtrls()
{
	ASSERT(m_pParent);
	T_SECT_D* pSectD = m_pParent->GetDataPointer();

#define __FLAG_ pSectD->SectBefore.nJoint
	BOOL JI1 = (__FLAG_ & 0x0001) != 0;
	BOOL JI2 = (__FLAG_ & 0x0002) != 0;
	BOOL JI3 = (__FLAG_ & 0x0004) != 0;
	BOOL JI4 = (__FLAG_ & 0x0008) != 0;
	BOOL JI5 = (__FLAG_ & 0x0010) != 0;
	BOOL JI6 = (__FLAG_ & 0x0020) != 0;
	BOOL JI7 = (__FLAG_ & 0x0040) != 0;
	BOOL JI8 = (__FLAG_ & 0x0080) != 0;
	BOOL JI9 = (__FLAG_ & 0x0100) != 0;
	BOOL JI10= (__FLAG_ & 0x0200) != 0;
	BOOL JI11= (__FLAG_ & 0x0400) != 0;
	BOOL JO1 = (__FLAG_ & 0x0800) != 0;
	BOOL JO2 = (__FLAG_ & 0x1000) != 0;
#undef __FLAG_

	CWnd* pWnd;

	//JO1 check
	pWnd = GetDlgItem(IDC_CMD_HO21_EDIT2);
	pWnd -> EnableWindow(JO1);
	if (!JO1) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BO21_EDIT2);
	pWnd -> EnableWindow(JO1);
	if (!JO1) pWnd->SetWindowText(_T("0"));
	//JO2 check
	pWnd = GetDlgItem(IDC_CMD_HO22_EDIT2);
	pWnd -> EnableWindow(JO2);
	if (!JO2) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BO22_EDIT);
	pWnd -> EnableWindow(JO2);
	if (!JO2) pWnd->SetWindowText(_T("0"));

	pWnd = GetDlgItem(IDC_CMD_HI21_EDIT);
	pWnd -> EnableWindow(JI1);
	if (!JI1) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BI11_EDIT2);
	pWnd -> EnableWindow(JI1);
	if (!JI1) pWnd->SetWindowText(_T("0"));

	pWnd = GetDlgItem(IDC_CMD_HI22_EDIT);
	pWnd -> EnableWindow(JI2);
	if (!JI2) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BI12_EDIT);
	pWnd -> EnableWindow(JI2);
	if (!JI2) pWnd->SetWindowText(_T("0"));

	pWnd = GetDlgItem(IDC_CMD_HI41_EDIT2);
	pWnd -> EnableWindow(JI3);
	if (!JI3) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BI41_EDIT2);
	pWnd -> EnableWindow(JI3);
	if (!JI3) pWnd->SetWindowText(_T("0"));

	pWnd = GetDlgItem(IDC_CMD_HI42_EDIT2);
	pWnd -> EnableWindow(JI4);
	if (!JI4) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BI42_EDIT2);
	pWnd -> EnableWindow(JI4);
	if (!JI4) pWnd->SetWindowText(_T("0"));

	pWnd = GetDlgItem(IDC_CMD_HI61_EDIT);
	pWnd -> EnableWindow(JI5);
	if (!JI5) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BI31_EDIT2);
	pWnd -> EnableWindow(JI5);
	if (!JI5) pWnd->SetWindowText(_T("0"));

	pWnd = GetDlgItem(IDC_CMD_HI62_EDIT);
	pWnd -> EnableWindow(JI6);
	if (!JI6) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BI32_EDIT2);
	pWnd -> EnableWindow(JI6);
	if (!JI6) pWnd->SetWindowText(_T("0"));

	pWnd = GetDlgItem(IDC_CMD_HI63_EDIT);
	pWnd -> EnableWindow(JI7);
	if (!JI7) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BI33_EDIT2);
	pWnd -> EnableWindow(JI7);
	if (!JI7) pWnd->SetWindowText(_T("0"));

	pWnd = GetDlgItem(IDC_CMD_HI91_EDIT2);
	pWnd -> EnableWindow(JI8);
	if (!JI8) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BI71_EDIT2);
	pWnd -> EnableWindow(JI8);
	if (!JI8) pWnd->SetWindowText(_T("0"));

	pWnd = GetDlgItem(IDC_CMD_HI92_EDIT2);
	pWnd -> EnableWindow(JI9);
	if (!JI9) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BI72_EDIT2);
	pWnd -> EnableWindow(JI9);
	if (!JI9) pWnd->SetWindowText(_T("0"));

	pWnd = GetDlgItem(IDC_CMD_HI81_EDIT2);
	pWnd -> EnableWindow(JI10);
	if (!JI10) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BI61_EDIT2);
	pWnd -> EnableWindow(JI10);
	if (!JI10) pWnd->SetWindowText(_T("0"));

	pWnd = GetDlgItem(IDC_CMD_HI82_EDIT2);
	pWnd -> EnableWindow(JI11);
	if (!JI11) pWnd->SetWindowText(_T("0"));
	pWnd = GetDlgItem(IDC_CMD_BI62_EDIT2);
	pWnd -> EnableWindow(JI11);
	if (!JI11) pWnd->SetWindowText(_T("0"));

}

/////////////////////////////////////////////////////////////////////////////////////////
//
// View Control 관련
//
void CCMSectItemPSC3CellOpt::OnChangeCmdEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CChildDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetDlg2Data();

	m_pParent->RedrawSection();
}
