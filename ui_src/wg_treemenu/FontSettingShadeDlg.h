#if !defined(AFX_FONTSETTINGSHADEDLG_H)
#define AFX_FONTSETTINGSHADEDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontSettingShadeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontSettingShadeDlg dialog

#include "ReportDefine.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MFontComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CFontSettingShadeDlg : public CChildDialog//CDialog //CBCGPDialog
{
// Construction
public:
	CFontSettingShadeDlg(FontShadeData* fd, CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFontSettingShadeDlg)
	enum { IDD = IDD_TM_REPORT_FONT_SETTING_SHADE };
	CBCGPButton	m_btnAlignL;  // left
	CBCGPButton	m_btnAlignC;  // center
	CBCGPButton	m_btnAlignR;  // right
	CBCGPButton	m_fontU;  // Underline
	CBCGPButton	m_fontI;  // Italic
	CBCGPButton	m_fontB;  // Bold
	CBCGPButton m_btnColor;
	MComboBox   m_cmbFontSize;
	MFontComboBox  m_cmbFont;
	CBCGPButton m_btnColorShade;
	//}}AFX_DATA

	CArray<UINT,UINT> m_AlignGroup;
	FontShadeData *m_pFontData;

	void Data2Dlg();
	void Dlg2Data();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontSettingShadeDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	
protected:
	void InitControls();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFontSettingShadeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnCaptionFontColor();
	afx_msg void OnSelchangeCmbCaptionFont();
	afx_msg void OnSelchangeCmbCaptionFontSize();
	afx_msg void OnChangeAlign();
	afx_msg void OnCheckCharType();
	afx_msg void OnTableShading();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTSETTINGSHADEDLG_H)
