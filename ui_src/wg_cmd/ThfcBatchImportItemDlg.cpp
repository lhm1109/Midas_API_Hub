#include "stdafx.h"
#include "wg_cmd.h"
#include "ThfcBatchImportItemDlg.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\EditData.h"
#include "..\wg_db\UndoCtrl.h"
#include "CMThfcRpsrProgDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CThfcBatchImportItemDlg::CThfcBatchImportItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CThfcBatchImportItemDlg::IDD, pParent)
{
	m_bGridInit = FALSE;
	m_aTimeCtrl.Add(IDC_THFC_IMPORT_FILE);
	m_aTimeCtrl.Add(IDC_THFC_DESIGN_SPECTRUM);
	//m_aTimeCtrl.Add(IDC_THFC_SEISMIC_GEN);
	m_aTimeCtrl.Add(IDC_THFC_TABLE);
	m_aTimeCtrl.Add(IDC_THFC_DESIGN_HEELDROP);

	m_aTypeCtrl.Add(IDC_THFC_TYPE_NORMAL);
	m_aTypeCtrl.Add(IDC_THFC_TYPE_ACC);
	m_aTypeCtrl.Add(IDC_THFC_TYPE_FORCE);
	m_aTypeCtrl.Add(IDC_THFC_TYPE_MOMENT);
	m_aTypeCtrl.Add(IDC_THFC_TYPE_NORMAL2);
}


void CThfcBatchImportItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_THFC_USER_TIME_CHECK,	m_bUserTime);
	DDX_Check(pDX, IDC_THFC_AUTO_ADJUST_CHECK,	m_bAdjust);
	DDX_Control(pDX, IDC_THFC_CH_TIME_EDIT,		m_edtTolTime);
	DDX_Control(pDX, IDC_THFC_CH_TIME2_EDIT,	m_edtStepTime);
	DDX_Control(pDX, IDC_THFC_SCALE,			m_edtStartTime);
	DDX_Control(pDX, IDC_THFC_SCALE2,			m_edtEndTime);
	DDX_Control(pDX, IDC_THFC_CH_TIME2_EDIT2,	m_edtEPA);
	DDX_Control(pDX, IDC_THFC_CH_TIME2_EDIT3,	m_edtEPAScale);

	DDX_Control(pDX, IDC_THFC_COM1, m_IntensityCom);
	DDX_Control(pDX, IDC_THFC_COM2, m_EffectCom);
	DDX_Control(pDX, IDC_THFC_LIST, m_ListFile);
	DDX_Control(pDX, IDC_THFC_LIST2, m_ListData);
}

BEGIN_MESSAGE_MAP(CThfcBatchImportItemDlg, CDialogMove)
	ON_BN_CLICKED(IDC_THFC_IMPORT_FILE,			OnEtcThfcAdd)
	ON_BN_CLICKED(IDC_THFC_DESIGN_SPECTRUM,		OnEtcThfcEdit)
	ON_BN_CLICKED(IDC_THFC_DESIGN_HEELDROP,		OnEtcThfcDel)
	ON_BN_CLICKED(IDC_THFC_AUTO_ADJUST_CHECK,	OnCheckAdjust)
	ON_BN_CLICKED(IDC_THFC_USER_TIME_CHECK,		OnCheckUserTime)
	
	ON_CBN_SELCHANGE(IDC_THFC_COM1, OnSelChangeSeisCombox)
	ON_CBN_SELCHANGE(IDC_THFC_COM2, OnSelChangeSeisCombox)
	ON_CBN_SELCHANGE(IDC_THFC_LIST, OnSelChangeFileList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_THFC_LIST2, OnSelChangeSeisList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CThfcBatchImportItemDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	SetHeaderTitle(TRUE);
	InitCtrl();
	MakeListItem();
	OnCheckUserTime();
	OnCheckAdjust();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	InitChart();
	OnSelChangeFileList();
	return TRUE;
}

void CThfcBatchImportItemDlg::InitCtrl()
{
	CString strEffect[] = { _T("6(0.05g)"),_T("7(0.10g)"),_T("7(0.15g)"), _T("8(0.20g)"), _T("8(0.30g)"), _T("9(0.40g)") };
	CString strIntensity[] = { _LS(IDS_CMD0417__Frequent_E_Q_),_LS(IDS_CMD0417__Middle_E_Q_),_LS(IDS_CMD0417__Scarce_E_Q_),_LS(IDS_CMD0417__Very_Scarce_E_Q_) };
	for (int i = 0; i < 6; i++)
	{
		m_EffectCom.AddString(strEffect[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		m_IntensityCom.AddString(strIntensity[i]);
	}
	m_IntensityCom.SetCurSel(0);
	m_EffectCom.SetCurSel(0);
	GetDlgItem(IDC_THFC_CH_TIME2_EDIT2)->EnableWindow(FALSE);
	m_edtTolTime.SetTextFormat(_T("%.4g"));
	m_edtStepTime.SetTextFormat(_T("%.4g"));
	m_edtStartTime.SetTextFormat(_T("%.4g"));
	m_edtEndTime.SetTextFormat(_T("%.4g"));
	m_edtEPA.SetTextFormat(_T("%.4g"));
	m_edtEPAScale.SetTextFormat(_T("%.4g"));
}

void CThfcBatchImportItemDlg::OnOK()
{
	if (!UpdateData(TRUE)) return;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_THFC_D ThfcD;
	ThfcD.Initialize();
	ThfcD.nThisFuncType = 1;
	ThfcD.nScaleMethod = 0;
	ThfcD.ScaleFactor = 1.0;
	ThfcD.Gravity = pDoc->m_pInitCtrl->DefaultGravity();
	CString strFuncName;
	if (!pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Time_History_Function)), CMDTYPE_REMOVE_ANALYSIS))
		return;
	for (int i = 0; i < m_aThfcCHData[1].GetSize(); i++)
	{
		const ThfcCHData& ThfcData = m_aThfcCHData[1][i];
		bool bFullLength = false;
		if (ThfcData.strName.GetLength() > 25)
		{
			strFuncName = ThfcData.strName.Left(25);
			bFullLength = true;
		}
		else
		{
			strFuncName = ThfcData.strName;
		}
		int nFileSuffix = 1;
		CString str;
		while (pDoc->m_pAttrCtrl->ExistThfc(strFuncName))
		{
			if (bFullLength)
				strFuncName = strFuncName.Left(25);
			else
				strFuncName = ThfcData.strName;
			strFuncName.Format(_T("%s-%d"), strFuncName, nFileSuffix++);
		}
		ThfcD.ThisFuncName = strFuncName;
		if(ThfcData.nDataType == 0)
			ThfcD.nThisDataType = 1;
		else
			ThfcD.nThisDataType = 2;
		ThfcD.arThisFuncData.SetSize(ThfcData.m_arTime.GetSize());
		for (int j = 0; j < ThfcD.arThisFuncData.GetSize(); j++)
		{
			ThfcD.arThisFuncData[j].dblTime = ThfcData.m_arTime[j];
			ThfcD.arThisFuncData[j].dblValue = ThfcData.m_arAccel[j];
		}
		if (!pDoc->m_pEditData->AddThfc(ThfcD))
		{
			EndEdit(FALSE);
			return;
		}
	}
	EndEdit(TRUE);
	CDialogMove::OnOK();
}

BOOL CThfcBatchImportItemDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			if (pMsg->wParam == VK_RETURN)
				if (pMsg->wParam == VK_ESCAPE)

					::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
			// DO NOT process further
		}
	}
	return CDialogMove::PreTranslateMessage(pMsg);
}

void CThfcBatchImportItemDlg::SetHeaderTitle(BOOL bInit)
{
	CString aTitle[] = { _LS(IDS_CMD_THFC_IMPORT_BATCH_NAME), _LS(IDS_CMD_THFC_IMPORT_BATCH_START_TIME),
		_LS(IDS_CMD_THFC_IMPORT_BATCH_END_TIME),_LS(IDS_CMD_THFC_IMPORT_BATCH_ADJUST_COE) };
	int nColWidth[4]; 	

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	if (bInit)
	{
		DWORD dwStyle;
		dwStyle = ListView_GetExtendedListViewStyle(m_ListData.GetSafeHwnd());
		dwStyle |= LVS_EX_FULLROWSELECT;
		ListView_SetExtendedListViewStyle(m_ListData.GetSafeHwnd(), dwStyle);
	}

	// Calculate width for each column
	nColWidth[0] = 120; nColWidth[1] = 100; nColWidth[2] = 100; nColWidth[3] = 100;

	// Set Title
	for (i = 0; i < 4; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		if (bInit)
			m_ListData.InsertColumn(i,&lvcolumn);
		m_ListData.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CThfcBatchImportItemDlg::MakeListItem()
{
	CString str;
	CString strFile;
	FILE*	fp;
	m_aThfcCHData[0].RemoveAll();
	ThfcCHData ThfcD;
	CCMThfcRpsrProgDlg* pPrg = NULL;
	for (int m = 0; m < m_arStrFileName.GetSize(); m++)
	{
		strFile = m_arStrFileName[m];
		if (!(fp = _tfopen(strFile, _T("r"))))
		{
			MessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_opening_f));
			return FALSE;
		}
		int nType = GetFileType(fp);
		//wave - data
		if (nType == 0)
		{
			//*.dbs
			//CString strFileExt = fp.GetFileExt();
			//if(strFileExt != "dbs" && strFileExt != "sgs")
			//	continue;
			if(!ReadEQData(fp,ThfcD))
				continue;
		}
		else if (nType == 1)
		{
			//*.txt(YJK data)
			if(!ReadEQData_CH_1(fp,ThfcD))
				continue;
		}
		else if (nType == 2)
		{
			//*.txt(Pk data)
			if (!ReadEQData_CH_2(fp, ThfcD))
				continue;
		}
		else
			continue;
		if(ThfcD.m_arTime.GetSize() < 3)
			continue;
		ThfcD.dStartTime	= ThfcD.m_arTime[0];
		ThfcD.dEndTime		= ThfcD.m_arTime[ThfcD.m_arTime.GetSize()-1];
		ThfcD.dTolTime		= ThfcD.dEndTime;
		ThfcD.dStepTime		= ThfcD.m_arTime[1] - ThfcD.m_arTime[0];
		ThfcD.nDataType		= nType;
		if(!GetEPA(ThfcD, pPrg,TRUE))
			continue;
		int nItem = m_ListFile.GetCount();
		int nTemp = strFile.ReverseFind('\\');
		CString strTempFile = strFile.Right(strFile.GetLength() - nTemp - 1);
		nTemp = strTempFile.ReverseFind('.');
		strTempFile = strTempFile.Left(nTemp);
		ThfcD.strName = strTempFile;
		m_aThfcCHData[0].Add(ThfcD);
		m_ListFile.AddString(strTempFile);
	}
	if (pPrg)
	{
		pPrg->SendMessage(WM_DESTROY, 0, 0);
		if (pPrg) { delete pPrg; pPrg = NULL; }
		pPrg = NULL;
	}
	if(m_ListFile.GetCount() < 1)
		return FALSE;
	m_ListFile.SetSel(0);
	return TRUE;
}

void CThfcBatchImportItemDlg::InitChart()
{
	////////////////////////////////////////////////////////////////
	// Initialize m_Graph

	m_GraphView.SetDragable(FALSE); // set data-drag unable
	// Add Background component
	//
	SRGraphBackground	*pB = new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent((SRGraphComponent *)pB);

	// Add Display Component
	//

	SRGraphDisplay *pDisplay = new SRGraphDisplay;
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX);
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	pDisplay->SetSizeDefaults();
	pDisplay->SetRect(0.2, 0, 100, 97.0);
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);
	pDisplay->GetStyle()->SetAlwaysShowZero(TRUE);

	// Logarithmic Axis Scaling
	//
	pDisplay->GetStyle()->SetLogX(FALSE);
	pDisplay->GetStyle()->SetLog(FALSE);

	//pDisplay->GetStyle()->SetShowXGrid(m_GridXFlag);
	//pDisplay->GetStyle()->SetShowYGrid(m_GridYFlag);

	m_Graph.AddComponent(pDisplay);

	// Add Label Component for X-Axis
	//
	SRGraphLabel	*pLabel = new SRGraphLabel;
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
	SetAxisLabelText(0, _LS(IDS_WG_CMD__ADD2__Time__sec_));

	// Add Label Component for Y-Axis
	//
	pLabel = new SRGraphLabel;
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
	SetAxisLabelText(1, _LS(IDS_WG_CMD__ADDD__Time_History_Data));

	/////////////////////////////////////////////////////////////
	// Attatch graphview to dialog's placeholder and connect graph to graphview

	CWnd* wnd = GetDlgItem(IDC_CMD_PLACEHOLDER);
	CRect rc;

	wnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	rc.left += 2;
	rc.top += 2;
	rc.right -= 2;
	rc.bottom -= 2;

	m_GraphView.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rc, this, IDC_CMD_PLACEHOLDER);
	m_GraphView.m_pGraph = &m_Graph;

	m_GraphView.Invalidate();
}

void CThfcBatchImportItemDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup * 2, x);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup * 2 + 1, y);
}

void CThfcBatchImportItemDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel = (SRGraphLabel *)m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

int CThfcBatchImportItemDlg::lex(FILE *fp, char *yytext)
{
	int		c, index;

	index = 0;
	c = getc(fp);

	// Discard white characters
	while ((c == ' ') || (c == '\r') || (c == '\t') || (c == '\n')) {
		c = getc(fp);
	}

	if (c == EOF) return -1;

	
	// Handle(Discard) Comments
	while (c == '*') {
		while (c != '\n')
		{
			c = getc(fp);
			if (c == EOF) return -1;
		}
		c = getc(fp);
		if (c == EOF) return -1;
	}

	switch (c) {
	case ',':
		yytext[index++] = c;
		yytext[index] = '\0';
		break;
	default:
		while ((c != '*') && (c != ',') && (c != '\r') && (c != '\n')) {
			yytext[index++] = c;
			c = getc(fp);
			if (c == EOF || index >= 120)
				return -1;
		}
		yytext[index] = '\0';
		if ((c == '*') || (c == ','))
			ungetc(c, fp);
	}

	return 1;
}

int CThfcBatchImportItemDlg::GetFileType(FILE *fp)
{
	char	yytext[120];
	int		c, index;

	index = 0;
	c = getc(fp);

	// Discard white characters
	while ((c == ' ') || (c == '\r') || (c == '\t') || (c == '\n')) {
		c = getc(fp);
	}

	if (c == EOF) return -1;

	while ((c != '*') && (c != ',') && (c != '\r') && (c != '\n') && (c != ':')) {
		if (c == ' ')//pk wave-data
		{
			for (int i = 0; i < index; i++)
			{
				if (yytext[i] != '.' && (yytext[i] < '0' || yytext[i] > '9'))
					return -1;
			}
			return 2;
		}
		yytext[index++] = c;
		c = getc(fp);
		if (c == EOF || index >= 120)
			return -1;
	}

	switch (c) {
	case ',':
		yytext[index++] = c;
		yytext[index] = '\0';
		break;
	case '*':
		while (c != '\n') 
		{
			c = getc(fp);
		}
		break;
	default:
		while ((c != '*') && (c != ',') && (c != '\r') && (c != '\n') && (c != ':')) {
			yytext[index++] = c;
			c = getc(fp);
			if (c == EOF || index >= 120)
				return -1;
		}
		yytext[index] = '\0';
		if ((c == '*') || (c == ','))
			ungetc(c, fp);
	}

	yytext[index] = '\0';
	if (strcmp(yytext, "Tg") == 0)
	{
		return 1;	//YJK Format
	}
	return 0;
}

void CThfcBatchImportItemDlg::Updata(const ThfcCHData& ThfcData)
{
	m_edtTolTime.SetEditUnit(ThfcData.dTolTime);
	m_edtStepTime.SetEditUnit(ThfcData.dStepTime);
	m_edtStartTime.SetEditUnit(ThfcData.dStartTime);
	m_edtEndTime.SetEditUnit(ThfcData.dEndTime);
	m_edtEPA.SetEditUnit(ThfcData.dEPA);
	m_edtEPAScale.SetEditUnit(ThfcData.dEPAScaleFactor);
	m_IntensityCom.SetCurSel(ThfcData.nIntensity);
	m_EffectCom.SetCurSel(ThfcData.nEffect);
	m_bUserTime = ThfcData.bAutoTime;
	m_bAdjust = ThfcData.bAdjust;
	UpdateData(FALSE);
}

void CThfcBatchImportItemDlg::OnThfcRedrawGraph(const CArray<double, double>& aX, const CArray<double,double>& aY)
{
	int nSize = aX.GetSize();
	m_Graph.KillData();
	for (int i = 0; i < nSize; i++)
	{
		SetValue(i, 0, aX[i], aY[i]);
	}
	m_GraphView.Invalidate();
}

BOOL CThfcBatchImportItemDlg::ReadEQData(FILE *fp, ThfcCHData& ThfcD)
{
	char		yytext[120];
	double		GravFactor;
	int nNumOfData = 0;
	GravFactor = 1.0;		// EQRecord view --> unitless normalized acceleration 
	ThfcD.m_arTime.RemoveAll();
	ThfcD.m_arAccel.RemoveAll();
	// Read and Set EQData
	//
	try
	{
		while (lex(fp, yytext) == 1) // Read X-Axis Value
		{
			// Set X-Axis Value
			ThfcD.m_arTime.Add(atof(yytext));
			if (nNumOfData >= 3)
			{
				m_dSampleTime = ThfcD.m_arTime[2] - ThfcD.m_arTime[1]; // 샘플의 시간간격을 찾기 위함 

			}

			lex(fp, yytext);	// comma
			if (strcmp(yytext, ",") != 0)
			{
				//MessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
				fclose(fp);
				return FALSE;
			}

			if (lex(fp, yytext) == -1)  // data end, the last y data;
			{
				// Set Y-Axis Value
				ThfcD.m_arAccel.Add(atof(yytext)*GravFactor);
				nNumOfData++;
				break;
			}

			// Set Y-Axis Value
			ThfcD.m_arAccel.Add(atof(yytext)*GravFactor);
			nNumOfData++;
			if (nNumOfData > 18000 - 1)
			{
				GSaveHistory(_LS(IDS_DB_THFC_WARNING__INVALID_NUM));
				break;
			}
		}
		if (nNumOfData < 1)
		{
			MessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Data_Found_));
			fclose(fp);
			return FALSE;
		}
		fclose(fp);
	}
	catch (CFileException e[])
	{
		e->ReportError();
		MessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occured_while_reading_da));
		return FALSE;
	}
	return TRUE;
}

BOOL CThfcBatchImportItemDlg::ReadEQData_CH_1(FILE *fp, ThfcCHData& ThfcD)
{
	char		yytext[120];
	double		GravFactor;
	int nNumOfData = 0;
	//GravFactor = 0.001;//mm->m
	//GravFactor = CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_GRAVITY, 0.001);
	GravFactor = CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_GRAVITY, 0.01);
	bool bstart = false;
	double dInterval = 0.0;
	double dXValue = 0.0;
	ThfcD.m_arTime.RemoveAll();
	ThfcD.m_arAccel.RemoveAll();
	// Read and Set EQData
	try
	{
		while (lex(fp, yytext) == 1)
		{
			if (yytext[0] == 'D')
			{
				char	temp[20];
				for (int i = 0; i < 20; i++)
				{
					temp[i] = yytext[i + 2];
					if (yytext[i + 2] == '\0')
						break;
				}
				dInterval = atof(temp);
				bstart = true;
				lex(fp, yytext);
				continue;
			}
			if(!bstart)
				continue;
			ThfcD.m_arTime.Add(dXValue);
			dXValue += dInterval;
			ThfcD.m_arAccel.Add(atof(yytext)*GravFactor);
			nNumOfData++;
		}
		if (nNumOfData < 1)
		{
			MessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Data_Found_));
			fclose(fp);
			return FALSE;
		}
		fclose(fp);
	}
	catch (CFileException e[])
	{
		e->ReportError();
		MessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occured_while_reading_da));
		return FALSE;
	}
	return TRUE;
}

BOOL CThfcBatchImportItemDlg::ReadEQData_CH_2(FILE *fp, ThfcCHData& ThfcD)
{
	char		yytext[120];
	double		GravFactor;
	int nNumOfData = 0;
	//GravFactor = 0.01;//cm->m
	GravFactor = CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_GRAVITY, 1.0);
	double dInterval = 0.0;
	double dXValue = 0.0;
	ThfcD.m_arTime.RemoveAll();
	ThfcD.m_arAccel.RemoveAll();
	// Read and Set EQData
	try
	{
		lex(fp, yytext);
		dInterval = atof(yytext);
		if (dInterval < 1.E-15)
			return FALSE;
		while (lex(fp, yytext) == 1) // Read X-Axis Value
		{
			// Set X-Axis Value
			ThfcD.m_arTime.Add(dXValue);
			dXValue += dInterval;
			ThfcD.m_arAccel.Add(atof(yytext)*GravFactor);
			nNumOfData++;
		}
		if (nNumOfData < 1)
		{
			MessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Data_Found_));
			fclose(fp);
			return FALSE;
		}
		fclose(fp);
	}
	catch (CFileException e[])
	{
		e->ReportError();
		MessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occured_while_reading_da));
		return FALSE;
	}
	return TRUE;
}

void CThfcBatchImportItemDlg::SetFunctionName(CString FuncName)
{
	CWnd* pWnd = GetDlgItem(IDC_THFC_FUNC_NAME);
	pWnd->SetWindowText(FuncName);
}

void CThfcBatchImportItemDlg::OnSelChangeFileList()
{
	int nCount = m_ListFile.GetCount();
	for (int nItem = 0; nItem < nCount; nItem++)
	{
		int nMask = m_ListFile.GetSel(nItem);
		if (nMask == 0)
			continue;
		if (nItem > m_aThfcCHData[0].GetSize())
		{
			ASSERT(FALSE);
			return;
		}
		const ThfcCHData& ThfcD = m_aThfcCHData[0][nItem];
		double dDEPA = GetDesignEPA();
		double dEPA_F = (fabs(ThfcD.dEPA) > 1.E-15) ? dDEPA / ThfcD.dEPA : 0.0;
		m_edtTolTime.SetTextFormat(_T("%.4f"));
		m_edtTolTime.SetEditUnit(ThfcD.dTolTime);
		m_edtStepTime.SetEditUnit(ThfcD.dStepTime);
		m_edtStartTime.SetEditUnit(ThfcD.dStartTime);
		m_edtEndTime.SetEditUnit(ThfcD.dEndTime);
		m_edtEPA.SetEditUnit(ThfcD.dEPA);
		m_edtEPAScale.SetEditUnit(dEPA_F);

		m_bUserTime = ThfcD.bAutoTime;
		m_bAdjust = ThfcD.bAdjust;
		break;
	}
	UpdateData(FALSE);
	OnCheckAdjust();
	OnCheckUserTime();
}

void CThfcBatchImportItemDlg::OnSelChangeSeisCombox()
{
	UpdateData(TRUE);
	if (!m_bAdjust)
		return;
	double dDEPA = GetDesignEPA();
	double dEPA = m_edtEPA.GetEditValue();
	double dEPA_F = (fabs(dEPA) > 1.E-15) ? dDEPA / dEPA : 0.0;
	CString csVal;
	csVal.Format(_T("%g"), dEPA_F);
	m_edtEPAScale.SetWindowText(csVal);
}

void CThfcBatchImportItemDlg::OnSelChangeSeisList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1)
	{
		*pResult = 0;
		return;
	}

	int nItem = pNMListView->iItem;
	if (nItem == -1)
	{
		*pResult = 0;
		return;
	}

	ThfcCHData& ThfCData = m_aThfcCHData[1][nItem];
	Updata(ThfCData);
	OnThfcRedrawGraph(ThfCData.m_arTime, ThfCData.m_arAccel);
	OnCheckUserTime();
	OnCheckAdjust();
}

BOOL CThfcBatchImportItemDlg::GetEPA(ThfcCHData &Data, CCMThfcRpsrProgDlg*& pPrg, BOOL bProgress)
{
	// Absolute Acceleration
	int nStep = 1000;// Data.m_arTime.GetSize();
	if (nStep > 18000)
	{
		AfxMessageBox(_LS(IDS_CMD_THFC_IMPORT_ERROR__INVALID_NUM));
		return FALSE;
	}

	if (bProgress && pPrg == NULL)
	{
		pPrg = new CCMThfcRpsrProgDlg;
		pPrg->Create(IDD_ETC_THFC_RPSR_PROG, NULL);
		pPrg->SetWindowText(_LS(IDS_CMD_THFC_IMPORT_CALC_WAVE_EPA));
		pPrg->ShowWindow(SW_SHOW);
	}

	Data.dEPA = 0.0;
	double dFrom = 0.01;// Data.dStartTime;
	double dTo = 10;// Data.dEndTime;
	float *Umax = NULL, *dUmax = NULL, *ddUmax = NULL, *Pvelo = NULL, *Pacc = NULL;
	Umax = new float[18000];
	dUmax = new float[18000];
	ddUmax = new float[18000];
	Pvelo = new float[18000];
	Pacc = new float[18000];

	float *U = NULL, *dU = NULL, *ddU = NULL, *TddU = NULL, *TddUmax = NULL, *P = NULL;
	U = new float[18001];
	dU = new float[18001];
	ddU = new float[18001];
	TddU = new float[18001];
	TddUmax = new float[18000];
	P = new float[18000];

	double dConvertUnit = 1.0;
	double dGravity = 9.815;// M_InitValueCurUnit(9.815, TON, M, D_UNITSYS_BASE_GRAVITY);
	if (Data.nDataType != 0)
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		dConvertUnit = 1.0/dGravity * pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_GRAVITY, 1.0);
	}
	

	float	dt = 0.02f;
	if (Data.m_arTime.GetSize() >= 3) dt = Data.m_arTime[2] - Data.m_arTime[1];

	// Scaling (Time, Spectral Value)
	int nSize = Data.m_arTime.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		P[i] = Data.m_arAccel[i] * dGravity * dConvertUnit;
	}

	int SN = 0;
	double dXI = 0.05;// DampRatio;
	double dOM = 0., dOMD = 0., dOMB = 0., dXIB = 0., SA = 0.;
	float Tn = 0.f, T = 0.f, dB0 = 0.f, dB1 = 0.f, dB2 = 0.f, dB3 = 0.f, dB4 = 0.f, dB5 = 0.f, dB6 = 0.f, dB7 = 0.f, dB8 = 0.f, dB9 = 0.f, dB10 = 0.f;
	float SS = 0.f, CC = 0.f, DSS = 0.f, DCC = 0.f, DDSS = 0.f, DDCC = 0.f;
	float dA1 = 0.f, dA2 = 0.f, dA3 = 0.f, dA4 = 0.f, dA5 = 0.f, dA6 = 0.f, dA7 = 0.f, dA8 = 0.f, dA9 = 0.f, dA10 = 0.f, dA11 = 0.f, dA12 = 0.f;
	double fValue = 0.0;

	double dToCalcTime = 0.1;
	double dT1CalcTime = 0.5;

	CArray<double, double> aX, aY;
	aX.RemoveAll(), aY.RemoveAll();
	aX.SetSize(nStep), aY.SetSize(nStep);

	if (bProgress) pPrg->m_Progress.SetRange(0,nStep);
	for (int j = 0; j < nStep; ++j)
	{
		if (bProgress) pPrg->m_Progress.SetPos(j);
		// Set Period
		T = dFrom + j * (dTo - dFrom) / nStep;
		Tn = T;
		if (Tn == 0) Tn = 0.0000000001f;

		dOM = 2.*CMathFunc::m_pi / Tn;
		dOMD = dOM * sqrt(1.0 - pow(dXI, 2));
		dOMB = dOMD * dXI;
		dXIB = dXI / sqrt(1.0 - pow(dXI, 2));

		dB0 = 2.0*dXI / dOM / dt;
		dB1 = 1.0 + dB0;
		dB2 = -1.0 / dt;
		dB3 = -dXIB * dB1 - dB2 / dOMD;
		dB4 = -dB1;
		dB5 = -dB0;
		dB6 = -dB2;
		dB7 = -dXIB * dB5 - dB6 / dOMD;
		dB8 = -dB5;
		dB9 = pow(dOMD, 2) - pow(dOMB, 2);
		dB10 = 2.0*dOMB*dOMD;

		SS = exp(-dXI * dOM*dt)*sin(dOMD*dt);
		CC = exp(-dXI * dOM*dt)*cos(dOMD*dt);
		DSS = -dOMB * SS + dOMD * CC;
		DCC = -dOMB * CC - dOMD * SS;
		DDSS = -dB9 * SS - dB10 * CC;
		DDCC = -dB9 * CC + dB10 * SS;

		dA1 = CC + dXIB * SS;
		dA2 = SS / dOMD;
		dA3 = (dB1 + dB2 * dt + dB3 * SS + dB4 * CC) / (pow(dOM, 2));
		dA4 = (dB5 + dB6 * dt + dB7 * SS + dB8 * CC) / (pow(dOM, 2));
		dA5 = DCC + dXIB * DSS;
		dA6 = DSS / dOMD;
		dA7 = (dB2 + dB3 * DSS + dB4 * DCC) / (pow(dOM, 2));
		dA8 = (dB6 + dB7 * DSS + dB8 * DCC) / (pow(dOM, 2));
		dA9 = DDCC + dXIB * DDSS;
		dA10 = DDSS / dOMD;
		dA11 = (dB3*DDSS + dB4 * DDCC) / (pow(dOM, 2));
		dA12 = (dB7*DDSS + dB8 * DDCC) / (pow(dOM, 2));

		float DDY1 = 0.f, PM1 = 0.f;
		int   IST = 0;
		if (Data.m_arTime[0] == 0.)
		{
			DDY1 = -P[0]; //DDY1 = -Data.aData[0].dblValue;        
			PM1 = DDY1;
			IST = 1;
		}

		U[0] = 0.0f;
		dU[0] = 0.0f;
		ddU[0] = P[0] - 2 * dOM*dXI*dU[0] - pow(dOM, 2)*U[0];    //ddU[0] = P[0]; 와 동일

		Umax[j] = U[0];
		dUmax[j] = dU[0];
		ddUmax[j] = ddU[0];
		TddUmax[j] = 0.0f;
		float PM[18000];
		for (int t = 0; t < 18000; ++t)
			PM[t] = 0.0f;

		for (int i = 0; i < nSize; i++)
		{
			if (i == nSize - 1 && IST == 1) continue;

			PM[i] = (float)PM1;
			PM1 = -P[i + IST];

			U[i + 1] = dA1 * U[i] + dA2 * dU[i] + dA3 * PM[i] + dA4 * PM1;
			dU[i + 1] = dA5 * U[i] + dA6 * dU[i] + dA7 * PM[i] + dA8 * PM1;
			ddU[i + 1] = dA9 * U[i] + dA10 * dU[i] + dA11 * PM[i] + dA12 * PM1;
			TddU[i + 1] = ddU[i + 1] + P[i + IST];

			Umax[j] = __max(Umax[j], fabs(U[i + 1]));      // Relative Displacement
			dUmax[j] = __max(dUmax[j], fabs(dU[i + 1]));		  // Relative Velocity
			ddUmax[j] = __max(ddUmax[j], fabs(ddU[i + 1]));     // Relative Acceleration
			TddUmax[j] = __max(TddUmax[j], fabs(TddU[i + 1]));    // Absolute Acceleration
		}

		Pvelo[j] = Umax[j] * dOM;
		Pacc[j] = Umax[j] * pow(dOM, 2);
		fValue = TddUmax[j];

		aX[j] = Tn;
		aY[j] = fValue / dGravity;

		if (dToCalcTime - T < 1.E-15 && T - dT1CalcTime < 1.E-15)
		{
			SA += aY[j];
			++SN;
		}
	}

	Data.dEPA = (SN > 0) ? (SA / SN) / 2.25 : 0.0;

	if (Umax)    delete[]Umax;
	if (dUmax)   delete[]dUmax;
	if (ddUmax)  delete[]ddUmax;
	if (Pvelo)   delete[]Pvelo;
	if (Pacc)    delete[]Pacc;
	if (U)       delete[]U;
	if (dU)      delete[]dU;
	if (ddU)     delete[]ddU;
	if (TddU)    delete[]TddU;
	if (TddUmax) delete[]TddUmax;
	if (P)       delete[]P;

	return TRUE;
}

double CThfcBatchImportItemDlg::GetDesignEPA()
{
	//China 2010 Code
	//double dPlusDTG = 0.0;
	int nInfCoefType = m_IntensityCom.GetCurSel();
	int nEffect = m_EffectCom.GetCurSel();
	double dDesignEPA = 0.0;
	if (nInfCoefType == 0)
	{
		switch (nEffect)
		{
		case 0: dDesignEPA = 18.0; break;
		case 1: dDesignEPA = 35.0; break;
		case 2: dDesignEPA = 55.0; break;
		case 3: dDesignEPA = 70.0; break;
		case 4: dDesignEPA = 110.0; break;
		case 5: dDesignEPA = 140.0; break;
		default: ASSERT(0); break;
		}
	}
	else if (nInfCoefType == 1)
	{
		switch (nEffect)
		{
		case 0: dDesignEPA = 50.0; break;
		case 1: dDesignEPA = 100.0; break;
		case 2: dDesignEPA = 150.0; break;
		case 3: dDesignEPA = 200.0; break;
		case 4: dDesignEPA = 300.0; break;
		case 5: dDesignEPA = 400.0; break;
		default: ASSERT(0); break;
		}
	}
	else if (nInfCoefType == 2)
	{
		switch (nEffect)
		{
		case 0: dDesignEPA = 125.0; break;
		case 1: dDesignEPA = 220.0; break;
		case 2: dDesignEPA = 310.0; break;
		case 3: dDesignEPA = 400.0; break;
		case 4: dDesignEPA = 510.0; break;
		case 5: dDesignEPA = 620.0; break;
		default: ASSERT(0); break;
		}
	}
	else if (nInfCoefType == 3)
	{
		switch (nEffect)
		{
		case 0: dDesignEPA = 160.0; break;
		case 1: dDesignEPA = 320.0; break;
		case 2: dDesignEPA = 460.0; break;
		case 3: dDesignEPA = 600.0; break;
		case 4: dDesignEPA = 840.0; break;
		case 5: dDesignEPA = 1080.0; break;
		default: ASSERT(0); break;
		}
	}
	else { ASSERT(0); }
	dDesignEPA /= 980.6;
	return dDesignEPA;
}

void CThfcBatchImportItemDlg::OnEtcThfcAdd()
{
	int nCount = m_ListFile.GetCount();
	for (int nItem = 0; nItem < nCount; nItem++)
	{
		int nMask = m_ListFile.GetSel(nItem);
		if(nMask == 0)
			continue;
		ThfcCHData ThfcData;
		ThfcData = m_aThfcCHData[0][nItem];
		int nFileSuffix = 1;
		for (int j = 0; j < m_aThfcCHData[1].GetCount(); j++)
		{
			if (m_aThfcCHData[1][j].strName == ThfcData.strName)
			{
				ThfcData.strName.Format(_T("%s-%d"), m_aThfcCHData[0][nItem].strName, nFileSuffix);
				j = 0;
				nFileSuffix++;
				continue;
			}
			if(nFileSuffix > 20)
				break;
		}
		ThfcData.nIntensity = m_IntensityCom.GetCurSel();
		ThfcData.nEffect = m_EffectCom.GetCurSel();
		ThfcData.bAutoTime = m_bUserTime;
		ThfcData.bAdjust = m_bAdjust;
		if (m_bUserTime)
		{
			ThfcData.dStartTime = m_edtStartTime.GetEditValue();
			ThfcData.dEndTime = m_edtEndTime.GetEditValue();
		}
		if(m_bAdjust)
			ThfcData.dEPAScaleFactor = m_edtEPAScale.GetEditValue();
		else
			ThfcData.dEPAScaleFactor = 1.0;
		if (ThfcData.dEndTime - ThfcData.dStartTime < ThfcData.dStepTime)
		{
			MessageBox(_LS(IDS_CMD_THFC_IMPORT_ERROR__GREATER_STEP_TIME));
			continue;
		}
		ThfcData.nFileIndex = nItem;
		LVITEM lvitem;
		CString str;
		int nIndex = m_ListData.GetItemCount();
		for (int i = 0; i < 4; i++)
		{
			lvitem.iItem = nIndex;
			lvitem.iSubItem = i;
			if (i == 0)
				str = ThfcData.strName;
			else if (i == 1)
				str.Format(_T("%g"), ThfcData.dStartTime);
			else if (i == 2)
				str.Format(_T("%g"), ThfcData.dEndTime);
			else if (i == 3)
				str.Format(_T("%g"), ThfcData.dEPAScaleFactor);
			lvitem.pszText = str.GetBuffer(0);
			lvitem.mask = LVIF_TEXT;
			if (i == 0)
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)nIndex;
				nIndex = m_ListData.InsertItem(&lvitem);
			}
			else m_ListData.SetItem(&lvitem);
			str.ReleaseBuffer();
		}
		CArray<double, double> aOriginX, aOriginY;
		aOriginX.Copy(ThfcData.m_arTime);
		aOriginY.Copy(ThfcData.m_arAccel);
		ThfcData.m_arTime.RemoveAll();
		ThfcData.m_arAccel.RemoveAll();
		double dTempStartTime = 0.0;
		for (int i = 0; i < aOriginX.GetSize(); i++)
		{
			if (aOriginX[i] < ThfcData.dStartTime - 1.E-15)
			{
				dTempStartTime = aOriginX[i];
				continue;
			}
			if (aOriginX[i] > ThfcData.dEndTime + 1.E-15)
				break;
			ThfcData.m_arTime.Add(aOriginX[i] - dTempStartTime);
			ThfcData.m_arAccel.Add(aOriginY[i] * ThfcData.dEPAScaleFactor);
		}
		OnThfcRedrawGraph(ThfcData.m_arTime, ThfcData.m_arAccel);
		m_aThfcCHData[1].Add(ThfcData);
	}
}

void CThfcBatchImportItemDlg::OnEtcThfcEdit()
{
	POSITION pos = m_ListData.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	int nIndex;
	CArray<int, int> aDelIndex;
	while (pos)
	{
		nIndex = m_ListData.GetNextSelectedItem(pos);
		ThfcCHData& ThfcData = m_aThfcCHData[1][nIndex];// m_aThfcCHData[0][nFileIndex];
		int nFileIndex = ThfcData.nFileIndex;
		ThfcData.nIntensity = m_IntensityCom.GetCurSel();
		ThfcData.nEffect = m_EffectCom.GetCurSel();
		ThfcData.bAutoTime = m_bUserTime;
		ThfcData.bAdjust = m_bAdjust;
		if (m_bUserTime)
		{
			ThfcData.dStartTime = m_edtStartTime.GetEditValue();
			ThfcData.dEndTime = m_edtEndTime.GetEditValue();
		}
		else
		{
			ThfcData.dStartTime = m_aThfcCHData[0][ThfcData.nFileIndex].dStartTime;
			ThfcData.dEndTime = m_aThfcCHData[0][ThfcData.nFileIndex].dEndTime;
		}
		if(m_bAdjust)
			ThfcData.dEPAScaleFactor = m_edtEPAScale.GetEditValue();
		else
			ThfcData.dEPAScaleFactor = 1.0;
		if (ThfcData.dEndTime - ThfcData.dStartTime < ThfcData.dStepTime)
		{
			MessageBox(_LS(IDS_CMD_THFC_IMPORT_ERROR__GREATER_STEP_TIME));
			continue;
		}
		CArray<double, double> aOriginX, aOriginY;
		aOriginX.Copy(m_aThfcCHData[0][nFileIndex].m_arTime);
		aOriginY.Copy(m_aThfcCHData[0][nFileIndex].m_arAccel);
		ThfcData.m_arTime.RemoveAll();
		ThfcData.m_arAccel.RemoveAll();
		double dTempStartTime = 0.0;
		for (int i = 0; i < aOriginX.GetSize(); i++)
		{
			if (aOriginX[i] < ThfcData.dStartTime - 1.E-15)
			{
				dTempStartTime = aOriginX[i];
				continue;
			}
			if (aOriginX[i] > ThfcData.dEndTime + 1.E-15)
				break;
			ThfcData.m_arTime.Add(aOriginX[i] - dTempStartTime);
			ThfcData.m_arAccel.Add(aOriginY[i] * ThfcData.dEPAScaleFactor);
		}
		//
		LVITEM lvitem;
		CString str;
		for (int i = 1; i < 4; i++)
		{
			lvitem.iItem = nIndex;
			lvitem.iSubItem = i;
			if (i == 1)
				str.Format(_T("%g"), ThfcData.dStartTime);
			else if (i == 2)
				str.Format(_T("%g"), ThfcData.dEndTime);
			else if (i == 3)
				str.Format(_T("%g"), ThfcData.dEPAScaleFactor);
			lvitem.pszText = str.GetBuffer(0);
			lvitem.mask = LVIF_TEXT;
			m_ListData.SetItem(&lvitem);
			str.ReleaseBuffer();
		}
	}
	const ThfcCHData& ThfcData = m_aThfcCHData[1][nIndex];
	OnThfcRedrawGraph(ThfcData.m_arTime, ThfcData.m_arAccel);
}

void CThfcBatchImportItemDlg::OnEtcThfcDel()
{
	POSITION pos = m_ListData.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	int nIndex;
	CArray<int, int> aDelIndex;
	while (pos)
	{
		nIndex = m_ListData.GetNextSelectedItem(pos);
		aDelIndex.Add(nIndex);
	}

	for (int i = m_ListData.GetItemCount() - 1; i >= 0; i--)
	{
		bool bDel = false;
		for (int j = 0; j < aDelIndex.GetCount(); j++)
		{
			if (aDelIndex[j] == i)
			{
				bDel = true;
				break;
			}
		}
		if (bDel)
		{
			m_ListData.DeleteItem(i);
			m_aThfcCHData[1].RemoveAt(i);
		}
			
	}
}

void CThfcBatchImportItemDlg::OnCheckAdjust()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_THFC_CH_TIME2_EDIT3)->EnableWindow(m_bAdjust);
	GetDlgItem(IDC_THFC_COM1)->EnableWindow(m_bAdjust);
	GetDlgItem(IDC_THFC_COM2)->EnableWindow(m_bAdjust);
	if (m_bAdjust)
		OnSelChangeSeisCombox();
}

void CThfcBatchImportItemDlg::OnCheckUserTime()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_THFC_SCALE)->EnableWindow(m_bUserTime);
	GetDlgItem(IDC_THFC_SCALE2)->EnableWindow(m_bUserTime);
}

BOOL CThfcBatchImportItemDlg::EndEdit(BOOL bEnd, BOOL bDirectDrawLabel, unsigned int nCommand)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (bEnd && pDoc->m_pDataCtrl->CheckModifiedModel())
	{
		BOOL bUpdateDefault = FALSE;
		pDoc->m_pUndoCtrl->CloseEditDB(bUpdateDefault, bDirectDrawLabel, nCommand);
		return TRUE;
	}
	else
	{
		pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
}