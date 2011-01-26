#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Arcball.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Light.h"

using namespace ci;
using namespace ci::app;

#include <vector>
#include "Chunk3D.h"

using namespace std;

class BasicApp : public AppBasic {
 public:

	Rand* rand;

	void keyDown( KeyEvent event );
	void resize ( ResizeEvent event);
	void draw();
	void setup();
	void update();
	void shutdown();

	void mouseDown ( MouseEvent event );
	void mouseDrag ( MouseEvent event );
	void mouseWheel( MouseEvent event );

	Arcball		mArcball;
	Vec2i		mInitialMouseDown, mCurrentMouseDown;

	vector<Chunk3D*> chunks_level0;

	Chunk3D* chunk;
	bool  drawClusters;
	float zoom;

	gl::Texture myTexture;

	int drawLevel;

};


void BasicApp::setup(){

	rand = new Rand();

	chunk = new Chunk3D();
	chunks_level0.push_back(chunk);
	mCurrentMouseDown = mInitialMouseDown = Vec2i( 200, 200 );

	myTexture = gl::Texture( loadImage( loadFile( "data/wall_texture_01.png" ) ) ); 
    
	myTexture.enableAndBind();
    
    myTexture.unbind();

	drawLevel   = 0;
	drawClusters = false;

}



void BasicApp::resize(ResizeEvent event) {
	
	gl::enableAlphaBlending();
	//glEnable( GL_LIGHTING );
	gl::enableDepthRead();
	gl::enableDepthWrite();

	mArcball.setWindowSize( getWindowSize() );
	mArcball.setCenter( getWindowCenter() );
	mArcball.setRadius( 150 );

	zoom = 10.0f;
}

void BasicApp::update() {
	if (chunks_level0.size()<5000) {
		if (rand->nextInt(100)>90) chunk = chunks_level0[rand->nextInt(0, chunks_level0.size())];
		int cnt = rand->nextInt(4) + 1;
		for (int i=0; i<cnt; i++) {
			Chunk3D::direction dir = (Chunk3D::direction) rand->randInt(8);
			chunks_level0.push_back(chunk = Chunk3D::createChunkAt(chunk, dir));
		}
	}
}

void BasicApp::shutdown() {
}



void BasicApp::draw()
{
	
	gl::clear( Color( 0, 0.1f, 0.2f ) );

	gl::pushModelView();

	gl::translate( getWindowCenter() );
	gl::rotate( mArcball.getQuat() );

	// LIGHT
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHT1 );

	glLightfv(GL_LIGHT0, GL_DIFFUSE, Vec4f(1.0, 0.8, 0.6, 1.0));
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Vec4f(0.2, 0.2, 0.5, 1.0));

	glLightfv( GL_LIGHT0, GL_POSITION, Vec4f( -5*zoom, -10*zoom, -5*zoom, 1));
	glLightfv( GL_LIGHT0, GL_POSITION, Vec4f( 50*zoom, 100*zoom, 50*zoom, 1));



	gl::color(ColorA(1.0f, 1.0f, 1.0f, 1.0f));
	myTexture.enableAndBind();
    for (int i=0; i<Chunk3D::chunks.size(); i++) {
		Chunk3D* c = Chunk3D::chunks[i];
		float size = pow(3.0f, c->level);
		if (c->level == drawLevel) {
			gl::drawCube( Vec3f(c->x*zoom,c->y*zoom,c->z*zoom), Vec3f( size*zoom, size*zoom, size*zoom ) );
		}
	}
	myTexture.unbind();
	
	if(drawClusters)  {
		glDisable(GL_LIGHTING);
		gl::color(ColorA(1.0f, 1.0f, 1.0f, 0.1f));
		for (int i=0; i<Chunk3D::chunks.size(); i++) {
			Chunk3D* c = Chunk3D::chunks[i];
			float size = pow(3.0f, c->level);
		
			if(c->level > drawLevel) {
		
				gl::drawStrokedCube( Vec3f(c->x*zoom,c->y*zoom,c->z*zoom), Vec3f( size*zoom, size*zoom, size*zoom ) );
			}
		}
	}

	gl::popModelView();

}

void BasicApp::mouseDown( MouseEvent event )
{
	mArcball.mouseDown( event.getPos() );
	mCurrentMouseDown = mInitialMouseDown = event.getPos();


}

void BasicApp::mouseWheel( MouseEvent event ) {
	zoom += event.getWheelIncrement();
};

void BasicApp::mouseDrag( MouseEvent event )
{
	mArcball.mouseDrag( event.getPos() );
	mCurrentMouseDown = event.getPos();
}



void BasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );

	if( event.getChar() == 'c' )
		drawClusters = !drawClusters;

	if( event.getChar() == '0' ) drawLevel = 0;
	if( event.getChar() == '1' ) drawLevel = 1;
	if( event.getChar() == '2' ) drawLevel = 2;
	if( event.getChar() == '3' ) drawLevel = 3;
	if( event.getChar() == '4' ) drawLevel = 4;
	if( event.getChar() == '5' ) drawLevel = 5;
	if( event.getChar() == '6' ) drawLevel = 6;

}

CINDER_APP_BASIC( BasicApp, RendererGl )