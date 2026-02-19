// DgnSectionTool.h: interface for the CDgnSectionTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNSECTIONTOOL_H__47DFA747_FF8E_46BD_925E_403D7C4F4F8D__INCLUDED_)
#define AFX_DGNSECTIONTOOL_H__47DFA747_FF8E_46BD_925E_403D7C4F4F8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\DbToolStruct.h"
#include "..\wg_db\DBDoc.h"

#include "..\wg_cmd\CMSecViewWnd.h"

#include "DgnStruct.h"

struct _DGN_POINT2D
{
	double dyz[2];  // y,z.
	void Initialize()
	{
		dyz[0]=0.0; dyz[1]=0.0;    
	}
};

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSectionTool  
{
protected:
	CDBDoc* m_pDoc;
public:
	CDgnSectionTool();
	virtual ~CDgnSectionTool();  

	// 해당 Section ID및 Position ID를 이용하여 도심을 원점으로 하는 단면의 좌표값을 넘겨줌(Y-Z평면의 값)
	//   SectD     : Section 정보를 가지는 구조체 
	//   iPosi     : Position ID (0=i단 , 1(또는 0이 아닌수)=j단)
	//   arViewGeom: 좌표값을 넘겨 받을 변수(배열의 첫번째 값은 외곽좌표, 이후는 내부 좌표들임, Y-Z평면의 값을 나타내는데 Y-Z값을 각각 X,Y에 입력하여 넘겨줌) 
	//   arPos     : arViewGeom의 단면의 기본 수치를 넘겨줌(Size=6 : [0]=OffsetX, [1]=MaxY(high), [2]=MinY(low), [3]=MinX(left), [4]=MaxX(right), [5]=OffsetY) 
	//   dOffsetY  : 도심을 기준으로 Y좌표값을 Offset시킬 양 
	//   dOffsetZ  : 도심을 기준으로 Z좌표값을 Offset시킬 양 
	// ※추가항목  : 각 DlgView_Object항목내에 Dlg에 표시될 색정보및 채우기 정보도 함께 입력해서 넘겨주고있음
	BOOL Get_SectionPolygonData_DlgView_ObjectArray(T_SECT_D& SectD, int iPosi, CArray<DlgView_Object*,DlgView_Object*> & arViewGeom, CArray<double, double&>& arPos, double dOffsetX=0.0, double dOffsetY=0.0);

	//SHIN ('06.02.27) 임시 DB변경시 수정요망
	// T_REBT_D 형식의 정보를 T_RBTE_D형식으로 변경시켜줌
	//   InData  : 변경할 T_REBT_D 정보 
	//   OutData : 변경된 T_RBTE_D 정보를 넘겨 받을 변수
	static BOOL Cng_From_REBT_To_RBTE(T_REBT_D InData, T_RBTE_D & OutData);
	// T_REBT_D 형식의 정보를 T_RBTE_D형식으로 변경시켜줌
	//   InData  : 변경할 T_RBTE_D 정보 
	//   OutData : 변경된 T_REBT_D 정보를 넘겨 받을 변수
	static BOOL Cng_From_RBTE_To_REBT(T_RBTE_D InData, T_REBT_D & OutData);
	// 해당 임의 단면 요소의 T_RBTE_D값을 넘겨줌
	//   iElem : 요소 번호
	//   RbteD : T_RBTE_D를 넘겨 받을 변수(요소에 없을 경우 Sect에 딸린 값을 변환하여 넘겨줌)
	//   RETURN: 자료가 없을 경우 FALSE를 넘김
	static BOOL GetRbte(int iElem, T_RBTE_D& RbteD);


	// Plate 철근 정보 /////////////////////////////////////////////////////////////// Add by SHIN(07.07.01) MNET:2668

	//설계영역의 한면에 대한 철근정보를 넘겨받음	
	static BOOL Get_DgncCpplData(T_RCPL_K RcplKey, _DGNC_CPPL_D& DgncCpplD);       	        
	//여러 설계영역의 한면에 대한 철근정보를 넘겨받음	
	static BOOL Get_DgncCpplDataList(CArray<T_RCPL_K, T_RCPL_K>& arRcplKey, CArray<_DGNC_CPPL_D, _DGNC_CPPL_D&>& arDgncCpplD);    
	
	static BOOL Get_DgncCpplSingleData(T_RCPL_K RcplKey, BOOL bTop, _DGNC_CPPL_SINGLE_D& DgncCpplSingleD);       	        
	static BOOL Get_DgncCpplSingleDataList(CArray<T_RCPL_K, T_RCPL_K>& arRcplKey, CArray<BOOL, BOOL>& arbTop, CArray<_DGNC_CPPL_SINGLE_D, _DGNC_CPPL_SINGLE_D&>& arDgncCpplSingleD);    	
};
#include "HeaderPost.h"

#endif // !defined(AFX_DGNSECTIONTOOL_H__47DFA747_FF8E_46BD_925E_403D7C4F4F8D__INCLUDED_)
