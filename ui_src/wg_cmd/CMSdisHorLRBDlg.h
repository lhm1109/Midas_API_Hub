#if !defined(__WG_CMD_CMSDISHORLRBDLG_H__)
#define __WG_CMD_CMSDISHORLRBDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdDisp.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCMSdisHorLRBDlg dialog

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MillustViewer.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMSdisHorLRBDlg : public CDialogMove
{
	// Construction
public:
	CCMSdisHorLRBDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSdisHorLRBDlg();

	T_SDIS_K m_key;
	T_SDIS_D m_data;
	void     SetParamData(T_SDIS_K key, T_SDIS_D data) { m_key=key; m_data=data; }

	// Dialog Data
	//{{AFX_DATA(CCMSdisHorLRBDlg)
	enum { IDD = IDD_ETC_SDVP_ISOLATOR_HOR_LRB_DLG };

	CEditUnit	m_edtK0  ;
	CTextUnit	m_untK0  ;
	CEditUnit	m_edtK2  ;
	CTextUnit	m_untK2  ;
	CEditUnit	m_edtQd  ;
	CTextUnit	m_untQd  ;

	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSdisHorLRBDlg)
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
	//{{AFX_MSG(CCMSdisHorLRBDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
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

#endif // !defined(__WG_CMD_CMSDISHORLRBDLG_H__)
