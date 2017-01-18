#include "OptionsDialog.h"

#include "ScheduleOptions.h"
#include "ButtonDetailsDialog.h"
#include "ProjectorDetailsDialog.h"

//(*InternalHeaders(OptionsDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(OptionsDialog)
const long OptionsDialog::ID_CHECKBOX3 = wxNewId();
const long OptionsDialog::ID_CHECKBOX2 = wxNewId();
const long OptionsDialog::ID_STATICTEXT3 = wxNewId();
const long OptionsDialog::ID_SPINCTRL1 = wxNewId();
const long OptionsDialog::ID_STATICTEXT1 = wxNewId();
const long OptionsDialog::ID_GRID1 = wxNewId();
const long OptionsDialog::ID_BUTTON4 = wxNewId();
const long OptionsDialog::ID_BUTTON8 = wxNewId();
const long OptionsDialog::ID_BUTTON3 = wxNewId();
const long OptionsDialog::ID_STATICTEXT2 = wxNewId();
const long OptionsDialog::ID_GRID2 = wxNewId();
const long OptionsDialog::ID_BUTTON5 = wxNewId();
const long OptionsDialog::ID_BUTTON6 = wxNewId();
const long OptionsDialog::ID_BUTTON7 = wxNewId();
const long OptionsDialog::ID_STATICTEXT4 = wxNewId();
const long OptionsDialog::ID_TEXTCTRL1 = wxNewId();
const long OptionsDialog::ID_BUTTON1 = wxNewId();
const long OptionsDialog::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(OptionsDialog,wxDialog)
	//(*EventTable(OptionsDialog)
	//*)
END_EVENT_TABLE()

OptionsDialog::OptionsDialog(wxWindow* parent, ScheduleOptions* options, wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    _options = options;

	//(*Initialize(OptionsDialog)
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer8;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, id, _("Options"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	CheckBox_SendOffWhenNotRunning = new wxCheckBox(this, ID_CHECKBOX3, _("Send data when not running sequence"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	CheckBox_SendOffWhenNotRunning->SetValue(false);
	FlexGridSizer1->Add(CheckBox_SendOffWhenNotRunning, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_Sync = new wxCheckBox(this, ID_CHECKBOX2, _("Use ArtNet/E1.31 Synchronisation Protocols"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	CheckBox_Sync->SetValue(false);
	FlexGridSizer1->Add(CheckBox_Sync, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer7 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Web Server Port:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer7->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl_WebServerPort = new wxSpinCtrl(this, ID_SPINCTRL1, _T("80"), wxDefaultPosition, wxDefaultSize, 0, 1, 64000, 80, _T("ID_SPINCTRL1"));
	SpinCtrl_WebServerPort->SetValue(_T("80"));
	FlexGridSizer7->Add(SpinCtrl_WebServerPort, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer7, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer3->AddGrowableCol(1);
	FlexGridSizer3->AddGrowableRow(0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Projectors:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer3->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	Grid_Projectors = new wxGrid(this, ID_GRID1, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxHSCROLL, _T("ID_GRID1"));
	Grid_Projectors->CreateGrid(0,2);
	Grid_Projectors->EnableEditing(true);
	Grid_Projectors->EnableGridLines(true);
	Grid_Projectors->SetRowLabelSize(150);
	Grid_Projectors->SetDefaultColSize(200, true);
	Grid_Projectors->SetColLabelValue(0, _("IP Address"));
	Grid_Projectors->SetColLabelValue(1, _("Password"));
	Grid_Projectors->SetDefaultCellFont( Grid_Projectors->GetFont() );
	Grid_Projectors->SetDefaultCellTextColour( Grid_Projectors->GetForegroundColour() );
	FlexGridSizer3->Add(Grid_Projectors, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
	Button_AddProjector = new wxButton(this, ID_BUTTON4, _("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
	FlexGridSizer4->Add(Button_AddProjector, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Button_ProjectorEdit = new wxButton(this, ID_BUTTON8, _("Edit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON8"));
	FlexGridSizer4->Add(Button_ProjectorEdit, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Button_DeleteProjector = new wxButton(this, ID_BUTTON3, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	FlexGridSizer4->Add(Button_DeleteProjector, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer5 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer5->AddGrowableCol(1);
	FlexGridSizer5->AddGrowableRow(0);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Buttons:    "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer5->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	Grid_Buttons = new wxGrid(this, ID_GRID2, wxDefaultPosition, wxSize(800,200), wxVSCROLL|wxHSCROLL, _T("ID_GRID2"));
	Grid_Buttons->CreateGrid(0,3);
	Grid_Buttons->EnableEditing(true);
	Grid_Buttons->EnableGridLines(true);
	Grid_Buttons->SetRowLabelSize(150);
	Grid_Buttons->SetDefaultColSize(150, true);
	Grid_Buttons->SetColLabelValue(0, _("Command"));
	Grid_Buttons->SetColLabelValue(1, _("Parameter"));
	Grid_Buttons->SetColLabelValue(2, _("Hotkey"));
	Grid_Buttons->SetDefaultCellFont( Grid_Buttons->GetFont() );
	Grid_Buttons->SetDefaultCellTextColour( Grid_Buttons->GetForegroundColour() );
	FlexGridSizer5->Add(Grid_Buttons, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer6 = new wxFlexGridSizer(0, 1, 0, 0);
	Button_ButtonAdd = new wxButton(this, ID_BUTTON5, _("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
	FlexGridSizer6->Add(Button_ButtonAdd, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Button_ButtonEdit = new wxButton(this, ID_BUTTON6, _("Edit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
	FlexGridSizer6->Add(Button_ButtonEdit, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Button_ButtonDelete = new wxButton(this, ID_BUTTON7, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
	FlexGridSizer6->Add(Button_ButtonDelete, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(FlexGridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer5, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer8 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer8->AddGrowableCol(1);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Web Directory:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer8->Add(StaticText4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrl_wwwRoot = new wxTextCtrl(this, ID_TEXTCTRL1, _("xScheduleWeb"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	FlexGridSizer8->Add(TextCtrl_wwwRoot, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(FlexGridSizer8, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	Button_Ok = new wxButton(this, ID_BUTTON1, _("Ok"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	FlexGridSizer2->Add(Button_Ok, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Button_Cancel = new wxButton(this, ID_BUTTON2, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	FlexGridSizer2->Add(Button_Cancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_GRID1,wxEVT_GRID_CELL_LEFT_CLICK,(wxObjectEventFunction)&OptionsDialog::OnGrid_ProjectorsCellLeftClick);
	Connect(ID_GRID1,wxEVT_GRID_CELL_LEFT_DCLICK,(wxObjectEventFunction)&OptionsDialog::OnGrid_ProjectorsCellLeftDClick);
	Connect(ID_GRID1,wxEVT_GRID_LABEL_LEFT_CLICK,(wxObjectEventFunction)&OptionsDialog::OnGrid_ProjectorsLabelLeftClick);
	Connect(ID_GRID1,wxEVT_GRID_LABEL_LEFT_DCLICK,(wxObjectEventFunction)&OptionsDialog::OnGrid_ProjectorsLabelLeftDClick);
	Connect(ID_GRID1,wxEVT_GRID_LABEL_RIGHT_DCLICK,(wxObjectEventFunction)&OptionsDialog::OnGrid_ProjectorsLabelRightDClick);
	Connect(ID_GRID1,wxEVT_GRID_SELECT_CELL,(wxObjectEventFunction)&OptionsDialog::OnGrid_ProjectorsCellSelect);
	Grid_Projectors->Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&OptionsDialog::OnGrid_ProjectorsKeyDown,0,this);
	Grid_Projectors->Connect(wxEVT_SIZE,(wxObjectEventFunction)&OptionsDialog::OnGrid_ProjectorsResize,0,this);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OptionsDialog::OnButton_AddProjectorClick);
	Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OptionsDialog::OnButton_ProjectorEditClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OptionsDialog::OnButton_DeleteProjectorClick);
	Connect(ID_GRID2,wxEVT_GRID_CELL_LEFT_CLICK,(wxObjectEventFunction)&OptionsDialog::OnGrid_ButtonsCellLeftClick);
	Connect(ID_GRID2,wxEVT_GRID_CELL_LEFT_DCLICK,(wxObjectEventFunction)&OptionsDialog::OnGrid_ButtonsCellLeftDClick);
	Connect(ID_GRID2,wxEVT_GRID_LABEL_LEFT_CLICK,(wxObjectEventFunction)&OptionsDialog::OnGrid_ButtonsLabelLeftClick);
	Connect(ID_GRID2,wxEVT_GRID_LABEL_LEFT_DCLICK,(wxObjectEventFunction)&OptionsDialog::OnGrid_ButtonsLabelLeftDClick);
	Connect(ID_GRID2,wxEVT_GRID_SELECT_CELL,(wxObjectEventFunction)&OptionsDialog::OnGrid_ButtonsCellSelect);
	Grid_Buttons->Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&OptionsDialog::OnGrid_ButtonsKeyDown,0,this);
	Grid_Buttons->Connect(wxEVT_SIZE,(wxObjectEventFunction)&OptionsDialog::OnGrid_ButtonsResize,0,this);
	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OptionsDialog::OnButton_ButtonAddClick);
	Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OptionsDialog::OnButton_ButtonEditClick);
	Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OptionsDialog::OnButton_ButtonDeleteClick);
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&OptionsDialog::OnTextCtrl_wwwRootText);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OptionsDialog::OnButton_OkClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OptionsDialog::OnButton_CancelClick);
	//*)

    Grid_Buttons->SetSelectionMode(wxGrid::wxGridSelectRows);
    Grid_Projectors->SetSelectionMode(wxGrid::wxGridSelectRows);

    CheckBox_SendOffWhenNotRunning->SetValue(options->IsSendOffWhenNotRunning());
    CheckBox_Sync->SetValue(options->IsSync());

    SpinCtrl_WebServerPort->SetValue(options->GetWebServerPort());

    TextCtrl_wwwRoot->SetValue(options->GetWWWRoot());

    LoadProjectors();

    LoadButtons();

    //SetSize(800, 600);

    ValidateWindow();
}

void OptionsDialog::LoadProjectors()
{
    Grid_Projectors->ClearGrid();
    auto ps = _options->GetProjectors();
    int i = 0;
    for (auto it = ps.begin(); it != ps.end(); ++it)
    {
        Grid_Projectors->AppendRows(1);
        Grid_Projectors->SetRowLabelValue(i, *it);
        Grid_Projectors->SetCellValue(i, 0, _options->GetProjectorIpAddress(*it));
        Grid_Projectors->SetCellValue(i, 1, _options->GetProjectorPassword(*it));
        i++;
    }
}

void OptionsDialog::LoadButtons()
{
    Grid_Buttons->ClearGrid();
    auto ps = _options->GetButtons();
    int i = 0;
    for (auto it = ps.begin(); it != ps.end(); ++it)
    {
        Grid_Buttons->AppendRows(1);
        Grid_Buttons->SetRowLabelValue(i, *it);
        Grid_Buttons->SetCellValue(i, 0, _options->GetButtonCommand(*it));
        Grid_Buttons->SetCellValue(i, 1, _options->GetButtonParameter(*it));
        if (_options->GetButtonHotkey(*it) == '~')
        {
            Grid_Buttons->SetCellValue(i, 2, "");
        }
        else
        {
            Grid_Buttons->SetCellValue(i, 2, _options->GetButtonHotkey(*it));
        }
        i++;
    }
}

OptionsDialog::~OptionsDialog()
{
	//(*Destroy(OptionsDialog)
	//*)
}


void OptionsDialog::OnButton_OkClick(wxCommandEvent& event)
{
    _options->SetSync(CheckBox_Sync->GetValue());
    _options->SetSendOffWhenNotRunning(CheckBox_SendOffWhenNotRunning->GetValue());
    _options->SetWebServerPort(SpinCtrl_WebServerPort->GetValue());
    _options->SetWWWRoot(TextCtrl_wwwRoot->GetValue().ToStdString());

    _options->ClearProjectors();
    for (int i = 0; i < Grid_Projectors->GetNumberRows(); i++)
    {
        _options->SetProjectorIPAddress(Grid_Projectors->GetRowLabelValue(i).ToStdString(), Grid_Projectors->GetCellValue(i, 0).ToStdString());
        _options->SetProjectorPassword(Grid_Projectors->GetRowLabelValue(i).ToStdString(), Grid_Projectors->GetCellValue(i, 1).ToStdString());
    }

    _options->ClearButtons();
    for (int i = 0; i < Grid_Buttons->GetNumberRows(); i++)
    {
        char hotkey = '~';
        if (Grid_Buttons->GetCellValue(i, 2).Length() > 0)
        {
            hotkey = Grid_Buttons->GetCellValue(i, 2)[0];
        }
        _options->SetButtonCommand(Grid_Buttons->GetRowLabelValue(i).ToStdString(), Grid_Buttons->GetCellValue(i, 0).ToStdString());
        _options->SetButtonParameter(Grid_Buttons->GetRowLabelValue(i).ToStdString(), Grid_Buttons->GetCellValue(i, 1).ToStdString());
        _options->SetButtonHotkey(Grid_Buttons->GetRowLabelValue(i).ToStdString(), hotkey);
    }

    EndDialog(wxID_OK);
}

void OptionsDialog::OnButton_CancelClick(wxCommandEvent& event)
{
    EndDialog(wxID_CANCEL);
}

void OptionsDialog::OnGrid_ProjectorsCellLeftClick(wxGridEvent& event)
{
    Grid_Projectors->SelectRow(event.GetRow());
    ValidateWindow();
}

void OptionsDialog::OnGrid_ProjectorsLabelLeftClick(wxGridEvent& event)
{
    Grid_Projectors->SelectRow(event.GetRow());
    ValidateWindow();
}

void OptionsDialog::OnGrid_ProjectorsKeyDown(wxKeyEvent& event)
{
    ValidateWindow();
}

void OptionsDialog::OnGrid_ProjectorsCellSelect(wxGridEvent& event)
{
    Grid_Projectors->SelectRow(event.GetRow());
    ValidateWindow();
}

void OptionsDialog::OnGrid_ButtonsCellLeftClick(wxGridEvent& event)
{
    Grid_Buttons->SelectRow(event.GetRow());
    ValidateWindow();
}

void OptionsDialog::OnGrid_ButtonsLabelLeftClick(wxGridEvent& event)
{
    Grid_Buttons->SelectRow(event.GetRow());
    ValidateWindow();
}

void OptionsDialog::OnGrid_ButtonsCellSelect(wxGridEvent& event)
{
    Grid_Buttons->SelectRow(event.GetRow());
    ValidateWindow();
}

void OptionsDialog::OnGrid_ButtonsKeyDown(wxKeyEvent& event)
{
    ValidateWindow();
}

void OptionsDialog::OnGrid_ButtonsResize(wxSizeEvent& event)
{
    int rhw = Grid_Buttons->GetRowLabelSize();
    Grid_Buttons->SetColSize(0, (event.GetSize().x - rhw - 66) / 2);
    Grid_Buttons->SetColSize(1, (event.GetSize().x - rhw - 66) / 2);
    Grid_Buttons->SetColSize(2, 45);

    //event.Skip();
}

void OptionsDialog::OnGrid_ProjectorsResize(wxSizeEvent& event)
{
    int rhw = Grid_Projectors->GetRowLabelSize();
    Grid_Projectors->SetColSize(0, (event.GetSize().x - rhw-1) / 2);
    Grid_Projectors->SetColSize(1, (event.GetSize().x - rhw-1) / 2);

    //event.Skip();
}

void OptionsDialog::OnButton_AddProjectorClick(wxCommandEvent& event)
{
    std::string projector = "";
    std::string ip = "";
    std::string password = "";

    Grid_Projectors->AppendRows(1);
    int row = Grid_Projectors->GetNumberRows() - 1;

    ProjectorDetailsDialog dlg(this, projector, ip, password);

    if (dlg.ShowModal() == wxID_OK)
    {
        Grid_Projectors->SetRowLabelValue(row, projector);
        Grid_Projectors->SetCellValue(row, 0, ip);
        Grid_Projectors->SetCellValue(row, 1, password);
    }
    else
    {
        Grid_Projectors->DeleteRows(row);
    }

    ValidateWindow();
}

void OptionsDialog::OnButton_ProjectorEditClick(wxCommandEvent& event)
{
    if (Grid_Projectors->GetSelectedRows().Count() != 1) return;

    int row = Grid_Projectors->GetSelectedRows()[0];

    EditProjector(row);
}

void OptionsDialog::EditProjector(int row)
{
    std::string projector = Grid_Projectors->GetRowLabelValue(row).ToStdString();
    std::string ip = Grid_Projectors->GetCellValue(row, 0).ToStdString();
    std::string password = Grid_Projectors->GetCellValue(row, 1).ToStdString();

    ProjectorDetailsDialog dlg(this, projector, ip, password);

    if (dlg.ShowModal() == wxID_OK)
    {
        Grid_Projectors->SetRowLabelValue(row, projector);
        Grid_Projectors->SetCellValue(row, 0, ip);
        Grid_Projectors->SetCellValue(row, 1, password);
    }

    ValidateWindow();
}

void OptionsDialog::OnButton_DeleteProjectorClick(wxCommandEvent& event)
{
    if (Grid_Projectors->GetSelectedRows().Count() != 1) return;

    int row = Grid_Projectors->GetSelectedRows()[0];
    Grid_Projectors->DeleteRows(row);

    ValidateWindow();
}

void OptionsDialog::OnButton_ButtonAddClick(wxCommandEvent& event)
{
    std::string label = "";
    std::string command = "";
    std::string parameter = "";
    char hotkey = '~';

    Grid_Buttons->AppendRows(1);
    int row = Grid_Buttons->GetNumberRows() - 1;

    ButtonDetailsDialog dlg(this, label, command, parameter, hotkey);

    if (dlg.ShowModal() == wxID_OK)
    {
        Grid_Buttons->SetRowLabelValue(row, label);
        Grid_Buttons->SetCellValue(row, 0, command);
        Grid_Buttons->SetCellValue(row, 1, parameter);
        Grid_Buttons->SetCellValue(row, 2, hotkey);
    }
    else
    {
        Grid_Buttons->DeleteRows(row);
    }

    ValidateWindow();
}

void OptionsDialog::OnButton_ButtonEditClick(wxCommandEvent& event)
{
    if (Grid_Buttons->GetSelectedRows().Count() != 1) return;

    int row = Grid_Buttons->GetSelectedRows()[0];

    EditButton(row);
}

void OptionsDialog::EditButton(int row)
{
    std::string label = Grid_Buttons->GetRowLabelValue(row).ToStdString();
    std::string command = Grid_Buttons->GetCellValue(row, 0).ToStdString();
    std::string parameter = Grid_Buttons->GetCellValue(row, 1).ToStdString();
    char hotkey = '~';
    if (Grid_Buttons->GetCellValue(row, 2).Length() > 0)
    {
        hotkey = Grid_Buttons->GetCellValue(row, 2)[0];
    }

    ButtonDetailsDialog dlg(this, label, command, parameter, hotkey);

    if (dlg.ShowModal() == wxID_OK)
    {
        Grid_Buttons->SetRowLabelValue(row, label);
        Grid_Buttons->SetCellValue(row, 0, command);
        Grid_Buttons->SetCellValue(row, 1, parameter);
        Grid_Buttons->SetCellValue(row, 2, hotkey);
    }

    ValidateWindow();
}

void OptionsDialog::OnButton_ButtonDeleteClick(wxCommandEvent& event)
{
    if (Grid_Buttons->GetSelectedRows().Count() != 1) return;

    int row = Grid_Buttons->GetSelectedRows()[0];
    Grid_Buttons->DeleteRows(row);

    ValidateWindow();
}

void OptionsDialog::ValidateWindow()
{
    if (Grid_Buttons->GetSelectedRows().Count() == 1)
    {
        Button_ButtonDelete->Enable(true);
        Button_ButtonEdit->Enable(true);
    }
    else
    {
        Button_ButtonDelete->Enable(false);
        Button_ButtonEdit->Enable(false);
    }

    if (Grid_Projectors->GetSelectedRows().Count() == 1)
    {
        Button_DeleteProjector->Enable(true);
        Button_ProjectorEdit->Enable(true);
    }
    else
    {
        Button_DeleteProjector->Enable(false);
        Button_ProjectorEdit->Enable(false);
    }

    if (TextCtrl_wwwRoot->GetValue() == "")
    {
        Button_Ok->Enable(false);
    }
    else
    {
        Button_Ok->Enable();
    }
}
void OptionsDialog::OnTextCtrl_wwwRootText(wxCommandEvent& event)
{
    ValidateWindow();
}

void OptionsDialog::OnGrid_ButtonsCellLeftDClick(wxGridEvent& event)
{
    EditButton(event.GetRow());
}

void OptionsDialog::OnGrid_ButtonsLabelLeftDClick(wxGridEvent& event)
{
    EditButton(event.GetRow());
}

void OptionsDialog::OnGrid_ProjectorsLabelRightDClick(wxGridEvent& event)
{
}

void OptionsDialog::OnGrid_ProjectorsCellLeftDClick(wxGridEvent& event)
{
    EditProjector(event.GetRow());
}

void OptionsDialog::OnGrid_ProjectorsLabelLeftDClick(wxGridEvent& event)
{
    EditProjector(event.GetRow());
}
