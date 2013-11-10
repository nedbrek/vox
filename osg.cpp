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

///@return a slave camera for the HUD
osg::Camera* createHUD()
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

	return camera;
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

int main(int argc, char **argv)
{
	Resources r;
	osg::Group *root = new osg::Group;
	osg::Group *world = new osg::Group;
	root->addChild(world);

	osg::Program *program = new osg::Program;
	osg::Shader *vertexShader = new osg::Shader(osg::Shader::VERTEX);
	vertexShader->loadShaderSourceFromFile("vertBasic.glsl");
	osg::Shader *fragmentShader = new osg::Shader(osg::Shader::FRAGMENT);
	fragmentShader->loadShaderSourceFromFile("fragCube.glsl");
	program->addShader(vertexShader);
	program->addShader(fragmentShader);
	world->getOrCreateStateSet()->setAttributeAndModes(program);

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

	osg::Camera *hudCamera = createHUD();
	root->addChild(hudCamera);

	osg::Geode* hudGeode = new osg::Geode;
	hudCamera->addChild(hudGeode);
	hudGeode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	osgText::Text *centerText = new osgText::Text;
	centerText->setDataVariance(osg::Object::DYNAMIC);
	hudGeode->addDrawable(centerText);

	osgText::Text *eyeText = new osgText::Text;
	eyeText->setDataVariance(osg::Object::DYNAMIC);
	eyeText->setPosition(osg::Vec3(0, 30, 0));
	hudGeode->addDrawable(eyeText);

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
	hudCamera->addChild(hudCrosshairs);

	osgViewer::Viewer viewer;
	viewer.setSceneData(root);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	viewer.addEventHandler(new osgViewer::StatsHandler);

	osg::Vec3f eye, center, up;
	while( !viewer.done() )
	{
		viewer.frame();

		viewer.getCamera()->getViewMatrixAsLookAt(eye, center, up);
		std::ostringstream os;
		os << "Center: " << center.x() << ' ' << center.y() << ' ' << center.z();
		centerText->setText(os.str());

		os.str("");
		os << "Eye: " << eye.x() << ' ' << eye.y() << ' ' << eye.z();
		eyeText->setText(os.str());
	}

	return 0;
}

