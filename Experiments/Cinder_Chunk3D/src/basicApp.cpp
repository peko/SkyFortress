#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Arcball.h"

using namespace ci;
using namespace ci::app;

#include <vector>
#include "Chunk3D.h"

using namespace std;

// We'll create a new Cinder Application by deriving from the BasicApp class
class BasicApp : public AppBasic {
 public:

	Rand* rand;

	void keyDown( KeyEvent event );
	void resize ( ResizeEvent event);
	void draw();
	void setup();
	void update();
	void shutdown();

	void mouseDown( MouseEvent event );
	void mouseDrag( MouseEvent event );

	Arcball		mArcball;
	Vec2i		mInitialMouseDown, mCurrentMouseDown;

	vector<Chunk3D*> chunks_level0;

	Chunk3D* chunk;

};


void BasicApp::setup(){

	rand = new Rand();

	chunk = new Chunk3D();
	chunks_level0.push_back(chunk);
	mCurrentMouseDown = mInitialMouseDown = Vec2i( 200, 200 );

	gl::enableAlphaBlending();
	gl::enableDepthRead();
	gl::enableDepthWrite();
}


void BasicApp::resize(ResizeEvent event) {
	mArcball.setWindowSize( getWindowSize() );
	mArcball.setCenter( getWindowCenter() );
	mArcball.setRadius( 150 );
}

void BasicApp::update() {
	if (chunks_level0.size()<4000) {
		if (rand->nextInt(100)>98) chunk = chunks_level0[rand->nextInt(0, chunks_level0.size())];
		Chunk3D::direction dir = (Chunk3D::direction) rand->randInt(8);
		int cnt = rand->nextInt(3)+1;
		for (int i=0; i<cnt; i++) chunks_level0.push_back(chunk = Chunk3D::createChunkAt(chunk, dir));
	}
}

void BasicApp::shutdown() {
}


void BasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
}

void BasicApp::draw()
{
	
	gl::clear( Color( 0, 0.1f, 0.2f ) );

	gl::pushModelView();
	gl::translate( getWindowCenter() );
	gl::scale( Vec3f( 10.0f, 10.0f, 10.0f ) );
	gl::rotate( mArcball.getQuat() );
	for (int i=0; i<Chunk3D::chunks.size(); i++) {
		Chunk3D* c = Chunk3D::chunks[i];
		float size = pow(3.0f, c->level);
		gl::color(ColorA(1.0f, 1.0f, 1.0f, 1.0f/(float)(c->level)));
		if(c->level>0) gl::drawStrokedCube( Vec3f(c->x,c->y,c->z), Vec3f( size, size, size ) );
		else gl::drawColorCube( Vec3f(c->x,c->y,c->z), Vec3f( size, size, size ) );
	}
	gl::popModelView();

}

void BasicApp::mouseDown( MouseEvent event )
{
	mArcball.mouseDown( event.getPos() );
	mCurrentMouseDown = mInitialMouseDown = event.getPos();
}

void BasicApp::mouseDrag( MouseEvent event )
{
	mArcball.mouseDrag( event.getPos() );
	mCurrentMouseDown = event.getPos();
}


CINDER_APP_BASIC( BasicApp, RendererGl )