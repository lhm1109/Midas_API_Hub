#if !defined(AFX_SEISIS1893PERIODCALCHELPER_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_)
#define AFX_SEISIS1893PERIODCALCHELPER_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisIS02PeriodCalcDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\StoryData.h"
#include "..\wg_db\UserDefType.h"

struct T_NPC_IS1893_2025_DIR_D
{
	int nMethod;
	double dH;
	double dAfp;
	double dAmw;
	double dAc;
	double dAw;
	double dD;

	void Initialize()
	{
		nMethod = 0;
		dH = 0.0;
		dAfp = 0.0;
		dAmw = 0.0;
		dAc = 0.0;
		dAw = 0.0;
		dD = 0.0;
	}
};

struct T_NPC_IS1893_2025_D
{
	T_NPC_IS1893_2025_DIR_D DirX;
	T_NPC_IS1893_2025_DIR_D DirY;
	T_STOR_K nBaseStoryK;

	void Initialize()
	{
		DirX.Initialize();
		DirY.Initialize();
		nBaseStoryK = 0;
	}
};

class CSeisIS1893_25PeriodCalcHelper
{
	// Construction
public:
	CSeisIS1893_25PeriodCalcHelper(T_STOR_K nBaseStoryK);   // standard constructor
	~CSeisIS1893_25PeriodCalcHelper();
	double m_dPeriod_X;
	double m_dPeriod_Y;
	bool m_bFileOpened;
	double m_dH;
	double m_dAmw;
	double m_drhomwall;
	double m_drhocol;
	double m_dT;
	double m_dAreaCol;
	double m_dAreaWall;
	double m_dAreaFloorPlan;
	T_STOR_K m_nBaseStoryK;
	double m_dBaseLevel;
	FILE* m_pOut;
	bool m_bPrint;
	FILE* m_Fout;
	CString m_strFile;
	//}}AFX_DATA
private:
	double m_dWallTerm;  //Stores Summation(rho_w*(0.2+(L/H)^2))
private:
	CDBDoc* m_pDoc;
	CAttrCtrl* m_pAttrCtrl;
	CUnitCtrl* m_pUnitCtrl;

private:
	CArray<std::tuple<T_ELEM_K, double, double>, std::tuple< T_ELEM_K, double, double>&> m_arWallData;

public:
	//double CalcTimePeriod(double dAc, double dAmw, double dH, double dAfp, int nDir,FILE* Fout, bool bPrint);
	double CalcTimePeriod(T_NPC_IS1893_2025_DIR_D& rData, int nDir, FILE* Fout, bool bPrint, bool bCalc);
private:
	//void InitializeAndCalcData(double dAfp, double dAc, double dAmw, double dH, int nDir);
	void InitializeAndCalcData(T_NPC_IS1893_2025_DIR_D& rData, int nDir, bool bCalc);
	double CalcFloorPlanArea();
	double CalcAreaFromNodeArr(CArray<T_NODE_K, T_NODE_K>& rArNodes);
	double Cross(const T_POINT2D& O, const T_POINT2D& A, const T_POINT2D& B);
	std::vector<T_POINT2D> ConvexHull(std::vector<T_POINT2D> points);
	double AreaPolygon(const std::vector<T_POINT2D>& vertices);
	double AreaTriangle(const T_NODE_D& A, const T_NODE_D& B, const T_NODE_D& C);
	T_STOR_K GetBaseStoryKey();
	void GetWallData(int nDir);
	double GetColArea();
	double GetLengthAlong(const T_NODE_K Node1, const T_NODE_K Node2, int nDir);
	void PrintCalcTimePeriod(FILE* Fout, int nDir);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISIS02PERIODCALCDLG_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_)