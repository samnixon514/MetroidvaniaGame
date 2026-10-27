#include "Game.h"
#include <iostream>

Game::Game()
{
	if (!ImGui::SFML::Init(m_window))
	{
		return;
	}
	
	m_graphics.setPlayer(m_animSprite);
}

void DefineGUI(float fps)
{
    // Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::Begin("GEC");				// Create a window called "3GP" and append into it.

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::Text("Stable FPS: %.0f", fps);

    ImGui::End();
}


void Game::running()
{
    while (m_window.isOpen())
    {

        //m_window.setMouseCursorVisible(false);

        // Process events
        while (const std::optional event = m_window.pollEvent())
        {
            // Feed ImGui
            ImGui::SFML::ProcessEvent(m_window, event.value());

            // User clicked on window close X
            if (event->is<sf::Event::Closed>())
                m_window.close();
        }
        // ImGui must be updated each frame
        sf::Time dt = m_updateClock.restart();
        ImGui::SFML::Update(m_window, m_uiDeltaClock.restart());

        m_frameCount++;
        if (m_fpsClock.getElapsedTime().asSeconds() >= 1.f)
        {
            m_fps = m_frameCount / m_fpsClock.restart().asSeconds();
            m_frameCount = 0;
        }
        // The UI gets defined each time // VARIABLES HERE
        DefineGUI(m_fps);

        //Input Handler - Movement, Jump and Attack
        m_animSprite.handleInput();

        m_animSprite.update(dt);

        m_graphics.update(dt);

        // Clear the window
        m_window.clear(sf::Color(5, 5, 15));

        m_graphics.draw(&m_window);

        // UI needs drawing last
        ImGui::SFML::Render(m_window);
        m_window.display();
    }

    std::cout << "Finished!" << std::endl;

    ImGui::SFML::Shutdown();
}
