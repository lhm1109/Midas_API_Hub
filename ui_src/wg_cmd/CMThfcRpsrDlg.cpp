// CMThfcRpsrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThfcRpsrDlg.h"

#include "..\wg_base\wg_base_FFT.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "CMThfcRpsrProgDlg.h"
#include "MyGraphDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CXCLR_BACKGRND RGB(255,255,255)
/////////////////////////////////////////////////////////////////////////////
// CCMThfcRpsrDlg dialog

CCMThfcRpsrDlg::CCMThfcRpsrDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMThfcRpsrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMThfcRpsrDlg)
	m_bLogX = TRUE;
	m_bLogY = TRUE;
	m_fAmpScale = 1.0f;
	m_fTimeScale = 1.0f;
	m_fFrom = 0.01f;
	m_fTo = 10.0f;
	m_nStep = 200;
	//}}AFX_DATA_INIT
	m_fDamping[0] = 0.0f;
	m_fDamping[1] = 0.01f;
	m_fDamping[2] = 0.02f;
	m_fDamping[3] = 0.05f;
	m_fDamping[4] = 0.10f;
	m_fDamping[5] = 0.20f;
}


void CCMThfcRpsrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThfcRpsrDlg)
	DDX_Control(pDX, IDC_CMD_SPECTRUM_TYPE, m_cmbSpecType);
	DDX_Check(pDX, IDC_ETC_SPFC_CHART_XLog, m_bLogX);
	DDX_Check(pDX, IDC_ETC_SPFC_CHART_YLog, m_bLogY);
	DDX_Text(pDX, IDC_CMD_AMP_SCALE_EDT, m_fAmpScale);
	DDX_Text(pDX, IDC_CMD_TIME_SCALE_EDT, m_fTimeScale);
	DDX_Text(pDX, IDC_CMD_FROM_EDT, m_fFrom);
	DDX_Text(pDX, IDC_CMD_TO_EDT,   m_fTo  );
	DDX_Text(pDX, IDC_CMD_STEP_EDT, m_nStep);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_CMD_DAMP_EDT1, m_fDamping[0]);
	DDX_Text(pDX, IDC_CMD_DAMP_EDT2, m_fDamping[1]);
	DDX_Text(pDX, IDC_CMD_DAMP_EDT3, m_fDamping[2]);
	DDX_Text(pDX, IDC_CMD_DAMP_EDT4, m_fDamping[3]);
	DDX_Text(pDX, IDC_CMD_DAMP_EDT5, m_fDamping[4]);
	DDX_Text(pDX, IDC_CMD_DAMP_EDT6, m_fDamping[5]);

	m_ctlDamping.Add(IDC_CMD_DAMP_EDT1);
	m_ctlDamping.Add(IDC_CMD_DAMP_EDT2);
	m_ctlDamping.Add(IDC_CMD_DAMP_EDT3);
	m_ctlDamping.Add(IDC_CMD_DAMP_EDT4);
	m_ctlDamping.Add(IDC_CMD_DAMP_EDT5);
	m_ctlDamping.Add(IDC_CMD_DAMP_EDT6);
}


BEGIN_MESSAGE_MAP(CCMThfcRpsrDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMThfcRpsrDlg)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	ON_BN_CLICKED(IDC_CMD_DAMP_ADD_BTN, OnCmdDampAddBtn)
	ON_BN_CLICKED(IDC_CMD_GENERATE_BTN, OnCmdGenerateBtn)
	ON_BN_CLICKED(IDC_ETC_SPFC_CHART_XLog, OnETCSPFCCHARTXLog)
	ON_BN_CLICKED(IDC_ETC_SPFC_CHART_YLog, OnETCSPFCCHARTYLog)
	ON_CBN_SELCHANGE(IDC_CMD_SPECTRUM_TYPE, OnSelchangeCmdSpectrumType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThfcRpsrDlg message handlers

BOOL CCMThfcRpsrDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_nNumOfDamping = 1;

	MakeCombo();
	InitChart();

	for (int i=1; i<6; i++)
		GetDlgItem(m_ctlDamping[i])->EnableWindow(FALSE);

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThfcRpsrDlg::MakeCombo()
{
	m_cmbSpecType.ResetContent();
	m_cmbSpecType.AddString(_LS(IDS_CMD_PSEUDO_ACCEL));
	m_cmbSpecType.AddString(_LS(IDS_CMD_PSEUDO_VELOCITY));
	m_cmbSpecType.AddString(_LS(IDS_CMD_ABS_ACCEL));
	m_cmbSpecType.AddString(_LS(IDS_CMD_REL_VELOCITY));
	m_cmbSpecType.AddString(_LS(IDS_CMD_REL_DISPLACEMENT));
	m_cmbSpecType.AddString(_LS(IDS_CMD_DVA_TRIPARTITE));

	// Extent Size
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_cmbSpecType.GetDC();
	for (int i=0; i<m_cmbSpecType.GetCount(); i++)
	{
		m_cmbSpecType.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_cmbSpecType.ReleaseDC(pDC);
	m_cmbSpecType.SetHorizontalExtent(dx);

	m_cmbSpecType.SetCurSel(0);
}

void CCMThfcRpsrDlg::InitChart()
{
	m_GraphView.SetDragable(FALSE); // set data-drag unable
	
	// Add Background component
	//
	SRGraphBackground	*pB=new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent((SRGraphComponent *) pB);
	
	// Add Display Component
	//
	MyGraphDisplay *pDisplay=new MyGraphDisplay;	
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX);
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	//pDisplay->SetSizeDefaults();
	//pDisplay->SetRect(0.2,0,100,97.0);
	pDisplay->SetRect(0.2,0,100,92);
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(FALSE);
	
	// Logarithmic Axis Scaling
	//
	pDisplay->GetStyle()->SetLogX(m_bLogX);
	pDisplay->GetStyle()->SetLog(m_bLogY);
	m_Graph.AddComponent(pDisplay);

	// Add Label Component for X-Axis
	//
	SRGraphLabel	*pLabel=new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::TopCenter);
	//pLabel->SetTextPosition(50.0, 93.5);
	pLabel->SetTextPosition(50.0, 88.0);
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Normal);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	m_Graph.AddComponent(pLabel);
	SetAxisLabelText(0,_LS(IDS_WG_CMD_PEROPD_TN));
	
	// Add Label Component for Y-Axis
	//
	pLabel=new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::MidCenter);
	pLabel->SetTextPosition(1.5, 50.0);
	pLabel->SetFontSize(10);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Upward);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent(pLabel);
	SetAxisLabelText(1,_LS(IDS_CMD_PSEUDO_ABS_ACCEL));
	
	/////////////////////////////////////////////////////////////
	// Attatch graphview to dialog's placeholder and connect graph to graphview
	CWnd* wnd = GetDlgItem(IDC_CMD_PLACEHOLDER);
	CRect rc;

	wnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	rc.left+=2;
	rc.top+=2;
	rc.right -= 2;
	rc.bottom -= 2;

	m_GraphView.Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rc, this, IDC_CMD_PLACEHOLDER);
	m_GraphView.m_pGraph = &m_Graph;
	
	m_GraphView.Invalidate();
}
void CCMThfcRpsrDlg::OnBtnDyGen()
{
	UpdateData(TRUE);
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_EARTHQUAKE;  // Earthquake RS
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = m_nKey;
	
	CString csName; csName.Empty();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if (nCount == 0) csName = _LS(IDS_CMD_DYNA_RPT_EQRS_NAME);
	else csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_EQRS_NAME), nCount + 1);
	
	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if(dlg.DoModal() == IDOK)
	{
		UchtD.strName = csName;

		UchtD.bLogX = m_bLogX;
		UchtD.bLogY = m_bLogY;
	
		UchtD.Data.Eqrs.nSpectrumType = m_cmbSpecType.GetCurSel();
		UchtD.Data.Eqrs.dTimeScale = m_fTimeScale;
		UchtD.Data.Eqrs.dAmpliScale = m_fAmpScale;
	
		UchtD.Data.Eqrs.aOutPeriod[0] = m_fFrom; 
		UchtD.Data.Eqrs.aOutPeriod[1] = m_fTo;   
		UchtD.Data.Eqrs.aOutPeriod[2] = m_nStep;
	
		UchtD.Data.Eqrs.dGrav = m_dGravity;
		UchtD.Data.Eqrs.nDataType = m_nDataType;
		UchtD.Data.Eqrs.nDampingNum = m_nNumOfDamping;
		for(int i = 0; i < m_nNumOfDamping; i++)
		{
			UchtD.Data.Eqrs.aDampRatio[i] = m_fDamping[i];
		}
	
		if(!pDoc->m_pDataCtrl->AddUcht(UchtD)) ASSERT(0);
	}
}
void CCMThfcRpsrDlg::SetEQData(CArray<T_THFC_BASE, T_THFC_BASE&>& arData, int nDataType, double dGravity, UINT Key)
{
	m_arData.RemoveAll();
	m_arData.Copy(arData);
	
	m_nNumOfData = m_arData.GetSize();  // 지진파의 데이터 개수
	
	m_nDataType = nDataType;
	m_dGravity = dGravity;
	m_nKey = Key;
}

void CCMThfcRpsrDlg::OnCmdDampAddBtn() 
{
	if(m_nNumOfDamping>=6)
	{	
		AfxMessageBox(_T("No more damping!"));
		return;
	}

	GetDlgItem(m_ctlDamping[m_nNumOfDamping])->EnableWindow(TRUE);
	m_nNumOfDamping++;
}

void CCMThfcRpsrDlg::OnXScale() 
{
	MyGraphDisplay	*pD=(MyGraphDisplay*) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLogX(m_bLogX);

	if (m_bLogX || m_bLogY) 
	{
		pD->GetStyle()->SetScaleLimit(TRUE);
		pD->SetMinRangeY(0.01);    // min range from 0 to 100
		pD->SetMinRangeX(0.01);    // min range from 0 to 100
	}
	else 	pD->GetStyle()->SetScaleLimit(FALSE);
	
	m_GraphView.Invalidate();
}

void CCMThfcRpsrDlg::OnCmdGenerateBtn() 
{
	UpdateData(TRUE);

	if(m_nStep >18000) 
	{
		AfxMessageBox(_LS(IDS_CMD_THFC_ERROR_INVLAID_NUMBER_OF_STEPS));
		return;
	}

	int nSpecType = m_cmbSpecType.GetCurSel();
	int count=0;

	BOOL bTripartite;
	bTripartite = (nSpecType==5)? TRUE : FALSE;
	if (bTripartite && m_fFrom < 0.01)
		m_fFrom = 0.01f;

	// MQC:1540-JHKIM-20080109
	// Log Scaling을 data에 직접 적용하던것 삭제, graph의 log scaling 기능을 이용함

	m_GraphView.SetZoomable(!bTripartite);
	if (bTripartite)
		m_GraphView.GraphZoomout(TRUE);

	MyGraphDisplay	*pD=(MyGraphDisplay*) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->SetTripartite(bTripartite);
	pD->SetDiagGrid(TRUE);

	if(!bTripartite)	
		OnXScale();
	else
	{
		pD->GetStyle()->SetLogX(TRUE);
	  pD->GetStyle()->SetLog(TRUE);
		pD->GetStyle()->SetScaleLimit(TRUE);
	  pD->SetMinRangeY(0.001);
		pD->SetMinRangeX(0.001);
	}

	CCMThfcRpsrProgDlg dlgProgress;
	dlgProgress.Create(IDD_ETC_THFC_RPSR_PROG,NULL);
	dlgProgress.ShowWindow(SW_SHOW);
	dlgProgress.m_Progress.SetRange(0, m_nStep);
	
	float *Umax=NULL, *dUmax=NULL, *ddUmax=NULL, *Pvelo=NULL, *Pacc=NULL;
	Umax   = new float[18000];
	dUmax  = new float[18000];
	ddUmax = new float[18000];
	Pvelo  = new float[18000];
	Pacc   = new float[18000];  
	
	float *U=NULL, *dU=NULL, *ddU=NULL, *TddU=NULL, *TddUmax=NULL, *P=NULL;
	U       = new float[18001];  
	dU      = new float[18001];  
	ddU     = new float[18001];  
	TddU    = new float[18001];  
	TddUmax = new float[18000];  
	P       = new float[18000];  

	float	Tn;
	float	st = m_fFrom;
	float	ed = m_fTo;
	int		step = m_nStep;
	float	dt;   // dt : original sampling period (0.02) * time scale : sampleTime :샘플의 시간간격  
	double fValue;

	Grav = m_dGravity;

	double dSampleTime = 0.02;
	if(m_arData.GetSize() >= 3)
	{
		dSampleTime = m_arData[2].dblTime - m_arData[1].dblTime;
		dt = dSampleTime * m_fTimeScale;
	}
	else
	{
		dt = 0.02 * m_fTimeScale;
	}

	// Scaling (Time, Spectral Value)
	for(int i=0; i<m_nNumOfData; ++i)
	{
		m_arData[i].dblTime = m_arData[i].dblTime * m_fTimeScale;    
		if (m_nDataType == 0) P[i] = m_arData[i].dblValue * m_fAmpScale * Grav; // normalized acceleration
		else                  P[i] = m_arData[i].dblValue * m_fAmpScale;    
	}

	double dXI;
	double dOM, dOMD, dOMB, dXIB;
	float dB0,dB1,dB2,dB3,dB4,dB5,dB6,dB7,dB8,dB9,dB10;
	float SS, CC, DSS, DCC, DDSS, DDCC;
	float dA1,dA2,dA3,dA4,dA5,dA6,dA7,dA8,dA9,dA10,dA11,dA12;

	m_Graph.KillData();
	for(int d=0;d < m_nNumOfDamping; d++)
	{
		dXI = m_fDamping[d];		// Damping Ratio

		for(int j=0;j< step; j++ )
		{
			// MQC:1540-JHKIM-20080109
	    // Log Scaling을 data에 직접 적용하던것 삭제, graph의 log scaling 기능을 이용함

			Tn=st + j*(ed-st)/step;
			if(Tn==0) Tn=0.0000000001f;

			dOM  = 2*PI/ Tn;                       // wn
			dOMD = dOM * sqrt(1.0 - pow(dXI, 2));  // wd
			dOMB = dOMD * dXI;
			dXIB = dXI / sqrt(1.0 - pow(dXI, 2));
			
			dB0  = 2.0*dXI/dOM/dt;
			dB1  = 1.0+dB0;
			dB2  =-1.0/dt;
			dB3  =-dXIB*dB1-dB2/dOMD;
			dB4  =-dB1;
			dB5  =-dB0;
			dB6  =-dB2;
			dB7  =-dXIB*dB5-dB6/dOMD;
			dB8  =-dB5;
			dB9  = pow(dOMD,2)-pow(dOMB,2);
			dB10 = 2.0*dOMB*dOMD;
			
			SS   =exp(-dXI*dOM*dt)*sin(dOMD*dt);
			CC   =exp(-dXI*dOM*dt)*cos(dOMD*dt);
			DSS  = -dOMB*SS+dOMD*CC;
			DCC  = -dOMB*CC-dOMD*SS;
			DDSS = -dB9*SS-dB10*CC;
			DDCC = -dB9*CC+dB10*SS;

			dA1  =  CC+dXIB*SS;
			dA2  =  SS/dOMD;
			dA3  = (dB1+dB2*dt+dB3*SS+dB4*CC)/(pow(dOM,2));
			dA4  = (dB5+dB6*dt+dB7*SS+dB8*CC)/(pow(dOM,2));
			dA5  =  DCC+dXIB*DSS;
			dA6  =  DSS/dOMD;
			dA7  = (dB2+dB3*DSS+dB4*DCC)/(pow(dOM,2));
			dA8  = (dB6+dB7*DSS+dB8*DCC)/(pow(dOM,2));
			dA9  =  DDCC+dXIB*DDSS;
			dA10 =  DDSS/dOMD;
			dA11 = (dB3*DDSS+dB4*DDCC)/(pow(dOM,2));
			dA12 = (dB7*DDSS+dB8*DDCC)/(pow(dOM,2));
			
			float DDY1, PM1;
			int   IST;
			if(m_arData[0].dblTime == 0.)
			{
				DDY1 = -P[0]; //DDY1 = -m_arData[0].dblValue;        
				PM1  = DDY1;
				IST  = 1;
			}
			else
			{
				PM1  = 0.0f;
				IST  = 0;
			}

			U[0]   = 0.0f;
			dU[0]  = 0.0f;
			ddU[0] = P[0]-2*dOM*dXI*dU[0]-pow(dOM,2)*U[0];    //ddU[0] = P[0]; 와 동일

			Umax[j]   = U[0];
			dUmax[j]  = dU[0];
			ddUmax[j] = ddU[0];
			TddUmax[j]= 0.0f;
			
			float PM[18000];
			for(int t=0; t<18000; ++t)      
				PM[t] = 0.0f;

			for(int i=0; i<m_nNumOfData; i++)      
			{
				if(i==m_nNumOfData-1 && IST==1) continue;

				PM[i] = (float)PM1;       	
				PM1   = - P[i+IST];        
				
				U[i+1]    = dA1*U[i] +  dA2*dU[i] +  dA3*PM[i] +  dA4*PM1;
				dU[i+1]   = dA5*U[i] +  dA6*dU[i] +  dA7*PM[i] +  dA8*PM1;
				ddU[i+1]  = dA9*U[i] + dA10*dU[i] + dA11*PM[i] + dA12*PM1;
				TddU[i+1] = ddU[i+1] + P[i+IST];
				
	            Umax[j]   =__max(Umax[j]   , fabs(U[i+1]) );      // Relative Displacement
				dUmax[j]  =__max(dUmax[j]  , fabs(dU[i+1]) );	  // Relative Velocity
				ddUmax[j] =__max(ddUmax[j] , fabs(ddU[i+1]));     // Relative Acceleration
				TddUmax[j]=__max(TddUmax[j], fabs(TddU[i+1]));    // Absolute Acceleration
			}
			
		  dlgProgress.m_Progress.SetPos(j);
 		  Pvelo[j]=Umax[j]*dOM;               // 		  Pvelo[j]=Umax[j]*wn;
			Pacc[j]=Umax[j]*pow(dOM,2);         

			// MQC:1540-JHKIM-20080109
	    // Normalized Accleration인 경우 다시 중력가속도로 나누어 무차원화 한다.
		  switch (nSpecType)
		  {
			case 0:	fValue=(m_nDataType==0) ? Pacc[j]/Grav : Pacc[j];		    break; // Pseudo Accel
			case 1: fValue=Pvelo[j];				break;			  
			case 2:	fValue=(m_nDataType==0) ? TddUmax[j]/Grav : TddUmax[j]; break; //Abs Accel
			case 3:	fValue=dUmax[j];				break;
			case 4:	fValue=Umax[j];					break;
			case 5:	fValue=Pvelo[j];				break;
		  }			
			SetValue(j, d, Tn, fValue);
		}
	}	

	if(  Umax!=NULL)  delete []  Umax;
	if( dUmax!=NULL)  delete [] dUmax;
	if(ddUmax!=NULL)  delete []ddUmax;
	if( Pvelo!=NULL)  delete [] Pvelo;
	if(  Pacc!=NULL)  delete []  Pacc;

	if(U      !=NULL)  delete []U      ;
	if(dU     !=NULL)  delete []dU     ;
	if(ddU    !=NULL)  delete []ddU    ;
	if(TddU   !=NULL)  delete []TddU   ;
	if(TddUmax!=NULL)  delete []TddUmax;
	if(P      !=NULL)  delete []P      ;

	CString str = _T("");
	m_cmbSpecType.GetLBText(nSpecType, str);

	// MQC:1540-JHKIM-20080109
	// Normalized Accleration인 경우 무차원이므로 (g), Accelration인 경우 [L/sec^2]
	if (nSpecType==0 || nSpecType==2)   //Acceleration
	{
		if(m_nDataType == 0) str += _T(" (g)"); // normalized acceleration
		else                 str += _T(" (") + CUnitCtrl::m_strGravity + _T(")");
	}
	else if (nSpecType==1 || nSpecType==3)  //Velocity
	{
		str += _T(" (") + CUnitCtrl::m_strVelocity + _T(")");
	}
	else if (nSpecType==4)  //Disp
	{
		str += _T(" (") + CUnitCtrl::m_strLength + _T(")");
	}
	else if (nSpecType==5)  //DVA
	{
		m_cmbSpecType.GetLBText(1, str);
		str += _T(" (") + CUnitCtrl::m_strVelocity + _T(")");
	}

	SetAxisLabelText(1, str);

	SetLegend();
	dlgProgress.ShowWindow(SW_HIDE);
	dlgProgress.DestroyWindow();

	// XY 축 Log Scale반영
	m_GraphView.Invalidate();
}

void CCMThfcRpsrDlg::SetLegend()
{	
	// Add Legend Component
	//
	SRGraphLegend	*pL=new SRGraphLegend;
	SRGraphStyle* pS=pL->GetStyle();
	pL->SetMeasurement(CX_PERCENT);
	pL->SetSizeDefaults();
	pL->SetRect(24., 94, 100., 100.);
	pL->GetStyle()->SetColor(CXCLR_BACKGRND);
	pL->SetScope(-1,-1,0,m_nNumOfDamping-1);
	pS->SetLegendKeyFirst(TRUE);
	pS->SetLegendKeyStyle(CX_KEY_BOX); 
	pL->SetFontSize(8);
	pL->SetKeySize(2.); // % of rect.WIDTH (def=5%)
	pL->SetXMargin(0.5); // % of screen WIDTH (def=2.5%)
	pL->SetYMargin(0.5); // % of screen HEIGHT (def=2.5%)
	pL->SetJustification(0); // align right
	pL->SetKeyAnnotationCharCount(8); // 15 chars each
	pL->SetKeyAnnotationLineCount(1); // 2 lines each

	pS->SetLegendLockRC(TRUE); // 2x3 layout
	pS->SetLegendColumns(6);
	pS->SetLegendRows(1);
	m_Graph.AddComponent(pL);

	SRGraphLabel *pLabel;	
	pLabel=new SRGraphLabel;   // Legend Label
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::MidLeft);
	pLabel->SetTextPosition(7.0, 95.5);
	pLabel->SetFontSize(9);
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Normal);
	pLabel->GetStyle()->SetColor(CXCLR_BACKGRND);
	m_Graph.AddComponent(pLabel);
	SetAxisLabelText(2, _T("Damping :"));
	
	for(int d = 0; d< m_nNumOfDamping; d++)
	{
		CString damping;
		damping.Format(_T(" %1.2f"),m_fDamping[d]);
		m_Graph.SetHeader(d,damping);
	}
}

void CCMThfcRpsrDlg::SetValue(int nIndex, int nGroup, double x,double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
}

void CCMThfcRpsrDlg::OnETCSPFCCHARTXLog() 
{
	OnCmdGenerateBtn();
}

void CCMThfcRpsrDlg::OnETCSPFCCHARTXLogDraw() 
{
}

void CCMThfcRpsrDlg::OnETCSPFCCHARTYLog() 
{
	UpdateData(TRUE);
	MyGraphDisplay	*pD=(MyGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLogX(m_bLogX);
	pD->GetStyle()->SetLog(m_bLogY);
	
	if (m_bLogX || m_bLogY)
	{
		pD->GetStyle()->SetScaleLimit(TRUE);
	  pD->SetMinRangeY(0.001);
		pD->SetMinRangeX(0.001);
	}
	else 
	 	pD->GetStyle()->SetScaleLimit(FALSE);

	m_GraphView.Invalidate();	
}

void CCMThfcRpsrDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CCMThfcRpsrDlg::OnSelchangeCmdSpectrumType() 
{
	int nSel = m_cmbSpecType.GetCurSel();
	
	if (nSel==5)
	{
		UpdateData(TRUE);
		m_bLogX = TRUE;
		m_bLogY = TRUE;
		UpdateData(FALSE);
	}
	GetDlgItem(IDC_ETC_SPFC_CHART_XLog)->EnableWindow(nSel!=5);
	GetDlgItem(IDC_ETC_SPFC_CHART_YLog)->EnableWindow(nSel!=5);
}
