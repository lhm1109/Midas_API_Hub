// DgnSeisMPhiCurveGraph.h: interface for the CDgnSeisMPhiCurveGraph class.
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNSEISMPHICURVEGRAPH_H__)
#define __DGNSEISMPHICURVEGRAPH_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\chartdata.h"
#include "..\wg_dgn\BrdgSeisEval.h"

#define D_SEIS_MPHI_PNT_NUM 6

struct T_CurveOpt_D
{
	double dAxialForce;
	BOOL   bIdealize;
	BOOL   bCurvature;
	double dCurvature;

	int    nAngleDir;  // 0:Longitudinal Dir. 1:Transverse Dir.
	int    nCurveType; 

	void Initialize()
	{
		dAxialForce = 0.0;
		bIdealize = FALSE;
		bCurvature = FALSE;
		dCurvature = 0.0;

		nAngleDir = 0;
		nCurveType = 0;
	}

	T_CurveOpt_D()	{}
	T_CurveOpt_D(T_CurveOpt_D& rData)	{*this = rData;}
	T_CurveOpt_D& operator = (T_CurveOpt_D& rData)
	{
		dAxialForce = rData.dAxialForce;
		bIdealize = rData.bIdealize;
		bCurvature = rData.bCurvature;
		dCurvature = rData.dCurvature;

		nAngleDir = rData.nAngleDir;
		nCurveType = rData.nCurveType;

		return *this;
	}
};

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisMPhiCurveGraph : public SREGraphView
{
public:
	CDgnSeisMPhiCurveGraph();
	virtual ~CDgnSeisMPhiCurveGraph();
//   DECLARE_DYNCREATE(CDgnSeisMPhiCurveGraph)
	
 // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCS_TextDrawView)
	virtual void OnInitialUpdate();
	virtual void ShowFeedback(SRGraphFeedback *pFeedback);
	void MakeBmp(CString strFilePath);
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void PostNcDestroy();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif  
	  
	//}}AFX_VIRTUAL
 // Implementation

public:
	void DrawGraph();
	void InitGrph();

	void SetData(T_BRDGEVAL_MPHI_RES MphiResD[2]) { m_MphiResD[0] = MphiResD[0]; m_MphiResD[1] = MphiResD[1]; };
	void SetCurveOpt(T_CurveOpt_D* pCurveOptD) { m_pCurveOptD = pCurveOptD; }

protected:
 	void SetValue(int nIndex, int nGroup, double x,double y);
	void SetValueDot(int nIndex, int nGroup, double x,double y);
	void AddBackGrndComponent();
	void AddTitleComponent();
	void AddDisplayComponent();
	void AddXAxisLabelComponent();
	void AddYAxisLabelComponent();
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);

	CChartData      m_Graph;

	T_BRDGEVAL_MPHI_RES m_MphiResD[2];
	T_CurveOpt_D* m_pCurveOptD;

	//{{AFX_MSG(CUmdSectionDrawWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISMPHICURVEGRAPH_H__)
