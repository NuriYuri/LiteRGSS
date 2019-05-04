# Lite RGSS
## What is Lite RGSS ?

`LiteRGSS` is a little Ruby Game library that reproduces some features of the RGSS. It has been written with `SFML` and only manage the Graphic part.

Nuri Yuri wrote LiteRGSS during his free time, that's why [SFML](https://www.sfml-dev.org/index-fr.php"SFML") has been used (easy to use, nothing to learn) and wanted to have a RGSS like library that was usable directly with the MRI (to allow thing like `require "socket"` which is impossible with `mkxp` and other RGSS like library)

During the year 2019 [Scorbutics](https://github.com/Scorbutics) started to refractor & make the C++ part of LiteRGSS better.

## Why using LiteRGSS instead of RGSS1 RGSS2 or RGSS3 ?

LiteRGSS is a fully OpenSource project where everyone is free to contribute. The RGSS from enterbrain is highly closed and using a very old version of Ruby.

If you use LiteRGSS you can freely include any Ruby STD library (like socket) or any Ruby Gem.

## LiteRGSS vs RGSS

LiteRGSS doesn't gives a default Audio module, I consider you should be free to choose whatever you want to use to process Audio in your game. Personally I like Fmod so I made a Fmod Audio module using [RubyFmod](https://github.com/NuriYuri/Ruby-Fmod). If you want you can use an other gem or make your own Audio module using OpenAL (or SFML-Audio).

LiteRGSS doesn't display text inside bitmaps. It uses a different system : the [Text](https://psdk.pokemonworkshop.fr/litergss/LiteRGSS/Text.html) object (a sprite like object but for texts) and the [Fonts](https://psdk.pokemonworkshop.fr/litergss/LiteRGSS/Fonts.html) module. See the documentation to learn more about those two things.

LiteRGSS doesn't allow tone and color (or wave) inside Sprites. If you want sprites with that sort of processing, use the [ShaderedSprites](https://psdk.pokemonworkshop.fr/litergss/LiteRGSS/ShaderedSprite.html). By the way, LiteRGSS allow Shader processing on tone (faster than in sprites) and in the Graphics module (more global), with this ability you can for example implement HqnX, ScaleX or SABR filter if you want Higher screen resolution without remaking all your ressources :)

## How to use the LiteRGSS ?

All the explaination (building, using) are on the official doc : https://psdk.pokemonworkshop.fr/litergss/

## Specials Thanks

* SFML for their library https://www.sfml-dev.org
* [Scorbutics](https://github.com/Scorbutics) for the contributions
* NetSurf Browser for their gif library :) https://netsurf-browser.org
* Lode Vandevenne for LodePNG
