// GTB_DgnRebarData.h: interface for the GTB_DgnRebarData class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTB_DGNREBARDATA_H__61D05277_B6A3_4F20_9EA9_51271CFF6EF3__INCLUDED_)
#define AFX_GTB_DGNREBARDATA_H__61D05277_B6A3_4F20_9EA9_51271CFF6EF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//임의단면에서 생성되는 철근 그리드에 들어갈 데이타
class GTB_DgnRebarData
{
protected:

public:
	CString m_strLable ; //철근 Lable
	CString m_strRebar ; //철근 Dia
	double  m_dCenterY ; //중심점 Y좌표
	double  m_dCenterZ ; //중심점 Z좌표
	 
	GTB_DgnRebarData();
	GTB_DgnRebarData(GTB_DgnRebarData& src);
	~GTB_DgnRebarData();

	

	void Initialize(){};

	GTB_DgnRebarData& operator = (const GTB_DgnRebarData& src)
	{
		m_strLable = src.m_strLable  ;
		m_strRebar = src.m_strRebar  ;
		m_dCenterY = src.m_dCenterY ;
		m_dCenterZ = src.m_dCenterZ ;
		return *this;
	}
};


#endif // !defined(AFX_GTB_DGNREBARDATA_H__61D05277_B6A3_4F20_9EA9_51271CFF6EF3__INCLUDED_)
