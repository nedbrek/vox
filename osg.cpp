#include "chunk.h"
#include <osg/CullFace>
#include <osg/PositionAttitudeTransform>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

/// create the geometry for one cube
osg::Geometry* oneCube()
{
	osg::Geometry *geometry = new osg::Geometry;

	osg::Vec3Array *cubeVerts = new osg::Vec3Array;
	cubeVerts->push_back(osg::Vec3(1, 1, 1)); // 0
	cubeVerts->push_back(osg::Vec3(0, 1, 1)); // 1
	cubeVerts->push_back(osg::Vec3(1, 1, 0)); // 2
	cubeVerts->push_back(osg::Vec3(0, 1, 0)); // 3
	cubeVerts->push_back(osg::Vec3(1, 0, 1)); // 4
	cubeVerts->push_back(osg::Vec3(0, 0, 1)); // 5
	cubeVerts->push_back(osg::Vec3(0, 0, 0)); // 6
	cubeVerts->push_back(osg::Vec3(1, 0, 0)); // 7
	geometry->setVertexArray(cubeVerts);

	osg::DrawElementsUByte *cubeIndexes = new osg::DrawElementsUByte(
	  osg::PrimitiveSet::TRIANGLE_STRIP, 0);
	  //osg::PrimitiveSet::QUADS, 0);

	/*cubeIndexes->push_back(6);
	cubeIndexes->push_back(3);
	cubeIndexes->push_back(2);
	cubeIndexes->push_back(7);*/
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

	osg::Vec4Array *colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1, 1, 1, 1));
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	//geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	return geometry;
}

int main(int argc, char **argv)
{
	osg::Group *root = new osg::Group;

	osg::Program *program = new osg::Program;
	osg::Shader *vertexShader = new osg::Shader(osg::Shader::VERTEX);
	vertexShader->loadShaderSourceFromFile("vertBasic.glsl");
	osg::Shader *fragmentShader = new osg::Shader(osg::Shader::FRAGMENT);
	fragmentShader->loadShaderSourceFromFile("fragCube.glsl");
	program->addShader(vertexShader);
	program->addShader(fragmentShader);
	root->getOrCreateStateSet()->setAttributeAndModes(program);

	osg::Texture2D *stoneTex = new osg::Texture2D;
	stoneTex->setImage(osgDB::readImageFile("textures/blocks/stone.png"));

	osg::Geometry *cubeGeom = oneCube();
	osg::Geode *stoneGeode = new osg::Geode;
	stoneGeode->addDrawable(cubeGeom);
	stoneGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, stoneTex);

	unsigned char *blocks = new unsigned char[4096];
	for (int i = 0; i < 4096; ++i)
		blocks[i] = 0;
	for (int x = 0; x < 16; ++x)
	for (int y = 0; y < 16; ++y)
		blocks[Chunk::index(x, y, 0)] = 1;

	Chunk **chunks = new Chunk*[16];
	unsigned chunkId = 0;
	for (int cx = 0; cx < 4; ++cx)
	for (int cy = 0; cy < 4; ++cy)
	{
		Chunk *chunk = new Chunk(blocks, cx, cy, 0);
		chunks[chunkId] = chunk;
		++chunkId;

		root->addChild(chunk->makeDumbChunk(stoneGeode));
	}

	osg::CullFace *cull = new osg::CullFace();
	cull->setMode(osg::CullFace::BACK);
	root->getOrCreateStateSet()->setAttributeAndModes(cull);

	/*{
		osgUtil::Optimizer opt;
		opt.optimize(root, osgUtil::Optimizer::DEFAULT_OPTIMIZATIONS | osgUtil::Optimizer::MERGE_GEODES);
	}*/
	osgViewer::Viewer viewer;
	viewer.setSceneData(root);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	viewer.addEventHandler(new osgViewer::StatsHandler);

	while( !viewer.done() )
	{
		viewer.frame();
	}

	return 0;
}

