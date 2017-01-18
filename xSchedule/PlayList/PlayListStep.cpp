#include "PlayListStep.h"

#include <wx/xml/xml.h>
#include <log4cpp/Category.hh>
#include "PlayListItemVideo.h"
#include "PlayListItemImage.h"
#include "PlayListItemESEQ.h"
#include "PlayListItemFSEQ.h"
#include "PlayListItemPJLink.h"
#include "PlayListItemAllOff.h"
#include "PlayListItemDelay.h"
#include "PlayListItemRunProcess.h"
#include "PlayListItemRunCommand.h"

int __playliststepid = 0;

PlayListStep::PlayListStep(wxXmlNode* node)
{
    _loops = -1;
    _pause = 0;
    _suspend = 0;
    _id = __playliststepid++;
    _startTime = 0;
    _framecount = 0;
    _excludeFromRandom = false;
    _lastSavedChangeCount = 0;
    _changeCount = 0;
    Load(node);
}

bool compare_priority(const PlayListItem* first, const PlayListItem* second)
{
    return first->GetPriority() > second->GetPriority();
}

PlayListStep::PlayListStep()
{
    _loops = -1;
    _pause = 0;
    _suspend = 0;
    _id = __playliststepid++;
    _startTime = 0;
    _framecount = 0;
    _name = "";
    _lastSavedChangeCount = 0;
    _changeCount = 0;
    _excludeFromRandom = false;
}

PlayListStep::PlayListStep(const PlayListStep& step)
{
    _loops = step._loops;
    _framecount = step._framecount;
    _name = step._name;
    _lastSavedChangeCount = step._lastSavedChangeCount;
    _changeCount = step._changeCount;
    _excludeFromRandom = step._excludeFromRandom;
    _id = step._id;
    for (auto it = step._items.begin(); it != step._items.end(); ++it)
    {
        _items.push_back((*it)->Copy());
    }
    _items.sort(compare_priority);
}

void PlayListStep::ClearDirty()
{
    _lastSavedChangeCount = _changeCount;

    for (auto it = _items.begin(); it != _items.end(); ++it)
    {
        (*it)->ClearDirty();
    }
}

PlayListStep::~PlayListStep()
{
    while (_items.size() > 0)
    {
        auto toremove = _items.front();
        _items.remove(toremove);
        delete toremove;
    }
}

wxXmlNode* PlayListStep::Save()
{
    wxXmlNode* res = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, "PlayListStep");

    res->AddAttribute("Name", _name);
    if (_excludeFromRandom)
    {
        res->AddAttribute("ExcludeRandom", "TRUE");
    }

    for (auto it = _items.begin(); it != _items.end(); ++it)
    {
        res->AddChild((*it)->Save());
    }

    return res;
}

void PlayListStep::Load(wxXmlNode* node)
{
    _name = node->GetAttribute("Name", "");
    _excludeFromRandom = node->GetAttribute("ExcludeRandom", "FALSE") == "TRUE";

    for (wxXmlNode* n = node->GetChildren(); n != nullptr; n = n->GetNext())
    {
        if (n->GetName() == "PLIVideo")
        {
            _items.push_back(new PlayListItemVideo(n));
        }
        else if (n->GetName() == "PLIFSEQ")
        {
            _items.push_back(new PlayListItemFSEQ(n));
        }
        else if (n->GetName() == "PLIPJLink")
        {
            _items.push_back(new PlayListItemPJLink(n));
        }
        else if (n->GetName() == "PLIESEQ")
        {
            _items.push_back(new PlayListItemESEQ(n));
        }
        else if (n->GetName() == "PLIImage")
        {
            _items.push_back(new PlayListItemImage(n));
        }
        else if (n->GetName() == "PLIAllSet")
        {
            _items.push_back(new PlayListItemAllOff(n));
        }
        else if (n->GetName() == "PLIDelay")
        {
            _items.push_back(new PlayListItemDelay(n));
        }
        else if (n->GetName() == "PLIProcess")
        {
            _items.push_back(new PlayListItemRunProcess(n));
        }
        else if (n->GetName() == "PLICommand")
        {
            _items.push_back(new PlayListItemRunCommand(n));
        }
    }

    _items.sort(compare_priority);
}

bool PlayListStep::IsDirty() const
{
    bool res = _lastSavedChangeCount != _changeCount;

    auto it = _items.begin();
    while (!res && it != _items.end())
    {
        res = res || (*it)->IsDirty();
        ++it;
    }

    return res;
}

void PlayListStep::RemoveItem(PlayListItem* item)
{
    _items.remove(item);
    _changeCount++;
}

std::string PlayListStep::GetName() const
{
    if (_name != "") return _name;

    if (_items.size() == 0) return "<unnamed>";

    return _items.front()->GetNameNoTime();
}

std::string PlayListStep::GetNameNoTime() const
{
    if (_name != "") return _name;

    if (_items.size() == 0) return "<unnamed>";

    return _items.front()->GetNameNoTime();
}

PlayListItem* PlayListStep::GetTimeSource(size_t &ms) const
{
    PlayListItem* timesource = nullptr;
    ms = 9999;
    for (auto it = _items.begin(); it != _items.end(); ++it)
    {
        if ((*it)->ControlsTiming())
        {
            if (timesource == nullptr)
            {
                timesource = *it;
                ms = (*it)->GetFrameMS();
            }
            else
            {
                if ((*it)->GetPriority() > timesource->GetPriority())
                {
                    timesource = *it;
                    ms = (*it)->GetFrameMS();
                }
            }
        }
        else if (timesource == nullptr)
        {
            if ((*it)->GetFrameMS() != 0 && (*it)->GetFrameMS() < ms)
            {
                ms = (*it)->GetFrameMS();
            }
        }
           
    }

    if (ms == 9999)
    {
        ms = 50;
    }

    return timesource;
}

bool PlayListStep::Frame(wxByte* buffer, size_t size)
{
    size_t msPerFrame = 1000;
    PlayListItem* timesource = GetTimeSource(msPerFrame);

    if (msPerFrame == 0)
    {
        msPerFrame = 50;
    }

    size_t frameMS;
    if (timesource != nullptr)
    {
        frameMS = timesource->GetPositionMS();
    }
    else
    {
        frameMS = wxGetUTCTimeMillis().GetLo() - _startTime;
    }

    for (auto it = _items.begin(); it != _items.end(); ++it)
    {
        (*it)->Frame(buffer, size, frameMS, msPerFrame);
    }

    if (timesource != nullptr)
    {
        return timesource->Done();
    }
    else
    {
        return frameMS >= GetLengthMS();
    }
}

size_t PlayListStep::GetFrameMS() const
{
    size_t ms = 0;
    PlayListItem* timesource = GetTimeSource(ms);

    return ms;
}

void PlayListStep::Start(int loops)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.info("Playlist step %s starting.", (const char*)GetNameNoTime().c_str());

    _loops = loops;
    _startTime = wxGetUTCTimeMillis().GetLo();
    for (auto it = _items.begin(); it != _items.end(); ++it)
    {
        (*it)->Start();
    }
}

void PlayListStep::Pause(bool pause)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    if (pause)
    {
        _pause = wxGetUTCTimeMillis().GetLo();
        logger_base.info("Playlist step %s pausing.", (const char*)GetNameNoTime().c_str());
    }
    else
    {
        logger_base.info("Playlist step %s unpausing.", (const char*)GetNameNoTime().c_str());
        _pause = 0;
    }

    for (auto it = _items.begin(); it != _items.end(); ++it)
    {
        (*it)->Pause(pause);
    }
}

void PlayListStep::Suspend(bool suspend)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    if (!IsPaused())
    {
        if (suspend)
        {
            _suspend = wxGetUTCTimeMillis().GetLo();
            logger_base.info("Playlist step %s suspending.", (const char*)GetNameNoTime().c_str());
        }
        else
        {
            logger_base.info("Playlist step %s unsuspending.", (const char*)GetNameNoTime().c_str());
            _suspend = 0;
        }
    }

    for (auto it = _items.begin(); it != _items.end(); ++it)
    {
        (*it)->Suspend(suspend);
    }
}

void PlayListStep::Restart()
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.info("Playlist step %s restarting.", (const char*)GetNameNoTime().c_str());

    _startTime = wxGetUTCTimeMillis().GetLo();
    for (auto it = _items.begin(); it != _items.end(); ++it)
    {
        (*it)->Restart();
    }
}

void PlayListStep::Stop()
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.info("Playlist step %s stopping.", (const char*)GetNameNoTime().c_str());

    for (auto it = _items.begin(); it != _items.end(); ++it)
    {
        (*it)->Stop();
    }
}

size_t PlayListStep::GetPosition() const
{
    size_t msPerFrame = 1000;
    PlayListItem* timesource = GetTimeSource(msPerFrame);

    size_t frameMS;
    if (timesource != nullptr)
    {
        frameMS = timesource->GetPositionMS();
    }
    else
    {
        auto now = wxGetUTCTimeMillis().GetLo();
        if (_pause == 0)
        {
            if (_suspend == 0)
            {
                frameMS = now - _startTime;
            }
            else
            {
                frameMS = now - _startTime - (now - _suspend);
            }
        }
        else
        {
            frameMS = now - _startTime - (now - _pause);
        }
    }

    return frameMS;
}

size_t PlayListStep::GetLengthMS() const
{
    size_t msPerFrame = 1000;
    PlayListItem* timesource = GetTimeSource(msPerFrame);
    if (timesource != nullptr)
    {
        return timesource->GetDurationMS();
    }
    else
    {
        size_t len = 0;
        for (auto it = _items.begin(); it != _items.end(); ++it)
        {
            len = std::max(len, (*it)->GetDurationMS());
        }

        if (len == 0)
        {
            for (auto it = _items.begin(); it != _items.end(); ++it)
            {
                len = std::max(len, (*it)->GetDurationMS(msPerFrame));
            }

            if (len == 0) len = msPerFrame;
        }

        return len;
    }
}

void PlayListStep::AdjustTime(wxTimeSpan by)
{
    _startTime += by.GetValue().GetLo();
}

std::string PlayListStep::FormatTime(size_t timems, bool ms) const
{
    if (ms)
    {
        return wxString::Format(wxT("%i:%02i.%03i"), timems / 60000, (timems % 60000) / 1000, timems % 1000).ToStdString();
    }
    else
    {
        return wxString::Format(wxT("%i:%02i"), timems / 60000, (timems % 60000) / 1000).ToStdString();
    }
}

std::string PlayListStep::GetStatus(bool ms) const
{
    std::string fps = "Unknown";

    if (GetFrameMS() > 0)
    {
        fps = wxString::Format(wxT("%i"), 1000 / GetFrameMS()).ToStdString();
    }
    
    return "Time: " + FormatTime(GetPosition(), ms) + " Left: " + FormatTime(GetLengthMS() - GetPosition(), ms) + " FPS: " + fps;
}

std::list<PlayListItem*> PlayListStep::GetItems()
{
    _items.sort(compare_priority);
    return _items;
}
