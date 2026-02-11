// SpfcItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SpfcItemDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\DBLib.h"

#include "ETC_DESIGN_SPECT.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"

#include <float.h>
#include "..\wg_base\ServiceProvider.h"
#include "..\wg_db\ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpfcItemDlg dialog


CSpfcItemDlg::CSpfcItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSpfcItemDlg::IDD, pParent)
{
	m_bSeismicEvaluation = FALSE;
	m_bSetParam = FALSE;
	m_bGridInit = FALSE;
	m_bShowReadOnly = FALSE;

	//{{AFX_DATA_INIT(CSpfcItemDlg)
	m_csFuncName = _LS(IDS_WG_CMD__ADDD__Spectrum);
	m_csDescription = _T("");
	m_nDataType = -1;
	m_nScaleType = 0;
	//}}AFX_DATA_INIT

	memset(m_dMnMxXVal,0,sizeof(m_dMnMxXVal));
	memset(m_dMnMxYVal,0,sizeof(m_dMnMxYVal));
	m_ParamData.Initialize();
	m_bInternal = false;
	m_nSpfcKey = NULL;
}

void CSpfcItemDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CSpfcItemDlg::UpdateBuffer()
{
	CWaitCursor wait;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SPFC_D Data;
	T_SPFC_K SpfcK;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case UR_SPFC_ADD:
		case UR_SPFC_MFS:
		case UR_SPFC_MFD:
		{
			if (!m_bInternal /*&& nKey == m_nSpfcKey*/) {
				pDoc->m_pViewBuff->GetSpfc(nKey, SpfcK, Data);
				m_csFuncName = Data.SpecFuncName;
				SetFuncName(Data.SpecFuncName);
				SetParamData(Data);
				m_wndGrid.MakeItemEx();
				OnDataChanged();
				UpdateData(FALSE);
			}


			//bDataChanged = TRUE;
		}
		break;
		default:
			break;
		}
	}
}
void CSpfcItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpfcItemDlg)
	DDX_Control(pDX, IDC_SPFC_SCALE, m_wndScaleFactor);
	DDX_Control(pDX, IDC_SPFC_GRAVITY, m_wndGravity);
	DDX_Control(pDX, IDC_SPFC_GRAVITY_UNIT, m_wndGravityUnit);
	DDX_Control(pDX, IDC_ETC_SPFC_CHART_YLog, m_btnYLog);
	DDX_Control(pDX, IDC_ETC_SPFC_CHART_XLog, m_btnXLog);
	DDX_Text(pDX, IDC_SPFC_FUNC_NAME, m_csFuncName);
	DDX_Text(pDX, IDC_CMD_ETC_DESCRIPT, m_csDescription);
	DDX_Text(pDX, IDC_CMD_DAMPING_EDT, m_dDamping);
	DDX_Radio(pDX, IDC_SPFC_TYPE_NORMAL, m_nDataType);
	DDX_Radio(pDX, IDC_SPFC_SCALE_RDO1, m_nScaleType);
	DDX_Control(pDX, IDC_SPFC_SCALE_UNIT, m_wndScaleUnit);
	DDX_Control(pDX, IDC_SPFC_SCALE2, m_wndScaleMaxVal);
	DDX_Control(pDX, IDC_CMD_APPLY, m_bAPPLY);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Control(pDX, IDC_SPFC_TABLE, m_wndGrid);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		m_ParamData.SpecFuncName = m_csFuncName;
		m_ParamData.nSpecFuncType = m_nDataType + 1;
		m_ParamData.Description = m_csDescription;

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

		m_ParamData.Gravity = m_wndGravity.GetEditValue();
		m_ParamData.dDampingRatio = m_dDamping;
	}
	else
	{
		if (!m_bGridInit) return;
		BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
		ROWCOL nCount = m_ParamData.arSpecFuncData.GetSize();
		m_wndGrid.SetRowCount(nCount+1);
		m_wndGrid.SetCurrentCell(nCount+1, 1);

		m_wndGrid.LockUpdate(bOldLock);
		if (!bOldLock) m_wndGrid.Redraw();
	}
}

T_SPFC_D& CSpfcItemDlg::GetParamData()
{
	return m_ParamData;
}

void CSpfcItemDlg::SetParamData(T_SPFC_D& data)
{
	m_bSetParam = TRUE;
	m_bModify4SpecCode = TRUE;  // MNET:2138-GSJI-20060601

	m_ParamData = data;    
	m_csOldName = m_ParamData.SpecFuncName;
}

void CSpfcItemDlg::OnBtnDyGen()
{
	UpdateData(TRUE);

	T_SPFC_K Key;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	Key = pDoc->m_pAttrCtrl->GetSpfcKey(m_ParamData.SpecFuncName);

	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_RS_FUNC;  // RS Func.
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = Key;

	CString csName; csName.Empty();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if (nCount == 0) csName = _LS(IDS_CMD_DYNA_RPT_RSFUNC_NAME); //_T("Response Spectrum function");
	else csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_RSFUNC_NAME), nCount + 1);

	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if(dlg.DoModal() == IDOK)
	{
		UchtD.strName = csName;

		UchtD.bLogX = m_btnXLog.GetCheck();
		UchtD.bLogY = m_btnYLog.GetCheck();
		if(!pDoc->m_pDataCtrl->AddUcht(UchtD)) { ASSERT(0); return; }
	}

}

void CSpfcItemDlg::InitParam()
{
	m_ParamData.Initialize();
	m_ParamData.ScaleFactor = 1.0;
	m_ParamData.Gravity = CDBDoc::GetDocPoint()->m_pInitCtrl->DefaultGravity();  
}

void CSpfcItemDlg::InitGrid()
{
	m_wndGrid.Initialize(&m_ParamData,m_bShowReadOnly);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	m_bGridInit = TRUE;
}

void CSpfcItemDlg::EnableGravity(BOOL bEnable)
{
	CWnd* pWnd = GetDlgItem(IDC_SPFC_GRAVITY);
	//pWnd->EnableWindow(bEnable);
	if(m_bShowReadOnly==TRUE)
		pWnd->EnableWindow(FALSE);
	else
		pWnd->EnableWindow(bEnable);
}

void CSpfcItemDlg::ChangeDataType(int nType)
{ 
	if (m_nDataType == nType) return;

	UINT aID[] = { 
		IDC_SPFC_TYPE_NORMAL, IDC_SPFC_TYPE_ACC, 
		IDC_SPFC_TYPE_VEL, IDC_SPFC_TYPE_DISP,
	};	

	// Turn off old radio
	CButton* pRadio = (CButton*)GetDlgItem(aID[m_nDataType]);
	pRadio->SetCheck(0);

	// Turn on new radio
	m_nDataType = nType;
	pRadio = (CButton*)GetDlgItem(aID[m_nDataType]);
	pRadio->SetCheck(1);

	EnableGravity(m_nDataType == 0);
	T_UNIT_SYSTEM us;
	m_ParamData.nSpecFuncType = m_nDataType+1;
	m_wndGrid.SetHeaderTitle(us, FALSE);
}

BEGIN_MESSAGE_MAP(CSpfcItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSpfcItemDlg)
	ON_BN_CLICKED(IDC_SPFC_IMPORT_FILE, OnSpfcImportFile)
	ON_BN_CLICKED(IDC_SPFC_SEISMIC_GEN, OnSpfcSeismicGen)
	ON_BN_CLICKED(IDC_SPFC_TYPE_ACC, OnChangeDataType)
	ON_BN_CLICKED(IDC_SPFC_DESIGN_SPECTRUM, OnSpfcDesignSpectrum)
	ON_BN_CLICKED(IDC_ETC_SPFC_CHART_XLog, OnETCSPFCCHARTXLog)
	ON_BN_CLICKED(IDC_ETC_SPFC_CHART_YLog, OnETCSPFCCHARTYLog)
	ON_BN_CLICKED(IDC_SPFC_TYPE_VEL, OnChangeDataType)
	ON_BN_CLICKED(IDC_SPFC_TYPE_DISP, OnChangeDataType)
	ON_BN_CLICKED(IDC_SPFC_TYPE_NORMAL, OnChangeDataType)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnApply)
	ON_BN_CLICKED(IDC_SPFC_SCALE_RDO1, OnChangeScaleRdo)
	ON_BN_CLICKED(IDC_SPFC_SCALE_RDO2, OnChangeScaleRdo)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpfcItemDlg message handlers

BOOL CSpfcItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	if(m_bShowReadOnly==TRUE)
		MakeControlToReadOnly();
	
	// TODO: Add extra initialization here
	m_bTempSetParam = m_bSetParam ;  
	m_bModify4SpecCode = m_bSetParam;
	if (!m_bSetParam) 
	{
		InitParam();
	}
	else
		m_bAPPLY.EnableWindow(FALSE);

	m_csFuncName = m_ParamData.SpecFuncName;
	m_nDataType = (m_ParamData.nSpecFuncType == 0) ? 0 : m_ParamData.nSpecFuncType-1;
	m_dDamping = m_ParamData.dDampingRatio ;

	m_nScaleType = m_ParamData.nScaleMethod;
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

	m_csDescription = m_ParamData.Description;
	CString csGravity;
	csGravity.Format(_T("%g"), m_ParamData.Gravity);
	m_wndGravity.SetWindowText(csGravity);
	m_wndGravity.SetUnitType(CUnitCtrl::m_SPFC_UNIT.Gravity);
	m_wndGravityUnit.SetUnitType(CUnitCtrl::m_SPFC_UNIT.Gravity);

	InitGrid();
	InitChart();

	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;
#if defined(_CIVIL)
	if (CProduct::IsMovingType(D_PRODUCT_MOVING_JP))
	{
		m_LogFlagX = TRUE;
		m_LogFlagY = TRUE;
		m_btnXLog.SetCheck(TRUE);
		m_btnYLog.SetCheck(TRUE);
	}
#endif

	m_wndGrid.MakeItemEx();

	UpdateData(FALSE);

	EnableGravity(m_nDataType == 0);
	OnDataChanged();
	OnChangeDataType();
	OnChangeScaleRdo();
	OnETCSPFCCHARTXLog();
	OnETCSPFCCHARTYLog();	

	T_SPFC_K Key;
	CDBDoc* m_pDoc = CDBDoc::GetDocPoint();
	Key = m_pDoc->m_pAttrCtrl->GetSpfcKey(m_ParamData.SpecFuncName);
	BOOL bEnable;
	if(Key > 0) bEnable = TRUE;
	else bEnable = FALSE;
	if(m_bShowReadOnly) bEnable = FALSE;

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	GetDlgItem(IDC_CMD_DYGEN)->EnableWindow(bEnable);

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSpfcItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE)) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess;

	m_bInternal = true;
	if (m_bSetParam)
		bSuccess = pDoc->m_pDataCtrl->ModifySpfc(m_csOldName, m_ParamData);
	else  
		bSuccess = pDoc->m_pDataCtrl->AddSpfc(m_ParamData);

	m_bInternal = false;
	if (!bSuccess) return;  

	CDialogMove::OnOK();
}

void CSpfcItemDlg::OnSpfcImportFile() 
{
	CString strFilter;
#if defined(_MGEN_US)
	strFilter.Format(_T("%s%s"), _LS(IDS_CMD_EXCEL_XLSM_FILE_FILTER), _LS(IDS_WG_CMD__ADDD__SGS_or_SPD_File____sgs____spd____));
#else
	strFilter = _LS(IDS_WG_CMD__ADDD__SGS_or_SPD_File____sgs____spd____);
#endif	

	TCHAR * szFilter = (TCHAR*)strFilter.GetBuffer();
	FILE* fp;
	CFileDialog	fd(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);

	if (fd.DoModal() == IDOK) 
	{
		if (fd.GetFileExt() == _T("sgs") || fd.GetFileExt() == _T("SGS") || 
			fd.GetFileExt() == _T("spd") || fd.GetFileExt() == _T("SPD") )
		{
			if(!( fp = _tfopen(fd.GetPathName().operator LPCTSTR(), _T("r")) ) )
			{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_opening_f));
			return;
			}

			if (fd.GetFileExt() == _T("sgs") || fd.GetFileExt() == _T("SGS") ) 
			{
				if(!ReadSGSData(fp)) 
				{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_reading_d));
				return;
				}
			}
			else if (fd.GetFileExt() == _T("spd") || fd.GetFileExt() == _T("SPD") )
			{
				if(!ReadSPDData(fp))
				{
					AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_reading_d));
					return;
				}
				SetDescription(fd.GetPathName());

			}
			m_wndGrid.MakeItemEx();
			SetFuncName(fd.GetFileTitle());
			OnDataChanged();
			UpdateData(FALSE);
		}
		else if (fd.GetFileExt() == _T("xlsm") || fd.GetFileExt() == _T("XLSM"))
		{
			CDBDoc* pDoc = CDBDoc::GetDocPoint();
			m_nSpfcKey = NULL;

			if (m_bSetParam) {
				m_nSpfcKey = pDoc->m_pAttrCtrl->GetSpfcKey(m_csOldName);
			}
			else
			{
				m_nSpfcKey = pDoc->m_pAttrCtrl->GetStartNumSpfc();
			}

			if (m_nSpfcKey == 0) {
				// message
				pDoc->DisplayHistoryMessage(_LS(IDS_DB_IREG_SPFCNAME_NOTEXIST), m_csOldName);
				return;
			}

			CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
			ASSERT(pServiceHeadOffice);
			int nResult = pServiceHeadOffice->ReqService(_ULS(main), 2, nullptr);
			if (nResult) {
				CString strServerURI = AfxGetApp()->GetProfileString(_T("CONNECTION"), _T("URI"));
				CString strMapiKey = AfxGetApp()->GetProfileString(_T("CONNECTION"), _T("Key"));
				CString strParam = _T("");
				strParam.Format(_T("start excel \"%s\" /e/%s/%s/%d"), fd.GetPathName(), strServerURI, strMapiKey, m_nSpfcKey);
				_wsystem(strParam);
			}
			return;
		}
		else 
		{
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_extension_));
			return;
		}
	}
}
void CSpfcItemDlg::OnSpfcSeismicGen() 
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

void CSpfcItemDlg::OnChangeDataType() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nMessageID = LOWORD(pMsg->wParam);

	UINT aID[] = { IDC_SPFC_TYPE_NORMAL, IDC_SPFC_TYPE_ACC, 
		IDC_SPFC_TYPE_VEL, IDC_SPFC_TYPE_DISP, };
	CMapEx<UINT,UINT,int,int> mRadioID;
	for(int i = 0; i < 4; i++)    mRadioID.SetAt(aID[i], 0);

	int nYesNo = IDYES ;

	if(m_wndGrid.GetRowCount() > 1 && m_bTempSetParam == FALSE && mRadioID.Exist(nMessageID) && nMessageID != aID[m_nDataType])
		nYesNo = AfxMessageBox(_LS(IDS_CMD_THFC_WARNING_CHANGE_DATA_TYPE), MB_YESNO) ;

	m_bTempSetParam = FALSE ;  

	if(nYesNo != IDYES)
	{
		// Turn off old radio
		CButton* pRadio = (CButton*)GetDlgItem(LOWORD(pMsg->wParam));	
		pRadio->SetCheck(0);

		// Turn on new radio		
		pRadio = (CButton*)GetDlgItem(aID[m_nDataType]);
		pRadio->SetCheck(1);
		return ;
	}

	switch(nMessageID)
	{
	case IDC_SPFC_TYPE_NORMAL:  m_nDataType = 0; break;
	case IDC_SPFC_TYPE_ACC:     m_nDataType = 1; break;
	case IDC_SPFC_TYPE_VEL:     m_nDataType = 2; break;
	case IDC_SPFC_TYPE_DISP:    m_nDataType = 3; break;
	}

	T_UNIT_SYSTEM us;
	m_ParamData.nSpecFuncType = m_nDataType+1;
	m_wndGrid.SetHeaderTitle(us, FALSE);
	EnableGravity(m_nDataType == 0);
	OnDataChanged();

	//Change Unit
	CString csUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_SPFC_UNIT.dblValue[m_nDataType], csUnit);
	if (m_nDataType == 0)
		csUnit.Format(_T("g"));
	m_wndScaleUnit.SetWindowText(csUnit);
}

BOOL CSpfcItemDlg::PreTranslateMessage(MSG* pMsg) 
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

void CSpfcItemDlg::OnDataChanged()
{  
	// 1. Add handler to update graph    
	T_SPFC_BASE data;
	int nSize = m_ParamData.arSpecFuncData.GetSize();

	memset(m_dMnMxXVal,0,sizeof(m_dMnMxXVal));
	memset(m_dMnMxYVal,0,sizeof(m_dMnMxYVal));

	m_Graph.KillData();	
	if(!nSize)return;

	for(int i=0; i<nSize; i++)
	{
		data = m_ParamData.arSpecFuncData[i];
		SetValue(i,0,data.dblPeriod,data.dblValue);

		if(i==0)
		{
			m_dMnMxXVal[0] = data.dblPeriod;
			m_dMnMxXVal[1] = data.dblPeriod;
			m_dMnMxYVal[0] = data.dblValue;
			m_dMnMxYVal[1] = data.dblValue;
			continue; 
		}

		if(data.dblPeriod < m_dMnMxXVal[0]) m_dMnMxXVal[0] = data.dblPeriod;
		if(data.dblPeriod > m_dMnMxXVal[1]) m_dMnMxXVal[1] = data.dblPeriod;

		if(data.dblValue < m_dMnMxYVal[0]) m_dMnMxYVal[0] = data.dblValue;
		if(data.dblValue > m_dMnMxYVal[1]) m_dMnMxYVal[1] = data.dblValue;
	}

	double dX = (m_dMnMxXVal[1]-m_dMnMxXVal[0])/20.;
	double dY = (m_dMnMxYVal[1]-m_dMnMxYVal[0])/20.;

	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->SetMinRangeY(m_dMnMxYVal[0] - dY);   
	pD->SetMinRangeX(0.01); 
	pD->SetMaxRangeY(m_dMnMxYVal[1] + dY);   
	pD->SetMaxRangeX(m_dMnMxXVal[1] + dX);
	pD->GetStyle()->SetScaleLimit(FALSE);
	pD->GetStyle()->SetUseMaxScale(TRUE);    

	m_GraphView.Invalidate();  

	// Use only when grid is modified.
	// 2. delete descript (only when DB DataChanged~) // MNET:2138-GSJI-20060525   
	//if(m_ParamData.nSpecCodeType==D_SPFC_CODE_USER)   //CSpfcItemGrid::DBChanged()에서 세팅  
	//	SetDescription(_T(""));       
}

INT_PTR CSpfcItemDlg::DoModalSpectDlgAndGetStr(CString& strFuncName, CString& strDesc)
{
	INT_PTR retval=IDCANCEL;
	CETC_DESIGN_SPECT dlg(&m_ParamData, m_bModify4SpecCode, 0, m_bSeismicEvaluation);  //MNET:2138-GSJI-20060529

	m_arAccel.RemoveAll();
	m_arPeriod.RemoveAll();
	dlg.m_parAccel = &m_arAccel;
	dlg.m_parPeriod = &m_arPeriod;
	CWnd* pDampingWnd = GetDlgItem(IDC_CMD_DAMPING_EDT);
	dlg.m_pDampingWnd = pDampingWnd; 

	retval = dlg.DoModal();
	if(retval==IDOK)
	{
		strFuncName = dlg.m_strFuncName;
		strDesc     = dlg.m_sDescript;
	}

	return retval;
}

void CSpfcItemDlg::OnSpfcDesignSpectrum() 
{
	CString strFuncName, strDesc;
	if(DoModalSpectDlgAndGetStr(strFuncName, strDesc) == IDOK)
	{    
		T_SPFC_BASE data;
		NewData();
		SetDescription(strDesc);

		for(int i=0; i<m_arAccel.GetSize(); i++)
		{
			data.dblPeriod = m_arPeriod[i];
			data.dblValue = m_arAccel[i];
			m_ParamData.arSpecFuncData.Add(data);
		}

		if(m_ParamData.nSpecCodeType == D_SPFC_CODE_USER)
		{
			m_nScaleType = m_ParamData.nScaleMethod;
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
			OnChangeScaleRdo();
			CString csGravity;
			csGravity.Format(_T("%g"), m_ParamData.Gravity);
			m_wndGravity.SetWindowText(csGravity);
			m_dDamping = m_ParamData.dDampingRatio;
			UpdateData(FALSE);
		}
		else
		{
			m_ParamData.nSpecFuncType = 1; // Normalized Accel.
			if(m_ParamData.nSpecCodeType == D_SPFC_CODE_DPT2018)
			{
				m_dDamping = m_ParamData.dDampingRatio;
				UpdateData(FALSE);
			}
		}

		m_wndGrid.MakeItemEx();
		SetFuncName(strFuncName);
		OnDataChanged();
		ChangeDataType(m_ParamData.nSpecFuncType-1);
		OnChangeDataType();

		//CETC_DESIGN_SPECT 에서 OK하면 bSetParam=TRUE 상태가 되도록 한다.
		m_bModify4SpecCode = TRUE;  
	}	
}

void CSpfcItemDlg::InitChart()
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
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pDisplay->SetRect(0.2,0,100,97.0);
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(FALSE);


	// Logarithmic Axis Scaling	
	//pDisplay->GetStyle()->SetLogX(m_LogFlagX);
	//pDisplay->GetStyle()->SetLog(m_LogFlagY);

	m_Graph.AddComponent(pDisplay);

	// Add Label Component for X-Axis	
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
	SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Period__sec_));

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
	SetAxisLabelText(1,_LS(IDS_WG_CMD__ADDD__Spectral_Data));





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

void CSpfcItemDlg::SetValue(int nIndex, int nGroup, double x,double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);


}

void CSpfcItemDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CSpfcItemDlg::OnETCSPFCCHARTXLog() 
{

	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLogX(m_btnXLog.GetCheck());

	/*
	if (m_btnXLog.GetCheck() == TRUE || m_btnYLog.GetCheck() == TRUE) 
	{
	pD->GetStyle()->SetScaleLimit(TRUE);
	//pD->GetStyle()->SetUseMaxScale(TRUE);
	pD->SetMinRangeY(0.01);   
	pD->SetMinRangeX(0.01);   
	//pD->SetMaxRangeX(10);   
	}
	else 
	{
	pD->GetStyle()->SetScaleLimit(FALSE);
	//pD->GetStyle()->SetUseMaxScale(FALSE);
	}
	*/
	m_GraphView.Invalidate();		
}

void CSpfcItemDlg::OnETCSPFCCHARTYLog() 
{
	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLog(m_btnYLog.GetCheck());

	/*
	if (m_btnXLog.GetCheck() == TRUE || m_btnYLog.GetCheck() == TRUE) 
	{
	pD->GetStyle()->SetScaleLimit(TRUE);
	//pD->GetStyle()->SetUseMaxScale(TRUE);
	pD->SetMinRangeY(0.001);
	pD->SetMinRangeX(0.001);   
	//pD->SetMaxRangeX(10);   
	}
	else 
	{
	pD->GetStyle()->SetScaleLimit(FALSE);
	//pD->GetStyle()->SetUseMaxScale(FALSE);
	}
	*/
	m_GraphView.Invalidate();	
}

BOOL CSpfcItemDlg::ReadSGSData(FILE *fp)
{
	char		yytext[120];
	CStringA		str;
	CString msg;
	int			nGroup=-2, xIndex, yIndex,nNumOfDamping=1;
	BOOL bDampingLegend;
	CStringA  strTitle[2];

	T_SPFC_BASE data;  
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
		else if( sType == csCmdVelo )                        nDataType = 2; // velocity;
		else if( sType == csCmdDisp )                        nDataType = 3; // displacement;
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
	m_ParamData.nSpecFuncType= nDataType+1;

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
				double temp = atof(yytext);
				if(temp > 1.0 || temp < 0.0)  temp = 0.05;
				m_dDamping = temp;
				nNumOfDamping=nGroup/2+1;
			}
		}


		// Read X-Axis Value
		lex(fp, yytext);
		while (strcmp(yytext, "*") != 0) 
		{
			// Set X-Axis Value
			if(nGroup == 0)   // only the first group of data can be imported.
				data.dblPeriod = atof(yytext);
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
			m_ParamData.arSpecFuncData.Add(data);
		}
		lex(fp, yytext);
		csText = yytext;
		csText.TrimRight();
		csText.MakeUpper();
	}

	if (nNumOfDamping > 1) MessageBox(_LS(IDS_WG_CMD__ADDD__Warning___There_are_more_than_one));
	fclose(fp);

	CString sDescript;
	sDescript.Format(_T("%s %s"),strTitle[0],strTitle[1]);
	SetDescription(sDescript);

	return TRUE;

}

BOOL CSpfcItemDlg::ReadSPDData(FILE *fp)
{
	char		yytext[80];	

	T_SPFC_BASE data;
	NewData();

	// 번역하지 마시오
	CStringA csCmdUnit = "UNIT";
	CStringA csCmdData = "DATA";
	CStringA csCmdType = "TYPE";
	CStringA csCmdAccel = "ACCEL";
	CStringA csCmdVelo = "VELO";
	CStringA csCmdDisp = "DISP";
	CStringA csCmdGrav = "GRAV";
	CStringA csText;

	// Read and Set Spectrum data
	//
	try
	{    
		////////////////////////////////////////////////////////////////////
		// UNIT & Type

		// Unit
		CStringA sUnit,sType;
		int nUnitRead,nDataType;
		T_UNIT_INDEX UnitIndexCurrent;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexCurrent); 

		lex(fp, yytext); // asterisk
		lex(fp, yytext);
		csText = yytext;
		csText.TrimRight();
		csText.MakeUpper();

		sUnit = "";

		if (csText == csCmdUnit) 
		{
			lex(fp, yytext);		// comma
			lex(fp, yytext);
			sUnit = yytext;
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

			lex(fp, yytext);		// asterisk
			lex(fp, yytext);
			csText = yytext;
			csText.TrimRight();
			csText.MakeUpper();
		}
		else
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Data_Unit_Information___nReadi));
			nUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
			nDataType = 0; // normalized accel;
		}

		if (csText == csCmdType) 
		{
			// Data type
			lex(fp, yytext);	// comma
			lex(fp, yytext);
			sType = yytext;
			sType.TrimRight();
			sType.MakeUpper();

			if(sType == csCmdAccel && sUnit == csCmdGrav )       nDataType = 0; // normalized accel;
			else if( sType == csCmdAccel && sUnit != csCmdGrav ) nDataType = 1; // accel;
			else if( sType == csCmdVelo )                        nDataType = 2; // velocity;
			else if( sType == csCmdDisp )                        nDataType = 3; // displacement;
			else 
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Data_Type_));
				fclose(fp);
				return FALSE;
			}

			lex(fp, yytext);		// asterisk
			lex(fp, yytext);
			csText = yytext;
			csText.TrimRight();
			csText.MakeUpper();
		}
		else // 단위및 타입정보가 없으면 디폴트 설정...
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Data_Type_Information___nReadi));
			nUnitRead= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
			nDataType = 0; // normalized accel;
		}

		ChangeDataType(nDataType);
		m_ParamData.nSpecFuncType= nDataType+1;

		//////////////////////////////////////////////////////////////
		T_UNIT_INDEX UnitIndexOld, UnitIndexNew;
		pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
		UnitIndexNew=UnitIndexOld;
		UnitIndexNew.nBase_Length=nUnitRead;

		pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew); // 읽은 단위계에서 현재 단위계로
		pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld); // conversion 을 하기 위해서...
		////////////////////////////////////////////////////////////////

		if(csText == csCmdData) 
		{
			while (lex(fp, yytext)==1) // Read X-Axis Value
			{	

				// Set X-Axis Value
				data.dblPeriod = atof(yytext);
				lex(fp, yytext);	// comma
				if(strcmp(yytext, ",") != 0) 
				{
					AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
					fclose(fp);
					return FALSE;
				}

				lex(fp, yytext);

				// Set Y-Axis Value
				data.dblValue = UnitCvt(atof(yytext));
				m_ParamData.arSpecFuncData.Add(data);				

			}
		}
		else
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Data_or_Wrong_Format_));
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

int CSpfcItemDlg::lex(FILE *fp, char *yytext)
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

double CSpfcItemDlg::UnitCvt(double dReadData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	return  pDoc->m_pUnitCtrl->ConvertUnitLengthPrevious(dReadData);
}

void CSpfcItemDlg::NewData()
{
	m_ParamData.arSpecFuncData.RemoveAll();
	m_Graph.KillData();

	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;
#if defined(_CIVIL)
	if (CProduct::IsMovingType(D_PRODUCT_MOVING_JP))
	{
		m_LogFlagX = TRUE;
		m_LogFlagY = TRUE;
	}
#endif

	m_btnXLog.SetCheck(m_LogFlagX);
	m_btnYLog.SetCheck(m_LogFlagY);
	OnETCSPFCCHARTYLog();
	OnETCSPFCCHARTXLog();
}

void CSpfcItemDlg::SetFuncName(CString FuncName)
{
	CWnd* pWnd = GetDlgItem(IDC_SPFC_FUNC_NAME);
	pWnd->SetWindowText(FuncName);
}

void CSpfcItemDlg::OnApply() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE)) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess;
	m_bInternal = true;
	if (m_bSetParam)
		bSuccess = pDoc->m_pDataCtrl->ModifySpfc(m_csOldName, m_ParamData);
	else  
		bSuccess = pDoc->m_pDataCtrl->AddSpfc(m_ParamData);
	m_bInternal = false;
	if (!bSuccess) return;
	else  m_bModify4SpecCode = FALSE;

	InitParam();
	NewData();
	SetFuncName(_T(""));
	SetDescription(_T(""));
	OnDataChanged();
	m_wndGrid.MakeItemEx();
}

void CSpfcItemDlg::SetDescription(CString Descript)
{
	CWnd* descript = GetDlgItem(IDC_CMD_ETC_DESCRIPT);
	descript->SetWindowText(Descript);
	m_csDescription = Descript; 
}

void CSpfcItemDlg::OnChangeScaleRdo()
{
	UpdateData();
	BOOL bScale = TRUE;
	if (m_nScaleType == 1)
		bScale = FALSE;
	//GetDlgItem(IDC_SPFC_SCALE)->EnableWindow(bScale);
	//GetDlgItem(IDC_SPFC_SCALE2)->EnableWindow(!bScale);

	if(m_bShowReadOnly==TRUE)
	{
		GetDlgItem(IDC_SPFC_SCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPFC_SCALE2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_SPFC_SCALE)->EnableWindow(bScale);
		GetDlgItem(IDC_SPFC_SCALE2)->EnableWindow(!bScale);
	}	
}

void CSpfcItemDlg::SetShowType(BOOL bShowReadOnly)
{
	m_bShowReadOnly = bShowReadOnly;
}

void CSpfcItemDlg::MakeControlToReadOnly()
{
	GetDlgItem(IDC_SPFC_FUNC_NAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_IMPORT_FILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_DESIGN_SPECTRUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_SEISMIC_GEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_TYPE_NORMAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_TYPE_ACC)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_TYPE_VEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_TYPE_DISP)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_SCALE_RDO1)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_SCALE_RDO2)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_SCALE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_SCALE2)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPFC_GRAVITY)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_DAMPING_EDT)->EnableWindow(FALSE);
	GetDlgItem(IDC_ETC_SPFC_CHART_XLog)->EnableWindow(FALSE);
	GetDlgItem(IDC_ETC_SPFC_CHART_YLog)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_ETC_DESCRIPT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
}
