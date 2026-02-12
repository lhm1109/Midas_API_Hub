#if !defined(__WG_CMD_CMSdisVerDlg_H__)
#define __WG_CMD_CMSdisVerDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdDisp.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCMSdisVerDlg dialog

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MillustViewer.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMSdisVerDlg : public CDialogMove
{
	// Construction
public:
	CCMSdisVerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSdisVerDlg();

	int m_nDevType; // 0:LRB, 1:NRB
	T_SDIS_K m_key;
	T_SDIS_D m_data;
	void     SetParamData(T_SDIS_K key, T_SDIS_D data, int nDevType) { m_key=key; m_data=data; m_nDevType=nDevType; }

	// Dialog Data
	//{{AFX_DATA(CCMSdisVerDlg)
	enum { IDD = IDD_ETC_SDVP_ISOLATOR_VER_DLG };

	BOOL      m_bConsVDN; // Consider Vertical Direction Nonlinearity
	CEditUnit	m_edtKv   ;
	CTextUnit	m_untKv   ;
	CEditUnit	m_edtBeta ;
	CEditUnit	m_edtAlpha;
	CEditUnit	m_edtSigv ;
	CTextUnit	m_untSigv ;

	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSdisVerDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void InitCtrl();
	void Data2Dlg();
	BOOL Dlg2Data();
	void SetBmpImage();
	void UpdateCtrls();

	mit::frx::MillustViewer m_ImgViewer;

	// Generated message map functions
	//{{AFX_MSG(CCMSdisVerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnConsVDNChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL m_bDBData; // DB에서 읽어온 데이터인가?
public:
	void SetDBData(BOOL bDBData) { m_bDBData = bDBData; }
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WG_CMD_CMSdisVerDlg_H__)
