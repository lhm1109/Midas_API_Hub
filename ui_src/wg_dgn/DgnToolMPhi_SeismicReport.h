// DgnToolMPhi_SeismicReport.h: interface for the CDgnToolMPhi_SeismicReport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNTOOLMPHI_SEISMICREPORT_H__E9BC612D_29BB_4801_9D68_CF65AF203552__INCLUDED_)
#define AFX_DGNTOOLMPHI_SEISMICREPORT_H__E9BC612D_29BB_4801_9D68_CF65AF203552__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnSeismicDesignStruct.h"
#include "..\wg_base\MSWord.h"
#include "DgnProgressDlg.h"

#define FONT_SIZE_BASE            10  // 기본
#define FONT_SIZE_COVER_TITLE1    28  // 표지 Title
#define FONT_SIZE_COVER_TITLE2    24  // 표지 Title2
#define FONT_SIZE_MAIN_TITLE      18  // 날짜
#define FONT_SIZE_TABLE_TITLE     20  // 목차
#define FONT_SIZE_SUB_TITLE       14  // Chapter
#define FONT_SIZE_CHAPTER_TITLE1  12  // Chapter1 (목차)
#define FONT_SIZE_CHAPTER_TITLE2  10  // Chapter2 (목차)
#define FONT_SIZE_TABLE_TEXT      8   // 표
#define FONT_SIZE_SMALL_TEXT      8   // 각주 or 캡션

class CUnitCtrl;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnToolMPhi_SeismicReport  
{
public:
	CDgnToolMPhi_SeismicReport();
	virtual ~CDgnToolMPhi_SeismicReport();

public:
	void PrintReport(CString strPath, CArray<BOOL, BOOL>& arChk, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, CDgnProgressDlg* pProDlg);
	void WriteReport();
	void WriteReport(CString strFilePath, _DGN_RESERVE_LATERAL_CAPA_RES& ResData);

protected:
	CWordBase*  m_pWord;
	
	int m_nSub1Num; // 1.
	int m_nSub2Num; // 1.1
	int m_nSub3Num; // 1.1.1
	int m_nSub4Num; // (1)

	CUnitCtrl* m_pUnitCtrl;

	//
	CDgnProgressDlg* m_pProDlg;
	int              m_nProTotal;
	int              m_nProCurr;

protected:
	void WriteSub1Title  (CString strTitle, CWordBase* pWord, BOOL bBold=TRUE);
	void WriteSub2Title  (CString strTitle, CWordBase* pWord, BOOL bTab=TRUE, BOOL bBold=FALSE);
	void WriteSub3Title  (CString strTitle, CWordBase* pWord, BOOL bNewline=TRUE, BOOL bTab=TRUE, BOOL bBold=FALSE);
	void WriteSub4Title  (CString strTitle, CWordBase* pWord, BOOL bTab=FALSE, BOOL bBold=FALSE);

	void WriteChap1Title (CString strTitle, CWordBase* pWord);
	void WriteChap2Title (CString strTitle, CWordBase* pWord);
	void WriteChap3Title (CString strTitle, CWordBase* pWord);

	void WriteReportCover(CWordBase* pWord);  // 표시
	void WriteReportTable(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData);  // 목차
	BOOL WriteReportContents(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData); // 내용 (본문)

	// 설계조건 Table 
	void WriteDesignConditionTable(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData); 
	// 모델의 제원 Table 
	void WriteModelConditionTable(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData); 
	
	// 교축(직각)방향에 대한 안정검토 결과 Table (타입 I, 타입 II 모두 포함)
	void WriteExamResultTable1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx); 
	// 교축(직각)방향에 대한 안정검토 결과 Table (타입 I, 타입 II 둘 중 하나 포함)
	void WriteExamResultTable2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); 

	// 교축(직각)방향에 대한 대상구조물 Table (교축방향 or 교축직각방향)
	void WriteStructureTable1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx); 
	// 교축방향, 교축직각방향에 대한 대상구조물 Table (교축방향 and 교축직각방향)
	void WriteStructureTable2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nPierIdx); 

	// 교축(직각)방향에 대한 재료조건 Table (교축방향 or 교축직각방향)
	void WriteMaterialConditionTable1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx); 
	// 교축방향, 교축직각방향에 대한 재료조건 Table (교축방향 and 교축직각방향)
	void WriteMaterialConditionTable2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nPierIdx); 

	// 수평내력 및 수평변위의 산출
	void WriteHorizontalForce(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); 
	void WriteHorizontalForce1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 사하중시의 수평내력과 수평변위의 관계
	void WriteHorizontalForce2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 균열시의 수평내력과 수평변위의 관계
	void WriteHorizontalForce3(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 초기항복시의 수평내력과 수평변위의 관계
	void WriteHorizontalForce4(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 항복시의 수평내력과 수평변위의 관계
	void WriteHorizontalForce5(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 종극시의 수평내력과 수평변위의 관계
	void WriteHorizontalForce6(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 콘크리트의 응력도 변형률관계

	void WriteShearForce(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 전단내력의 산출
	void WriteFailureType(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 파괴형태의 판정

	// 지진시보유수평내력의 조사
	void WriteEarthquakeExam(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); 
	void WriteEarthquakeExam1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 설계수평진도
	void WriteEarthquakeExam2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 등가중량
	void WriteEarthquakeExam3(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 구조물특성보정계수
	void WriteEarthquakeExam4(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 허용소성률
	void WriteEarthquakeExam5(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 지진시보유수평내력
	void WriteEarthquakeExam6(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 지진시보유수평내력의 조사

	// 잔류변위의 조사
	void WriteRemainingDispExam(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); 
	void WriteRemainingDispExam1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 교각의 응답소성률
	void WriteRemainingDispExam2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 잔류변위
	void WriteRemainingDispExam3(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType); // 잔류변위의 산정
 
};
#include "HeaderPost.h"

#endif // !defined(AFX_DGNTOOLMPHI_SEISMICREPORT_H__E9BC612D_29BB_4801_9D68_CF65AF203552__INCLUDED_)
