#include "gpPointLegend.h"

mpPointLegend::mpPointLegend()
    : mpInfoLayer()
{
}

mpPointLegend::mpPointLegend(wxRect rect, const wxBrush* brush)
    : mpInfoLayer(rect, brush)
{
}

mpPointLegend::~mpPointLegend()
{
}

void mpPointLegend::UpdateInfo(mpWindow& w, wxEvent& event)
{
}

void mpPointLegend::Plot(wxDC& dc, mpWindow& w)
{
    if(m_visible) {
        // Adjust relative position inside the window
        int scrx = w.GetScrX();
        int scry = w.GetScrY();
        if((m_winX != scrx) || (m_winY != scry)) {
#ifdef MATHPLOT_DO_LOGGING
            // wxLogMessage(_("mpInfoLayer::Plot() screen size has changed from %d x %d to %d x %d"), m_winX, m_winY,
            // scrx, scry);
#endif
            if(m_winX != 1)
                m_dim.x = (int)floor((double)(m_dim.x * scrx / m_winX));
            if(m_winY != 1) {
                m_dim.y = (int)floor((double)(m_dim.y * scry / m_winY));
                UpdateReference();
            }
            // Finally update window size
            m_winX = scrx;
            m_winY = scry;
        }

        dc.SetBrush(m_brush);
        dc.SetFont(m_font);
        dc.SetPen(m_pen);
        dc.SetTextForeground(m_text);

        int tmpX = 0;
        int tmpY = 0;

        mpLayer* ly = nullptr;
        mpPointLayer* point = nullptr;
        int cLayers = w.CountAllLayers();
        int cLayersPoint = 0;

        wxCoord maxX = 0.0;
        wxCoord maxY = 0.0;

        dc.GetMultiLineTextExtent(wxT("-9999.99"), &maxX, &maxY);

        double x = 0.0;
        double y = 0.0;

        const double centreX = maxX / 2.0;
        const double centreY = maxY / 2.0;

        const double margin = 5.0;
        const double align = 10.0;

        for(unsigned int i = 0, j = 0; i < cLayers; i++) {
            ly = w.GetLayer(i);
            if(ly != nullptr && ly->GetLayerType() == mpLayer_POINT && ly->IsVisible() == true)
                cLayersPoint++;
        }

        m_dim.height = cLayersPoint * maxY;
        m_dim.width = margin + 4 * 2 + align + maxX + align + maxX + align + maxX + margin;

        dc.DrawRectangle(m_dim.x, m_dim.y, m_dim.width, m_dim.height);

        for(unsigned int i = 0, j = 0; i < cLayers; i++) {

            ly = w.GetLayer(i);
            if(ly != nullptr && ly->GetLayerType() == mpLayer_POINT && ly->IsVisible() == true) {

                point = dynamic_cast<mpPointLayer*>(ly);
                if(point != nullptr) {

                    wxString name = point->GetName();

                    point->GetPosition(x, y);
                    wxString valueX = wxString::Format(wxT("%.2f"), x);
                    wxString valueY = wxString::Format(wxT("%.2f"), y);

                    dc.SetPen(ly->GetPen());
                    dc.SetBrush(ly->GetBrush());

                    dc.DrawCircle(m_dim.x + margin + 4, m_dim.y + centreY + maxY * j, 4);

                    dc.GetMultiLineTextExtent(name, &tmpX, &tmpY);
                    dc.DrawLabel(name, wxRect(m_dim.x + margin + 4 * 2 + align, m_dim.y + maxY * j, tmpX, tmpY));

                    dc.GetMultiLineTextExtent(valueX, &tmpX, &tmpY);
                    dc.DrawLabel(valueX, wxRect(m_dim.x + margin + 4 * 2 + align + maxX + align, m_dim.y + maxY * j, tmpX, tmpY));

                    dc.GetMultiLineTextExtent(valueY, &tmpX, &tmpY);
                    dc.DrawLabel(valueY, wxRect(m_dim.x + margin + 4 * 2 + align + maxX + align + maxX + align, m_dim.y + maxY * j, tmpX, tmpY));

                    j++;
                }
            }
        }
    }
}
