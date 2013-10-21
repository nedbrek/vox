#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>

int main(int argc, char **argv)
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

	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	osgViewer::Viewer viewer;
	viewer.setSceneData(geode);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	//viewer.realize();

	while( !viewer.done() )
	{
		viewer.frame();
	}

	return 0;
}

