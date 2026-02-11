// Thgc_GlobalCtrl_PointSpringSupportDlg.h: interface for the CThgc_GlobalCtrl_PointSpringSupportDlg class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_NTGC_GLOBALCTRL_POINT_SPRING_SUPPORT_DLG_H__08B08815_EA12_404E_BA5C_4660F7052BED__INCLUDED_)
#define AFX_NTGC_GLOBALCTRL_POINT_SPRING_SUPPORT_DLG_H__08B08815_EA12_404E_BA5C_4660F7052BED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../wg_base/DlgChild.h"  

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CThgc_GlobalCtrl_PointSpringSupportDlg : public CDlgChild
{
	// Construction
public:
	CThgc_GlobalCtrl_PointSpringSupportDlg(CWnd* pParent = NULL);   // standard constructor
	~CThgc_GlobalCtrl_PointSpringSupportDlg();

	// Dialog Data
	//{{AFX_DATA(CThgc_GlobalCtrl_PointSpringSupportDlg)
	enum { IDD = IDD_ETC_THGC_GLOBAL_POINT_SPRING_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThgc_GlobalCtrl_PointSpringSupportDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CThgc_GlobalCtrl_PointSpringSupportDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Implementation


public:
	void SetData(int  nPointSpringSupport, int  nElasticLink);
	void GetData(int &nPointSpringSupport, int &nElasticLink);
	CRect m_rectPosition;

protected:
	void MoveDlg();
	void Data2Dlg();
	void Dlg2Data();
	int m_nPointSpringSupport;
	int m_nElasticLink;

	CArray<UINT, UINT> m_aCtrlRdo;   // Point Spring Support
	CArray<UINT, UINT> m_aCtrlRdo2;  // Elastic Link
};

#include "HeaderPost.h"

#endif // !defined(AFX_NTGC_GLOBALCTRL_POINT_SPRING_SUPPORT_DLG_H__08B08815_EA12_404E_BA5C_4660F7052BED__INCLUDED_)
