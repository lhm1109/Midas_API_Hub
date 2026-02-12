#if !defined(__REPORTSETUPDLG_H__)
#define __REPORTSETUPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportSetupDlg.h : header file
//

#include "FontSettingDlg.h"
#include "FontSettingShadeDlg.h"
#include "ReportTableDlg.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\EditUnit.h"

#include "../MIT_frx/MTabCtrl.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MFontComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CReportSetupDlg dialog

class CReportSetupDlg : public CDialogMove
{
// Construction
public:
	CReportSetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportSetupDlg();

// Dialog Data
	//{{AFX_DATA(CReportSetupDlg)
	enum { IDD = IDD_TM_REPORT_SETUP_DLG };
	BOOL        m_bRepeatHeader;
	CEditUnit   m_editDecimal;
	BOOL        m_bExp;
	MButton     m_btnInit;
	mit::frx::MTabCtrl	  m_ctrlTab;
	BOOL        m_bUpdateTable;

	CBCGPButton	m_btnTxtAlignL;  // left
	CBCGPButton	m_btnTxtAlignC;  // center
	CBCGPButton	m_btnTxtAlignR;  // right
	CBCGPButton	m_fontTxtU;  // Underline
	CBCGPButton	m_fontTxtI;  // Italic
	CBCGPButton	m_fontTxtB;  // Bold
	CBCGPButton m_btnTxtColor;
	MComboBox   m_cmbTxtFontSize;
	MFontComboBox  m_cmbTxtFont;

	CBCGPButton	m_btnCImgAlignL;  // left
	CBCGPButton	m_btnCImgAlignC;  // center
	CBCGPButton	m_btnCImgAlignR;  // right
	CBCGPButton	m_fontCImgU;  // Underline
	CBCGPButton	m_fontCImgI;  // Italic
	CBCGPButton	m_fontCImgB;  // Bold
	CBCGPButton m_btnCImgColor;
	MComboBox   m_cmbCImgFontSize;
	MFontComboBox  m_cmbCImgFont;

	CBCGPButton	m_btnCTblAlignL;  // left
	CBCGPButton	m_btnCTblAlignC;  // center
	CBCGPButton	m_btnCTblAlignR;  // right
	CBCGPButton	m_fontCTblU;  // Underline
	CBCGPButton	m_fontCTblI;  // Italic
	CBCGPButton	m_fontCTblB;  // Bold
	CBCGPButton m_btnCTblColor;
	MComboBox   m_cmbCTblFontSize;
	MFontComboBox  m_cmbCTblFont;

	CBCGPButton	m_btnFtrAlignL;  // left
	CBCGPButton	m_btnFtrAlignC;  // center
	CBCGPButton	m_btnFtrAlignR;  // right
	CBCGPButton	m_fontFtrU;  // Underline
	CBCGPButton	m_fontFtrI;  // Italic
	CBCGPButton	m_fontFtrB;  // Bold
	CBCGPButton m_btnFtrColor;
	MComboBox   m_cmbFtrFontSize;
	MFontComboBox  m_cmbFtrFont;
	//}}AFX_DATA

public:
	FontData  m_fdTxt;
	FontData  m_fdCImg;
	FontData  m_fdCTbl;
	FontData  m_fdFtr;
	FontShadeData  m_fdTblHeader;
	FontShadeData  m_fdTblCell;

	CArray<UINT,UINT> m_TxtAlignGroup;
	CArray<UINT,UINT> m_CImgAlignGroup;
	CArray<UINT,UINT> m_CTblAlignGroup;
	CArray<UINT,UINT> m_FtrAlignGroup;

public:
	void Data2Dlg();
	void Dlg2Data();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportSetupDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedInitialize();
	afx_msg void OnSelchangeTableTab(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnTxtFontColor();
	afx_msg void OnSelchangeTxtFont();
	afx_msg void OnSelchangeTxtFontSize();
	afx_msg void OnChangeTxtAlign();
	afx_msg void OnCheckTxtCharType();

	afx_msg void OnCImgFontColor();
	afx_msg void OnSelchangeCImgFont();
	afx_msg void OnSelchangeCImgFontSize();
	afx_msg void OnChangeCImgAlign();
	afx_msg void OnCheckCImgCharType();

	afx_msg void OnCTblFontColor();
	afx_msg void OnSelchangeCTblFont();
	afx_msg void OnSelchangeCTblFontSize();
	afx_msg void OnChangeCTblAlign();
	afx_msg void OnCheckCTblCharType();

	afx_msg void OnFtrFontColor();
	afx_msg void OnSelchangeFtrFont();
	afx_msg void OnSelchangeFtrFontSize();
	afx_msg void OnChangeFtrAlign();
	afx_msg void OnCheckFtrCharType();
//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void InitControls();
	void LayoutChildDlg();
	//// #### CAMRWordView::MakeSetupStr() 도 같이 수정할것!!!!
	void ReadRegistry();
	void WriteRegistry();
	CString MakeSetupStr();
	void ExportWordToSetup();
	//// #### /////////////////////////////////////////////////

	void SetFontCombo(MFontComboBox& cmbFont, MComboBox& cmbFontSize);
	void SetImageFontButtons(CBCGPButton& btnB, CBCGPButton& btnI, CBCGPButton& btnU, CBCGPButton& btnCol, 
													 CBCGPButton& btnL, CBCGPButton& btnC, CBCGPButton& btnR);
	void ReadRegistryFontData(FontData& fdDefault, CString strSection, FontData& fdResult);
	void ReadRegistryFontShadeData(FontShadeData& fdDefault, CString strSection, FontShadeData& fdResult);
	void WriteRegistryFontData(CString strSection, FontData& fdResult);
	void WriteRegistryFontShadeData(CString strSection, FontShadeData& fdResult);
	void OnFontColor(FontData& fd, CBCGPButton& btnColor);
	void OnSelchangeFont(FontData& fd, MFontComboBox& cmbFont);
	void OnSelchangeFontSize(FontData& fd, MComboBox& cmbFontSize);
	void OnChangeAlign(FontData& fd, CArray<UINT,UINT>& AlignGroup);
	void OnCheckCharType(FontData& fd, CBCGPButton& btnB, CBCGPButton& btnI, CBCGPButton& btnU);
	void SetImageFontButtons(CBCGPButton& btn, UINT nBmpResId);

protected:
	CFontSettingShadeDlg* m_pTblH;  // Table Header
	CFontSettingShadeDlg* m_pTblC;  // Table Cell
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__REPORTSETUPDLG_H__)
