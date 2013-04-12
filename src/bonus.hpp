
#ifndef DEF_BONUS
#define DEF_BONUS

#include <vector>
#include <string>
struct SDL_Surface;
namespace boost {
	namespace filesystem {
		class path;
	};
};

class Bonus
{
	public:
		Bonus();
		~Bonus();

		std::string getName() const;
		signed int getPts() const;
		signed int getLength() const;
		int getTime() const;
		SDL_Surface* getImg() const;
		int getFact() const;

		static bool loadAll(const std::string& dir);

	private:
		signed int m_pts;
		signed int m_length;
		int m_time;
		SDL_Surface* m_img;
		int m_fact;
		std::string m_name;
		bool load(const boost::filesystem::path& path);
		void storeValue(const std::string& key, const std::string& value);
};

extern std::vector<Bonus> bonus;

#endif

