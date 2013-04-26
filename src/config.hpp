
#ifndef DEF_CONFIG
#define DEF_CONFIG

// Ressources
#define rcdir "rc/"
#define bonus_subdir "bonus/" // bonus_dir rcdir/bonus_subdir
#define bg_path "bg.png"

// Map
#define widthMap 40
#define heightMap 30
#define sizeTile 20

// Snakes
#define snake_subdir "snake/1.png"

// SDL
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask 0xff000000
#define gmask 0x00ff0000
#define bmask 0x0000ff00
#define amask 0x000000ff
#else
#define rmask 0x000000ff
#define gmask 0x0000ff00
#define bmask 0x00ff0000
#define amask 0xff000000
#endif

// Gameplay
#define max_players 4

// Gui
#define gui_subdir "gui/"

#endif

