// ReportSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ReportSetupDlg.h"
#include "ReportDefine.h"
#include "LoadWordLib.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportSetupDlg dialog


CReportSetupDlg::CReportSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CReportSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportSetupDlg)
	m_bRepeatHeader = FALSE;
	m_bExp          = FALSE;
	//m_bUseFooter    = FALSE;
	//}}AFX_DATA_INIT

	m_pTblH = NULL;
	m_pTblC = NULL;
	m_bUpdateTable = FALSE;
}

CReportSetupDlg::~CReportSetupDlg()
{
	if(m_pTblH!=NULL)
	{
		delete m_pTblH;
		m_pTblH = NULL;
	}
	if(m_pTblC!=NULL)
	{
		delete m_pTblC;
		m_pTblC = NULL;
	}
}

void CReportSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportSetupDlg)
	DDX_Check  (pDX, IDC_TM_REPORT_SETUP_TABLE_REPEAT_CHK, m_bRepeatHeader);
	DDX_Control(pDX, IDC_TM_REPORT_SETUP_TABLE_DECIMAL_EDIT, m_editDecimal);
	DDX_Check  (pDX, IDC_TM_REPORT_SETUP_TABLE_EXP_CHK, m_bExp);
	DDX_Control(pDX, IDC_TM_REPORT_SETUP_INITIALIZE_BTN, m_btnInit);
	DDX_Control(pDX, IDC_TM_REPORT_SETUP_TABLE_TAB, m_ctrlTab);
	//DDX_Check  (pDX, IDC_TM_REPORT_SETUP_TABLE_FOOTER_CHK, m_bUseFooter);

	DDX_Control(pDX, IDC_TEXT_CAPTION_LEFT_RDO, m_btnTxtAlignL);
	DDX_Control(pDX, IDC_TEXT_CAPTION_MIDDLE_RDO, m_btnTxtAlignC);
	DDX_Control(pDX, IDC_TEXT_CAPTION_RIGHT_RDO, m_btnTxtAlignR);
	DDX_Control(pDX, IDC_TEXT_UNDERLINE_CHK, m_fontTxtU);
	DDX_Control(pDX, IDC_TEXT_ITALIC_CHK, m_fontTxtI);
	DDX_Control(pDX, IDC_TEXT_BOLD_CHK, m_fontTxtB);
	DDX_Control(pDX, IDC_TEXT_CAPTION_FONT_COLOR_BTN, m_btnTxtColor);
	DDX_Control(pDX, IDC_TEXT_CAPTION_FONT_SIZE_CMB, m_cmbTxtFontSize);
	DDX_Control(pDX, IDC_TEXT_CAPTION_FONT_CMB, m_cmbTxtFont);

	DDX_Control(pDX, IDC_IMAGE_CAPTION_LEFT_RDO, m_btnCImgAlignL);
	DDX_Control(pDX, IDC_IMAGE_CAPTION_MIDDLE_RDO, m_btnCImgAlignC);
	DDX_Control(pDX, IDC_IMAGE_CAPTION_RIGHT_RDO, m_btnCImgAlignR);
	DDX_Control(pDX, IDC_IMAGE_UNDERLINE_CHK, m_fontCImgU);
	DDX_Control(pDX, IDC_IMAGE_ITALIC_CHK, m_fontCImgI);
	DDX_Control(pDX, IDC_IMAGE_BOLD_CHK, m_fontCImgB);
	DDX_Control(pDX, IDC_IMAGE_CAPTION_FONT_COLOR_BTN, m_btnCImgColor);
	DDX_Control(pDX, IDC_IMAGE_CAPTION_FONT_SIZE_CMB, m_cmbCImgFontSize);
	DDX_Control(pDX, IDC_IMAGE_CAPTION_FONT_CMB, m_cmbCImgFont);

	DDX_Control(pDX, IDC_TABLE_CAPTION_LEFT_RDO, m_btnCTblAlignL);
	DDX_Control(pDX, IDC_TABLE_CAPTION_MIDDLE_RDO, m_btnCTblAlignC);
	DDX_Control(pDX, IDC_TABLE_CAPTION_RIGHT_RDO, m_btnCTblAlignR);
	DDX_Control(pDX, IDC_TABLE_UNDERLINE_CHK, m_fontCTblU);
	DDX_Control(pDX, IDC_TABLE_ITALIC_CHK, m_fontCTblI);
	DDX_Control(pDX, IDC_TABLE_BOLD_CHK, m_fontCTblB);
	DDX_Control(pDX, IDC_TABLE_CAPTION_FONT_COLOR_BTN, m_btnCTblColor);
	DDX_Control(pDX, IDC_TABLE_CAPTION_FONT_SIZE_CMB, m_cmbCTblFontSize);
	DDX_Control(pDX, IDC_TABLE_CAPTION_FONT_CMB, m_cmbCTblFont);

	DDX_Control(pDX, IDC_FOOTER_CAPTION_LEFT_RDO, m_btnFtrAlignL);
	DDX_Control(pDX, IDC_FOOTER_CAPTION_MIDDLE_RDO, m_btnFtrAlignC);
	DDX_Control(pDX, IDC_FOOTER_CAPTION_RIGHT_RDO, m_btnFtrAlignR);
	DDX_Control(pDX, IDC_FOOTER_UNDERLINE_CHK, m_fontFtrU);
	DDX_Control(pDX, IDC_FOOTER_ITALIC_CHK, m_fontFtrI);
	DDX_Control(pDX, IDC_FOOTER_BOLD_CHK, m_fontFtrB);
	DDX_Control(pDX, IDC_FOOTER_CAPTION_FONT_COLOR_BTN, m_btnFtrColor);
	DDX_Control(pDX, IDC_FOOTER_CAPTION_FONT_SIZE_CMB, m_cmbFtrFontSize);
	DDX_Control(pDX, IDC_FOOTER_CAPTION_FONT_CMB, m_cmbFtrFont);
	DDX_Check(pDX, IDC_TM_ANALYOPT_UPDATE_TABLE_CHK, m_bUpdateTable);
//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportSetupDlg, CDialogMove)
	//{{AFX_MSG_MAP(CReportSetupDlg)
	ON_BN_CLICKED(IDC_TM_REPORT_SETUP_INITIALIZE_BTN, OnClickedInitialize)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TM_REPORT_SETUP_TABLE_TAB, OnSelchangeTableTab)

	ON_BN_CLICKED(IDC_TEXT_CAPTION_FONT_COLOR_BTN, OnTxtFontColor)
	ON_CBN_SELCHANGE(IDC_TEXT_CAPTION_FONT_CMB, OnSelchangeTxtFont)
	ON_CBN_SELCHANGE(IDC_TEXT_CAPTION_FONT_SIZE_CMB, OnSelchangeTxtFontSize)
	ON_BN_CLICKED(IDC_TEXT_CAPTION_LEFT_RDO, OnChangeTxtAlign)
	ON_BN_CLICKED(IDC_TEXT_CAPTION_MIDDLE_RDO, OnChangeTxtAlign)
	ON_BN_CLICKED(IDC_TEXT_CAPTION_RIGHT_RDO, OnChangeTxtAlign)
	ON_BN_CLICKED(IDC_TEXT_BOLD_CHK, OnCheckTxtCharType)
	ON_BN_CLICKED(IDC_TEXT_ITALIC_CHK, OnCheckTxtCharType)
	ON_BN_CLICKED(IDC_TEXT_UNDERLINE_CHK, OnCheckTxtCharType)

	ON_BN_CLICKED(IDC_IMAGE_CAPTION_FONT_COLOR_BTN, OnCImgFontColor)
	ON_CBN_SELCHANGE(IDC_IMAGE_CAPTION_FONT_CMB, OnSelchangeCImgFont)
	ON_CBN_SELCHANGE(IDC_IMAGE_CAPTION_FONT_SIZE_CMB, OnSelchangeCImgFontSize)
	ON_BN_CLICKED(IDC_IMAGE_CAPTION_LEFT_RDO, OnChangeCImgAlign)
	ON_BN_CLICKED(IDC_IMAGE_CAPTION_MIDDLE_RDO, OnChangeCImgAlign)
	ON_BN_CLICKED(IDC_IMAGE_CAPTION_RIGHT_RDO, OnChangeCImgAlign)
	ON_BN_CLICKED(IDC_IMAGE_BOLD_CHK, OnCheckCImgCharType)
	ON_BN_CLICKED(IDC_IMAGE_ITALIC_CHK, OnCheckCImgCharType)
	ON_BN_CLICKED(IDC_IMAGE_UNDERLINE_CHK, OnCheckCImgCharType)

	ON_BN_CLICKED(IDC_TABLE_CAPTION_FONT_COLOR_BTN, OnCTblFontColor)
	ON_CBN_SELCHANGE(IDC_TABLE_CAPTION_FONT_CMB, OnSelchangeCTblFont)
	ON_CBN_SELCHANGE(IDC_TABLE_CAPTION_FONT_SIZE_CMB, OnSelchangeCTblFontSize)
	ON_BN_CLICKED(IDC_TABLE_CAPTION_LEFT_RDO, OnChangeCTblAlign)
	ON_BN_CLICKED(IDC_TABLE_CAPTION_MIDDLE_RDO, OnChangeCTblAlign)
	ON_BN_CLICKED(IDC_TABLE_CAPTION_RIGHT_RDO, OnChangeCTblAlign)
	ON_BN_CLICKED(IDC_TABLE_BOLD_CHK, OnCheckCTblCharType)
	ON_BN_CLICKED(IDC_TABLE_ITALIC_CHK, OnCheckCTblCharType)
	ON_BN_CLICKED(IDC_TABLE_UNDERLINE_CHK, OnCheckCTblCharType)

	ON_BN_CLICKED(IDC_FOOTER_CAPTION_FONT_COLOR_BTN, OnFtrFontColor)
	ON_CBN_SELCHANGE(IDC_FOOTER_CAPTION_FONT_CMB, OnSelchangeFtrFont)
	ON_CBN_SELCHANGE(IDC_FOOTER_CAPTION_FONT_SIZE_CMB, OnSelchangeFtrFontSize)
	ON_BN_CLICKED(IDC_FOOTER_CAPTION_LEFT_RDO, OnChangeFtrAlign)
	ON_BN_CLICKED(IDC_FOOTER_CAPTION_MIDDLE_RDO, OnChangeFtrAlign)
	ON_BN_CLICKED(IDC_FOOTER_CAPTION_RIGHT_RDO, OnChangeFtrAlign)
	ON_BN_CLICKED(IDC_FOOTER_BOLD_CHK, OnCheckFtrCharType)
	ON_BN_CLICKED(IDC_FOOTER_ITALIC_CHK, OnCheckFtrCharType)
	ON_BN_CLICKED(IDC_FOOTER_UNDERLINE_CHK, OnCheckFtrCharType)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportSetupDlg message handlers

void CReportSetupDlg::OnOK() 
{
	UpdateData(TRUE);

	Dlg2Data();
	WriteRegistry();

	BOOL bIsLoaded=FALSE;
	bIsLoaded = CLoadWordLib::GetInstance()->IsLoadedWordLibrary();
	if(bIsLoaded)
	{
		ExportWordToSetup();
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->SetAutoExportUTBL(m_bUpdateTable);
	
	CDialogMove::OnOK();
}

BOOL CReportSetupDlg::OnInitDialog() 
{
	CWaitCursor wait;

	CDialogMove::OnInitDialog();

	InitControls();
	ReadRegistry();
	LayoutChildDlg();
	Data2Dlg();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	m_bUpdateTable = pDoc->GetAutoExportUTBL();
	GetDlgItem(IDC_TM_ANALYOPT_UPDATE_TABLE_CHK)->SetWindowTextW(_LS(IDS_CMD_ANALYOPT_SAVE_USER_DEFINED_DATA));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportSetupDlg::LayoutChildDlg()
{
	m_ctrlTab.DeleteAllItems();
	m_ctrlTab.InsertItem(0, _LS(IDS_TM_DYNA_REPORT_Header));
	m_ctrlTab.InsertItem(1, _LS(IDS_TM_DYNA_REPORT_Data));

	CRect rtText;
	GetDlgItem(IDC_TM_REPORT_SETUP_PLACEHOLDER)->GetWindowRect(rtText);
	ScreenToClient(rtText);  

	m_pTblH = new CFontSettingShadeDlg(&m_fdTblHeader, this);
	m_pTblH->Create(IDD_TM_REPORT_FONT_SETTING_SHADE, this );
	m_pTblH->MoveWindow(rtText);
	m_pTblH->ShowWindow(TRUE);

	m_pTblC = new CFontSettingShadeDlg(&m_fdTblCell, this);
	m_pTblC->Create(IDD_TM_REPORT_FONT_SETTING_SHADE, this );
	m_pTblC->MoveWindow(rtText);
	m_pTblC->ShowWindow(FALSE);
}

void CReportSetupDlg::InitControls() 
{
	//////////////////////////////////////////////
	// font Data setting
	m_fdTxt      .SetDefault();
	m_fdCImg     .SetDefault();
	m_fdCTbl     .SetDefault();
	m_fdTblHeader.SetDefault();
	m_fdTblCell  .SetDefault();
	m_fdFtr      .SetDefault();

	// Font
	SetFontCombo(m_cmbTxtFont , m_cmbTxtFontSize );
	SetFontCombo(m_cmbCImgFont, m_cmbCImgFontSize);
	SetFontCombo(m_cmbCTblFont, m_cmbCTblFontSize);
	SetFontCombo(m_cmbFtrFont , m_cmbFtrFontSize );
		
	// Align
	m_TxtAlignGroup.RemoveAll();
	m_TxtAlignGroup.Add(IDC_TEXT_CAPTION_LEFT_RDO);
	m_TxtAlignGroup.Add(IDC_TEXT_CAPTION_MIDDLE_RDO);
	m_TxtAlignGroup.Add(IDC_TEXT_CAPTION_RIGHT_RDO);  

	m_CImgAlignGroup.RemoveAll();
	m_CImgAlignGroup.Add(IDC_IMAGE_CAPTION_LEFT_RDO);
	m_CImgAlignGroup.Add(IDC_IMAGE_CAPTION_MIDDLE_RDO);
	m_CImgAlignGroup.Add(IDC_IMAGE_CAPTION_RIGHT_RDO);  

	m_CTblAlignGroup.RemoveAll();
	m_CTblAlignGroup.Add(IDC_TABLE_CAPTION_LEFT_RDO);
	m_CTblAlignGroup.Add(IDC_TABLE_CAPTION_MIDDLE_RDO);
	m_CTblAlignGroup.Add(IDC_TABLE_CAPTION_RIGHT_RDO);  

	m_FtrAlignGroup.RemoveAll();
	m_FtrAlignGroup.Add(IDC_FOOTER_CAPTION_LEFT_RDO);
	m_FtrAlignGroup.Add(IDC_FOOTER_CAPTION_MIDDLE_RDO);
	m_FtrAlignGroup.Add(IDC_FOOTER_CAPTION_RIGHT_RDO);  

	// Buttons
	SetImageFontButtons(m_fontTxtB, m_fontTxtI, m_fontTxtU, m_btnTxtColor, m_btnTxtAlignL, m_btnTxtAlignC, m_btnTxtAlignR);
	SetImageFontButtons(m_fontCImgB, m_fontCImgI, m_fontCImgU, m_btnCImgColor, m_btnCImgAlignL, m_btnCImgAlignC, m_btnCImgAlignR);
	SetImageFontButtons(m_fontCTblB, m_fontCTblI, m_fontCTblU, m_btnCTblColor, m_btnCTblAlignL, m_btnCTblAlignC, m_btnCTblAlignR);
	SetImageFontButtons(m_fontFtrB, m_fontFtrI, m_fontFtrU, m_btnFtrColor, m_btnFtrAlignL, m_btnFtrAlignC, m_btnFtrAlignR);
}

void CReportSetupDlg::SetFontCombo(MFontComboBox& cmbFont, MComboBox& cmbFontSize)
{
	//init font size combo box
	CString strSize;
	DWORD   aFontSize[] = {8,9,10,11,12,14,16,18,20,22,24,26,28,36,48,72};
	int nSize = sizeof(aFontSize)/sizeof(DWORD);

	cmbFont.SetCurSel(1); //init font combo box
	
	//init font size combo box
	for(int i=0; i<nSize; i++)
	{  
		strSize.Format(_T("%d"), aFontSize[i]);
		cmbFontSize.AddString(strSize);
	}    
	cmbFontSize.SetCurSel(2);
}

void CReportSetupDlg::SetImageFontButtons(CBCGPButton& btnB, CBCGPButton& btnI, CBCGPButton& btnU, CBCGPButton& btnCol, 
																					CBCGPButton& btnL, CBCGPButton& btnC, CBCGPButton& btnR)
{
	SetImageFontButtons(btnB, IDB_BTN_FONT_BOLD_24 );
	SetImageFontButtons(btnI, IDB_BTN_FONT_ITAL_24 );
	SetImageFontButtons(btnU, IDB_BTN_FONT_UNDE_24 );
	SetImageFontButtons(btnL, IDB_BTN_FONT_LEFT_24 );
	SetImageFontButtons(btnC, IDB_BTN_FONT_CENT_24 );
	SetImageFontButtons(btnR, IDB_BTN_FONT_RIGHT_24);
	
	btnCol.m_bDontUseWinXPTheme = TRUE;
	btnCol.m_nFlatStyle = CBCGPButton::FlatStyle(CBCGPButton::BUTTONSTYLE_SEMIFLAT);
}

void CReportSetupDlg::SetImageFontButtons(CBCGPButton& btn, UINT nBmpResId)
{
	btn.m_bTransparent = TRUE;
	btn.SetImage(nBmpResId);
	btn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	btn.SetWindowText(_T(""));
}

void CReportSetupDlg::ReadRegistryFontData(FontData& fdDefault, CString strSection, FontData& fdResult)
{
	int RedFactor   =0;
	int GreenFactor =0;
	int BlueFactor  =0;
	int nAlign      =0;
	
	fdResult.strFont    = AfxGetApp()->GetProfileString(strSection,_LSX(Font Name), fdDefault.strFont            );
	fdResult.nFontSize  = AfxGetApp()->GetProfileInt   (strSection,_LSX(Font Size), fdDefault.nFontSize          );
	RedFactor           = AfxGetApp()->GetProfileInt   (strSection,_LSX(Color R)  , GetRValue(fdDefault.dwColor ));
	GreenFactor         = AfxGetApp()->GetProfileInt   (strSection,_LSX(Color G)  , GetGValue(fdDefault.dwColor ));
	BlueFactor          = AfxGetApp()->GetProfileInt   (strSection,_LSX(Color B)  , GetBValue(fdDefault.dwColor ));
	fdResult.dwColor    = RGB(RedFactor, GreenFactor, BlueFactor); 
	nAlign              = AfxGetApp()->GetProfileInt   (strSection,_LSX(Align)    , (int)fdDefault.eAlign        );
	fdResult.eAlign     = (TEXT_ALIGN)nAlign;
	fdResult.bBold      = AfxGetApp()->GetProfileInt   (strSection,_LSX(Bold)     , fdDefault.bBold              );
	fdResult.bItalic    = AfxGetApp()->GetProfileInt   (strSection,_LSX(Italic)   , fdDefault.bItalic            );
	fdResult.bUnderline = AfxGetApp()->GetProfileInt   (strSection,_LSX(Underline), fdDefault.bUnderline         );
}

void CReportSetupDlg::ReadRegistryFontShadeData(FontShadeData& fdDefault, CString strSection, FontShadeData& fdResult)
{
	int RedFactor   =0;
	int GreenFactor =0;
	int BlueFactor  =0;
	int nAlign      =0;
	
	fdResult.strFont    = AfxGetApp()->GetProfileString(strSection, _LSX(Font Name), fdDefault.strFont            );
	fdResult.nFontSize  = AfxGetApp()->GetProfileInt   (strSection, _LSX(Font Size), fdDefault.nFontSize          );
	RedFactor           = AfxGetApp()->GetProfileInt   (strSection, _LSX(Color R)  , GetRValue(fdDefault.dwColor ));
	GreenFactor         = AfxGetApp()->GetProfileInt   (strSection, _LSX(Color G)  , GetGValue(fdDefault.dwColor ));
	BlueFactor          = AfxGetApp()->GetProfileInt   (strSection, _LSX(Color B)  , GetBValue(fdDefault.dwColor ));
	fdResult.dwColor    = RGB(RedFactor, GreenFactor, BlueFactor); 
	nAlign              = AfxGetApp()->GetProfileInt   (strSection, _LSX(Align)    , (int)fdDefault.eAlign        );
	fdResult.eAlign     = (TEXT_ALIGN)nAlign;
	fdResult.bBold      = AfxGetApp()->GetProfileInt   (strSection, _LSX(Bold)     , fdDefault.bBold              );
	fdResult.bItalic    = AfxGetApp()->GetProfileInt   (strSection, _LSX(Italic)   , fdDefault.bItalic            );
	fdResult.bUnderline = AfxGetApp()->GetProfileInt   (strSection, _LSX(Underline), fdDefault.bUnderline         );
	RedFactor           = AfxGetApp()->GetProfileInt   (strSection, _LSX(Shading R), GetRValue(fdDefault.dwShadeColor));
	GreenFactor         = AfxGetApp()->GetProfileInt   (strSection, _LSX(Shading G), GetGValue(fdDefault.dwShadeColor));
	BlueFactor          = AfxGetApp()->GetProfileInt   (strSection, _LSX(Shading B), GetBValue(fdDefault.dwShadeColor));
	fdResult.dwShadeColor= RGB(RedFactor, GreenFactor , BlueFactor); 
}

// #### CAMRWordView::MakeSetupStr() 도 같이 수정할것!!!!
void CReportSetupDlg::ReadRegistry() 
{
	FontData fdDefault; fdDefault.SetDefault();
	CString strSection=_T("");
	
	// Text Setup
#if defined(_ORG)
	fdDefault.strFont =_T("굴림체");
#endif
	strSection = _LSX(DynaGen Report\\Text);
	ReadRegistryFontData(fdDefault, strSection, m_fdTxt);
	
	// Caption Image
	fdDefault.SetDefault();
	fdDefault.bBold  = TRUE;
	fdDefault.eAlign = ALIGN_CENTER;
	strSection = _LSX(DynaGen Report\\Image Caption);
	ReadRegistryFontData(fdDefault, strSection, m_fdCImg);

	// Caption Table
	fdDefault.SetDefault();
	fdDefault.bBold  = TRUE;
	fdDefault.eAlign = ALIGN_LEFT;
	strSection = _LSX(DynaGen Report\\Table Caption);
	ReadRegistryFontData(fdDefault, strSection, m_fdCTbl);
	
	// Table Repeat
	strSection = _LSX(DynaGen Report\\Table);
	m_bRepeatHeader = AfxGetApp()->GetProfileInt(strSection, _LSX(Repeat header), 1);
	
	// Special Table
	strSection = _LSX(DynaGen Report\\Table);
	m_editDecimal.SetEditUnit( AfxGetApp()->GetProfileInt(strSection, _LSX(Special Decimal), 3) );
	m_bExp     = AfxGetApp()->GetProfileInt(strSection, _LSX(Special Exp), 0);
	
	// Table Header
	FontShadeData fdDefaultShade;
	fdDefaultShade.SetDefault();
	fdDefaultShade.bBold  = TRUE;
	fdDefaultShade.eAlign = ALIGN_CENTER;
	fdDefaultShade.dwShadeColor = RGB(225,225,225);
	strSection = _LSX(DynaGen Report\\Table\\Header);
	ReadRegistryFontShadeData(fdDefaultShade, strSection, m_fdTblHeader);

	// Table Cell
	fdDefaultShade.SetDefault();
	strSection = _LSX(DynaGen Report\\Table\\Cell);
	ReadRegistryFontShadeData(fdDefaultShade, strSection, m_fdTblCell);

	// Footer
	fdDefault.SetDefault();
	strSection = _LSX(DynaGen Report\\Footer);
	ReadRegistryFontData(fdDefault, strSection, m_fdFtr);
	//m_bUseFooter = AfxGetApp()->GetProfileInt(strSection, _LSX(Use Footer), 1);

	UpdateData(FALSE);
}

void CReportSetupDlg::WriteRegistryFontData(CString strSection, FontData& fdResult)
{
	AfxGetApp()->WriteProfileString(strSection ,_LSX(Font Name) , fdResult.strFont);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Font Size) , fdResult.nFontSize);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Color R)   , GetRValue(fdResult.dwColor));
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Color G)   , GetGValue(fdResult.dwColor));
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Color B)   , GetBValue(fdResult.dwColor));
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Align)     , fdResult.eAlign);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Bold)      , fdResult.bBold);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Italic)    , fdResult.bItalic);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Underline) , fdResult.bUnderline);
}

void CReportSetupDlg::WriteRegistryFontShadeData(CString strSection, FontShadeData& fdResult)
{
	AfxGetApp()->WriteProfileString(strSection ,_LSX(Font Name) , fdResult.strFont);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Font Size) , fdResult.nFontSize);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Color R)   , GetRValue(fdResult.dwColor));
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Color G)   , GetGValue(fdResult.dwColor));
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Color B)   , GetBValue(fdResult.dwColor));
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Align)     , fdResult.eAlign);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Bold)      , fdResult.bBold);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Italic)    , fdResult.bItalic);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Underline) , fdResult.bUnderline);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Shading R)   , GetRValue(fdResult.dwShadeColor));
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Shading G)   , GetGValue(fdResult.dwShadeColor));
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Shading B)   , GetBValue(fdResult.dwShadeColor));
}

void CReportSetupDlg::WriteRegistry() 
{
	CString strSection=_T("");
	
	// Text Setup
	strSection = _LSX(DynaGen Report\\Text);
	WriteRegistryFontData(strSection, m_fdTxt);

	// Caption Image
	strSection = _LSX(DynaGen Report\\Image Caption);
	WriteRegistryFontData(strSection, m_fdCImg);
	AfxGetApp()->WriteProfileInt(strSection ,_LSX(Position)  , 1);

	// Caption Table
	strSection = _LSX(DynaGen Report\\Table Caption);
	WriteRegistryFontData(strSection, m_fdCTbl);
	AfxGetApp()->WriteProfileInt(strSection ,_LSX(Position)  , 0);

	// Table Repeat
	strSection = _LSX(DynaGen Report\\Table);
	AfxGetApp()->WriteProfileInt(strSection ,_LSX(Repeat header) , m_bRepeatHeader);
	
	// Special Table
	strSection = _LSX(DynaGen Report\\Table);
	AfxGetApp()->WriteProfileInt(strSection, _LSX(Special Decimal), int(m_editDecimal.GetEditValue()));
	AfxGetApp()->WriteProfileInt(strSection, _LSX(Special Exp), m_bExp);
	
	// Table Header
	strSection = _LSX(DynaGen Report\\Table\\Header);
	WriteRegistryFontShadeData(strSection, m_fdTblHeader);

	// Table Cell
	strSection = _LSX(DynaGen Report\\Table\\Cell);
	WriteRegistryFontShadeData(strSection, m_fdTblCell);

	// Footer
	strSection = _LSX(DynaGen Report\\Footer);
	WriteRegistryFontData(strSection, m_fdFtr);
	AfxGetApp()->WriteProfileInt   (strSection ,_LSX(Use Footer) , 1);
}

void CReportSetupDlg::Data2Dlg() 
{
	//////////////////////////////////////////////////////////
	// Text
	if(!m_fdTxt.strFont.IsEmpty())
		m_cmbTxtFont.SelectFont(m_fdTxt.strFont);
	
	CString strTmp=_T("");
	strTmp.Format(_T("%d"), m_fdTxt.nFontSize);
	
	m_cmbTxtFontSize.SelectString(0, strTmp);
	m_btnTxtColor.SetFaceColor(m_fdTxt.dwColor);  
	
	m_fontTxtU.SetCheck(m_fdTxt.bUnderline);
	m_fontTxtI.SetCheck(m_fdTxt.bItalic);
	m_fontTxtB.SetCheck(m_fdTxt.bBold);
	
	CDlgUtil::CtrlRadioSetCheck(this, m_TxtAlignGroup, m_fdTxt.eAlign);
	
	//////////////////////////////////////////////////////////
	// Caption Image
	if(!m_fdCImg.strFont.IsEmpty())
		m_cmbCImgFont.SelectFont(m_fdCImg.strFont);
	
	strTmp.Format(_T("%d"), m_fdCImg.nFontSize);
	
	m_cmbCImgFontSize.SelectString(0, strTmp);
	m_btnCImgColor.SetFaceColor(m_fdCImg.dwColor);  
	
	m_fontCImgU.SetCheck(m_fdCImg.bUnderline);
	m_fontCImgI.SetCheck(m_fdCImg.bItalic);
	m_fontCImgB.SetCheck(m_fdCImg.bBold);
	
	CDlgUtil::CtrlRadioSetCheck(this, m_CImgAlignGroup, m_fdCImg.eAlign);

	//////////////////////////////////////////////////////////
	// Caption Table
	if(!m_fdCTbl.strFont.IsEmpty())
		m_cmbCTblFont.SelectFont(m_fdCTbl.strFont);
	
	strTmp.Format(_T("%d"), m_fdCTbl.nFontSize);
	
	m_cmbCTblFontSize.SelectString(0, strTmp);
	m_btnCTblColor.SetFaceColor(m_fdCTbl.dwColor);  
	
	m_fontCTblU.SetCheck(m_fdCTbl.bUnderline);
	m_fontCTblI.SetCheck(m_fdCTbl.bItalic);
	m_fontCTblB.SetCheck(m_fdCTbl.bBold);
	
	CDlgUtil::CtrlRadioSetCheck(this, m_CTblAlignGroup, m_fdCTbl.eAlign);

	//////////////////////////////////////////////////////////
	// Footer
	if(!m_fdFtr.strFont.IsEmpty())
		m_cmbFtrFont.SelectFont(m_fdFtr.strFont);
	
	strTmp.Format(_T("%d"), m_fdFtr.nFontSize);
	
	m_cmbFtrFontSize.SelectString(0, strTmp);
	m_btnFtrColor.SetFaceColor(m_fdFtr.dwColor);  
	
	m_fontFtrU.SetCheck(m_fdFtr.bUnderline);
	m_fontFtrI.SetCheck(m_fdFtr.bItalic);
	m_fontFtrB.SetCheck(m_fdFtr.bBold);
	
	CDlgUtil::CtrlRadioSetCheck(this, m_FtrAlignGroup, m_fdFtr.eAlign);

	UpdateData(FALSE);
}

void CReportSetupDlg::Dlg2Data() 
{
	UpdateData(TRUE);
}

void CReportSetupDlg::OnClickedInitialize() 
{
	m_fdTxt      .Initialize();
	m_fdCImg     .Initialize();
	m_fdCTbl     .Initialize();
	m_fdTblHeader.Initialize();
	m_fdTblCell  .Initialize();
	m_fdFtr      .Initialize();

#if defined(_ORG)
	m_fdTxt.strFont = _T("굴림체");
#endif

	m_fdCImg.bBold        = TRUE;
	m_fdCImg.eAlign       = ALIGN_CENTER;
	m_fdCTbl.bBold        = TRUE;
	m_fdCTbl.eAlign       = ALIGN_LEFT;
	m_fdTblHeader.bBold   = TRUE;
	m_fdTblHeader.eAlign  = ALIGN_CENTER;
	m_fdTblHeader.dwShadeColor = RGB(225,225,225);

	m_bRepeatHeader = TRUE;
	m_bExp          = FALSE;
	m_editDecimal.SetEditUnit(3);
	//m_bUseFooter    = TRUE;

	Data2Dlg();
	m_pTblH->Data2Dlg();
	m_pTblC->Data2Dlg();

	UpdateData(FALSE);
}

// #### CAMRWordView::MakeSetupStr() 도 같이 수정할것!!!!
CString CReportSetupDlg::MakeSetupStr()
{
	CString strSetup=_T(" ");

	// Apply Range
	CString strSetupStart=_T(""), strBold=_T(""), strItalic=_T("");
	strSetupStart = _ULS('[DRG_SETUP ApplyAll=true]\r\n'); // 문서의 현재위치 이하에 모두 적용할지 아니면 struct 1개에만 적용할지 여부 (default=true)

	// Text Setup
	CString strText=_T(""), strAlign=_T("");
	m_fdTxt.strFont.Replace(' ', '*');
	if      (m_fdTxt.eAlign==ALIGN_LEFT  ) strAlign = _LSX(left);
	else if (m_fdTxt.eAlign==ALIGN_CENTER) strAlign = _LSX(center);
	else if (m_fdTxt.eAlign==ALIGN_RIGHT ) strAlign = _LSX(right);
	else strAlign = _LSX(left);

	strBold   = m_fdTxt.bBold? _LSX(true):_LSX(false);
	strItalic = m_fdTxt.bItalic? _LSX(true):_LSX(false);

	strText.Format(_ULS('[DRG_TXT_PROPERTY family=%s size=%d bold=%s italic=%s underline=%d color=%ld align=%s][/DRG_TXT_PROPERTY]\r\n')
								, m_fdTxt.strFont, m_fdTxt.nFontSize, strBold, strItalic, m_fdTxt.bUnderline?1:0
								, m_fdTxt.dwColor, strAlign);

	// Caption Image
	CString strCapImage=_T(""), strPos=_T("");
	m_fdCImg.strFont.Replace(' ', '*');
	if      (m_fdCImg.eAlign==ALIGN_LEFT  ) strAlign = _LSX(left);
	else if (m_fdCImg.eAlign==ALIGN_CENTER) strAlign = _LSX(center);
	else if (m_fdCImg.eAlign==ALIGN_RIGHT ) strAlign = _LSX(right);
	else strAlign = _LSX(left);

	strPos = _LSX(bottom);
	
	strBold   = m_fdCImg.bBold? _LSX(true):_LSX(false);
	strItalic = m_fdCImg.bItalic? _LSX(true):_LSX(false);

	strCapImage.Format(_ULS('[DRG_IMGCAP_PROPERTY family=%s size=%d bold=%s italic=%s underline=%d color=%ld align=%s pos=%s][/DRG_IMGCAP_PROPERTY]\r\n')
		, m_fdCImg.strFont, m_fdCImg.nFontSize, strBold, strItalic, m_fdCImg.bUnderline?1:0
		, m_fdCImg.dwColor, strAlign, strPos);

	// Table Style
	CString strTBStyleStart=_T("");
	
	// Caption Table
	CString strCapTable=_T("");
	m_fdCTbl.strFont.Replace(' ', '*');
	if      (m_fdCTbl.eAlign==ALIGN_LEFT  ) strAlign = _LSX(left);
	else if (m_fdCTbl.eAlign==ALIGN_CENTER) strAlign = _LSX(center);
	else if (m_fdCTbl.eAlign==ALIGN_RIGHT ) strAlign = _LSX(right);
	else strAlign = _LSX(left);
 
	strPos = _LSX(top);

	strBold   = m_fdCTbl.bBold? _LSX(true):_LSX(false);
	strItalic = m_fdCTbl.bItalic? _LSX(true):_LSX(false);
	
	strCapTable.Format(_ULS('[DRG_TBLCAP_PROPERTY family=%s size=%d bold=%s italic=%s underline=%d color=%ld align=%s pos=%s][/DRG_TBLCAP_PROPERTY]\r\n')
		, m_fdCTbl.strFont, m_fdCTbl.nFontSize, strBold, strItalic, m_fdCTbl.bUnderline?1:0
		, m_fdCTbl.dwColor, strAlign, strPos);

	// Table Header
	CString strTableHeader=_T(""), strRepeatHeader=_T("");
	m_fdTblHeader.strFont.Replace(' ', '*');
	if      (m_fdTblHeader.eAlign==ALIGN_LEFT  ) strAlign = _LSX(left);
	else if (m_fdTblHeader.eAlign==ALIGN_CENTER) strAlign = _LSX(center);
	else if (m_fdTblHeader.eAlign==ALIGN_RIGHT ) strAlign = _LSX(right);
	else strAlign = _LSX(left);

	strBold   = m_fdTblHeader.bBold? _LSX(true):_LSX(false);
	strItalic = m_fdTblHeader.bItalic? _LSX(true):_LSX(false);
	strRepeatHeader = m_bRepeatHeader? _LSX(true):_LSX(false);

	int r=0,g=0,b=0;
	r = GetRValue(m_fdTblHeader.dwColor);
	g = GetGValue(m_fdTblHeader.dwColor);
	b = GetBValue(m_fdTblHeader.dwColor);
	COLORREF dwColor=0;
	dwColor = RGB(b, g, r);
	r = GetRValue(m_fdTblHeader.dwShadeColor);
	g = GetGValue(m_fdTblHeader.dwShadeColor);
	b = GetBValue(m_fdTblHeader.dwShadeColor);
	COLORREF dwShadeColor=0;
	dwShadeColor = RGB(b, g, r);

	strTableHeader.Format(_ULS('[DRG_TBLHEAD_PROPERTY family=%s size=%d bold=%s italic=%s underline=%d color=%ld align=%s shape=%d repeat=%s][/DRG_TBLHEAD_PROPERTY]\r\n')
		, m_fdTblHeader.strFont, m_fdTblHeader.nFontSize, strBold, strItalic, m_fdTblHeader.bUnderline?1:0
		, dwColor, strAlign, dwShadeColor, strRepeatHeader);

	// Table Cell
	CString strTableCell=_T("");
	m_fdTblCell.strFont.Replace(' ', '*');
	if      (m_fdTblCell.eAlign==ALIGN_LEFT  ) strAlign = _LSX(left);
	else if (m_fdTblCell.eAlign==ALIGN_CENTER) strAlign = _LSX(center);
	else if (m_fdTblCell.eAlign==ALIGN_RIGHT ) strAlign = _LSX(right);
	else strAlign = _LSX(left);

	strBold   = m_fdTblCell.bBold? _LSX(true):_LSX(false);
	strItalic = m_fdTblCell.bItalic? _LSX(true):_LSX(false);

	r=0,g=0,b=0;
	r = GetRValue(m_fdTblCell.dwColor);
	g = GetGValue(m_fdTblCell.dwColor);
	b = GetBValue(m_fdTblCell.dwColor);
	dwColor=0;
	dwColor = RGB(b, g, r);
	r = GetRValue(m_fdTblCell.dwShadeColor);
	g = GetGValue(m_fdTblCell.dwShadeColor);
	b = GetBValue(m_fdTblCell.dwShadeColor);
	dwShadeColor=0;
	dwShadeColor = RGB(b, g, r);

	strTableCell.Format(_ULS('[DRG_TBLCELL_PROPERTY family=%s size=%d bold=%s italic=%s underline=%d color=%ld align=%s shape=%d][/DRG_TBLCELL_PROPERTY]\r\n')
		, m_fdTblCell.strFont, m_fdTblCell.nFontSize, strBold, strItalic, m_fdTblCell.bUnderline?1:0
		, dwColor, strAlign, dwShadeColor);

	CString strTBStyleEnd=_T("");
	strTBStyleEnd = _ULS('[/DRG_TBLSTYLE_PROPERTY]\r\n');

	// Footer
	CString strFooter=_T("");
	m_fdFtr.strFont.Replace(' ', '*');
	if      (m_fdFtr.eAlign==ALIGN_LEFT  ) strAlign = _LSX(left);
	else if (m_fdFtr.eAlign==ALIGN_CENTER) strAlign = _LSX(center);
	else if (m_fdFtr.eAlign==ALIGN_RIGHT ) strAlign = _LSX(right);
	else strAlign = _LSX(left);
	
	strBold   = m_fdFtr.bBold? _LSX(true):_LSX(false);
	strItalic = m_fdFtr.bItalic? _LSX(true):_LSX(false);
	strRepeatHeader = _LSX(true);//m_bUseFooter? _LSX(true):_LSX(false);
	
	strFooter.Format(_ULS('[DRG_FOOTER_PROPERTY family=%s size=%d bold=%s italic=%s underline=%d color=%ld align=%s repeat=%s][/DRG_FOOTER_PROPERTY]\r\n')
		, m_fdFtr.strFont, m_fdFtr.nFontSize, strBold, strItalic, m_fdFtr.bUnderline?1:0
		, m_fdFtr.dwColor, strAlign, strRepeatHeader);

	CString strSetupEnd=_T("");
	strSetupEnd = _ULS('[/DRG_SETUP]\r\n');

	strSetup.Format(_T("%s%s%s%s%s%s%s%s%s%s"),
		strSetupStart, strText, strCapImage, strTBStyleStart, strCapTable, strTableHeader, strTableCell, strTBStyleEnd, strFooter, strSetupEnd);

	return strSetup;
} 

void CReportSetupDlg::ExportWordToSetup()
{
	// Make structure to export
	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL
	
	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[1];
	
#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif
	
	sCaption      = _T(" ");
	sType         = _LSX(SET);
	sFullPath     = _T(" ");
	sExtInfo      = _T(" ");
	sContents     = MakeSetupStr();
//   sContents     = "[DRG_SETUP ApplyAll=true]\r\n"
//     "[DRG_TXT_PROPERTY family=굴림체 size=9 bold=true italic=true underline=1 color=0 align=left][/DRG_TXT_PROPERTY]\r\n"
//     "[DRG_IMGCAP_PROPERTY family=궁서체 size=14 bold=true italic=true underline=1 color=65280 align=center pos=bottom][/DRG_IMGCAP_PROPERTY]\r\n"
//     "[DRG_TBLSTYLE_PROPERTY style=1]\r\n"
//     " [DRG_TBLCAP_PROPERTY family=돋움체 size=16 bold=true italic=true underline=1 color=255 align=center pos=top][/DRG_TBLCAP_PROPERTY]\r\n"
//     " [DRG_TBLHEAD_PROPERTY family=굴림체 size=12 bold=true italic=false underline=1 color=16711680 align=right shape=3355647 repeat=false][/DRG_TBLHEAD_PROPERTY]\r\n"
//     " [DRG_TBLCELL_PROPERTY family=굴림체 size=10 bold=false italic=true underline=1 color=255 align=left shape=0][/DRG_TBLCELL_PROPERTY]\r\n"
//     "[/DRG_TBLSTYLE_PROPERTY]\r\n"
//     "[/DRG_SETUP]\r\n";

	iAppendFlag   = D_WORD_APPEND_INSERT; 
	sExt          = _T(" ");
	
	CStringDeepCopyCollector SDCColl;

	pIftag[0].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
	pIftag[0].sCaption     = SDCColl.StringDeepCopy(sCaption    );
	pIftag[0].sType        = SDCColl.StringDeepCopy(sType       );
	pIftag[0].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
	pIftag[0].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
	pIftag[0].sContents    = SDCColl.StringDeepCopy(sContents   );
	pIftag[0].sExt         = SDCColl.StringDeepCopy(sExt        );
	pIftag[0].iAppendFlag  = iAppendFlag;

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod != NULL) 
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(1, &pIftag);
		pExport=NULL;
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}
	hMod = NULL;

	delete[] pIftag;
}

void CReportSetupDlg::OnSelchangeTableTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nSel = m_ctrlTab.GetCurSel();
	if(nSel==0)
	{
		m_pTblH->ShowWindow(TRUE);
		m_pTblC->ShowWindow(FALSE);
	}
	else if(nSel==1)
	{
		m_pTblH->ShowWindow(FALSE);
		m_pTblC->ShowWindow(TRUE);
	}
	else
	{
		ASSERT(0);
	}

	*pResult = 0;
}

// OnFontColor
void CReportSetupDlg::OnFontColor(FontData& fd, CBCGPButton& btnColor)
{
	COLORREF dwColor = fd.dwColor;
	
	CBCGPColorDialog dlg(dwColor, 0, this);
	if(dlg.DoModal() == IDOK)
	{
		dwColor = dlg.GetColor();
		if(dwColor != fd.dwColor)
		{
			btnColor.SetFaceColor(dwColor);
			fd.dwColor = dwColor;
		}		
	}
}
void CReportSetupDlg::OnTxtFontColor() 
{
	OnFontColor(m_fdTxt, m_btnTxtColor);
}
void CReportSetupDlg::OnCImgFontColor() 
{
	OnFontColor(m_fdCImg, m_btnCImgColor);
}
void CReportSetupDlg::OnCTblFontColor() 
{
	OnFontColor(m_fdCTbl, m_btnCTblColor);
}
void CReportSetupDlg::OnFtrFontColor() 
{
	OnFontColor(m_fdFtr, m_btnFtrColor);
}

// OnSelchangeFont
void CReportSetupDlg::OnSelchangeFont(FontData& fd, MFontComboBox& cmbFont)
{
	int nCursel = cmbFont.GetCurSel();
	if(nCursel< 0)
	{
		cmbFont.SetCurSel(1);
	}
	
	CBCGPFontDesc *pFD = cmbFont.GetSelFont();
	if(!pFD->m_strName.IsEmpty())
	{
		fd.strFont = pFD->m_strName;
	}
}
void CReportSetupDlg::OnSelchangeTxtFont() 
{
	OnSelchangeFont(m_fdTxt, m_cmbTxtFont);
}
void CReportSetupDlg::OnSelchangeCImgFont() 
{
	OnSelchangeFont(m_fdCImg, m_cmbCImgFont);
}
void CReportSetupDlg::OnSelchangeCTblFont() 
{
	OnSelchangeFont(m_fdCTbl, m_cmbCTblFont);
}
void CReportSetupDlg::OnSelchangeFtrFont() 
{
	OnSelchangeFont(m_fdFtr, m_cmbFtrFont);
}

// OnSelchangeFontSize
void CReportSetupDlg::OnSelchangeFontSize(FontData& fd, MComboBox& cmbFontSize)
{ 
	CString strSize=_T("");
	int nSel = cmbFontSize.GetCurSel();
	cmbFontSize.GetLBText(nSel, strSize);
	
	fd.nFontSize = _ttoi(strSize);
}
void CReportSetupDlg::OnSelchangeTxtFontSize() 
{ 
	OnSelchangeFontSize(m_fdTxt, m_cmbTxtFontSize);
}
void CReportSetupDlg::OnSelchangeCImgFontSize() 
{ 
	OnSelchangeFontSize(m_fdCImg, m_cmbCImgFontSize);
}
void CReportSetupDlg::OnSelchangeCTblFontSize() 
{ 
	OnSelchangeFontSize(m_fdCTbl, m_cmbCTblFontSize);
}
void CReportSetupDlg::OnSelchangeFtrFontSize() 
{ 
	OnSelchangeFontSize(m_fdFtr, m_cmbFtrFontSize);
}

// OnChangeAlign
void CReportSetupDlg::OnChangeAlign(FontData& fd, CArray<UINT,UINT>& AlignGroup)
{
	int nVal=0;
	CDlgUtil::CtrlRadioGetCheck(this, AlignGroup, nVal);
	
	fd.eAlign = (TEXT_ALIGN)nVal;	
}
void CReportSetupDlg::OnChangeTxtAlign() 
{
	OnChangeAlign(m_fdTxt, m_TxtAlignGroup);
}
void CReportSetupDlg::OnChangeCImgAlign() 
{
	OnChangeAlign(m_fdCImg, m_CImgAlignGroup);
}
void CReportSetupDlg::OnChangeCTblAlign() 
{
	OnChangeAlign(m_fdCTbl, m_CTblAlignGroup);
}
void CReportSetupDlg::OnChangeFtrAlign() 
{
	OnChangeAlign(m_fdFtr, m_FtrAlignGroup);
}

// OnCheckCharType
void CReportSetupDlg::OnCheckCharType(FontData& fd, CBCGPButton& btnB, CBCGPButton& btnI, CBCGPButton& btnU)
{
	fd.bBold      = btnB.GetCheck();
	fd.bItalic    = btnI.GetCheck();
	fd.bUnderline = btnU.GetCheck(); 
}
void CReportSetupDlg::OnCheckTxtCharType() 
{
	OnCheckCharType(m_fdTxt, m_fontTxtB, m_fontTxtI, m_fontTxtU);
}
void CReportSetupDlg::OnCheckCImgCharType() 
{
	OnCheckCharType(m_fdCImg, m_fontCImgB, m_fontCImgI, m_fontCImgU);
}
void CReportSetupDlg::OnCheckCTblCharType() 
{
	OnCheckCharType(m_fdCTbl, m_fontCTblB, m_fontCTblI, m_fontCTblU);
}
void CReportSetupDlg::OnCheckFtrCharType() 
{
	OnCheckCharType(m_fdFtr, m_fontFtrB, m_fontFtrI, m_fontFtrU);
}
