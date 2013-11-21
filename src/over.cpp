
#include "over.hpp"
#include "gui.hpp"
#include "snake.hpp"
#include "button.hpp"
#include "music.hpp"
#include <sstream>
#include <SDL_ttf.h>

    GameOver::GameOver(Gui* g, SDL_Surface* s, Snake* snks[max_players])
: m_gui(g), m_scr(s), m_txt(NULL), m_back(NULL)
{
    SnkRank rank[max_players];
    int win = computeWin(snks, rank);
    for(int i = 0; i < max_players; ++i)
        createScr(rank, i);

    std::ostringstream oss;
    if(win >= 0)
        oss << "Player " << win + 1 << " won !";
    else
        oss << "Equality !";
    m_txt = TTF_RenderText_Blended(m_gui->getFont(),
            oss.str().c_str(),
            m_gui->getPlayerColor(win));

    m_back = new Button("End", m_gui);
    SDL_Rect pos;
    pos.x = m_scr->w / 8 * 7;
    pos.y = m_scr->h / 8 * 7;
    m_back->setPos(pos);
}

GameOver::~GameOver()
{
    for(int i = 0; i < max_players; ++i)
    {
        if(m_scores[i] != NULL)
            SDL_FreeSurface(m_scores[i]);
    }

    if(m_txt != NULL)
        SDL_FreeSurface(m_txt);

    if(m_back != NULL)
        delete m_back;
}

bool GameOver::run()
{
    SDL_Color bgc = m_gui->bgColor();
    Uint32 bg = SDL_MapRGB(m_scr->format, bgc.r, bgc.g, bgc.b);

    bool continuer = true;
    SDL_Rect pos;
    SDL_Event ev;

    while(continuer)
    {
        // Affichage
        SDL_FillRect(m_scr, NULL, SDL_MapRGB(m_scr->format, 0, 0, 0));
        SDL_BlitSurface(m_gui->background(), NULL, m_scr, NULL);

        pos.x = m_scr->w / 2 - m_txt->w / 2;
        pos.y = m_scr->h / 4 - m_txt->h / 2;
        {
            SDL_Rect tmp = pos;
            tmp.x -= 10;
            tmp.y -= 10;
            tmp.w = m_txt->w + 20;
            tmp.h = m_txt->h + 20;
            SDL_FillRect(m_scr, &tmp, bg);
        }
        SDL_BlitSurface(m_txt, NULL, m_scr, &pos);

        pos.x = 300; pos.w = 200;
        pos.y = 300; pos.h = 140;
        SDL_FillRect(m_scr, &pos, bg);

        int y = pos.y + 5;
        for(int i = 0; i < max_players; ++i)
        {
            pos.x = m_scr->w / 2 - m_scores[i]->w / 2;
            y += 25;
            pos.y = y - m_scores[i]->h / 2;
            SDL_BlitSurface(m_scores[i], NULL, m_scr, &pos);
        }
        m_back->blitOn(m_scr);
        SDL_Flip(m_scr);

        // Évènements
        while(SDL_PollEvent(&ev))
        {
            switch(ev.type)
            {
                case SDL_QUIT:
                    return false;
                case SDL_KEYDOWN:
                    if(ev.key.keysym.sym == SDLK_BACKSPACE
                            || ev.key.keysym.sym == SDLK_ESCAPE)
                        continuer = false;
                    else if(ev.key.keysym.sym == SDLK_RETURN) {
                        music->playSound(Music::OK);
                        continuer = false;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    pos.x = ev.motion.x;
                    pos.y = ev.motion.y;
                    m_back->mouse(pos);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    pos.x = ev.button.x;
                    pos.y = ev.button.y;
                    if(m_back->clicked(pos))
                    {
                        music->playSound(Music::OK);
                        continuer = false;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    return true;
}

void GameOver::createScr(SnkRank snks[max_players], int idx)
{
    std::ostringstream oss;
    if(snks[idx].snk != NULL)
        oss << idx + 1 << ". Player " << snks[idx].idx + 1 << " : " << snks[idx].snk->getScore(true);
    else
        oss << "No player";

    m_scores[idx] = TTF_RenderText_Blended(m_gui->getFont(),
            oss.str().c_str(),
            m_gui->getPlayerColor(snks[idx].idx));
}

int GameOver::computeWin(Snake* snks[max_players], SnkRank rank[max_players]) const
{
    int w = 0;
    unsigned int max = 0;
    for(int i = 0; i < max_players; ++i)
    {
        rank[i].scr = 0;
        rank[i].idx = -1;
        rank[i].snk = NULL;
    }

    for(int i = 0; i < max_players; ++i)
    {
        if(snks[i] == NULL)
            continue;

        // Rank
        unsigned int scr = snks[i]->getScore(true);

        int j;
        for(j = max_players - 1; j > 0  && rank[j - 1].scr <= scr; --j)
        {
            rank[j].scr = rank[j - 1].scr;
            rank[j].idx = rank[j - 1].idx;
        }
        rank[j].scr = snks[i]->getScore(true);
        rank[j].idx = i;

        // Winner
        if(scr > max)
        {
            max = scr;
            w = i;
        }
        else if(scr == max)
            w = -1;
    }

    for(int i = 0; i < max_players; ++i)
    {
        if(rank[i].idx >= 0)
            rank[i].snk = snks[rank[i].idx];
    }

    return w;
}

