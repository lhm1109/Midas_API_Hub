// DgnStlSPCSectJTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlSPCSectJTabDlg.h"

#include "DgnSteelSPCSectJGrid.h"
#include "DgnStlSPCSectDlg.h"

#include "..\wg_db\DgnDataCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_cmd\CMGeometry.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSPCSectJTabDlg dialog


CDgnStlSPCSectJTabDlg::CDgnStlSPCSectJTabDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnStlSPCSectJTabDlg::IDD, pParent)
{
	m_pParent = (CDgnStlSPCSectDlg*)pParent;
	m_arPoint1Group.RemoveAll();
	m_arPoint2Group.RemoveAll();
	m_arPoint3Group.RemoveAll();
	m_arPoint4Group.RemoveAll();
	m_arPoint1SubGroup.RemoveAll();
	m_arPoint2SubGroup.RemoveAll();
	m_arPoint3SubGroup.RemoveAll();
	m_arPoint4SubGroup.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
	m_nRowCount = 0;

	m_Grid = NULL;

	bExistJ = TRUE;

	//{{AFX_DATA_INIT(CDgnStlSPCSectJTabDlg)	
	//}}AFX_DATA_INIT
}


void CDgnStlSPCSectJTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlSPCSectJTabDlg)
	DDX_Control(pDX, IDC_DGN_CENT_Y_EDIT, m_editCent_Y);
	DDX_Control(pDX, IDC_DGN_CENT_Z_EDIT, m_editCent_Z);
	DDX_Control(pDX, IDC_DGN_1_Y_EDIT, m_edit1_Y);
	DDX_Control(pDX, IDC_DGN_1_Z_EDIT, m_edit1_Z);
	DDX_Control(pDX, IDC_DGN_2_Y_EDIT, m_edit2_Y);
	DDX_Control(pDX, IDC_DGN_2_Z_EDIT, m_edit2_Z);
	DDX_Control(pDX, IDC_DGN_3_Y_EDIT, m_edit3_Y);
	DDX_Control(pDX, IDC_DGN_3_Z_EDIT, m_edit3_Z);
	DDX_Control(pDX, IDC_DGN_4_Y_EDIT, m_edit4_Y);
	DDX_Control(pDX, IDC_DGN_4_Z_EDIT, m_edit4_Z);
	DDX_Control(pDX, IDC_DGN_REDRAW_BTN, m_btnRedraw);
	DDX_Control(pDX, IDC_DGN_UNIT_STATIC, m_textUnit);
	DDX_Control(pDX, IDC_DGN_1_COMBO, m_cboPoint1);  
	DDX_Control(pDX, IDC_DGN_2_COMBO, m_cboPoint2);  
	DDX_Control(pDX, IDC_DGN_3_COMBO, m_cboPoint3);  
	DDX_Control(pDX, IDC_DGN_4_COMBO, m_cboPoint4);  
	DDX_Control(pDX, IDC_DGN_POINT_1_RADIO, m_RadioPoint1);
	DDX_Control(pDX, IDC_DGN_POINT_2_RADIO, m_RadioPoint2);
	DDX_Control(pDX, IDC_DGN_POINT_3_RADIO, m_RadioPoint3);
	DDX_Control(pDX, IDC_DGN_POINT_4_RADIO, m_RadioPoint4);
	DDX_Control(pDX, IDC_DGN_GRID, *m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlSPCSectJTabDlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnStlSPCSectJTabDlg)
	ON_BN_CLICKED(IDC_DGN_REDRAW_BTN, OnClickRedrawBtn)
	ON_EN_KILLFOCUS(IDC_DGN_1_Y_EDIT, OnKillfocusPoint1YEdit)
	ON_EN_KILLFOCUS(IDC_DGN_2_Y_EDIT, OnKillfocusPoint2YEdit)
	ON_EN_KILLFOCUS(IDC_DGN_3_Y_EDIT, OnKillfocusPoint3YEdit)
	ON_EN_KILLFOCUS(IDC_DGN_4_Y_EDIT, OnKillfocusPoint4YEdit)
	ON_EN_KILLFOCUS(IDC_DGN_1_Z_EDIT, OnKillfocusPoint1ZEdit)
	ON_EN_KILLFOCUS(IDC_DGN_2_Z_EDIT, OnKillfocusPoint2ZEdit)
	ON_EN_KILLFOCUS(IDC_DGN_3_Z_EDIT, OnKillfocusPoint3ZEdit)
	ON_EN_KILLFOCUS(IDC_DGN_4_Z_EDIT, OnKillfocusPoint4ZEdit)
	ON_BN_CLICKED(IDC_DGN_POINT_1_RADIO, OnClickRadioPoint1)
	ON_BN_CLICKED(IDC_DGN_POINT_2_RADIO, OnClickRadioPoint2)
	ON_BN_CLICKED(IDC_DGN_POINT_3_RADIO, OnClickRadioPoint3)
	ON_BN_CLICKED(IDC_DGN_POINT_4_RADIO, OnClickRadioPoint4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSPCSectJTabDlg message handlers

void CDgnStlSPCSectJTabDlg::PostNcDestroy() 
{
	if(m_Grid)		{ delete m_Grid;		m_Grid	= NULL;	}  
		
	CChildDialog::PostNcDestroy();
}

BOOL CDgnStlSPCSectJTabDlg::OnInitDialog() 
{
	InitLineData();
	m_Grid = new CDgnSteelSPCSectJGrid(m_nRowCount);

	CChildDialog::OnInitDialog();

	// Init Grid
	m_Grid->Initialize(this); 

	SetInitUnit();	
	DataToDlg();
	DataToGrid();  

	SetPointModeNon();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlSPCSectJTabDlg::SetInitUnit()
{
	m_edit1_Y.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edit1_Z.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edit2_Y.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edit2_Z.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_edit3_Y.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edit3_Z.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_edit4_Y.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edit4_Z.SetUnitType(D_UNITSYS_BASE_LENGTH);	

	m_editCent_Y.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editCent_Z.SetUnitType(D_UNITSYS_BASE_LENGTH);

	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	int iLenID = CurIndex.nBase_Length;
	
	CString strLenUnit=_T("");
	switch(iLenID)
	{
		case(D_UNITSYS_LENGTH_INDEX_M ):	
			strLenUnit.Format(_LS(IDS_DGN_CURRENT_UNIT), D_UNITSYS_LENGTH_NAME_M);
			break;
		case(D_UNITSYS_LENGTH_INDEX_CM):	
			strLenUnit.Format(_LS(IDS_DGN_CURRENT_UNIT), D_UNITSYS_LENGTH_NAME_CM);			
			break;
		case(D_UNITSYS_LENGTH_INDEX_MM):	
			strLenUnit.Format(_LS(IDS_DGN_CURRENT_UNIT), D_UNITSYS_LENGTH_NAME_MM);			
			break;
		case(D_UNITSYS_LENGTH_INDEX_IN):	
			strLenUnit.Format(_LS(IDS_DGN_CURRENT_UNIT), D_UNITSYS_LENGTH_NAME_IN);			
			break;
		case(D_UNITSYS_LENGTH_INDEX_FT):	
			strLenUnit.Format(_LS(IDS_DGN_CURRENT_UNIT), D_UNITSYS_LENGTH_NAME_FT);			
			break;
	}
	m_textUnit.SetWindowText(strLenUnit);
}

void CDgnStlSPCSectJTabDlg::InitLineData()
{
	m_nRowCount = 0;

	int nShapeSize = m_sectData->aShape.GetSize();//Group의 갯수
	int nLdgnSize = 0;//각각Group의 SubGroup갯수
	T_GSEC_LSHAP shapData;
	for(int i=0 ; i<nShapeSize ; ++i)
	{
		shapData.Initialize();
		shapData = m_sectData->aShape[i];
		nLdgnSize = shapData.aLdgn.GetSize();
		for(int j=0 ; j<nLdgnSize ; ++j)
		{
			++m_nRowCount;
		}
	}	
}

void CDgnStlSPCSectJTabDlg::SelectLineRedraw(ROWCOL nRow)
{ 
	m_pParent->SelectLineRedraw(nRow);
}

void CDgnStlSPCSectJTabDlg::DataToDlg()
{
	if(bExistJ)
	{
		m_editCent_Y.SetEditUnit(m_sectData->Design.YBar);
		m_editCent_Z.SetEditUnit(m_sectData->Design.ZBar);

		m_edit1_Y.SetEditUnit(m_sectData->Stiffness.dx1);
		m_edit1_Z.SetEditUnit(m_sectData->Stiffness.dy1);
		m_edit2_Y.SetEditUnit(m_sectData->Stiffness.dx2);
		m_edit2_Z.SetEditUnit(m_sectData->Stiffness.dy2);
		m_edit3_Y.SetEditUnit(m_sectData->Stiffness.dx3);
		m_edit3_Z.SetEditUnit(m_sectData->Stiffness.dy3);
		m_edit4_Y.SetEditUnit(m_sectData->Stiffness.dx4);
		m_edit4_Z.SetEditUnit(m_sectData->Stiffness.dy4);

		SetPointComboItem();

		if(m_sectData->aShape.GetSize() <= 0)
			return;

		CString str = _T("");
		str = m_sectData->aShape[m_sectData->ShapeIndex[0]].aLdgn[m_sectData->LdgnIndex[0]].sName;
		m_cboPoint1.SelectString(-1, str);
		str = m_sectData->aShape[m_sectData->ShapeIndex[1]].aLdgn[m_sectData->LdgnIndex[1]].sName;
		m_cboPoint2.SelectString(-1, str);
		str = m_sectData->aShape[m_sectData->ShapeIndex[2]].aLdgn[m_sectData->LdgnIndex[2]].sName;
		m_cboPoint3.SelectString(-1, str);
		str = m_sectData->aShape[m_sectData->ShapeIndex[3]].aLdgn[m_sectData->LdgnIndex[3]].sName;
		m_cboPoint4.SelectString(-1, str);
	}
	else
	{
		m_editCent_Y.SetWindowText(_T("-"));
		m_editCent_Z.SetWindowText(_T("-"));
		m_edit1_Y.SetWindowText(_T("-"));
		m_edit1_Z.SetWindowText(_T("-"));
		m_edit2_Y.SetWindowText(_T("-"));
		m_edit2_Z.SetWindowText(_T("-"));
		m_edit3_Y.SetWindowText(_T("-"));
		m_edit3_Z.SetWindowText(_T("-"));
		m_edit4_Y.SetWindowText(_T("-"));
		m_edit4_Z.SetWindowText(_T("-"));
	}

	SetControlEnable();
}

BOOL CDgnStlSPCSectJTabDlg::DlgToData()
{
	if(!bExistJ)
		return FALSE;

	if(m_arPoint1Group.GetSize() == 0 || m_arPoint2Group.GetSize() == 0 ||
		 m_arPoint3Group.GetSize() == 0 || m_arPoint4Group.GetSize() == 0)
	{
		return FALSE;
	}

	if(m_arPoint1SubGroup.GetSize() == 0 || m_arPoint2SubGroup.GetSize() == 0 ||
		 m_arPoint3SubGroup.GetSize() == 0 || m_arPoint4SubGroup.GetSize() == 0)
	{
		return FALSE;
	}

	m_sectData->Stiffness.dx1 = m_edit1_Y.GetEditValue();
	m_sectData->Stiffness.dy1 = m_edit1_Z.GetEditValue();
	m_sectData->Stiffness.dx2 = m_edit2_Y.GetEditValue();
	m_sectData->Stiffness.dy2 = m_edit2_Z.GetEditValue();
	m_sectData->Stiffness.dx3 = m_edit3_Y.GetEditValue();
	m_sectData->Stiffness.dy3 = m_edit3_Z.GetEditValue();
	m_sectData->Stiffness.dx4 = m_edit4_Y.GetEditValue();
	m_sectData->Stiffness.dy4 = m_edit4_Z.GetEditValue();

	int nIndex = m_cboPoint1.GetCurSel();
	m_sectData->ShapeIndex[0] = m_arPoint1Group[nIndex];
	m_sectData->LdgnIndex[0] = m_arPoint1SubGroup[nIndex];
	nIndex = m_cboPoint2.GetCurSel();
	m_sectData->ShapeIndex[1] = m_arPoint2Group[nIndex];
	m_sectData->LdgnIndex[1] = m_arPoint2SubGroup[nIndex];
	nIndex = m_cboPoint3.GetCurSel();
	m_sectData->ShapeIndex[2] = m_arPoint3Group[nIndex];
	m_sectData->LdgnIndex[2] = m_arPoint3SubGroup[nIndex];
	nIndex = m_cboPoint4.GetCurSel();
	m_sectData->ShapeIndex[3] = m_arPoint4Group[nIndex];
	m_sectData->LdgnIndex[3] = m_arPoint4SubGroup[nIndex];  

	return TRUE;
}

void CDgnStlSPCSectJTabDlg::SetControlEnable()
{
	if(bExistJ)
	{
		m_edit1_Y.EnableWindow(TRUE);
		m_edit1_Z.EnableWindow(TRUE);
		m_edit2_Y.EnableWindow(TRUE);
		m_edit2_Z.EnableWindow(TRUE);
		m_edit3_Y.EnableWindow(TRUE);
		m_edit3_Z.EnableWindow(TRUE);
		m_edit4_Y.EnableWindow(TRUE);
		m_edit4_Z.EnableWindow(TRUE);
	}
	else
	{
		m_edit1_Y.EnableWindow(FALSE);
		m_edit1_Z.EnableWindow(FALSE);
		m_edit2_Y.EnableWindow(FALSE);
		m_edit2_Z.EnableWindow(FALSE);
		m_edit3_Y.EnableWindow(FALSE);
		m_edit3_Z.EnableWindow(FALSE);
		m_edit4_Y.EnableWindow(FALSE);
		m_edit4_Z.EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Grid 관련 
//

void CDgnStlSPCSectJTabDlg::DataToGrid()
{
	if(!bExistJ)
		return;
	
	m_aGroupIndex.RemoveAll();
	m_aGroupIndex.SetSize(m_nRowCount);
	m_aSubGroupIndex.RemoveAll();
	m_aSubGroupIndex.SetSize(m_nRowCount);

	InitLineData();
	m_Grid->SetRowCount(m_nRowCount);

	CString strGroup = _T("");
	CString strSubGroup = _T("");
	CString strClass = _T("");
	double	dLength = 0;
	double	dThickness = 0;
	CString	strPanel = _T("");
	CString strException = _T("");

	T_GSEC_LSHAP shapData;
	T_GSEC_LDGN	ldgnData;
	int nShapeSize = m_sectData->aShape.GetSize();//Group의 갯수
	int nLdgnSize = 0;//각각Group의 SubGroup갯수
	
	int rowCount = 0;
	for(int i=0 ; i<nShapeSize ; ++i)
	{
		shapData.Initialize();
		shapData = m_sectData->aShape[i];
		strGroup = shapData.sName;

		nLdgnSize = shapData.aLdgn.GetSize();
		for(int j=0 ; j<nLdgnSize ; ++j)
		{
			ldgnData.Initialize();
			ldgnData = shapData.aLdgn[j];
				
			strSubGroup = ldgnData.sName;
			if(ldgnData.iBoundary == 0)
				strClass = _LS(IDS_DGN_CLASS_1);//_T("자유돌출판")
			else
				strClass = _LS(IDS_DGN_CLASS_2);//_T("양연지지판")
			dLength = ldgnData.db;
			dThickness = ldgnData.dt;
			if(ldgnData.iPanNum == 0)
				strPanel = _T("-");
			else
				strPanel.Format(_T("%d"), ldgnData.iPanNum);

			if(ldgnData.bExcept)
				strException = _T("1");
			else
				strException = _T("0");
			
			///////////////////////////////////////////////////////////
			m_Grid->SetValueRange(CGXRange(rowCount+1, 0), strGroup);
			m_Grid->SetValueRange(CGXRange(rowCount+1, 1), strSubGroup);
			m_Grid->SetValueRange(CGXRange(rowCount+1, 2), strClass);
			m_Grid->SetValueRange(CGXRange(rowCount+1, 3), dLength);
			m_Grid->SetValueRange(CGXRange(rowCount+1, 4), dThickness);
			m_Grid->SetValueRange(CGXRange(rowCount+1, 5), strPanel);
			if(ldgnData.iBoundary == 0)//_T("자유돌출판")
				m_Grid->SetValueRange(CGXRange(rowCount+1, 6), strException);
			else
			{       
				m_Grid->SetStyleRange(CGXRange(rowCount+1, 6), CGXStyle()
			  .SetControl(GX_IDS_CTRL_STATIC)    
				.SetHorizontalAlignment(DT_CENTER)
			  .SetEnabled(FALSE)
				.SetValueType(GX_VT_STRING));
				m_Grid->SetValueRange(CGXRange(rowCount+1, 6), _T(""));
			}	
			m_aGroupIndex[rowCount] = i;
			m_aSubGroupIndex[rowCount] = j;
			rowCount++;
			///////////////////////////////////////////////////////////
		}
	}
}

void CDgnStlSPCSectJTabDlg::GridToData()
{
	if(!bExistJ)
		return;
	
	double dLength = 0.;
	double dThickness = 0.;
	int nPanel = 0;
	BOOL bException = 0;
	CString strTemp = _T("");
	for(int i=0 ; i<m_nRowCount ;++i)
	{
		strTemp = m_Grid->GetValueRowCol(i+1, 3);
		dLength = _tstof(strTemp);
		strTemp	=	m_Grid->GetValueRowCol(i+1, 4);
		dThickness = _tstof(strTemp);
		CString strTemp = m_Grid->GetValueRowCol(i+1, 5);
		if(strTemp == _T("-"))
			nPanel = 0;
		else
		{
			int nTemp = _ttoi(strTemp);
			if(nTemp <= 0)
				nPanel = 0;
			else
				nPanel = nTemp;
		}
		strTemp	=	m_Grid->GetValueRowCol(i+1, 6);
		if(strTemp == _T("0"))
			bException = FALSE;
		else
			bException = TRUE;
		if(m_sectData->aShape[m_aGroupIndex[i]].aLdgn[m_aSubGroupIndex[i]].iBoundary != 0)//_T("양연지지판")
		  bException = FALSE;		
		m_sectData->aShape[m_aGroupIndex[i]].aLdgn[m_aSubGroupIndex[i]].db = dLength;
		m_sectData->aShape[m_aGroupIndex[i]].aLdgn[m_aSubGroupIndex[i]].dt = dThickness;
		m_sectData->aShape[m_aGroupIndex[i]].aLdgn[m_aSubGroupIndex[i]].iPanNum = nPanel;
		m_sectData->aShape[m_aGroupIndex[i]].aLdgn[m_aSubGroupIndex[i]].bExcept = bException;  
	}
}




void CDgnStlSPCSectJTabDlg::OnClickRedrawBtn() 
{
	if(!DlgToData()) return;
	GridToData();

	m_pParent->Redraw(FALSE);
	m_pParent->SetPointMode(POINT_MODE_NONE);
	SetPointModeNon();
}

void CDgnStlSPCSectJTabDlg::PointRedraw() 
{
	if(!DlgToData()) return;
	GridToData();	
	
	m_pParent->Redraw(FALSE);
	//m_pParent->SetPointMode(POINT_MODE_NONE);
}

void CDgnStlSPCSectJTabDlg::InitPoint1Combo()
{
	double dY, dZ = 0.0;
	dY = m_edit1_Y.GetEditValue();
	dZ = m_edit1_Z.GetEditValue();

	m_arPoint1Group.RemoveAll();
	m_arPoint1SubGroup.RemoveAll();
	CSectUtil::GetSubGroupList(m_sectData, dY, dZ, m_arPoint1Group, m_arPoint1SubGroup);
	
	CString str = _T("");
	if(m_arPoint1Group.GetSize()==m_arPoint1SubGroup.GetSize() && m_arPoint1Group.GetSize()>0)
	{
		m_cboPoint1.ResetContent();

		int nSize = m_arPoint1SubGroup.GetSize();
		for(int i=0 ; i<nSize ; ++i)
		{
			str = m_sectData->aShape[m_arPoint1Group[i]].aLdgn[m_arPoint1SubGroup[i]].sName;
			m_cboPoint1.AddString(str);
		}

		m_cboPoint1.SetCurSel(0);
	}
}

void CDgnStlSPCSectJTabDlg::InitPoint2Combo()
{
	double dY, dZ = 0.0;
	dY = m_edit2_Y.GetEditValue();
	dZ = m_edit2_Z.GetEditValue();

	m_arPoint2Group.RemoveAll();
	m_arPoint2SubGroup.RemoveAll();
	CSectUtil::GetSubGroupList(m_sectData, dY, dZ, m_arPoint2Group, m_arPoint2SubGroup);
	
	CString str = _T("");
	if(m_arPoint2Group.GetSize()==m_arPoint2SubGroup.GetSize() && m_arPoint2Group.GetSize()>0)
	{
		m_cboPoint2.ResetContent();

		int nSize = m_arPoint2SubGroup.GetSize();
		for(int i=0 ; i<nSize ; ++i)
		{
			str = m_sectData->aShape[m_arPoint2Group[i]].aLdgn[m_arPoint2SubGroup[i]].sName;
			m_cboPoint2.AddString(str);
		}
		m_cboPoint2.SetCurSel(0);
	}
}

void CDgnStlSPCSectJTabDlg::InitPoint3Combo()
{
	double dY, dZ = 0.0;
	dY = m_edit3_Y.GetEditValue();
	dZ = m_edit3_Z.GetEditValue();

	m_arPoint3Group.RemoveAll();
	m_arPoint3SubGroup.RemoveAll();
	CSectUtil::GetSubGroupList(m_sectData, dY, dZ, m_arPoint3Group, m_arPoint3SubGroup);
	
	CString str = _T("");
	if(m_arPoint3Group.GetSize()==m_arPoint3SubGroup.GetSize() && m_arPoint3Group.GetSize()>0)
	{
		m_cboPoint3.ResetContent();

		int nSize = m_arPoint3SubGroup.GetSize();
		for(int i=0 ; i<nSize ; ++i)
		{
			str = m_sectData->aShape[m_arPoint3Group[i]].aLdgn[m_arPoint3SubGroup[i]].sName;
			m_cboPoint3.AddString(str);
		}
		m_cboPoint3.SetCurSel(0);
	}
}

void CDgnStlSPCSectJTabDlg::InitPoint4Combo()
{
	double dY, dZ = 0.0;
	dY = m_edit4_Y.GetEditValue();
	dZ = m_edit4_Z.GetEditValue();

	m_arPoint4Group.RemoveAll();
	m_arPoint4SubGroup.RemoveAll();
	CSectUtil::GetSubGroupList(m_sectData, dY, dZ, m_arPoint4Group, m_arPoint4SubGroup);
	
	CString str = _T("");
	if(m_arPoint4Group.GetSize()==m_arPoint4SubGroup.GetSize() && m_arPoint4Group.GetSize()>0)
	{
		m_cboPoint4.ResetContent();

		int nSize = m_arPoint4SubGroup.GetSize();
		for(int i=0 ; i<nSize ; ++i)
		{
			str = m_sectData->aShape[m_arPoint4Group[i]].aLdgn[m_arPoint4SubGroup[i]].sName;
			m_cboPoint4.AddString(str);
		}
		m_cboPoint4.SetCurSel(0);
	}
}

void CDgnStlSPCSectJTabDlg::OnKillfocusPoint1YEdit() 
{	
	//(2009.01.05) Add by Unsang :: MinLength와 비교 후 만족하지 않을 경우 Message생성!!
	double dX, dY;
	dX = m_edit1_Y.GetEditValue();
	dY = m_edit1_Z.GetEditValue();

	BOOL bSearch = FALSE;
	double PositionX, PositionY = 0.0;
	
	if(!CSectUtil::GetNearPoint(m_sectData, dX, dY, PositionX, PositionY))
	{
		AfxMessageBox(_T("허용범위를 벗어난 값을 입력하였습니다."));
		m_edit1_Y.SetEditUnit(m_sectData->Stiffness.dx1);
	}
	
	InitPoint1Combo();
	PointRedraw();
}

void CDgnStlSPCSectJTabDlg::OnKillfocusPoint2YEdit() 
{	
	//(2009.01.05) Add by Unsang :: MinLength와 비교 후 만족하지 않을 경우 Message생성!!
	double dX, dY;
	dX = m_edit2_Y.GetEditValue();
	dY = m_edit2_Z.GetEditValue();

	BOOL bSearch = FALSE;
	double PositionX, PositionY = 0.0;
	
	if(!CSectUtil::GetNearPoint(m_sectData, dX, dY, PositionX, PositionY))
	{
		AfxMessageBox(_T("허용범위를 벗어난 값을 입력하였습니다."));
		m_edit2_Y.SetEditUnit(m_sectData->Stiffness.dx2);
	}
	
	InitPoint2Combo();
	PointRedraw();
}

void CDgnStlSPCSectJTabDlg::OnKillfocusPoint3YEdit() 
{	
	//(2009.01.05) Add by Unsang :: MinLength와 비교 후 만족하지 않을 경우 Message생성!!
	double dX, dY;
	dX = m_edit3_Y.GetEditValue();
	dY = m_edit3_Z.GetEditValue();

	BOOL bSearch = FALSE;
	double PositionX, PositionY = 0.0;
	
	if(!CSectUtil::GetNearPoint(m_sectData, dX, dY, PositionX, PositionY))
	{
		AfxMessageBox(_T("허용범위를 벗어난 값을 입력하였습니다."));
		m_edit3_Y.SetEditUnit(m_sectData->Stiffness.dx3);
	}
	
	InitPoint3Combo();
	PointRedraw();
}

void CDgnStlSPCSectJTabDlg::OnKillfocusPoint4YEdit() 
{	
	//(2009.01.05) Add by Unsang :: MinLength와 비교 후 만족하지 않을 경우 Message생성!!
	double dX, dY;
	dX = m_edit4_Y.GetEditValue();
	dY = m_edit4_Z.GetEditValue();

	BOOL bSearch = FALSE;
	double PositionX, PositionY = 0.0;
	
	if(!CSectUtil::GetNearPoint(m_sectData, dX, dY, PositionX, PositionY))
	{
		AfxMessageBox(_T("허용범위를 벗어난 값을 입력하였습니다."));
		m_edit4_Y.SetEditUnit(m_sectData->Stiffness.dx4);
	}
	
	InitPoint4Combo();
	PointRedraw();
}

void CDgnStlSPCSectJTabDlg::OnKillfocusPoint1ZEdit() 
{	
	//(2009.01.05) Add by Unsang :: MinLength와 비교 후 만족하지 않을 경우 Message생성!!
	double dX, dY;
	dX = m_edit1_Y.GetEditValue();
	dY = m_edit1_Z.GetEditValue();

	BOOL bSearch = FALSE;
	double PositionX, PositionY = 0.0;
	
	if(!CSectUtil::GetNearPoint(m_sectData, dX, dY, PositionX, PositionY))
	{
		AfxMessageBox(_T("허용범위를 벗어난 값을 입력하였습니다."));
		m_edit1_Z.SetEditUnit(m_sectData->Stiffness.dy1);
	}
	
	InitPoint1Combo();
	PointRedraw();
}

void CDgnStlSPCSectJTabDlg::OnKillfocusPoint2ZEdit() 
{	
	//(2009.01.05) Add by Unsang :: MinLength와 비교 후 만족하지 않을 경우 Message생성!!
	double dX, dY;
	dX = m_edit2_Y.GetEditValue();
	dY = m_edit2_Z.GetEditValue();

	BOOL bSearch = FALSE;
	double PositionX, PositionY = 0.0;
	
	if(!CSectUtil::GetNearPoint(m_sectData, dX, dY, PositionX, PositionY))
	{
		AfxMessageBox(_T("허용범위를 벗어난 값을 입력하였습니다."));
		m_edit2_Z.SetEditUnit(m_sectData->Stiffness.dy2);
	}
	
	InitPoint2Combo();
	PointRedraw();
}

void CDgnStlSPCSectJTabDlg::OnKillfocusPoint3ZEdit() 
{	
	//(2009.01.05) Add by Unsang :: MinLength와 비교 후 만족하지 않을 경우 Message생성!!
	double dX, dY;
	dX = m_edit3_Y.GetEditValue();
	dY = m_edit3_Z.GetEditValue();

	BOOL bSearch = FALSE;
	double PositionX, PositionY = 0.0;
	
	if(!CSectUtil::GetNearPoint(m_sectData, dX, dY, PositionX, PositionY))
	{
		AfxMessageBox(_T("허용범위를 벗어난 값을 입력하였습니다."));
		m_edit3_Z.SetEditUnit(m_sectData->Stiffness.dy3);
	}
	
	InitPoint3Combo();
	PointRedraw();
}

void CDgnStlSPCSectJTabDlg::OnKillfocusPoint4ZEdit() 
{	
	//(2009.01.05) Add by Unsang :: MinLength와 비교 후 만족하지 않을 경우 Message생성!!
	double dX, dY;
	dX = m_edit4_Y.GetEditValue();
	dY = m_edit4_Z.GetEditValue();

	BOOL bSearch = FALSE;
	double PositionX, PositionY = 0.0;
	
	if(!CSectUtil::GetNearPoint(m_sectData, dX, dY, PositionX, PositionY))
	{
		AfxMessageBox(_T("허용범위를 벗어난 값을 입력하였습니다."));
		m_edit4_Z.SetEditUnit(m_sectData->Stiffness.dy4);
	}
	
	InitPoint4Combo();
	PointRedraw();
}

void CDgnStlSPCSectJTabDlg::SetPointComboItem()
{
	InitPoint1Combo();
	InitPoint2Combo();
	InitPoint3Combo();
	InitPoint4Combo();
}

void CDgnStlSPCSectJTabDlg::SetPointEdit(int nPointMode, double dPositionX, double dPositionY)
{
	if(nPointMode == POINT_MODE_NONE)
		return;
	else if(nPointMode == POINT_MODE_1)
	{
		m_edit1_Y.SetEditUnit(dPositionX);
	  m_edit1_Z.SetEditUnit(dPositionY);
		InitPoint1Combo();
	}
	else if(nPointMode == POINT_MODE_2)
	{
		m_edit2_Y.SetEditUnit(dPositionX);
	  m_edit2_Z.SetEditUnit(dPositionY);
		InitPoint2Combo();
	}
	else if(nPointMode == POINT_MODE_3)
	{
		m_edit3_Y.SetEditUnit(dPositionX);
	  m_edit3_Z.SetEditUnit(dPositionY);
		InitPoint3Combo();
	}
	else if(nPointMode == POINT_MODE_4)
	{
		m_edit4_Y.SetEditUnit(dPositionX);
	  m_edit4_Z.SetEditUnit(dPositionY);
		InitPoint4Combo();
	} 
	PointRedraw();
}

void CDgnStlSPCSectJTabDlg::OnClickRadioPoint1() 
{
	if(m_RadioPoint1.GetCheck())
		m_pParent->SetPointMode(POINT_MODE_1);
}

void CDgnStlSPCSectJTabDlg::OnClickRadioPoint2() 
{
	if(m_RadioPoint2.GetCheck())
		m_pParent->SetPointMode(POINT_MODE_2);
}

void CDgnStlSPCSectJTabDlg::OnClickRadioPoint3() 
{
	if(m_RadioPoint3.GetCheck())
		m_pParent->SetPointMode(POINT_MODE_3);
}

void CDgnStlSPCSectJTabDlg::OnClickRadioPoint4() 
{
	if(m_RadioPoint4.GetCheck())
		m_pParent->SetPointMode(POINT_MODE_4);
}

void CDgnStlSPCSectJTabDlg::SetPointModeNon()
{
	m_RadioPoint1.SetCheck(FALSE);
	m_RadioPoint2.SetCheck(FALSE);
	m_RadioPoint3.SetCheck(FALSE);
	m_RadioPoint4.SetCheck(FALSE);
}
