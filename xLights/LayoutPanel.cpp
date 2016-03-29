#include "LayoutPanel.h"

//(*InternalHeaders(LayoutPanel)
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/splitter.h>
#include <wx/intl.h>
#include <wx/stattext.h>
#include <wx/choice.h>
//*)

#include <wx/clipbrd.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/tglbtn.h>
#include <wx/sstream.h>

#include "ModelPreview.h"
#include "xLightsMain.h"
#include "DrawGLUtils.h"
#include "ModelDialog.h"
#include "ChannelLayoutDialog.h"
#include "models/ModelGroup.h"


#include "models/ModelImages.h"


//(*IdInit(LayoutPanel)
const long LayoutPanel::ID_CHOICE1 = wxNewId();
const long LayoutPanel::ID_BUTTON_SELECT_MODEL_GROUPS = wxNewId();
const long LayoutPanel::ID_LISTBOX_ELEMENT_LIST = wxNewId();
const long LayoutPanel::ID_PANEL2 = wxNewId();
const long LayoutPanel::ID_SPLITTERWINDOW1 = wxNewId();
const long LayoutPanel::ID_CHECKBOXOVERLAP = wxNewId();
const long LayoutPanel::ID_BUTTON_MODELS_PREVIEW = wxNewId();
const long LayoutPanel::ID_BUTTON_SAVE_PREVIEW = wxNewId();
const long LayoutPanel::ID_PANEL5 = wxNewId();
const long LayoutPanel::ID_PANEL1 = wxNewId();
const long LayoutPanel::ID_SPLITTERWINDOW2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(LayoutPanel,wxPanel)
	//(*EventTable(LayoutPanel)
	//*)
END_EVENT_TABLE()


const long LayoutPanel::ID_PREVIEW_ALIGN = wxNewId();
const long LayoutPanel::ID_PREVIEW_MODEL_PROPERTIES = wxNewId();
const long LayoutPanel::ID_PREVIEW_MODEL_NODELAYOUT = wxNewId();
const long LayoutPanel::ID_PREVIEW_MODEL_EXPORTCSV = wxNewId();
const long LayoutPanel::ID_PREVIEW_ALIGN_TOP = wxNewId();
const long LayoutPanel::ID_PREVIEW_ALIGN_BOTTOM = wxNewId();
const long LayoutPanel::ID_PREVIEW_ALIGN_LEFT = wxNewId();
const long LayoutPanel::ID_PREVIEW_ALIGN_RIGHT = wxNewId();
const long LayoutPanel::ID_PREVIEW_ALIGN_H_CENTER = wxNewId();
const long LayoutPanel::ID_PREVIEW_ALIGN_V_CENTER = wxNewId();
const long LayoutPanel::ID_PREVIEW_DISTRIBUTE = wxNewId();
const long LayoutPanel::ID_PREVIEW_H_DISTRIBUTE = wxNewId();
const long LayoutPanel::ID_PREVIEW_V_DISTRIBUTE = wxNewId();

class NewModelBitmapButton : public wxBitmapButton
{
public:

    NewModelBitmapButton(wxWindow *parent, const wxBitmap &bmp, const std::string &type)
        : wxBitmapButton(parent, wxID_ANY, bmp), bitmap(bmp), state(0), modelType(type) {
        SetToolTip("Create new " + type);
    }
    virtual ~NewModelBitmapButton() {}

    void SetState(unsigned int s) {
        if (s > 2) {
            s = 0;
        }
        state = s;
        if (state == 2) {
            SetBitmap(bitmap.ConvertToDisabled());
        } else if (state == 1) {
            const wxImage imgDisabled = bitmap.ConvertToImage().ConvertToDisabled(128);
#if wxCHECK_VERSION(3, 1, 0)
            SetBitmap(wxBitmap(imgDisabled, -1, bitmap.GetScaleFactor()));
#else 
            SetBitmap(wxBitmap(imgDisabled));
#endif
        } else {
            SetBitmap(bitmap);
        }
    }
    unsigned int GetState() {
        return state;
    }
    const std::string &GetModelType() {
        return modelType;
    }
protected:
private:
    const std::string modelType;
    unsigned int state;
    wxBitmap bitmap;
};



LayoutPanel::LayoutPanel(wxWindow* parent, xLightsFrame *xl) : xlights(xl),
    m_creating_bound_rect(false), mPointSize(2), m_rotating(false), m_dragging(false),
    m_over_handle(0), selectedButton(nullptr), newModel(nullptr), selectedModel(nullptr)
{
    appearanceVisible = sizeVisible = stringPropsVisible = false;

	//(*Initialize(LayoutPanel)
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizerPreview;
	wxStaticText* StaticText1;
	wxFlexGridSizer* LeftPanelSizer;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizerPreview = new wxFlexGridSizer(1, 1, 0, 0);
	FlexGridSizerPreview->AddGrowableCol(0);
	FlexGridSizerPreview->AddGrowableRow(0);
	SplitterWindow2 = new wxSplitterWindow(this, ID_SPLITTERWINDOW2, wxDefaultPosition, wxDefaultSize, wxSP_3D, _T("ID_SPLITTERWINDOW2"));
	SplitterWindow2->SetMinimumPaneSize(10);
	SplitterWindow2->SetSashGravity(0.5);
	LeftPanel = new wxPanel(SplitterWindow2, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	LeftPanelSizer = new wxFlexGridSizer(0, 1, 0, 0);
	LeftPanelSizer->AddGrowableCol(0);
	LeftPanelSizer->AddGrowableRow(1);
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText1 = new wxStaticText(LeftPanel, wxID_ANY, _("View:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ViewChoice = new wxChoice(LeftPanel, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	ViewChoice->Append(_("All Models"));
	ViewChoice->SetSelection( ViewChoice->Append(_("My Display")) );
	ViewChoice->Append(_("Selected Groups"));
	FlexGridSizer1->Add(ViewChoice, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSelectModelGroups = new wxButton(LeftPanel, ID_BUTTON_SELECT_MODEL_GROUPS, _("Model Groups"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_SELECT_MODEL_GROUPS"));
	FlexGridSizer1->Add(ButtonSelectModelGroups, 1, wxALL, 2);
	LeftPanelSizer->Add(FlexGridSizer1, 1, wxALL|wxEXPAND, 1);
	ModelSplitter = new wxSplitterWindow(LeftPanel, ID_SPLITTERWINDOW1, wxDefaultPosition, wxDefaultSize, wxSP_3D, _T("ID_SPLITTERWINDOW1"));
	ModelSplitter->SetMinSize(wxSize(50,50));
	ModelSplitter->SetMinimumPaneSize(50);
	ModelSplitter->SetSashGravity(0.5);
	ListBoxElementList = new wxListView(ModelSplitter, ID_LISTBOX_ELEMENT_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_SORT_ASCENDING, wxDefaultValidator, _T("ID_LISTBOX_ELEMENT_LIST"));
	SecondPanel = new wxPanel(ModelSplitter, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	ModelSplitter->SplitHorizontally(ListBoxElementList, SecondPanel);
	LeftPanelSizer->Add(ModelSplitter, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE, 2);
	CheckBoxOverlap = new wxCheckBox(LeftPanel, ID_CHECKBOXOVERLAP, _("Overlap checks enabled"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXOVERLAP"));
	CheckBoxOverlap->SetValue(false);
	LeftPanelSizer->Add(CheckBoxOverlap, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	ButtonModelsPreview = new wxButton(LeftPanel, ID_BUTTON_MODELS_PREVIEW, _("Models"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_MODELS_PREVIEW"));
	FlexGridSizer2->Add(ButtonModelsPreview, 1, wxALL|wxEXPAND, 5);
	ButtonSavePreview = new wxButton(LeftPanel, ID_BUTTON_SAVE_PREVIEW, _("Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_SAVE_PREVIEW"));
	FlexGridSizer2->Add(ButtonSavePreview, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	LeftPanelSizer->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	LeftPanel->SetSizer(LeftPanelSizer);
	LeftPanelSizer->Fit(LeftPanel);
	LeftPanelSizer->SetSizeHints(LeftPanel);
	PreviewGLPanel = new wxPanel(SplitterWindow2, ID_PANEL1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL1"));
	PreviewGLSizer = new wxFlexGridSizer(2, 1, 0, 0);
	PreviewGLSizer->AddGrowableCol(0);
	PreviewGLSizer->AddGrowableRow(1);
	ToolSizer = new wxFlexGridSizer(0, 10, 0, 0);
	PreviewGLSizer->Add(ToolSizer, 1, wxALL|wxEXPAND, 3);
	PreviewGLPanel->SetSizer(PreviewGLSizer);
	PreviewGLSizer->Fit(PreviewGLPanel);
	PreviewGLSizer->SetSizeHints(PreviewGLPanel);
	SplitterWindow2->SplitVertically(LeftPanel, PreviewGLPanel);
	SplitterWindow2->SetSashPosition(175);
	FlexGridSizerPreview->Add(SplitterWindow2, 1, wxALL|wxEXPAND, 1);
	SetSizer(FlexGridSizerPreview);
	FlexGridSizerPreview->Fit(this);
	FlexGridSizerPreview->SetSizeHints(this);

	Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&LayoutPanel::OnViewChoiceSelect);
	Connect(ID_BUTTON_SELECT_MODEL_GROUPS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LayoutPanel::OnButtonSelectModelGroupsClick);
	Connect(ID_LISTBOX_ELEMENT_LIST,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&LayoutPanel::OnListBoxElementListItemSelect);
	Connect(ID_LISTBOX_ELEMENT_LIST,wxEVT_COMMAND_LIST_COL_CLICK,(wxObjectEventFunction)&LayoutPanel::OnListBoxElementListColumnClick);
	Connect(ID_SPLITTERWINDOW1,wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED,(wxObjectEventFunction)&LayoutPanel::OnModelSplitterSashPosChanged);
	Connect(ID_CHECKBOXOVERLAP,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&LayoutPanel::OnCheckBoxOverlapClick);
	Connect(ID_BUTTON_MODELS_PREVIEW,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LayoutPanel::OnButtonModelsPreviewClick);
	Connect(ID_BUTTON_SAVE_PREVIEW,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LayoutPanel::OnButtonSavePreviewClick);
	Connect(ID_SPLITTERWINDOW2,wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED,(wxObjectEventFunction)&LayoutPanel::OnSplitterWindowSashPosChanged);
	//*)

    modelPreview = new ModelPreview( (wxPanel*) PreviewGLPanel, xlights->PreviewModels, true);
    PreviewGLSizer->Add(modelPreview, 1, wxALL | wxEXPAND, 0);
    PreviewGLSizer->Fit(PreviewGLPanel);
    PreviewGLSizer->SetSizeHints(PreviewGLPanel);
    FlexGridSizerPreview->Fit(this);
    FlexGridSizerPreview->SetSizeHints(this);

    modelPreview->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&LayoutPanel::OnPreviewLeftDown,0,this);
    modelPreview->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&LayoutPanel::OnPreviewLeftUp,0,this);
    modelPreview->Connect(wxEVT_RIGHT_DOWN,(wxObjectEventFunction)&LayoutPanel::OnPreviewRightDown,0,this);
    modelPreview->Connect(wxEVT_MOTION,(wxObjectEventFunction)&LayoutPanel::OnPreviewMouseMove,0,this);
    modelPreview->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&LayoutPanel::OnPreviewMouseLeave,0,this);

    propertyEditor = new wxPropertyGrid(ModelSplitter,
                                        wxID_ANY, // id
                                        wxDefaultPosition, // position
                                        wxDefaultSize, // size
                                        // Here are just some of the supported window styles
                                        //wxPG_AUTO_SORT | // Automatic sorting after items added
                                        wxPG_SPLITTER_AUTO_CENTER | // Automatically center splitter until user manually adjusts it
                                        // Default style
                                        wxPG_DEFAULT_STYLE );
#ifdef __WXOSX__
    propertyEditor->SetExtraStyle(wxPG_EX_NATIVE_DOUBLE_BUFFERING | wxWS_EX_PROCESS_IDLE);
#else
    propertyEditor->SetExtraStyle(wxWS_EX_PROCESS_IDLE);
#endif
    ModelSplitter->ReplaceWindow(SecondPanel, propertyEditor);
    LeftPanelSizer->Fit(LeftPanel);
    LeftPanelSizer->SetSizeHints(LeftPanel);

    //LeftPanelSizer->Add(propertyEditor, 1, wxALL|wxEXPAND, 0);
    //LeftPanelSizer->AddGrowableRow(13);
    propertyEditor->Connect(wxEVT_PG_CHANGING, (wxObjectEventFunction)&LayoutPanel::OnPropertyGridChanging,0,this);
    propertyEditor->Connect(wxEVT_PG_CHANGED, (wxObjectEventFunction)&LayoutPanel::OnPropertyGridChange,0,this);
    propertyEditor->SetValidationFailureBehavior(wxPG_VFB_MARK_CELL | wxPG_VFB_BEEP);

    wxConfigBase* config = wxConfigBase::Get();
    int msp = config->Read("LayoutModelSplitterSash", -1);
    int sp = config->Read("LayoutMainSplitterSash", -1);
    if (sp != -1) {
        SplitterWindow2->SetSashGravity(0.0);
        SplitterWindow2->SetSashPosition(sp);
    }
    if (msp != -1) {
        ModelSplitter->SetSashGravity(0.0);
        ModelSplitter->SetSashPosition(msp);
    }


    wxListItem elementCol;
    elementCol.SetText(_T("Element Name"));
    elementCol.SetImage(-1);
    elementCol.SetAlign(wxLIST_FORMAT_LEFT);
    ListBoxElementList->InsertColumn(0, elementCol);

    elementCol.SetText(_T("Start Chan"));
    elementCol.SetAlign(wxLIST_FORMAT_LEFT);
    ListBoxElementList->InsertColumn(1, elementCol);

    elementCol.SetText(_T("End Chan"));
    elementCol.SetAlign(wxLIST_FORMAT_LEFT);
    ListBoxElementList->InsertColumn(2, elementCol);
    ListBoxElementList->SetColumnWidth(0,10);
    ListBoxElementList->SetColumnWidth(1,10);
    ListBoxElementList->SetColumnWidth(2,10);

    AddModelButton("Arches", arches);
    AddModelButton("Candy Canes", canes);
    AddModelButton("Circle", circles);
    AddModelButton("Custom", custom);
    AddModelButton("Matrix", matrix);
    AddModelButton("Single Line", singleline);
    AddModelButton("Star", star);
    AddModelButton("Tree", tree);
    AddModelButton("Window Frame", frame);
    AddModelButton("Wreath", wreath);

    modelPreview->Connect(wxID_CUT, wxEVT_MENU, (wxObjectEventFunction)&LayoutPanel::DoCut,0,this);
    modelPreview->Connect(wxID_COPY, wxEVT_MENU, (wxObjectEventFunction)&LayoutPanel::DoCopy,0,this);
    modelPreview->Connect(wxID_PASTE, wxEVT_MENU, (wxObjectEventFunction)&LayoutPanel::DoPaste,0,this);

}

void LayoutPanel::AddModelButton(const std::string &type, const char *data[]) {
    wxImage image(data);
#ifdef __WXOSX__
    wxBitmap bitmap(image, -1, 2.0);
#else
    image.Rescale(24, 24, wxIMAGE_QUALITY_HIGH);
    wxBitmap bitmap(image);
#endif
    NewModelBitmapButton *button = new NewModelBitmapButton(PreviewGLPanel, bitmap, type);
    ToolSizer->Add(button, 1, wxALL, 0);
    buttons.push_back(button);
    Connect(button->GetId(), wxEVT_BUTTON, (wxObjectEventFunction)&LayoutPanel::OnNewModelTypeButtonClicked);
}

LayoutPanel::~LayoutPanel()
{
	//(*Destroy(LayoutPanel)
	//*)
}

void LayoutPanel::OnPropertyGridChange(wxPropertyGridEvent& event) {
    wxString name = event.GetPropertyName();
    if (name == "BkgBrightness") {
        xlights->SetPreviewBackgroundBrightness(event.GetValue().GetLong());
    } else if (name == "BkgSizeWidth") {
        xlights->SetPreviewSize(event.GetValue().GetLong(), modelPreview->GetVirtualCanvasHeight());
    } else if (name == "BkgSizeHeight") {
        xlights->SetPreviewSize(modelPreview->GetVirtualCanvasWidth(), event.GetValue().GetLong());
    } else if (name == "BkgImage") {
        xlights->SetPreviewBackgroundImage(event.GetValue().GetString());
    } else if (name == "BkgFill") {
        xlights->SetPreviewBackgroundScaled(event.GetValue().GetBool());
    } else if (selectedModel != nullptr) {
        //model property
        if ("ModelRotation" == name) {
            PreviewRotationUpdated(event.GetValue().GetInteger());
        } else if ("ModelWidth" == name) {
            double newscalex, newscaley;
            selectedModel->GetScales(newscalex, newscaley);
            newscalex *= 100.0;
            newscaley *= 100.0;

            PreviewScaleUpdated(event.GetValue().GetDouble(), newscaley);
        } else if ("ModelHeight" == name) {
            double newscalex, newscaley;
            selectedModel->GetScales(newscalex, newscaley);
            newscalex *= 100.0;
            newscaley *= 100.0;
            PreviewScaleUpdated(newscalex, event.GetValue().GetDouble());
        } else if ("ModelX" == name) {
            selectedModel->SetHcenterOffset(event.GetValue().GetDouble() / 100.0f);
            selectedModel->UpdateXmlWithScale();
            xlights->UpdatePreview();
            xlights->UnsavedRgbEffectsChanges = true;
        } else if ("ModelY" == name) {
            selectedModel->SetVcenterOffset(event.GetValue().GetDouble() / 100.0f);
            selectedModel->UpdateXmlWithScale();
            xlights->UpdatePreview();
            xlights->UnsavedRgbEffectsChanges = true;
        } else if ("ModelName" == name) {
            if (selectedModel->name != event.GetValue().GetString().ToStdString()) {
                for(int i=0;i<ListBoxElementList->GetItemCount();i++) {
                    if (selectedModel->name == ListBoxElementList->GetItemText(i)) {
                        ListBoxElementList->SetItemText(i, event.GetValue().GetString());
                    }
                }
                xlights->RenameModel(selectedModel->name, event.GetValue().GetString().ToStdString());
            }
        } else if ("ModelMyDisplay" == name) {
            selectedModel->SetMyDisplay(event.GetValue().GetBool());
            xlights->UnsavedRgbEffectsChanges = true;
            xlights->UpdatePreview();
        } else {
            int i = selectedModel->OnPropertyGridChange(propertyEditor, event);
            if (i & 0x0001) {
                xlights->UpdatePreview();
            }
            if (i & 0x0002) {
                xlights->UnsavedRgbEffectsChanges = true;
            }
            if (i & 0x0004) {
                CallAfter(&LayoutPanel::resetPropertyGrid);
            }
            if (i == 0) {
                printf("Did not handle %s   %s\n",
                       event.GetPropertyName().ToStdString().c_str(),
                       event.GetValue().GetString().ToStdString().c_str());
            }
        }
    }
}
void LayoutPanel::OnPropertyGridChanging(wxPropertyGridEvent& event) {
    if (selectedModel != nullptr) {
        if ("ModelName" == event.GetPropertyName()) {
            if (xlights->AllModels[event.GetValue().GetString().ToStdString()] != nullptr) {
                event.Veto();
            }
        } else {
            selectedModel->OnPropertyGridChanging(propertyEditor, event);
        }
    }
}

void LayoutPanel::PreviewScaleUpdated(float xscale, float yscale)
{
    int sel=ListBoxElementList->GetFirstSelected();
    if (sel == wxNOT_FOUND) return;
    Model* m=(Model*)ListBoxElementList->GetItemData(sel);
    m->SetScale(xscale/100.0, yscale/100.0);
    m->UpdateXmlWithScale();
    xlights->UnsavedRgbEffectsChanges = true;
    UpdatePreview();
}

void LayoutPanel::PreviewRotationUpdated(int newRotation)
{
    int sel=ListBoxElementList->GetFirstSelected();
    if (sel == wxNOT_FOUND) return;
    Model* m=(Model*)ListBoxElementList->GetItemData(sel);
    m->SetModelCoord(newRotation);
    m->UpdateXmlWithScale();
    xlights->UnsavedRgbEffectsChanges = true;
    UpdatePreview();
}

void LayoutPanel::UpdatePreview()
{
    if(!modelPreview->StartDrawing(mPointSize)) return;
    if(m_creating_bound_rect)
    {
        DrawGLUtils::DrawRectangle(xlYELLOW,true,m_bound_start_x,m_bound_start_y,m_bound_end_x,m_bound_end_y);
    }
    modelPreview->Render();
    modelPreview->EndDrawing();
}

void LayoutPanel::resetPropertyGrid() {
    clearPropGrid();
    SetupPropGrid(selectedModel);
}
void LayoutPanel::clearPropGrid() {
    wxPGProperty *p = propertyEditor->GetPropertyByName("ModelAppearance");
    if (p != nullptr) {
        appearanceVisible = propertyEditor->IsPropertyExpanded(p);
    }
    p = propertyEditor->GetPropertyByName("ModelSize");
    if (p != nullptr) {
        sizeVisible = propertyEditor->IsPropertyExpanded(p);
    }
    p = propertyEditor->GetPropertyByName("ModelStringProperties");
    if (p != nullptr) {
        stringPropsVisible = propertyEditor->IsPropertyExpanded(p);
    }
    propertyEditor->Clear();
}
int wxCALLBACK MyCompareFunction(wxIntPtr item1, wxIntPtr item2, wxIntPtr WXUNUSED(sortData))
{
    wxString a = ((Model*)item1)->name;
    wxString b = ((Model*)item2)->name;
    return a.CmpNoCase(b);
}
void LayoutPanel::UpdateModelList(bool addGroups) {
    UnSelectAllModels();
    ListBoxElementList->DeleteAllItems();

    std::vector<Model *> models;
    std::string selection = ViewChoice->GetStringSelection().ToStdString();
    switch (ViewChoice->GetSelection()) {
        case 0:
            for (auto it = xlights->AllModels.begin(); it != xlights->AllModels.end(); it++) {
                if (it->second->GetDisplayAs() != "ModelGroup") {
                    models.push_back(it->second);
                }
            }
            break;
        case 1:
            for (auto it = xlights->AllModels.begin(); it != xlights->AllModels.end(); it++) {
                if (it->second->GetDisplayAs() != "ModelGroup" && it->second->IsMyDisplay()) {
                    models.push_back(it->second);
                }
            }
            break;
        case 2:
            for (auto it = xlights->PreviewModels.begin(); it != xlights->PreviewModels.end(); it++) {
                models.push_back(*it);
            }
            break;
        default: {
            Model *grp = xlights->AllModels[selection];
            if (grp != nullptr && grp->GetDisplayAs() == "ModelGroup") {
                ModelGroup *mgrp = (ModelGroup*)grp;
                for (auto it = mgrp->Models().begin(); it != mgrp->Models().end(); it++) {
                    models.push_back(*it);
                }
            }
            }
            break;
    }
    if (addGroups) {
        while (ViewChoice->GetCount() > 3) {
            ViewChoice->Delete(3);
        }
        for (auto it = xlights->AllModels.begin(); it != xlights->AllModels.end(); it++) {
            Model *model = it->second;
            if (model->GetDisplayAs() == "ModelGroup") {
                ViewChoice->Append(it->first);
                if (it->first == selection) {
                    ViewChoice->SetSelection(ViewChoice->GetCount() - 1);
                }
            }
        }
    }


    for (auto it = models.begin(); it != models.end(); it++) {
        Model *model = *it;
        if (model->GetDisplayAs() == "ModelGroup") {
            continue;
        }
        long itemIndex = ListBoxElementList->InsertItem(ListBoxElementList->GetItemCount(), model->name);

        std::string start_channel = model->GetModelXml()->GetAttribute("StartChannel").ToStdString();
        model->SetModelStartChan(start_channel);
        int end_channel = model->GetLastChannel()+1;
        ListBoxElementList->SetItem(itemIndex,1, start_channel);
        ListBoxElementList->SetItem(itemIndex,2, wxString::Format(wxT("%i"),end_channel));
        ListBoxElementList->SetItemPtrData(itemIndex,(wxUIntPtr)model);
    }


    ListBoxElementList->SortItems(MyCompareFunction,0);
    ListBoxElementList->SetColumnWidth(0,wxLIST_AUTOSIZE);
    ListBoxElementList->SetColumnWidth(1,wxLIST_AUTOSIZE);
    ListBoxElementList->SetColumnWidth(2,wxLIST_AUTOSIZE);
    modelPreview->SetModels(models);
    UpdatePreview();
}
void LayoutPanel::UnSelectAllModels()
{
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        modelPreview->GetModels()[i]->Selected = false;
        modelPreview->GetModels()[i]->GroupSelected = false;
    }
    UpdatePreview();

    propertyEditor->Freeze();
    clearPropGrid();
    propertyEditor->Append(new wxImageFileProperty("Background Image",
                                                   "BkgImage",
                                                   modelPreview->GetBackgroundImage()));
    propertyEditor->Append(new wxBoolProperty("Fill", "BkgFill", modelPreview->GetScaleBackgroundImage()))->SetAttribute("UseCheckbox", 1);
    wxPGProperty* prop = propertyEditor->Append(new wxUIntProperty("Width", "BkgSizeWidth", modelPreview->GetVirtualCanvasWidth()));
    prop->SetAttribute("Min", 0);
    prop->SetAttribute("Max", 4096);
    prop->SetEditor("SpinCtrl");
    prop = propertyEditor->Append(new wxUIntProperty("Height", "BkgSizeHeight", modelPreview->GetVirtualCanvasHeight()));
    prop->SetAttribute("Min", 0);
    prop->SetAttribute("Max", 4096);
    prop->SetEditor("SpinCtrl");
    prop = propertyEditor->Append(new wxStringProperty("Brightness",
                                                       "BkgBrightness",
                                                       wxString::Format("%d", modelPreview->GetBackgroundBrightness())));
    prop->SetAttribute("Min", 0);
    prop->SetAttribute("Max", 100);
    prop->SetEditor("SpinCtrl");
    propertyEditor->Thaw();
}

void LayoutPanel::SetupPropGrid(Model *model) {
    double newscalex, newscaley;
    model->GetScales(newscalex, newscaley);
    newscalex *= 100.0;
    newscaley *= 100.0;

    propertyEditor->Freeze();
    clearPropGrid();

    wxPGProperty* prop = propertyEditor->Append(new wxStringProperty("Name", "ModelName", model->name));

    model->AddProperties(propertyEditor);

    wxPGProperty *p2 = propertyEditor->Append(new wxPropertyCategory("Size/Location", "ModelSize"));

    prop = propertyEditor->Append(new wxFloatProperty("X (%)", "ModelX", model->GetHcenterOffset() * 100.0));
    prop->SetAttribute("Precision", 2);
    prop->SetAttribute("Step", 0.5);
    prop->SetEditor("SpinCtrl");
    prop = propertyEditor->Append(new wxFloatProperty("Y (%)", "ModelY", model->GetVcenterOffset() * 100.0));
    prop->SetAttribute("Precision", 2);
    prop->SetAttribute("Step", 0.5);
    prop->SetEditor("SpinCtrl");
    prop = propertyEditor->Append(new wxFloatProperty("Width", "ModelWidth", newscalex));
    prop->SetAttribute("Precision", 2);
    prop->SetAttribute("Step", 0.1);
    prop->SetEditor("SpinCtrl");
    prop = propertyEditor->Append(new wxFloatProperty("Height", "ModelHeight", newscaley));
    prop->SetAttribute("Precision", 2);
    prop->SetAttribute("Step", 0.1);
    prop->SetEditor("SpinCtrl");
    prop = propertyEditor->Append(new wxIntProperty("Rotation", "ModelRotation", model->GetRotation()));
    prop->SetAttribute("Min", "-180");
    prop->SetAttribute("Max", "180");
    prop->SetEditor("SpinCtrl");
    if (!sizeVisible) {
        propertyEditor->Collapse(p2);
    }
    if (!appearanceVisible) {
        prop = propertyEditor->GetPropertyByName("ModelAppearance");
        if (prop != nullptr) {
            propertyEditor->Collapse(prop);
        }
    }
    if (!stringPropsVisible) {
        prop = propertyEditor->GetPropertyByName("ModelStringProperties");
        if (prop != nullptr) {
            propertyEditor->Collapse(prop);
        }
    }
    propertyEditor->Thaw();
}

void LayoutPanel::SelectModel(const std::string & name)
{
    modelPreview->SetFocus();
    int foundStart = 0;
    int foundEnd = 0;
    Model* m = nullptr;
    for(int i=0;i<ListBoxElementList->GetItemCount();i++)
    {
        if (name == ListBoxElementList->GetItemText(i))
        {
            if (ListBoxElementList->GetItemState(i, wxLIST_STATE_SELECTED) == false) {
                ListBoxElementList->SetItemState(i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
                ListBoxElementList->EnsureVisible(i);
                return;
            }
            m=(Model*)ListBoxElementList->GetItemData(i);
            m->Selected = true;
            if (CheckBoxOverlap->GetValue()== true) {
                foundStart = m->GetNumberFromChannelString(m->ModelStartChannel);
                foundEnd = m->GetNumberFromChannelString(ListBoxElementList->GetItemText(i,2).ToStdString());
            }
            SetupPropGrid(m);
            break;
        }
    }
    if (m == nullptr) {
        propertyEditor->Freeze();
        clearPropGrid();
        propertyEditor->Thaw();
    }
    selectedModel = m;
    if (CheckBoxOverlap->GetValue()) {
        for(int i=0;i<ListBoxElementList->GetItemCount();i++)
        {
            if (name != ListBoxElementList->GetItemText(i)) {
                Model* m=(Model*)ListBoxElementList->GetItemData(i);
                if (m != NULL) {
                    int startChan = m->GetNumberFromChannelString(ListBoxElementList->GetItemText(i,1).ToStdString());
                    int endChan = m->GetNumberFromChannelString(ListBoxElementList->GetItemText(i,2).ToStdString());
                    if ((startChan >= foundStart) && (endChan <= foundEnd)) {
                        m->Overlapping = true;
                    } else if ((startChan >= foundStart) && (startChan <= foundEnd)) {
                        m->Overlapping = true;
                    } else if ((endChan >= foundStart) && (endChan <= foundEnd)) {
                        m->Overlapping = true;
                    } else {
                        m->Overlapping = false;
                    }
                }
            }
        }
    }
    UpdatePreview();
}


void LayoutPanel::OnCheckBoxOverlapClick(wxCommandEvent& event)
{
    if (CheckBoxOverlap->GetValue() == false) {
        for(int i=0;i<ListBoxElementList->GetItemCount();i++)
        {
            Model* m=(Model*)ListBoxElementList->GetItemData(i);
            if (m != NULL) {
                m->Overlapping = false;
            }
        }
    }
}

void LayoutPanel::OnButtonModelsPreviewClick(wxCommandEvent& event)
{
    // update xml with offsets and scale
    for (size_t i=0; i < modelPreview->GetModels().size(); i++)
    {
        modelPreview->GetModels()[i]->UpdateXmlWithScale();
    }
    xlights->ShowModelsDialog();
    UpdatePreview();
}

void LayoutPanel::OnButtonSavePreviewClick(wxCommandEvent& event)
{
    // update xml with offsets and scale
    for (size_t i=0; i < modelPreview->GetModels().size(); i++)
    {
        modelPreview->GetModels()[i]->UpdateXmlWithScale();
    }
    xlights->SaveEffectsFile();
    xlights->SetStatusText(_("Preview layout saved"));
}


void LayoutPanel::OnButtonSelectModelGroupsClick(wxCommandEvent& event)
{
    CurrentPreviewModels dialog(this,xlights->ModelGroupsNode,xlights->AllModels);
    dialog.ShowModal();

    for (wxXmlNode *node = xlights->ModelGroupsNode->GetChildren(); node != nullptr; node = node->GetNext()) {
        wxString oldName = node->GetAttribute("oldName", "");
        node->DeleteAttribute("oldName");
        if (oldName != "") {
            Element* elem_to_rename = xlights->GetSequenceElements().GetElement(oldName.ToStdString());
            if( elem_to_rename != NULL ) {
                elem_to_rename->SetName(node->GetAttribute("name").ToStdString());
            }
        }
    }

    xlights->UnsavedRgbEffectsChanges=true;
    xlights->UpdateModelsList();
    UpdatePreview();
    wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
    xlights->RowHeadingsChanged(eventRowHeaderChanged);
}

void LayoutPanel::OnListBoxElementListItemSelect(wxListEvent& event)
{
    UnSelectAllModels();
    int sel = ListBoxElementList->GetFirstSelected();
    if (sel == wxNOT_FOUND)
    {
        ListBoxElementList->SetToolTip("");
        return;
    }
    std::string name = ListBoxElementList->GetItemText(sel).ToStdString();
    SelectModel(name);

    for (int i = 0; i < ListBoxElementList->GetItemCount(); i++)
    {
        if (name == ListBoxElementList->GetItemText(i))
        {
            Model* m = (Model*)ListBoxElementList->GetItemData(i);
            ListBoxElementList->SetToolTip(xlights->GetChannelToControllerMapping(m->GetNumberFromChannelString(m->ModelStartChannel)));
        }
    }
}

int wxCALLBACK SortElementsFunctionASC(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortColumn)
{
    Model* a = (Model *)item1;
    Model* b = (Model *)item2;

    if (sortColumn == 1) {
        int ia = a->GetNumberFromChannelString(a->ModelStartChannel);
        int ib = b->GetNumberFromChannelString(b->ModelStartChannel);
        if (ia > ib)
            return 1;
        if (ia < ib)
            return -1;
        return 0;
    } else if (sortColumn == 2) {
        int ia = a->GetLastChannel();
        int ib = b->GetLastChannel();
        if (ia > ib)
            return 1;
        if (ia < ib)
            return -1;
        return 0;
    } else {
        return strcasecmp(a->name.c_str(), b->name.c_str());
    }
    return 0;
}

int wxCALLBACK SortElementsFunctionDESC(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortColumn)
{
    return SortElementsFunctionASC(item2, item1, sortColumn);
}
void LayoutPanel::OnListBoxElementListColumnClick(wxListEvent& event)
{
    int col = event.GetColumn();
    static bool x = false;
    x = !x;
    x ? ListBoxElementList->SortItems(SortElementsFunctionASC,col):ListBoxElementList->SortItems(SortElementsFunctionDESC,col);
}

int LayoutPanel::FindModelsClicked(int x,int y,std::vector<int> &found)
{
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        if(modelPreview->GetModels()[i]->HitTest(modelPreview,x,y))
        {
            found.push_back(i);
        }
    }
    return found.size();
}



bool LayoutPanel::SelectSingleModel(int x,int y)
{
    std::vector<int> found;
    int modelCount = FindModelsClicked(x,y,found);
    if (modelCount==0)
    {
        return false;
    }
    else if(modelCount==1)
    {
        SelectModel(modelPreview->GetModels()[found[0]]->name);
        mHitTestNextSelectModelIndex = 0;
        return true;
    }
    else if (modelCount>1)
    {
        for(int i=0;i<modelCount;i++)
        {
            if(mHitTestNextSelectModelIndex==i)
            {
                SelectModel(modelPreview->GetModels()[found[i]]->name);
                mHitTestNextSelectModelIndex += 1;
                mHitTestNextSelectModelIndex %= modelCount;
                return true;
            }
        }
    }
    return false;
}
void LayoutPanel::SelectAllInBoundingRect()
{
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        if(modelPreview->GetModels()[i]->IsContained(modelPreview,m_bound_start_x,m_bound_start_y,
                                         m_bound_end_x,m_bound_end_y))
        {
            modelPreview->GetModels()[i]->GroupSelected = true;
        }
    }
}

bool LayoutPanel::SelectMultipleModels(int x,int y)
{
    std::vector<int> found;
    int modelCount = FindModelsClicked(x,y,found);
    if (modelCount==0)
    {
        return false;
    }
    else if(modelCount>0)
    {
        propertyEditor->Freeze();
        clearPropGrid();
        propertyEditor->Thaw();

        if(modelPreview->GetModels()[found[0]]->Selected)
        {
            modelPreview->GetModels()[found[0]]->Selected = false;
            modelPreview->GetModels()[found[0]]->GroupSelected = false;
        }
        else if (modelPreview->GetModels()[found[0]]->GroupSelected)
        {
            SetSelectedModelToGroupSelected();
            modelPreview->GetModels()[found[0]]->Selected = true;
            SelectModel(modelPreview->GetModels()[found[0]]->name);
        }
        else
        {
            modelPreview->GetModels()[found[0]]->GroupSelected = true;
        }
        UpdatePreview();
        return true;
    }
    return false;
}

void LayoutPanel::SetSelectedModelToGroupSelected()
{
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        if(modelPreview->GetModels()[i]->Selected)
        {
            modelPreview->GetModels()[i]->Selected = false;
            modelPreview->GetModels()[i]->GroupSelected = true;
        }
    }
}


void LayoutPanel::OnPreviewLeftDown(wxMouseEvent& event)
{
    modelPreview->SetFocus();
    int y = event.GetY();
    if (event.ControlDown())
    {
        SelectMultipleModels(event.GetX(),y);
        m_dragging = true;
        m_previous_mouse_x = event.GetX();
        m_previous_mouse_y = event.GetY();
    }
    else if (event.ShiftDown())
    {
        m_creating_bound_rect = true;
        m_bound_start_x = event.GetX();
        m_bound_start_y = modelPreview->GetVirtualCanvasHeight() - y;
    }
    else if (m_over_handle == OVER_ROTATE_HANDLE)
    {
        m_rotating = true;
    }
    else if (m_over_handle != OVER_NO_HANDLE)
    {
        m_resizing = true;
    }
    else if (selectedButton != nullptr)
    {
        //create a new model
        m_rotating = false;
        m_resizing = true;
        m_creating_bound_rect = false;
        m_previous_mouse_x = event.GetX();
        m_previous_mouse_y = event.GetY();
        newModel = CreateNewModel(selectedButton->GetModelType());

        if (newModel != nullptr) {
            newModel->Selected = true;
            int wi, ht;
            modelPreview->GetVirtualCanvasSize(wi,ht);
            if (wi > 0 && ht > 0)
            {
                newModel->SetOffset((double)(m_previous_mouse_x)/(double)(wi), 1.0 - (double)(m_previous_mouse_y)/(double)ht);
                newModel->UpdateXmlWithScale();
            }

            modelPreview->GetModels().push_back(newModel);
        }
    }
    else
    {
        m_rotating = false;
        m_resizing = false;
        m_creating_bound_rect = false;

        if(!event.wxKeyboardState::ControlDown())
        {
            UnSelectAllModels();
        }

        if(SelectSingleModel(event.GetX(),y))
        {
            m_dragging = true;
            m_previous_mouse_x = event.GetX();
            m_previous_mouse_y = event.GetY();
            xlights->SetStatusText(wxString::Format("x=%d y=%d",m_previous_mouse_x,m_previous_mouse_y));
        }
    }
}

void LayoutPanel::OnPreviewLeftUp(wxMouseEvent& event)
{
    int y = event.GetY();

    m_rotating = false;
    m_dragging = false;
    m_resizing = false;
    if(m_creating_bound_rect)
    {
        m_bound_end_x = event.GetPosition().x;
        m_bound_end_y = modelPreview->GetVirtualCanvasHeight() - y;
        SelectAllInBoundingRect();
        m_creating_bound_rect = false;
        UpdatePreview();
    }
    if (newModel != nullptr) {
        xlights->AllModels.AddModel(newModel);
        UpdateModelList();
        if (selectedButton->GetState() == 1) {
            SelectModel(newModel->name);
            newModel = nullptr;
            if (selectedButton != nullptr) {
                selectedButton->SetState(0);
                selectedButton = nullptr;
            }
        } else {
            newModel = nullptr;
        }
    }
}


void LayoutPanel::OnPreviewMouseLeave(wxMouseEvent& event)
{
    m_dragging = false;
}

void LayoutPanel::OnPreviewMouseMove(wxMouseEvent& event)
{
    int y = event.GetY();
    int wi,ht;

    if (m_creating_bound_rect)
    {
        m_bound_end_x = event.GetPosition().x;
        m_bound_end_y = modelPreview->GetVirtualCanvasHeight() - y;
        UpdatePreview();
        return;
    }

    Model *m = newModel;
    if (m == nullptr) {
        int sel=ListBoxElementList->GetFirstSelected();
        if (sel == wxNOT_FOUND) return;
        m=(Model*)ListBoxElementList->GetItemData(sel);
    }

    if(m_rotating)
    {
        m->RotateWithHandles(modelPreview,event.ShiftDown(), event.GetPosition().x,y);
        m->UpdateXmlWithScale();
        SetupPropGrid(m);
        xlights->UnsavedRgbEffectsChanges = true;
        UpdatePreview();
    }
    else if(m_resizing)
    {
        m->ResizeWithHandles(modelPreview,event.GetPosition().x,y);
        double scalex, scaley;
        m->GetScales(scalex, scaley);
        m->UpdateXmlWithScale();
        xlights->UnsavedRgbEffectsChanges = true;
        SetupPropGrid(m);
        UpdatePreview();
    }
    else if (m_dragging && event.Dragging())
    {
        double delta_x = event.GetPosition().x - m_previous_mouse_x;
        double delta_y = -(event.GetPosition().y - m_previous_mouse_y);
        modelPreview->GetSize(&wi,&ht);
        if (wi > 0 && ht > 0)
        {
            for (int i=0; i<modelPreview->GetModels().size(); i++)
            {
                if(modelPreview->GetModels()[i]->Selected || modelPreview->GetModels()[i]->GroupSelected)
                {
                    modelPreview->GetModels()[i]->AddOffset(delta_x/wi, delta_y/ht);
                    modelPreview->GetModels()[i]->UpdateXmlWithScale();
                    SetupPropGrid(modelPreview->GetModels()[i]);
                    xlights->UnsavedRgbEffectsChanges = true;
                }
            }
        }
        m_previous_mouse_x = event.GetPosition().x;
        m_previous_mouse_y = event.GetPosition().y;
        xlights->SetStatusText(wxString::Format("x=%d y=%d",m_previous_mouse_x,m_previous_mouse_y));
        UpdatePreview();
    }
    else
    {
        if(m->Selected)
        {
            m_over_handle = m->CheckIfOverHandles(modelPreview,event.GetPosition().x,modelPreview->GetVirtualCanvasHeight() - y);
        }
    }
}

void LayoutPanel::OnPreviewRightDown(wxMouseEvent& event)
{

    wxMenu mnu;
    wxMenu *mnuAlign;
    wxMenu *mnuDistribute;
    int selectedModelCnt = ModelsSelectedCount();
    if (selectedModelCnt > 1)
    {
        mnuAlign = new wxMenu();
        mnuAlign->Append(ID_PREVIEW_ALIGN_TOP,"Top");
        mnuAlign->Append(ID_PREVIEW_ALIGN_BOTTOM,"Bottom");
        mnuAlign->Append(ID_PREVIEW_ALIGN_LEFT,"Left");
        mnuAlign->Append(ID_PREVIEW_ALIGN_RIGHT,"Right");
        mnuAlign->Append(ID_PREVIEW_ALIGN_H_CENTER,"Horizontal Center");
        mnuAlign->Append(ID_PREVIEW_ALIGN_V_CENTER,"Vertical Center");
        mnuAlign->Connect(wxEVT_MENU, (wxObjectEventFunction)&LayoutPanel::OnPreviewModelPopup, NULL, this);

        mnuDistribute = new wxMenu();
        mnuDistribute->Append(ID_PREVIEW_H_DISTRIBUTE,"Horizontal");
        mnuDistribute->Append(ID_PREVIEW_V_DISTRIBUTE,"Vertical");
        mnuDistribute->Connect(wxEVT_MENU, (wxObjectEventFunction)&LayoutPanel::OnPreviewModelPopup, NULL, this);

        mnu.Append(ID_PREVIEW_ALIGN, 	        "Align", mnuAlign,"");
        mnu.Append(ID_PREVIEW_DISTRIBUTE,"Distribute", mnuDistribute,"");
        mnu.AppendSeparator();
    }
    else if (selectedModelCnt == 0)
    {
        return;
    }
    mnu.Append(ID_PREVIEW_MODEL_PROPERTIES,"Model Properties");
    mnu.Append(ID_PREVIEW_MODEL_NODELAYOUT,"Node Layout");
    mnu.Append(ID_PREVIEW_MODEL_EXPORTCSV,"Export CSV");
    mnu.Connect(wxEVT_MENU, (wxObjectEventFunction)&LayoutPanel::OnPreviewModelPopup, NULL, this);
    PopupMenu(&mnu);
}


void LayoutPanel::OnPreviewModelPopup(wxCommandEvent &event)
{
    if (event.GetId() == ID_PREVIEW_ALIGN_TOP)
    {
        PreviewModelAlignTops();
    }
    else if (event.GetId() == ID_PREVIEW_ALIGN_BOTTOM)
    {
        PreviewModelAlignBottoms();
    }
    else if (event.GetId() == ID_PREVIEW_ALIGN_LEFT)
    {
        PreviewModelAlignLeft();
    }
    else if (event.GetId() == ID_PREVIEW_ALIGN_RIGHT)
    {
        PreviewModelAlignRight();
    }
    else if (event.GetId() == ID_PREVIEW_ALIGN_H_CENTER)
    {
        PreviewModelAlignHCenter();
    }
    else if (event.GetId() == ID_PREVIEW_ALIGN_V_CENTER)
    {
        PreviewModelAlignVCenter();
    }
    else if (event.GetId() == ID_PREVIEW_MODEL_NODELAYOUT)
    {
        int sel = ListBoxElementList->GetFirstSelected();
        if (sel == wxNOT_FOUND) return;
        Model* md=(Model*)ListBoxElementList->GetItemData(sel);
        wxString html=md->ChannelLayoutHtml();
        ChannelLayoutDialog dialog(this);
        dialog.SetHtmlSource(html);
        dialog.ShowModal();
    }
    else if (event.GetId() == ID_PREVIEW_MODEL_EXPORTCSV)
    {
        ExportModel();
    }
    else if (event.GetId() == ID_PREVIEW_MODEL_PROPERTIES)
    {
        ShowModelProperties();
    }

}


#define retmsg(msg)  \
{ \
wxMessageBox(msg, _("Export Error")); \
return; \
}

void LayoutPanel::ExportModel()
{
#if 0
    model name
    display as
    type of strings
#strings
#nodes
    start channel
    start node = (channel+2)/3;
    my display
    brightness
#endif // 0
    wxLogNull logNo; //kludge: avoid "error 0" message from wxWidgets after new file is written
    wxString filename = wxFileSelector(_("Choose output file"), wxEmptyString, wxEmptyString, wxEmptyString, "Export files (*.csv)|*.csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    //    if (filename.IsEmpty()) retmsg(wxString("Please choose an output file name."));
    if (filename.IsEmpty()) return;

    wxFile f(filename);
    //    bool isnew = !wxFile::Exists(filename);
    if (!f.Create(filename, true) || !f.IsOpened()) retmsg(wxString::Format("Unable to create file %s. Error %d\n", filename, f.GetLastError()));
    f.Write(_("Model_name, Display_as, String_type, String_count, Node_count, Start_channel, Start_node, My_display, Brightness+-\n"));

    int sel = ListBoxElementList->GetFirstSelected();
    if (sel == wxNOT_FOUND) return;
    Model* model=(Model*)ListBoxElementList->GetItemData(sel);
    wxString stch = model->GetModelXml()->GetAttribute("StartChannel", wxString::Format("%d?", model->NodeStartChannel(0) + 1)); //NOTE: value coming from model is probably not what is wanted, so show the base ch# instead
    f.Write(wxString::Format("\"%s\", \"%s\", \"%s\", %d, %d, %s, %d, %d\n", model->name, model->GetDisplayAs(), model->GetStringType(), model->GetNodeCount() / model->NodesPerString(), model->GetNodeCount(), stch, /*WRONG:*/ model->NodeStartChannel(0) / model->NodesPerString() + 1, model->MyDisplay));
    f.Close();
}

void LayoutPanel::ShowModelProperties()
{
    int sel = ListBoxElementList->GetFirstSelected();
    if (sel == wxNOT_FOUND) return;
    Model* m=(Model*)ListBoxElementList->GetItemData(sel);

    wxXmlNode* e=m->GetModelXml();
    int DlgResult;
    bool ok;
    ModelDialog *dialog = new ModelDialog(this, xlights);
    dialog->SetFromXml(e, &xlights->GetNetInfo());
    dialog->TextCtrl_Name->Enable(false); // do not allow name changes; -why? -DJ
    do
    {
        ok=true;
        DlgResult=dialog->ShowModal();
        if (DlgResult == wxID_OK)
        {
            // validate inputs
            if (ok)
            {
                dialog->UpdateXml(e);
                xlights->UnsavedRgbEffectsChanges=true;
                xlights->UpdateModelsList();

            }
        }
    }
    while (DlgResult == wxID_OK && !ok);
    UpdatePreview();
    delete dialog;
}

void LayoutPanel::PreviewModelAlignTops()
{
    int selectedindex = GetSelectedModelIndex();
    if (selectedindex<0)
        return;
    int top = modelPreview->GetModels()[selectedindex]->GetTop(modelPreview);
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        if(modelPreview->GetModels()[i]->GroupSelected)
        {
            modelPreview->GetModels()[i]->SetTop(modelPreview,top);
        }
    }
    UpdatePreview();
}

void LayoutPanel::PreviewModelAlignBottoms()
{
    int selectedindex = GetSelectedModelIndex();
    if (selectedindex<0)
        return;
    int bottom = modelPreview->GetModels()[selectedindex]->GetBottom(modelPreview);
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        if(modelPreview->GetModels()[i]->GroupSelected)
        {
            modelPreview->GetModels()[i]->SetBottom(modelPreview,bottom);
        }
    }
    UpdatePreview();
}

void LayoutPanel::PreviewModelAlignLeft()
{
    int selectedindex = GetSelectedModelIndex();
    if (selectedindex<0)
        return;
    int left = modelPreview->GetModels()[selectedindex]->GetLeft(modelPreview);
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        if(modelPreview->GetModels()[i]->GroupSelected)
        {
            modelPreview->GetModels()[i]->SetLeft(modelPreview,left);
        }
    }
    UpdatePreview();
}

void LayoutPanel::PreviewModelAlignRight()
{
    int selectedindex = GetSelectedModelIndex();
    if (selectedindex<0)
        return;
    int right = modelPreview->GetModels()[selectedindex]->GetRight(modelPreview);
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        if(modelPreview->GetModels()[i]->GroupSelected)
        {
            modelPreview->GetModels()[i]->SetRight(modelPreview,right);
        }
    }
    UpdatePreview();
}

void LayoutPanel::PreviewModelAlignHCenter()
{
    int selectedindex = GetSelectedModelIndex();
    if (selectedindex<0)
        return;
    float center = modelPreview->GetModels()[selectedindex]->GetHcenterOffset();
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        if(modelPreview->GetModels()[i]->GroupSelected)
        {
            modelPreview->GetModels()[i]->SetHcenterOffset(center);
        }
    }
    UpdatePreview();
}

void LayoutPanel::PreviewModelAlignVCenter()
{
    int selectedindex = GetSelectedModelIndex();
    if (selectedindex<0)
        return;
    float center = modelPreview->GetModels()[selectedindex]->GetVcenterOffset();
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        if(modelPreview->GetModels()[i]->GroupSelected)
        {
            modelPreview->GetModels()[i]->SetVcenterOffset(center);
        }
    }
    UpdatePreview();
}


int LayoutPanel::GetSelectedModelIndex()
{
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        if(modelPreview->GetModels()[i]->Selected)
        {
            return i;
        }
    }
    return -1;
}
int LayoutPanel::ModelsSelectedCount()
{
    int selectedModelCount=0;
    for (int i=0; i<modelPreview->GetModels().size(); i++)
    {
        if(modelPreview->GetModels()[i]->Selected || modelPreview->GetModels()[i]->GroupSelected)
        {
            selectedModelCount++;
        }
    }
    return selectedModelCount;
}

void LayoutPanel::OnModelSplitterSashPosChanged(wxSplitterEvent& event)
{
    wxConfigBase* config = wxConfigBase::Get();
    config->Write("LayoutModelSplitterSash", event.GetSashPosition());
}

void LayoutPanel::OnSplitterWindowSashPosChanged(wxSplitterEvent& event)
{
    wxConfigBase* config = wxConfigBase::Get();
    config->Write("LayoutMainSplitterSash", event.GetSashPosition());
}

void LayoutPanel::OnViewChoiceSelect(wxCommandEvent& event)
{
    UpdateModelList(false);
}


void LayoutPanel::OnNewModelTypeButtonClicked(wxCommandEvent& event) {
    for (auto it = buttons.begin(); it != buttons.end(); it++) {
        if (event.GetId() == (*it)->GetId()) {
            int state = (*it)->GetState();
            (*it)->SetState(state + 1);
            if ((*it)->GetState()) {
                selectedButton = (*it);
                UnSelectAllModels();
            } else {
                selectedButton = nullptr;
            }
        } else if ((*it)->GetState()) {
            (*it)->SetState(0);
        }
    }
}


Model *LayoutPanel::CreateNewModel(const std::string &type) {
    return xlights->AllModels.CreateDefaultModel(type, xlights->GetNetInfo());
}


void LayoutPanel::DeleteSelectedModel() {
    xlights->AllModels.Delete(selectedModel->name);
    selectedModel = nullptr;
    xlights->UpdateModelsList();
    xlights->UnsavedRgbEffectsChanges = true;
}

void LayoutPanel::DoCopy(wxCommandEvent& event) {
    if (!modelPreview->HasFocus()) {
        event.Skip();
    } else if (selectedModel != nullptr) {
        wxString copy_data;
        wxXmlDocument doc;
        wxXmlNode *parent = selectedModel->GetModelXml()->GetParent();
        wxXmlNode *next = selectedModel->GetModelXml()->GetNext();
        parent->RemoveChild(selectedModel->GetModelXml());
        doc.SetRoot(selectedModel->GetModelXml());
        
        wxStringOutputStream stream;
        doc.Save(stream);
        copy_data = stream.GetString();
        doc.DetachRoot();
        
        parent->InsertChild(selectedModel->GetModelXml(), next);

        if (!copy_data.IsEmpty() && wxTheClipboard->Open()) {
            if (!wxTheClipboard->SetData(new wxTextDataObject(copy_data))) {
                wxMessageBox(_("Unable to copy data to clipboard."), _("Error"));
            }
            wxTheClipboard->Close();
        }
    }
}
void LayoutPanel::DoCut(wxCommandEvent& event) {
    if (!modelPreview->HasFocus()) {
        event.Skip();
    } else if (selectedModel != nullptr) {
        DoCopy(event);
        DeleteSelectedModel();
    }
}
void LayoutPanel::DoPaste(wxCommandEvent& event) {
    if (!modelPreview->HasFocus()) {
        event.Skip();
    } else {
        if (wxTheClipboard->Open()) {
            wxTextDataObject data;
            wxTheClipboard->GetData(data);
            wxStringInputStream in(data.GetText());
            wxXmlDocument doc(in);
            
            wxXmlNode *nd = doc.GetRoot();
            doc.DetachRoot();
            Model *newModel = xlights->AllModels.CreateModel(nd, xlights->GetNetInfo());
            int cnt = 1;
            std::string name = newModel->name;
            while (xlights->AllModels[name] != nullptr) {
                name = newModel->name + "-" + std::to_string(cnt++);
            }
            newModel->name = name;
            newModel->GetModelXml()->DeleteAttribute("name");
            newModel->GetModelXml()->AddAttribute("name", name);
            newModel->AddOffset(0.02, 0.02);
            newModel->UpdateXmlWithScale();
            xlights->AllModels.AddModel(newModel);
            
            xlights->UpdateModelsList();
            xlights->UnsavedRgbEffectsChanges = true;
            wxTheClipboard->Close();
            SelectModel(name);
        }
    }
}
