#ifndef PIANOPANEL_H
#define PIANOPANEL_H

//(*Headers(PianoPanel)
#include <wx/panel.h>
class wxCheckBox;
class wxTextCtrl;
class wxNotebookEvent;
class wxNotebook;
class wxStaticText;
class wxSlider;
class wxBitmapButton;
class wxFlexGridSizer;
class wxButton;
class wxChoice;
//*)

class PianoPanel: public wxPanel
{
	public:

		PianoPanel(wxWindow* parent);
		virtual ~PianoPanel();

        wxString defaultDir;
    
		//(*Declarations(PianoPanel)
		wxBitmapButton* BitmapButton_Piano_ShapeFilename;
		wxChoice* Choice_Piano_Style;
		wxStaticText* StaticText15_layout_kludge_1;
		wxPanel* Panel5;
		wxCheckBox* CheckBox_Piano_Clipping;
		wxPanel* Panel4;
		wxStaticText* StaticText_Piano_NumKeys;
		wxSlider* Slider_Piano_NumKeys;
		wxStaticText* StaticText1;
		wxStaticText* StaticText_Piano_NumRows;
		wxButton* Button_Piano_MapFilename;
		wxBitmapButton* BitmapButton_Piano_Style;
		wxPanel* Panel3;
		wxButton* Button_Piano_CueFilename;
		wxStaticText* StaticText21;
		wxBitmapButton* BitmapButton_Piano_MapFilename;
		wxTextCtrl* TextCtrl_Piano_MapFilename;
		wxTextCtrl* TextCtrl_Piano_CueFilename;
		wxStaticText* StaticText14_layout_kludge2;
		wxTextCtrl* TextCtrl_Piano_ShapeFilename;
		wxBitmapButton* BitmapButton_Piano_CueFilename;
		wxButton* Button_Piano_ShapeFilename;
		wxNotebook* Notebook2;
		wxSlider* Slider_Piano_NumRows;
		wxBitmapButton* BitmapButton_Piano_Clipping;
		wxBitmapButton* BitmapButton_Piano_NumKeys;
		wxBitmapButton* BitmapButton_Piano_NumRows;
		wxChoice* Choice_Piano_KeyPlacement;
		wxBitmapButton* BitmapButton_Piano_KeyPlacement;
		//*)

	protected:

		//(*Identifiers(PianoPanel)
		static const long ID_STATICTEXT1;
		static const long ID_CHOICE_Piano_Style;
		static const long ID_BITMAPBUTTON_SLIDER_Piano_Style;
		static const long ID_STATICTEXT_Piano_NumKeys;
		static const long ID_SLIDER_Piano_NumKeys;
		static const long ID_BITMAPBUTTON_Piano_NumKeys;
		static const long ID_STATICTEXT_Piano_NumRows;
		static const long ID_SLIDER_Piano_KeyWidth;
		static const long ID_BITMAPBUTTON_Piano_NumRows;
		static const long ID_STATICTEXT20;
		static const long ID_CHOICE_PianoPlacement;
		static const long ID_BITMAPBUTTON_Piano_KeyPlacement;
		static const long ID_BUTTON_Piano_CueFilename;
		static const long ID_BITMAPBUTTON_Piano_Filename;
		static const long ID_STATICTEXT14;
		static const long ID_TEXTCTRL_Piano_CueFilename;
		static const long ID_PANEL9;
		static const long ID_BUTTON1;
		static const long ID_BITMAPBUTTON_Piano_MapFilename;
		static const long ID_STATICTEXT13;
		static const long ID_TEXTCTRL_Piano_MapFilename;
		static const long ID_PANEL11;
		static const long ID_BUTTON2;
		static const long ID_BITMAPBUTTON_Piano_ShapeFilename;
		static const long ID_CHECKBOX1;
		static const long ID_BITMAPBUTTON_Piano_Clipping;
		static const long ID_TEXTCTRL_Piano_ShapeFilename;
		static const long ID_PANEL13;
		static const long ID_NOTEBOOK2;
		//*)

	public:

		//(*Handlers(PianoPanel)
    void UpdateLinkedSliderFloat(wxCommandEvent& event);
    void UpdateLinkedTextCtrlFloat(wxScrollEvent& event);
    void UpdateLinkedTextCtrl360(wxScrollEvent& event);
    void UpdateLinkedSlider360(wxCommandEvent& event);
    void UpdateLinkedTextCtrl(wxScrollEvent& event);
    void UpdateLinkedSlider(wxCommandEvent& event);
    void OnLockButtonClick(wxCommandEvent& event);
    
    void OnPiano_StyleSelect(wxCommandEvent& event);
    void OnSlider_Piano_NumKeysCmdScroll(wxScrollEvent& event);
    void OnTextCtrl_Piano_FilenameText(wxCommandEvent& event);
    void OnButton_Piano_FilenameClick(wxCommandEvent& event);
    void OnSlider_Piano_KeyWidthCmdScroll(wxScrollEvent& event);
    void OnButton_Piano_CueFilenameClick(wxCommandEvent& event);
    void OnButton_Piano_MapFilenameClick(wxCommandEvent& event);
    void OnButton_Piano_ShapeFilenameClick(wxCommandEvent& event);
    void OnTextCtrl_Piano_CueFilenameText(wxCommandEvent& event);
    void OnTextCtrl_Piano_MapFilenameText(wxCommandEvent& event);
    void OnTextCtrl_Piano_ShapeFilenameText(wxCommandEvent& event);
    void OnSlider_Piano_NumRowsCmdScroll(wxScrollEvent& event);

		//*)

		DECLARE_EVENT_TABLE()
};

#endif
