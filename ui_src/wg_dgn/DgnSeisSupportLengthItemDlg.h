#if !defined(__DGNSEISSUPPORTLENGTHITEMDLG_H__)
#define __DGNSEISSUPPORTLENGTHITEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisSupportLengthItemDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\McomboBox.h"
#include "..\mit_frx\MillustViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisSupportLengthItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisSupportLengthItemDlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CDgnSeisSupportLengthItemDlg(CWnd* pParent = NULL);   // standard constructor

	//----------------------------------------------------------------------
	// Interface
public:
	void SetParamData(T_SUPL_K Key, T_SUPL_D &Data)
	{
		m_Key = Key; m_Data = Data; m_bModify = TRUE;
	}

public:
	// Dialog Data
	//{{AFX_DATA(CDgnSeisSupportLengthItemDlg)
	enum { IDD = IDD_DGN_SEIS_SUPPORT_LENGTH_ITEM_DLG };

	mit::frx::MEdit      m_edtName;
	mit::frx::MComboBox  m_cmbAnev;
	CEditUnit m_edtH;
	CTextUnit m_untH;
	CEditUnit m_edtTheta;
	CTextUnit m_untTheta;
	CEditUnit m_edtNc;
	CTextUnit m_untNc;
	CEditUnit m_edtL;
	CTextUnit m_untL;
	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisSupportLengthItemDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void InitUnit();
	void InitCombo();

	void Data2Dlg();
	BOOL Dlg2Data();

	void UpdateName();

	void CalcH();
	void CalcTheta();

	CDBDoc* m_pDoc;

	T_SUPL_K m_Key;
	T_SUPL_D m_Data;

	BOOL m_bModify;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisSupportLengthItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSeisSupportLengthAnevCmb();
	afx_msg void OnSeisSupportLengthOKBtn();
	afx_msg void OnSeisSupportLengthCancelBtn();
	afx_msg void OnSeisSupportLengthApplyBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISSUPPORTLENGTHITEMDLG_H__)

