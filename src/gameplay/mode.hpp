
#ifndef DEF_MODE
#define DEF_MODE

#include <SDL.h>
#include <string>
#include "gameplay/map.hpp"

class Mode
{
    public:
        Mode(const std::string& path);
        ~Mode();

        unsigned int prob() const;
        void start();
        bool end() const;
        void bonus(Map* map);

        static bool loadAll(const std::string& dir);
        static void freeAll();

    private:
        Uint32 m_start;
        Uint32 m_end;
        Uint32 m_bonus;
        Uint32 m_mtime;
        unsigned int m_prob;
        std::string m_path;
};

std::vector<Mode*> modes;

#endif

