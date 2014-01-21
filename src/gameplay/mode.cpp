
#include "mode.hpp"
#include "gameplay/bonus.hpp"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <fstream>
#include <sstream>

std::vector<Mode*> modes;
namespace fs = boost::filesystem;

Mode::Mode(const std::string& path)
{
    std::ifstream ifs(path);
    if(!ifs)
        return;
	
    boost::regex syntax("^[[:blank:]]*(time|bonus|fact|freq)[[:blank:]]*=[[:blank:]]*(([[:word:]]|\\.|-|\\+|/|\\\\)+)");
    boost::smatch sm;
    std::string line;

    while(std::getline(ifs, line)) {
		if(boost::regex_search(line, sm, syntax)) {
            if(sm[1] == "time") {
                std::istringstream iss(sm[2]);
                iss >> m_end;
            }
            else if(sm[1] == "bonus")
                m_path = sm[2];
            else if(sm[1] == "fact") {
                std::istringstream iss(sm[2]);
                iss >> m_prob;
            }
            else if(sm[1] == "freq") {
                std::istringstream iss(sm[2]);
                iss >> m_bonus;
            }
        }
    }
}

Mode::~Mode()
{}

unsigned int Mode::prob() const
{
    return m_prob;
}

void Mode::start()
{
    m_start = m_mtime = SDL_GetTicks();
    Bonus::freeAll();
    Bonus::loadAll(m_path);
}

bool Mode::end() const
{
    return (SDL_GetTicks() - m_start > m_end);
}

void Mode::bonus(Map* map)
{
    if(SDL_GetTicks() - m_mtime > m_bonus) {
        map->addBonus();
        m_mtime = SDL_GetTicks();
    }
}

bool Mode::loadAll(const std::string& dir)
{
    modes.clear();
    fs::path path(dir);
    fs::directory_iterator end;
    for(fs::directory_iterator it(path); it != end; ++it) {
        if(!fs::is_directory(*it)) {
            fs::path p = *it;
            Mode* m = new Mode(p.string());
            modes.push_back(m);
        }
    }
    return true;
}

void Mode::freeAll()
{
    for(size_t i = 0; i < modes.size(); ++i)
        delete modes[i];
    modes.clear();
}

