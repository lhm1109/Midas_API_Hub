#if !defined(__WG_CMD_CMSdisHorSLDDlg_H__)
#define __WG_CMD_CMSdisHorSLDDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdDisp.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCMSdisHorSLDDlg dialog

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMSdisHorSLDDlg : public CDialogMove
{
	// Construction
public:
	CCMSdisHorSLDDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSdisHorSLDDlg();

	T_SDIS_K m_key;
	T_SDIS_D m_data;
	void     SetParamData(T_SDIS_K key, T_SDIS_D data) { m_key=key; m_data=data; }

	// Dialog Data
	//{{AFX_DATA(CCMSdisHorSLDDlg)
	enum { IDD = IDD_ETC_SDVP_ISOLATOR_HOR_SLD_DLG };

	CEditUnit	m_edtK0 ;
	CTextUnit	m_untK0 ;
	CEditUnit	m_edtMu0;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSdisHorSLDDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void InitCtrl();
	void Data2Dlg();
	BOOL Dlg2Data();
	void UpdateCtrls();

	// Generated message map functions
	//{{AFX_MSG(CCMSdisHorSLDDlg)
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

#endif // !defined(__WG_CMD_CMSdisHorSLDDlg_H__)
