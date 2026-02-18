#if !defined(__DgnConBentUpBarForBeamSectDlg_H__)
#define __DgnConBentUpBarForBeamSectDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBentUpBarForBeamSectDlg.h : header file
//
#include "DgnDlgBase.h"

#include "../wg_db/wg_db_TextUnit.h"
#include "../wg_db/wg_db_EditUnit.h"
#include "../wg_db/SelectCtrl.h"
#include "../MIT_frx/MillustViewer.h"
#include "../MIT_frx/MEdit.h"

#include "DgnConBentUpInfoGrid.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CDgnConBentUpBarForBeamSectDlg dialog

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnConBentUpBarForBeamSectDlg : public CDgnDlgBase
{
// Construction
public:
	CDgnConBentUpBarForBeamSectDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnConBentUpBarForBeamSectDlg();

// Dialog Data
	//{{AFX_DATA(CDgnConBentUpBarForBeamSectDlg)
	enum { IDD = IDD_DGN_CON_BENT_UP_BAR };	
	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBentUpBarForBeamSectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnConBentUpBarForBeamSectDlg)
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();	
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	afx_msg void OnOk();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnMod();
	afx_msg void OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult);
		
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CListCtrl m_BurbList;
	CSelectEdit m_edtEList;
	MEdit		  m_edtName;
	CArray<UINT, UINT> m_aXAxisDirRdo;
protected:

	CDBDoc* m_pDoc;
	
	CDgnConBentUpInfoGrid* m_pGridBentUpInfo;
	
	T_BURB_D m_Data;

	CArray<T_BURB_K, T_BURB_K> m_aBurbKInList;

	MillustViewer m_wndPicture;

	//T_RAKR_D m_Data;

	void SetControl();	
	void Data2Dlg();
	void Data2DlgGrid();
	void Dlg2Data();
	void Dlg2DataGrid();
	void UpdateBuffer();
	void RefreshListByDB();
	BOOL EndEdit(BOOL bEnd);
	void SelElemByEdtEList();	
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnConBentUpBarForBeamSectDlg_H__)
