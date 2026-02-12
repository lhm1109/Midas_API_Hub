#if !defined(AFX_CMDTENDONAREADLG_H__8D52BAAE_F6B4_4741_8BF2_06CC99341743__INCLUDED_)
#define AFX_CMDTENDONAREADLG_H__8D52BAAE_F6B4_4741_8BF2_06CC99341743__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDTendonAreaDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\mit_frx\MComboBox.h"

using namespace mit::frx;

struct T_TENDON_AREA
{
	CString csDiaName;
	double  dArea;
	double  dW_Factor;
	double  dC_Factor;
	double  dR_Factor;
	double  dY_Strength;
	double  dU_Strength;
	double  dAnchorageSlip;

	void Initialize()
	{
		csDiaName.Empty();
		dArea       = 0.0;
		dW_Factor   = 0.0;
		dC_Factor   = 0.0;
		dR_Factor   = 0.0;
		dY_Strength = 0.0;
		dU_Strength = 0.0;
		dAnchorageSlip = 0.0;
	}

	T_TENDON_AREA()
	{

	}

	T_TENDON_AREA(const CString& DiaName, double Area)
	{
		Initialize();
		csDiaName = DiaName;
		dArea     = Area;
	}	
};

typedef CArray<T_TENDON_AREA, T_TENDON_AREA&> T_TENDON_AREA_LIST;

/////////////////////////////////////////////////////////////////////////////
// CCMDTendonAreaDlg dialog

class CDBDoc;
class CCMDTendonAreaDlg : public CChildDialogMove 
{
// Construction
public:
	CCMDTendonAreaDlg(CWnd* pParent = NULL);   // standard constructor
	void MakeTendonAreaDB(T_MATL_K MatlK);  
	BOOL GetTendonAreaData(T_TENDON_AREA &data);
	BOOL MakeTendonAreaDB_CH_GB(T_MATL_D& MatlD);
	BOOL MakeTendonAreaDB_CH_TB10092_17(T_MATL_D& MatlD);
	BOOL MakeTendonAreaDB_CH_JTG3362(T_MATL_D& MatlD);
	BOOL MakeTendonAreaDB_CH_CJJ11_2019(T_MATL_D& MatlD);
	BOOL MakeTendonAreaDB_CH_GB19(T_MATL_D& MatlD);
	BOOL MakeTendonAreaDB_CH_QCR9300_18(T_MATL_D& MatlD);
//   void MakeTendonAreaDB_CH_GB(T_MATL_K MatlK,
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aStd1470, 
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aStd1570,
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aStd1720,
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aStd1860,
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aStd1960,
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aWir1470, 
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aWir1570,
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aWir1770,
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aWir1860,
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aStb540, 
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aStb785,
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aStb930,
//     CArray<T_TENDON_AREA, T_TENDON_AREA&>& aStb1080);


// Dialog Data
	//{{AFX_DATA(CCMDTendonAreaDlg)
	enum { IDD = IDD_CMD_DEF_TENDON_TYPE_TAREA_DLG };
	MComboBox	m_wndTendonAreaCombo;
	CFormulaEditSpin	m_strTAreaNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDTendonAreaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;  
	//T_MATL_K  m_MatlKey;
	T_TENDON_AREA m_TargetData;
	CArray<T_TENDON_AREA, T_TENDON_AREA&> m_aAreaDB;
	
	void SetTendonAreaCombo();
	BOOL GetTargetData();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMDTendonAreaDlg)
	afx_msg void OnDeltaposCmdTendonAreaSpin(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDTENDONAREADLG_H__8D52BAAE_F6B4_4741_8BF2_06CC99341743__INCLUDED_)
