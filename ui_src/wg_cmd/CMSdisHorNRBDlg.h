#if !defined(__WG_CMD_CMSDISHORNRBDLG_H__)
#define __WG_CMD_CMSDISHORNRBDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdDisp.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCMSdisHorNRBDlg dialog

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMSdisHorNRBDlg : public CDialogMove
{
	// Construction
public:
	CCMSdisHorNRBDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSdisHorNRBDlg();

	T_SDIS_K m_key;
	T_SDIS_D m_data;
	void     SetParamData(T_SDIS_K key, T_SDIS_D data) { m_key=key; m_data=data; }

	// Dialog Data
	//{{AFX_DATA(CCMSdisHorNRBDlg)
	enum { IDD = IDD_ETC_SDVP_ISOLATOR_HOR_NRB_DLG };

	CEditUnit	m_edtKh;
	CTextUnit	m_untKh;

	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSdisHorNRBDlg)
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
	//{{AFX_MSG(CCMSdisHorNRBDlg)
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

#endif // !defined(__WG_CMD_CMSDISHORNRBDLG_H__)
