#if !defined(AFX_STLDCAMBERFORCS_H__)
#define AFX_STLDCAMBERFORCS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldCamberForCS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStldCamberForCS dialog
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\SelectCtrl.h"

#include "..\wg_tb\TBPostCamb.h"


class CStldCamberForCS : public CMenuBarChildDlg , public CDBUpdateConnector
{
	// Construction
public:
	CStldCamberForCS(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

	// Dialog Data
	//{{AFX_DATA(CStldCamberForCS)
	enum { IDD = IDD_TM_STLD_CMCS };

	int		      m_nOption;
	int         m_nCamberType;
	int         m_nCamberResult;
	CSelectEdit m_edtSelNode;
	CEditUnit   m_edtCamber;
	CTextUnit   m_untCamber;
	CMouseEdit  m_edtStart;
	CMouseEdit  m_edtEnd;
	CEditUnit   m_edtMaxCamber;
	CTextUnit   m_untMaxCamber;
	//}}AFX_DATA

protected:
	void InitUnit();
	void InitControls();
	void InitMemberData();
	void AlignControls();
	void EnableDisableControls();
	BOOL IsCamberRst();
	void ControlsShowHide();
	void GetNodeCamberSummationData(int nRstType, CMap<T_NODE_K,T_NODE_K, double, double>& mapNodeCamberSummationData);
	BOOL CalcCamberValByLength(CArray<T_NODE_K, T_NODE_K>& aNodeKey,	double WX1, double WY1, double WZ1, double WX2, double WY2, double WZ2, CArray<double, double>& aCamberVal);

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);


	CArray<UINT, UINT> m_aDefCamber;
	CArray<UINT, UINT> m_aKeyNodeCamber;
	CArray<UINT, UINT> m_aUserCamber;


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldCamberForCS)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStldCamberForCS)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmCamberOptRdo();
	afx_msg void OnTmCamberTypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDCAMBERFORCS_H__)

