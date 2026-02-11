#if !defined(__ETC_GDS_USERPAGE_H__)
#define __ETC_GDS_USERPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_UserPage.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "MyChildDialog.h"
#include "SpfcItemGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_UserPage dialog

struct T_SPFC_D;
class CETC_DESIGN_SPECT;
class CETC_GDS_UserPage : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_UserPage(T_SPFC_D *pData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CETC_GDS_UserPage)
	enum { IDD = IDD_ETC_SPFC_USER_PAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CSpfcItemGrid* m_pGrid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_UserPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	void Data2Dlg();
	BOOL Dlg2Data();

	void ChangeHeaderTitle();
	void SpectralDataChanged();
	void MakeSpectrumData(BOOL bOnlyCalc=FALSE);
	void MakePOSpectrumData(double dDamping,double SRa,double SRv,CArray <double,double>& aPeriod,CArray <double,double>& aAccel);
	
protected:
	T_SPFC_D *m_pSpfcD;

	CETC_DESIGN_SPECT *m_pParent;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_UserPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETC_GDS_USERPAGE_H__)
