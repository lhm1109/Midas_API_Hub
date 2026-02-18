// DgnConWeldMeshDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConWeldMeshDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"

#include "..\wg_common\wg_common_TBGrid.h"

#include "..\wg_base\wg_base_I_GridColorMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConWeldMeshDlg dialog


CDgnConWeldMeshDlg::CDgnConWeldMeshDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConWeldMeshDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConWeldMeshDlg)
	m_nType = -1;
	m_strMeshName=_T("");
	m_strVBarName=_T(""); m_dVBarSpace=0.0; m_dVBarArea=0.0;
	m_strHBarName=_T(""); m_dHBarSpace=0.0; m_dHBarArea=0.0;
	//}}AFX_DATA_INIT
}


void CDgnConWeldMeshDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConWeldMeshDlg)
	DDX_Control(pDX, IDC_DGN_CON_WELDMESH_NAME, m_Fabric);
	DDX_Radio(pDX, IDC_DGN_CON_WELDMESH_TYPE1, m_nType);
	DDX_Control(pDX, IDC_DGN_CON_WELDMESH_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConWeldMeshDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConWeldMeshDlg)
	ON_CBN_SELCHANGE(IDC_DGN_CON_WELDMESH_NAME, OnSelchangeDgnWeldMeshName)
	ON_BN_CLICKED(IDC_DGN_CON_WELDMESH_TYPE1, OnDgnChangeMeshType)
	ON_BN_CLICKED(IDC_DGN_CON_WELDMESH_TYPE2, OnDgnChangeMeshType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConWeldMeshDlg message handlers

void CDgnConWeldMeshDlg::OnSelchangeDgnWeldMeshName() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int Index = m_Fabric.GetCurSel();
	m_Fabric.GetLBText(Index,m_strMeshName);
	Write_GridTitle();

	UpdateData(FALSE);
}

void CDgnConWeldMeshDlg::OnDgnChangeMeshType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iFabricType = m_nType;
	Init_WeldMeshComboBox(iFabricType);

	UpdateData(FALSE);
}

void CDgnConWeldMeshDlg::OnOK() 
{
	CDialogMove::OnOK();
}

void CDgnConWeldMeshDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

BOOL CDgnConWeldMeshDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	m_pDoc->m_pMatlDB->Create_RebarData(CurIndex.nBase_Length, CurIndex.nBase_Force, TRUE);

	int iFabricType = 0;
	if(fabs(m_dVBarSpace-m_dHBarSpace) < 1.0E-07 && m_strVBarName == m_strHBarName) 
		iFabricType = 0;
	else
		iFabricType = 1;
	m_nType = iFabricType;
	Init_WeldMeshComboBox(iFabricType);

	m_strMeshName = FindMeshName(m_strVBarName, m_dVBarSpace, m_strHBarName, m_dHBarSpace);
	int Index=0;
	if(m_strMeshName != _T(""))
	{
		Index = m_Fabric.FindStringExact(-1,m_strMeshName);
		m_Fabric.SetCurSel(Index);
	}

	InitGrid();

	// Initialize Data.
	UpdateData(FALSE);

	return TRUE;
}

void CDgnConWeldMeshDlg::InitGrid()
{
	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);

	m_wndGrid.GetParam()->EnableUndo(FALSE);

	m_wndGrid.SetRowCount(3);
	m_wndGrid.SetColCount(3);

	m_wndGrid.SetRowHeight(0,3, globalUtils.ScaleByDPI(22),NULL,GX_UPDATENOW);

	Write_GridTitle();

	m_wndGrid.GetParam()->EnableSelection(FALSE);
	m_wndGrid.GetParam()->EnableTrackRowHeight(FALSE);
	m_wndGrid.GetParam()->EnableTrackColWidth(FALSE);
	m_wndGrid.GetParam()->EnableMoveCols(FALSE);
	m_wndGrid.GetParam()->EnableMoveRows(FALSE);

	m_wndGrid.GetParam()->EnableUndo(TRUE);

}

void CDgnConWeldMeshDlg::Write_GridTitle()
{
	CString strAreaUnit=_T("");
	CString strLenUnit=_T("");

	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_MM)			{strLenUnit = D_UNITSYS_LENGTH_NAME_MM;	strAreaUnit.Format(_T("%s2/%s"), strLenUnit, _T("m"));}
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_CM)	{strLenUnit = D_UNITSYS_LENGTH_NAME_CM;	strAreaUnit.Format(_T("%s2/%s"), strLenUnit, _T("m"));}
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_M)	{strLenUnit = D_UNITSYS_LENGTH_NAME_M ;	strAreaUnit.Format(_T("%s2/%s"), strLenUnit, _T("m"));}
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_IN)	{strLenUnit = D_UNITSYS_LENGTH_NAME_IN;	strAreaUnit.Format(_T("%s2/%s"), strLenUnit, _T("ft"));}
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_FT)	{strLenUnit = D_UNITSYS_LENGTH_NAME_FT;	strAreaUnit.Format(_T("%s2/%s"), strLenUnit, _T("ft"));}
	else	ASSERT(0);

	Get_WeldMeshData(m_strMeshName, m_strVBarName, m_dVBarSpace, m_dVBarArea, m_strHBarName, m_dHBarSpace, m_dHBarArea);
	double dVBarDia = m_pDoc->m_pMatlDB->Get_RebarDia(m_strVBarName);
	double dHBarDia = m_pDoc->m_pMatlDB->Get_RebarDia(m_strHBarName);

	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(GetSysColor(COLOR_3DFACE));


	CRect rect;
	GetDlgItem(IDC_DGN_CON_WELDMESH_GRID)->GetWindowRect(&rect);
	int iWidth1 = int(rect.Width()*0.95*3/9.);
	int iWidth2 = int(rect.Width()*0.95*2/9.);
	int iWidth3 = int(rect.Width()*0.95*2/9.);
	int iWidth4 = int(rect.Width()*0.95*2/9.);

	//Title
	CString title=_T("");
	title=_T("Wire");
	style.SetValue(title);
	m_wndGrid.SetCoveredCellsRowCol(0,0,1,0);
	m_wndGrid.SetStyleRange(CGXRange(0,0),style);
	m_wndGrid.SetColWidth(0,0,iWidth1,NULL,GX_UPDATENOW);
	title.Format(_T("%s \n (%s)"), _T("Diameter"), strLenUnit);
	style.SetValue(title);
	m_wndGrid.SetCoveredCellsRowCol(0,1,1,1);
	m_wndGrid.SetStyleRange(CGXRange(0,1),style);
	m_wndGrid.SetColWidth(1,1,iWidth2,NULL,GX_UPDATENOW);
	title.Format(_T("%s \n (%s)"), _T("Spacing"), strLenUnit);
	style.SetValue(title);
	m_wndGrid.SetCoveredCellsRowCol(0,2,1,2);
	m_wndGrid.SetStyleRange(CGXRange(0,2),style);
	m_wndGrid.SetColWidth(2,2,iWidth3,NULL,GX_UPDATENOW);
	title.Format(_T("%s \n (%s)"), _T("Area"), strAreaUnit);
	style.SetValue(title);
	m_wndGrid.SetCoveredCellsRowCol(0,3,1,3);
	m_wndGrid.SetStyleRange(CGXRange(0,3),style);
	m_wndGrid.SetColWidth(3,3,iWidth4,NULL,GX_UPDATENOW);
	// Bar
	title.Format(_T("%s (%s)"), _T("Main"), m_strVBarName);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(2,0),style);
	title.Format(_T("%s (%s)"), _T("Cross"), m_strHBarName);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(3,0),style);

	//Data
	style.SetControl(GX_IDS_CTRL_STATIC);
	//Main Wire Data
	title.Format(_T("%.2f"), dVBarDia);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(2,1),style);
	title.Format(_T("%.2f"), m_dVBarSpace);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(2,2),style);
	title.Format(_T("%.2f"), m_dVBarArea);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(2,3),style);
	//Cross Wire Data
	title.Format(_T("%.2f"), dHBarDia);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(3,1),style);
	title.Format(_T("%.2f"), m_dHBarSpace);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(3,2),style);
	title.Format(_T("%.2f"), m_dHBarArea);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(3,3),style);
}

void CDgnConWeldMeshDlg::Init_WeldMeshComboBox(int iFabricType)
{
	// BSP Standard Fabric
	m_Fabric.ResetContent();

	if(iFabricType == 0) //A SQUARE FABRIC
	{
		//A SQUARE FABRIC
		m_Fabric.AddString(_T("A565"));
		m_Fabric.AddString(_T("A393"));
		m_Fabric.AddString(_T("A318"));
		m_Fabric.AddString(_T("A252"));
		m_Fabric.AddString(_T("A193"));
		m_Fabric.AddString(_T("A142"));
		m_Fabric.AddString(_T("A98"));
		m_Fabric.AddString(_T("A63"));
		//D SQUARE FABRIC
		m_Fabric.AddString(_T("DA785"));
		m_Fabric.AddString(_T("DA636"));
		m_Fabric.AddString(_T("DA503"));
		m_Fabric.AddString(_T("DA385"));
		m_Fabric.AddString(_T("DA283"));
		m_Fabric.AddString(_T("DA196"));
		m_Fabric.AddString(_T("DA126"));
	}
	else if(iFabricType == 1) //RECTANGULAR FABRIC
	{
		//B RECTANGULAR FABRIC
		m_Fabric.AddString(_T("B1131"));
		m_Fabric.AddString(_T("B785"));
		m_Fabric.AddString(_T("B636"));
		m_Fabric.AddString(_T("B503"));
		m_Fabric.AddString(_T("B385"));
		m_Fabric.AddString(_T("B283"));
		m_Fabric.AddString(_T("B196"));
		//C RECTANGULAR FABRIC
		m_Fabric.AddString(_T("C785"));
		m_Fabric.AddString(_T("C636"));
		m_Fabric.AddString(_T("C503"));
		m_Fabric.AddString(_T("C385"));
		m_Fabric.AddString(_T("C283"));
		m_Fabric.AddString(_T("C196"));
	}
	else ASSERT(0);
}

void CDgnConWeldMeshDlg::Get_WeldMeshData(CString strMeshName, CString& strVBarName, double& dVBarSpace, double& dVBarArea, CString& strHBarName, double& dHBarSpace, double& dHBarArea)
{
	// BSP Standard Fabric

	//A SQUARE FABRIC
	if(strMeshName == _T("A565"))       { strVBarName = _T("P12"); dVBarSpace  = 200.0; dVBarArea  = 565.0; strHBarName = _T("P12"); dHBarSpace  = 200.0; dHBarArea  = 565.0;}
	else if(strMeshName == _T("A393"))  { strVBarName = _T("P10"); dVBarSpace  = 200.0; dVBarArea  = 393.0; strHBarName = _T("P10"); dHBarSpace  = 200.0; dHBarArea  = 393.0;}
	else if(strMeshName == _T("A318"))  { strVBarName = _T("P9");  dVBarSpace  = 200.0; dVBarArea  = 318.0; strHBarName = _T("P9");  dHBarSpace  = 200.0; dHBarArea  = 318.0;}
	else if(strMeshName == _T("A252"))  { strVBarName = _T("P8");  dVBarSpace  = 200.0; dVBarArea  = 252.0; strHBarName = _T("P8");  dHBarSpace  = 200.0; dHBarArea  = 252.0;}
	else if(strMeshName == _T("A193"))  { strVBarName = _T("P7");  dVBarSpace  = 200.0; dVBarArea  = 193.0; strHBarName = _T("P7");  dHBarSpace  = 200.0; dHBarArea  = 193.0;}
	else if(strMeshName == _T("A142"))  { strVBarName = _T("P6");  dVBarSpace  = 200.0; dVBarArea  = 142.0; strHBarName = _T("P6");  dHBarSpace  = 200.0; dHBarArea  = 142.0;}
	else if(strMeshName == _T("A98"))   { strVBarName = _T("P5");  dVBarSpace  = 200.0; dVBarArea  =  98.0; strHBarName = _T("P5");  dHBarSpace  = 200.0; dHBarArea  =  98.0;}
	else if(strMeshName == _T("A63"))   { strVBarName = _T("P4");  dVBarSpace  = 200.0; dVBarArea  =  63.0; strHBarName = _T("P4");  dHBarSpace  = 200.0; dHBarArea  =  63.0;}
	//B RECTANGULAR FABRIC
	else if(strMeshName == _T("B1131")) { strVBarName = _T("P12"); dVBarSpace  = 100.0; dVBarArea  = 1131.0; strHBarName = _T("P8");  dHBarSpace  = 200.0; dHBarArea  = 252.0;}
	else if(strMeshName == _T("B785"))  { strVBarName = _T("P10"); dVBarSpace  = 100.0; dVBarArea  = 785.0; strHBarName = _T("P8");  dHBarSpace  = 200.0; dHBarArea  = 252.0;}
	else if(strMeshName == _T("B636"))  { strVBarName = _T("P9");  dVBarSpace  = 100.0; dVBarArea  = 636.0; strHBarName = _T("P8");  dHBarSpace  = 200.0; dHBarArea  = 252.0;}
	else if(strMeshName == _T("B503"))  { strVBarName = _T("P8");  dVBarSpace  = 100.0; dVBarArea  = 503.0; strHBarName = _T("P8");  dHBarSpace  = 200.0; dHBarArea  = 252.0;}
	else if(strMeshName == _T("B385"))  { strVBarName = _T("P7");  dVBarSpace  = 100.0; dVBarArea  = 385.0; strHBarName = _T("P7");  dHBarSpace  = 200.0; dHBarArea  = 193.0;}
	else if(strMeshName == _T("B283"))  { strVBarName = _T("P6");  dVBarSpace  = 100.0; dVBarArea  = 283.0; strHBarName = _T("P7");  dHBarSpace  = 200.0; dHBarArea  = 193.0;}
	else if(strMeshName == _T("B196"))  { strVBarName = _T("P5");  dVBarSpace  = 100.0; dVBarArea  = 196.0; strHBarName = _T("P7");  dHBarSpace  = 200.0; dHBarArea  = 193.0;}
	//C RECTANGULAR FABRIC
	else if(strMeshName == _T("C785"))  { strVBarName = _T("P10"); dVBarSpace  = 100.0; dVBarArea  = 785.0; strHBarName = _T("P6");  dHBarSpace  = 400.0; dHBarArea  = 71.0;}
	else if(strMeshName == _T("C636"))  { strVBarName = _T("P9");  dVBarSpace  = 100.0; dVBarArea  = 636.0; strHBarName = _T("P6");  dHBarSpace  = 400.0; dHBarArea  = 71.0;}
	else if(strMeshName == _T("C503"))  { strVBarName = _T("P8");  dVBarSpace  = 100.0; dVBarArea  = 503.0; strHBarName = _T("P5");  dHBarSpace  = 400.0; dHBarArea  = 49.0;}
	else if(strMeshName == _T("C385"))  { strVBarName = _T("P7");  dVBarSpace  = 100.0; dVBarArea  = 385.0; strHBarName = _T("P5");  dHBarSpace  = 400.0; dHBarArea  = 49.0;}
	else if(strMeshName == _T("C283"))  { strVBarName = _T("P6");  dVBarSpace  = 100.0; dVBarArea  = 283.0; strHBarName = _T("P5");  dHBarSpace  = 400.0; dHBarArea  = 49.0;}
	else if(strMeshName == _T("C196"))  { strVBarName = _T("P5");  dVBarSpace  = 100.0; dVBarArea  = 196.0; strHBarName = _T("P5");  dHBarSpace  = 400.0; dHBarArea  = 49.0;}
	//D SQUARE FABRIC
	else if(strMeshName == _T("DA785")) { strVBarName = _T("P10"); dVBarSpace  = 100.0; dVBarArea  = 785.0; strHBarName = _T("P10"); dHBarSpace  = 100.0; dHBarArea  = 785.0;}
	else if(strMeshName == _T("DA636")) { strVBarName = _T("P9");  dVBarSpace  = 100.0; dVBarArea  = 636.0; strHBarName = _T("P9");  dHBarSpace  = 100.0; dHBarArea  = 636.0;}
	else if(strMeshName == _T("DA503")) { strVBarName = _T("P8");  dVBarSpace  = 100.0; dVBarArea  = 503.0; strHBarName = _T("P8");  dHBarSpace  = 100.0; dHBarArea  = 503.0;}
	else if(strMeshName == _T("DA385")) { strVBarName = _T("P7");  dVBarSpace  = 100.0; dVBarArea  = 385.0; strHBarName = _T("P7");  dHBarSpace  = 100.0; dHBarArea  = 385.0;}
	else if(strMeshName == _T("DA283")) { strVBarName = _T("P6");  dVBarSpace  = 100.0; dVBarArea  = 283.0; strHBarName = _T("P6");  dHBarSpace  = 100.0; dHBarArea  = 283.0;}
	else if(strMeshName == _T("DA196")) { strVBarName = _T("P5");  dVBarSpace  = 100.0; dVBarArea  = 196.0; strHBarName = _T("P5");  dHBarSpace  = 100.0; dHBarArea  = 196.0;}
	else if(strMeshName == _T("DA126")) { strVBarName = _T("P4");  dVBarSpace  = 100.0; dVBarArea  = 126.0; strHBarName = _T("P4");  dHBarSpace  = 100.0; dHBarArea  = 126.0;}

	double dAreaFactor=0.0; // Area per m or ft
	double dLenFactor=0.0;	// Ratio per mm.
	Get_LengthAreaFactor(dLenFactor, dAreaFactor);

	dVBarSpace = dVBarSpace/dLenFactor;
	dVBarArea = dVBarArea/dAreaFactor;
	dHBarSpace = dHBarSpace/dLenFactor;
	dHBarArea = dHBarArea/dAreaFactor;
}

CString CDgnConWeldMeshDlg::FindMeshName(CString strVBarName, double dVBarSpace, CString strHBarName, double dHBarSpace)
{
	CString strMeshName = _T("");

	double dAreaFactor=0.0; // Area per m or ft
	double dLenFactor=0.0;	// Ratio per mm.
	Get_LengthAreaFactor(dLenFactor, dAreaFactor);

	dVBarSpace = dVBarSpace*dLenFactor;
	dHBarSpace = dHBarSpace*dLenFactor;

	// BSP Standard Fabric

	if     (strVBarName == _T("P12")&& fabs(dVBarSpace-200.0) < 1.0E-07 && strHBarName == _T("P12")&& fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("A565");
	else if(strVBarName == _T("P10")&& fabs(dVBarSpace-200.0) < 1.0E-07 && strHBarName == _T("P10")&& fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("A393");
	else if(strVBarName == _T("P9") && fabs(dVBarSpace-200.0) < 1.0E-07 && strHBarName == _T("P9")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("A318");
	else if(strVBarName == _T("P8") && fabs(dVBarSpace-200.0) < 1.0E-07 && strHBarName == _T("P8")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("A252");
	else if(strVBarName == _T("P7") && fabs(dVBarSpace-200.0) < 1.0E-07 && strHBarName == _T("P7")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("A193");
	else if(strVBarName == _T("P6") && fabs(dVBarSpace-200.0) < 1.0E-07 && strHBarName == _T("P6")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("A142");
	else if(strVBarName == _T("P5") && fabs(dVBarSpace-200.0) < 1.0E-07 && strHBarName == _T("P5")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("A98"); 
	else if(strVBarName == _T("P4") && fabs(dVBarSpace-200.0) < 1.0E-07 && strHBarName == _T("P4")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("A63"); 
	else if(strVBarName == _T("P12")&& fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P8")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("B1131");
	else if(strVBarName == _T("P10")&& fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P8")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("B785");
	else if(strVBarName == _T("P9") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P8")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("B636");
	else if(strVBarName == _T("P8") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P8")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("B503");
	else if(strVBarName == _T("P7") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P7")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("B385");
	else if(strVBarName == _T("P6") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P7")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("B283");
	else if(strVBarName == _T("P5") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P7")&&  fabs(dHBarSpace-200.0) < 1.0E-07)  strMeshName = _T("B196");
	else if(strVBarName == _T("P10")&& fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P6")&&  fabs(dHBarSpace-400.0) < 1.0E-07)  strMeshName = _T("C785");
	else if(strVBarName == _T("P9") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P6")&&  fabs(dHBarSpace-400.0) < 1.0E-07)  strMeshName = _T("C636");
	else if(strVBarName == _T("P8") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P5")&&  fabs(dHBarSpace-400.0) < 1.0E-07)  strMeshName = _T("C503");
	else if(strVBarName == _T("P7") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P5")&&  fabs(dHBarSpace-400.0) < 1.0E-07)  strMeshName = _T("C385");
	else if(strVBarName == _T("P6") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P5")&&  fabs(dHBarSpace-400.0) < 1.0E-07)  strMeshName = _T("C283");
	else if(strVBarName == _T("P5") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P5")&&  fabs(dHBarSpace-400.0) < 1.0E-07)  strMeshName = _T("C196");
	else if(strVBarName == _T("P10")&& fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P10")&& fabs(dHBarSpace-100.0) < 1.0E-07)  strMeshName = _T("DA785");
	else if(strVBarName == _T("P9") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P9")&&  fabs(dHBarSpace-100.0) < 1.0E-07)  strMeshName = _T("DA636");
	else if(strVBarName == _T("P8") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P8")&&  fabs(dHBarSpace-100.0) < 1.0E-07)  strMeshName = _T("DA503");
	else if(strVBarName == _T("P7") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P7")&&  fabs(dHBarSpace-100.0) < 1.0E-07)  strMeshName = _T("DA385");
	else if(strVBarName == _T("P6") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P6")&&  fabs(dHBarSpace-100.0) < 1.0E-07)  strMeshName = _T("DA283");
	else if(strVBarName == _T("P5") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P5")&&  fabs(dHBarSpace-100.0) < 1.0E-07)  strMeshName = _T("DA196");
	else if(strVBarName == _T("P4") && fabs(dVBarSpace-100.0) < 1.0E-07 && strHBarName == _T("P4")&&  fabs(dHBarSpace-100.0) < 1.0E-07)  strMeshName = _T("DA126");

	return strMeshName;
}

void CDgnConWeldMeshDlg::Get_LengthAreaFactor(double& dLenFactor, double& dAreaFactor)
{
	//dAreaFactor : Ratio per Unit2/m or Unit/ft
	//dLenFactor  : Ratio per mm.
	// Check Current Unit.
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_MM)			{dLenFactor = 1.0;    dAreaFactor = 1.0;}
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_CM)	{dLenFactor = 10.0;   dAreaFactor = 100;}
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_M)	{dLenFactor = 1000.0; dAreaFactor = 1000000.0;}
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_IN)	{dLenFactor = 25.4;   dAreaFactor = pow(25.4,2)/0.3048;}
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_FT)	{dLenFactor = 304.8;  dAreaFactor = pow(304.8,2)/0.3048;}
	else	ASSERT(0);
}