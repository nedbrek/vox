#include "hud.h"
#include <osg/Camera>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgText/Text>
#include <string>

//----------------------------------------------------------------------------
Line::~Line()
{
}

//----------------------------------------------------------------------------
class LineImpl : public Line
{
protected:
	osgText::Text* makeText(int offset)
	{
		osgText::Text *textNode = new osgText::Text;
		textNode->setDataVariance(osg::Object::DYNAMIC);
		textNode->setPosition(osg::Vec3(0, offset, 0));

		return textNode;
	}
};
//----------------------------------------------------------------------------
class StaticLine : public LineImpl
{
public:
	StaticLine(osg::Geode *hudGeode, int offset, const char *str)
	{
		osgText::Text *text = makeText(offset);
		text->setText(str);
		hudGeode->addDrawable(text);
	}
};

class LineVar : public LineImpl
{
public:
	LineVar(osg::Geode *hudGeode, int offset, const char *prefix, const std::string &initVal)
	: prefix_(prefix)
	{
		textNode_ = makeText(offset);
		hudGeode->addDrawable(textNode_);

		update(initVal);
	}

	void update(const std::string &newVal)
	{
		textNode_->setText(prefix_ + newVal);
	}

protected:
	osg::ref_ptr<osgText::Text> textNode_;
	std::string prefix_;
};

//----------------------------------------------------------------------------
Hud::Hud(osg::Group *rootNode)
{
	osg::Camera *camera = new osg::Camera;
	// 2D projection with absolute, straight-on view
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1366, 0, 768));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());

	// make sure we draw on top of everything (and pass through events)
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setAllowEventFocus(false);

	rootNode->addChild(camera);

	osg::Geode* hudGeode = new osg::Geode;
	camera->addChild(hudGeode);
	hudGeode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	hudNode_ = hudGeode;

	osg::Geode *hudCrosshairs = new osg::Geode;
	osg::Geometry *crosshairGeom = new osg::Geometry;
	osg::Vec3Array* hudVertices = new osg::Vec3Array;
	hudVertices->push_back(osg::Vec3(1366/2, 768/2-8, 0));
	hudVertices->push_back(osg::Vec3(1366/2, 768/2+8, 0));
	hudVertices->push_back(osg::Vec3(1366/2-8, 768/2, 0));
	hudVertices->push_back(osg::Vec3(1366/2+8, 768/2, 0));
	crosshairGeom->setVertexArray(hudVertices);
	crosshairGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, hudVertices->size()));

	hudCrosshairs->addDrawable(crosshairGeom);
	camera->addChild(hudCrosshairs);
}

Hud::~Hud()
{
	for(std::vector<Line*>::const_iterator i = lines_.begin(); i != lines_.end(); ++i)
		delete *i;
}

void Hud::addStaticLine(const char *str)
{
	const size_t ret = lines_.size();
	lines_.push_back(new StaticLine(hudNode_, ret * 30, str));
}

size_t Hud::addVarLine(const char *prefix, const std::string &initVal)
{
	const size_t ret = lines_.size();
	lines_.push_back(new LineVar(hudNode_, ret * 30, prefix, initVal));
	return ret;
}

template<>
void Hud::updateVarLine<std::string>(size_t id, const std::string &newVal)
{
	LineVar *l = id < lines_.size() ? dynamic_cast<LineVar*>(lines_[id]) : NULL;
	if (l)
	{
		l->update(newVal);
	}
}

