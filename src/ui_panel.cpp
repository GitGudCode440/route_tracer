#include "imgui.h"
#include "ui_panel.hpp"


void UIPanel::ShowUIPanel()
{
    ImGui::Begin("Karachi Route Tracer");

    ImGui::TextColored(ImVec4(0.6f, 0.9f, 1.0f, 1.0f), "Pathfinding Controls");
    ImGui::Separator();
    ImGui::Spacing();

    static int mode = 0;  // 0 = Node IDs, 1 = Coordinates
    ImGui::Text("Select Pathfinding Mode:");
    ImGui::RadioButton("Node IDs", &mode, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Coordinates", &mode, 1);
    ImGui::Spacing();

    if (mode == 0) {
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "Node Search");
        ImGui::InputInt("Start Node", &m_startNode);
        ImGui::InputInt("End Node", &m_endNode);
        if (ImGui::Button("Run A* (Node IDs)")) m_runAStarWithNodes = true;
        ImGui::Spacing();
    }

    if (mode == 1) {
        ImGui::TextColored(ImVec4(0.3f, 0.9f, 0.3f, 1.0f), "Coordinate Search");

        ImGui::Text("Start Location:");
        ImGui::InputFloat("Start Latitude", &m_startLat, 0.0f, 0.0f, "%.6f");
        ImGui::InputFloat("Start Longitude", &m_startLon, 0.0f, 0.0f, "%.6f");
        ImGui::Spacing();

        ImGui::Text("End Location:");
        ImGui::InputFloat("End Latitude", &m_endLat, 0.0f, 0.0f, "%.6f");
        ImGui::InputFloat("End Longitude", &m_endLon, 0.0f, 0.0f, "%.6f");

        if (ImGui::Button("Run A* (Coordinates)")) m_runAStarWithCoords = true;
        ImGui::Spacing();
    }

    ImGui::TextColored(ImVec4(0.9f, 0.5f, 0.2f, 1.0f), "Search Road");
    ImGui::InputText("Road Name", m_searchBuffer, IM_ARRAYSIZE(m_searchBuffer));
    if (ImGui::Button("Search")) m_searchRequested = true;

    ImGui::TextColored(ImVec4(0.5f, 0.7f, 1.0f, 1.0f), "Path Color");
    ImGui::ColorEdit3("Path Color", m_pathColor);

    

    ImGui::End();
}