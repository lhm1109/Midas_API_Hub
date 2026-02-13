#if !defined(__DgnCRCDgnParameter_AS5100_H__)
#define __DgnCRCDgnParameter_AS5100_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCDgnParameter_AS5100.h : header file
//

#include "..\wg_cmd\MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnCRCDgnParameter_AS5100 dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCRCDgnParameter_AS5100 : public CMyChildDialog
{
	// Construction
public:
	CDgnCRCDgnParameter_AS5100(T_PSCD_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	~CDgnCRCDgnParameter_AS5100(); 

	// Dialog Data
	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER_AS5100};

	CEditUnit m_edtAggregateSize;
	CTextUnit m_untAggregateSize;
	CEditUnit m_edtMaxStlStress;
	CTextUnit m_untMaxStlStress;
	
	BOOL      m_bTransfer;
	BOOL      m_bFlexural;
	BOOL      m_bShear;
	BOOL      m_bTorsional;
	BOOL      m_bSimplified;

	BOOL      m_bCrack; 
	//}}AFX_DATA

protected:
	void InitUnit();
	void Data2Dlg();

public:
	BOOL Dlg2Data();

	T_PSCD_D* m_pParamData;

	// Overrides
	//{{AFX_VIRTUAL(CDgnCRCDgnParameter_AS5100)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CDgnCRCDgnParameter_AS5100)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnAS5100SelAllBtn();
	afx_msg void OnDgnAS5100UnSelAllBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Attribute 
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}

#endif // !defined(__DgnCRCDgnParameter_AS5100_H__)
