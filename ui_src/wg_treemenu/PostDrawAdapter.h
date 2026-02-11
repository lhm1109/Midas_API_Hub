// PostDrawAdapter.h: interface for the CPostDrawAdapter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POSTDRAWADAPTER_H__C968B8C6_94FC_4E5A_891B_764F56A6D25A__INCLUDED_)
#define AFX_POSTDRAWADAPTER_H__C968B8C6_94FC_4E5A_891B_764F56A6D25A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

 #include "..\wg_db\DB_ST_DT.h"
 #include "..\wg_db\DB_ST_DN_RESULT_PSC.h"
 #include "..\wg_db\DB_ST_POST_READ.h"
 #include "..\wg_db\DB_ST_DN_RESULT_CH.H"

#include <map>
using namespace std;

class CPostDrawAdapter  
{
public:
	CPostDrawAdapter(const CString& _docpath = _T(""));
	virtual ~CPostDrawAdapter();

public:
	CString Build_CHRC_CBCS_Graphic(const std::map<T_ELEM_K,T_CBCS_CH>& mapdatas);
	CString Build_PSC_BDCY_Graphic(const std::map<T_ELEM_K,T_BDCY_JTG>& mapdatas);

	CString Build_CHRC_CBCS_Table(const std::map<T_ELEM_K,T_CBCS_CH>& mapdatas);
	CString Build_PSC_BDCY_Table(const std::map<T_ELEM_K,T_BDCY_JTG>& mapdatas);


	CString Build_PSC_CUMS_Graphic(const std::map<T_ELEM_K,T_CUMS_JTG>& mapdatas);
	CString Build_CHRC_CSIS_Graphic(const std::map<T_ELEM_K,T_CSIS_CH>& mapdatas);

	CString Build_PSC_CUMS_Table(const std::map<T_ELEM_K,T_CUMS_JTG>& mapdatas);
	CString Build_CHRC_CSIS_Table(const std::map<T_ELEM_K,T_CSIS_CH>& mapdatas);


	CString Build_PSC_CRMT_Graphic(const std::map<T_ELEM_K,T_CRMT_JTG>& mapdatas);
	CString Build_PSC_CRMT_Graphic2(const std::map<T_ELEM_K,T_CRMT_JTG>& mapdatas);

	CString Build_CHRC_CTSS_Graphic(const std::map<T_ELEM_K,T_CTSS_CH>& mapdatas);

	CString Build_PSC_CRMT_Table(const std::map<T_ELEM_K,T_CRMT_JTG>& mapdatas);
	CString Build_CHRC_CTSS_Table(const std::map<T_ELEM_K,T_CTSS_CH>& mapdatas);


	//iBrgtype -0All 1Ashort 2ALong  3 Normal Compress(JTG d62 2004 7.1.5-1)
	CString Build_PSC_FYCM_Graphic(int iBrgtype , const std::map<T_ELEM_K,T_FYCM_JTG>& mapdatas);

	CString Build_PSC_FYCM_Table(int iBrgtype , const std::map<T_ELEM_K,T_FYCM_JTG>& mapdatas);
	CString Build_PSC_FYPC_Table(const std::map<T_TDNA_K,T_FYPC_JTG>& mapdatas);


	//CString Build_CHRC_CTSS_Graphic(const std::map<T_ELEM_K,T_CTSS_CH>& mapdatas);
	CString Build_PSC_FPMS_Graphic(int iBrgtype , const std::map<T_ELEM_K,T_FPMS_JTG>& mapdatas);
	CString Build_PSC_FPMS_Table(int iBrgtype , const std::map<T_ELEM_K,T_FPMS_JTG>& mapdatas);

	CString Build_PSC_FYSG_Graphic(const std::map<T_ELEM_K,T_FYSG_D>& mapdatas);
	CString Build_PSC_FYSG_Table(const std::map<T_ELEM_K,T_FYSG_D>& mapdatas);

	CString Build_Bearing_Reac_Table(CArray<T_NODE_K, T_NODE_K>& ValueNodeKeyList,CArray<T_REAC_D, T_REAC_D&>& ReacDataList);
	CString Build_Bearing_Force_Graphic(const CArray<T_ELEM_K, T_ELEM_K>& ValueElemKeyList,const CArray<T_STRB_D, T_STRB_D&>& ForceDataList,const CString& loadcombname, int iForceType = 5 );

	CString GetMaxMinTypeShear(int iMaxType);
	CString GetMaxMinType(int iMaxType);
	CString GetTopBottomType(int iLocType);
	CString GetLoadCombType(int iMaxType);
	CString GetLoadCombName(T_LCOM_K lcomK);
private:
	CString m_DocPath;
};

#endif // !defined(AFX_POSTDRAWADAPTER_H__C968B8C6_94FC_4E5A_891B_764F56A6D25A__INCLUDED_)
