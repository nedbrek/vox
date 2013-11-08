#ifndef RESOURCES_H
#define RESOURCES_H

#include <osg/Array>
#include <vector>
namespace osg { class Texture2D; }

//----------------------------------------------------------------------------
class Resources
{
public:
	Resources();
	~Resources();

	osg::Vec3Array* cubeVerts() { return cubeVerts_; }

	osg::Texture2D& textureFromId(unsigned char id);

protected:
	osg::Vec3Array *cubeVerts_;
	std::vector<osg::Texture2D*> textures_;
};
#endif

