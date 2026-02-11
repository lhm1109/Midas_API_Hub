// IehpYieldPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IehpYieldPropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "IehpYieldPropGrid.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"


/////////////////////////////////////////////////////////////////////////////
// CIehpYieldPropDlg dialog


CIehpYieldPropDlg::CIehpYieldPropDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIehpYieldPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIehpYieldPropDlg)
	m_nLinear = -1;
	m_nYldUser = 2;
	m_bConsiderPM = FALSE;
	m_strBetaYplus = _T("");
	m_strBetaYmnus = _T("");
	m_strBetaZmnus = _T("");
	m_strBetaZplus = _T("");
	m_strGamma = _T("");
	//}}AFX_DATA_INIT
/*
	m_Data.Initialize();
	m_strName.Empty();

	m_wndGrid = NULL;
	m_wndGrid = new CIehpYieldPropGrid(this);

	m_arYldUser.RemoveAll();
	m_arYldUser.Add(IDC_IEHP_YLD_BETA_YPLUS);
	m_arYldUser.Add(IDC_IEHP_YLD_BETA_YMNUS);
	m_arYldUser.Add(IDC_IEHP_YLD_BETA_GAMM);
	m_arYldUser.Add(IDC_IEHP_YLD_BETA_ZPLUS);
	m_arYldUser.Add(IDC_IEHP_YLD_BETA_ZMNUS);
*/
}

CIehpYieldPropDlg::~CIehpYieldPropDlg()
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
}

void CIehpYieldPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpYieldPropDlg)
	DDX_Control(pDX, IDC_IEHP_ALPHA_EDT, m_edtAlpha);
	DDX_Control(pDX, IDC_IEHP_AXIAL_STATIC, m_untAxial);
	DDX_Control(pDX, IDC_IEHP_AXIAL_EDIT, m_edtAxial);
	DDX_Radio(pDX, IDC_IEHP_LINEAR_RDO, m_nLinear);
	DDX_Radio(pDX, IDC_IEHP_YLD_CALC, m_nYldUser);
	DDX_Check(pDX, IDC_IEHP_YLD_PM_CHK, m_bConsiderPM);
	DDX_Text(pDX, IDC_IEHP_YLD_BETA_YPLUS, m_strBetaYplus);
	DDX_Text(pDX, IDC_IEHP_YLD_BETA_YMNUS, m_strBetaYmnus);
	DDX_Text(pDX, IDC_IEHP_YLD_BETA_ZMNUS, m_strBetaZmnus);
	DDX_Text(pDX, IDC_IEHP_YLD_BETA_ZPLUS, m_strBetaZplus);
	DDX_Text(pDX, IDC_IEHP_YLD_BETA_GAMM, m_strGamma);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIehpYieldPropDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIehpYieldPropDlg)
	ON_BN_CLICKED(IDC_IEHP_LINEAR_RDO, OnIehpLinearRdo)
	ON_BN_CLICKED(IDC_IEHP_YLD_CALC, OnIehpYldCalc)
	ON_BN_CLICKED(IDC_IEHP_ELIPSE_RDO, OnIehpLinearRdo)
	ON_BN_CLICKED(IDC_IEHP_USER_RDO, OnIehpLinearRdo)
	ON_BN_CLICKED(IDC_IEHP_YLD_USER_SPEC, OnIehpYldCalc)
	ON_BN_CLICKED(IDC_IEHP_YLD_USER_TABL, OnIehpYldCalc)
	ON_EN_CHANGE(IDC_IEHP_YLD_BETA_GAMM, OnChangeUserDirSpec)
	ON_EN_CHANGE(IDC_IEHP_YLD_BETA_YMNUS, OnChangeUserDirSpecY)
	ON_EN_CHANGE(IDC_IEHP_YLD_BETA_YPLUS, OnChangeUserDirSpecY)
	ON_EN_CHANGE(IDC_IEHP_YLD_BETA_ZMNUS, OnChangeUserDirSpecZ)
	ON_EN_CHANGE(IDC_IEHP_YLD_BETA_ZPLUS, OnChangeUserDirSpecZ)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_YIELD_INPUT_TYPE_USER_RDO, &CIehpYieldPropDlg::OnBnClickedYieldInputTypeUserRdo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpYieldPropDlg Data & Window handlers
void CIehpYieldPropDlg::Data2Dlg()
{
	/*
	m_nLinear = m_Data.nInterpolationType;
	CString str; str.Empty();
	str.Format(_T("%g"), m_Data.dAlpha);
	m_edtAlpha.SetWindowText(str);
	m_edtAxial.SetEditUnit(m_Data.dPybal);

	// 그리드 데이터 세팅
	m_wndGrid->SetDataToGrid(m_Data.dP, m_Data.dMy, m_Data.dMz);

	str.Format(_T("%g"), m_Data.dBetay[0]);
	m_strBetaYplus = str;
	str.Format(_T("%g"), m_Data.dBetay[1]);
	m_strBetaYmnus = str;
	str.Format(_T("%g"), m_Data.dBetaz[0]);
	m_strBetaZplus = str;
	str.Format(_T("%g"), m_Data.dBetaz[1]);
	m_strBetaZmnus = str;
	str.Format(_T("%g"), m_Data.dGamma);
	m_strGamma = str;

	m_nYldUser = m_Data.nCalcMethod;
	m_bConsiderPM = m_Data.bConsiderPMInteraction;

	for(int i=0; i<11; i++) m_dR2[i] = sin(3.141592*i/22);
	
	UpdateData(FALSE);
	*/
}
BOOL CIehpYieldPropDlg::Dlg2Data()
{
	/*
	UpdateData();
	m_Data.nInterpolationType = m_nLinear;
	CString str; str.Empty();
	m_edtAlpha.GetWindowText(str);
	m_Data.dAlpha = _tstof(str);
	m_Data.dPybal = m_edtAxial.GetEditValue();

	m_Data.nCalcMethod = m_nYldUser;
	m_Data.bConsiderPMInteraction = m_bConsiderPM;
	
	// 그리드 입력값 가져오기 
	m_wndGrid->GetGridToData(m_Data.dP, m_Data.dMy, m_Data.dMz);

	m_Data.dBetay[0] = _tstof(m_strBetaYplus);
	m_Data.dBetay[1] = _tstof(m_strBetaYmnus);
	m_Data.dBetaz[0] = _tstof(m_strBetaZplus);
	m_Data.dBetaz[1] = _tstof(m_strBetaZmnus);
	m_Data.dGamma    = _tstof(m_strGamma);

	if(!CheckDataCtrl()) return FALSE;
	*/
	return TRUE;
}
BOOL CIehpYieldPropDlg::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar)
{
	GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_NAME_VAR_ERROR), lpszData, lpszName, lpszVar);
	return FALSE;
}
#define CheckError(x) CheckErrorNameDataVar(_LS(IDS_DB_DT_IEHP), m_strName, x)
BOOL CIehpYieldPropDlg::CheckDataCtrl()
{
	/*
	UpdateData(TRUE);
	CString str; str.Empty();
	m_edtAlpha.GetWindowText(str);
	double dV = _tstof(str);
	if(dV<1. || dV>2.) return CheckError(_LS(IDS_DB_ALPHA));

	//if(m_nYldUser && m_edtAxial.GetEditValue()<=0.) return CheckError(_T("Pb"));
	
	if(m_nYldUser==1)
	{
		for(int i=1; i<5; i++)
		{
			if(m_Data.dP[i] <= 0     || m_Data.dP[i] >= 1)     return CheckError(_T("P/Pb"));
			if(m_Data.dMy[0][i] <= 0 || m_Data.dMy[0][i] >= 1) return CheckError(_T("M/Mmax in y(+)"));
			if(m_Data.dMz[0][i] <= 0 || m_Data.dMz[0][i] >= 1) return CheckError(_T("M/Mmax in z(+)"));
		}
		for(i=6; i<10; i++)
		{
			if(m_Data.dP[i] <= -1    || m_Data.dP[i] >= 0)     return CheckError(_T("P/Pb"));
			if(m_Data.dMy[0][i] <= 0 || m_Data.dMy[0][i] >= 1) return CheckError(_T("M/Mmax in y(+)"));
			if(m_Data.dMz[0][i] <= 0 || m_Data.dMz[0][i] >= 1) return CheckError(_T("M/Mmax in z(+)"));
		}
		for(i = 0; i < 10; i++)
			if(m_Data.dP[i] <= m_Data.dP[i+1]) return CheckError(_T("P/Pb"));
	}
	else if(m_nYldUser==2)
	{
		if(m_Data.dBetay[0]<1. || m_Data.dBetay[0]>3.) return CheckError(_T("Beta y(+)"));
		if(m_Data.dBetay[1]<1. || m_Data.dBetay[1]>3.) return CheckError(_T("Beta y(-)"));
		if(m_Data.dBetaz[0]<1. || m_Data.dBetaz[0]>3.) return CheckError(_T("Beta z(+)"));
		if(m_Data.dBetaz[1]<1. || m_Data.dBetaz[1]>3.) return CheckError(_T("Beta z(-)"));
		if(m_Data.dGamma<1.    || m_Data.dGamma>3.)    return CheckError(_T("Gamma"));
	}
	return TRUE;
#undef CheckError
	*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIehpYieldPropDlg message handlers

BOOL CIehpYieldPropDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
/*	
	// TODO: Add extra initialization here
	m_wndGrid->SubclassDlgItem(IDC_IEHP_TABLE, this);
	m_wndGrid->Initialize();

	m_edtAxial.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dPybal);
	m_untAxial.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dPybal);

	Data2Dlg();
	OnIehpLinearRdo();
	OnIehpYldCalc();

////////////////////////
	InitChartY();
	UpdateChartY();

	InitChartZ();
	UpdateChartZ();

	GetDlgItem(IDC_IEHP_YLD_CALC)->EnableWindow(FALSE);
	GetDlgItem(IDC_IEHP_YLD_USER_TABL)->EnableWindow(FALSE);
	*/
////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
////////////////////////////////////////////////////////////////////////////
void CIehpYieldPropDlg::InitChartY()
{
	/*
	m_GraphViewY.SetDragable(FALSE); // set data-drag unable

	SRGraphBackground	*pB=new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_GraphY.AddComponent((SRGraphComponent *) pB);
	
	SRGraphDisplay *pDisplay=new SRGraphDisplay;	
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX);
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	pDisplay->SetSizeDefaults();
	pDisplay->SetFontSize(7);
	pDisplay->SetRect(0,0,100,100);
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(TRUE);

	pDisplay->GetStyle()->SetLogX(FALSE);
	pDisplay->GetStyle()->SetLog(FALSE);

	m_GraphY.AddComponent(pDisplay);
	
	CWnd* pWnd = GetDlgItem(IDC_STATIC_Y);

	CRect rc;
	pWnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);

	rc.left   += 2;
	rc.top    += 2;
	rc.right  -= 2;
	rc.bottom -= 2;

	m_GraphViewY.Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rc, this, IDC_STATIC_Y);
	m_GraphViewY.m_pGraph = &m_GraphY;

	m_GraphViewY.Invalidate();
	*/

}
void CIehpYieldPropDlg::InitChartZ()
{
	/*
	m_GraphViewZ.SetDragable(FALSE); // set data-drag unable

	SRGraphBackground	*pB=new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_GraphZ.AddComponent((SRGraphComponent *) pB);
	
	SRGraphDisplay *pDisplay=new SRGraphDisplay;	
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX);
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	pDisplay->SetSizeDefaults();
	pDisplay->SetFontSize(7);
	pDisplay->SetRect(0,0,100,100);
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(TRUE);

	pDisplay->GetStyle()->SetLogX(FALSE);
	pDisplay->GetStyle()->SetLog(FALSE);

	m_GraphZ.AddComponent(pDisplay);
	
	CWnd* pWnd = GetDlgItem(IDC_STATIC_Z);

	CRect rc;
	pWnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);

	rc.left   += 2;
	rc.top    += 2;
	rc.right  -= 2;
	rc.bottom -= 2;

	m_GraphViewZ.Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rc, this, IDC_STATIC_Z);
	m_GraphViewZ.m_pGraph = &m_GraphZ;
	
	m_GraphViewZ.Invalidate();
	*/
}
void CIehpYieldPropDlg::SetValueY(int nIndex, int nGroup, double x,double y)
{
	/*
	m_GraphY.SetValue(nIndex, nGroup*2, x);
	m_GraphY.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_GraphY.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_GraphY.SetValue(nIndex, nGroup*2+1, y);
	*/
}
void CIehpYieldPropDlg::SetValueZ(int nIndex, int nGroup, double x,double y)
{
	/*
	m_GraphZ.SetValue(nIndex, nGroup*2, x);
	m_GraphZ.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_GraphZ.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_GraphZ.SetValue(nIndex, nGroup*2+1, y);
	*/
}
void CIehpYieldPropDlg::UpdateChartY()
{
	/*
	m_GraphY.KillData();	
	double x,y;

	if(m_nYldUser==1) // USER : Table
	{
		m_wndGrid->GetGridToData(m_Data.dP, m_Data.dMy, m_Data.dMz);
		for(int i=0; i<11; i++)
		{
		  x = m_Data.dMy[0][i];
		  y = m_Data.dP[i];
		
			SetValueY(i,0,x,y);
		}
	}
	else if(m_nYldUser==2) // USER : Direct Specification
	{
		UpdateData();
		
		if(m_strGamma.IsEmpty() || m_strBetaYplus.IsEmpty() || m_strBetaYmnus.IsEmpty()) return;
		double Gamma = _tstof(m_strGamma);
		double BetaYP = _tstof(m_strBetaYplus);
	  double BetaYM = _tstof(m_strBetaYmnus);

		if(BetaYP<1. || BetaYM<1.) return;

		for(int i=0; i<11; i++)
		{
			x = m_dR2[i];
			y = pow((1. - pow(x, Gamma)), 1./BetaYP);
			SetValueY(i,0,x,y);
		}
		x=1; y=0; SetValueY(11,0,x,y);
		for(i=0; i<11; i++)
		{
			x = m_dR2[10-i];
			y = -pow((1. - pow(x, Gamma)), 1./BetaYM);
			SetValueY(i+12,0,x,y);
		}
	}
	m_GraphViewY.Invalidate();
	*/
}
void CIehpYieldPropDlg::UpdateChartZ()
{
	/*
	m_GraphZ.KillData();	
	double x,y;

	if(m_nYldUser==1) // Table
	{
		m_wndGrid->GetGridToData(m_Data.dP, m_Data.dMy, m_Data.dMz);
		for(int i=0; i<11; i++)
		{
		  x = m_Data.dMz[0][i];
		  y = m_Data.dP[i];

			SetValueZ(i,0,x,y);
		}
	}
	else if(m_nYldUser==2) // Direct Specification
	{
		UpdateData();
		if(m_strGamma.IsEmpty() || m_strBetaZplus.IsEmpty() || m_strBetaZmnus.IsEmpty()) return;
		double Gamma = _tstof(m_strGamma);
		double BetaZP = _tstof(m_strBetaZplus); 
	  double BetaZM = _tstof(m_strBetaZmnus);

		if(BetaZP<1. || BetaZM<1.) return;

		for(int i=0; i<11; i++)
		{
			x = m_dR2[i];
			y = pow((1. - pow(x, Gamma)), 1./BetaZP);
			SetValueZ(i,0,x,y);
		}
		x=1; y=0; SetValueZ(11,0,x,y);
		for(i=0; i<11; i++)
		{
			x = m_dR2[10-i];
			y = -pow((1. - pow(x, Gamma)), 1./BetaZM);
			SetValueZ(i+12,0,x,y);
		}
	}
	m_GraphViewZ.Invalidate();
	*/
}
/////////////////////////////////////////////////////////////////////////////
void CIehpYieldPropDlg::OnIehpLinearRdo() 
{
	/*
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_edtAlpha.SetReadOnly(TRUE);
	if     (m_nLinear == 0) m_edtAlpha.SetWindowText(_T("1"));// Linear
	else if(m_nLinear == 1) m_edtAlpha.SetWindowText(_T("2"));// Elipse
	else m_edtAlpha.SetReadOnly(FALSE);
	*/
}

void CIehpYieldPropDlg::OnOK() 
{
	// TODO: Add extra validation here
	//if(!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CIehpYieldPropDlg::OnIehpYldCalc() 
{
	/*
	UpdateData();
	if(m_nYldUser==0)
	{
		GetDlgItem(IDC_IEHP_AXIAL_EDIT)->EnableWindow(FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arYldUser, FALSE);
		m_wndGrid->SetEnabled(FALSE);
	}
	else if(m_nYldUser==1)
	{
		GetDlgItem(IDC_IEHP_AXIAL_EDIT)->EnableWindow(TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_arYldUser, FALSE);
		m_wndGrid->SetEnabled(TRUE);
	}
	else if(m_nYldUser==2)
	{
		GetDlgItem(IDC_IEHP_AXIAL_EDIT)->EnableWindow(TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_arYldUser, TRUE);
		m_wndGrid->SetEnabled(FALSE);
	}
	else ASSERT(0);
	*/
}

void CIehpYieldPropDlg::OnChangeUserDirSpec() 
{
	//UpdateChartY();
	//UpdateChartZ();
}

void CIehpYieldPropDlg::OnChangeUserDirSpecY() 
{
	//UpdateChartY();
}

void CIehpYieldPropDlg::OnChangeUserDirSpecZ() 
{
	//UpdateChartZ();
}


void CIehpYieldPropDlg::OnBnClickedYieldInputTypeUserRdo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
