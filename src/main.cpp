#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
using namespace std;


int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Zombie gaem");

    sf::CircleShape player(20.f, 3);    // player
    player.setFillColor(sf::Color::Red);
    player.setOrigin(player.getRadius(), player.getRadius());
    player.setPosition(window.getSize().x/2, window.getSize().y/2); // place them in the middle
        // camera is static; doesnt follow the player

    float playerSpeed = 200.f;
    
    sf::VertexArray raycast(sf::Lines, 2);      // <-- raycast
    sf::Clock clock;



    //----Main Loop----
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();  // framerate independent "physics"

        //-------------Player Movement-------------
        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                movement.y -= playerSpeed * deltaTime;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                movement.y += playerSpeed * deltaTime;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                movement.x -= playerSpeed * deltaTime;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                movement.x += playerSpeed * deltaTime;}
        player.move(movement);

        //-------------Player Rotation-------------
        sf::Vector2f playerPos = player.getPosition();
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        float dx = mousePos.x - playerPos.x;
        float dy = mousePos.y - playerPos.y;
        float angle = std::atan2(dy, dx);
        float degrees = angle * 180 / M_PI;

        player.setRotation(degrees + 90.f);

        //-------------Player Raycast-------------
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                sf::Vector2f direction(mousePos.x - playerPos.x, mousePos.y - playerPos.y);
                float length = std::sqrt((direction.x * direction.x) + (direction.y + direction.y));
                if (length != 0)
                        direction /= length;
                sf::Vector2f rayPoint = playerPos + direction * 10000.f;

                raycast[0].position = playerPos;
                raycast[0].color = sf::Color::Green;
                raycast[1].position = rayPoint;
                raycast[1].color = sf::Color::Red;

                // raycast doesnt have any hit detection implemented
        }

        //-------------Rendering-------------
        window.clear(sf::Color::Black);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                window.draw(raycast);
        window.draw(player);
        window.display();
    }

    return 0;
}