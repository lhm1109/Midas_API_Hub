// FontSettingShadeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "FontSettingShadeDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontSettingShadeDlg dialog

#define CBCGPDialog CChildDialog//CDialog

CFontSettingShadeDlg::CFontSettingShadeDlg(FontShadeData* fd, CWnd* pParent)
: CBCGPDialog(CFontSettingShadeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFontSettingShadeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_pFontData = &fd;
	//m_pParent = pParent;

	m_pFontData = fd;
}


void CFontSettingShadeDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFontSettingShadeDlg)
	DDX_Control(pDX, IDC_CAPTION_LEFT_RDO, m_btnAlignL);
	DDX_Control(pDX, IDC_CAPTION_MIDDLE_RDO, m_btnAlignC);
	DDX_Control(pDX, IDC_CAPTION_RIGHT_RDO, m_btnAlignR);
	DDX_Control(pDX, IDC_UNDERLINE_CHK, m_fontU);
	DDX_Control(pDX, IDC_ITALIC_CHK, m_fontI);
	DDX_Control(pDX, IDC_BOLD_CHK, m_fontB);
	DDX_Control(pDX, IDC_CAPTION_FONT_COLOR_BTN, m_btnColor);
	DDX_Control(pDX, IDC_CAPTION_FONT_SIZE_CMB, m_cmbFontSize);
	DDX_Control(pDX, IDC_CAPTION_FONT_CMB, m_cmbFont);
	DDX_Control(pDX, IDC_TM_TABLE_COLOR_BTN, m_btnColorShade);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFontSettingShadeDlg, CBCGPDialog)
//{{AFX_MSG_MAP(CFontSettingShadeDlg)
	ON_BN_CLICKED(IDC_CAPTION_FONT_COLOR_BTN, OnBtnCaptionFontColor)
	ON_CBN_SELCHANGE(IDC_CAPTION_FONT_CMB, OnSelchangeCmbCaptionFont)
	ON_CBN_SELCHANGE(IDC_CAPTION_FONT_SIZE_CMB, OnSelchangeCmbCaptionFontSize)
	ON_BN_CLICKED(IDC_CAPTION_LEFT_RDO, OnChangeAlign)
	ON_BN_CLICKED(IDC_CAPTION_MIDDLE_RDO, OnChangeAlign)
	ON_BN_CLICKED(IDC_CAPTION_RIGHT_RDO, OnChangeAlign)
	ON_BN_CLICKED(IDC_BOLD_CHK, OnCheckCharType)
	ON_BN_CLICKED(IDC_ITALIC_CHK, OnCheckCharType)
	ON_BN_CLICKED(IDC_UNDERLINE_CHK, OnCheckCharType)
	ON_BN_CLICKED(IDC_TM_TABLE_COLOR_BTN , OnTableShading)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontSettingShadeDlg message handlers

BOOL CFontSettingShadeDlg::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitControls();
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFontSettingShadeDlg::DestroyWindow() 
{ 
	return CBCGPDialog::DestroyWindow();  
}

void CFontSettingShadeDlg::InitControls()
{ 
	//init font combo box
	m_cmbFont.SetCurSel(1); 

	//init font size combo box
	CString strSize;
	DWORD   aFontSize[] = {8,9,10,11,12,14,16,18,20,22,24,26,28,36,48,72};
	int nSize = sizeof(aFontSize)/sizeof(DWORD);
	for(int i=0; i<nSize; i++)
	{  
		strSize.Format(_T("%d"), aFontSize[i]);
		m_cmbFontSize.AddString(strSize);
	}    
	m_cmbFontSize.SetCurSel(2);

	//align
	m_AlignGroup.RemoveAll();
	m_AlignGroup.Add(IDC_CAPTION_LEFT_RDO);
	m_AlignGroup.Add(IDC_CAPTION_MIDDLE_RDO);
	m_AlignGroup.Add(IDC_CAPTION_RIGHT_RDO);  

	// Buttons
	m_fontB.m_bTransparent = TRUE;
	m_fontB.SetImage(IDB_BTN_FONT_BOLD_24);
	m_fontB.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_fontB.SetWindowText(_T(""));
	//m_fontB.Invalidate();

	m_fontI.m_bTransparent = TRUE;
	m_fontI.SetImage(IDB_BTN_FONT_ITAL_24);
	m_fontI.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_fontI.SetWindowText(_T(""));
	//m_fontI.Invalidate();

	m_fontU.m_bTransparent = TRUE;
	m_fontU.SetImage(IDB_BTN_FONT_UNDE_24);
	m_fontU.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_fontU.SetWindowText(_T(""));
	//m_fontU.Invalidate();

	m_btnColor.m_bDontUseWinXPTheme = TRUE;
	m_btnColor.m_nFlatStyle = CBCGPButton::FlatStyle(CBCGPButton::BUTTONSTYLE_SEMIFLAT);

	m_btnAlignL.m_bTransparent = TRUE;
	m_btnAlignL.SetImage(IDB_BTN_FONT_LEFT_24);
	m_btnAlignL.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_btnAlignL.SetWindowText(_T(""));
	//m_btnAlignL.Invalidate();

	m_btnAlignC.m_bTransparent = TRUE;
	m_btnAlignC.SetImage(IDB_BTN_FONT_CENT_24);
	m_btnAlignC.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_btnAlignC.SetWindowText(_T(""));
	//m_btnAlignC.Invalidate();

	m_btnAlignR.m_bTransparent = TRUE;
	m_btnAlignR.SetImage(IDB_BTN_FONT_RIGHT_24);
	m_btnAlignR.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_btnAlignR.SetWindowText(_T(""));
	//m_btnAlignR.Invalidate();

	m_btnColorShade.m_bDontUseWinXPTheme = TRUE;
	m_btnColorShade.m_nFlatStyle = CBCGPButton::FlatStyle(CBCGPButton::BUTTONSTYLE_SEMIFLAT);
}

void CFontSettingShadeDlg::Data2Dlg()
{
	if(!m_pFontData->strFont.IsEmpty())
		m_cmbFont.SelectFont(m_pFontData->strFont);

	CString strTmp=_T("");
	strTmp.Format(_T("%d"), m_pFontData->nFontSize);
	
	m_cmbFontSize.SelectString(0,strTmp);
	m_btnColor.SetFaceColor(m_pFontData->dwColor);  
	m_btnColorShade.SetFaceColor(m_pFontData->dwShadeColor);  

	m_fontU.SetCheck(m_pFontData->bUnderline);
	m_fontI.SetCheck(m_pFontData->bItalic);
	m_fontB.SetCheck(m_pFontData->bBold);

	CDlgUtil::CtrlRadioSetCheck(this, m_AlignGroup, m_pFontData->eAlign);

//   OnSelchangeCmbCaptionFont(); 
//   OnChangeAlign();  
//   OnCheckCharType();
}

void CFontSettingShadeDlg::Dlg2Data()
{
}

void CFontSettingShadeDlg::OnBtnCaptionFontColor() 
{
	COLORREF dwColor = m_pFontData->dwColor;

	CBCGPColorDialog dlg (dwColor, 0, this);
	if (dlg.DoModal () == IDOK)
	{
		dwColor = dlg.GetColor ();
		if(dwColor != m_pFontData->dwColor)
		{
			m_btnColor.SetFaceColor(dwColor);
			m_pFontData->dwColor = dwColor;
		}		
	}
}

void CFontSettingShadeDlg::OnSelchangeCmbCaptionFont() 
{
	int nCursel = m_cmbFont.GetCurSel();
	if(nCursel< 0)
	{
		m_cmbFont.SetCurSel(1);
	}

	CBCGPFontDesc *pFD = m_cmbFont.GetSelFont();
	if(!pFD->m_strName.IsEmpty())
	{
		m_pFontData->strFont = pFD->m_strName;
	}
}

void CFontSettingShadeDlg::OnSelchangeCmbCaptionFontSize() 
{ 
	CString strSize=_T("");
	int nSel = m_cmbFontSize.GetCurSel();
	m_cmbFontSize.GetLBText( nSel , strSize);
	
	m_pFontData->nFontSize = _ttoi(strSize);
}

void CFontSettingShadeDlg::OnChangeAlign() 
{
	int nVal=0;
	CDlgUtil::CtrlRadioGetCheck(this , m_AlignGroup , nVal);

	m_pFontData->eAlign = (TEXT_ALIGN)nVal;	
}

void CFontSettingShadeDlg::OnCheckCharType() 
{
	m_pFontData->bBold      = m_fontB.GetCheck();
	m_pFontData->bItalic    = m_fontI.GetCheck();
	m_pFontData->bUnderline =	m_fontU.GetCheck(); 
}

void CFontSettingShadeDlg::OnTableShading() 
{
	UpdateData(TRUE);

	COLORREF dwColor = m_pFontData->dwShadeColor;
	CBCGPColorDialog dlg (dwColor, 0, this);
	if (dlg.DoModal () == IDOK)
	{
		dwColor = dlg.GetColor ();
		if(dwColor != m_pFontData->dwShadeColor)
		{
			m_btnColorShade.SetFaceColor(dwColor);
			m_pFontData->dwShadeColor = dwColor;
		}		
	}
}