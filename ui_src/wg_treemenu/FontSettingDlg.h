#if !defined(AFX_FONTSETTINGDLG_H__A71AABB7_8EF6_443A_A21C_8A413A1287BC__INCLUDED_)
#define AFX_FONTSETTINGDLG_H__A71AABB7_8EF6_443A_A21C_8A413A1287BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontSettingDlg dialog

#include "ReportDefine.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
// "ReportDefine.h"으로 이동함...
//#include "FontEdit.h"

// #define WM_MY_MESSAGE_OK           WM_USER+100
// #define WM_MY_MESSAGE_CANCEL       WM_USER+101
// #define WM_MY_EDIT_CANCEL          WM_USER+102
// #define WM_MY_CHANGE_SLAVETITLE    WM_USER+103

// enum TEXT_ALIGN 
// {  
//   ALIGN_LEFT=0, 
//   ALIGN_CENTER, 
//   ALIGN_RIGHT 
// };
// enum EDIT_TYPE  
// {  
//   EDIT_CAPTION=0, 
//   EDIT_DESCRIPTION, 
//   EDIT_TABLE_ROWTITLEFONT, 
//   EDIT_TABLE_COLTITLEFONT, 
//   EDIT_TABLE_CELLTITLEFONT
// };
// enum CAPTION_POSITION
// {  
//   POSITION_TOP=0, 
//   POSITION_BOT, 
//   POSITION_NONE
// };
// enum TABLE_TYPE
// {  
//   TABLE_HEADER=0, 
//   TABLE_CELL
// };
// 
// struct FontData
// {
//   CFontEdit *pTargetWnd;
//   CString     strFont;
//   UINT        nFontSize;
//   COLORREF    dwColor;  //Table에서 Row, Col, Cell에서도 사용함
//   TEXT_ALIGN  eAlign;
//   BOOL        bBold;
//   BOOL        bItalic;
//   BOOL        bUnderline;
// 
//   FontData()
//   {
//     Initialize();
//   }
//   void FontData::Initialize()
//   {
//     pTargetWnd  =NULL;
//     strFont     ="";
//     nFontSize   =0;
//     dwColor     =0;
//     eAlign      =ALIGN_LEFT;
//     bBold       =FALSE;
//     bItalic     =FALSE;
//     bUnderline  =FALSE;
//   }
//   void FontData::SetDefault()
//   {
// #if defined(_ORG)
//     SetDefaultORG();
// #elif defined(_CH)
//     SetDefaultCH();
// #elif defined(_JP)
//     SetDefaultJP();
// #elif defined(_US)
//     SetDefaultUS();
// #else
// #error Unknown Language Definition!
//     SetDefaultUS();
// #endif
//   }
//   void FontData::SetDefaultORG()
//   {
//     pTargetWnd  =NULL;
//     strFont     ="굴림체";
//     nFontSize   =10;
//     dwColor     =RGB(0,0,0);
//     eAlign      =ALIGN_LEFT;
//     bBold       =FALSE;
//     bItalic     =FALSE;
//     bUnderline  =FALSE;
//   }
//   void FontData::SetDefaultCH()
//   {
//     pTargetWnd  =NULL;
//     strFont     ="芥竟";
//     nFontSize   =10;
//     dwColor     =RGB(0,0,0);
//     eAlign      =ALIGN_LEFT;
//     bBold       =FALSE;
//     bItalic     =FALSE;
//     bUnderline  =FALSE;
//   }
//   void FontData::SetDefaultJP()
//   {
//     pTargetWnd  =NULL;
//     strFont     ="굃굍 긕긘긞긏";
//     nFontSize   =10;
//     dwColor     =RGB(0,0,0);
//     eAlign      =ALIGN_LEFT;
//     bBold       =FALSE;
//     bItalic     =FALSE;
//     bUnderline  =FALSE;
//   }
//   void FontData::SetDefaultUS()
//   {
//     pTargetWnd  =NULL;
//     strFont     ="Courier New";
//     nFontSize   =10;
//     dwColor     =RGB(0,0,0);
//     eAlign      =ALIGN_LEFT;
//     bBold       =FALSE;
//     bItalic     =FALSE;
//     bUnderline  =FALSE;
//   }
// };

class CFontSettingDlg : public CDialog //CBCGPDialog
{
// Construction
public:
	CFontSettingDlg(FontData &fd, CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFontSettingDlg)
	enum { IDD = IDD_TM_REPORT_FONT_SETTING };
	CBCGPButton	m_btnAlignLeft;
	CBCGPButton	m_btnAlignCent;
	CBCGPButton	m_btnAlignRight;
	CBCGPButton	m_fontUnderline;
	CBCGPButton	m_fontItalic;
	CBCGPButton	m_fontBold;
	CBCGPButton m_btnColor;
	MComboBox   m_cmbFontSize;
	CBCGPFontComboBox  m_cmbFont;
	//}}AFX_DATA

	CArray<UINT,UINT> m_AlignGroup;
	FontData *m_pFontData;
	CWnd* m_pParent;

	//void SetTargetWnd(CWnd *pTarget);
	void Data2Dlg();
	void Dlg2Data();
	void SetFontData(FontData &fd); //AMR_Create_Data 다이얼로그에서 사용
	void DisableFontSize(int nFontSize=10); //Default는 10으로 강제 초기화 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontSettingDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	
protected:

	void InitWindowPos();
	void InitFontCombo();
	void InitControls();

public:
	static int  m_nDefaultFontIndex;
	static CString m_strDefaultFontName;
//   static void GetCommonDefaultFont(FontData& defaultFont, CUSTOM_RESULT_TYPE rt, EDIT_TYPE et);
//   static void GetDefaultFont(CUSTOM_RESULT_TYPE rt, EDIT_TYPE et ,FontData& defaultFont);
//   static void SetDefaultFont(CUSTOM_RESULT_TYPE rt, EDIT_TYPE et ,FontData& defaultFont);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFontSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnCaptionFontColor();
	afx_msg void OnSelchangeCmbCaptionFont();
	afx_msg void OnSelchangeCmbCaptionFontSize();
	afx_msg void OnChangeAlign();
	afx_msg void OnCheckCharType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTSETTINGDLG_H__A71AABB7_8EF6_443A_A21C_8A413A1287BC__INCLUDED_)
