#if !defined(AFX_CMGRIDCROSSREFERENCEDLG_H__2546F999_5629_4ABC_92EA_4DA8352D8723__INCLUDED_)
#define AFX_CMGRIDCROSSREFERENCEDLG_H__2546F999_5629_4ABC_92EA_4DA8352D8723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_base\wg_base_MouseEdit.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"


#include "CMChildBarBase.h"
#include "CMGXGridWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CCMGXMainGirderGridWnd grid
using namespace mit::frx;
class CCMGXCrossRefGridWnd : public CCMGXGridWnd2
{
public:
	CCMGXCrossRefGridWnd() {}
	virtual ~CCMGXCrossRefGridWnd() {}

protected:

public:
	BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
};

/////////////////////////////////////////////////////////////////////////////
// CCMGridCrossReferenceDlg dialog

class CCMGridCrossReferenceDlg : public CCMChildBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCMGridCrossReferenceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute(){}
	virtual BOOL ExternalInit(UINT key);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Dialog Data
	//{{AFX_DATA(CCMGridCrossReferenceDlg)
	enum { IDD = IDD_CMD_GRID_CROSS_REFERENCE_DLG };
	MComboBox	m_cmbCalcPosition2;
	MComboBox	m_cmbCalcPosition1;
	CListCtrl	m_ctrlCRList;
	MButton m_ctrlIncImpactFactor;
	CMouseEdit m_wndMasterNode;
	CMouseEdit m_wndMasterElem;
	MComboBox  m_cmbMasterLink;
	MEdit	m_ctrlName;
	//}}AFX_DATA
	CSelectEdit	m_editSelectNode;
	CSelectEdit	m_editSelectElem;
	CSelectEdit	m_editSelectLink;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridCrossReferenceDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AlignCtrl();
	BOOL Dlg2Data(T_CREF_D& data, int nSeq=-1);
	void Data2Dlg(T_CREF_D& data);
	void SetListCtrlHeader();
	void MakeCombo();
	BOOL GetSelectedData(int &nIndex);
	void MakeItemEx();
	void SetItem(int nIndex, T_CREF_K key, T_CREF_D &data);
	CString DataToStr(int i, T_CREF_K key, T_CREF_D &data);
	BOOL ValidData(T_CREF_D& data);
	void SetDataSequence();
	void GetSubListData(CArray<T_CREF_BASE, T_CREF_BASE&>& aElemBase);
	void SetSubListData(CArray<T_CREF_BASE, T_CREF_BASE&>& aElemBase);
	CString GetCalcPositionString(int nPosition);
	int GetCalcPosition(CString str);
	void OnCmdOpDelete();

	void SetGridTitle();
	void InitGrid();
	void Grid_InsertItem(int nPos, T_CREF_BASE& data);
	int Grid_ExistKey(int nKey);

	CCMGXCrossRefGridWnd m_wndGrid;
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_arCalcItem;
	CArray<UINT, UINT> m_arCalcType;
	
	CArray<UINT, UINT> m_arCalcMasterPos;
	CArray<UINT, UINT> m_arCalcList;
	CArray<UINT, UINT> m_arCalcTypeElem;
	CArray<UINT, UINT> m_arCalcTypeLink;
	CArray<UINT, UINT> m_arCalcTypeNode;

	// Generated message map functions
	//{{AFX_MSG(CCMGridCrossReferenceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnTmClose();
	afx_msg void OnCmdGridCalcAddBtn();
	afx_msg void OnCmdGridOperAddBtn();
	afx_msg void OnCmdGridOperDelBtn();
	afx_msg void OnCmdGridOperModBtn();
	afx_msg void OnCmdCalcCtrl();
	afx_msg void OnCmdCalcItemCtrl();
	afx_msg void OnCmdCalcTypeCtrl();
	afx_msg void OnItemchangedCrefList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMGRIDCROSSREFERENCEDLG_H__2546F999_5629_4ABC_92EA_4DA8352D8723__INCLUDED_)
