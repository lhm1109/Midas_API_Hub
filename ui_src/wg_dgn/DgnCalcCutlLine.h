// DgnCalcCutlLine.h: interface for the CDgnCalcCutlLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNCALCCUTLLINE_H__AF156D58_209E_445F_AE1E_6988BDBBEB2F__INCLUDED_)
#define AFX_DGNCALCCUTLLINE_H__AF156D58_209E_445F_AE1E_6988BDBBEB2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DATE : 2003.1.7. by KYE-HONG
// FUNC : 다각형의 Cutting Line과 그와 관련된 값을 보간하여 구해오는 함수 
struct T_CALC_CUT_PONT
{
	double dX,dY,dZ;
	
	void Initialize()
	{
		dX=dY=dZ=0.0;
	}
};

struct T_CALC_CUT_DATA
{
	T_CALC_CUT_PONT Pt;
	double dVal;

	void Initialize()
	{
		Pt.Initialize(); 
		dVal=0.0;
	}
};

struct T_CALC_POINT2D
{
	double dX,dY;
	void Initialize()
	{
		dX=dY=0.0; 
	}
};

const int CALCUTL_ERROR_NO_ERROR         = 0;
const int CALCUTL_ERROR_NOT_INCLUDE_LINE = 1;

const int LNPL_STATE_NOT_INCLUDE         = 0; // Polygon안에 Line이 포함되지 않음 
const int LNPL_STATE_ONE_POINT           = 1; // Polygon안에 Line중 한점만 포함
const int LNPL_STATE_TWO_POINT           = 2; // Polygon안에 Line 두점 모두 포함

class CDgnCalcCutlLine  
{
public:
	CDgnCalcCutlLine();
	virtual ~CDgnCalcCutlLine();

	void  Initialize();
	void  AddData(double x,double y,double z,double value);
	void  AddLine(double x,double y,double z); 
	BOOL  CalcCultLine(double dX[2],double dY[2],double dZ[2],double dVal[2]);
	BOOL  IsErrorData();
	BOOL  IsPolygonCut(double dX[2],double dY[2],double dZ[2],double dVal[2],BOOL bExtendedLine=FALSE);
	int   IsStateLineAndPoly(BOOL bInclude[2]);
	BOOL  CalcCutlDataExtendedLine(double dX[2],double dY[2],double dZ[2],double dVal[2]);
	BOOL  IsSame3DPoint(T_CALC_CUT_PONT &p1, T_CALC_CUT_PONT &p2);
	BOOL  IsSame3DPoint(double dX[2], double dY[2], double dZ[2] );
	BOOL  MakeIdenticalDirection(double dX[2],double dY[2],double dZ[2],double dVal[2]);
	
	BOOL  IsInside(int number_of_vertices, T_CALC_POINT2D* vertices, double point_x, double point_y);
	double Angle(double vector_x1, double vector_y1, double vector_x2, double vector_y2);
	double Norm2D(double x, double y);

    void SetLenTol(const double& dTol) { m_dLenTol = dTol; }

private:
	CArray<T_CALC_CUT_PONT,T_CALC_CUT_PONT&> m_arPoint;
	CArray<T_CALC_CUT_DATA,T_CALC_CUT_DATA&> m_arPolygon;

    double m_dLenTol;
};

#endif // !defined(AFX_DGNCALCCUTLLINE_H__AF156D58_209E_445F_AE1E_6988BDBBEB2F__INCLUDED_)
