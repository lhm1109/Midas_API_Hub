#if _MSC_VER > 1000
#pragma once
#endif

#include "DrawEditBtn.h"
#include "DgnTendonTemplateData.h"

#include "..\dgnengine\src\DgnBase\DgnBaseShape_Struct.h"
#include "..\dgnengine\src\DgnBase\DgnBase_Struct.h"
#include "..\wg_db\DB_ST_DT.h"

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnDraw_SectViewPannel CDrawEditBtn
//
/////////////////////////////////////////////////////////////////////////////

enum DRAW_EDIT_BTN_TYPE
{
	DRAW_EDIT_BTN_TYPE_YZ = 0,
	DRAW_EDIT_BTN_TYPE_XY,
	DRAW_EDIT_BTN_TYPE_XZ
};

class CUnitCtrl;
class CDgnTendonTemplateCtrl;
class __MY_EXT_CLASS__ CDrawEditBtn_TndnTemplate : public CDrawEditBtn
{
// Construction
public:
	CDrawEditBtn_TndnTemplate(CDgnTendonTemplateCtrl* pTemplateCtrl);   // standard constructor
	CDrawEditBtn_TndnTemplate();
	virtual ~CDrawEditBtn_TndnTemplate();

	void SetTemplateCtrl(CDgnTendonTemplateCtrl* pTemplateCtrl) { m_pTemplateCtrl = pTemplateCtrl; }

protected:	// Implementation
	DECLARE_MESSAGE_MAP()
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:	//variable
	CDgnTendonTemplateCtrl*		m_pTemplateCtrl;
	CUnitCtrl* m_pUnitCtrl;

	DGN_GSEC_SHAPE_D			m_SectD;   // 단면정보
	DGN_GSEC_POLYGON_LIST		m_arConcOutPoly;
	DGN_GSEC_POLYGON_LIST		m_arConcInPoly;
	DGN_GSEC_POLYGON_LIST		m_arSteelOutPoly;
	DGN_GSEC_POLYGON_LIST		m_arSteelInPoly;
	DGN_GSEC_LINE_LIST			m_arSteelLine;
	DGN_GSEC_VERTEXLINE_LIST	m_arSteelVertexLine;
	DGN_GSEC_POLYGON_LIST		m_arSteelLinePoly;
	double m_dOutPos[6];		// 단면의 경계값 ([0]=dOffsetX, [1]=dOffsetY, [2]=dXmax, [3]=dXmin, [4]=dYmax, [5]=dYmin)
	CArray<DGN_GSEC_VERTEX, DGN_GSEC_VERTEX&> m_aSnapVertex;
	// 공통
	DRAW_EDIT_BTN_TYPE    m_nDrawType;// 0:YZ, 1:XY, 2: XZ             //None, 1:Rebar, 2:Stiffener, 3:Stress Point, 4:Part, 5:Stiffness
	BOOL	m_bCenter;
	int		m_nSectSnapType; // Section정보에 Snap을 지원할지 여부
	BOOL	m_bLineDetail;
	BOOL	m_bIsOverlapChk; // Rebar와 Stress Point 중첩여부 Check여부
	BOOL	m_bShowElemLine;
	BOOL	m_bShowPreview;
	BOOL  m_bHasList;
	BOOL  m_bUseOrigin;
	BOOL  m_bMoveOrigin;

	//Formats
	UINT	m_uiDrawFormatKeySection;
	UINT	m_uiDrawFormatKeySection_Inside;
	UINT	m_uiDrawFormatKeySection_Line;
	UINT	m_uiDrawFormatKeySection_XYZ;
	UINT	m_uiDrawFormatKeyElemLine;
	UINT	m_uiDrawFormatKeyTndn_Debonded;
	UINT	m_uiDrawFormatKeyTndn;
	UINT	m_uiDrawFormatKeyTndn_Preview;
	UINT	m_uiDrawFormatKeyText;
	UINT	m_uiDrawFormatKeyDimen;
	UINT	m_uiDrawFormatKeyDimen_Vert;
	UINT	m_uiDrawFormatKeyDimen_Horz;
	UINT	m_uiDrawFormatKeyOrigin;
	
	double m_dGapY;
	double m_dGapZ;
	double m_dTotalHeight;
	double m_dTotalWidth;

	double m_dBoundBox_Left;
	double m_dBoundBox_Right;
	double m_dBoundBox_Top;
	double m_dBoundBox_Bottom;
	
	double m_dMarginFactor[3];

	double m_dPos;
	double m_dTendonZoomFactor; // 단면안의 텐던 굵기 표현. default : 0.01

	int    m_nAxisMargin;

	UINT   m_uiOrigin; //Origin Key...
	BOOL   m_InitOrigin;
	double m_dOrigin_Org[2];

	std::map<double, int> m_mapVertDim;

	CArray<T_NODE_D, T_NODE_D&> aTendonProfile;
	CArray<BOOL, BOOL> aProfileInputPoint;

	std::map<int, TndnDrawStruct> m_mapDrawTendonData;
	std::vector<TndnDrawStruct> m_vecDrawTendonData_Preview;
	TndnTemplate* m_pTndnData;

protected:	//function
	void ResetVariable(BOOL bWithSect);
	void ResetVariable_Sect();
	void ResetSnapPos();
	void CalcBoundingBox(double dX, double dY, double* dLeft = NULL, double* dRight = NULL, double* dTop = NULL, double* dBottom = NULL);
	void GetPosByCPoint(CPoint& cPt, double& dPtX, double& dPtY, int& nSnapX, int& nSnapY, BOOL bSnap = FALSE);

 	BOOL IsCompositeSection(int nShape);  // nShape : DGN_SECT_TYPE_XXX
	void MakeCurrLinePreview(double dPtX, double dPtY, int nSnapX, int nSnapY);
	void MakeDrawSectUnit_YZ();
	void MakeDrawSectUnit_XZXY();
	void MakeDrawTndn_XZXY();
	void MakeDrawTndn_XZXY_Debonded(TndnDrawStruct* pDrawStruct, T_LINE_R_GROUP& tLineGroup, T_LINE_R_GROUP& tLineGroupF, T_LINE_R_GROUP& tLineGroupE);
	void MakeDrawTndn_YZ();
	void MakeDrawDim_YZ();
	void MakeDrawOrigin();
	virtual void MakeDrawSubClass();  
	virtual void SetParentOriginPos_AutoDlg(double dPos[2]);
	virtual void SetParentSelectedKey( int nIdx, vecTndnKey& keyList, nrUINT& nrUintTarget, int idxOrigin );
	virtual void SetParentOriginPos( T_POINT_R_GROUP &tPointGroup );

	void DrawAxisUnit(CDrawCtrl* pDrawCtrl, int nAxisMode, int nAxisSize, int nAxisWidth, BOOL bCapital, T_DRAW_FORMAT AxisDrawFormat,
								BOOL bCDC_AxisOrgPoint, CPoint AxisOrgPoint, T_POINT AxisOrgPointR, double dAxis_Vector1[2], double dAxis_Vector2[2]);
	
	
	virtual double GetScaledElemLen(double dElemLen, T_ELEM_K ElemK);
	virtual void SetDrawFormat();
	virtual void MakeDrawPreviewData();

	virtual void StartCurrUnitSelect(CPoint CurrP); // LButtonDown
	virtual void CompleteCurrUnitSelect(CPoint CurrP); // LButtonUp

	

	virtual void InProgressCurrUnitSelect(CPoint CurrP);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); // 직접적으로 연결되지 않으므로 부모에서 부터 연결

public:
	virtual void MakeDrawUnit(BOOL bFit = TRUE);

public:		//function
	void SetDrawType(DRAW_EDIT_BTN_TYPE nType, BOOL bRemake = FALSE);

	BOOL SetSectData(T_SECT_D& SectD, T_ELEM_K tElemKey, double dPos, BOOL bAfter, BOOL bCenter, BOOL bFit = TRUE);
	BOOL SetTdnaData(TndnTemplate* pData = NULL, BOOL bFit = TRUE);
	BOOL SetOriginPt_Init(T_SECT_D& rSectD);
	BOOL IsSamePolygon(const DGN_GSEC_POLYGON_LIST& arOrg, const DGN_GSEC_POLYGON_LIST& arCmp, double& dDiffY, double& dDiffZ); //// Diff는 Org-Cmp

	void Redraw(BOOL bFit = TRUE);

	void SetShowElemLine(BOOL bShow)	{ m_bShowElemLine = bShow; }
	BOOL GetShowElemLine()		     		{ return m_bShowElemLine; }
	void SetShowPreview(BOOL bShow)		{ m_bShowPreview = bShow; }
	BOOL GetShowPreview()				      { return m_bShowPreview; }
	void SetHasList(BOOL bSet)        { m_bHasList = bSet; }
	BOOL GetHasList()                 { return m_bHasList; }
	void SetUseOrigin(BOOL bUse)      { m_bUseOrigin = bUse; }
	BOOL GetUseOrigin()               { return m_bUseOrigin; }

	// Origin 좌표를 가져온다.(현재단위계)
	BOOL GetDrawOrigin(double* dOrigin);
	// Origin 좌표를 수정 그린다.(현재단위계)
	void SetDrawOrigin(double* dOrigin, BOOL bConsiderBound, BOOL bSetParent);

	BOOL SetPreviewData(const vecTndnTemp& vecTndnTempInfo, BOOL bFit);
	void SetSelectedLinkKeyListByTndnKey(const vecTndnKey& keyList, BOOL bRedraw = TRUE);
	
};

#include "HeaderPost.h"
