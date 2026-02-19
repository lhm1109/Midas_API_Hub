#if !defined(__DGNCFCODEDLG_H__)
#define __DGNCFCODEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCfCodeDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnCfCodeDlg dialog
class __MY_EXT_CLASS__ CDgnCfCodeDlg : public CDialogMove
{
	// Construction
public:
	CDgnCfCodeDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDgnCfCodeDlg();

	// Dialog Data
	//{{AFX_DATA(CDgnCfCodeDlg)
	enum { IDD = IDD_DGN_CF_CODE_DLG };
	CComboBox	m_DesignCode;
	CComboBox	m_DesignSubCode;
	CComboBox	m_WindCase;
	BOOL m_bIteration;
	BOOL m_bNotUseBLu;
	//Add by tss(GB50018-02)
	BOOL   m_bSeismic;
	double m_dStructImportCoef;	
	double m_dNetGrossAreaRatio;
	int		m_nBeamChkProc; //0:PureBend 1:Pure&CompBend
	BOOL	m_bPurlinBend;
	BOOL	m_bSlabCons;
	BOOL	m_bLowerEdge;
	BOOL	m_bUpEdge;
	int		m_nWindCase;
	double	m_dBendModuluY;
	double	m_dBendModuluZ;

public:
	void Initial_ExistData();
	void Initial_Data();
	void SelchangeDgnCtrldataDesigncode(BOOL bUpdateDef);
	void InitWindCaseCombx();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCfCodeDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT,UINT> m_aNationalAnnexCtrl;
	CArray<UINT,UINT> m_aEuroCodeCtrl;
	CArray<UINT,UINT> m_aEtcCtrl;
	CArray<UINT,UINT> m_aGBCodeCtrl;

	CArray<BOOL, BOOL> m_aCodeEnable;
	int m_nOldCodeIndex;

	// Generated message map functions
	//{{AFX_MSG(CDgnCfCodeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDgnStlctrlSpecial();
	afx_msg void OnSelchangeDgnCtrldataDesigncode();
	afx_msg void OnDgnStlChkLowerEdge();
	afx_msg void OnSelchangeDgnWindCase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif 