// WindClassifyKBC2015Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindClassifyKBC2015Dlg.h"

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindClassifyKBC2015Dlg dialog
CWindClassifyKBC2015Dlg::CWindClassifyKBC2015Dlg(BOOL bShowBuildingType, CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindClassifyKBC2015Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindClassifyKBC2015Dlg)
	m_nStructure = 0;
	m_nPlane = 0;
	m_dHeight = 0.0;
	m_dWidth = 0.0;
	m_dBreadth = 0.0;
	m_dArea = 0.0;
	m_nWindCodeType = KBC_W_2016;
	m_bShowBuildingType = bShowBuildingType;
	//}}AFX_DATA_INIT

	m_aCtrlStructure.RemoveAll();
	m_aCtrlStructure.Add(IDC_ETC_WIND_STRUCT_RDO1);
	m_aCtrlStructure.Add(IDC_ETC_WIND_STRUCT_RDO2);
	
	m_aCtrlPlane.RemoveAll();
	m_aCtrlPlane.Add(IDC_ETC_WIND_PLANE_RDO1);
	m_aCtrlPlane.Add(IDC_ETC_WIND_PLANE_RDO2);
}


void CWindClassifyKBC2015Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindClassifyKBC2015Dlg)
	DDX_Control(pDX, IDC_ETC_WIND_EDIT1, m_edtHeight);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT2, m_edtBreadthX);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT3, m_edtBreadthY);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT4, m_edtArea);

	DDX_Control(pDX, IDC_ETC_WIND_UNIT1, m_wndHeightUnit);
	DDX_Control(pDX, IDC_ETC_WIND_UNIT2, m_wndBreadthXUnit);
	DDX_Control(pDX, IDC_ETC_WIND_UNIT3, m_wndBreadthYUnit);
	DDX_Control(pDX, IDC_ETC_WIND_UNIT4, m_wndAreaUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWindClassifyKBC2015Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindClassifyKBC2015Dlg)
	ON_BN_CLICKED(IDC_ETC_WIND_STRUCT_RDO1, OnCmdStructRadio)
	ON_BN_CLICKED(IDC_ETC_WIND_STRUCT_RDO2, OnCmdStructRadio)
	ON_BN_CLICKED(IDC_ETC_WIND_PLANE_RDO1, OnCmdShapeRadio)
	ON_BN_CLICKED(IDC_ETC_WIND_PLANE_RDO2, OnCmdShapeRadio)
	ON_BN_CLICKED(IDC_ETC_WIND_CLASSIFY_BTN, OnCmdClassify)
	ON_BN_CLICKED(IDC_ETC_WIND_CLOSE, OnCmdCancel)
	ON_EN_CHANGE(IDC_ETC_WIND_EDIT2, OnChangeBreadthEdit)
	ON_EN_CHANGE(IDC_ETC_WIND_EDIT3, OnChangeBreadthEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindClassifyKBC2015Dlg message handlers

BOOL CWindClassifyKBC2015Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	

	// TODO: Add extra initialization here
	AlignCtrl();
	UpdateData(FALSE);
		
	((CButton *)GetDlgItem(IDC_ETC_WIND_EDIT5))->SetWindowText(_T(""));
	((CButton *)GetDlgItem(IDC_ETC_WIND_EDIT6))->SetWindowText(_T(""));

	InitialData();
	OnCmdShapeRadio();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindClassifyKBC2015Dlg::AlignCtrl()
{
	if(m_bShowBuildingType) return;

	CArray<UINT, UINT> aCtrlBuildingType;
	aCtrlBuildingType.Add(IDC_ETC_WIND_EDIT6);
	aCtrlBuildingType.Add(IDC_ETC_WIND_TITLE_STC8);

	CRect RectDlg, RectGrp, RectType;
	GetWindowRect(&RectDlg);

	GetDlgItem(IDC_ETC_WIND_RESULT_GRB)->GetWindowRect(&RectGrp);
	GetDlgItem(IDC_ETC_WIND_EDIT6)->GetClientRect(&RectType);

	// 1. 필요없는 것은 Hide 시킨다.
	CDlgUtil::CtrlShowHide(this, aCtrlBuildingType, m_bShowBuildingType);

	// 2. Size 조절한다.
	ScreenToClient(RectGrp); 
	ScreenToClient(RectType); 

	int nHeight = RectType.Height();
	GetDlgItem(IDC_ETC_WIND_RESULT_GRB)->SetWindowPos(NULL, RectGrp.left, RectGrp.top, RectGrp.Width(), RectGrp.Height()-nHeight, 0);
	
	CArray<UINT, UINT> aMoveCtrl;
	aMoveCtrl.Add(IDC_ETC_WIND_CLOSE);
	CDlgUtil::CtrlMoveDistY(this, aMoveCtrl, -nHeight);

	SetWindowPos(NULL, RectDlg.left, RectDlg.top, RectDlg.Width(), RectDlg.Height()-nHeight, SWP_NOMOVE | SWP_NOZORDER);

}
void CWindClassifyKBC2015Dlg::InitialData()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlStructure, m_nStructure);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPlane, m_nPlane);

	m_edtHeight.SetEditUnit(m_dHeight);
	m_edtBreadthX.SetEditUnit(m_dWidth);
	m_edtBreadthY.SetEditUnit(m_dBreadth);
	m_edtArea.SetEditUnit(m_dArea);

	m_wndHeightUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndBreadthXUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndBreadthYUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndAreaUnit.SetUnitType(D_UNITSYS_BASE_AREA);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlPlane, m_nStructure==0);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_EDIT4, m_nStructure==1);

	UpdateData(FALSE);
}

void CWindClassifyKBC2015Dlg::OnCmdStructRadio()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStructure, m_nStructure);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlPlane, m_nStructure==0);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_EDIT4, m_nStructure==1);

	if(m_nStructure==0)
	{
		CalcArea();
	}
}

void CWindClassifyKBC2015Dlg::OnCmdShapeRadio()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPlane, m_nPlane);

	if(m_nPlane==0)
	{
		GetDlgItem(IDC_ETC_WIND_TITLE_STC4)->SetWindowText(_LS(IDS_CMD_WIND_XBreadth_Rect));
		GetDlgItem(IDC_ETC_WIND_TITLE_STC5)->SetWindowText(_LS(IDS_CMD_WIND_YBreadth_Rect));
	}
	else
	{
		GetDlgItem(IDC_ETC_WIND_TITLE_STC4)->SetWindowText(_LS(IDS_CMD_WIND_XBreadth_Circle));
		GetDlgItem(IDC_ETC_WIND_TITLE_STC5)->SetWindowText(_LS(IDS_CMD_WIND_YBreadth_Circle));
	}

	CalcArea();

	GetDlgItem(IDC_ETC_WIND_EDIT3)->EnableWindow(m_nPlane==0);
}

void CWindClassifyKBC2015Dlg::OnCmdClassify() 
{
	double dHeight  = m_edtHeight.GetEditValue();
	double dWidth   = m_edtBreadthX.GetEditValue();
	double dBreadth = m_edtBreadthY.GetEditValue();	
	double dArea    = m_edtArea.GetEditValue();

	// 단위를 m로 바꾼다.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CUnitCtrl* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;

	m_dHeight   = pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dHeight);
	m_dWidth    = pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dWidth);
	m_dBreadth  = pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dBreadth);
	m_dArea     = pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_AREA, dArea);
	
	if(m_dHeight <= 0.0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Hei)); return;
	}
	if(m_dWidth  <= 0.0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_X_B)); return;
	}
	if(m_dBreadth <= 0.0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Y_B)); return;
	}
	if(m_dArea <= 0.0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Area)); return;
	}
	
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStructure, m_nStructure);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPlane, m_nPlane);

	double dValue1 = m_dHeight/sqrt(m_dWidth*m_dBreadth);
	double dValue2 = m_dHeight/sqrt(m_dArea);

	BOOL bSimplifiedCond1 = m_dHeight <= 20.0;                                      // 간편법이 되기 위한 조건 1
	BOOL bSimplifiedCond2 = (dValue1<=1.0 || dValue2<=1.0);                         // 간편법이 되기 위한 조건 2
	BOOL bSimplifiedCond3 = (m_dWidth/m_dHeight>1.0 && m_dBreadth/m_dHeight>1.0);   // 간편법이 되기 위한 조건 3 (MQC 15959로 인해서 추가됨: JWKWON-2017-03-17)
	BOOL bSimplifiedCond4 = (m_dHeight*0.5 <= m_dBreadth && m_dBreadth <= 30.0) && (m_dHeight*0.5 <= m_dWidth && m_dWidth <= 30.0);
	BOOL bSimplified = FALSE;
	if(m_nStructure==0 && bSimplifiedCond1 && bSimplifiedCond2) // 정형, <= 20m
	{
		if (m_nWindCodeType == KDS_W_2022)
		{
			if(bSimplifiedCond4) bSimplified = TRUE;
		}
		else
		{
			if (bSimplifiedCond3) bSimplified = TRUE;
		}
	}

	CString strMethod=_T("");
	CString strType=_T("");
	if(bSimplified)
	{
		strMethod = _LS(IDS_CMD_WIND_METHOD_SIMPLE);
	}
	else
	{
		strMethod = _LS(IDS_CMD_WIND_METHOD_GEN);

		strType = _LS(IDS_CMD_WIND_BUILDING_MIDDLE);
		if(m_nPlane==1 || (m_nPlane==0 && (dValue1>=3.0 || dValue2>=3.0))) // Circle or Rectangular
		{
			strType = _LS(IDS_CMD_WIND_BUILDING_HIGH);
		}
	}
		
	((CButton *)GetDlgItem(IDC_ETC_WIND_EDIT5))->SetWindowText(strMethod);
	((CButton *)GetDlgItem(IDC_ETC_WIND_EDIT6))->SetWindowText(strType);
}

void CWindClassifyKBC2015Dlg::OnCmdCancel() 
{
	CDialogMove::OnCancel();		
}

void CWindClassifyKBC2015Dlg::OnChangeBreadthEdit() 
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStructure, m_nStructure);
	if(m_nStructure==0)
	{
		CalcArea();
	}
}

void CWindClassifyKBC2015Dlg::CalcArea()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPlane, m_nPlane);

	if(m_nPlane==0)
	{
		double dWidth			= m_edtBreadthX.GetEditValue();
		double dBreadth		= m_edtBreadthY.GetEditValue();
		m_edtArea.SetEditUnit(dWidth*dBreadth);
	}
	else
	{
		double dD			= m_edtBreadthX.GetEditValue();
		m_edtBreadthY.SetEditUnit(dD);
		m_edtArea.SetEditUnit(M_PI*(dD/2)*(dD/2));
	}
}