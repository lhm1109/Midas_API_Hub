#pragma once

#include "HeaderPre.h"

class CDBDoc;
class __MY_EXT_CLASS__ CDgnLengthTool final
{
public:
	CDgnLengthTool();
	virtual ~CDgnLengthTool();

public:
	double Get_2DMembLeng(T_SBDO_K SbdoK, int nLengthUnit, CMapEx<int, int, double, double>* pMap = nullptr);

private:
	void   Get_ProjectionFactor2XYPlane(T_ELEM_K ElemK, double dAxisVec[], double& dRotAngle);
	void   Get_ProjectionNode2XYPlane(double dRotAngle, double dAxisVec[], const T_NODE_D& OrgNode, T_NODE_D& TrsNode);

private:
	CDBDoc* m_pDoc;
};

#include "HeaderPost.h"
