#include <osg/CullFace>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

unsigned index(unsigned x, unsigned y, unsigned z)
{
	return x + y * 16 + z * 256;
}

osg::Geode* cubeInstanced()
{
	osg::Geode *geode = new osg::Geode;
	osg::Geometry *geometry = new osg::Geometry;
	geode->addDrawable(geometry);

	osg::Vec3Array *cubeVerts = new osg::Vec3Array;
	cubeVerts->push_back(osg::Vec3(0, 1, 0)); // 3
	cubeVerts->push_back(osg::Vec3(1, 1, 0)); // 2
	cubeVerts->push_back(osg::Vec3(0, 0, 0)); // 6
	cubeVerts->push_back(osg::Vec3(1, 0, 0)); // 7
	cubeVerts->push_back(osg::Vec3(1, 0, 1)); // 4
	cubeVerts->push_back(osg::Vec3(1, 1, 0)); // 2
	cubeVerts->push_back(osg::Vec3(1, 1, 1)); // 0
	cubeVerts->push_back(osg::Vec3(0, 1, 0)); // 3
	cubeVerts->push_back(osg::Vec3(0, 1, 1)); // 1
	cubeVerts->push_back(osg::Vec3(0, 0, 0)); // 6
	cubeVerts->push_back(osg::Vec3(0, 0, 1)); // 5
	cubeVerts->push_back(osg::Vec3(1, 0, 1)); // 4
	cubeVerts->push_back(osg::Vec3(0, 1, 1)); // 1
	cubeVerts->push_back(osg::Vec3(1, 1, 1)); // 0
	/*cubeVerts->push_back(osg::Vec3(0, 0, 0));
	cubeVerts->push_back(osg::Vec3(0, 0, 1));
	cubeVerts->push_back(osg::Vec3(1, 0, 1));
	cubeVerts->push_back(osg::Vec3(1, 0, 0));*/

	geometry->setVertexArray(cubeVerts);

	//geometry->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4, 1));
	geometry->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_STRIP, 0, 14, 4096));
	geometry->setUseDisplayList(false);

    osg::BoundingBox bb(-1, 17, -1, 17, -1, 17);
    geometry->setInitialBound(bb);

	return geode;
}

osg::Geode* oneCube()
{
	osg::Geode *geode = new osg::Geode;
	osg::Geometry *geometry = new osg::Geometry;
	geode->addDrawable(geometry);

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

	return geode;
}

/// populate a chunk - should probably take a random seed
void fillChunk(unsigned char *blockIds)
{
	for(unsigned i = 0; i < 4096; ++i)
		blockIds[i] = 0;

	for(unsigned x = 0; x < 16; ++x)
	for(unsigned z = 0; z < 16; ++z)
		blockIds[index(x, 0, z)] = 1;

	//for(unsigned x = 0; x < 16; ++x)
	//for(unsigned z = 0; z < 16; ++z)
		//blockIds[index(x, 1, z)] = 2;
}

int main(int argc, char **argv)
{
	//osg::Geode *geode = oneCube();
	osg::Geode *geode = cubeInstanced();

	osg::Program *program = new osg::Program;
	osg::Shader *vertexShader = new osg::Shader(osg::Shader::VERTEX);
	vertexShader->loadShaderSourceFromFile("vertInstance.glsl");
	osg::Shader *fragmentShader = new osg::Shader(osg::Shader::FRAGMENT);
	fragmentShader->loadShaderSourceFromFile("fragCube.glsl");
	program->addShader(vertexShader);
	program->addShader(fragmentShader);
	geode->getOrCreateStateSet()->setAttributeAndModes(program);

	osg::Texture2D *stoneTex = new osg::Texture2D;
	stoneTex->setImage(osgDB::readImageFile("textures/blocks/stone.png"));
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, stoneTex);

	osg::CullFace *cull = new osg::CullFace();
	cull->setMode(osg::CullFace::BACK);
	geode->getOrCreateStateSet()->setAttributeAndModes(cull);

	osgViewer::Viewer viewer;
	viewer.setSceneData(geode);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	viewer.addEventHandler(new osgViewer::StatsHandler);

	while( !viewer.done() )
	{
		viewer.frame();
	}

	return 0;
}

