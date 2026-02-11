#if !defined(__TM_STLD_WIND_ROOT_DLG_H__)
#define __TM_STLD_WIND_ROOT_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\wg_cmd\CMDlgBarBase.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_cmd\CMWindPressureBaseDlg.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureRootDlg dialog
class CMenuBarChildDlg;
class CStldWindPressureRootDlg : public CCMDlgBarBase, public CDBUpdateConnector 
{
public:
	CStldWindPressureRootDlg(CWnd* pParent=NULL);
	virtual ~CStldWindPressureRootDlg();

	DECLARE_DYNCREATE(CStldWindPressureRootDlg)

	enum { IDD = IDD_TM_STLD_WINDP_ROOT_DLG };

	void CreateChildDlgs();
	void DeleteChildDlgs();

	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void LayoutChildDialog();
	virtual void LayoutAdjust() { LayoutChildDialog(); }

	int  _GetHeightFromFont();
	void InitComboMenu();

	virtual void OnTmExecute();
	virtual void OnTmClose();
	
	virtual void OnInitialUpdate();
	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam) {}
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam) {}
	virtual CWnd* GetDlgByID(int nID);

	MButton	     m_wndTableButton;
	MComboBox	   m_ComboMenu;  
	CPlaceHolder m_wndPlaceHolder;  

protected:
#define CRootChildDlg CCMWindPressureBaseDlg
	CArray<CRootChildDlg*, CRootChildDlg*> m_ChildDlgs;
	CArray<int,int>                        m_DlgIDs;    //__TMMODE_CMD_WIND_PRESSURE_AREA__ ÂüÁ¶.... TreeMenuMode.h
	CArray<UINT, UINT>                     m_TableIDs;
	CStringArray                           m_ComboStrs;
	
	int   m_nCurChildDlg;
	CRect m_RectPlaceHolder;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCmdTableButton();
	afx_msg void OnComboMenuSelect();

	DECLARE_MESSAGE_MAP()
};

#endif
