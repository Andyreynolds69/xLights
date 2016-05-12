#include "TimeLine.h"
#include "Waveform.h"
#include "wx/wx.h"
#include "wx/brush.h"
#include "../xLightsMain.h"

wxDEFINE_EVENT(EVT_TIME_LINE_CHANGED, wxCommandEvent);

BEGIN_EVENT_TABLE(TimeLine, wxWindow)
EVT_MOTION(TimeLine::mouseMoved)
EVT_LEFT_DOWN(TimeLine::mouseLeftDown)
EVT_LEFT_UP(TimeLine::mouseLeftUp)
EVT_MOUSE_CAPTURE_LOST(TimeLine::OnLostMouseCapture)
//EVT_LEAVE_WINDOW(TimeLine::mouseLeftWindow)
//EVT_RIGHT_DOWN(TimeLine::rightClick)
//EVT_KEY_DOWN(TimeLine::keyPressed)
//EVT_KEY_UP(TimeLine::keyReleased)
//EVT_MOUSEWHEEL(TimeLine::mouseWheelMoved)
EVT_PAINT(TimeLine::render)
END_EVENT_TABLE()

// These values are used to calculate zoom
// 1    =   1*(1/frequency) per major tick on the scale
// 200  = 200*(1/frequency) per major tick on the scale
// etc....
const int TimeLine::ZoomLevelValues[] = {1,2,4,10,20,40,100,200,400,600,1200,2400,4800};

static const int marker_size = 8;

void TimeLine::OnLostMouseCapture(wxMouseCaptureLostEvent& event)
{
    m_dragging = false;
}

void TimeLine::mouseLeftDown( wxMouseEvent& event)
{
    mCurrentPlayMarkerStart = GetPositionFromSelection(event.GetX());
    mCurrentPlayMarkerStartMS = GetAbsoluteTimeMSfromPosition(mCurrentPlayMarkerStart);
    mCurrentPlayMarkerEnd = -1;
    mCurrentPlayMarkerEndMS = -1;
    if( !m_dragging )
    {
        m_dragging = true;
        CaptureMouse();
    }
    SetFocus();
    Refresh(false);
}

void TimeLine::mouseMoved( wxMouseEvent& event)
{
    if( m_dragging ) {
        mCurrentPlayMarkerEndMS = GetAbsoluteTimeMSfromPosition(event.GetX());
        if( mCurrentPlayMarkerEndMS < mStartTimeMS ) {
            mCurrentPlayMarkerEndMS = mStartTimeMS;
        }
        mCurrentPlayMarkerEnd = GetPositionFromTimeMS(mCurrentPlayMarkerEndMS);
        Refresh(false);
    }
}

void TimeLine::mouseLeftUp( wxMouseEvent& event)
{
    triggerPlay();
    if(m_dragging)
    {
        ReleaseMouse();
        m_dragging = false;
    }
    Refresh(false);
}

void TimeLine::triggerPlay()
{
    timeline_initiated_play = true;
    if( mCurrentPlayMarkerEndMS != -1 && mCurrentPlayMarkerStartMS > mCurrentPlayMarkerEndMS )
    {
        std::swap(mCurrentPlayMarkerStart, mCurrentPlayMarkerEnd);
        std::swap(mCurrentPlayMarkerStartMS, mCurrentPlayMarkerEndMS);
    }
    wxCommandEvent playEvent(EVT_PLAY_SEQUENCE);
    wxPostEvent(this, playEvent);
}

int TimeLine::GetPositionFromSelection(int position)
{
    int time = GetAbsoluteTimeMSfromPosition(position);
    time = RoundToMultipleOfPeriod(time,mFrequency);     // Round to nearest period
    return GetPositionFromTimeMS(time); // Recalulate Position with corrected time
}

TimeLine::TimeLine(wxPanel* parent, wxWindowID id, const wxPoint &pos, const wxSize &size,long style, const wxString &name):
                   wxWindow((wxWindow*)parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    mParent = (wxPanel*)parent;
    DOUBLE_BUFFER(this);
    mIsInitialized = false;
    mZoomLevel = 0;
    mFrequency = 40;
    m_dragging = false;
    // most variable initialization is done in the Initialize() function
}

TimeLine::~TimeLine()
{
}

void TimeLine::CheckNeedToScrollToPlayStart(bool paused)
{
    int marker = paused ? mCurrentPlayMarker : mCurrentPlayMarkerStart;
    int StartTime;
    int EndTime;
    GetViewableTimeRange(StartTime, EndTime);
    int scroll_start = GetPositionFromTimeMS(StartTime);
    int scroll_end = GetPositionFromTimeMS(EndTime);
    if(marker < scroll_start || marker > scroll_end)
    {
        int markerMS = paused ? mCurrentPlayMarkerMS : mCurrentPlayMarkerStartMS;
        int new_start_time = markerMS - 1000;
        if( new_start_time < 0 )
        {
            new_start_time = 0;
        }
        SetStartTimeMS(new_start_time);
        RaiseChangeTimeline();
    }
}

void TimeLine::RaiseChangeTimeline()
{
    Refresh();
    Update();
    TimelineChangeArguments *tla = new TimelineChangeArguments(mZoomLevel,mStartPixelOffset,mCurrentPlayMarker);
    wxCommandEvent eventTimeLineChanged(EVT_TIME_LINE_CHANGED);
    eventTimeLineChanged.SetClientData((void*)tla);
    eventTimeLineChanged.SetInt(0);
    wxPostEvent(mParent, eventTimeLineChanged);
}

void TimeLine::SetSequenceEnd(int ms)
{
    mSequenceEndMarkerMS = ms;
    mSequenceEndMarker = GetPositionFromTimeMS(ms);
    mEndPos = GetPositionFromTimeMS(mEndTimeMS);
    Refresh();
    Update();
}

bool TimeLine::SetPlayMarkerMS(int ms)
{
    mCurrentPlayMarkerMS = ms;
    bool changed = false;
    if( ms < mStartTimeMS ) {
        if (mCurrentPlayMarker != -1) {
            changed = true;
        }
        mCurrentPlayMarker = -1;
    } else {
        int i = GetPositionFromTimeMS(ms);
        if (mCurrentPlayMarker != i) {
            changed = true;
        }
        mCurrentPlayMarker = i;
    }
    if (changed) {
        wxClientDC dc(this);
        render(dc);
    }
    return changed;
}

void TimeLine::SetZoomMarkerMS(int ms)
{
    mZoomMarkerMS = ms;
}

int TimeLine::GetPlayMarker()
{
    return mCurrentPlayMarker;
}

void TimeLine::SetSelectedPositionStart(int pos, bool reset_end)
{
    mSelectedPlayMarkerStart = GetPositionFromSelection(pos);
    mSelectedPlayMarkerStartMS = GetAbsoluteTimeMSfromPosition(mSelectedPlayMarkerStart);
    if( reset_end )
    {
        mSelectedPlayMarkerEnd = -1;
        mSelectedPlayMarkerEndMS = -1;
    }
    mZoomMarkerMS = mSelectedPlayMarkerStartMS;
    Refresh(false);
}

void TimeLine::SetSelectedPositionStartMS(int time)
{
    mSelectedPlayMarkerStartMS = time;
    mSelectedPlayMarkerStart = GetPositionFromTimeMS(mSelectedPlayMarkerStartMS);
    mSelectedPlayMarkerEnd = -1;
    mSelectedPlayMarkerEndMS = -1;
    mZoomMarkerMS = mSelectedPlayMarkerStartMS;
    Refresh(false);
}

void TimeLine::SetSelectedPositionEnd(int pos)
{
    mSelectedPlayMarkerEndMS = GetAbsoluteTimeMSfromPosition(pos);
    if( mSelectedPlayMarkerEndMS < mStartTimeMS ) {
        mSelectedPlayMarkerEndMS = mStartTimeMS;
    }
    mSelectedPlayMarkerEnd = GetPositionFromTimeMS(mSelectedPlayMarkerEndMS);
    Refresh(false);
}

void TimeLine::SetMousePositionMS(int ms)
{
    mMousePositionMS = ms;
    if( ms < mStartTimeMS ) {
        mMousePosition = -1;
    } else {
        mMousePosition = GetPositionFromTimeMS(ms);
    }
    Refresh(false);
}

void TimeLine::LatchSelectedPositions()
{
    if( mSelectedPlayMarkerEndMS != -1 && mSelectedPlayMarkerStartMS > mSelectedPlayMarkerEndMS )
    {
        std::swap(mSelectedPlayMarkerStart, mSelectedPlayMarkerEnd);
        std::swap(mSelectedPlayMarkerStartMS, mSelectedPlayMarkerEndMS);
    }
}

// signal the start of play so timeline can adjust marks
void TimeLine::PlayStarted()
{
    if( !timeline_initiated_play )
    {
        mCurrentPlayMarkerStart = mSelectedPlayMarkerStart;
        mCurrentPlayMarkerEnd = mSelectedPlayMarkerEnd;
        mCurrentPlayMarkerStartMS = mSelectedPlayMarkerStartMS;
        mCurrentPlayMarkerEndMS = mSelectedPlayMarkerEndMS;
    }
    CheckNeedToScrollToPlayStart();
    timeline_initiated_play = false;
    Refresh(false);
}

// signal play stop so timeline can adjust marks
void TimeLine::PlayStopped()
{
    mCurrentPlayMarkerStart = -1;
    mCurrentPlayMarkerEnd = -1;
    mCurrentPlayMarker = -1;
    mCurrentPlayMarkerStartMS = -1;
    mCurrentPlayMarkerEndMS = -1;
    mCurrentPlayMarkerMS = -1;
    Refresh(false);
}

// return the time where to begin playing
int TimeLine::GetNewStartTimeMS()
{
    int time = 0;
    if( timeline_initiated_play ) {
        time = mCurrentPlayMarkerStartMS;
    } else {
        if( mSelectedPlayMarkerStartMS > 0 ) {
            time = mSelectedPlayMarkerStartMS;
        }
    }
    return time;
}

// return the time where to end playing
int TimeLine::GetNewEndTimeMS()
{
    int time = -1;
    if( timeline_initiated_play )
    {
        if( mCurrentPlayMarkerEndMS >= 0 )
        {
            time = mCurrentPlayMarkerEndMS;
        }
    }
    else
    {
        if( mSelectedPlayMarkerEndMS >= 0 )
        {
            time = mSelectedPlayMarkerEndMS;
        }
    }
    return time;
}

void TimeLine::SetStartTimeMS(int time)
{
    mStartTimeMS = time;
    mEndTimeMS = GetMaxViewableTimeMS();
    mStartPixelOffset = GetPixelOffsetFromStartTime();
    RecalcMarkerPositions();
}

float TimeLine::GetStartTimeMS()
{
    return mStartTimeMS;
}

void TimeLine::SetStartPixelOffset(int offset)
{
    mStartPixelOffset = offset;
    mStartTimeMS = GetFirstTimeLabelFromPixelOffset(mStartPixelOffset);
    mEndTimeMS = GetMaxViewableTimeMS();
}

void TimeLine::ResetMarkers(int ms)
{
    mCurrentPlayMarkerEnd = -1;
    mCurrentPlayMarkerEndMS = -1;
    mSelectedPlayMarkerStartMS = ms;
    mSelectedPlayMarkerStart = GetPositionFromTimeMS(ms);
    mSelectedPlayMarkerEnd = -1;
    mSelectedPlayMarkerEndMS = -1;
    mCurrentPlayMarker = -1;
    mCurrentPlayMarkerMS = -1;
    mCurrentPlayMarkerStart = mSelectedPlayMarkerStart;
    mCurrentPlayMarkerStartMS = mSelectedPlayMarkerStartMS;
    mZoomMarkerMS = mStartTimeMS + (mEndTimeMS - mStartTimeMS)/2;
    mMousePositionMS = -1;
    mMousePosition = -1;
}

int TimeLine::GetStartPixelOffset()
{
    return  mStartPixelOffset;
}

float TimeLine::GetFirstTimeLabelFromPixelOffset(int offset)
{
    if (offset == 0)
    {
        return 0;
    }
    else
    {
        return (float)((offset/PIXELS_PER_MAJOR_HASH)+1) * (float)ZoomLevelValues[mZoomLevel]/(float)(mFrequency);
    }
}

void TimeLine::SetTimeFrequency(int frequency)
{
    mFrequency = frequency;
}

int TimeLine::GetTimeFrequency()
{
    return  mFrequency;
}

void TimeLine::SetZoomLevel(int level)
{
    mZoomLevel = level;
    if( (mZoomMarkerMS != -1) && ((mEndTimeMS - mStartTimeMS) > 0) )
    {
        float marker_ratio = (double)(mZoomMarkerMS - mStartTimeMS) / (double)(mEndTimeMS - mStartTimeMS);
        int total_ms = GetTotalViewableTimeMS();
        mStartTimeMS = mZoomMarkerMS - marker_ratio * total_ms;
        if( mStartTimeMS < 0 )
        {
            mStartTimeMS = 0;
        }
    }
    else
    {
        mStartTimeMS = 0;
    }
    mEndTimeMS = GetMaxViewableTimeMS();
    mStartPixelOffset = GetPixelOffsetFromStartTime();
    RecalcMarkerPositions();
    RaiseChangeTimeline();
}

void TimeLine::RecalcMarkerPositions()
{
     if( mCurrentPlayMarkerMS == -1 ) {
        mCurrentPlayMarker = -1;
     } else {
        mCurrentPlayMarker = GetPositionFromTimeMS(mCurrentPlayMarkerMS);
     }
     if( mCurrentPlayMarkerStartMS == -1 ) {
        mCurrentPlayMarkerStart = -1;
     } else {
        mCurrentPlayMarkerStart = GetPositionFromTimeMS(mCurrentPlayMarkerStartMS);
     }
     if( mCurrentPlayMarkerEndMS == -1 ) {
        mCurrentPlayMarkerEnd = -1;
     } else {
        mCurrentPlayMarkerEnd = GetPositionFromTimeMS(mCurrentPlayMarkerEndMS);
     }
     if( mSelectedPlayMarkerStartMS == -1 ) {
        mSelectedPlayMarkerStart = -1;
     } else {
        mSelectedPlayMarkerStart = GetPositionFromTimeMS(mSelectedPlayMarkerStartMS);
     }
     if( mSelectedPlayMarkerEndMS == -1 ) {
        mSelectedPlayMarkerEnd = -1;
     } else {
        mSelectedPlayMarkerEnd = GetPositionFromTimeMS(mSelectedPlayMarkerEndMS);
     }
     mSequenceEndMarker = GetPositionFromTimeMS(mSequenceEndMarkerMS);
     mEndPos = GetPositionFromTimeMS(mEndTimeMS);
}

int TimeLine::GetZoomLevel()
{
    return  mZoomLevel;
}

void TimeLine::ZoomOut()
{
    if(mZoomLevel<mMaxZoomLevel)
    {
        SetZoomLevel(mZoomLevel+1);
        if(GetTotalViewableTimeMS()> mTimeLength)
        {
            mStartTimeMS = 0;
            mStartPixelOffset = 0;
            mEndTimeMS = GetMaxViewableTimeMS();
            mEndPos = GetPositionFromTimeMS(mEndTimeMS);
            mSequenceEndMarker = GetPositionFromTimeMS(mSequenceEndMarkerMS);
            RaiseChangeTimeline();
        }
    }
}

void TimeLine::ZoomIn()
{
    wxString s;
    if(mZoomLevel>0)
    {
        SetZoomLevel(mZoomLevel-1);
    }
}

int TimeLine::GetPixelOffsetFromStartTime()
{
    float nMajorHashs = (float)mStartTimeMS/(float)TimePerMajorTickInMS();
    int offset = nMajorHashs * PIXELS_PER_MAJOR_HASH;
    return offset;
}

int TimeLine::GetPositionFromTimeMS(int timeMS)
{
    double majorHashs = (double)timeMS/(double)TimePerMajorTickInMS();
    double xAbsolutePosition = majorHashs * (double)PIXELS_PER_MAJOR_HASH;
    return (int)(xAbsolutePosition - mStartPixelOffset);
}

void TimeLine::GetPositionsFromTimeRange(int startTimeMS,int endTimeMS,EFFECT_SCREEN_MODE &screenMode,int &x1, int &x2, int& x3, int& x4)
{
    if(startTimeMS < mStartTimeMS && endTimeMS > mEndTimeMS)
    {
        screenMode = SCREEN_L_R_ACROSS;
        x1 = 0;
        x2 = GetSize().x;
        double majorHashs = (double)(startTimeMS - mStartTimeMS)/(double)TimePerMajorTickInMS();
        x3=(int)(majorHashs * (double)PIXELS_PER_MAJOR_HASH);
        majorHashs = (double)(endTimeMS - mStartTimeMS)/(double)TimePerMajorTickInMS();
        x4=(int)(majorHashs * (double)PIXELS_PER_MAJOR_HASH);
    }
    else if(startTimeMS < mStartTimeMS && endTimeMS > mStartTimeMS && endTimeMS <= mEndTimeMS)
    {
        screenMode = SCREEN_R_ON;
        double majorHashs = (double)(endTimeMS - mStartTimeMS)/(double)TimePerMajorTickInMS();
        x1=0;
        x2=(int)(majorHashs * (double)PIXELS_PER_MAJOR_HASH);
        majorHashs = (double)(startTimeMS - mStartTimeMS)/(double)TimePerMajorTickInMS();
        x3=(int)(majorHashs * (double)PIXELS_PER_MAJOR_HASH);
        x4=x2;
    }
    else if(startTimeMS >= mStartTimeMS && startTimeMS < mEndTimeMS && endTimeMS > mEndTimeMS)
    {
        screenMode = SCREEN_L_ON;
        double majorHashs = (double)(startTimeMS - mStartTimeMS)/(double)TimePerMajorTickInMS();
        x1=(int)(majorHashs * (double)PIXELS_PER_MAJOR_HASH);
        x2=GetSize().x;
        majorHashs = (double)(endTimeMS - mStartTimeMS)/(double)TimePerMajorTickInMS();
        x4=(int)(majorHashs * (double)PIXELS_PER_MAJOR_HASH);
        x3=x1;
    }
    else if(startTimeMS >= mStartTimeMS && endTimeMS <= mEndTimeMS)
    {
        screenMode = SCREEN_L_R_ON;
        double majorHashs = (double)(startTimeMS - mStartTimeMS)/(double)TimePerMajorTickInMS();
        x1=(int)(majorHashs * (double)PIXELS_PER_MAJOR_HASH);
        majorHashs = (double)(endTimeMS - mStartTimeMS)/(double)TimePerMajorTickInMS();
        x2=(int)(majorHashs * (double)PIXELS_PER_MAJOR_HASH);
        x3=x1;
        x4=x2;
    }
    else if((startTimeMS < mStartTimeMS && endTimeMS < mStartTimeMS) ||
            (startTimeMS > mStartTimeMS && endTimeMS > mStartTimeMS))
    {
        screenMode = SCREEN_L_R_OFF;
        x1=0;
        x2=0;
        x3=x1;
        x4=x2;
    }
}

void TimeLine::SetTimeLength(int ms)
{
    mTimeLength = ms;
}

int TimeLine::GetTimeLength()
{
    return mTimeLength;
}

int TimeLine::GetTimeMSfromPosition(int position)
{
    float nMajorHashs = (float)position/(float)PIXELS_PER_MAJOR_HASH;
    int time = (int)(nMajorHashs*TimePerMajorTickInMS());
    return time;
}

int TimeLine::GetAbsoluteTimeMSfromPosition(int position)
{
    float nMajorHashs = (float)position/(float)PIXELS_PER_MAJOR_HASH;
    int time = mStartTimeMS + (int)(nMajorHashs*TimePerMajorTickInMS());
    time = RoundToMultipleOfPeriod(time,mFrequency);
    return time;
}

int TimeLine::GetRawTimeMSfromPosition(int position)
{
    float nMajorHashs = (float)position/(float)PIXELS_PER_MAJOR_HASH;
    int time = mStartTimeMS + (int)(nMajorHashs*TimePerMajorTickInMS());
    return time;
}

int TimeLine::GetMaxViewableTimeMS()
{
    float width = (float)GetSize().x;
    float majorTicks = width / (float)PIXELS_PER_MAJOR_HASH;
    return (int)((majorTicks * (float)TimePerMajorTickInMS()) + mStartTimeMS);
}

int TimeLine::GetTotalViewableTimeMS()
{
    float width = (float)GetSize().x;
    float majorTicks = width / (float)PIXELS_PER_MAJOR_HASH;
    return (int)((majorTicks * (float)TimePerMajorTickInMS()));
}

int TimeLine::GetZoomLevelValue()
{
    return  ZoomLevelValues[mZoomLevel];
}

int TimeLine::GetMaxZoomLevel()
{
    int i;
    float width = (float)GetSize().x;
    for(i=0;i<=MAX_ZOOM_OUT_INDEX;i++)
    {
        float majorTicks = width / (float)PIXELS_PER_MAJOR_HASH;
        int timeMS =  (int)((float)ZoomLevelValues[i] * ((float)1000/(float)mFrequency) * majorTicks);
        if(timeMS > mTimeLength)
        {
             break;
        }
    }
    mMaxZoomLevel = i;
    return mMaxZoomLevel;
}

void TimeLine::MoveToLeft(int numberOfPixels)
{

}

void TimeLine::MoveToRight(int numberOfPixels)
{

}

void TimeLine::Initialize()
{
    mIsInitialized = true;
    mStartPixelOffset = 0;
    mZoomLevel = 0;
    mStartTimeMS = 0;
    mEndTimeMS = GetMaxViewableTimeMS();
    mCurrentPlayMarkerStart = -1;
    mCurrentPlayMarkerEnd = -1;
    mSelectedPlayMarkerStart = -1;
    mSelectedPlayMarkerEnd = -1;
    mCurrentPlayMarker = -1;
    mCurrentPlayMarkerStartMS = -1;
    mCurrentPlayMarkerEndMS = -1;
    mSelectedPlayMarkerStartMS = -1;
    mSelectedPlayMarkerEndMS = -1;
    mCurrentPlayMarkerMS = -1;
    timeline_initiated_play = false;
    m_dragging = false;
}

void TimeLine::render( wxPaintEvent& event )
{
    wxPaintDC dc(this);
    render(dc);
}
void TimeLine::render( wxDC& dc ) {
    if (!mIsInitialized)
        return;
    wxCoord w,h;
    float t;
    int labelCount=0;
    wxPen pen(wxColor(128,128,128));
    const wxPen* pen_black = wxBLACK_PEN;
    const wxPen* pen_green = wxGREEN_PEN;
    const wxPen* pen_transparent = wxTRANSPARENT_PEN;
    dc.SetPen(pen);
    dc.GetSize(&w,&h);
    wxBrush brush(wxColor(212,208,200),wxBRUSHSTYLE_SOLID);
    wxBrush brush_range(wxColor(187, 173,193),wxBRUSHSTYLE_SOLID);
    wxBrush brush_past_end(wxColor(153, 204, 255),wxBRUSHSTYLE_CROSSDIAG_HATCH);
    dc.SetBrush(brush);
    dc.DrawRectangle(0,0,w,h+1);

    wxFont f = dc.GetFont();
    f.SetPointSize(7.0);
    dc.SetFont(f);

    // Draw the selection fill if its a range
    if( mSelectedPlayMarkerStart != -1 && mSelectedPlayMarkerEnd != -1 ) {
        int left_pos = std::min(mSelectedPlayMarkerStart, mSelectedPlayMarkerEnd);
        int right_pos = std::max(mSelectedPlayMarkerStart, mSelectedPlayMarkerEnd) - 1;
        dc.SetPen(*pen_transparent);
        dc.SetBrush(brush_range);
        dc.DrawRectangle(left_pos, 0, right_pos - left_pos + 1, h);
    }

    dc.SetBrush(brush);
    dc.SetPen(pen);
    wxString format;
    int minutes=0;
    int seconds=0;
    int subsecs=0;
    wxString sTime;
    for(int x=0;x<w;x++)
    {

        // Draw hash marks
        if ((x+mStartPixelOffset)%(PIXELS_PER_MAJOR_HASH/2)==0)
        {
            dc.DrawLine(x,h - 10,x,h-1);
        }
        // Draw time label
        if((x+mStartPixelOffset)%PIXELS_PER_MAJOR_HASH==0)
        {
            t = GetTimeMSfromPosition(x+mStartPixelOffset);
            wxRect r(x-25,h/2-10,50,12);
            minutes = t/60000;
            seconds = (t-(minutes*60000))/1000;
            subsecs = t - (minutes*60000 + seconds*1000);

            if(minutes > 0)
            {
                if(mFrequency>=40)
                    sTime =  wxString::Format("%d:%02d.%.3d",minutes,seconds,subsecs);
                else
                    sTime =  wxString::Format("%d:%.2d.%.2d",minutes,seconds,subsecs/10);
            }
            else
            {
                if(mFrequency>=40)
                    sTime =  wxString::Format("%2d.%.3d",seconds,subsecs);
                else
                    sTime =  wxString::Format("%2d.%.2d",seconds,subsecs/10);
            }
            labelCount++;
            dc.DrawLabel(sTime,r,wxALIGN_CENTER);
        }
    }

    // draw timeline selection range or point
    if( mCurrentPlayMarkerStart >= 0 )
    {
        int left_pos = mCurrentPlayMarkerStart + 1;
        if( mCurrentPlayMarkerEnd >= 0 && mCurrentPlayMarkerStart != mCurrentPlayMarkerEnd)
        {
            left_pos = std::min(mCurrentPlayMarkerStart, mCurrentPlayMarkerEnd) + 1;
            int right_pos = std::max(mCurrentPlayMarkerStart, mCurrentPlayMarkerEnd) - 1;
            DrawTriangleMarkerFacingRight(dc, right_pos, marker_size, h);
            DrawRectangle(dc, left_pos+marker_size+1, h-marker_size-1, right_pos-marker_size, h-marker_size+1);
        }
        DrawTriangleMarkerFacingLeft(dc, left_pos, marker_size, h);
    }

    // draw green current play arrow
    if( mCurrentPlayMarker >= 0 )
    {
        dc.SetPen(*pen_green);
        int play_start_mark = mCurrentPlayMarker - marker_size;
        int play_end_mark = mCurrentPlayMarker + marker_size;
        int x1, x2, y_bottom = 0;
        for( x1 = play_start_mark, x2 = play_end_mark; x1 <= x2; x1++, x2--, y_bottom++ )
        {
            dc.DrawLine(x1, y_bottom, x2+1, y_bottom);
        }
        dc.SetPen(*pen_black);
        dc.DrawLine(play_start_mark, 0, play_end_mark, 0);
        dc.DrawLine(play_start_mark, 0, mCurrentPlayMarker, y_bottom);
        dc.DrawLine(play_end_mark, 0, mCurrentPlayMarker, y_bottom);
        dc.DrawPoint(mCurrentPlayMarker, y_bottom);
    }

    // Draw the selection line if not a range
    if( mSelectedPlayMarkerStart != -1 && mSelectedPlayMarkerEnd == -1 ) {
        dc.SetPen(*pen_black);
        dc.DrawLine(mSelectedPlayMarkerStart, 0, mSelectedPlayMarkerStart, h-1);
    }

    // grey out where sequence ends
    dc.SetBrush(brush_past_end);
    dc.DrawRectangle(mSequenceEndMarker, 0, mEndPos, h);
}

void TimeLine::DrawTriangleMarkerFacingLeft(wxDC& dc, int& play_start_mark, const int& tri_size, int& height)
{
    const wxPen* pen_black = wxBLACK_PEN;
    const wxPen* pen_grey = wxLIGHT_GREY_PEN;
    int y_top = height-tri_size;
    int y_bottom = y_top;
    int arrow_end = play_start_mark + 1;
    dc.SetPen(*pen_grey);
    for( ; y_bottom < height-1; y_bottom++, y_top--, arrow_end++ )
    {
        dc.DrawLine(arrow_end,y_top,arrow_end,y_bottom);
    }
    dc.SetPen(*pen_black);
    dc.DrawLine(play_start_mark,y_top,play_start_mark,height-1);
    dc.DrawLine(play_start_mark+1,height-tri_size,arrow_end,y_top);
    dc.DrawLine(play_start_mark+1,height-tri_size,arrow_end,y_bottom);
    dc.DrawLine(arrow_end,y_top,arrow_end,y_bottom);
}

void TimeLine::DrawTriangleMarkerFacingRight(wxDC& dc, int& play_start_mark, const int& tri_size, int& height)
{
    const wxPen* pen_black = wxBLACK_PEN;
    const wxPen* pen_grey = wxLIGHT_GREY_PEN;
    int y_top = height-tri_size;
    int y_bottom = y_top;
    int arrow_end = play_start_mark - 1;
    dc.SetPen(*pen_grey);
    for( ; y_bottom < height-1; y_bottom++, y_top--, arrow_end-- )
    {
        dc.DrawLine(arrow_end,y_top,arrow_end,y_bottom);
    }
    dc.SetPen(*pen_black);
    dc.DrawLine(play_start_mark,y_top,play_start_mark,height-1);
    dc.DrawLine(play_start_mark-1,height-tri_size,arrow_end,y_top);
    dc.DrawLine(play_start_mark-1,height-tri_size,arrow_end,y_bottom);
    dc.DrawLine(arrow_end,y_top,arrow_end,y_bottom);
}

void TimeLine::DrawRectangle(wxDC& dc, int x1, int y1, int x2, int y2)
{
    const wxPen* pen_outline = wxMEDIUM_GREY_PEN;
    const wxPen* pen_grey = wxLIGHT_GREY_PEN;
    dc.SetPen(*pen_grey);
    for( int y = y1; y <= y2; y++ )
    {
        dc.DrawLine(x1, y, x2, y);
    }
    dc.SetPen(*pen_outline);
    dc.DrawLine(x1, y1, x2, y1);
    dc.DrawLine(x1, y2, x2, y2);
}

int TimeLine::TimePerMajorTickInMS()
{
    return (int)((double)ZoomLevelValues[mZoomLevel] * ((double)1000.0/(double)mFrequency));
}

void TimeLine::GetViewableTimeRange(int &StartTime, int &EndTime)
{
    StartTime = mStartTimeMS;
    EndTime = mEndTimeMS;
}

TimelineChangeArguments::TimelineChangeArguments(int zoomLevel, int startPixelOffset,int currentTime)
{
    ZoomLevel = zoomLevel;
    StartPixelOffset = startPixelOffset;
    CurrentTimeMS = currentTime;
}

int TimeLine::RoundToMultipleOfPeriod(int number,double frequency)
{
    double period = (double)1/frequency;
    int i = (int)(((double)number/1000.0)/period);
    double d = ((double)number/1000.0)/period;
    if(d-(double)i < .5)
    {
        return (int)(((double)i * period) * 1000.0);
    }
    else
    {
        return (int)(((double)(i+1) * period) * 1000.0);
    }
}

TimelineChangeArguments::~TimelineChangeArguments()
{
}

void TimeLine::RecalcEndTime()
{
    mEndTimeMS = GetMaxViewableTimeMS();
}

