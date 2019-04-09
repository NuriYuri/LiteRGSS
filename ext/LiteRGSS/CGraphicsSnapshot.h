#ifndef CGraphicsSnapshot_H
#define CGraphicsSnapshot_H

#include <memory>
#include "ruby.h"
#include <SFML/Graphics.hpp>

class CGraphicsSnapshot {
public:
    VALUE takeSnapshot(sf::RenderWindow& window);
    void freeze(sf::RenderWindow& window, VALUE self);
    void transition(VALUE self, int argc, VALUE* argv);
    void stop();
    void draw(sf::RenderWindow& window);
    void init();
private:
    void takeSnapshotOn(sf::RenderWindow& window, sf::Texture& text) const;
    void transitionBasic(VALUE self, long time);
    void transitionRGSS(VALUE self, long time, VALUE bitmap);

    std::unique_ptr<sf::Texture> Graphics_freeze_texture = nullptr;
    std::unique_ptr<sf::Sprite> Graphics_freeze_sprite = nullptr;
    std::unique_ptr<sf::Shader> Graphics_freeze_shader = nullptr;
    
    bool RGSSTransition = false;
};

#endif