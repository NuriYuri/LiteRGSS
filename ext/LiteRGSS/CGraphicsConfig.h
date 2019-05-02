#ifndef CGraphicsConfig_H
#define CGraphicsConfig_H

#include <tuple>
#include <SFML/Graphics.hpp>

struct CGraphicsVideoConfig {
	sf::VideoMode vmode;
	unsigned int width = 640;
	unsigned int height = 480;
	double scale = 1.0;
};

struct CGraphicsConfig {
	CGraphicsVideoConfig video;
	bool smoothScreen = false;
	sf::String title;
	unsigned int frameRate = 60;
	bool vSync = false;
	bool fullscreen = false;
};

class CGraphicsConfigLoader {
public:
	CGraphicsConfig load() const;

private:
	CGraphicsVideoConfig loadVideoFromConfigs() const;
	bool loadSmoothScreenFromConfigs() const;
	sf::String loadTitleFromConfigs() const;
	unsigned int loadFrameRateFromConfigs() const;
	bool loadVSYNCFromConfigs() const;
	bool loadFullScreenFromConfigs() const;
};

#endif