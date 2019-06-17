#ifndef CSHADER_FACTORY_H
#define CSHADER_FACTORY_H

#include <memory>
#include <SFML/Graphics.hpp>

class CShaderFactory {   
public:
    CShaderFactory();
    ~CShaderFactory() = default;

    std::unique_ptr<sf::Shader> createUnique() const;
    sf::Shader* createNew() const;
    
    static void warnIfNotAvailable();
    void toggle(bool enable);
    bool areEnabled() const;

private:
    bool m_enabled = true;
};
#endif