#include "ruby_common.h"
#include "CShaderFactory.h"

CShaderFactory::CShaderFactory() :
    m_enabled(sf::Shader::isAvailable()) {
}

sf::Shader* CShaderFactory::createNew() const {
    return areEnabled() ? new sf::Shader() : nullptr;
}

std::unique_ptr<sf::Shader> CShaderFactory::createUnique() const {
    return areEnabled() ? std::make_unique<sf::Shader>() : nullptr;
}

void CShaderFactory::warnIfNotAvailable() {
    if(!sf::Shader::isAvailable()) {
        rb_warn("Shaders are not available :(");
    }
}

void CShaderFactory::toggle(bool enable) {
    if(enable) { 
        warnIfNotAvailable();
    }
    m_enabled = enable;
}

bool CShaderFactory::areEnabled() const {
    return m_enabled;
}