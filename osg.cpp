#include "hud.h"
#include "chunk.h"
#include "resources.h"
#include <osg/CullFace>
#include <osg/PositionAttitudeTransform>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

class EventHandler : public osgGA::GUIEventHandler
{
protected:
	osg::Vec3f eye, center, up;

public:
	EventHandler()
	{
		eye = osg::Vec3f(0, 2, 1.5);
		center = osg::Vec3f(0, 1, 1.5);
		up = osg::Vec3f(0, 0, 1);
	}

	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
	{
		osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer*>(&us);

		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::KEYDOWN:
			switch (ea.getKey())
			{
			case 'a':
				eye.set(eye.x() + 1, eye.y(), eye.z());
				center.set(center.x() + 1, center.y(), center.z());
				viewer->getCamera()->setViewMatrixAsLookAt(eye, center, up);
				return true;

			case 'd':
				eye.set(eye.x() - 1, eye.y(), eye.z());
				center.set(center.x() - 1, center.y(), center.z());
				viewer->getCamera()->setViewMatrixAsLookAt(eye, center, up);
				return true;

			case 'x':
				eye.set(eye.x(), eye.y() + 1, eye.z());
				center.set(center.x(), center.y() + 1, center.z());
				viewer->getCamera()->setViewMatrixAsLookAt(eye, center, up);
				return true;

			case 'w':
				eye.set(eye.x(), eye.y() - 1, eye.z());
				center.set(center.x(), center.y() - 1, center.z());
				viewer->getCamera()->setViewMatrixAsLookAt(eye, center, up);
				return true;

			default:;
			}

		default:;
		}
		return false;
	}
};

std::ostream& operator<<(std::ostream &os, osg::Vec3 vec)
{
	return os << vec.x() << ' ' << vec.y() << ' ' << vec.z();;
}

///@return geometry for one cube
osg::Geometry* oneCube(Resources &r)
{
	osg::Geometry *geometry = new osg::Geometry;

	geometry->setVertexArray(r.cubeVerts());

	osg::DrawElementsUByte *cubeIndexes = new osg::DrawElementsUByte(
	  osg::PrimitiveSet::TRIANGLE_STRIP, 0);

	cubeIndexes->push_back(3);
	cubeIndexes->push_back(2);
	cubeIndexes->push_back(6);
	cubeIndexes->push_back(7);
	cubeIndexes->push_back(4);
	cubeIndexes->push_back(2);
	cubeIndexes->push_back(0);
	cubeIndexes->push_back(3);
	cubeIndexes->push_back(1);
	cubeIndexes->push_back(6);
	cubeIndexes->push_back(5);
	cubeIndexes->push_back(4);
	cubeIndexes->push_back(1);
	cubeIndexes->push_back(0);
	geometry->addPrimitiveSet(cubeIndexes);

	return geometry;
}

void addShader(osg::Group *node)
{
	osg::Program *program = new osg::Program;
	osg::Shader *vertexShader = new osg::Shader(osg::Shader::VERTEX);
	vertexShader->loadShaderSourceFromFile("vertBasic.glsl");
	osg::Shader *fragmentShader = new osg::Shader(osg::Shader::FRAGMENT);
	fragmentShader->loadShaderSourceFromFile("fragCube.glsl");
	program->addShader(vertexShader);
	program->addShader(fragmentShader);
	node->getOrCreateStateSet()->setAttributeAndModes(program);
}

int main(int argc, char **argv)
{
	Resources r;
	osg::Group *root = new osg::Group;
	osg::Group *world = new osg::Group;
	root->addChild(world);

	addShader(world);

	osg::Texture2D *stoneTex = new osg::Texture2D;
	stoneTex->setImage(osgDB::readImageFile("textures/blocks/stone.png"));

	osg::Geometry *cubeGeom = oneCube(r);
	osg::Geode *stoneGeode = new osg::Geode;
	stoneGeode->addDrawable(cubeGeom);
	stoneGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, stoneTex);

	unsigned char *blocks = new unsigned char[4096];
	for (int i = 0; i < 4096; ++i)
		blocks[i] = 0;
	for (int x = 0; x < 16; ++x)
	for (int y = 0; y < 16; ++y)
		blocks[Chunk::index(x, y, 0)] = 1;

	for (int cx = 0; cx < 16; ++cx)
	for (int cy = 0; cy < 16; ++cy)
	{
		Chunk chunk(blocks, cx, cy, 0);

		osg::LOD *lod = new osg::LOD;
		lod->addChild(chunk.makeChunkMesh(r), 16, 2048);
		lod->addChild(chunk.makeDumbChunk(stoneGeode), 0, 16);
		world->addChild(lod);
	}

	osg::CullFace *cull = new osg::CullFace();
	cull->setMode(osg::CullFace::BACK);
	world->getOrCreateStateSet()->setAttributeAndModes(cull);

	/*{
		osgUtil::Optimizer opt;
		opt.optimize(world, osgUtil::Optimizer::DEFAULT_OPTIMIZATIONS | osgUtil::Optimizer::MERGE_GEODES);
	}*/

	Hud hud(root);

	const size_t centerIdx = hud.addVarLine("Center: ", "");
	const size_t eyeIdx = hud.addVarLine("Eye: ", "");

	osgViewer::Viewer viewer;
	viewer.setSceneData(root);
	viewer.realize();

	osg::Vec3f eye, center, up;
	eye = osg::Vec3f(0, 2, 1.5);
	center = osg::Vec3f(0, 1, 1.5);
	up = osg::Vec3f(0, 0, 1);
	viewer.getCamera()->setViewMatrixAsLookAt(eye, center, up);

	osgViewer::GraphicsWindow *window = dynamic_cast<osgViewer::GraphicsWindow*>(
	    viewer.getCamera()->getGraphicsContext());
	window->useCursor(false);

	osg::ref_ptr<EventHandler> eh = new EventHandler;
	viewer.addEventHandler(eh);
	viewer.addEventHandler(new osgViewer::StatsHandler);

	while( !viewer.done() )
	{
		viewer.frame();

		viewer.getCamera()->getViewMatrixAsLookAt(eye, center, up);
		hud.updateVarLine(centerIdx, center);
		hud.updateVarLine(eyeIdx, eye);
	}

	return 0;
}

