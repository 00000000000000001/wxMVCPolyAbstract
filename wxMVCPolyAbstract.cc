#include <wx/wx.h>
#include <vector>

wxString strToWxStr(std::string const& str)
{
    wxString res(str.c_str(), wxConvUTF8);
    return res;
}

std::string wxStrToStr(wxString const& str)
{
    std::string res = std::string(str.mb_str());
    return res;
}

class View;

class AbstractPublisher 
{
	std::vector<View*> views_{};
public:
	void addView(View* view);
	void update();
};

class Model: public AbstractPublisher
{
    std::string str_{};
    View* view_{};
public:
    void setValue(const std::string& str);
    std::string& getValue();
};

// View
class View
{
protected:
    Model* model_{};
public:
    virtual void update() = 0;
    void subscribe(Model* m);
};

// Controller
class Controller : public wxApp
{
    Model* model_;
public:
    bool OnInit();
    void setValue(const std::string& str);
};

// View
void View::subscribe(Model* model)
{
    model_ = model;
    model->addView(this);
    update();
}

// DerivedView
class DerivedView: public wxFrame, public View
{
    std::string str_;
    wxTextCtrl* tc_;
    wxStaticText* st_;
    wxButton* b_;
public: 
    DerivedView(): wxFrame((wxFrame *)NULL, -1,  _("wxMVC"))
    {
        tc_ = new wxTextCtrl(this, wxID_ANY, _(""));
        st_ = new wxStaticText(this, wxID_ANY, _(""));
        b_ = new wxButton(this, wxID_ANY, _("set value"));

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(tc_, 1, wxEXPAND);
        sizer->Add(st_, 1, wxEXPAND);
        sizer->Add(b_, 1, wxEXPAND);

        Bind(wxEVT_BUTTON, &DerivedView::OnClick, this);

        SetSizerAndFit(sizer);
        Centre();
        Show();
    }
    
    void update();
    void OnClick(wxCommandEvent& event);
};

// AbstractPublisher
void AbstractPublisher::addView(View* view)
{
    views_.push_back(view);
}

void AbstractPublisher::update()
{   // update all views
    unsigned long n{views_.size()};
    for (unsigned long i = 0; i < n; ++i)
    {
        views_[i]->update();
    }
}

// Model
void Model::setValue(const std::string& str)
{
    str_ = str;
    update();
}

std::string& Model::getValue()
{
    return str_;
}

// DerivedView
void DerivedView::update()
{
    str_ = model_->getValue();
    st_->SetLabel(strToWxStr(str_));
}

void DerivedView::OnClick(wxCommandEvent& event)
{
    model_->setValue(wxStrToStr(tc_->GetValue()));
}

// Controller
void Controller::setValue(const std::string& str)
{
    model_->setValue(str);
}
bool Controller::OnInit()
{
    model_ = new Model();
	DerivedView* view = new DerivedView();
    // DerivedView2* view2 = new View();

    view->subscribe(model_);
	model_->addView(view);

    return true;
}

wxIMPLEMENT_APP(Controller);