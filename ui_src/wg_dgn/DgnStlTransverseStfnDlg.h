#if !defined(__DGN_STLTRANSVERSESTFNDLG_H__)
#define __DGN_STLTRANSVERSESTFNDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DBDoc.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_cmd\wg_cmd_CMSectViewWnd.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnStlTransverseStfnDlg dialog

#include "DgnSectionDrawWnd.h"

#include "HeaderPre.h"

using namespace mit::frx;
class __MY_EXT_CLASS__ CDgnStlTransverseStfnDlg : public CDialogMove
{
// Construction
public:
	CDgnStlTransverseStfnDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() {}
 
// Dialog Data
	//{{AFX_DATA(CDgnStlTransverseStfnDlg)
	enum { IDD = IDD_DGN_STL_TRAN_STFN_DLG };
	MButton	    m_btnDel;
	MButton	    m_btnAdd;
	CListCtrl	m_wndList;

	MButton	m_chkSameTvsf;

	CEditUnit	m_ht_i;
	CTextUnit	m_ht_i_Unit;
	CEditUnit	m_t_i;
	CTextUnit	m_t_i_Unit;
	CEditUnit	m_Pitch_i;
	CTextUnit	m_Pitch_i_Unit;
	 //
	CEditUnit	m_ht_m;
	CTextUnit	m_ht_m_Unit;
	CEditUnit	m_t_m;
	CTextUnit	m_t_m_Unit;
	CEditUnit	m_Pitch_m;
	CTextUnit	m_Pitch_m_Unit;
	 //
	CEditUnit	m_ht_j;
	CTextUnit	m_ht_j_Unit;
	CEditUnit	m_t_j;
	CTextUnit	m_t_j_Unit;
	CEditUnit	m_Pitch_j;
	CTextUnit	m_Pitch_j_Unit;	

	MEdit	m_editTip;

	//}}AFX_DATA

public:
		

protected:
	CDBDoc* m_pDoc;
	T_TVSF_K m_TvsfKey;
	T_TVSF_D m_Data;
	T_SECT_D m_SectD;
	CMap<int,int,int,int> m_arBeamData;   // Element ¡ﬂ Steel Regular sect Data ¿˙¿Â 
	BOOL m_bModify;

	void Data2Dlg();
	BOOL Dlg2Data();  

	void SetInitUnit();
	void ChangeDlgItemInfo();
	BOOL SelectSection(CArray<T_TVSF_K, T_TVSF_K>&  arKey);
	BOOL IsDiffSections(T_SECT_D& OrginSectData, T_SECT_D& SectData);

	void ChangeItem(int nIndex);
	void MakeItemEx();
	void SetHeaderTitle();
	void InitTabCtrl();
	int  FindInsertionPos(T_SECT_K nID);
	BOOL InsertItem(T_SECT_K Key, CString strSName);
	CString	DataToStr(int i, T_SECT_K Key, CString strSName, T_TVSF_D &rData);


protected:
	//Tab Control.
	int m_nIMJ;     // 0:I  1:M  2:J
	
	CMap<int,int,T_TVSF_K,T_TVSF_K> m_arTvsfKey;

	CDgnSectionDrawWnd     m_SectWnd;  // Veiw Window
	CCMSecViewWnd m_wndSecView; // variable for section preview

	//void SetRchkData(int nIMJ, T_RCHK_K RchkKey, T_RCHK_BEAM* pBeamData);
	//void UpdateViewWnd();

	CString GetShapeOfSection(T_SECT_D& SectD);
	void ShowSizeTip(T_SECT_D& rData);
	void SetShowTip(CString& csTip);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlTransverseStfnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDgnStlTransverseStfnDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDgnDesignSection();
	afx_msg void OnDgnDelete();
	afx_msg void OnSameStiffener();
	afx_msg void OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGN_STLTRANSVERSESTFNDLG_H__)
