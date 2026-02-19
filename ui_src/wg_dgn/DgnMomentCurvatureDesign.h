// DgnMomentCurvatureDesign.h: interface for the CDgnMomentCurvatureDesign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNMOMENTCURVATUREDESIGN_H__EAE42201_60DE_45C8_91C3_60B3F0558B77__INCLUDED_)
#define AFX_DGNMOMENTCURVATUREDESIGN_H__EAE42201_60DE_45C8_91C3_60B3F0558B77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DT.h"

#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_MPhi_Struct.h"
#include "Dgn_SeismicManager.h"
#include "DgnDrawSection.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnMomentCurvatureDesign  
{
public:
	CDgnMomentCurvatureDesign();
	virtual ~CDgnMomentCurvatureDesign();

private:
	CMap<T_MPCC_K, T_MPCC_K, int, int>                                    m_arCalcNumPoint;
	CMap<T_MPCC_K, T_MPCC_K, double, double>                              m_arCalcAngle;
	CMap<T_MPCC_K, T_MPCC_K, DGN_CALC_MPHI_SECT, DGN_CALC_MPHI_SECT&>     m_arMPhiD;
	CMap<T_MPCC_K, T_MPCC_K, DGN_CALC_MPHI_RESULT, DGN_CALC_MPHI_RESULT&> m_arResult;
	CDBDoc* m_pDoc;
	int m_nCode_Length, m_nCode_Force;

	CDgn_SeismicManager m_DllManager;
	CDgnDrawSection     m_DrawSection;

public:
	// 해당 KeyList Moment-Curvature를 계산합니다.
	//   KeyList : 계산할 KeyList
	BOOL CalcMomentCurvature(CArray<T_MPCC_K, T_MPCC_K>& KeyList);	
	// 입력한 값으로 Moment-Curvature를 계산합니다.(현재 입력창의 값으로 계산을 수행할때)
	int CalcMomentCurvature(T_MPCC_D& MpccD, DGN_CALC_MPHI_RESULT& ResultD, int iIdx=-1);
	// 입력한 값으로 Moment-Curvature를 계산합니다
	// > MpccD : 입력값
	// < MPhiD, dAxisAngle, nNumPoint : M-Phi 계산시 사용한 DgnEngine입력값
	// < ResultD : 계산 결과
	int CalcMomentCurvature(T_MPCC_D& MpccD, DGN_CALC_MPHI_SECT& MPhiD, double& dAxisAngle, int& nNumPoint, DGN_CALC_MPHI_RESULT& ResultD);
	
	// 해당 Key의 Moment-Curvature 결과를 넘겨줍니다.
	//   Key     : 결과를 넘겨받을 정보의 Key
	// < ResultD : 결과가 있을때 M-Φ결과를 넘겨 받을 변수
	BOOL Get_MPhiResult(T_MPCC_K Key, DGN_CALC_MPHI_RESULT& ResultD);
	// M-Φ결과정보를 가지고 있는 KeyList를 넘겨줌
	int  Get_MPhiResultKeyList(CArray<T_MPCC_K, T_MPCC_K>& KeyList);
	
	// 해당 KeyList의 Report문서를 출력합니다.
	BOOL Print_MPhiResult(CArray<T_MPCC_K, T_MPCC_K>& KeyList);

	// For Civil Seismic Evaluation
	BOOL Get_EvgpMPhiResult(const int& iIdx, const T_MPCC_D& MpccD, DGN_CALC_MPHI_RESULT& ResultD);
	BOOL Print_EvgpMPhiResult(CString strFileName, CArray<T_MPCC_D, T_MPCC_D&>& arMpccD);


	// 해당 PlaceHolder에 해당 단면 삽도를 갱신함 (OnPaint시 호출해줄것)
	//   pPlaceHolder : 그림이 그려질 PlaceHolder 포인터
	//   MpccD        : 입력창의 그려질 당시의 MPCC값
	BOOL Draw_Section(CDC* pDC, CRect canvas, T_MPCC_D* pMpccD, BOOL bEMF = FALSE);
	// 해당 PlaceHolder에 해당 M-Φ곡선 삽도를 갱신함 (OnPaint시 호출해줄것)
	//   pPlaceHolder : 그림이 그려질 PlaceHolder 포인터
	//   Key          : 그림을 그릴 계산되어진 MPCC의 Key
	//   nType        : 곡선형식(0:Moment-Curvature  1:NeutralAxis-Curvature  2:Conc.Strain-Curvature  3:Steel Strain-Curvature  4:Conc.Comp.-Curvature  5:SteelComp.-Curvature  6:SteelTen.-Curvature)
	BOOL Draw_MPhiCurve(CDC* pDC, CRect canvas, DGN_CALC_MPHI_RESULT* pResultD, int nType, BOOL bEMF = FALSE);

	BOOL Make_SectionFig(CString strFileName, CRect canvas, T_MPCC_D* pMpccD);
	BOOL Make_MPhiCurveFig(CString strFileName, CRect canvas, DGN_CALC_MPHI_RESULT* pResultD, int nType);

private:
	void ConvValueTgtUnit2CurUnit(DGN_CALC_MPHI_RESULT& ResultUnitD);
	void ConvValueTgtUnit2CurUnit(DGN_CALC_MPHI_UNIT_RESULT& ResultUnitD);
	void Set_DefaultData(T_MPCC_K Key, DGN_CALC_MPHI_RESULT& ResultD);//임시파일 제거할것
};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNMOMENTCURVATUREDESIGN_H__EAE42201_60DE_45C8_91C3_60B3F0558B77__INCLUDED_)
