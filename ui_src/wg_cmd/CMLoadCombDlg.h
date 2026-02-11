#if !defined(__CMLOADCOMBDLG_H__)
#define __CMLOADCOMBDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CMDlgBase.h"
#include "../MIT_frx/MTabCtrl.h"
#include "../MIT_frx/MComboBox.h"
#include "../MIT_frx/MButton.h"
#include "../MIT_frx/MEdit.h"

namespace LOADCOMB_EXCEL
{
	class MString;
}

using namespace mit::frx;

#include "CMLoadCombMainGrid.h"
#include "CMLoadCombSubGrid.h"
#include "CmLoadCombGrid.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMLoadCombDlg : public CCMDlgBase
{
	//General : 일반, Seismic : KISTEC 내진성능평가, Linear : MOE 내진성능평가
// Construction
public:
	enum { IDD = IDD_CMD_LOADCOMB_DLG };

	CCMLoadCombDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMLoadCombDlg();

	void UpdateStld();
	CGXGridWnd* MakeVirtualTB(UINT UtblK);

	// Overrides
	// ClassWizard generated virtual function overrides
	BOOL DestroyWindow() override;

	constexpr BOOL IsSpreadSheetStyle() const noexcept { return m_bSpreadSheetStyle; }
	constexpr void SetSeismicEvaluation(const LCOM_TYPE nType) noexcept { m_nSeismicType = nType; }
	constexpr void SetLcomType(const int nLcomType) noexcept { m_nLcomType = nLcomType; }

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint) override;
	void UpdateBuffer();
	void OnStageChanged();

	void InitLcomTypeCombo();
	CString GetDefaultFileName() const;

	bool ExportLoadCombListFromExcel(const CString& strFile) const;
	bool ExportLoadCombListFromLCP(const CString& strFile) const;
	bool ImportLoadCombListFromExcel(const CString& strFile) const;
	bool ImportLoadCombListFromLCB(const CString& strFile) const;

	CString GetCombActiveStr(int nLcomType, int nActive) const;
	std::vector<CString> GetCombActiveStringLists(int nLcomType) const;
	CString GetCombTypeStr(int nCombType) const;

	void SetTabWndText();
	void AlignControl();
	void GetTabTitleAndType(CArray<CString, CString&>& aTabTitle, CArray<int, int>& aTabLcomType);

	bool ConvertLcomTypeToString(const int nLcomType, CString& strDesignType) const;
	bool ConvertToDesignType(const LOADCOMB_EXCEL::MString& strLcomType, int& nLcomType) const;
	int ConvertToLoadCombActive(const int nLcomType, const  LOADCOMB_EXCEL::MString& type) const;
	int ConvertToLoadCombType(const LOADCOMB_EXCEL::MString& type) const;

private:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnExitSizeMove();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDlgClose();
	afx_msg void OnPaint();
	afx_msg void OnCmdBtnExport();
	afx_msg void OnCmdBtnAutogen();
	afx_msg void OnCmdBtnImport();
	afx_msg void OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdBtnChangeStyle();
	afx_msg void OnCmdBtnCopy();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnCmdBtnCopyIntoDgn();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	int  m_bSpreadSheetStyle;
	CArray<UINT, UINT> m_aCtrlNormalStyle;
	CArray<UINT, UINT> m_aCtrlSpreadStyle;
	CArray<UINT, UINT> arLcomType;

	CArray<CString, CString&> m_aTabTitle; // CTabCtrl 의 Title 설정이 char* 를 받기 때문에 이름을 Member로 들고있는게 안정적일 것 같음
	CArray<int, int> m_aTabLcomType;

	std::shared_ptr<CCMLoadCombMainGrid> m_pLoadCombList;
	std::shared_ptr<CCMLoadCombSubGrid>  m_pContentList;
	std::shared_ptr<CCMLoadCombGrid>     m_pCombList;
	int m_nLcomType;

	BOOL m_bTB10002_1_05;

// resizing
	CRect m_rectDlg;
	CRect m_rectItem[9];
	BOOL m_bPosSet;
	LCOM_TYPE m_nSeismicType;
	CRect m_defaultRect;
private:
	// Dialog Data
	MComboBox	m_cmbLcomType;
	MButton	m_wndAutoGenBtn;
	MTabCtrl	m_wndTab;
	bool isMaximized = false;
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMLOADCOMBDLG_H__)
