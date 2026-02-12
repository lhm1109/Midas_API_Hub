#if !defined(__AFX_SECTFORRESULTANTFORCEDLG_H__)
#define __AFX_SECTFORRESULTANTFORCEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SectForResultantForceDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_CobxGrup.h"
#include "..\wg_db\UserDefType.h"

#include "..\wg_base\FoldGroupBox.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CSectForResultantForceDlg dialog

class CSectRForcGrid;
class CSectRVbemGrid;
class CSectForResultantForceDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CSectForResultantForceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() {};
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CSectForResultantForceDlg)
	enum { IDD = IDD_TM_SECT_FOR_RFOC };
	//}}AFX_DATA
	mit::frx::MComboBox m_cmbMode;
	CEditUnit m_edtTolerance;	
	CTextUnit m_untTolerance;
	mit::frx::MEdit     m_edtDiv;
	CFoldGroupBox_Cross	m_grpCoord;
	CMouseEdit	m_edtInputCoord;
	mit::frx::MEdit	m_edtInput;
	mit::frx::MButton m_chkZVec;
	CMouseEdit	m_edtZVec;
	mit::frx::MEdit	m_edtName;
	CCobxGrup	m_wndGrupCobx;
	CArray<UINT, UINT> m_aCtrlCoord;
	CArray<UINT, UINT> m_aCtrlTol;
	CArray<UINT, UINT> m_aCtrlDiv;
	CArray<UINT, UINT> m_aCtrlName;
	CArray<UINT, UINT> m_aCtrlGrup;

// Overrides
public:
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectForResultantForceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CSectRForcGrid* m_pGridLoc;
	CSectRVbemGrid* m_pGridVBeam;
	
	void UpdateBuffer();
	// Generated message map functions
	//{{AFX_MSG(CSectForResultantForceDlg)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnSelchangeCmbMode();
	afx_msg void OnTmBtnAdd();
	afx_msg void OnTmBtnMod();
	afx_msg void OnTmBtnDel();
	afx_msg void OnTmChkZVector();
	afx_msg void OnSelchangeTmGrupCmb();
	afx_msg void OnDestroy();
	afx_msg void ConvertToVsec();
	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseEditEnter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseEditEscape(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMViewCancel(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnVsecGridClicked(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVbemGridClicked(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	double	m_dTolerance;

	BOOL   m_bIsComplete;
	CArray<double,double> m_arXYZ[3]; 
	double m_dZVec[3];

protected:
	void InitControls();
	void InitGrid();
	void AlignControls();
	void ShowHideCtrls();
	void EnableDisableCtrls();
	void Dlg2Data();

	int   GetMode();
	void  SetSelFaceEdgeMode(BOOL bInit=TRUE);
	void  SetZVec(double dX, double dY, double dZ);

	void ResetPolygonCoord();

private:
	void CalcCentroidAuto();
	void CalcCentroidAuto_PolyGon();
	//void CalcCentroidAuto_Elements();
	BOOL IsElemOutofMinMax(T_ELEM_D& ElemD, double dMin, double dMax, int iDir, double dTol);
	BOOL IsElemOutofMinMax(T_ELEM_D& ElemD, double dMinX, double dMaxX, double dMinY, double dMaxY, double dMinZ, double dMaxZ, double dTol);
	BOOL IsElemIntersectWithConvexHull(T_ELEM_D& ElemD, T_POLYLINE2D& ConvxHull, double dTol);
	int  GetSelectedElemKeyList(CArray<UINT,UINT>& raElemK_sel);
	int  GetSelectedNodeKeyList(CArray<UINT,UINT>& raNodeK_sel);
	int  GetSelectedGrupElemKeyList(CArray<UINT,UINT>& raElemK_sel);
	BOOL GetCuttingLine2D(T_POINT2D& pt2D_CutP1, T_POINT2D& pt2D_CutP2, const T_POINT3D& ptDiv, const T_POINT3D& pt3D_normalVec, const double dCuttingScale);
	int  GetCuttingElemList(CArray<UINT,UINT>& raElemK_sel);
	int  GetCuttingElemList(CArray<UINT,UINT>& raElemK_sel, const CArray<double,double> arXYZ[3], const CArray<UINT,UINT>& aElemK_candi);
	int  GetElemListByCutting(CArray<UINT,UINT>& raElemK_cutting, const T_POINT2D& pt2D_CutP1, const T_POINT2D& pt2D_CutP2, const CArray<UINT,UINT>& aElemK_candi);
	void GetMBR3D(const CArray<double,double> arXYZ[3], double& dMaxX,double& dMinX, double& dMaxY,double& dMinY, double& dMaxZ,double& dMinZ);
	BOOL GetDirVectorFromPolygon(double dZVec[3], CArray<double,double> arXYZ[3]);
	BOOL GetCutPoints2DfromPolygon(T_POINT2D& pt2D_CutP1, T_POINT2D& pt2D_CutP2, CArray<double,double> arXYZ[3]);

	BOOL GetStartEndPosition(T_NODE_D& NodeD_start, T_NODE_D& NodeD_end, T_POINT3D& pt3DOnCuttingPlane);
	BOOL GetDataVsec_fromPolygon(T_VSEC_D& VsecD, int iDir, LPCTSTR lpszName);
	BOOL GetDataVsec_fromElemNode(T_VSEC_D& VsecD, 
		const CArray<T_ELEM_K,T_ELEM_K>& aElemK, const CArray<T_NODE_K,T_NODE_K>& aNodeK, 
		const T_POINT3D& ptCentG, const double dZVec[3], LPCTSTR lpszName);
	CString GetSelectedGrupName();
	BOOL GetVecFromMouseEdit(CMouseEdit& editMouse, double dZVec[3]);
	std::map<T_VBEM_K, T_SECT_D> m_mapVbemToSect;
	std::map<T_VBEM_K, T_SECV_D> m_mapVbemToSecv;
	BOOL CSectForResultantForceDlg::VirtualSection_PSC_I(T_SECT_K SectK,
		CArray<T_NODE_K, T_NODE_K>& arSlabNode, CArray<T_NODE_K, T_NODE_K>& arGirderNode,
		CArray<T_ELEM_K, T_ELEM_K>& arSlabElem, CArray<T_ELEM_K, T_ELEM_K>& arGirderElem,
		T_VSEC_D& VsecD, T_SECT_D& SectT2,double tc);
	std::map<T_SECV_K, std::vector<T_ELEM_K>> m_mapSecvToElem;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__AFX_SECTFORRESULTANTFORCEDLG_H__)
