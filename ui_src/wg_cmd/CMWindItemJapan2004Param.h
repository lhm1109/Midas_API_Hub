#if !defined(__CMWINDITEMJAPAN2004PARAM_H__)
#define __CMWINDITEMJAPAN2004PARAM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemJapan2004Param.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004Param dialog
class CCMWindItemJapan2004Param : public CDialogMove
{
// Construction
public:
	CCMWindItemJapan2004Param(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMWindItemJapan2004Param)
	enum { IDD = IDD_ETC_WIND_ITEM_JAPAN2004_PARAM };
	CEditUnit	m_editModeZ;
	CEditUnit	m_editModeY;
	CEditUnit	m_editModeX;
	CBCGPStatic  	m_unitFreqZ;
	CEditUnit	m_editFreqZ;
	CBCGPStatic	  m_unitFreqY;
	CEditUnit	m_editFreqY;
	CBCGPStatic	  m_unitFreqX;
	CEditUnit	m_editFreqX;
	CEditUnit	m_editDampZ;
	CEditUnit	m_editDampY;
	CEditUnit	m_editDampX;
	CTextUnit	m_unitBreadthY;
	CEditUnit	m_editBreadthY;
	CTextUnit	m_unitBreadthX;
	CEditUnit	m_editBreadthX;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemJapan2004Param)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitControls();
	void Data2Dlg();
	BOOL Dlg2Data();

public:
	double m_dBreadthX;
	double m_dBreadthY;
	double m_dDampRatioX;
	double m_dDampRatioY;
	double m_dDampRatioR;
	double m_dFrequencyX;
	double m_dFrequencyY;
	double m_dFrequencyR;
	double m_dModeFactorX;
	double m_dModeFactorY;
	double m_dModeFactorR;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMWindItemJapan2004Param)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMJAPAN2004PARAM_H__)
