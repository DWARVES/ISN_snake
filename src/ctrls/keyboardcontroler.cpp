
#include "keyboardcontroler.hpp"
#include "snake.hpp"

SDLKey KeyboardControler::m_keys[NB_KEYS_CONFIGS][(unsigned int)KeyboardControler::Last];
bool KeyboardControler::m_used[NB_KEYS_CONFIGS];
bool KeyboardControler::m_init = false;

    KeyboardControler::KeyboardControler()
: Controler(), m_keysUsed(-1), m_valid(SDLK_UNKNOWN)
{
    loadPict("keyboard.png");
    if(!m_init)
        init();
}

KeyboardControler::~KeyboardControler()
{
    if(m_keysUsed >= 0)
        m_used[m_keysUsed] = false;
}

bool KeyboardControler::validEvent(const SDL_Event& ev) const
{
    if(ev.type != SDL_KEYDOWN)
        return false;

    int c = confFromKey(ev.key.keysym.sym);
    if(c < 0)
        return false;
    if(m_used[c])
        return false;

    m_valid = ev.key.keysym.sym;
    m_keysUsed = c;
    m_used[c] = true;
    return true;
}

bool KeyboardControler::stillValid(const SDL_Event& ev) const
{
    if(ev.type != SDL_KEYUP)
        return true;

    if(ev.key.keysym.sym == m_valid) {
        m_used[m_keysUsed] = false;
        m_keysUsed = -1;
        return false;
    }
    else
        return true;
}

bool KeyboardControler::processEvent(const SDL_Event& ev)
{
    if(ev.type != SDL_KEYDOWN)
        return false;

    SDLKey k = ev.key.keysym.sym;
    if(k == m_keys[m_keysUsed][(unsigned int)Up])
        setMove(&Snake::moveUp);
    else if(k == m_keys[m_keysUsed][(unsigned int)Down])
        setMove(&Snake::moveDown);
    else if(k == m_keys[m_keysUsed][(unsigned int)Right])
        setMove(&Snake::moveRight);
    else if(k == m_keys[m_keysUsed][(unsigned int)Left])
        setMove(&Snake::moveLeft);
    else
        return false;
    return true;
}

Controler* KeyboardControler::duplicate() const
{
    return new KeyboardControler;
}

int KeyboardControler::confFromKey(SDLKey key) const
{
    for(unsigned int c = 0; c < NB_KEYS_CONFIGS; ++c) {
        for(unsigned int k = 0; k < (unsigned int)Last; ++k) {
            if(key == m_keys[c][k])
                return c;
        }
    }
    return -1;
}

void KeyboardControler::init()
{
    m_keys[0][(unsigned int)Left]  = SDLK_LEFT;
    m_keys[0][(unsigned int)Right] = SDLK_RIGHT;
    m_keys[0][(unsigned int)Up]    = SDLK_UP;
    m_keys[0][(unsigned int)Down]  = SDLK_DOWN;

    m_keys[1][(unsigned int)Left]  = SDLK_q;
    m_keys[1][(unsigned int)Right] = SDLK_d;
    m_keys[1][(unsigned int)Up]    = SDLK_z;
    m_keys[1][(unsigned int)Down]  = SDLK_s;

    m_keys[2][(unsigned int)Left]  = SDLK_j;
    m_keys[2][(unsigned int)Right] = SDLK_l;
    m_keys[2][(unsigned int)Up]    = SDLK_i;
    m_keys[2][(unsigned int)Down]  = SDLK_k;

    for(unsigned int i = 0; i < NB_KEYS_CONFIGS; ++i)
        m_used[i] = false;
    m_init = true;
}

