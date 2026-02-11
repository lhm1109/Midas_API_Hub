// FontSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "FontSettingDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CFontSettingDlg::m_nDefaultFontIndex = 0;
CString CFontSettingDlg::m_strDefaultFontName = _T("");
/////////////////////////////////////////////////////////////////////////////
// CFontSettingDlg dialog

#define CBCGPDialog CDialog

CFontSettingDlg::CFontSettingDlg(FontData &fd, CWnd* pParent)
: CBCGPDialog(CFontSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFontSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_pTargetEdit = pTarget;
	m_pFontData = &fd;
	m_pParent = pParent;
}


void CFontSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFontSettingDlg)
	DDX_Control(pDX, IDC_CAPTION_LEFT_RDO, m_btnAlignLeft);
	DDX_Control(pDX, IDC_CAPTION_MIDDLE_RDO, m_btnAlignCent);
	DDX_Control(pDX, IDC_CAPTION_RIGHT_RDO, m_btnAlignRight);
	DDX_Control(pDX, IDC_UNDERLINE_CHK, m_fontUnderline);
	DDX_Control(pDX, IDC_ITALIC_CHK, m_fontItalic);
	DDX_Control(pDX, IDC_BOLD_CHK, m_fontBold);
	DDX_Control(pDX, IDC_CAPTION_FONT_COLOR_BTN, m_btnColor);
	DDX_Control(pDX, IDC_CAPTION_FONT_SIZE_CMB, m_cmbFontSize);
	DDX_Control(pDX, IDC_CAPTION_FONT_CMB, m_cmbFont);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFontSettingDlg, CBCGPDialog)
//{{AFX_MSG_MAP(CFontSettingDlg)
	ON_BN_CLICKED(IDC_CAPTION_FONT_COLOR_BTN, OnBtnCaptionFontColor)
	ON_CBN_SELCHANGE(IDC_CAPTION_FONT_CMB, OnSelchangeCmbCaptionFont)
	ON_CBN_SELCHANGE(IDC_CAPTION_FONT_SIZE_CMB, OnSelchangeCmbCaptionFontSize)
	ON_BN_CLICKED(IDC_CAPTION_LEFT_RDO, OnChangeAlign)
	ON_BN_CLICKED(IDC_CAPTION_MIDDLE_RDO, OnChangeAlign)
	ON_BN_CLICKED(IDC_CAPTION_RIGHT_RDO, OnChangeAlign)
	ON_BN_CLICKED(IDC_BOLD_CHK, OnCheckCharType)
	ON_BN_CLICKED(IDC_ITALIC_CHK, OnCheckCharType)
	ON_BN_CLICKED(IDC_UNDERLINE_CHK, OnCheckCharType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontSettingDlg message handlers

BOOL CFontSettingDlg::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	
	m_btnColor.m_bDontUseWinXPTheme = TRUE;
	m_btnColor.m_nFlatStyle = CBCGPButton::FlatStyle(CBCGPButton::BUTTONSTYLE_3D);
	m_cmbFont.SetCurSel(1); 
	m_nDefaultFontIndex = 1;
	CBCGPFontDesc *pFD = m_cmbFont.GetSelFont();
	if(!pFD->m_strName.IsEmpty())
	{
		//m_strDefaultFontName   = pFD->m_strName;
	}
	else
	{
		ASSERT(0);
	}

	// TODO: Add extra initialization here
	//Move Position 
	InitWindowPos();
	InitControls();
	Data2Dlg();

	//InitFont
//  CFont *font = m_pTargetEdit->GetFont();
//  LOGFONT logFont;
//  font->GetLogFont(&logFont);

	
//  m_cmbFont.SelectFont((logFont).lfFaceName);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFontSettingDlg::PreCreateWindow(CREATESTRUCT& cs) 
{ 
	/*
	CRect rtTarget;
	m_pTargetEdit->GetWindowRect(rtTarget);
	cs.x = rtTarget.left;
	cs.y = rtTarget.top - cs.cy;
	*/ 
	
	return CBCGPDialog::PreCreateWindow(cs);
}

void CFontSettingDlg::InitWindowPos()
{
	CRect rtTarget,rtThis;
	this->GetWindowRect(rtThis);
	m_pFontData->pTargetWnd->GetWindowRect(rtTarget);
	int nWidth = rtThis.Width();
	int nHeight= rtThis.Height();
	rtThis.left = rtTarget.left;
	rtThis.right= rtTarget.left + nWidth;
	rtThis.top  = rtTarget.top-nHeight;
	rtThis.bottom = rtTarget.top;
	MoveWindow(rtThis);
}

void CFontSettingDlg::DisableFontSize(int nFontSize)
{
	//테이블, 이미지, 그래프인 경우는 5~10 사이로 결정하도록 함
	//init font combo box
	//init font size combo box
	int i;
	BOOL bFind = FALSE;
	CString strSize;
	DWORD   aFontSize[] = {5, 6, 7, 8, 9, 10};
	int nSize = sizeof(aFontSize)/sizeof(DWORD);
	
	m_cmbFontSize.ResetContent();
	for(i=0 ; i<nSize  ; i++)
	{  
		strSize.Format(_T("%d"), aFontSize[i]);
		m_cmbFontSize.AddString(strSize);
	}    

	//nFontSize값으로 초기화하고 없으면 10으로 초기화 
	for(int i=0; i<nSize; i++)
	{
		if(aFontSize[i] == nFontSize)
		{
			bFind = TRUE;
			m_cmbFontSize.SetCurSel(i);
		}
	}

	if(!bFind) m_cmbFontSize.SetCurSel(5); //10
	//m_cmbFontSize.EnableWindow(FALSE);
}

void CFontSettingDlg::InitFontCombo()
{
}

BOOL CFontSettingDlg::DestroyWindow() 
{ 
	
	return CBCGPDialog::DestroyWindow();  
}

void CFontSettingDlg::InitControls()
{ 

	//init font combo box
	//init font size combo box
	CString strSize;
	DWORD   aFontSize[] = {8 ,9 ,10 ,11 ,12 ,14 ,16 ,18 ,20 ,22 ,24 ,26 ,28 ,36 ,48 ,72};
	int nSize = sizeof(aFontSize)/sizeof(DWORD);
	for(int i=0 ; i<nSize  ; i++)
	{  
		strSize.Format(_T("%d"), aFontSize[i]);
		m_cmbFontSize.AddString(strSize);
	}    
	m_cmbFontSize.SetCurSel(2);
	//color  

	//align
	m_AlignGroup.RemoveAll();
	m_AlignGroup.Add(IDC_CAPTION_LEFT_RDO);
	m_AlignGroup.Add(IDC_CAPTION_MIDDLE_RDO);
	m_AlignGroup.Add(IDC_CAPTION_RIGHT_RDO);  


	// Buttons
	m_fontBold.m_bTransparent = TRUE;
	m_fontBold.SetImage (IDB_BTN_FONT_BOLD_24);
	m_fontBold.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_fontBold.SetWindowText (_T(""));
	//m_fontBold.SizeToContent ();
	m_fontBold.Invalidate ();

	m_fontItalic.m_bTransparent = TRUE;
	m_fontItalic.SetImage (IDB_BTN_FONT_ITAL_24);
	m_fontItalic.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_fontItalic.SetWindowText (_T(""));
	//m_fontItalic.SizeToContent ();
	m_fontItalic.Invalidate ();

	m_fontUnderline.m_bTransparent = TRUE;
	m_fontUnderline.SetImage (IDB_BTN_FONT_UNDE_24);
	m_fontUnderline.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_fontUnderline.SetWindowText (_T(""));
	//m_fontUnderline.SizeToContent ();
	m_fontUnderline.Invalidate ();


	//m_btnColor.m_bTransparent = TRUE;
	m_btnColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	//m_btnColor.SetWindowText (_T(""));
	//m_btnColor.Invalidate ();

	m_btnAlignLeft.m_bTransparent = TRUE;
	m_btnAlignLeft.SetImage (IDB_BTN_FONT_LEFT_24);
	m_btnAlignLeft.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_btnAlignLeft.SetWindowText (_T(""));
	m_btnAlignLeft.Invalidate ();

	m_btnAlignCent.m_bTransparent = TRUE;
	m_btnAlignCent.SetImage (IDB_BTN_FONT_CENT_24);
	m_btnAlignCent.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_btnAlignCent.SetWindowText (_T(""));
	m_btnAlignCent.Invalidate ();

	m_btnAlignRight.m_bTransparent = TRUE;
	m_btnAlignRight.SetImage (IDB_BTN_FONT_RIGHT_24);
	m_btnAlignRight.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
	m_btnAlignRight.SetWindowText (_T(""));
	m_btnAlignRight.Invalidate ();
	
}

void CFontSettingDlg::SetFontData(FontData &fd)
{
	m_pFontData = &fd;
	Data2Dlg();
}


void CFontSettingDlg::Data2Dlg()
{
	if(!m_pFontData->strFont.IsEmpty())
		m_cmbFont.SelectFont( m_pFontData->strFont );

	CString strTmp;
	strTmp.Format(_T("%d"), m_pFontData->nFontSize);
	
	m_cmbFontSize.SelectString(0,strTmp);
	m_btnColor.SetFaceColor( m_pFontData->dwColor );  

	m_fontUnderline.SetCheck( m_pFontData->bUnderline);
	m_fontItalic.SetCheck( m_pFontData->bItalic);
	m_fontBold.SetCheck( m_pFontData->bBold);

	CDlgUtil::CtrlRadioSetCheck( this, m_AlignGroup , m_pFontData->eAlign);

	///////////////////////////////////////////////////////////////////
	OnSelchangeCmbCaptionFont(); 
	m_pFontData->pTargetWnd->SetTextColor( m_pFontData->dwColor);
	OnChangeAlign();  
	OnCheckCharType();
	
}

void CFontSettingDlg::Dlg2Data()
{
	
}

void CFontSettingDlg::OnBtnCaptionFontColor() 
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
	
	m_pFontData->pTargetWnd->SetTextColor( dwColor);
	
}

void CFontSettingDlg::OnSelchangeCmbCaptionFont() 
{
	int nCursel = m_cmbFont.GetCurSel();
	if(nCursel< 0)
	{
		m_cmbFont.SetCurSel(1);
	}

	CBCGPFontDesc *pFD = m_cmbFont.GetSelFont();
	CFont newFont, *pFont = m_pFontData->pTargetWnd->GetFont();
	
	LOGFONT lfNew;
	pFont->GetLogFont(&lfNew);
	
	if(!pFD->m_strName.IsEmpty())
	{
		m_pFontData->pTargetWnd->SetFont(pFD->m_strName);
		m_pFontData->strFont = pFD->m_strName;
	}
}

void CFontSettingDlg::OnSelchangeCmbCaptionFontSize() 
{ 
	CString strSize;
	int nSel = m_cmbFontSize.GetCurSel();
	m_cmbFontSize.GetLBText( nSel , strSize);
	
	m_pFontData->nFontSize = _ttoi(strSize);
}

void CFontSettingDlg::OnChangeAlign() 
{
	int nVal;
	CDlgUtil::CtrlRadioGetCheck(this , m_AlignGroup , nVal);

	m_pFontData->eAlign = (TEXT_ALIGN)nVal;	

	//m_pFontData->pTargetWnd->SetAlign(nVal);
}

void CFontSettingDlg::OnCheckCharType() 
{
	m_pFontData->bBold      = m_fontBold.GetCheck();
	m_pFontData->bItalic    = m_fontItalic.GetCheck();
	m_pFontData->bUnderline =	m_fontUnderline.GetCheck(); 

	//m_pFontData->pTargetWnd->SetStyle( m_pFontData->bBold , m_pFontData->bItalic , m_pFontData->bUnderline);
}
/*
void CFontSettingDlg::GetDefaultFont(CUSTOM_RESULT_TYPE rt, EDIT_TYPE et ,FontData& defaultFont)
{
	CString regSection;

	
	switch(rt)
	{
	case CUSTOM_RESULT_SEPERATOR:  
	case CUSTOM_RESULT_GROUP   :  regSection = _T("AMR_Group");  	break;
	case CUSTOM_RESULT_MEMO    :  regSection = _T("AMR_Memo");	  break;
	case CUSTOM_RESULT_USER_IMG:
	case CUSTOM_RESULT_IMAGE   :  regSection = _T("AMR_Image") ; 	break;
	case CUSTOM_RESULT_TABLE   :  regSection = _T("AMR_Table") ; 	break;
	case CUSTOM_RESULT_GRAPH   :  regSection = _T("AMR_Graph") ; 	break;
	case CUSTOM_RESULT_USER_TXT:
	case CUSTOM_RESULT_STRING  :  regSection = _T("AMR_Text") ; 	break;
	case CUSTOM_RESULT_INC_WORD:  regSection = _T("AMR_Word") ; 	break;
	default: ASSERT(0); break;
	}

	switch(et) 
	{
	case EDIT_CAPTION:  	            regSection+= _T("_Caption") ;             break;
	case EDIT_DESCRIPTION:            regSection+= _T("_Description");	        break;
	case EDIT_TABLE_ROWTITLEFONT:     regSection+= _T("_RowTitleFont");	        break;
	case EDIT_TABLE_COLTITLEFONT:     regSection+= _T("_ColTitleFont");	        break;
	case EDIT_TABLE_CELLTITLEFONT:    regSection+= _T("_CellTitleFont");	      break;
	default: ASSERT(0); break;
	}


	FontData amrDefaultFont;
	GetCommonDefaultFont(amrDefaultFont, rt, et);
	

	defaultFont.strFont   = AfxGetApp()->GetProfileString(regSection ,_T("FontName") , amrDefaultFont.strFont);
	defaultFont.nFontSize = AfxGetApp()->GetProfileInt(regSection ,_T("fontsize") , amrDefaultFont.nFontSize);
	int RedFactor         = AfxGetApp()->GetProfileInt(regSection ,_T("ColorR")   , GetRValue(amrDefaultFont.dwColor));
	int GreenFactor       = AfxGetApp()->GetProfileInt(regSection ,_T("ColorG")   , GetGValue(amrDefaultFont.dwColor));
	int BlueFactor        = AfxGetApp()->GetProfileInt(regSection ,_T("ColorB")   , GetBValue(amrDefaultFont.dwColor));
	defaultFont.eAlign    = (TEXT_ALIGN)AfxGetApp()->GetProfileInt(regSection ,_T("Align") , (int)amrDefaultFont.eAlign);
	defaultFont.bBold     = AfxGetApp()->GetProfileInt(regSection ,_T("BOLD") , amrDefaultFont.bBold);
	defaultFont.bItalic   = AfxGetApp()->GetProfileInt(regSection ,_T("ITALIC") , amrDefaultFont.bItalic);
	defaultFont.bUnderline= AfxGetApp()->GetProfileInt(regSection ,_T("UNDERLINE") , amrDefaultFont.bUnderline);

	defaultFont.dwColor = RGB(RedFactor, GreenFactor , BlueFactor); 
}

void CFontSettingDlg::GetCommonDefaultFont(FontData& defaultFont, CUSTOM_RESULT_TYPE rt, EDIT_TYPE et)
{ 

#if defined(_ORG)
	m_strDefaultFontName = _LSX(굴림체);
#elif defined(_CH)
	m_strDefaultFontName = _LSX(芥竟);
#elif defined(_JP)
	m_strDefaultFontName = _LSX(굃굍 긕긘긞긏);
#elif defined(_US)
	m_strDefaultFontName = _LSX(Courier New);
#else
#error Unknown Language Definition!
	m_strDefaultFontName = _LSX(Courier New);
#endif

	switch(rt)
	{
	case CUSTOM_RESULT_GROUP:
		{
			switch(et)
			{
			case EDIT_CAPTION:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 11;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = TRUE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE;  
				}
				break;
			case EDIT_DESCRIPTION:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 11;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = TRUE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE;  
				}
				break;
			}
		}
		break;
	case CUSTOM_RESULT_MEMO:
		{
			switch(et)
			{
			case EDIT_CAPTION:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 10;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE; 
				}
				break;
			case EDIT_DESCRIPTION:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 10;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE;  
				}
				break;
			}
		}
		break;
	case CUSTOM_RESULT_USER_IMG:
	case CUSTOM_RESULT_IMAGE   :
		{
			defaultFont.strFont   = m_strDefaultFontName;
			defaultFont.nFontSize = 9;
			defaultFont.dwColor   = RGB(0,0,0);
			defaultFont.eAlign    = ALIGN_LEFT;
			defaultFont.bBold     = FALSE;
			defaultFont.bItalic   = FALSE;
			defaultFont.bUnderline= FALSE;      
		}
		break;
	case CUSTOM_RESULT_TABLE   :	
		{
			switch(et)
			{
			case EDIT_CAPTION:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 9;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE;  
				}
				break;
			case EDIT_TABLE_ROWTITLEFONT: 
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 7;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE;      
				}
				break;
			case EDIT_TABLE_COLTITLEFONT: 
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 9;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_MIDDLE;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE;        
				}
				break;
			case EDIT_TABLE_CELLTITLEFONT:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 7;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE;        
				}
				break;
			}
		}
		break;
	case CUSTOM_RESULT_GRAPH   :  
		{
			defaultFont.strFont   = m_strDefaultFontName;
			defaultFont.nFontSize = 9;
			defaultFont.dwColor   = RGB(0,0,0);
			defaultFont.eAlign    = ALIGN_LEFT;
			defaultFont.bBold     = FALSE;
			defaultFont.bItalic   = FALSE;
			defaultFont.bUnderline= FALSE;  
		}
		break;
	case CUSTOM_RESULT_USER_TXT:
		{
			switch(et)
			{
			case EDIT_CAPTION:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 10;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE; 
				}
				break;
			case EDIT_DESCRIPTION:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 9;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE;  
				}
				break;
			}
		}
		break;
	case CUSTOM_RESULT_STRING  :
		{
			switch(et)
			{
			case EDIT_CAPTION:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 9;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE; 
				}
				break;
			case EDIT_DESCRIPTION:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 9;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE;  
				}
				break;
			}
		}
		break;
	case CUSTOM_RESULT_INC_WORD:
		{
			switch(et)
			{
			case EDIT_CAPTION:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 10;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE; 
				}
				break;
			case EDIT_DESCRIPTION:
				{
					defaultFont.strFont   = m_strDefaultFontName;
					defaultFont.nFontSize = 9;
					defaultFont.dwColor   = RGB(0,0,0);
					defaultFont.eAlign    = ALIGN_LEFT;
					defaultFont.bBold     = FALSE;
					defaultFont.bItalic   = FALSE;
					defaultFont.bUnderline= FALSE;  
				}
				break;
			}
		}
		break;
	default: 
		break;
	}  
}

void CFontSettingDlg::SetDefaultFont(CUSTOM_RESULT_TYPE rt, EDIT_TYPE et ,FontData& defaultFont)
{  
	CString regSection;

	switch(rt)
	{
	case CUSTOM_RESULT_SEPERATOR:  
	case CUSTOM_RESULT_GROUP:     regSection = _T("AMR_Group");   break;
	case CUSTOM_RESULT_MEMO:      regSection = _T("AMR_Memo") ; 	break;
	case CUSTOM_RESULT_USER_IMG:
	case CUSTOM_RESULT_IMAGE:     regSection = _T("AMR_Image");   break;
	case CUSTOM_RESULT_TABLE:     regSection = _T("AMR_Table");   break;
	case CUSTOM_RESULT_GRAPH:     regSection = _T("AMR_Graph");   break;
	case CUSTOM_RESULT_USER_TXT:
	case CUSTOM_RESULT_STRING:    regSection = _T("AMR_Text") ; 	break;
	case CUSTOM_RESULT_INC_WORD:  regSection = _T("AMR_Word") ; 	break;
	default:  ASSERT(0); break;
	}

	switch(et) 
	{
	case EDIT_CAPTION:  	            regSection+= _T("_Caption");                break;
	case EDIT_DESCRIPTION:            regSection+=  _T("_Description");	          break;
	case EDIT_TABLE_ROWTITLEFONT:     regSection+= _T("_RowTitleFont");	          break;
	case EDIT_TABLE_COLTITLEFONT:     regSection+= _T("_ColTitleFont");	          break;
	case EDIT_TABLE_CELLTITLEFONT:    regSection+= _T("_CellTitleFont");	        break;
	default: ASSERT(0);break;
	}

	AfxGetApp()->WriteProfileString(regSection ,_T("FontName") , defaultFont.strFont);
	AfxGetApp()->WriteProfileInt(regSection ,_T("fontsize") , defaultFont.nFontSize);
	AfxGetApp()->WriteProfileInt(regSection ,_T("ColorR")   , GetRValue(defaultFont.dwColor));
	AfxGetApp()->WriteProfileInt(regSection ,_T("ColorG")   , GetGValue(defaultFont.dwColor));
	AfxGetApp()->WriteProfileInt(regSection ,_T("ColorB")   , GetBValue(defaultFont.dwColor));
	AfxGetApp()->WriteProfileInt(regSection ,_T("Align") , defaultFont.eAlign);
	AfxGetApp()->WriteProfileInt(regSection ,_T("BOLD") , defaultFont.bBold);
	AfxGetApp()->WriteProfileInt(regSection ,_T("ITALIC") , defaultFont.bItalic);
	AfxGetApp()->WriteProfileInt(regSection ,_T("UNDERLINE") , defaultFont.bUnderline);

}
*/
BOOL CFontSettingDlg::PreTranslateMessage(MSG* pMsg) 
{ 
	if (pMsg->wParam == VK_RETURN)
	{
		m_pParent->SendMessage(WM_MY_MESSAGE_OK, 0, 0);
		
		return TRUE;
	}
	else if (pMsg->wParam == VK_ESCAPE)
	{
		m_pParent->SendMessage(WM_MY_MESSAGE_CANCEL, 0, 0);
		
		return TRUE;
	}

	
	return CDialog::PreTranslateMessage(pMsg);
}
