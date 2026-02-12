// WindGustKBC2009Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"

//#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

//#include "..\wg_base\wg_base_StrParser.h"
// #include "..\wg_base\wg_base_MsgDll.h"  
// 
#include "..\wg_base\DlgUtil.h"

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
// #include "..\wg_db\EditData.h"

#include "StldWindPressureNodalDlg_StructureType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureArea_KBC2009_GustF dialog
CStldWindPressureNodalDlg_StructureType::CStldWindPressureNodalDlg_StructureType(CWnd* pParent /*=NULL*/)
	: CDialogMove(CStldWindPressureNodalDlg_StructureType::IDD, pParent)
{
	m_pWdpr = NULL;
	m_pImageList = NULL;
}

CStldWindPressureNodalDlg_StructureType::~CStldWindPressureNodalDlg_StructureType()
{
	if(m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
	
}

void CStldWindPressureNodalDlg_StructureType::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCTTYPE,   m_cbxType);
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_B1,    m_edtB1);
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_B2,    m_edtB2);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_B3,    m_edtB3);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_B4,    m_edtB4);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_R1,    m_edtR1);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_R2,    m_edtR2);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_H,     m_edtH);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_ANGLE, m_edtAngle);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_B1_UNIT,    m_untB1);
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_B2_UNIT,    m_untB2);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_B3_UNIT,    m_untB3);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_B4_UNIT,    m_untB4);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_R1_UNIT,    m_untR1);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_R2_UNIT,    m_untR2);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_H_UNIT,     m_untH);	
	DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_ANGLE_UNIT, m_untAngle);
	//DDX_Control(pDX, IDC_WINDP_NODAL_STRUCT_SPIN,       m_angleSpin);
}

BEGIN_MESSAGE_MAP(CStldWindPressureNodalDlg_StructureType, CDialogMove)
	ON_CBN_SELCHANGE(IDC_WINDP_NODAL_STRUCTTYPE,         OnSelChangeType)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_WINDP_NODAL_STRUCT_SPIN, OnDeltaposCmdAngle)     //Span Num
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureArea_KBC2009_GustF message handlers

BOOL CStldWindPressureNodalDlg_StructureType::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitControl();

	Data2Dlg();
	OnSelChangeType();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldWindPressureNodalDlg_StructureType::OnOK() 
{
	if(Dlg2Data())
		CDialogMove::OnOK();
}

void CStldWindPressureNodalDlg_StructureType::OnCancel() 
{
	CDialogMove::OnCancel();		
}

void CStldWindPressureNodalDlg_StructureType::InitTypeCombo()
{
	m_pImageList = new CImageList;
	m_pImageList->Create(65,50,ILC_COLOR,4,4);
	// Load Bitmap at ImageList.
	CBitmap* pBitmap = new CBitmap;
	pBitmap->LoadBitmap(IDB_WINDP_NODAL_STRUCTURE);
	m_pImageList->Add(pBitmap, RGB(0,0,0));
	delete pBitmap;

	// Set ImageList at ComboBoxEx.
	m_cbxType.SetImageList(m_pImageList);
	// Insert Item.
	COMBOBOXEXITEM cbi[4];
	CString str[4] = {_T("")};
	int nItem;
	for(int i=0; i<4; i++)
	{
		cbi[i].mask				    = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
		cbi[i].iItem			    = 0;
		cbi[i].pszText		    = (LPTSTR)(LPCTSTR)str[i];
		cbi[i].cchTextMax	    = str[i].GetLength();
		cbi[i].iImage			    = i;			// Default Image Numeber.
		cbi[i].iSelectedImage	= i;	// Selecting Image Numeber.
		cbi[i].iOverlay		    = 1;
		cbi[i].iIndent		    = 0;
	}
	// Add Items at Comboboxex.
	// Sorting order is Descending.
	for(int i=4; i>0; i--)	nItem = m_cbxType.InsertItem(&cbi[i-1]);
	// Initialze Comboboxex.
	m_cbxType.SetCurSel(0);
}

void CStldWindPressureNodalDlg_StructureType::InitControl()
{
	InitTypeCombo();
	
	m_edtB1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtB2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtB3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtB4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtR1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtR2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtH .SetUnitType(D_UNITSYS_BASE_LENGTH);
	//m_edtAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);

	m_untB1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untB2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untB3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untB4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untR1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untR2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untH .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
}

void CStldWindPressureNodalDlg_StructureType::Data2Dlg()
{
	m_cbxType.SetCurSel(m_Data.nType - 1);
	m_edtB1.SetEditUnit(m_Data.dB1);
	m_edtB2.SetEditUnit(m_Data.dB2);
	m_edtB3.SetEditUnit(m_Data.dB3);
	m_edtB4.SetEditUnit(m_Data.dB4);
	m_edtR1.SetEditUnit(m_Data.dR1);
	m_edtR2.SetEditUnit(m_Data.dR2);
	m_edtH.SetEditUnit(m_Data.dH);
	m_edtAngle.SetValue(m_Data.dAngleOfB1);
}

BOOL CStldWindPressureNodalDlg_StructureType::Dlg2Data()
{
	m_Data.nType = m_cbxType.GetCurSel() + 1;
	m_Data.dB1 = m_edtB1.GetEditValue();
	m_Data.dB2 = m_edtB2.GetEditValue();
	m_Data.dB3 = m_edtB3.GetEditValue();
	m_Data.dB4 = m_edtB4.GetEditValue();
	m_Data.dR1 = m_edtR1.GetEditValue();
	m_Data.dR2 = m_edtR2.GetEditValue();
	m_Data.dH  = m_edtH.GetEditValue();
	m_Data.dAngleOfB1 = m_edtAngle.GetEditValue();

	return bCheckValue();
}

void CStldWindPressureNodalDlg_StructureType::OnSelChangeType()
{
	int nType = m_cbxType.GetCurSel() + 1;
	if(nType == 1) // Trapezoidal
	{
		m_edtAngle.EnableWindow(TRUE);
		//m_angleSpin.EnableWindow(TRUE);
		m_edtB1.EnableWindow(TRUE);
		m_edtB2.EnableWindow(TRUE);
		m_edtB3.EnableWindow(TRUE);
		m_edtB4.EnableWindow(TRUE);
		m_edtH.EnableWindow(TRUE);
		m_edtR1.EnableWindow(FALSE);
		m_edtR2.EnableWindow(FALSE);
	}
	else if(nType == 2) //Cylindrical
	{
		m_edtH.EnableWindow(TRUE);
		m_edtR1.EnableWindow(TRUE);
		m_edtR2.EnableWindow(TRUE);
		m_edtAngle.EnableWindow(FALSE);
		//m_angleSpin.EnableWindow(FALSE);
		m_edtB1.EnableWindow(FALSE);
		m_edtB2.EnableWindow(FALSE);
		m_edtB3.EnableWindow(FALSE);
		m_edtB4.EnableWindow(FALSE);
	}
	else if(nType == 3) //Spherical
	{
		m_edtR1.EnableWindow(TRUE);
		m_edtAngle.EnableWindow(FALSE);
		//m_angleSpin.EnableWindow(FALSE);
		m_edtB1.EnableWindow(FALSE);
		m_edtB2.EnableWindow(FALSE);
		m_edtB3.EnableWindow(FALSE);
		m_edtB4.EnableWindow(FALSE);
		m_edtH.EnableWindow(FALSE);
		m_edtR2.EnableWindow(FALSE);    
	}
	else if(nType == 4) //Semi-Spherical
	{
		m_edtR1.EnableWindow(TRUE);
		m_edtAngle.EnableWindow(FALSE);
		//m_angleSpin.EnableWindow(FALSE);
		m_edtB1.EnableWindow(FALSE);
		m_edtB2.EnableWindow(FALSE);
		m_edtB3.EnableWindow(FALSE);
		m_edtB4.EnableWindow(FALSE);
		m_edtH.EnableWindow(FALSE);
		m_edtR2.EnableWindow(FALSE);   
	}
	else
		ASSERT(0);
}

void CStldWindPressureNodalDlg_StructureType::GetNodalStructure( T_WDPR_D* pWdpr )
{
	pWdpr->nodalStruct = m_Data;
}


void CStldWindPressureNodalDlg_StructureType::SetWdpr( T_WDPR_D* pWdpr )
{
	m_pWdpr = pWdpr;
	m_Data = m_pWdpr->nodalStruct;
	if(m_Data.nType == 0)
	{
		m_Data.nType = 1;
	}
}

void CStldWindPressureNodalDlg_StructureType::OnDeltaposCmdAngle( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_WINDP_NODAL_STRUCT_ANGLE, -90, 90, pNMHDR, pResult);  
	*pResult = 0;    
}

BOOL CStldWindPressureNodalDlg_StructureType::bCheckValue()
{
	BOOL bB1 = FALSE;
	BOOL bB2 = FALSE;
	BOOL bB3 = FALSE;
	BOOL bB4 = FALSE;
	BOOL bR1 = FALSE;
	BOOL bR2 = FALSE;
	BOOL bH  = FALSE;
	
	if(m_Data.nType == 1) //¿∞∏È√º
	{ 
		bB1 = TRUE;
		bB2 = TRUE;
		bB3 = TRUE;
		bB4 = TRUE;
		bH  = TRUE;
	}
	else if(m_Data.nType == 2) //±‚µ’
	{
		bR1 = TRUE;
		bR2 = TRUE;
		bH = TRUE;
	}
	else if(m_Data.nType == 3) //±∏
	{
		bR1 = TRUE;
	}
	else if(m_Data.nType == 4) //π›±∏
	{
		bR1 = TRUE;
	}
	
	CString strTemp;
	strTemp.Empty();
	if(bB1 && m_Data.dB1 <= 1E-5)  { strTemp.Format(_LS(IDS_TM_WINDP_NODAL_INVALID_SIZE), _T("B1")); }
	if(bB2 && m_Data.dB2 <= 1E-5)  { strTemp.Format(_LS(IDS_TM_WINDP_NODAL_INVALID_SIZE), _T("B2")); }
	if(bB3 && m_Data.dB3 <= 1E-5)  { strTemp.Format(_LS(IDS_TM_WINDP_NODAL_INVALID_SIZE), _T("B3")); }
	if(bB4 && m_Data.dB4 <= 1E-5)  { strTemp.Format(_LS(IDS_TM_WINDP_NODAL_INVALID_SIZE), _T("B4")); }
	if(bR1 && m_Data.dR1 <= 1E-5)  { strTemp.Format(_LS(IDS_TM_WINDP_NODAL_INVALID_SIZE), _T("R1")); }
	if(bR2 && m_Data.dR2 <= 1E-5)  { strTemp.Format(_LS(IDS_TM_WINDP_NODAL_INVALID_SIZE), _T("R2")); }
	if(bH  && m_Data.dH  <= 1E-5)  { strTemp.Format(_LS(IDS_TM_WINDP_NODAL_INVALID_SIZE), _T("H"));  }
	
	if(strTemp.IsEmpty())
		return TRUE;  

	AfxMessageBox(strTemp);
	return FALSE;
}