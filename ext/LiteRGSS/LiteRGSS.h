#ifndef LITERGSS_HEADER_FILE
#define LITERGSS_HEADER_FILE
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "ruby.h" // Windows Patch
#include "Texture.hpp"
#include <SFML/Graphics.hpp>
#include "Drawable_Disposable.h"
#include "Color.h"
#include "Tone.h"
#include "Rect.h"
#include "Image.h"
#include "Viewport.h"
#include "Sprite.h"
#include "Fonts.h"
#include "Text.h"
#include "Input.h"
#include "Table.h"
#include "Table32.h"
#include "Yuki.h"
#include "BlendMode.h"
#include "Shader.h"
#include "ShaderSprite.h"
#include "Shape.h"
#include "Yuki_Gif.h"
#include "Window.h"
#include "SpriteMap.h"
#include "utils/common.h"
#include "utils/ruby_common.h"

void Init_DrawableDisposable();
void Init_Graphics();

void Init_Bitmap();
void Init_Image();
void Init_Sprite();
void Init_Rect();
void Init_Viewport();
void Init_Color();
void Init_Tone();
void Init_Fonts();
void Init_Text();
void Init_Input();
void Init_Table();
void Init_Table32();
void Init_Yuki();
void Init_Shader();
void Init_ShaderSprite();
void Init_YukiGifReader();
void Init_BlendMode();
void Init_Shape();
void Init_Window();
void Init_SpriteMap();


#endif
