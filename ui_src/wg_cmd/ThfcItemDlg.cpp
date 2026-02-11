// ThfcItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ThfcItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\DialogActionReactor.h"

//#include "ETC_DESIGN_SPECT.h"   // MNET:2138-GSJI-20060601  복사실수로 판단되어 막음.
#include "EQ_REC_DLG.h"
#include "HeelLoadMainDlg.h"
#include "CMThfcRpsrDlg.h"
#include "..\wg_base\wg_base_FFT.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThfcItemDlg dialog


CThfcItemDlg::CThfcItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CThfcItemDlg::IDD, pParent)
{
	m_bSinusoidal = FALSE;
	m_bSetParam = FALSE;
	m_bGridInit = FALSE;
	//{{AFX_DATA_INIT(CThfcItemDlg)
	m_csFuncName = _T("");
	m_csDescription = _T("");

	m_nDataType = -1;
	m_nScaleType = 0;
	//}}AFX_DATA_INIT

	m_aTimeCtrl.Add(IDC_THFC_IMPORT_FILE);
	m_aTimeCtrl.Add(IDC_THFC_DESIGN_SPECTRUM);
	//m_aTimeCtrl.Add(IDC_THFC_SEISMIC_GEN);
	m_aTimeCtrl.Add(IDC_THFC_TABLE);
	m_aTimeCtrl.Add(IDC_THFC_DESIGN_HEELDROP);

	m_aSinuCtrl.Add(IDC_THFC_SIN_FRAME);
	m_aSinuCtrl.Add(IDC_THFC_SIN_ST1);
	m_aSinuCtrl.Add(IDC_THFC_SIN_ST2);
	m_aSinuCtrl.Add(IDC_THFC_SIN_ST3);
	m_aSinuCtrl.Add(IDC_THFC_SIN_ST4);
	m_aSinuCtrl.Add(IDC_THFC_SIN_ST5);
	m_aSinuCtrl.Add(IDC_THFC_SIN_ST6);
	m_aSinuCtrl.Add(IDC_THFC_SIN_A_TITLE);
	m_aSinuCtrl.Add(IDC_THFC_SIN_A_EDIT);
	m_aSinuCtrl.Add(IDC_THFC_SIN_A_UNIT);
	m_aSinuCtrl.Add(IDC_THFC_SIN_C_TITLE);
	m_aSinuCtrl.Add(IDC_THFC_SIN_C_EDIT);
	m_aSinuCtrl.Add(IDC_THFC_SIN_C_UNIT);
	m_aSinuCtrl.Add(IDC_THFC_SIN_F_TITLE);
	m_aSinuCtrl.Add(IDC_THFC_SIN_F_EDIT);
	m_aSinuCtrl.Add(IDC_THFC_SIN_F_UNIT);
	m_aSinuCtrl.Add(IDC_THFC_SIN_D_TITLE);
	m_aSinuCtrl.Add(IDC_THFC_SIN_D_EDIT);
	m_aSinuCtrl.Add(IDC_THFC_SIN_PA_TITLE);
	m_aSinuCtrl.Add(IDC_THFC_SIN_PA_EDIT);
	m_aSinuCtrl.Add(IDC_THFC_SIN_PA_UNIT);

	m_aSinuCtrl.Add(IDC_THFC_GR_FRAME);
	m_aSinuCtrl.Add(IDC_THFC_GR_CHECK);
	m_aSinuCtrl.Add(IDC_THFC_GR_ST1);
	m_aSinuCtrl.Add(IDC_THFC_GR_ST2);
	m_aSinuCtrl.Add(IDC_THFC_GR_TIME_INC);
	m_aSinuCtrl.Add(IDC_THFC_GR_D_TIME);
	m_aSinuCtrl.Add(IDC_THFC_SIN_REDRAW_GRAPH);

	m_aParamCtrl.Add(IDC_THFC_GR_TIME_INC);
	m_aParamCtrl.Add(IDC_THFC_GR_D_TIME);

	m_aTypeCtrl.Add(IDC_THFC_TYPE_NORMAL);
	m_aTypeCtrl.Add(IDC_THFC_TYPE_ACC);
	m_aTypeCtrl.Add(IDC_THFC_TYPE_FORCE);
	m_aTypeCtrl.Add(IDC_THFC_TYPE_MOMENT);
	m_aTypeCtrl.Add(IDC_THFC_TYPE_NORMAL2);
}


void CThfcItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThfcItemDlg)
	DDX_Control(pDX, IDC_ETC_THFC_CHART_FFT, m_btnFFT);
	DDX_Control(pDX, IDC_THFC_GRAVITY, m_wndGravity);
	DDX_Control(pDX, IDC_THFC_SCALE, m_wndScaleFactor);
	DDX_Control(pDX, IDC_THFC_GR_CHECK, m_wndDrawCheck);
	DDX_Control(pDX, IDC_THFC_GR_D_TIME, m_wndDTime);
	DDX_Control(pDX, IDC_THFC_GR_TIME_INC, m_wndTimeInc);
	DDX_Control(pDX, IDC_THFC_SIN_PA_UNIT, m_wndSinPAUnit);
	DDX_Control(pDX, IDC_THFC_SIN_PA_EDIT, m_wndSinPA);
	DDX_Control(pDX, IDC_THFC_SIN_D_EDIT, m_wndSinD);
	DDX_Control(pDX, IDC_THFC_SIN_F_UNIT, m_wndSinFUnit);
	DDX_Control(pDX, IDC_THFC_SIN_F_EDIT, m_wndSinF);
	DDX_Control(pDX, IDC_THFC_SIN_C_UNIT, m_wndSinCUnit);
	DDX_Control(pDX, IDC_THFC_SIN_C_EDIT, m_wndSinC);
	DDX_Control(pDX, IDC_THFC_SIN_A_UNIT, m_wndSinAUnit);
	DDX_Control(pDX, IDC_THFC_SIN_A_EDIT, m_wndSinA);
	DDX_Control(pDX, IDC_THFC_GRAVITY_UNIT, m_wndGravityUnit);
	DDX_Control(pDX, IDC_ETC_THFC_CHART_YLog, m_btnYLog);
	DDX_Control(pDX, IDC_ETC_THFC_CHART_XLog, m_btnXLog);
	DDX_Text(pDX, IDC_THFC_FUNC_NAME, m_csFuncName);
	DDX_Text(pDX, IDC_CMD_ETC_DESCRIPT,m_csDescription);
	DDX_Radio(pDX, IDC_THFC_TYPE_NORMAL, m_nDataType);
	DDX_Control(pDX, IDC_THFC_SCALE2, m_wndScaleMaxVal);
	DDX_Control(pDX, IDC_THFC_SCALE_UNIT, m_wndScaleUnit);
	DDX_Radio(pDX, IDC_THFC_SCALE_RDO1, m_nScaleType);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Control(pDX, IDC_THFC_TABLE, m_wndGrid);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		m_ParamData.ThisFuncName = m_csFuncName;
		m_ParamData.nThisFuncType = (m_bSinusoidal) ? 2 : 1;
		m_ParamData.nThisDataType = m_nDataType + 1;
		m_ParamData.Description = m_csDescription;
		
		if (m_ParamData.nThisFuncType == 1)
		{
			//Time Function 일때
			m_ParamData.nScaleMethod = m_nScaleType;
			if (m_nScaleType == 0)
			{
				if (!m_wndScaleFactor.GetEditValue(m_ParamData.ScaleFactor))
				{
					AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_));
					pDX->Fail();
				}
			}
			else 
			{
				m_ParamData.dMaxValue = m_wndScaleMaxVal.GetEditValue();
			}
		}
		m_ParamData.Gravity = m_wndGravity.GetEditValue();
		
		m_ParamData.SinuA = m_wndSinA.GetEditValue();
		m_ParamData.SinuC = m_wndSinC.GetEditValue();
		m_ParamData.SinuFrequency = m_wndSinF.GetEditValue();
		m_ParamData.SinuDampingFactor = m_wndSinD.GetEditValue();
		m_ParamData.SinuPhaseAngle = m_wndSinPA.GetEditValue();
	}
	else
	{
		if (!m_bGridInit) return;
		BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
		ROWCOL nCount = m_ParamData.arThisFuncData.GetSize();
		m_wndGrid.SetRowCount(nCount+1);
		m_wndGrid.SetCurrentCell(nCount+1, 1);

		m_wndGrid.LockUpdate(bOldLock);
		if (!bOldLock) m_wndGrid.Redraw();
	}
}

void CThfcItemDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	// 1st point
	GetDlgItem(IDC_THFC_IMPORT_FILE)->GetWindowRect(rRef);
	GetDlgItem(IDC_THFC_SIN_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aSinuCtrl, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

void CThfcItemDlg::SetSinuCtrlUnitType(int nDataType)
{
	m_wndSinA.SetUnitType(CUnitCtrl::m_THFC_UNIT.SinuA[nDataType]);
	m_wndSinAUnit.SetUnitType(CUnitCtrl::m_THFC_UNIT.SinuA[nDataType]);
	m_wndSinC.SetUnitType(CUnitCtrl::m_THFC_UNIT.SinuC[nDataType]);
	m_wndSinCUnit.SetUnitType(CUnitCtrl::m_THFC_UNIT.SinuC[nDataType]);
	m_wndSinF.SetUnitType(CUnitCtrl::m_THFC_UNIT.SinuFrequency);
	m_wndSinFUnit.SetUnitType(CUnitCtrl::m_THFC_UNIT.SinuFrequency);
	m_wndSinD.SetUnitType(CUnitCtrl::m_THFC_UNIT.SinuDampingFactor);
	m_wndSinPA.SetUnitType(CUnitCtrl::m_THFC_UNIT.SinuPhaseAngle);
	m_wndSinPAUnit.SetUnitType(CUnitCtrl::m_THFC_UNIT.SinuPhaseAngle);

	if (nDataType == 0) // Normal
	{
		m_wndSinAUnit.SetWindowText(_T("g"));
		m_wndSinCUnit.SetWindowText(_T("g/sec"));
	}
}

BOOL CThfcItemDlg::CalcParamDataFromSinu(CArray<T_THFC_BASE, T_THFC_BASE&> &aThisFuncData)
{
	double A, C, f, D, PA;
	A = m_wndSinA.GetEditValue();
	C = m_wndSinC.GetEditValue();
	f = m_wndSinF.GetEditValue();
	D = m_wndSinD.GetEditValue();
	PA = m_wndSinPA.GetEditValue();

	if (D < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Damping_factor_must_be_gr));
		return FALSE;
	}
	if (f == 0.0)
	{
		aThisFuncData.RemoveAll();
		return TRUE;
	}

#define D_THFC_MAX_COUNT 5000
#define D_THFC_DEF_CYCLE 10
	double tInc, tDraw;
	BOOL bCalcParam = m_wndDrawCheck.GetCheck() == 1;
	if (bCalcParam) // Calculate drawing parameter
	{
		tInc = 1.0/(36.0*f);     // sampling every 10 deg.

		BOOL bCalcChanged = FALSE;
		tDraw = D_THFC_DEF_CYCLE/f;
		if ((int)(tDraw/tInc) > D_THFC_MAX_COUNT)
		{
			tDraw = tInc*D_THFC_MAX_COUNT;
			bCalcChanged = TRUE;
		}
		CString csVal;
		csVal.Format(_T("1.0/(36.0*%g)"), f);
		m_wndTimeInc.SetWindowText(csVal);
		if (bCalcChanged) csVal.Format(_T("%g"), tDraw);
		else csVal.Format(_T("%d/%g"), D_THFC_DEF_CYCLE, f);
		m_wndDTime.SetWindowText(csVal);
	}
	else  // User input parameter
	{
		if (!m_wndTimeInc.GetEditValue(tInc))
		{
			tInc = 1.0/(36.0*f);     // sampling every 10 deg.
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Warning___Invalid_time_increment_), tInc);
		}
		if (!m_wndDTime.GetEditValue(tDraw))
		{
			tDraw = D_THFC_DEF_CYCLE/f;
			if ((int)(tDraw/tInc) > D_THFC_MAX_COUNT) 
				tDraw = tInc*D_THFC_MAX_COUNT;
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Warning___Invalid_drawing_time__C), tDraw);
		}
		else  // 입력 받은 Drawing time의 범위 검증
		{
			if ((int)(tDraw/tInc) > D_THFC_MAX_COUNT) 
			{
				tDraw = tInc*D_THFC_MAX_COUNT;
				GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Warning___Drawing_time_is_too_lon), tDraw);
			}
		}
	}

	double t = 0;
	double Ft;
	double eVal, sinVal;
	T_THFC_BASE data;

	for (int i = 0; i < D_THFC_MAX_COUNT && t <= tDraw; i++)
	{
		eVal = (A+C*t)*exp(-D*t);
		sinVal = sin(2*PI*f*t + PA*PI/180.0);
		Ft = eVal*sinVal;
		if (Ft == HUGE_VAL || Ft == -HUGE_VAL)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___F_t__value_is_out_of_rang));
			return FALSE;
		}

		data.dblTime = t;
		data.dblValue = Ft;
		aThisFuncData.Add(data);

		t += tInc;
	}
	return TRUE;
}

T_THFC_D& CThfcItemDlg::GetParamData()
{
	return m_ParamData;
}

void CThfcItemDlg::SetParamData(T_THFC_D& data)
{
	if (data.nThisFuncType == 1) m_bSinusoidal = FALSE;
	else m_bSinusoidal = TRUE;

	m_bSetParam = TRUE;
	m_ParamData = data;
	m_csOldName = m_ParamData.ThisFuncName;
}

void CThfcItemDlg::OnBtnDyGen()
{
	UpdateData(TRUE);

	T_THFC_K Key;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	Key = pDoc->m_pAttrCtrl->GetThfcKey(m_ParamData.ThisFuncName);
	
	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_TH_FUNC; // TH Func.
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = Key;

	CString csName; csName.Empty();
	
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if (nCount == 0) csName = _LS(IDS_CMD_DYNA_RPT_THFUNC_NAME); //_T("Time History Function");
	else csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_THFUNC_NAME), nCount + 1);

	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if(dlg.DoModal() == IDOK)
	{
		UchtD.strName = csName;
		UchtD.bLogX = m_btnXLog.GetCheck();
		UchtD.bLogY = m_btnYLog.GetCheck();
		UchtD.nFuncType = m_btnFFT.GetCheck();
		if(!pDoc->m_pDataCtrl->AddUcht(UchtD)) ASSERT(0);

	}
}
void CThfcItemDlg::InitGrid()
{
	m_wndGrid.Initialize(&m_ParamData);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_bGridInit = TRUE;
}

void CThfcItemDlg::EnableGravity(BOOL bEnable)
{
	CWnd* pWnd = GetDlgItem(IDC_THFC_GRAVITY);
	pWnd->EnableWindow(bEnable);
}

void CThfcItemDlg::ChangeDataType(int nType)
{
	if (m_nDataType == nType) return;
	UINT aID[] = { 
		IDC_THFC_TYPE_NORMAL, IDC_THFC_TYPE_ACC, 
		IDC_THFC_TYPE_FORCE, IDC_THFC_TYPE_MOMENT, IDC_THFC_TYPE_NORMAL2
	};
	// Turn off old radio
	CButton* pRadio = (CButton*)GetDlgItem(aID[m_nDataType]);
	pRadio->SetCheck(0);

	// Turn on new radio
	m_nDataType = nType;
	pRadio = (CButton*)GetDlgItem(aID[m_nDataType]);
	pRadio->SetCheck(1);

	// enable/disable type radio
	/*
	BOOL bEnable;
	int nNum = sizeof(aID)/sizeof(UINT);
	for (int i = 0; i < nNum; i++)
	{
		bEnable = (i == m_nDataType) ? TRUE : FALSE; 
		GetDlgItem(aID[i])->EnableWindow(bEnable);
	}
	*/

	EnableGravity(m_nDataType == 0);
	T_UNIT_SYSTEM us;
	m_ParamData.nThisDataType = m_nDataType+1;
	m_wndGrid.SetHeaderTitle(us, FALSE);
}

BEGIN_MESSAGE_MAP(CThfcItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CThfcItemDlg)
	ON_BN_CLICKED(IDC_THFC_IMPORT_FILE, OnThfcImportFile)
	ON_BN_CLICKED(IDC_THFC_SEISMIC_GEN, OnThfcSeismicGen)
	ON_BN_CLICKED(IDC_THFC_TYPE_ACC, OnChangeDataType)
	ON_BN_CLICKED(IDC_THFC_DESIGN_SPECTRUM, OnThfcEQRecord)
	ON_BN_CLICKED(IDC_ETC_THFC_CHART_XLog, OnETCTHFCCHARTXLog)
	ON_BN_CLICKED(IDC_ETC_THFC_CHART_YLog, OnETCTHFCCHARTYLog)
	ON_BN_CLICKED(IDC_THFC_SIN_REDRAW_GRAPH, OnThfcSinRedrawGraph)
	ON_BN_CLICKED(IDC_THFC_GR_CHECK, OnThfcGrCheck)
	ON_BN_CLICKED(IDC_THFC_DESIGN_HEELDROP, OnThfcDesignHeeldrop)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnApply)
	ON_BN_CLICKED(IDC_THFC_SCALE_RDO1, OnChangeScaleRdo)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_THFC_TYPE_FORCE, OnChangeDataType)
	ON_BN_CLICKED(IDC_THFC_TYPE_MOMENT, OnChangeDataType)
	ON_BN_CLICKED(IDC_THFC_TYPE_NORMAL, OnChangeDataType)
	ON_BN_CLICKED(IDC_THFC_TYPE_NORMAL2, OnChangeDataType)
	ON_BN_CLICKED(IDC_THFC_SCALE_RDO2, OnChangeScaleRdo)
	ON_BN_CLICKED(IDC_ETC_THFC_CHART_FFT, OnEtcThfcChartFft)
	ON_BN_CLICKED(IDC_CMD_RESP_SPECTRUM, OnResponseSpectrum)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThfcItemDlg message handlers

BOOL CThfcItemDlg::OnInitDialog() 
{
	AlignControls();
	CDialogMove::OnInitDialog();
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	// TODO: Add extra initialization here
	CDlgUtil::CtrlShowHide(this, m_aTimeCtrl, !m_bSinusoidal);
	CDlgUtil::CtrlShowHide(this, m_aSinuCtrl, m_bSinusoidal);
	
	GetDlgItem(IDC_CMD_RESP_SPECTRUM)->ShowWindow(!m_bSinusoidal);
	m_wndScaleFactor.EnableWindow(!m_bSinusoidal);

	if (!m_bSetParam) 
	{
		m_ParamData.Initialize();
		m_ParamData.ScaleFactor = 1.0;
		m_ParamData.Gravity = pDoc->m_pInitCtrl->DefaultGravity();
	}
	m_nScaleType = m_ParamData.nScaleMethod;

	// 호출
	DialogActionReactor::DoDlgReact(DialogActionReactor::kOnInitAfter, CThfcItemDlg::IDD, this);

	//KYE-HONG-2006025
	//Gen2006 JP보내기 전에 김종민과장께서 후처리 그래프가 
	//문제있다고 해서 원인을 찾던중.. 
	//Function을 Sine형태로 저장할때 Scale Factor를 사용하지 않는데
	//Control이 활성화 되있다. 이것을 비활성화 시키는 것으로 한다. 
	CArray<UINT, UINT> aScaleCtrl;
	aScaleCtrl.Add(IDC_THFC_SCALE_RDO1);
	aScaleCtrl.Add(IDC_THFC_SCALE);
	aScaleCtrl.Add(IDC_THFC_SCALE_RDO2);
	aScaleCtrl.Add(IDC_THFC_SCALE2);
	aScaleCtrl.Add(IDC_THFC_SCALE_UNIT);
	if( m_bSinusoidal ) 
	{
		m_nScaleType = 0;
		m_ParamData.ScaleFactor  = 1.0;
		m_ParamData.nScaleMethod = 1; // Max Val
		CDlgUtil::CtrlEnableDisable(this, aScaleCtrl, FALSE);
	}

	m_csFuncName = m_ParamData.ThisFuncName;
	m_csDescription = m_ParamData.Description;
	m_nDataType = (m_ParamData.nThisDataType == 0) ? 0 : m_ParamData.nThisDataType-1;
	BOOL bEnable = (m_nDataType==0 || m_nDataType==1); // normalize acc, acc만 가능함// MQC:1540-JHKIM-20080109
	GetDlgItem(IDC_CMD_RESP_SPECTRUM)->EnableWindow(bEnable);
	
	if (m_nScaleType == 0)
	{
		//Scale Factor
		CString csScale;
		csScale.Format(_T("%g"), m_ParamData.ScaleFactor);
		m_wndScaleFactor.SetWindowText(csScale);
	}
	else
	{
		m_wndScaleMaxVal.SetEditUnit(m_ParamData.dMaxValue);
	}
	
	CString csGravity;
	csGravity.Format(_T("%g"), m_ParamData.Gravity);
	m_wndGravity.SetWindowText(csGravity);
	m_wndGravity.SetUnitType(CUnitCtrl::m_THFC_UNIT.Gravity);
	m_wndGravityUnit.SetUnitType(CUnitCtrl::m_THFC_UNIT.Gravity);

	SetSinuCtrlUnitType(m_nDataType);
	CString csVal;
	csVal.Format(_T("%g"), m_ParamData.SinuA);
	m_wndSinA.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_ParamData.SinuC);
	m_wndSinC.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_ParamData.SinuFrequency);
	m_wndSinF.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_ParamData.SinuDampingFactor);
	m_wndSinD.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_ParamData.SinuPhaseAngle);
	m_wndSinPA.SetWindowText(csVal);

	InitGrid();
	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;

	m_wndDrawCheck.SetCheck(1);
	CDlgUtil::CtrlEnableDisable(this, m_aParamCtrl, FALSE);
	m_wndTimeInc.SetWindowText(_T("0.1"));
	m_wndDTime.SetWindowText(_T("10"));

	InitChart();
	m_wndGrid.MakeItemEx();

	UpdateData(FALSE);

	EnableGravity(m_nDataType == 0);
	OnDataChanged();
	ChangeDataType();
	//OnChangeDataType();
	OnChangeScaleRdo();

	T_THFC_K Key;
	CDBDoc* m_pDoc = CDBDoc::GetDocPoint();
	Key = m_pDoc->m_pAttrCtrl->GetThfcKey(m_ParamData.ThisFuncName);

	BOOL bEnable2;
	if(Key > 0) bEnable2 = TRUE;
	else bEnable2 = FALSE;

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	GetDlgItem(IDC_CMD_DYGEN)->EnableWindow(bEnable2);

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CThfcItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE)) return;
	if (this->m_bSinusoidal)
	{
		CArray<T_THFC_BASE, T_THFC_BASE&> aThisFuncData;
		if (!CalcParamDataFromSinu(aThisFuncData)) return;

		m_ParamData.arThisFuncData.RemoveAll();
		int nSize = aThisFuncData.GetSize();
		for(int i=0; i<nSize; i++)
		{
			m_ParamData.arThisFuncData.Add(aThisFuncData[i]);
		}
	}


	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess;

	if (m_bSetParam)
		bSuccess = pDoc->m_pDataCtrl->ModifyThfc(m_csOldName, m_ParamData);
	else
		bSuccess = pDoc->m_pDataCtrl->AddThfc(m_ParamData);

	if (!bSuccess) return;
	CDialogMove::OnOK();
}

void CThfcItemDlg::OnThfcImportFile() 
{
	//static char  szFilter[] = _LS(IDS_WG_CMD__ADDD__SGS_or_THD_File____sgs____thd____);
	//const TCHAR*  szFilter = _LS(IDS_WG_CMD__ADDD__SGS_or_THD_File____sgs____thd____);
	const TCHAR* szFilter = _LS(IDS_WG_CMD__ADDD__SGS_or_THD_File____sgs____thd____at2);
	FILE* fp;
	CFileDialog	fd(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
			
	if (fd.DoModal() == IDOK) 
	{
		CString strFileExt = fd.GetFileExt();

	  if (strFileExt == _T("sgs") || strFileExt == _T("SGS") || 
				strFileExt == _T("tgs") || strFileExt == _T("TGS") ||
				strFileExt == _T("thd") || strFileExt == _T("THD") ||
				strFileExt == _T("at2") || strFileExt == _T("AT2"))
		{
			if(!( fp = _tfopen(fd.GetPathName().operator LPCTSTR(), _T("r")) ) )
			{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_opening_f));
				return;
			}
	 
			if (strFileExt == _T("sgs") || strFileExt == _T("SGS") ) 
			{
				if(!ReadSGSData(fp)) 
				{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_reading_d));
					return;
				}
			}
			else if(strFileExt == _T("tgs") || strFileExt == _T("TGS"))
			{
				if(!ReadTGSData(fp))
				{
					AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_reading_d));
					return;
				}
				SendMessage(WM_COMMAND, MAKEWPARAM(IDC_THFC_TYPE_FORCE, BN_CLICKED));
				CButton * pBtn = (CButton*)GetDlgItem(IDC_THFC_TYPE_FORCE);
				pBtn->SetCheck(TRUE);
				pBtn = (CButton*)GetDlgItem(IDC_THFC_TYPE_NORMAL);
				pBtn->SetCheck(FALSE);
				pBtn = (CButton*)GetDlgItem(IDC_THFC_TYPE_NORMAL2);
				pBtn->SetCheck(FALSE);
				pBtn = (CButton*)GetDlgItem(IDC_THFC_TYPE_ACC);
				pBtn->SetCheck(FALSE);
				pBtn = (CButton*)GetDlgItem(IDC_THFC_TYPE_MOMENT);
				pBtn->SetCheck(FALSE);
			}
		  else if (strFileExt == _T("thd") || strFileExt == _T("THD") )
			{
				if(!ReadTHDData(fp))
				{
					AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_reading_d));
					return;
				}
				
				SetDescription(fd.GetPathName());
			}
		  else if (strFileExt == _T("at2") || strFileExt == _T("AT2"))
			{
				if (!ReadAT2Data(fp))
				{
					AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_reading_d));
					return;
				}
				SendMessage(WM_COMMAND, MAKEWPARAM(IDC_THFC_TYPE_NORMAL, BN_CLICKED));
				CButton* pBtn = (CButton*)GetDlgItem(IDC_THFC_TYPE_NORMAL);
				pBtn->SetCheck(TRUE);
				pBtn = (CButton*)GetDlgItem(IDC_THFC_TYPE_ACC);
				pBtn->SetCheck(FALSE);
				pBtn = (CButton*)GetDlgItem(IDC_THFC_TYPE_FORCE);
				pBtn->SetCheck(FALSE);
				pBtn = (CButton*)GetDlgItem(IDC_THFC_TYPE_MOMENT);
				pBtn->SetCheck(FALSE);
				pBtn = (CButton*)GetDlgItem(IDC_THFC_TYPE_NORMAL2);
				pBtn->SetCheck(FALSE);
			}
			m_wndGrid.MakeItemEx();
			CString ddd = fd.GetFileTitle();
			SetFunctionName(fd.GetFileTitle());
			OnDataChanged();
		}
		else 
		{
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_extension_));
			return;
		}

		SetEnableDygenButton();
	}
}

void CThfcItemDlg::OnThfcSeismicGen() 
{
	CString sSGSw;

	sSGSw=_T("SGSw.exe");
	
	LPTSTR pParam = sSGSw.GetBuffer(_MAX_FNAME);

	PROCESS_INFORMATION	piProcInfo;
	STARTUPINFO	siStartInfo;
	siStartInfo.cb=sizeof(STARTUPINFO);
	siStartInfo.lpReserved=NULL;
	siStartInfo.lpDesktop=NULL;
	siStartInfo.lpTitle=NULL;
	siStartInfo.dwFlags=STARTF_FORCEOFFFEEDBACK;
	siStartInfo.lpReserved2=NULL;
	siStartInfo.cbReserved2=0;
	siStartInfo.lpReserved2=NULL;

	if(!::CreateProcess(NULL, pParam, NULL, NULL, FALSE, 0,
						NULL, NULL,	&siStartInfo, &piProcInfo)) MessageBox(_LS(IDS_WG_CMD__ADDD__Error));//throw((DWORD)::GetLastError());

	sSGSw.ReleaseBuffer();
}

void CThfcItemDlg::OnChangeDataType() 
{
	// TODO: Add your control notification handler code here

	// 실제 단위 변환이 되지 않는다는 메시지 출력 
	// 단, 데이터가 하나 이상 있는 경우에만 
	UINT aID[] = { IDC_THFC_TYPE_NORMAL, IDC_THFC_TYPE_ACC, 
				   IDC_THFC_TYPE_FORCE,  IDC_THFC_TYPE_MOMENT, IDC_THFC_TYPE_NORMAL2,};

	const MSG* pMsg = GetCurrentMessage();  
	UINT nMessageID = LOWORD(pMsg->wParam);
	CMapEx<UINT,UINT,int,int> mRadioID;
	for(int i = 0; i < 5; i++)    mRadioID.SetAt(aID[i], 0);

	if(mRadioID.Exist(nMessageID) && nMessageID != aID[m_nDataType])
	{
		BOOL bExistData = FALSE;
		if (m_bSinusoidal)
			bExistData = (m_wndSinA.GetEditValue() != 0.0 || m_wndSinC.GetEditValue() != 0.0);
		else 
			bExistData = (m_wndGrid.GetRowCount() > 1);

		int nYesNo = IDYES ;

		if (bExistData)
		{
			nYesNo = AfxMessageBox(_LS(IDS_CMD_THFC_WARNING_CHANGE_DATA_TYPE), MB_YESNO) ;

			if(nYesNo != IDYES)
			{
				CDlgUtil::CtrlRadioSetCheck(this , m_aTypeCtrl, m_nDataType);
				return;
			}
		}
	}

	switch(nMessageID)
	{
		case IDC_THFC_TYPE_NORMAL:  m_nDataType = 0; break;    
		case IDC_THFC_TYPE_ACC:     m_nDataType = 1; break;
		case IDC_THFC_TYPE_FORCE:   m_nDataType = 2; break;
		case IDC_THFC_TYPE_MOMENT:  m_nDataType = 3; break;
		case IDC_THFC_TYPE_NORMAL2: m_nDataType = 4; break;
	}
	
	BOOL bEnable = (m_nDataType==0 || m_nDataType==1); // normalize acc, acc만 가능함// MQC:1540-JHKIM-20080109
	GetDlgItem(IDC_CMD_RESP_SPECTRUM)->EnableWindow(bEnable);

	ChangeDataType();
}

void CThfcItemDlg::ChangeDataType()
{
	const MSG* pMsg = GetCurrentMessage();
	switch(LOWORD(pMsg->wParam))
	{
		case IDC_THFC_TYPE_NORMAL:  m_nDataType = 0; break;
		case IDC_THFC_TYPE_ACC:     m_nDataType = 1; break;
		case IDC_THFC_TYPE_FORCE:   m_nDataType = 2; break;
		case IDC_THFC_TYPE_MOMENT:  m_nDataType = 3; break;
		case IDC_THFC_TYPE_NORMAL2: m_nDataType = 4; break;   // added by KYM(2002.5.4)
	}
	T_UNIT_SYSTEM us;
	m_ParamData.nThisDataType = m_nDataType+1;
	m_wndGrid.SetHeaderTitle(us, FALSE);
	EnableGravity(m_nDataType == 0);
	SetSinuCtrlUnitType(m_nDataType);
	OnDataChanged();

	//Unit change시킬것 (단위만 변환되고 실제 값은 변환되지 않는다.)
	CString csUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_THFC_UNIT.dblValue[m_nDataType], csUnit);
	if (m_nDataType == 0)
		csUnit.Format(_T("g"));
	m_wndScaleUnit.SetWindowText(csUnit);
}

BOOL CThfcItemDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
	  if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{
		  if(pMsg->wParam == VK_RETURN)
			if(pMsg->wParam == VK_ESCAPE)
			
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;        
			// DO NOT process further
		}
	}
	return CDialogMove::PreTranslateMessage(pMsg);
}

void CThfcItemDlg::OnDataChanged()
{
	SetAxisLabelText(0,_LS(IDS_WG_CMD__ADD2__Time__sec_));
	SetAxisLabelText(1,_LS(IDS_WG_CMD__ADDD__Time_History_Data));

	// Add handler to update graph
	T_THFC_BASE data;
	int nSize = m_ParamData.arThisFuncData.GetSize();
	
	m_Graph.KillData();	
	for(int i=0; i<nSize; i++)
	{
		data = m_ParamData.arThisFuncData[i];
		SetValue(i,0,data.dblTime,data.dblValue);
	}
	m_GraphView.Invalidate();

	m_btnFFT.SetCheck(0);
}

// This is Eqrthquake Record
void CThfcItemDlg::OnThfcEQRecord() 
{
	EQ_REC_DLG dlg;
	
	//dlg.m_parAccel = &m_arAccel;
	//dlg.m_parPeriod = &m_arTime;
	
	m_arAccel.RemoveAll();
	m_arTime.RemoveAll();

	if(dlg.DoModal()==IDOK)
	{
		//SetDescription(dlg.m_sDescript);
		T_THFC_BASE data;
		NewData();

		m_fScaleFactor=dlg.m_fScaleFactor;
		m_fTimeScale=dlg.m_fTimeScale;
		m_strEarthquake=dlg.m_strEarthquake;
		SetDescription(dlg.m_sDescript);
		
		if(!ReadEQData(dlg.m_fp))
		{	
			m_wndGrid.MakeItemEx();
			OnDataChanged();
			return ;
		}
		
		for(int i=0; i<m_arAccel.GetSize(); i++)
		{
			data.dblTime = m_arTime[i];
			data.dblValue = m_arAccel[i];
			m_ParamData.arThisFuncData.Add(data);
		}
		m_ParamData.nThisDataType = 1; // Normalized Accel.
		m_wndGrid.MakeItemEx();
		SetFunctionName(dlg.m_sFileTitle);
		OnDataChanged();
		ChangeDataType(0);

		SetEnableDygenButton();
	}
}

void CThfcItemDlg::InitChart()
{
	////////////////////////////////////////////////////////////////
	// Initialize m_Graph
	
	m_GraphView.SetDragable(FALSE); // set data-drag unable
	// Add Background component
	//
	SRGraphBackground	*pB=new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent((SRGraphComponent *) pB);
	
	// Add Display Component
	//

	SRGraphDisplay *pDisplay=new SRGraphDisplay;	
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX);
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	pDisplay->SetSizeDefaults();
	pDisplay->SetRect(0.2,0,100,97.0);
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(TRUE);
	
	
	// Logarithmic Axis Scaling
	//
	pDisplay->GetStyle()->SetLogX(m_LogFlagX);
	pDisplay->GetStyle()->SetLog(m_LogFlagY);

	//pDisplay->GetStyle()->SetShowXGrid(m_GridXFlag);
	//pDisplay->GetStyle()->SetShowYGrid(m_GridYFlag);
	

	m_Graph.AddComponent(pDisplay);

	// Add Label Component for X-Axis
	//
	SRGraphLabel	*pLabel=new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::TopCenter);
	pLabel->SetTextPosition(50.0, 93.5);
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Normal);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	m_Graph.AddComponent(pLabel);
	SetAxisLabelText(0,_LS(IDS_WG_CMD__ADD2__Time__sec_));
	
	// Add Label Component for Y-Axis
	//
	pLabel=new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::MidCenter);
	pLabel->SetTextPosition(1.5, 50.0);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Upward);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	//pLabel->GetStyle()->SetColor(CXCLR_BACKGRND);
	m_Graph.AddComponent(pLabel);
	SetAxisLabelText(1,_LS(IDS_WG_CMD__ADDD__Time_History_Data));
	

	
	

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

void CThfcItemDlg::SetValue(int nIndex, int nGroup, double x,double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
}

void CThfcItemDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CThfcItemDlg::OnETCTHFCCHARTXLog() 
{
		
	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLogX(m_btnXLog.GetCheck());
	
	if (m_btnXLog.GetCheck() == TRUE || m_btnYLog.GetCheck() == TRUE) 
	{
		pD->GetStyle()->SetScaleLimit(TRUE);
	  pD->SetMinRangeY(0.001);   
	  pD->SetMinRangeX(0.001);   
	}
	else 
		pD->GetStyle()->SetScaleLimit(FALSE);
	
	m_GraphView.Invalidate();	
}

void CThfcItemDlg::OnETCTHFCCHARTYLog() 
{
	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLog(m_btnYLog.GetCheck());

	if (m_btnXLog.GetCheck() == TRUE || m_btnYLog.GetCheck() == TRUE) 
	{
		pD->GetStyle()->SetScaleLimit(TRUE);
	  pD->SetMinRangeY(0.001);
		pD->SetMinRangeX(0.001);
	}
	else 
		pD->GetStyle()->SetScaleLimit(FALSE);

	m_GraphView.Invalidate();	

	
}

// CThfcItemDlg::ReadSGSData(FILE *fp)를 참고로 만듬. (TGS는 SGS를 참고로 만든 프로그램임.)
BOOL CThfcItemDlg::ReadTGSData(FILE *fp)
{
	char		yytext[120];
	CString		str, msg;
	int			nGroup=-2, xIndex, yIndex;
	CString  strTitle[2], sFactor;
	T_THFC_BASE data;
	
	NewData();
	
	// 번역하지 마시오
	CStringA csCmdSGSw = "TGSW";
	CStringA csCmdTitle = "TITLE";
	CStringA csCmdXAxis = "X-AXIS";
	CStringA csCmdYAxis = "Y-AXIS";
	CStringA csCmdFactor = "FACTOR";
	CStringA csCmdData = "DATA";
	
	CStringA csText;

	lex(fp, yytext);	// asterisk

	lex(fp, yytext);
	csText = yytext;
	csText.TrimRight();
	csText.MakeUpper();

	if (csText != csCmdSGSw) 
	{
		msg.Format(_LS(IDS_WG_CMD__ADDD__error__Invalid_Data_Format___));
		MessageBox(msg);
		return FALSE;
	}

	// Read Graph Title
	//
	int i=0,numOfTitles = 0;
	str.Empty();
	lex(fp, yytext);	// asterisk

	lex(fp, yytext);
	csText = yytext;
	csText.TrimRight();
	csText.MakeUpper();

	while (csText == csCmdTitle) 
	{
		lex(fp, yytext);		// comma
		lex(fp, yytext);

		if (strcmp(yytext, "*") != 0) 
		{
			str += yytext;
			
			lex(fp, yytext);
			while (strcmp(yytext, "*") != 0)   // for in-Title comma 
			{
				str = str +yytext+" ";
				lex(fp, yytext);
			}

			if(i<2)		    strTitle[i++]=str;
			str="";
		}

		lex(fp, yytext);
		csText = yytext;
		csText.TrimRight();
		csText.MakeUpper();
	}
	// End of Reading Graph Title

	// Read X-Axis Label
	if (csText == csCmdXAxis) {
		lex(fp, yytext);		// comma
		lex(fp, yytext);
		lex(fp, yytext);	// asterisk
	  lex(fp, yytext);
		csText = yytext;
		csText.TrimRight();
		csText.MakeUpper();
	}
	// Read Y-Axis Label
	//
	if (csText == csCmdYAxis) {
		lex(fp, yytext);		// comma
		lex(fp, yytext);
		lex(fp, yytext);	// asterisk
	  lex(fp, yytext);
		csText = yytext;
		csText.TrimRight();
		csText.MakeUpper();
	}
	
	////////////////////////////////////////////////////////////////////
	// UNIT & Type


	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX UnitIndexOld, UnitIndexNew;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
 
	UnitIndexNew=UnitIndexOld;
	// 2013-03-15. by Hsshim : TGS는 단위 변경이 없는 프로그램으로 고정단위로 강제 할당. (X: sec, Y: kN(Force))
	UnitIndexNew.nBase_Force=D_UNITSYS_FORCE_INDEX_KN;
	
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew); // 읽은 단위계에서 현재 단위계로
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld); // conversion 을 하기 위해서...
	////////////////////////////////////////////////////////////////


	if(csText == csCmdFactor)
	{
		lex(fp, yytext);		// comma
		lex(fp, yytext);    // Factor Data
		sFactor = yytext;
		lex(fp, yytext);		// *
		lex(fp, yytext);    // DATA 
	}
	// Read and Set Graph(Seismic) Data

	csText = yytext;
	csText.TrimRight();
	csText.MakeUpper();

	//	while (csText == csCmdData) 
	if(csText == csCmdData)
	// 첫번째 데이터만을 보여줄 것임 ...!! 
	{
		double	d;
		nGroup += 2;
		xIndex = yIndex = 0;
		

		// Read X-Axis Value
		lex(fp, yytext);
		while (strcmp(yytext, "*") != 0) 
	  {				
			// Set X-Axis Value
			if(nGroup == 0)   // only the first group of data can be imported.
				data.dblTime = atof(yytext);

			lex(fp, yytext);	// comma
			if(strcmp(yytext, "*") != 0) 
			{
			 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
			 fclose(fp);
			 return FALSE;
			}

			// Read Y-Axis Value
			lex(fp, yytext);
			if ((d=atof(yytext)) < 0.) 
			{
				if (m_btnYLog.GetCheck()) 
				{
					msg.Format(_LS(IDS_WG_CMD__ADDD__Warning__Negative_Value_Cannot_Be));
					MessageBox(msg);
					//((SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE))->GetStyle()->SetLog(FALSE);
					m_btnYLog.SetCheck(FALSE); //LogFlagY = FALSE;
				}
			}
			// Set Y-Axis Value
			if(nGroup == 0)   // only the first group of data can be imported.
				data.dblValue = UnitCvt(atof(yytext),D_UNITSYS_BASE_FORCE);

			// Read X-Axis Value
			lex(fp, yytext);
			if ((d=atof(yytext)) < 0.) 
			{
			  if (m_btnYLog.GetCheck()) 
				{
					msg.Format(_LS(IDS_WG_CMD__ADDD__Warning__Negative_Value_Cannot_Be_MSG4));
					((SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE))->GetStyle()->SetLogX(FALSE);
					m_btnYLog.SetCheck(FALSE);
				}
			}
		  m_ParamData.arThisFuncData.Add(data);
		}
	}

	
	fclose(fp);
	
	CString sDescript;
	sDescript.Format(_T("%s\n%s"),strTitle[0],strTitle[1]);
	SetDescription(sDescript);
		
	return TRUE;
}


BOOL CThfcItemDlg::ReadSGSData(FILE *fp)
{
	char		yytext[120];
	CString		str, msg;
	int			nGroup=-2, xIndex, yIndex,nNumOfDamping=1;
	BOOL bDampingLegend;
	CString  strTitle[2];
	T_THFC_BASE data;
	
	NewData();
	
	// 번역하지 마시오
	CStringA csCmdSGSw = "SGSW";
	CStringA csCmdTitle = "TITLE";
	CStringA csCmdXAxis = "X-AXIS";
	CStringA csCmdYAxis = "Y-AXIS";
	CStringA csCmdUnit = "UNIT&TYPE";
	CStringA csCmdFlag = "FLAGS";
	CStringA csCmdData = "DATA";
	CStringA csCmdDamping = "DAMPING";
	CStringA csCmdAccel = "ACCEL";
	CStringA csCmdVelo = "VELO";
	CStringA csCmdDisp = "DISP";
	CStringA csCmdGrav = "GRAV";
	CStringA csText;

	//SetAxisLabelText(0,"");
	//SetAxisLabelText(1,"");

	lex(fp, yytext);	// asterisk

	lex(fp, yytext);
	csText = yytext;
	csText.TrimRight();
	csText.MakeUpper();

	if (csText != csCmdSGSw) 
	{
		msg.Format(_LS(IDS_WG_CMD__ADDD__error__Invalid_Data_Format___));
		MessageBox(msg);
		return FALSE;
	}

	// Read Graph Title
	//
	int i=0,numOfTitles = 0;
	str.Empty();
	lex(fp, yytext);	// asterisk

	lex(fp, yytext);
	csText = yytext;
	csText.TrimRight();
	csText.MakeUpper();

	while (csText == csCmdTitle) {
		lex(fp, yytext);		// comma
		lex(fp, yytext);

		if (strcmp(yytext, "*") != 0) 
		{
			str += yytext;
			
			lex(fp, yytext);
			while (strcmp(yytext, "*") != 0)   // for in-Title comma 
			{
				str = str +yytext+" ";
				lex(fp, yytext);
			}

			if(i<2)		    strTitle[i++]=str;
			str="";
		}

		lex(fp, yytext);
		csText = yytext;
		csText.TrimRight();
		csText.MakeUpper();
	}
	// End of Reading Graph Title

	// Read X-Axis Label
	//
	if (csText == csCmdXAxis) {
		lex(fp, yytext);		// comma
		lex(fp, yytext);
		//SetAxisLabelText(0, yytext);
	  lex(fp, yytext);	// asterisk
	  lex(fp, yytext);
		csText = yytext;
		csText.TrimRight();
		csText.MakeUpper();
	}
	// Read Y-Axis Label
	//
	if (csText == csCmdYAxis) {
		lex(fp, yytext);		// comma
		lex(fp, yytext);
		//SetAxisLabelText(1, yytext);
		lex(fp, yytext);	// asterisk
	  lex(fp, yytext);
		csText = yytext;
		csText.TrimRight();
		csText.MakeUpper();
	}

	////////////////////////////////////////////////////////////////////
	// UNIT & Type

	// Unit
	CStringA sUnit,sType;
	int nUnitRead,nDataType;
	T_UNIT_INDEX UnitIndexCurrent;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexCurrent); 

	if (csText == csCmdUnit) {
		lex(fp, yytext);		// comma
		lex(fp, yytext);
		CStringA sUnit = yytext;
		sUnit.TrimRight();
		sUnit.MakeUpper();
		if(sUnit != csCmdGrav) 
		{
			if(sUnit == "MM" ) nUnitRead = 0;
			else if(sUnit == "CM" ) nUnitRead =1;
			else if(sUnit == "M" ) nUnitRead = 2;
			else if(sUnit == "INCH" || sUnit == "IN" || sUnit == "IN.") nUnitRead =3;
			else if(sUnit == "FEET" || sUnit == "FT" || sUnit == "FT.") nUnitRead =4;
			else 
			{
			 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_UNIT_type_));
			 fclose(fp);
			 return FALSE;
			}
		}
		else
		{
			nUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
			nDataType = 0; // normalized accel;
		}
				
		// Data type
		lex(fp, yytext);	// asterisk
	  lex(fp, yytext);
		sType = yytext;
		sType.TrimRight();
		sType.MakeUpper();
		if(sType == csCmdAccel && sUnit == csCmdGrav )       nDataType = 0; // normalized accel;
		else if( sType == csCmdAccel && sUnit != csCmdGrav ) nDataType = 1; // accel;
		else if( sType == csCmdVelo || sType == csCmdDisp )
		{
		   AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___This_is_VELO_or_DISP_type));
			 fclose(fp);
			 return FALSE;
		}
		else 
		{
			 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Data_Type_));
			 fclose(fp);
			 return FALSE;
		}
		lex(fp, yytext);	// asterisk

	  lex(fp, yytext);
		csText = yytext;
		csText.TrimRight();
		csText.MakeUpper();
	}
	else // 단위및 타입정보가 없으면 디폴트 설정...
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Data_Unit___Type_Information__));
		nUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
		nDataType = 0; // normalized accel;
	}
	ChangeDataType(nDataType);
	m_ParamData.nThisDataType= nDataType+1;

	//////////////////////////////////////////////////////////////
	T_UNIT_INDEX UnitIndexOld, UnitIndexNew;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
	UnitIndexNew=UnitIndexOld;
	UnitIndexNew.nBase_Length=nUnitRead;

	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew); // 읽은 단위계에서 현재 단위계로
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld); // conversion 을 하기 위해서...
	////////////////////////////////////////////////////////////////
	
	
	 
		
	// Read Graph Flag
	//
	if (csText == csCmdFlag) 
	{
		lex(fp, yytext);		// comma
		lex(fp, yytext);
		BOOL bTripartiteFlag = atof(yytext);
		if(bTripartiteFlag)
		{
			m_btnXLog.SetCheck(TRUE);
			m_btnYLog.SetCheck(TRUE);
			SRGraphDisplay	*pD=(SRGraphDisplay *)m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
			pD->GetStyle()->SetLogX(TRUE);
			pD->GetStyle()->SetLog(TRUE);
		}
				
		lex(fp, yytext);		// comma
		lex(fp, yytext);
		bDampingLegend = atof(yytext);
	}
	else
	{
	 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format__FLAGS_not_f));
	 fclose(fp);
	 return FALSE;
	}
	
	// Read and Set Graph(Seismic) Data
	//
	lex(fp, yytext);	// asterisk

	lex(fp, yytext);
	csText = yytext;
	csText.TrimRight();
	csText.MakeUpper();

	//	while (csText == csCmdData) 
	if(csText == csCmdData)
	// 첫번째 데이터만을 보여줄 것임 ...!! 
	{
		double	d;
		nGroup += 2;
		xIndex = yIndex = 0;
		
		if(bDampingLegend)
		{
			lex(fp, yytext);	// asterisk

			lex(fp, yytext);
			csText = yytext;
			csText.TrimRight();
			csText.MakeUpper();

		  if (csText == csCmdDamping) 
			{
				lex(fp, yytext);		// comma
				lex(fp, yytext);
				//m_fDamping[nGroup/2] = atof(yytext);
				nNumOfDamping=nGroup/2+1;
			}
		}


		// Read X-Axis Value
		lex(fp, yytext);
		while (strcmp(yytext, "*") != 0) 
	  {				
			// Set X-Axis Value
			if(nGroup == 0)   // only the first group of data can be imported.
				data.dblTime = atof(yytext);
			lex(fp, yytext);	// comma
			if(strcmp(yytext, ",") != 0) 
			{
			 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
			 fclose(fp);
			 return FALSE;
			}

			// Read Y-Axis Value
			lex(fp, yytext);
			if ((d=atof(yytext)) < 0.) 
			{
				if (m_btnYLog.GetCheck()) 
				{
					msg.Format(_LS(IDS_WG_CMD__ADDD__Warning__Negative_Value_Cannot_Be));
					MessageBox(msg);
					//((SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE))->GetStyle()->SetLog(FALSE);
					m_btnYLog.SetCheck(FALSE); //LogFlagY = FALSE;
				}
			}
			// Set Y-Axis Value
			if(nGroup == 0)   // only the first group of data can be imported.
				data.dblValue = UnitCvt(atof(yytext));
			
			// Read X-Axis Value
			lex(fp, yytext);
			if ((d=atof(yytext)) < 0.) 
			{
			  if (m_btnYLog.GetCheck()) 
				{
					msg.Format(_LS(IDS_WG_CMD__ADDD__Warning__Negative_Value_Cannot_Be_MSG4));
					((SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE))->GetStyle()->SetLogX(FALSE);
					m_btnYLog.SetCheck(FALSE);
				}
			}
		  m_ParamData.arThisFuncData.Add(data);
		}
	  lex(fp, yytext);
		csText = yytext;
		csText.TrimRight();
		csText.MakeUpper();
	}

	if (nNumOfDamping > 1) MessageBox(_LS(IDS_WG_CMD__ADDD__Warning___There_are_more_than_one));
	fclose(fp);
	
	CString sDescript;
	sDescript.Format(_T("%s\n%s"),strTitle[0],strTitle[1]);
	SetDescription(sDescript);
		
	return TRUE;

}

BOOL CThfcItemDlg::ReadTHDData(FILE *fp) // Time History Data File
{
	char		yytext[80];

	T_THFC_BASE data;
	NewData();

	// 번역하지 마시오
	CStringA csCmdGrav = "GRAV";
	CStringA csCmdUnit = "UNIT";
	CStringA csCmdType = "TYPE";
	CStringA csCmdAccel = "ACCEL";
	CStringA csCmdForce = "FORCE";
	CStringA csCmdMoment = "MOMENT";
	CStringA csCmdData = "DATA";

	// Read and Set Time history data
	//
	try
	{    
		////////////////////////////////////////////////////////////////////
		// UNIT & Type

		// Unit
		CStringA sUnit,sType, csText;
		int nLenUnitRead,nFrcUnitRead,nDataType,nUnitType;
		T_UNIT_INDEX UnitIndexCurrent;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexCurrent); 

		lex(fp, yytext); // asterisk
		lex(fp, yytext);
		csText = yytext;
		csText.TrimRight();
		csText.MakeUpper();

		sUnit = _T("");
		
		if (csText == csCmdUnit) 
		{		  
			///////////////////////////////////////////////////////////////////////////////////
			// Length Unit
			lex(fp, yytext);		// comma
		  lex(fp, yytext);
		  sUnit = yytext;
			sUnit.TrimRight();
			sUnit.MakeUpper();
			if(sUnit != csCmdGrav) 
			{
				if(sUnit == "MM" ) nLenUnitRead = D_UNITSYS_LENGTH_INDEX_MM;
				else if(sUnit == "CM" ) nLenUnitRead =D_UNITSYS_LENGTH_INDEX_CM;
				else if(sUnit == "M" ) nLenUnitRead = D_UNITSYS_LENGTH_INDEX_M;
				else if(sUnit == "INCH" || sUnit == "IN" || sUnit == "IN.") nLenUnitRead =D_UNITSYS_LENGTH_INDEX_IN;
				else if(sUnit == "FEET" || sUnit == "FT" || sUnit == "FT.") nLenUnitRead =D_UNITSYS_LENGTH_INDEX_FT;
				else if(sUnit == "NULL") 	nLenUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
				else 
				{
				 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Length_Unit_Type_));
				 fclose(fp);
				 return FALSE;
				}
			}
			else
			{
				nLenUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
				nDataType = 0; // normalized accel;
			}
			
			///////////////////////////////////////////////////////////////////////////////////
			// Force Unit
			lex(fp, yytext);		// comma
		  lex(fp, yytext);
		  CString sFrcUnit = yytext;
			sFrcUnit.TrimRight(); 
			sFrcUnit.MakeUpper();

			if(sFrcUnit == "KG" || sFrcUnit == "KGF") nFrcUnitRead = D_UNITSYS_FORCE_INDEX_KG;
			else if(sFrcUnit == "TON" || sFrcUnit == "TONF") nFrcUnitRead =D_UNITSYS_FORCE_INDEX_TON;
			else if(sFrcUnit == "N" ) nFrcUnitRead = D_UNITSYS_FORCE_INDEX_N;
			else if(sFrcUnit == "KN" ) nFrcUnitRead =D_UNITSYS_FORCE_INDEX_KN;
			else if(sFrcUnit == "LBF" ) nFrcUnitRead =D_UNITSYS_FORCE_INDEX_LBF;
			else if(sFrcUnit == "KIP") nFrcUnitRead = D_UNITSYS_FORCE_INDEX_KIP;
			else if(sFrcUnit == "NULL") 	nFrcUnitRead= UnitIndexCurrent.nBase_Force; // 단위변환 없음.
			else 
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Force_unit_type_));
				fclose(fp);
				return FALSE;
			}

			lex(fp, yytext); // asterisk
			lex(fp, yytext);
			csText = yytext;
			csText.TrimRight();
			csText.MakeUpper();
		}
		else
		{
		  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Data_Unit_Information___nReadi));
			nLenUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
			nFrcUnitRead= UnitIndexCurrent.nBase_Force; // 단위변환 없음.
		}

		if (csText == csCmdType) 
		{    
			///////////////////////////////////////////////////////////////////////////////////
			// Data type
			lex(fp, yytext);	// comma
		lex(fp, yytext);
			sType = yytext;
			sType.TrimRight();    
			sType.MakeUpper();

			if(sType == csCmdAccel && sUnit == csCmdGrav )       
			{ nDataType = 0; // normalized accel;
				nUnitType = D_UNITSYS_NONE;
			}
			else if( sType == csCmdAccel && sUnit != csCmdGrav) 
			{nDataType = 1; // accel;
			 nUnitType = D_UNITSYS_BASE_LENGTH;
			}
			else if( sType == csCmdForce )                   
			{	nDataType = 2; // force
			  nUnitType = D_UNITSYS_BASE_FORCE;
			}
			else if( sType == csCmdMoment )                  
			{	nDataType = 3; // moment
				nUnitType = D_UNITSYS_BASE_MOMENT;
			}
			else 
			{
				 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Data_Type_));
				 fclose(fp);
				 return FALSE;
			}

			lex(fp, yytext); // asterisk
			lex(fp, yytext);
			csText = yytext;
			csText.TrimRight();
			csText.MakeUpper();	  
		}
		else // 단위및 타입정보가 없으면 디폴트 설정...
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Data_Type_Information___nReadi));
			nDataType = 0; // normalized accel;
			nUnitType = D_UNITSYS_NONE;
		}
		
		ChangeDataType(nDataType);
		m_ParamData.nThisDataType= nDataType+1;

		//////////////////////////////////////////////////////////////
		T_UNIT_INDEX UnitIndexOld, UnitIndexNew;
	  pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
	  UnitIndexNew=UnitIndexOld;
	  UnitIndexNew.nBase_Length=nLenUnitRead;
	  UnitIndexNew.nBase_Force=nFrcUnitRead;

		pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew); // 읽은 단위계에서 현재 단위계로
	  pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld); // conversion 을 하기 위해서...
		////////////////////////////////////////////////////////////////
	
	  if(csText == csCmdData) 
		{
			while (lex(fp,yytext) == 1) // Read X-Axis Value
			{	
			 
				// Set X-Axis Value
				data.dblTime = atof(yytext);
				lex(fp, yytext);	// comma
				if(strcmp(yytext, ",") != 0) 
				{
				 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
				 fclose(fp);
				 return FALSE;
				}
				
				lex(fp, yytext);
				
				// Set Y-Axis Value
				data.dblValue = UnitCvt(atof(yytext),nUnitType);
				m_ParamData.arThisFuncData.Add(data);
				
			}
		}
		else
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Data_or_Wrong_Format_));
			fclose(fp);
			return FALSE;
		}

	}
	catch (CFileException* e)  /// useless because it was not CFile;
	{
		e;  // not used
		fclose(fp);
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occured_while_reading_da));
		return FALSE;
	}

	fclose(fp);
	return TRUE;
}

BOOL CThfcItemDlg::ReadAT2Data(FILE *fp)
{
    char yytext[120];
        
    NewData();

    // 번역하지 마시오
    const CString& csCmdDT = "DT=";
    try
    {    
        bool bFinish = true;
        CString csText = _T("");
        while ( bFinish )
        {
            lex(fp, yytext);
            csText = yytext;
            if ( csText.Find(csCmdDT) > -1 )
            {
                bFinish = false;
            }
        }

        csText.Trim(_T("DT="));
        csText.Trim(_T("SEC"));
        csText.Trim();
        const double dTime = _tstof(csText);

        lex(fp, yytext); /// last comma.
        
        T_THFC_BASE ThfcBase;
        int nCount = 0;

        while ( lex(fp, yytext) > -1 )
        {
            csText = yytext;            
            
            CString token;
            int pos = 0;
			while ((token = csText.Tokenize(_T(" "), pos)) != _T(""))
            {
				token.Trim();  // 공백 제거
				// token 처리 코드
				nCount++;
				ThfcBase.dblTime = dTime * nCount;
				ThfcBase.dblValue = _ttof(token);
				m_ParamData.arThisFuncData.Add(ThfcBase);
			}
        }

        if ( bFinish )
        {
            fclose(fp);
            AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occured_while_reading_da));
            return FALSE;
        }        
    }
    catch ( CFileException* e )  /// useless because it was not CFile;
    {
        e;  // not used
        fclose(fp);
        AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occured_while_reading_da));
        return FALSE;
    }

    fclose(fp);
    return TRUE;
}

int CThfcItemDlg::lex(FILE *fp, char *yytext)
{
	int		c, index;

	index	= 0;
	c		= getc(fp);

	// Discard white characters
	while ((c==' ') || (c=='\r') || (c=='\t') || (c=='\n')) {
		c = getc(fp);
	}

	if (c == EOF ) return -1;
	
	// Handle(Discard) Comments
	while (c == '*') {
		c = getc(fp);
		if (c == '*') {
			c = getc(fp);
			while (c != '\n')
				c = getc(fp);
			c = getc(fp);
		}
		else {
			ungetc(c, fp);
			yytext[index++] = '*';
			yytext[index] = '\0';
			return 1;
		}
	}

	switch (c) {
		case ',':
			yytext[index++] = c;
			yytext[index] = '\0';
			break ;
		default:
			while ((c!='*') && (c!=',') && (c!='\r') && (c!='\n')) {
				yytext[index++] = c;
				c = getc(fp);
				if (c == EOF || index >=120)
					return -1;
			}
			yytext[index] = '\0';
			if ((c=='*') || (c==','))
				ungetc(c, fp);
	}
	
	return 1;
}

double CThfcItemDlg::UnitCvt(double dReadData,int nUnitType)
{ 
	// default nUnitType = D_UNITSYS_BASE_LENGTH;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dRetValue =pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitType,dReadData);
	return  dRetValue;

}

void CThfcItemDlg::NewData()
{
	m_ParamData.arThisFuncData.RemoveAll();
	m_Graph.KillData();
	m_btnXLog.SetCheck(FALSE);
	m_btnYLog.SetCheck(FALSE);
	OnETCTHFCCHARTYLog();
	OnETCTHFCCHARTXLog();
}

void CThfcItemDlg::OnThfcSinRedrawGraph() 
{
	// TODO: Add your control notification handler code here
	CArray<T_THFC_BASE, T_THFC_BASE&> aThisFuncData;

	if (!CalcParamDataFromSinu(aThisFuncData)) return;

	T_THFC_BASE data;
	int nSize = aThisFuncData.GetSize();
	m_Graph.KillData();	
	for(int i=0; i<nSize; i++)
	{
		data = aThisFuncData[i];
		SetValue(i,0,data.dblTime,data.dblValue);
	}
	m_GraphView.Invalidate();

	m_btnFFT.SetCheck(0);
}

void CThfcItemDlg::OnThfcGrCheck() 
{
	BOOL bEnable;
	if (m_wndDrawCheck.GetCheck() == 0) bEnable = TRUE;
	else bEnable = FALSE;
	CDlgUtil::CtrlEnableDisable(this, m_aParamCtrl, bEnable);
}


BOOL CThfcItemDlg::ReadEQData(FILE *fp)
{

	char		yytext[120];
	double		GravFactor;

	int nNumOfData=0;
	

	GravFactor=1.0;		// EQRecord view --> unitless normalized acceleration 
	
	// Read and Set EQData
	//
	try
	{
	  while (lex(fp,yytext) == 1) // Read X-Axis Value
	  {	
		  
		  // Set X-Axis Value
		  m_arTime.Add(atof(yytext)*m_fTimeScale);
			
		  if(nNumOfData>=3) 
		  { 
			m_dSampleTime = m_arTime[2] - m_arTime[1]; // 샘플의 시간간격을 찾기 위함 
			
		  }
			  
		  lex(fp, yytext);	// comma
		  if(strcmp(yytext, ",") != 0) 
		  {
		   AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
		   fclose(fp);
		   return FALSE;
		  }
			  
			  if(lex(fp, yytext)==-1)  // data end, the last y data;
			  {
			  // Set Y-Axis Value
			  m_arAccel.Add(atof(yytext)*m_fScaleFactor*GravFactor);
			  nNumOfData++;
			  break;
			  }

			  // Set Y-Axis Value
			  m_arAccel.Add(atof(yytext)*m_fScaleFactor*GravFactor);
			  nNumOfData++;
			  
	  }
	  if(nNumOfData < 1) 
	  {
		  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Data_Found_));
		  fclose(fp);
		  return FALSE;
	  }
	fclose(fp);
	
	}
	
	catch (CFileException e[])
	{
		e->ReportError();
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occured_while_reading_da));
		return FALSE;
	}

	return TRUE;
 
}

//2013-06-14 Sjhuh - 데이터 생성 시 Dygen Button Enable 기능 구현. (기존에는 생성 후 다시 불러와야 Enable 됬음.)
void CThfcItemDlg::SetEnableDygenButton()
{
	if(m_ParamData.arThisFuncData.GetSize() != 0)
	{
		CWnd* pItem = GetDlgItem(IDC_CMD_DYGEN);
		pItem->EnableWindow(TRUE);
	}
}

void CThfcItemDlg::SetFunctionName(CString FuncName)
{
	CWnd* pWnd = GetDlgItem(IDC_THFC_FUNC_NAME);
	pWnd->SetWindowText(FuncName);
}

void CThfcItemDlg::OnThfcDesignHeeldrop() 
{
	CHeelLoadMainDlg dlg(this);
	
	dlg.m_parAccel = &m_arAccel;
	dlg.m_parPeriod = &m_arTime;
	m_arAccel.RemoveAll();
	m_arTime.RemoveAll();

	if(dlg.DoModal()==IDOK)
	{
		T_THFC_BASE data;
		NewData();
		
		for(int i=0; i<m_arAccel.GetSize(); i++)
		{
			data.dblTime = m_arTime[i];
			data.dblValue = m_arAccel[i];
			m_ParamData.arThisFuncData.Add(data);
		}
		if (dlg.m_nCurrentDlg == 24 || dlg.m_nCurrentDlg == 25)
			m_ParamData.nThisDataType = 5;
		else
			m_ParamData.nThisDataType = 3; // Force
		m_wndGrid.MakeItemEx();
		SetFunctionName(dlg.m_strFuncName);
		OnDataChanged();
		if (dlg.m_nCurrentDlg == 24 || dlg.m_nCurrentDlg == 25)
			ChangeDataType(4);
		else
			ChangeDataType(2);

		SetEnableDygenButton();
	}
}


void CThfcItemDlg::OnApply() 
{
	if (!UpdateData(TRUE)) return;
	if (this->m_bSinusoidal)
	{
		CArray<T_THFC_BASE, T_THFC_BASE&> aThisFuncData;
		if (!CalcParamDataFromSinu(aThisFuncData)) return;

		m_ParamData.arThisFuncData.RemoveAll();
		int nSize = aThisFuncData.GetSize();
		for(int i=0; i<nSize; i++)
		{
			m_ParamData.arThisFuncData.Add(aThisFuncData[i]);
		}
	}


	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess;

	if (m_bSetParam)
		bSuccess = pDoc->m_pDataCtrl->ModifyThfc(m_csOldName, m_ParamData);
	else
		bSuccess = pDoc->m_pDataCtrl->AddThfc(m_ParamData);

	if (!bSuccess) return;
		
	NewData();
	SetFunctionName(_T(""));
	SetDescription(_T(""));
	OnDataChanged();
	m_wndGrid.MakeItemEx();


}

void CThfcItemDlg::SetDescription(CString Descript)
{
	CWnd* descript = GetDlgItem(IDC_CMD_ETC_DESCRIPT);
	descript->SetWindowText(Descript);
	m_csDescription = Descript;
}

void CThfcItemDlg::OnChangeScaleRdo()
{
	UpdateData();
	BOOL bScale = TRUE;
	if (m_nScaleType == 1)
		bScale = FALSE;
	if (!m_bSinusoidal)
	{
		GetDlgItem(IDC_THFC_SCALE)->EnableWindow(bScale);
		GetDlgItem(IDC_THFC_SCALE2)->EnableWindow(!bScale);
	}
}

void CThfcItemDlg::SetFFT()
{
	CArray<double,double> arXVal;
	CArray<double,double> arYVal;
	CArray<T_THFC_BASE, T_THFC_BASE&> aThisFuncData;
	CArray<T_THFC_BASE, T_THFC_BASE&> *pThfcList = NULL;
	
	if(m_bSinusoidal)
	{
		if (!CalcParamDataFromSinu(aThisFuncData)) return;
		pThfcList = &aThisFuncData;
	}
	else
	{
		pThfcList = &(m_ParamData.arThisFuncData);
	}
	if(!pThfcList){ ASSERT(0); return; }

	arXVal.SetSize(pThfcList->GetSize());
	arYVal.SetSize(pThfcList->GetSize());
	for(int idx=0; idx<pThfcList->GetSize(); idx++)
	{
		arXVal[idx] = pThfcList->GetAt(idx).dblTime;
		arYVal[idx] = pThfcList->GetAt(idx).dblValue;
	}
	
	int ColCount=0;
	double *RealIn,*RealOut,*ImagOut;
	double value;
	double base_Freq;
	int x,pow2;
	double fMax=0;
	CFFT fft;
	double dSampleTime;
		
	// Add handler to update graph
	int x1    = arXVal.GetSize();
	
	if(x1<2)
	{
		return;
	}
	
	if ( x1 & (x1-1) )	 // Index num is not power of two; so proceding modification;
	{
		pow2 = log(x1)/log(2); // MNET:2661 20070302 JHKIM
		x = pow(2,pow2);
		
		RealIn  = new double[x];
		RealOut = new double[x];
		ImagOut = new double[x];
		for (int i=x1; i< x; i++)RealIn[i]=0;
	}
	else 
	{
		x=x1;
		RealIn  = new double[x];
		RealOut = new double[x];
		ImagOut = new double[x];
	}
	
	for(int i=0; i<x; ++i) RealIn[i] = arYVal[i];

	dSampleTime =arXVal[1]-arXVal[0];
	
	fft.DoFFT(0,x,RealIn,NULL,RealOut,ImagOut);
	base_Freq = 1/ (dSampleTime)/x ;	// m_dSampleTime : 샘플의 시간간격 
	
	for(int index=1; index<= x/2; index++)    // for normalizaion searching max
	{	
		value = sqrt(pow(RealOut[index],2.0) + pow(ImagOut[index],2.0)) * dSampleTime; // MNET:2661 20070222 JHKIM
		if(fMax<=value)fMax   = value ;
	}
	
	SetAxisLabelText(0,_LS(IDS_CMD_THFC_ITEM_FREQUENCY)); // _T("Frequency(Hz)")
	SetAxisLabelText(1,_LS(IDS_CMD_THFC_ITEM_AMPLITUDE_VALUE)); // _T("Amplitude value")
	m_Graph.KillData();	
	if (fMax ==0.0 ) fMax = 1.0; // prevent from being divided by zero.
	for(int index=1; index<=x/2; index++)    // view the manual
	{	
		value = sqrt(pow(RealOut[index],2.0) + pow(ImagOut[index],2.0)) * dSampleTime; // MNET:2661 20070222 JHKIM
		
		SetValue(index-1,0,base_Freq*index,value);
	}
	
	delete []RealIn;
	delete []RealOut;
	delete []ImagOut;
	m_GraphView.Invalidate();
}

void CThfcItemDlg::OnEtcThfcChartFft() 
{
	if(m_btnFFT.GetCheck() == TRUE)
	{
		SetFFT(); 
	}
	else 
	{
		if(m_bSinusoidal) OnThfcSinRedrawGraph();
		else              OnDataChanged();
	}
}

void CThfcItemDlg::OnResponseSpectrum()
{
	if (m_ParamData.arThisFuncData.GetSize() < 3)
	{
		//AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_));
		AfxMessageBox(_LS(IDS_CMD_THFC_DATA_NUM));  //  "[오류] 데이터의 개수는 3개 이상이어야 합니다.
		return;
	}

	// MNET:XXXX-GSJI-20061221 : OnCmdGenerateBtn() 수행시 제약사항
	if (m_ParamData.arThisFuncData.GetSize() > 18000)
	{
		AfxMessageBox(_LS(IDS_CMD_THFC_DATA_NUM_MAX)); // [오류] 데이터의 개수 18000개 이하인 경우만 기능을 지원합니다.
		return;
	}
	
	CArray<UINT, UINT> aDataType;
	aDataType.Add(IDC_THFC_TYPE_NORMAL);
	aDataType.Add(IDC_THFC_TYPE_ACC);
	aDataType.Add(IDC_THFC_TYPE_FORCE);
	aDataType.Add(IDC_THFC_TYPE_MOMENT);
	aDataType.Add(IDC_THFC_TYPE_NORMAL2);
	int nDataType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, aDataType, nDataType);

	CCMThfcRpsrDlg dlg;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	UINT Key = pDoc->m_pAttrCtrl->GetThfcKey(m_ParamData.ThisFuncName);
	dlg.SetEQData(m_ParamData.arThisFuncData, nDataType, m_wndGravity.GetEditValue(), Key);
	dlg.DoModal();
}

void CThfcItemDlg::AutoBtnDyGen()
{
	T_THFC_K Key;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//Key = pDoc->m_pAttrCtrl->GetThfcKey(m_ParamData.ThisFuncName);
	Key = m_ParamData.nThisFuncId;
	bool bExist = false;
	T_UCHT_K UchtK;
	T_UCHT_D UchtD;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartUcht();
	while (pos)
	{
		 pDoc->m_pAttrCtrl->GetNextUcht(pos,UchtK,UchtD);
		 if (UchtD.nType == D_TYPE_TH_FUNC && UchtD.nKey == Key)
		 {
			 bExist = true;
			 break;
		 }
	}
	//if exist;no add
	if (!bExist)
	{
		UchtD.Initialize(D_TYPE_TH_FUNC);
		UchtD.nKey = Key;
		T_THFC_D ThfcD;
		pDoc->m_pAttrCtrl->GetThfc(Key, ThfcD);
		CString strUchtName = ThfcD.ThisFuncName;
		int nCount = 1;
		while (pDoc->m_pAttrCtrl->ExistUcht(strUchtName))
		{
			strUchtName.Format(_T("%s%d"), ThfcD.ThisFuncName, nCount++);
		}
		UchtD.strName = strUchtName;
		//dlg.SetNamePtr(&csName);
		//UchtD.strName = csName;
		//UchtD.bLogX = TRUE;
		//UchtD.bLogY = TRUE;
		//UchtD.nFuncType = TRUE;
		if (!pDoc->m_pDataCtrl->AddUcht(UchtD)) ASSERT(0);
	}
}
