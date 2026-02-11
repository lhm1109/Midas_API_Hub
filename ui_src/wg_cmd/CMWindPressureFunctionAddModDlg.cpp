// WindProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindPressureFunctionAddModDlg.h"
#include "CMWindPressureFunctionHelper.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_base\DynamicReportTemplateDlg.h"
#include "..\wg_base\PL_Parser.h"
#include "..\wg_base\SpecialChar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum ColumnIndex
{
	COLINDEX_NO = 0,
	COLINDEX_X = 1,
	COLINDEX_Y = 2,
	COLINDEX_Z = 3,
	COLINDEX_PRESSURE = 4,
};

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureFunctionAddModDlg dialog

CCMWindPressureFunctionAddModDlg::CCMWindPressureFunctionAddModDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMWindPressureFunctionAddModDlg::IDD, pParent)
{
	m_Key = 0;
	m_pHelpDlg = NULL;
	m_aMethodRdo.RemoveAll();
	m_nMethod = WDFC_METHOD_FUNC;
}

CCMWindPressureFunctionAddModDlg::~CCMWindPressureFunctionAddModDlg()
{
	DestoryHelperDlg();  
}


void CCMWindPressureFunctionAddModDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CMD_WINDP_COORD_SYS, m_cbxCoord);
	DDX_Control(pDX, IDC_WINDP_FIX_AXIS, m_cbxFixAxis);
	DDX_Control(pDX, IDC_WINDP_TABLE, m_GridWnd);
}


BEGIN_MESSAGE_MAP(CCMWindPressureFunctionAddModDlg, CDialogMove)
	ON_BN_CLICKED(IDC_WINDP_METHOD1_RDO, OnCmdMethodChanged)
	ON_BN_CLICKED(IDC_WINDP_METHOD2_RDO, OnCmdMethodChanged)
	ON_BN_CLICKED(IDC_WINDP_CALCULATE,        OnCmdCalculate)
	ON_CBN_SELCHANGE(IDC_CMD_WINDP_COORD_SYS, OnCmdCoordChanged)
	ON_CBN_SELCHANGE(IDC_WINDP_FIX_AXIS,      OnCmdFixAxisChanged)
	ON_BN_CLICKED(IDC_WINDP_BTN_FUNCHELPER,   OnCmdFuncHelper)  
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureFunctionAddModDlg message handlers

BOOL CCMWindPressureFunctionAddModDlg::OnInitDialog() 
{  
	CDialogMove::OnInitDialog();  

	m_aMethodRdo.RemoveAll();
	m_aMethodRdo.Add(IDC_WINDP_METHOD1_RDO);
	m_aMethodRdo.Add(IDC_WINDP_METHOD2_RDO);

	InitMinMax();
	InitComboCoord();
	InitComboFixAxis();
	
	Data2Dlg();

	//그리드 초기화 및 업데이트
	InitGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindPressureFunctionAddModDlg::OnOK()
{
	if(Dlg2Data(TRUE))
	{
	  if(!CheckValid())
		  return;

		BOOL bRetVal;

		if(m_Key == 0)
			bRetVal = CDBDoc::GetDocPoint()->m_pDataCtrl->AddWdfc(m_Data);
		else
			bRetVal = CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyWdfc(m_Key, m_Key, m_Data);

		if(bRetVal)
			CDialogMove::OnOK();
	}
}

void CCMWindPressureFunctionAddModDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCMWindPressureFunctionAddModDlg::OnCmdMethodChanged()
{
	int nMethod = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aMethodRdo, nMethod);
	if (nMethod == m_nMethod) return;
	m_nMethod = nMethod;
	UpdateGrid();
}

void CCMWindPressureFunctionAddModDlg::OnCmdCalculate()
{
	Dlg2Data(FALSE, TRUE);

	Calculate(TRUE);

	UpdateGrid();
}

BOOL CCMWindPressureFunctionAddModDlg::Grid2Data()
{
	int nSize = m_GridWnd.GetRowCount();

	CString strTemp;
	m_Data.caPresure.RemoveAll();
	for (int i = 0; i < nSize; i++)
	{
		strTemp = m_GridWnd.GetValueRowCol(i + 1, COLINDEX_PRESSURE);
		double dPres = _ttof(strTemp);
		m_Data.caPresure.Add(dPres);
	}

	return TRUE;
}

BOOL CCMWindPressureFunctionAddModDlg::Dlg2Data(BOOL bWarning/* = FALSE*/, BOOL bCalc/* = FALSE*/)
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aMethodRdo, m_nMethod);
	m_Data.nMethod = m_nMethod;

	GetDlgItem(IDC_WINDP_FUNCNAME)->GetWindowText(m_Data.strName);
	GetDlgItem(IDC_WINDP_FUNCEQUATION)->GetWindowText(m_Data.strEquation);
	GetDlgItem(IDC_WINDP_DESCRIPTION)->GetWindowText(m_Data.strDescription);

	m_Data.strName.TrimLeft();
	m_Data.strName.TrimRight();
	if(m_Data.strName.IsEmpty() && bWarning)
	{
		AfxMessageBox(_LS(IDS_CMD_WINDP_VELOCITY_NONAME));
		return FALSE;
	}

	BOOL bNameExist = FALSE;
	T_WDFC_K findKey = CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetWdfcKeyByName(m_Data.strName);
	if(findKey != m_Key && findKey > 0)
	{
		bNameExist = TRUE;    
	}

	if(bNameExist && bWarning)
	{
		AfxMessageBox(_LS(IDS_CMD_WINDP_VELOCITY_SAMENAME));
		return FALSE; 
	}

	int nSel = m_cbxCoord.GetCurSel();
	if(nSel == 0)      m_Data.nCoordType = 1;
	else if(nSel == 1) m_Data.nCoordType = 2;

	CString strTemp;
	GetDlgItem(IDC_WINDP_START_COOR1)->GetWindowText(strTemp);
	m_d1From = _tstof(strTemp);
	GetDlgItem(IDC_WINDP_END_COOR1)->GetWindowText(strTemp);
	m_d1To = _tstof(strTemp);
	GetDlgItem(IDC_WINDP_INC_COOR1)->GetWindowText(strTemp);
	m_d1Inc = _tstof(strTemp);
	GetDlgItem(IDC_WINDP_FIXCOOR1)->GetWindowText(strTemp);
	m_d2Fix = _tstof(strTemp);
	GetDlgItem(IDC_WINDP_FIXCOOR2)->GetWindowText(strTemp);
	m_d3Fix = _tstof(strTemp);

	m_Data.nFixIdx = m_cbxFixAxis.GetCurSel();
	m_Data.dVal1Start = m_d1From;
	m_Data.dVal1End   = m_d1To;
	m_Data.dVal1Inc   = m_d1Inc;
	m_Data.dVal2      = m_d2Fix;
	m_Data.dVal3      = m_d3Fix;

	m_Data.caPresure.RemoveAll();
	if(m_nMethod == WDFC_METHOD_USER)
	{
		CArray<double, double> caLevel;

		if (!bCalc)
		{
			if (m_caPresure.GetSize() != CWindLoadGeneratorCtrl::GetPresLevel(m_Data.dVal1Start, m_Data.dVal1End, m_Data.dVal1Inc, caLevel))
			{
				if (AfxMessageBox(_LS(IDS_CMS_WINDP_FUNCTION_DEF_PRES_COUNT_NOT_MATCH), MB_YESNO) == IDYES)
				{
					OnCmdCalculate();
				}
				else
				{
					return FALSE;
				}
			}
			Grid2Data();
		}
	}
	

	return TRUE;
}

BOOL CCMWindPressureFunctionAddModDlg::Data2Dlg()
{
	m_nMethod = m_Data.nMethod;
	CDlgUtil::CtrlRadioSetCheck(this, m_aMethodRdo, m_nMethod);

	GetDlgItem(IDC_WINDP_FUNCNAME)->SetWindowText(m_Data.strName);
	GetDlgItem(IDC_WINDP_FUNCEQUATION)->SetWindowText(m_Data.strEquation);
	GetDlgItem(IDC_WINDP_DESCRIPTION)->SetWindowText(m_Data.strDescription);
	if(m_Data.nCoordType == 1)      m_cbxCoord.SetCurSel(0);
	else if(m_Data.nCoordType == 2) m_cbxCoord.SetCurSel(1);

	OnCmdCoordChanged();

	if(m_Key > 0) //저장한 값을 부른다.
	{
	  if(0 <= m_Data.nFixIdx && m_Data.nFixIdx <= 2)
		  m_cbxFixAxis.SetCurSel(m_Data.nFixIdx);

	  m_d1From = m_Data.dVal1Start;
	  m_d1To   = m_Data.dVal1End;
	  m_d1Inc  = m_Data.dVal1Inc;
	  m_d2Fix  = m_Data.dVal2;
	  m_d3Fix  = m_Data.dVal3;
	  m_caPresure.RemoveAll();
	  if (m_nMethod == WDFC_METHOD_USER)
		m_caPresure.Copy(m_Data.caPresure);

	  CString strTemp;

	  strTemp.Format(_T("%g"), m_Data.dVal1Start);
	  GetDlgItem(IDC_WINDP_START_COOR1)->SetWindowText(strTemp);
	  strTemp.Format(_T("%g"), m_Data.dVal1End);
	  GetDlgItem(IDC_WINDP_END_COOR1)->SetWindowText(strTemp);
	  strTemp.Format(_T("%g"), m_Data.dVal1Inc);
	  GetDlgItem(IDC_WINDP_INC_COOR1)->SetWindowText(strTemp);
	  strTemp.Format(_T("%g"), m_Data.dVal2);
	  GetDlgItem(IDC_WINDP_FIXCOOR1)->SetWindowText(strTemp);
	  strTemp.Format(_T("%g"), m_Data.dVal3);
	  GetDlgItem(IDC_WINDP_FIXCOOR2)->SetWindowText(strTemp);

	  Calculate(FALSE, TRUE);
	}

	return TRUE;
}

void CCMWindPressureFunctionAddModDlg::InitComboCoord()
{
	m_cbxCoord.ResetContent();
	m_cbxCoord.AddString(_LS(IDS_CMD_WINDP_FUNCTION_DEF_COORD_REC));
	m_cbxCoord.AddString(_LS(IDS_CMD_WINDP_FUNCTION_DEF_COORD_CYL));
	m_cbxCoord.SetCurSel(0);
}

void CCMWindPressureFunctionAddModDlg::InitComboFixAxis()
{
	int nSel = m_cbxCoord.GetCurSel();

	m_cbxFixAxis.ResetContent();
	if(nSel == 0) //Rec
	{
		m_cbxFixAxis.AddString(_T("X, Y"));
		m_cbxFixAxis.AddString(_T("Y, Z"));
		m_cbxFixAxis.AddString(_T("X, Z"));
	}
	else
	{
		m_cbxFixAxis.AddString(_T("R, TH"));
		m_cbxFixAxis.AddString(_T("TH, Z"));
		m_cbxFixAxis.AddString(_T("R, Z"));
	}
	m_cbxFixAxis.SetCurSel(0);

	//if(m_Key == 0)
	SetStaticCtrlNameValue();
}

void CCMWindPressureFunctionAddModDlg::OnCmdCoordChanged()
{
	InitComboFixAxis();

	if(m_pHelpDlg)
	{
		m_pHelpDlg->ResetXYZ();
	}
}

BOOL CCMWindPressureFunctionAddModDlg::GetStringByCoordFixedAxis( CString& csD1, CString& csD2, CString& csD3, BOOL bWithUnit, CString* pPres )
{
	int nSelCoord   = m_cbxCoord.GetCurSel();
	int nSelFixAxis = m_cbxFixAxis.GetCurSel();

	if(nSelCoord == 0) // Rec...
	{
		csD1.Format(_T("X"));
		csD2.Format(_T("Y"));
		csD3.Format(_T("Z"));
	}
	else if(nSelCoord == 1) //Cyl
	{
		csD1.Format(_T("R"));
		csD2.Format(_T("TH"));
		csD3.Format(_T("Z"));
	}
	else return FALSE;

	if(bWithUnit)
	{
		if(nSelCoord == 0) // Rec...
		{
			csD1 += _T("\n(m)");
			csD2 += _T("\n(m)");
			csD3 += _T("\n(m)");
		}
		else if(nSelCoord == 1) //Cyl
		{
			csD1 += _T("\n(m)");
			csD2 += _T("\n(") + _LS(IDS_MIT_UNIT_DEGREE) + _T(")");
			csD3 += _T("\n(m)");
		}
	}
	if(pPres)
	{
		pPres->Format(_LS(IDS_CMD_WINDP_WINDPRESSURE));
		if(bWithUnit)
		{
			*pPres += _T("\n(kN/m");
			*pPres += D_CH_2SQUARE;
			*pPres += _T(")");
		}
	}

	return TRUE;  
}

BOOL CCMWindPressureFunctionAddModDlg::GetStringByCoordFixedAxis( CString& csD1, CString& csD2, CString& csD3 )
{
	int nSelCoord   = m_cbxCoord.GetCurSel();
	int nSelFixAxis = m_cbxFixAxis.GetCurSel();

	if(nSelCoord == 0) // Rec...
	{
		if(nSelFixAxis == 0) // X, Y
		{
			csD1.Format(_T("Z"));
			csD2.Format(_T("X"));
			csD3.Format(_T("Y"));
		}
		else if(nSelFixAxis == 1) // Y, Z
		{
			csD1.Format(_T("X"));
			csD2.Format(_T("Y"));
			csD3.Format(_T("Z"));
		}
		else if(nSelFixAxis == 2) // X, Z
		{
			csD1.Format(_T("Y"));
			csD2.Format(_T("X"));
			csD3.Format(_T("Z"));
		}
		else return FALSE;
	}
	else if(nSelCoord == 1) //Cyl
	{
		if(nSelFixAxis == 0) // R, TH
		{
			csD1.Format(_T("Z"));
			csD2.Format(_T("R"));
			csD3.Format(_T("TH"));
		}
		else if(nSelFixAxis == 1) // TH, Z
		{
			csD1.Format(_T("R"));
			csD2.Format(_T("TH"));
			csD3.Format(_T("Z"));
		}
		else if(nSelFixAxis == 2) // R, Z
		{
			csD1.Format(_T("TH"));
			csD2.Format(_T("R"));
			csD3.Format(_T("Z"));
		}
		else return FALSE;
	}
	else return FALSE;

	return TRUE;
}

void CCMWindPressureFunctionAddModDlg::SetStaticCtrlNameValue()
{
	CString strD1, strD2, strD3;
	if(!GetStringByCoordFixedAxis(strD1, strD2, strD3))
		ASSERT(0);

	GetDlgItem(IDC_WINDP_FIX_AXIS1)->SetWindowText(strD1);
	GetDlgItem(IDC_WINDP_FIXCOOR1_STC)->SetWindowText(strD2);
	GetDlgItem(IDC_WINDP_FIXCOOR2_STC)->SetWindowText(strD3);

	CString strTemp;
	double dFrom, dTo, dInc;
	if(GetValueByCoordFixedAxis(dFrom, dTo, dInc))
	{
		strTemp.Format(_T("%.12g"),dFrom);
		GetDlgItem(IDC_WINDP_START_COOR1)->SetWindowText(strTemp);
		strTemp.Format(_T("%.12g"),dTo);
		GetDlgItem(IDC_WINDP_END_COOR1)->SetWindowText(strTemp);
		strTemp.Format(_T("%.12g"),dInc);
		GetDlgItem(IDC_WINDP_INC_COOR1)->SetWindowText(strTemp);
	}
}

void CCMWindPressureFunctionAddModDlg::OnCmdFixAxisChanged()
{
	SetStaticCtrlNameValue();
}


void CCMWindPressureFunctionAddModDlg::SetWdfcKey( T_WDFC_K Key )
{
	m_Key = Key;
	if(m_Key > 0)
	{
		CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetWdfc(Key, m_Data);
	}
}

void CCMWindPressureFunctionAddModDlg::InitGrid()
{
	m_GridWnd.Initialize();
	
	// Sample setup for the grid
	m_GridWnd.GetParam()->EnableUndo(FALSE);  
	m_GridWnd.SetRowCount(m_caD1.GetSize());
	m_GridWnd.SetColCount(4);

	// cell 바깥영역 배경색 지정 
	m_GridWnd.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header 배경색 지정 
	m_GridWnd.ChangeColHeaderStyle(CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	m_GridWnd.ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// 기타 Cells 배경색 지정
	m_GridWnd.ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.ReadOnlyModeBg));  
	
	m_GridWnd.SetStyleRange(CGXRange().SetCols(0,4), CGXStyle()
		.SetWrapText(TRUE) 
		.SetEnabled(FALSE));

	m_GridWnd.SetFrozenCols(0,0);

	// double click시 셀 편집이 되도록 한다.
	m_GridWnd.GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	CRect rectGrid;
	m_GridWnd.GetWindowRect(rectGrid);
	rectGrid.Width();
	int nIndexWidth = 30;
	int nWndWidth = rectGrid.Width() - nIndexWidth - GetSystemMetrics(SM_CXVSCROLL) - 5; //윈도우 너비에서 index와 scrollbar 사이즈 뺀다.
	int nXYZWidth = nWndWidth * 0.22;
	int nPressureWidth = nWndWidth - (nXYZWidth * 3);

	m_GridWnd.SetColWidth(0, 0, nIndexWidth); // Name
	m_GridWnd.SetColWidth(1, 3, nXYZWidth);
	m_GridWnd.SetColWidth(4, 4, nPressureWidth);
	m_GridWnd.SetRowHeightByDPI(0, 0, 35, NULL);
	
	m_GridWnd.SetScrollBarMode(SB_VERT,gxnEnabled);
	m_GridWnd.SetScrollBarMode(SB_HORZ,gxnAutomatic);
	
	CString strD1, strD2, strD3, strPres;
	if(GetStringByCoordFixedAxis(strD1, strD2, strD3, TRUE, &strPres))
	{
		m_GridWnd.SetStyleRange(CGXRange(0,1),	CGXStyle().SetValue(strD1));
		m_GridWnd.SetStyleRange(CGXRange(0,2),	CGXStyle().SetValue(strD2));
		m_GridWnd.SetStyleRange(CGXRange(0,3),	CGXStyle().SetValue(strD3));
		m_GridWnd.SetStyleRange(CGXRange(0,4),	CGXStyle().SetValue(strPres));
	}
	else ASSERT(0);
	

//   CGXGridParam* param;
//   param=m_GridWnd.GetParam();
//   param->EnableSelection(GX_SELFULL);
//   param->EnableTrackRowHeight(FALSE);
	
	m_GridWnd.GetParam()->SetLockReadOnly(TRUE);
	m_GridWnd.GetParam()->EnableSelection(GX_SELFULL);
	m_GridWnd.GetParam()->EnableTrackRowHeight(FALSE);
	m_GridWnd.GetParam()->EnableTrackColWidth(FALSE);
	m_GridWnd.GetParam()->EnableMoveCols(FALSE);
	m_GridWnd.GetParam()->EnableMoveRows(FALSE);

	if(m_nMethod == WDFC_METHOD_FUNC)
	{
		if (!m_Data.strEquation.IsEmpty())
		{
			Dlg2Data(FALSE);
			Calculate(FALSE); // 처음 대화상자가 열릴땐  메시지 표시 안함.
		}
	}
	UpdateGrid();
}

void CCMWindPressureFunctionAddModDlg::UpdateGrid()
{  
	m_GridWnd.SetStyleRange(CGXRange().SetCols(COLINDEX_PRESSURE, COLINDEX_PRESSURE), CGXStyle().SetEnabled(m_nMethod == WDFC_METHOD_USER));

	m_GridWnd.SetStyleRange(CGXRange().SetCols(1), CGXStyle().SetHorizontalAlignment(DT_RIGHT));
	
	CString strD1, strD2, strD3, strPres;
	if(!GetStringByCoordFixedAxis(strD1, strD2, strD3, TRUE, &strPres))
		ASSERT(0);
	
	// Column이름 초기화
	m_GridWnd.SetValueRange(CGXRange(0, COLINDEX_X)			, strD1);
	m_GridWnd.SetValueRange(CGXRange(0, COLINDEX_Y)			, strD2);
	m_GridWnd.SetValueRange(CGXRange(0, COLINDEX_Z)			, strD3);
	m_GridWnd.SetValueRange(CGXRange(0, COLINDEX_PRESSURE)	, strPres);

	int i=0;
	int nSize = m_caD1.GetSize();
	if(nSize <= 0)
		return;

	m_GridWnd.SetRowCount(nSize);
	
	CString strTemp;
	for(i = 0; i < nSize; i++)
	{
		strTemp.Format(_T("%d"), i+1);
		m_GridWnd.SetValueRange(CGXRange(i+1, COLINDEX_NO)		, strTemp);
		strTemp.Format(_T("%.6g"), m_caD1[i]);
		m_GridWnd.SetValueRange(CGXRange(i+1, COLINDEX_X)		, strTemp);
		strTemp.Format(_T("%.6g"), m_caD2[i]);
		m_GridWnd.SetValueRange(CGXRange(i+1, COLINDEX_Y)		, strTemp);
		strTemp.Format(_T("%.6g"), m_caD3[i]);
		m_GridWnd.SetValueRange(CGXRange(i+1, COLINDEX_Z)		, strTemp);
		strTemp.Format(_T("%.6g"), m_caPresure[i]);
		m_GridWnd.SetValueRange(CGXRange(i+1, COLINDEX_PRESSURE), strTemp);
	}

//   m_GridWnd.SetStyleRange(CGXRange(1,2,nSize,3), CGXStyle()
//     .SetInterior(CTBGrid::m_Color.ReadOnlyModeBg));
	
	//기본 셀들은 우측 정렬.
	m_GridWnd.SetStyleRange(CGXRange(0,0,nSize,4), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER));
	m_GridWnd.SetStyleRange(CGXRange(1,1,nSize,4), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT));
}

void CCMWindPressureFunctionAddModDlg::InitMinMax()
{
	m_dMinX = 0.0; //모두 m 단위계로
	m_dMinY = 0.0; //모두 m 단위계로
	m_dMinZ = 0.0; //모두 m 단위계로
	m_dMaxX = 0.0; //모두 m 단위계로
	m_dMaxY = 0.0; //모두 m 단위계로
	m_dMaxZ = 0.0; //모두 m 단위계로

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_NODE_K NodeK;
	T_NODE_D NodeD;
	POSITION posNode = pDoc->m_pAttrCtrl->GetStartNode();
	BOOL bInit = TRUE;
	while(posNode)
	{
		pDoc->m_pAttrCtrl->GetNextNode(posNode, NodeK, NodeD);
		if(bInit)
		{
			m_dMinX = NodeD.x;
			m_dMinY = NodeD.y;
			m_dMinZ = NodeD.z;
			m_dMaxX = NodeD.x;
			m_dMaxY = NodeD.y;
			m_dMaxZ = NodeD.z;
			bInit = FALSE;
		}
		if(m_dMinX > NodeD.x) m_dMinX = NodeD.x;
		if(m_dMinY > NodeD.y) m_dMinY = NodeD.y;
		if(m_dMinZ > NodeD.z) m_dMinZ = NodeD.z;
		if(m_dMaxX < NodeD.x) m_dMaxX = NodeD.x;
		if(m_dMaxY < NodeD.y) m_dMaxY = NodeD.y;
		if(m_dMaxZ < NodeD.z) m_dMaxZ = NodeD.z;
	}

	double dLenFac = pDoc->m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	
	m_dMinX *= dLenFac; //모두 m 단위계로
	m_dMinY *= dLenFac; //모두 m 단위계로
	m_dMinZ *= dLenFac; //모두 m 단위계로
	m_dMaxX *= dLenFac; //모두 m 단위계로
	m_dMaxY *= dLenFac; //모두 m 단위계로
	m_dMaxZ *= dLenFac; //모두 m 단위계로
}

BOOL CCMWindPressureFunctionAddModDlg::GetValueByCoordFixedAxis( double& dFrom, double& dTo, double& dInc )
{
	int nSelCoord   = m_cbxCoord.GetCurSel();
	int nSelFixAxis = m_cbxFixAxis.GetCurSel();
	
	if(nSelCoord == 0) // Rec...
	{
		if(nSelFixAxis == 0) // X, Y
		{
			dFrom = m_dMinZ;
			dTo = m_dMaxZ;
		}
		else if(nSelFixAxis == 1) // Y, Z
		{
			dFrom = m_dMinX;
			dTo = m_dMaxX;
		}
		else if(nSelFixAxis == 2) // X, Z
		{
			dFrom = m_dMinY;
			dTo = m_dMaxY;
		}
		else return FALSE;
		dInc = (m_dMaxZ - m_dMinZ) / 10.0;
	}
	else if(nSelCoord == 1) //Cyl
	{
		if(nSelFixAxis == 0) // R, TH
		{
			dFrom = m_dMinZ;
			dTo = m_dMaxZ;
			dInc = (m_dMaxZ - m_dMinZ) / 10.0;
		}
		else if(nSelFixAxis == 1) // TH, Z
		{
			dFrom = 0;
			dTo = fabs(m_dMaxZ - m_dMinZ) / 2.0;
			dInc = dTo / 10.0;      
		}
		else if(nSelFixAxis == 2) // R, Z
		{
			dFrom = 0.0;
			dTo = 360.0;
			dInc = 30.0;
		}
		else return FALSE;
	}
	else return FALSE;

	if(fabs(dFrom) < 1E-5) dFrom = 0.0;
	if(fabs(dTo) < 1E-5)   dTo = 0.0;
	if(fabs(dInc) < 1E-5)  dInc = 0.0;

	return TRUE;
}

void CCMWindPressureFunctionAddModDlg::Calculate( BOOL bWarning /*= FALSE*/, BOOL bInit/* = FALSE*/)
{
	CString strD1, strD2, strD3;
	if(!GetStringByCoordFixedAxis(strD1, strD2, strD3))
		ASSERT(0);
	
	strD1.MakeLower();
	strD2.MakeLower();
	strD3.MakeLower();  
	
	m_caD1.RemoveAll();
	m_caD2.RemoveAll();
	m_caD3.RemoveAll();
	if(!bInit || m_nMethod == WDFC_METHOD_FUNC)
		m_caPresure.RemoveAll();
	
	double dPressure;
	double dTemp = m_d1From;

	int nSelCoord   = m_cbxCoord.GetCurSel();
	int nSelFixAxis = m_cbxFixAxis.GetCurSel();

	if(m_d1Inc < 1E-3)
	{
		if(bWarning) AfxMessageBox(_LS(IDS_CMD_WINDP_FUNCTION_DEF_TOOSMAL_INC));
		return;
	}
	if ((m_d1To - m_d1From) < 1E-3)
	{
		if(bWarning) AfxMessageBox(_LS(IDS_CMD_WINDP_FUNCTION_DEF_INVALID_TOFROM));
		return;
	}
	
	while(TRUE)
	{
		if(dTemp > m_d1To)
		{
			dTemp = m_d1To;
		}
		
		if(!CWindLoadGeneratorCtrl::GetFunctionPressure(m_Data.strEquation, LPTSTR(LPCTSTR(strD1)), dTemp,
			LPTSTR(LPCTSTR(strD2)), m_d2Fix, LPTSTR(LPCTSTR(strD3)), m_d3Fix, dPressure, bWarning))
			return;

		if(fabs(dTemp) < 1E-5)     dTemp = 0.0;
		if(fabs(m_d2Fix) < 1E-5)   m_d2Fix = 0.0;
		if(fabs(m_d3Fix) < 1E-5)   m_d3Fix = 0.0;
		if(fabs(dPressure) < 1E-5) dPressure = 0.0;

		if(nSelCoord == 0)
		{
			if(nSelFixAxis == 0)
			{
				m_caD1.Add(m_d2Fix);
				m_caD2.Add(m_d3Fix);
				m_caD3.Add(dTemp);
			}
			else if(nSelFixAxis == 1)
			{
				m_caD1.Add(dTemp);
				m_caD2.Add(m_d2Fix);
				m_caD3.Add(m_d3Fix);
			}
			else if(nSelFixAxis == 2)
			{
				m_caD1.Add(m_d2Fix);
				m_caD2.Add(dTemp);
				m_caD3.Add(m_d3Fix);
			}
			else continue;
		}
		else if(nSelCoord == 1)
		{
			if(nSelFixAxis == 0)
			{
				m_caD1.Add(m_d2Fix);
				m_caD2.Add(m_d3Fix);
				m_caD3.Add(dTemp);
			}
			else if(nSelFixAxis == 1)
			{
				m_caD1.Add(dTemp);
				m_caD2.Add(m_d2Fix);
				m_caD3.Add(m_d3Fix);        
			}
			else if(nSelFixAxis == 2)
			{
				m_caD1.Add(m_d2Fix);
				m_caD2.Add(dTemp);
				m_caD3.Add(m_d3Fix); 
			}
			else continue;
		}
		else continue;

		if(!bInit || m_nMethod == WDFC_METHOD_FUNC)
			m_caPresure.Add(dPressure);
		
		if (fabs(dTemp - m_d1To) < 1E-3)
			break;
		
		dTemp += m_d1Inc;
	}
}

void CCMWindPressureFunctionAddModDlg::OnCmdFuncHelper()
{
	if(m_pHelpDlg) //이미 열려있다면 지운다(토글)
	{
		DestoryHelperDlg();
	}
	else // 열려 있지 않다면 새로 듸운다.
	{
		m_pHelpDlg = new CCMWindPressureFunctionHelper(this);
		m_pHelpDlg->Create(CCMWindPressureFunctionHelper::IDD, this);

		CRect rectParent, rectChild, rectNewPos;
		GetWindowRect(&rectParent);
		m_pHelpDlg->GetWindowRect(&rectChild);
		m_pHelpDlg->ShowWindow(SW_SHOW);

		rectNewPos.top = rectParent.top;
		rectNewPos.bottom = rectNewPos.top + rectChild.Height();

		BOOL  bMaximized = FALSE;
		BOOL  bOutside = FALSE;
		DWORD style = this->GetStyle();
		int   nSystemWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int   nRightPos = rectParent.right + rectChild.Width();

		//모니터를 벗어나거나, 최대화상태일때.
		if(rectParent.right > nSystemWidth) nRightPos -= nSystemWidth;
		if(nRightPos > nSystemWidth)        bOutside = TRUE;
		if (style & WS_MAXIMIZE)            bMaximized = TRUE;

		if(bMaximized || bOutside)
		{
			rectNewPos.left = rectParent.right - rectChild.Width();
			rectNewPos.right = rectNewPos.left + rectChild.Width();
		}
		else
		{
			rectNewPos.left = rectParent.right;
			rectNewPos.right = rectNewPos.left + rectChild.Width();
		}

		m_pHelpDlg->MoveWindow(rectNewPos);
	}
}

void CCMWindPressureFunctionAddModDlg::DestoryHelperDlg()
{
	if(m_pHelpDlg)
	{
		if(m_pHelpDlg->GetSafeHwnd() && ::IsWindow(m_pHelpDlg->GetSafeHwnd()))
		{
			m_pHelpDlg->DestroyWindow();
		}
	}
	m_pHelpDlg = NULL;  
}

void CCMWindPressureFunctionAddModDlg::AppendEquation( const CString& strAdd )
{
	CString strTemp;
	CString strOrg;
	GetDlgItem(IDC_WINDP_FUNCEQUATION)->GetWindowText(strOrg);
	strTemp.Format(_T("%s%s"), strOrg, strAdd);
	GetDlgItem(IDC_WINDP_FUNCEQUATION)->SetWindowText(strTemp);
}

void CCMWindPressureFunctionAddModDlg::ApplyEquation( const CString& strApply )
{
	GetDlgItem(IDC_WINDP_FUNCEQUATION)->SetWindowText(strApply);
}

BOOL CCMWindPressureFunctionAddModDlg::IsRectCoordSys()
{
	int nSel = m_cbxCoord.GetCurSel();
	if(nSel == 0)      return TRUE;
	else if(nSel == 1) return FALSE;

	ASSERT(0);
	return FALSE;  
}

BOOL CCMWindPressureFunctionAddModDlg::CheckValid()
{
	CString strD1;
	CString strD2;
	CString strD3;
	if(!GetStringByCoordFixedAxis(strD1, strD2, strD3))
		return FALSE;

	strD1.MakeLower();
	strD2.MakeLower();
	strD3.MakeLower();
	double dPressure;
	return CWindLoadGeneratorCtrl::GetFunctionPressure(m_Data.strEquation,
		LPTSTR(LPCTSTR(strD1)), 0.0,
		LPTSTR(LPCTSTR(strD2)), 0.0,
		LPTSTR(LPCTSTR(strD3)), 0.0, dPressure, TRUE);
}
