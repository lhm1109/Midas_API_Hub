#if !defined(__DGNSEISBEARINGPROPITEMDLG_H__)
#define __DGNSEISBEARINGPROPITEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisBearingPropItemDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "DgnSeisBearingPropEditBtn.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisBearingPropItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisBearingPropItemDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisBearingPropItemDlg(CWnd* pParent = NULL);   // standard constructor
	
	//----------------------------------------------------------------------
	// Interface
public:
	void SetParamData(T_BRPR_K Key, T_BRPR_D &Data) 
	{ m_Key = Key; m_Data = Data; m_bModify = TRUE; }	
	void MakeEmfAndClose(CString strPath);
public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisBearingPropItemDlg)
	enum { IDD = IDD_DGN_SEIS_BEARING_PROP_ITEM_DLG };

	mit::frx::MEdit     m_edtName;
	mit::frx::MComboBox m_cmbInstall;
	mit::frx::MComboBox m_cmbAnchor;
	CEditUnit m_edthef;
	CTextUnit m_unthef;
	CEditUnit m_edtfy;
	CTextUnit m_untfy;
	CEditUnit m_edtfuta;
	CTextUnit m_untfuta;
	CEditUnit m_edtdh;
	CTextUnit m_untdh;
	CEditUnit m_edteh;
	CTextUnit m_unteh;
	CEditUnit m_edtNp;
	CTextUnit m_untNp;
	CEditUnit m_edtBa;
	CTextUnit m_untBa;
	CEditUnit m_edtHa;
	CTextUnit m_untHa;
	CEditUnit m_edtSx;
	CTextUnit m_untSx;
	CEditUnit m_edtN1;
	CEditUnit m_edtS1;
	CTextUnit m_untS1;
	CEditUnit m_edtSy;
	CTextUnit m_untSy;
	CEditUnit m_edtN2;
	CEditUnit m_edtS2;
	CTextUnit m_untS2;
    //
	int       m_nBearing;
	mit::frx::MComboBox m_cmbStlBearing;
	CEditUnit m_edtda;
	CTextUnit m_untda;
	CEditUnit m_edtds;
	CTextUnit m_untds;
	CEditUnit m_edtCapaL;
	CTextUnit m_untCapaL;
	CEditUnit m_edtCapaT;
	CTextUnit m_untCapaT;
	CEditUnit m_edtDispL;
	CTextUnit m_untDispL;
	CEditUnit m_edtDispT;
	CTextUnit m_untDispT;
	//

	BOOL	  m_bContiWeld;
	int       m_nViewType;	
	CDgnSeisBearingPropEditBtn m_View;

	mit::frx::MillustViewer m_wndPicture;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisBearingPropItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitUnit();
	void InitCombo(int nInstall, int nAnchor);
	void InitView();

	void AlignControl();
	void ControlsShowHide();
	void EnableDisableControls();

	void Data2Dlg();
	BOOL Dlg2Data();
	void SetViewData();

	void CalcBa();
	void CalcHa();

	BOOL CheckBrpr();
	BOOL CheckBrpr4Anev(T_ANEV_D& AnevD);

	CDBDoc* m_pDoc;
	
	T_BRPR_K m_Key;
	T_BRPR_D m_Data;

	BOOL m_bModify;
	BOOL m_bSetEmf;
	CString m_strPath;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisBearingPropItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSeisBearingPropInstallCmb();
	afx_msg void OnSeisBearingPropAnchorCmb();
	afx_msg void OnSeisBearingPropDaEdt();
	afx_msg void OnSeisBearingPropSxEdt();
	afx_msg void OnSeisBearingPropN1Edt();
	afx_msg void OnSeisBearingPropS1Edt();
	afx_msg void OnSeisBearingPropSyEdt();
	afx_msg void OnSeisBearingPropN2Edt();
	afx_msg void OnSeisBearingPropS2Edt();
	afx_msg void OnSeisBearingPropDrawRdo();
	afx_msg void OnSeisBearingPropGuideRdo();
	//afx_msg void OnSeisBearingPropDaRdo();
	afx_msg void OnSeisBearingPropOKBtn();
	afx_msg void OnSeisBearingPropCancelBtn();
	afx_msg void OnSeisBearingPropApplyBtn();
	afx_msg void OnSeisBearingEvalBearingRdo();
	afx_msg void OnSeisBearingEvalStlBearingCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISBEARINGPROPITEMDLG_H__)
