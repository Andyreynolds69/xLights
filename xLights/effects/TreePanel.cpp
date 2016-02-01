#include "TreePanel.h"
#include "../../include/padlock16x16-blue.xpm"
#include "EffectPanelUtils.h"

//(*InternalHeaders(TreePanel)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/bitmap.h>
#include <wx/slider.h>
#include <wx/bmpbuttn.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//(*IdInit(TreePanel)
const long TreePanel::ID_STATICTEXT87 = wxNewId();
const long TreePanel::ID_SLIDER_Tree_Branches = wxNewId();
const long TreePanel::IDD_TEXTCTRL_Tree_Branches = wxNewId();
const long TreePanel::ID_BITMAPBUTTON_SLIDER_Tree_Branches = wxNewId();
const long TreePanel::ID_SLIDER_Tree_Speed = wxNewId();
const long TreePanel::IDD_TEXTCTRL_Tree_Speed = wxNewId();
//*)

BEGIN_EVENT_TABLE(TreePanel,wxPanel)
	//(*EventTable(TreePanel)
	//*)
END_EVENT_TABLE()

TreePanel::TreePanel(wxWindow* parent)
{
	//(*Initialize(TreePanel)
	wxFlexGridSizer* FlexGridSizer71;
	wxTextCtrl* TextCtrl59;
	wxStaticText* StaticText182;
	wxSlider* Slider22;
	wxTextCtrl* TextCtrl60;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer71 = new wxFlexGridSizer(5, 4, 0, 0);
	FlexGridSizer71->AddGrowableCol(1);
	StaticText86 = new wxStaticText(this, ID_STATICTEXT87, _("Number Branches"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT87"));
	FlexGridSizer71->Add(StaticText86, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Tree_Branches = new wxSlider(this, ID_SLIDER_Tree_Branches, 3, 1, 10, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Tree_Branches"));
	FlexGridSizer71->Add(Slider_Tree_Branches, 1, wxALL|wxEXPAND, 5);
	TextCtrl59 = new wxTextCtrl(this, IDD_TEXTCTRL_Tree_Branches, _("3"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Tree_Branches"));
	TextCtrl59->SetMaxLength(3);
	FlexGridSizer71->Add(TextCtrl59, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_TreeBranches = new wxBitmapButton(this, ID_BITMAPBUTTON_SLIDER_Tree_Branches, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_Tree_Branches"));
	BitmapButton_TreeBranches->SetDefault();
	FlexGridSizer71->Add(BitmapButton_TreeBranches, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText182 = new wxStaticText(this, wxID_ANY, _("Speed"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer71->Add(StaticText182, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Slider22 = new wxSlider(this, ID_SLIDER_Tree_Speed, 10, 1, 50, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Tree_Speed"));
	FlexGridSizer71->Add(Slider22, 1, wxALL|wxEXPAND, 5);
	TextCtrl60 = new wxTextCtrl(this, IDD_TEXTCTRL_Tree_Speed, _("10"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Tree_Speed"));
	TextCtrl60->SetMaxLength(3);
	FlexGridSizer71->Add(TextCtrl60, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer71);
	FlexGridSizer71->Fit(this);
	FlexGridSizer71->SetSizeHints(this);

	Connect(ID_SLIDER_Tree_Branches,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&TreePanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_Tree_Branches,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&TreePanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_Tree_Branches,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TreePanel::OnLockButtonClick);
	Connect(ID_SLIDER_Tree_Speed,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&TreePanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_Tree_Speed,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&TreePanel::UpdateLinkedSlider);
	//*)
    SetName("ID_PANEL_TREE");
}

TreePanel::~TreePanel()
{
	//(*Destroy(TreePanel)
	//*)
}

PANEL_EVENT_HANDLERS(TreePanel)