#ifndef L_GRAPHICS_LOCAL_H
#define L_GRAPHICS_LOCAL_H
#include "LiteRGSS.h"

struct GraphicUpdateMessage {
    VALUE errorObject;
    const char* message;
};

void local_LoadVideoModeFromConfigs(sf::VideoMode& vmode);
const sf::String local_LoadTitleFromConfigs();
unsigned int local_LoadFrameRateFromConfigs();
bool local_LoadVSYNCFromConfigs();
bool local_LoadFullScreenFromConfigs();
void local_LoadSmoothScreenFromConfigs();
void* local_Graphics_Update_Internal(void* data);
VALUE local_Graphics_Update_RaiseError(VALUE self, GraphicUpdateMessage* message);
void local_Graphics_Update_Process_Event(GraphicUpdateMessage*& message);
void local_Graphics_Update_Draw(std::vector<CDrawable_Element*>* stack);
void local_Graphics_Take_Snapshot(sf::Texture* text);
void local_Graphics_Clear_Stack();
void local_Graphics_TransitionBasic(VALUE self, long time);
void local_Graphics_TransitionRGSS(VALUE self, long time, VALUE bitmap);
void local_Graphics_LoadShader();
void local_Graphics_DrawBrightness();
void local_Graphics_initRender();

extern sf::RenderWindow* game_window;
extern bool InsideGraphicsUpdate;
extern sf::Texture* Graphics_freeze_texture;
extern sf::Sprite* Graphics_freeze_sprite;
extern sf::Shader* Graphics_freeze_shader;
extern unsigned char Graphics_Brightness;

#endif