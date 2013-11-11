#ifndef HUD_H
#define HUD_H

#include <osg/ref_ptr>
#include <sstream>
#include <string>
#include <vector>
namespace osg
{
	class Geode;
	class Group;
}

/// one line of text
class Line
{
public:
	virtual ~Line() = 0;
};

/// manages the 2D overlay in front of the camera
class Hud
{
public:
	Hud(osg::Group *rootNode);
	~Hud();

	/// a line which is always the same (mostly for debugging)
	void addStaticLine(const char *str);

	/// a line with a variable portion
	size_t addVarLine(const char *prefix, const std::string &initVal);

	template<typename T>
	void updateVarLine(size_t id, const T &newVal)
	{
		std::ostringstream os;
		os << newVal;
		updateVarLine(id, os.str());
	}

protected:
	std::vector<Line*> lines_;
	osg::ref_ptr<osg::Geode> hudNode_;
};

#endif

