
#include "bonus.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <sstream>

std::vector<Bonus*> bonus;
namespace fs = boost::filesystem;

Bonus::Bonus()
	: m_pts(0), m_length(0), m_time(0), m_img(NULL), m_fact(0), m_name("")
{}

Bonus::~Bonus()
{
	if(m_img != NULL)
		SDL_FreeSurface(m_img);
}

std::string Bonus::getName() const
{
	return m_name;
}

signed int Bonus::getPts() const
{
	return m_pts;
}

signed int Bonus::getLength() const
{
	return m_length;
}

int Bonus::getTime() const
{
	return m_time;
}

SDL_Surface* Bonus::getImg() const
{
	return m_img;
}

int Bonus::getFact() const
{
	return m_fact;
}

bool Bonus::load(const fs::path& path)
{
	fs::ifstream file(path);
	if(!file)
		return false;

	boost::regex syntax("^[[:blank:]]*(pts|length|time|picture|fact)[[:blank:]]*=[[:blank:]]*(([[:word:]]|\\.|-|\\+|/|\\\\)+)");
	boost::smatch found;
	fs::path branch = path.branch_path();

	std::string line;
	while(std::getline(file, line))
	{
		if(boost::regex_search(line, found, syntax))
			storeValue(found[1], found[2], &branch);
	}
	m_name = path.leaf().string();

	return true;
}

bool Bonus::loadAll(const std::string& dir)
{
	bonus.clear();
	fs::path p(dir);
	bool ret = true;
	fs::directory_iterator end;
	for(fs::directory_iterator it(p); it != end; ++it)
	{
		if(!fs::is_directory(*it))
		{
			Bonus* bon = new Bonus;
			if(!bon->load(*it))
				ret = false;
			else
				bonus.push_back(bon);
		}
	}
	return ret;
}

void Bonus::storeValue(const std::string& key, const std::string& value, const fs::path* path)
{
	if(key == "pts")
	{
		std::istringstream iss(value);
		iss >> m_pts;
	}
	else if(key == "length")
	{
		std::istringstream iss(value);
		iss >> m_length;
	}
	else if(key == "time")
	{
		std::istringstream iss(value);
		iss >> m_time;
	}
	else if(key == "picture")
	{
		fs::path rp;
		if(path == NULL // No path
				|| !fs::is_directory(*path) // Invalid path
				|| !path->root_path().string().empty()) // Absolute path
			rp = "";
		else
			rp = *path;
		rp /= value;

		m_img = IMG_Load(rp.string().c_str());
		if(m_img == NULL)
			return;
		SDL_Surface* tmp = SDL_DisplayFormat(m_img);
		if(tmp == NULL)
			return;
		SDL_FreeSurface(m_img);
		m_img = tmp;
	}
	else if(key == "fact")
	{
		std::istringstream iss(value);
		iss >> m_fact;
	}
}

void Bonus::freeAll()
{
	for(auto it = bonus.begin(); it != bonus.end(); ++it)
		delete *it;
}

