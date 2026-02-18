// Dgn_SeismicManager.h: interface for the CDgn_SeismicManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGN_SEISMICMANAGER_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_)
#define AFX_DGN_SEISMICMANAGER_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\dgnengine\src\DgnBase\DgnBaseShape_Struct.h"
#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_MPhi_Struct.h"
#include "..\dgnengine\src\DgnReportBase\DgnReportBase_MPhi_Struct.h"
#include "..\dgnengine\idesign\DGN_lib\DgnMPhiUtil.h"

#include "Dgn_DllManager_Base.h"

#include <tuple>
#include <map>

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgn_SeismicManager : public CDgn_DllManager_Base       
{
public:
	CDgn_SeismicManager();
	virtual ~CDgn_SeismicManager();
public:
	static bool Parallel_Calc_MPhiCurve();
	static bool Parallel_Calc_MPhiCurve(void *pProgDlg);
	int LCalc_MPhiCurve(double dAngle, int nPointNum, DGN_CALC_MPHI_SECT& InData, DGN_CALC_MPHI_RESULT& OutData);
	int GCalc_MPhiCurve(double dAngle, int nPointNum, DGN_CALC_MPHI_SECT& InData, DGN_CALC_MPHI_RESULT& OutData);	
	BOOL GCalc_ConfinedConcretePolygonData_Column(DGN_GSEC_SHAPE_D& rSection, DGN_GSEC_REBAR_COLUMN& rRebar, DGN_GSEC_POLYGON_LIST& arOutConcPolyData, DGN_GSEC_POLYGON_LIST& arInConcPolyData, double dOffsetX, double dOffsetY);
	
	BOOL GPrint_MPhiCurveReport(DGN_REPORT_OPTION PrintOption, CArray<DGN_REPORT_MPHI_RESULT, DGN_REPORT_MPHI_RESULT&>& arResult);

public:
	BOOL ConvertMpcc_ToEngine(const T_MPCC_D& MpccD, const T_ECUC_D& EcucD, int nCode_Length, int nCode_Force, DGN_CALC_MPHI_SECT& rData, double& dAxisAngle, int& nNumPoint, BOOL bCreateRebarDB=FALSE);	
	BOOL ConvertFimp_ToEngine(const T_FIMP_D& InData, DGN_CALC_FIMP_D& rData);

	BOOL ConvertResult_ToReportDLL(DGN_CALC_MPHI_SECT& InData, DGN_CALC_MPHI_RESULT& InRes, CString strSectFigFileName, CString strChartFigFileName, DGN_REPORT_MPHI_RESULT& rData);
	BOOL ConvertResult_ToReportDLL(DGN_CALC_MPHI_UNIT_RESULT& InData, DGN_REPORT_MPHI_UNIT_RESULT& rData);

private:
	bool ConvertFimp_ToEngine(const T_FIMP_CON_KENTPK& inKENTPK, OUT DGN_CALC_FIMP_CON_KENTPK& rKENTPK);
	bool ConvertFimp_ToEngine(const T_FIMP_CON_STANJP& inSTANJP, OUT DGN_CALC_FIMP_CON_STANJP& rSTANJP);
	bool ConvertFimp_ToEngine(const T_FIMP_CON_ROADJP& inROADJP, OUT DGN_CALC_FIMP_CON_ROADJP& rROADJP);
	bool ConvertFimp_ToEngine(const T_FIMP_CON_NAGOYA& inNAGOYA, OUT DGN_CALC_FIMP_CON_NAGOYA& rNAGOYA);
	bool ConvertFimp_ToEngine(const T_FIMP_CON_TRILIN& inTRILIN, OUT DGN_CALC_FIMP_CON_TRILIN& rTRILIN);
	bool ConvertFimp_ToEngine(const T_FIMP_CON_GB1002& inGB1002, OUT DGN_CALC_FIMP_CON_GB1002& rGB1002);
	bool ConvertFimp_ToEngine(const T_FIMP_CON_MANDER& inMANDER, OUT DGN_CALC_FIMP_CON_MANDER& rMANDER);
	bool ConvertFimp_ToEngine(const T_FIMP_CON_GB1010& inGB1010, OUT DGN_CALC_FIMP_CON_GB1010& rGB1010);
	
	bool ConvertFimp_ToEngine(const T_FIMP_STL_MENEGO& inMENEGO, OUT DGN_CALC_FIMP_STL_MENEGO& rMENEGO);
	bool ConvertFimp_ToEngine(const T_FIMP_STL_BILINE& inBILINE, OUT DGN_CALC_FIMP_STL_BILINE& rBILINE);
	bool ConvertFimp_ToEngine(const T_FIMP_STL_GENBIL& inGENBIL, OUT DGN_CALC_FIMP_STL_GENBIL& rGENBIL);
	bool ConvertFimp_ToEngine(const T_FIMP_STL_TRILIN& inTRILIN, OUT DGN_CALC_FIMP_STL_TRILIN& rTRILIN);
	bool ConvertFimp_ToEngine(const T_FIMP_STL_PARK& inPARK, OUT DGN_CALC_FIMP_STL_PAKSHD& rPAKSHD);
	bool ConvertFimp_ToEngine(const T_FIMP_STL_ROADJP& inROADJP, OUT DGN_CALC_FIMP_STL_ROADJP& rROADJP);
	bool ConvertFimp_ToEngine(const T_FIMP_STL_GB2010& inGB2010, OUT DGN_CALC_FIMP_STL_GB1010& rGB1010);
};

typedef struct _stMPHIData
{
	double dAngle;
	int nPointNum;
	DGN_CALC_MPHI_SECT   InD;
	int nError;
	DGN_CALC_MPHI_RESULT ResD;

	_stMPHIData()
	{
		dAngle = 0.0;
		nPointNum = 50;
		nError = 0;
		InD.Initialize();
		ResD.Initialize();
	}
	_stMPHIData(const DGN_CALC_MPHI_SECT& InData)
	{
		dAngle = 0.0;
		nPointNum = 50;
		nError = 0;
		InD = InData;
		ResD.Initialize();
	}

} STMPhiData;

typedef std::tuple<UINT, bool, UINT, UINT, UINT> tupleKey; /// order : ElemK (or unInternalWallID), bWall, LcomK, PosiK, SignK.
class __MY_EXT_CLASS__ CDgn_SeismicDataPool
{
public:
	static CDgn_SeismicDataPool* Instance(){
		static CDgn_SeismicDataPool* pInst = new CDgn_SeismicDataPool();
		return pInst;
	}

	void InitPool();
	int  AddInputData(const UINT& ElemK, const bool bWall, const UINT& LcomK, const UINT& PosiK, const UINT& SignK, const double& dAngel, const int& nPointNum, const DGN_CALC_MPHI_SECT& InData);
	int  AddResult(const UINT& ElemK, const bool bWall, const UINT& LcomK, const UINT& PosiK, const UINT& SignK, const int&nError, const DGN_CALC_MPHI_RESULT& ResData);
	bool GetResult(const UINT& ElemK, const bool bWall, const UINT& LcomK, const UINT& PosiK, const UINT& SignK, int& nError, DGN_CALC_MPHI_RESULT& ResData);
	void ClearResult(const UINT& ElemK, const bool bWall);
	int  GetDataVector(std::vector<dgn::lib::T_DGN_MPHI_D>& vecData);

private:
	CDgn_SeismicDataPool() { InitPool(); };

	// ElemK; Element Key
	// LcomK; Design Load combination No.
	// PosiK; Design Position No.
	// SignK; Beam-pos/neg, Column,wall-major/minor
	int  GetSerialIndex(const UINT& ElemK, const bool bWall, const UINT& LcomK, const UINT& PosiK, const UINT& SignK);
	void SetSerialIndex(const UINT& ElemK, const bool bWall, const UINT& LcomK, const UINT& PosiK, const UINT& SignK, const UINT& iIdx);

	std::unordered_map<tupleKey, int, tuple_hash> m_mapIndex;
	std::vector<STMPhiData> m_vecData;
};

#include "HeaderPost.h"

#endif // !defined(AFX_DGN_SEISMICMANAGER_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_)
