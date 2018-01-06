#ifndef _ICONTROLGROUP_
#define _ICONTROLGROUP_

#include "IPlugStructs.h"

class IPlugBase;
class IControl;

class IControlGroup
{
public:
	IControlGroup(IPlugBase *pPlug);
	~IControlGroup();

	IControl* AddControl(IControl* pControl);
	void AttachSubgroup(IControlGroup* pControlGroup);
	
	DRECT GetGroupRECT();
	void SetGroupRECT(DRECT GroupRECT);

	IControlGroup* GetSubgroup(int index);
	IControl* GetControl(int index);
	IControl* GetControlIncludingSubgroups(int index);

	void HideControls(bool hideSubgroups = false);
	void ShowControls(bool showSubgroups = false);

	void MoveGroup(double x, double y);
	void MoveGroupHorizontally(double x);
	void MoveGroupVertically(double y);

	void MoveGroupLeftEdge(double L);
	void MoveGroupTopEdge(double T);
	void MoveGroupRightEdge(double R);
	void MoveGroupBottomEdge(double B);

	void RelativelyMoveGroup(double x, double y);
	void RelativelyMoveGroupHorizontally(double x);
	void RelativelyMoveGroupVertically(double y);

	void RelativelyMoveGroupLeftEdge(double L);
	void RelativelyMoveGroupTopEdge(double T);
	void RelativelyMoveGroupRightEdge(double R);
	void RelativelyMoveGroupBottomEdge(double B);

	int GetNumberOfSubgroups();
	int GetNumberOfControls();
	int GetNumberOfControlsIncludingSubgroups();
	
	double L();
	double T();
	double R();
	double B();
	double W();
	double H();
	inline double MW() const;
	inline double MH() const;

private:
	void MoveSubgroupRelativeToGroup(IControlGroup* pControlGroup);
	void MoveControlRelativeToGroup(IControl* pControl);
	void MoveAllControlsRelativeToGroup();
	int FindIndexOfPropertiesForControl(IControl* pControl);

	struct IControlProperties
	{
		IControlProperties(IControl *pControl, IRECT drawRECT, IRECT targetRECT)
		{
			control = pControl;
			originalDrawRECT = drawRECT;
			originalTargetRECT = targetRECT;
		}

		IControl *control;
		IRECT originalDrawRECT;
		IRECT originalTargetRECT;
	};

	IPlugBase *mPlug;
	DRECT groupRECT = DRECT(999999, 999999, 0, 0);
	WDL_PtrList<IControlGroup> controlSubgroups;
	WDL_PtrList<IControlProperties> controlProps;
	WDL_PtrList<DRECT> originalGroupRECTs;
	bool moveControlRelativeToGroup = false;
	bool moveSubgroupRelativeToGroup = false;
};
#endif