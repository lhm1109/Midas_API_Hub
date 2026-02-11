// SeisIS02PeriodCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisIS1893_25PeriodCalcHelper.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_SpecialChar.h"
#include "..\wg_base\TextFileOut.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisIS1893_25PeriodCalcHelper dialog

CSeisIS1893_25PeriodCalcHelper::CSeisIS1893_25PeriodCalcHelper(T_STOR_K nBaseStoryK)
{
	m_dH = 0.0;
	m_bPrint = FALSE;
	m_Fout = NULL;
	m_dAreaFloorPlan = 0.0;
	m_dAreaCol = 0.0;
	m_dAreaWall = 0.0;
	m_dWallTerm = 0.0;
	m_bFileOpened = false;
	m_dAmw = 0.0;
	m_drhomwall = 0.0;
	m_drhocol = 0.0;
	m_nBaseStoryK = nBaseStoryK;
	m_dT = 0.0;
}

CSeisIS1893_25PeriodCalcHelper::~CSeisIS1893_25PeriodCalcHelper()
{
}

void CSeisIS1893_25PeriodCalcHelper::InitializeAndCalcData(T_NPC_IS1893_2025_DIR_D& rData, int nDir, bool bCalc)
{
	m_pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	m_pAttrCtrl = m_pDoc->m_pAttrCtrl;
	m_pUnitCtrl = m_pDoc->m_pUnitCtrl;

	m_dH = rData.dH;

	if(dgn::EQ0(rData.dAfp) || bCalc)
	{
		m_dAreaCol = rData.dAc = GetColArea();
		m_dAreaFloorPlan = rData.dAfp = CalcFloorPlanArea();
	}
	else
	{
		m_dAreaCol = rData.dAc;
		m_dAreaFloorPlan = rData.dAfp;
	}
	ASSERT(!dgn::EQ0(m_dAreaFloorPlan));
	m_dAmw = rData.dAmw;
	GetWallData(nDir);
	rData.dAw = m_dAreaWall;
}

double CSeisIS1893_25PeriodCalcHelper::CalcFloorPlanArea()
{
	CStoryData StoryData(m_pDoc);
	// Nodes and elements connected to the corresponding layer and the upper layer
	int nSelectionType = 5;
	StoryData.MakeStoryData(nSelectionType);
	CArray<T_NODE_K, T_NODE_K> aNodeK;    aNodeK.RemoveAll();
	StoryData.GetStoryNodeKey(m_nBaseStoryK, aNodeK);

	return CalcAreaFromNodeArr(aNodeK);
}

double CSeisIS1893_25PeriodCalcHelper::CalcAreaFromNodeArr(CArray<T_NODE_K, T_NODE_K>& rArNodes)
{
	int nNumNodes = rArNodes.GetSize();//
	std::vector<T_POINT2D> vPoints; vPoints.clear();

	for(int i = 0; i < nNumNodes; i++)
	{
		T_NODE_D NodeD; NodeD.Initialize();
		if(!m_pAttrCtrl->GetNode(rArNodes.GetAt(i), NodeD)) continue;
		vPoints.push_back(T_POINT2D(NodeD.x, NodeD.y));
	}
	auto HullD = ConvexHull(vPoints);

	return AreaPolygon(HullD);
}

double CSeisIS1893_25PeriodCalcHelper::Cross(const T_POINT2D& O, const T_POINT2D& A, const T_POINT2D& B)
{
	return ( A.x - O.x ) * ( B.y - O.y ) - ( A.y - O.y ) * ( B.x - O.x );
}

// Convex Hull - Andrew's monotone chain
std::vector<T_POINT2D> CSeisIS1893_25PeriodCalcHelper::ConvexHull(std::vector<T_POINT2D> points)
{
	if(points.size() <= 1) return points;

	std::sort(points.begin(), points.end(), [](const T_POINT2D& a, const T_POINT2D& b) {
		return a.x < b.x || ( a.x == b.x && a.y < b.y );
	});

	std::vector<T_POINT2D> hull;

	// Lower hull
	for(const auto& p : points)
	{
		while(hull.size() >= 2 &&
			Cross(hull[hull.size() - 2], hull.back(), p) <= 0)
			hull.pop_back();
		hull.push_back(p);
	}

	// Upper hull
	size_t lower_size = hull.size();
	for(auto it = points.rbegin(); it != points.rend(); ++it)
	{
		while(hull.size() > lower_size &&
			Cross(hull[hull.size() - 2], hull.back(), *it) <= 0)
			hull.pop_back();
		hull.push_back(*it);
	}

	// Remove duplicate start/end point
	hull.pop_back();
	return hull;
}

double CSeisIS1893_25PeriodCalcHelper::AreaPolygon(const std::vector<T_POINT2D>& vertices)
{
	double area = 0.0;
	size_t n = vertices.size();

	for(size_t i = 0; i < n; i++)
	{
		size_t j = ( i + 1 ) % n;
		area += vertices[i].x * vertices[j].y;
		area -= vertices[j].x * vertices[i].y;
	}

	return std::abs(area) / 2.0;
}

T_STOR_K CSeisIS1893_25PeriodCalcHelper::GetBaseStoryKey()
{
	m_dBaseLevel = m_pAttrCtrl->GetBaseLevel(TRUE);
	CStoryData StoryData(m_pDoc);

	// Nodes and elements connected to the corresponding layer and the upper layer
	int nSelectionType = 5;
	StoryData.MakeStoryData(nSelectionType);

	CArray<T_STOR_K, T_STOR_K> aStorK;   aStorK.RemoveAll();
	m_pAttrCtrl->GetStorKeyList(aStorK);

	CArray<T_NODE_K, T_NODE_K> aNodeK;    aNodeK.RemoveAll();
	double dZ = DBL_MAX;
	T_STOR_K nBaseStoryK = 0;
	for(int i = 0; i < aStorK.GetSize(); ++i)
	{
		StoryData.GetStoryNodeKey(aStorK[i], aNodeK);
		for(int j = 0; j < aNodeK.GetSize(); ++j)
		{
			T_NODE_D NodeD; NodeD.Initialize();
			if(m_pAttrCtrl->GetNode(aNodeK[j], NodeD))
			{
				if(dgn::EQ(NodeD.z, m_dBaseLevel))
				{
					dZ = NodeD.z;
					m_dBaseLevel = dZ;
					nBaseStoryK = aStorK[i];
					break;
				}
				else if(NodeD.z > m_dBaseLevel && NodeD.z <= dZ)
				{
					dZ = NodeD.z;
					m_dBaseLevel = dZ;
					nBaseStoryK = aStorK[i];
					break;
				}
				else
				{
					break;
				}
			}
			else
			{
				ASSERT(0);
			}
		}
	}
	return nBaseStoryK;
}

void CSeisIS1893_25PeriodCalcHelper::GetWallData(int nDir)
{
	CStoryData StoryData(m_pDoc);
	StoryData.MakeStoryData(1);  //Nodes and elems corresponding to current and upper portion
	CArray<T_ELEM_K, T_ELEM_K> arElemK; arElemK.RemoveAll();
	StoryData.GetStoryElemKey(m_nBaseStoryK, arElemK);
	m_dAreaWall = 0.0;
	m_dWallTerm = 0.0;
	m_arWallData.RemoveAll();
	for(int i = 0; i < arElemK.GetSize(); i++)
	{
		if(!m_pAttrCtrl->IsConWall(arElemK.GetAt(i)))
		{
			continue;
		}
		T_ELEM_D ElemD;
		T_ELEM_K ElemK = arElemK.GetAt(i);
		if(!m_pAttrCtrl->GetElem(ElemK, ElemD))
		{
			ASSERT(0); continue;
		}

		T_MATL_D MatlD;
		if(!m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) { ASSERT(0); continue; }
		if(MatlD.Type != _LSX(C)) { continue; }

		T_THIK_D ThikD;
		if(!m_pAttrCtrl->GetThik(ElemD.elpro, ThikD)) { ASSERT(0); continue; }

		T_NODE_K Node1 = ElemD.elnod[0];
		T_NODE_K Node2 = ElemD.elnod[1];

		double dLength = GetLengthAlong(Node1, Node2, nDir);

		double dThick = ThikD.Value.ThickIn;
		if(dgn::EQ0(dThick)) { ASSERT(0); }

		m_dAreaWall += ( dThick * dLength );
		/*CString strWallData = _T("");
		strWallData.Format(_T("ElemK: %d, LengthX: %4.5f, LengthY: %4.5f, Thik: %4.5f"), ElemK, dLengthX, dLengthY, dThick);*/

		double dWallRho = dThick * dLength / m_dAreaFloorPlan;
		// Store wall rho and length data
		m_arWallData.Add(std::make_tuple(ElemK, dWallRho, dLength));

		m_dWallTerm += dWallRho * ( 0.2 + ( dLength / m_dH ) * ( dLength / m_dH ) );
	}
}

double CSeisIS1893_25PeriodCalcHelper::GetLengthAlong(const T_NODE_K Node1, const T_NODE_K Node2, int nDir)
{
	T_NODE_D Node1D, Node2D;
	if(!m_pAttrCtrl->GetNode(Node1, Node1D) || !m_pAttrCtrl->GetNode(Node2, Node2D)) { ASSERT(0); return 0.0; }
	switch(nDir)
	{
	case 3:  //Both
	{
		double dx = Node1D.x - Node2D.x;
		double dy = Node1D.y - Node2D.y;
		return sqrt(dx * dx + dy * dy);
	}
	case 1:	//X direction
		return fabs(Node1D.x - Node2D.x);
	case 2:	//Y direction
		return fabs(Node1D.y - Node2D.y);
	default:
	{
		ASSERT(0);
		return 0.0;
	}
	}
}

double CSeisIS1893_25PeriodCalcHelper::GetColArea()
{
	CStoryData StoryData(m_pDoc);
	StoryData.MakeStoryData(1);  //Nodes and elems corresponding to current and upper portion
	CArray<T_ELEM_K, T_ELEM_K> arElemK; arElemK.RemoveAll();
	StoryData.GetStoryElemKey(m_nBaseStoryK, arElemK);
	double dAreaColumn = 0.0;
	for(int i = 0; i < arElemK.GetSize(); i++)
	{
		if(!m_pAttrCtrl->IsColmTypeElem(arElemK.GetAt(i)))
		{
			continue;
		}
		T_ELEM_D ElemD;
		T_ELEM_K ElemK = arElemK.GetAt(i);
		if(!m_pAttrCtrl->GetElem(ElemK, ElemD))
		{
			ASSERT(0); continue;
		}

		T_MATL_D MatlD;
		if(!m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) { ASSERT(0); continue; }
		if(MatlD.Type != _LSX(C) && MatlD.Type != _LSX(SRC)) { continue; }

		T_SECT_D SectD;
		if(!m_pAttrCtrl->GetSect(ElemD.elpro, SectD)) { ASSERT(0); continue; }
		if(SectD.nStype != D_SECT_TYPE_REGULAR && SectD.nStype != D_SECT_TYPE_SRC) { continue; }

		if(SectD.nStype == D_SECT_TYPE_REGULAR)
		{
			dAreaColumn += SectD.SectBefore.SectI.Stiffness.Area;
		}
		else if(SectD.nStype == D_SECT_TYPE_SRC)
		{
			dAreaColumn += SectD.SectBefore.SectI.Stiffness.Area;
		}
	}
	return dAreaColumn;
}

double CSeisIS1893_25PeriodCalcHelper::CalcTimePeriod(T_NPC_IS1893_2025_DIR_D& rData, int nDir, FILE* Fout, bool bPrint, bool bCalc)
{
	m_Fout = Fout;
	m_bPrint = bPrint;
	InitializeAndCalcData(rData, nDir, bCalc);
	double dNumerator = 0.075 * pow(rData.dH, 0.75);
	double drhomwall = rData.dAmw / m_dAreaFloorPlan;
	double drhocol = m_dAreaCol / m_dAreaFloorPlan;
	double dDenominator = sqrtf(1.0 + m_dWallTerm + ( 0.2 * m_dAreaCol / m_dAreaFloorPlan ) + ( 0.02 * rData.dAmw / m_dAreaFloorPlan ));
	m_drhomwall = drhomwall;
	m_drhocol = drhocol;
	double dT = dgn::SafeDiv(dNumerator, dDenominator);
	m_dT = dT;
	if(bPrint && m_Fout)
	{
		PrintCalcTimePeriod(m_Fout, nDir);
	}
	return dT;
}
void CSeisIS1893_25PeriodCalcHelper::PrintCalcTimePeriod(FILE* Fout, int nDir)
{
	if(!m_Fout) return;
	CString strDir = ( nDir == 1 ) ? _T("X") : _T("Y");

	CTextFileOut::Write(Fout, _T("\n\n"));
	CTextFileOut::Write(Fout,
		_T("IS 1893:2025 PERIOD CALCULATION REPORT (%s-DIRECTION)\n"), strDir);
	CTextFileOut::Write(Fout,
		_T("---------------------------------------------------------------------------------------\n"));
	CTextFileOut::Write(Fout,
		_T(" Height of building (H)         = %.2f m\n"), m_dH);
	CTextFileOut::Write(Fout,
		_T(" Total floor plan area (Afp)    = %.3f m^2\n"), m_dAreaFloorPlan);
	CTextFileOut::Write(Fout,
		_T(" Column area (Ac)               = %.3f m^2\n"), m_dAreaCol);
	CTextFileOut::Write(Fout,
		_T(" Density of Column (pcc)       = %.3f m^2\n"), m_drhocol);
	CTextFileOut::Write(Fout,
		_T(" Shear Wall area   (Aw)         = %.3f m^2\n"), m_dAreaWall);
	CTextFileOut::Write(Fout,
		_T(" Masonry Wall area   (Amw)      = %.3f m^2\n"), m_dAmw);
	CTextFileOut::Write(Fout,
		_T(" Density of Masonry Wall (pmw) = %.3f m^2\n"), m_drhomwall);
	CTextFileOut::Write(Fout,
		_T("\n---------------------------------------------\n"));
	CTextFileOut::Write(Fout,
		_T("\n Wall Details (%s-Direction):\n"), strDir);

	bool bHasWall = false;
	for(int i = 0; i < m_arWallData.GetSize(); ++i)
	{
		if(std::get<2>(m_arWallData[i]) > 0.0)
		{
			bHasWall = true;
			break;
		}
	}
	if(bHasWall)
	{
		CTextFileOut::Write(Fout,
			_T("\n ElemID\tLength\tDensity\t  (L/H)\t   (0.2+(L/H)^2)\n\n"));

		for(int i = 0; i < m_arWallData.GetSize(); ++i)
		{
			const auto& w = m_arWallData[i];
			double L = std::get<2>(w);
			if(L <= 0.0) continue;

			double rho = std::get<1>(w);
			double term = 0.2 + ( L / m_dH ) * ( L / m_dH );
			double ratio = m_drhocol;

			CTextFileOut::Write(Fout,
				_T("\t%d\t%.3f\t%.6f\t%.3f\t%.6f\n"),
				std::get<0>(w), L, rho, ratio, term);
		}
	}
	else
	{
		CTextFileOut::Write(Fout,
			_T("\n No shear walls present in base story level\n"));
	}
	CTextFileOut::Write(Fout,
		_T("\n---------------------------------------------\n"));
	CTextFileOut::Write(Fout, _T("\n Result...\n"));
	CTextFileOut::Write(Fout, _T("\n Approximate fundamental translational natural period Equation (IS 1893:2025) Part-5 Cl 8.2.4.3(a):\n"));
	CTextFileOut::Write(Fout,
		_T(" Ta = 0.075*H^0.75 / sqrt(1 + 0.2*pcc + sumof[pcsw{0.2+(Lcsw/H)^2}] + 0.02pmw\n\n"));
	CTextFileOut::Write(Fout,
		_T(" Fundamental Time Period   (Ta)      = %.4f sec\n"), m_dT);
	CTextFileOut::Write(Fout,
		_T("\n---------------------------------------------------------------------------------------\n"));
}