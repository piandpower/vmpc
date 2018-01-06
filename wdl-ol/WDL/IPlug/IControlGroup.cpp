#include "IControlGroup.h"
#include "IControl.h"
#include "IPlugGUIResize.h"



IControlGroup::IControlGroup(IPlugBase * pPlug)
{
	mPlug = pPlug;

	if (mPlug->GetGUIResize())
	{
		for (int i = 0; i < mPlug->GetGUIResize()->GetViewModeSize(); i++)
		{
			originalGroupRECTs.Add(new DRECT());
		}
	}
	else
	{
		originalGroupRECTs.Add(new DRECT());
	}
}

IControlGroup::~IControlGroup()
{
	controlProps.Empty(true);
	originalGroupRECTs.Empty(true);
}

IControl* IControlGroup::AddControl(IControl* pControl)
{
	controlProps.Add(new IControlProperties(pControl, *pControl->GetDrawRECT(), *pControl->GetTargetRECT()));
	//if (moveControlRelativeToGroup) MoveControlRelativeToGroup(pControl);

	if (mPlug->GetGUIResize())
	{
		for (int i = 0; i < mPlug->GetGUIResize()->GetViewModeSize(); i++)
		{
			*originalGroupRECTs.Get(i) = originalGroupRECTs.Get(i)->Union(pControl->GetNonScaledDrawRECT());
		}
	}
	else
	{
		*originalGroupRECTs.Get(0) = originalGroupRECTs.Get(0)->Union(pControl->GetNonScaledDrawRECT());
	}

	return pControl;
}

void IControlGroup::AttachSubgroup(IControlGroup * pControlGroup)
{
	controlSubgroups.Add(pControlGroup);
	//if (moveSubgroupRelativeToGroup) MoveSubgroupRelativeToGroup(pControlGroup);
}

DRECT IControlGroup::GetGroupRECT()
{
	return groupRECT;
}

void IControlGroup::SetGroupRECT(DRECT GroupRECT)
{
	groupRECT = GroupRECT;

	MoveAllControlsRelativeToGroup();
}

void IControlGroup::HideControls(bool hideSubgroups)
{
	for (int i = 0; i < controlProps.GetSize(); i++)
	{
		IControl *pControl = controlProps.Get(i)->control;

		if (pControl->GetGUIResize() && pControl->GetGUIResize()->IsAttachedToIPlugBase()) pControl->GetGUIResize()->HideControl(pControl);
		else controlProps.Get(i)->control->Hide(true);
	}

	for (int i = 0; i < controlSubgroups.GetSize() && hideSubgroups; i++)
	{
		controlSubgroups.Get(i)->HideControls(hideSubgroups);
	}
}

void IControlGroup::ShowControls(bool showSubgroups)
{
	for (int i = 0; i < controlProps.GetSize(); i++)
	{
		IControl *pControl = controlProps.Get(i)->control;

		if (pControl->GetGUIResize() && pControl->GetGUIResize()->IsAttachedToIPlugBase()) pControl->GetGUIResize()->ShowControl(pControl);
		else controlProps.Get(i)->control->Hide(false);
	}

	for (int i = 0; i < controlSubgroups.GetSize() && showSubgroups; i++)
	{
		controlSubgroups.Get(i)->HideControls(showSubgroups);
	}
}

void IControlGroup::MoveGroup(double x, double y)
{
	double width = groupRECT.W();
	double height = groupRECT.H();

	groupRECT.L = x;
	groupRECT.T = y;
	groupRECT.R = x + width;
	groupRECT.B = y + height;

	MoveAllControlsRelativeToGroup();
}

void IControlGroup::MoveGroupHorizontally(double x)
{
	double width = groupRECT.W();

	groupRECT.L = x;
	groupRECT.R = x + width;

	MoveAllControlsRelativeToGroup();
}

void IControlGroup::MoveGroupVertically(double y)
{
	double height = groupRECT.H();

	groupRECT.T = y;
	groupRECT.B = y + height;

	MoveAllControlsRelativeToGroup();
}

void IControlGroup::MoveGroupLeftEdge(double L)
{
	groupRECT.L = L;
	MoveAllControlsRelativeToGroup();
}

void IControlGroup::MoveGroupTopEdge(double T)
{
	groupRECT.T = T;
	MoveAllControlsRelativeToGroup();
}

void IControlGroup::MoveGroupRightEdge(double R)
{
	groupRECT.R = R;
	MoveAllControlsRelativeToGroup();
}

void IControlGroup::MoveGroupBottomEdge(double B)
{
	groupRECT.B = B;
	MoveAllControlsRelativeToGroup();
}

void IControlGroup::RelativelyMoveGroup(double x, double y)
{
	groupRECT.L += x;
	groupRECT.T += y;
	groupRECT.R += x;
	groupRECT.B += y;

	MoveAllControlsRelativeToGroup();
}

void IControlGroup::RelativelyMoveGroupHorizontally(double x)
{
	groupRECT.L += x;
	groupRECT.R += x;

	MoveAllControlsRelativeToGroup();
}

void IControlGroup::RelativelyMoveGroupVertically(double y)
{
	groupRECT.T += y;
	groupRECT.B += y;

	MoveAllControlsRelativeToGroup();
}

void IControlGroup::RelativelyMoveGroupLeftEdge(double L)
{
	groupRECT.L += L;
	MoveAllControlsRelativeToGroup();
}

void IControlGroup::RelativelyMoveGroupTopEdge(double T)
{
	groupRECT.T += T;
	MoveAllControlsRelativeToGroup();
}

void IControlGroup::RelativelyMoveGroupRightEdge(double R)
{
	groupRECT.R += R;
	MoveAllControlsRelativeToGroup();
}

void IControlGroup::RelativelyMoveGroupBottomEdge(double B)
{
	groupRECT.B += B;
	MoveAllControlsRelativeToGroup();
}

int IControlGroup::GetNumberOfSubgroups()
{
	return controlSubgroups.GetSize();
}

int IControlGroup::GetNumberOfControls()
{
	return controlProps.GetSize();
}

int IControlGroup::GetNumberOfControlsIncludingSubgroups()
{
	int numberOfControls = 0;

	// Gets number of attached IControls in a group
	numberOfControls = controlProps.GetSize();

	for (int i = 0; i < controlSubgroups.GetSize(); i++)
		numberOfControls += controlSubgroups.Get(i)->GetNumberOfControls();

	return numberOfControls;
}

double IControlGroup::L() { return groupRECT.L; }

double IControlGroup::T() { return groupRECT.T; }

double IControlGroup::R() { return groupRECT.R; }

double IControlGroup::B() { return groupRECT.B; }

double IControlGroup::W() { return groupRECT.W(); }

double IControlGroup::H() { return groupRECT.H(); }

inline double IControlGroup::MW() const { return 0.5 * (groupRECT.L + groupRECT.R); }

inline double IControlGroup::MH() const { return 0.5 * (groupRECT.T + groupRECT.B); }

IControlGroup * IControlGroup::GetSubgroup(int index)
{
	return controlSubgroups.Get(index);
}

IControl * IControlGroup::GetControl(int index)
{
	return controlProps.Get(index)->control;
}

IControl * IControlGroup::GetControlIncludingSubgroups(int index)
{
	if (index < controlProps.GetSize()) return controlProps.Get(index)->control;
	else index -= controlProps.GetSize();

	int i = 0;
	for (; index >= controlSubgroups.Get(i)->GetNumberOfControls(); i++)
		index -= controlSubgroups.Get(i)->GetNumberOfControls();

	return controlSubgroups.Get(i)->GetControl(index);
}

void IControlGroup::MoveSubgroupRelativeToGroup(IControlGroup * pControlGroup)
{
	DRECT subgroupRECT = pControlGroup->GetGroupRECT();

	subgroupRECT.L += groupRECT.L;
	subgroupRECT.T += groupRECT.T;
	subgroupRECT.R += groupRECT.L;
	subgroupRECT.B += groupRECT.T;

	pControlGroup->SetGroupRECT(subgroupRECT);
}

void IControlGroup::MoveControlRelativeToGroup(IControl* pControl)
{
	if (mPlug->GetGUIResize() && mPlug->GetGUIResize()->IsAttachedToIPlugBase())
	{
		int currentViewMode = pControl->GetGUIResize()->GetViewMode();
		DRECT *originalGroupRECT = originalGroupRECTs.Get(currentViewMode);

		DRECT originalDrawRECT = pControl->GetGUIResize()->GetOriginalDrawRECTForControl(pControl);		
		pControl->GetGUIResize()->MoveControl(pControl, groupRECT.L + (originalDrawRECT.L - originalGroupRECT->L), groupRECT.T + (originalDrawRECT.T - originalGroupRECT->T), drawAreaOnly);

		DRECT originalTargetRECT = pControl->GetGUIResize()->GetOriginalTargetRECTForControl(pControl);
		pControl->GetGUIResize()->MoveControl(pControl, groupRECT.L + (originalTargetRECT.L - originalGroupRECT->L), groupRECT.T + (originalTargetRECT.T - originalGroupRECT->T), targetAreaOnly);
	}
	else
	{
		int propsIndex = FindIndexOfPropertiesForControl(pControl);
		if (propsIndex < 0) return;

		IControlProperties *props = controlProps.Get(propsIndex);

		DRECT *originalGroupRECT = originalGroupRECTs.Get(0);

		pControl->GetDrawRECT()->L = int(groupRECT.L + (props->originalDrawRECT.L - originalGroupRECT->L));
		pControl->GetDrawRECT()->T = int(groupRECT.T + (props->originalDrawRECT.T - originalGroupRECT->T));
		pControl->GetDrawRECT()->R = int(groupRECT.L + (props->originalDrawRECT.R - originalGroupRECT->L));
		pControl->GetDrawRECT()->B = int(groupRECT.T + (props->originalDrawRECT.B - originalGroupRECT->T));

		*pControl->GetNonScaledDrawRECT() = *pControl->GetDrawRECT();

		pControl->GetTargetRECT()->L = int(groupRECT.L + (props->originalTargetRECT.L - originalGroupRECT->L));
		pControl->GetTargetRECT()->T = int(groupRECT.T + (props->originalTargetRECT.T - originalGroupRECT->T));
		pControl->GetTargetRECT()->R = int(groupRECT.L + (props->originalTargetRECT.R - originalGroupRECT->L));
		pControl->GetTargetRECT()->B = int(groupRECT.T + (props->originalTargetRECT.B - originalGroupRECT->T));
	}
}

void IControlGroup::MoveAllControlsRelativeToGroup()
{
	for (int i = 0; i < controlProps.GetSize(); i++)
	{
		MoveControlRelativeToGroup(controlProps.Get(i)->control);
	}

	for (int i = 0; i < controlSubgroups.GetSize(); i++)
	{
		controlSubgroups.Get(i)->MoveAllControlsRelativeToGroup();
	}

	// Update original draw rect
	if (mPlug->GetGUIResize())
	{
		int currentViewMode = mPlug->GetGUIResize()->GetViewMode();
		DRECT *originalGroupRECT = originalGroupRECTs.Get(currentViewMode);
		*originalGroupRECT = groupRECT;
	}
	else
	{
		DRECT *originalGroupRECT = originalGroupRECTs.Get(0);
		*originalGroupRECT = groupRECT;
	}
}

int IControlGroup::FindIndexOfPropertiesForControl(IControl * pControl)
{
	for (int i = 0; i < controlProps.GetSize(); i++)
	{
		if (controlProps.Get(i)->control == pControl) return i;
	}

	return -1;
}
