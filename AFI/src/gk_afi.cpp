#include <iostream>

#include "zpr.h"
#include "Physicist.h"
#include "Ball.h"
#include "PoolTable.h"
#include "Cue.h"
#include "Vertex.h"
#include <GL/glut.h>
#include <osg/Matrix>
#include <osg/Vec3>
#include <osg/Quat>
#include <vector>
#include "imageloader.h"

int i = 0;





Ball whiteBall = Ball();
Physicist physicist;
PoolTable poolTable;
Cue cue;

const int FRAMES_PER_SECOND = 10;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
DWORD next_game_tick;
int sleep_time = 0;


bool loadOBJ(const char * path, std::vector< Vertex> & out_vertices, std::vector< Vertex > & out_uvs, std::vector< Vertex > & out_normals){
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< Vertex> temp_vertices;
	std::vector< Vertex> temp_uvs;
	std::vector< Vertex > temp_normals;
	int max = 0;

	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}

	while( 1 ){
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		//std::cout<<lineHeader<<std::endl;
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		// else : parse lineHeader
		if ( strcmp( lineHeader, "v" ) == 0 ){
			Vertex vertex;
			fscanf(file, "%lf %lf %lf\n", &vertex.x, &vertex.y, &vertex.z );
			//std::cout<<vertex.x<<" "<<vertex.y<<" "<<vertex.z<<std::endl;
			//Sleep(1000);
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			Vertex uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			//std::cout<<uv.x<<" "<<uv.y<<" "<<std::endl;
			//Sleep(1000);
			uv.y = -uv.y;
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			Vertex normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			//std::cout<<normal.x<<" "<<normal.y<<" "<<normal.z<<std::endl;
			//Sleep(1000);
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			if(max < vertexIndex[0])
				max = vertexIndex[0];
			if(max < vertexIndex[1])
				max = vertexIndex[1];
			if(max < vertexIndex[2])
				max = vertexIndex[2];
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			//std::cout<<vertexIndex[0]<<" "<<vertexIndex[1]<<" "<<vertexIndex[2]<<std::endl;
			//Sleep(100);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	// For each vertex of each triangle
	std::cout<<vertexIndices.size()<<std::endl;
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		//std::cout<<temp_vertices[i].x<<" "<<temp_vertices[i].y<<" "<<temp_vertices[i].z<<std::endl;
		//Sleep(1000);
		Vertex vertex = temp_vertices[ vertexIndex-1 ];
		Vertex uv = temp_uvs[uvIndex -1];
		Vertex normal = temp_normals[normalIndex-1];
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
		//std::cout<<vertex.x<<" "<<vertex.y<<" "<<vertex.z<<std::endl;
		//std::cout<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<std::endl;
		//Sleep(1000);
	}
	std::cout<<max<<std::endl;
	//Sleep(10000);
};

std::vector< Vertex > vertices;
std::vector< Vertex > uvs;
std::vector< Vertex > normals;



/*void DrawFigure(int n, GLfloat v[][3], int s[][3])
{
static GLfloat col[3] = {1.0f, 0.0f, 0.0f};
glBegin(GL_TRIANGLES);
for(int i = 0; i < n; i ++){      // tworzymy n œcian trójk¹tnych
glColor3fv(col);             // kolor wg numeru œciany
for(int j = 0; j < 3; j++)
glVertex3fv(v[ s[i][j] ]); 
}
glEnd();

}*/


//! TODO
//! Tutaj inicjalizowane s¹ pierwsze pozycje obiektów
void initObjects()
{
	
	/*
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)800/(GLfloat)600,0.1f,300.0f);
	glTranslatef(0.0f,0.0f,0.0f);   // przesuwamy pocz¹tkowy uk³ad w ty³ o 6 jednostek
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);  // w³¹cza bufor g³êbokoœci
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);   // w³¹cza opcjê eliminacji œcian
	glFrontFace(GL_CW);       // œciany o wierzcho³kach u³o¿onych zgodnie z ruchem wskazówek
	// zegara bêd¹ traktowane jako zwrócone przodem
	glCullFace(GL_BACK);      // pomija rysowanie œcian odwróconych ty³em
	*/

}



//! \param objectMatrix Macierz pisuj¹ca pozycjê 3D obiektu na scenie
void refreshObject(const osg::Matrix & objectMatrix)
{
	auto t = objectMatrix.getTrans();
	glTranslated(t.x(), t.y(), t.z());

	auto s = objectMatrix.getScale();
	glScaled(s.x(), s.y(), s.z());

	double angle, x, y, z;
	objectMatrix.getRotate().getRotate(angle, x, y, z);
	glRotated(osg::RadiansToDegrees(angle), x, y, z);
}



// Drawing (display) routine.
void drawScene()
{
		//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, _textureId);
	
	//Bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	// Clear screen to background color.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Ball whiteBall = Ball();
	//std::cout<<vertices[vertices.size() - 1].x<<std::endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity;
	
	
	
	poolTable.drawTable(vertices.size() , vertices, normals, uvs);

	//DrawFigure(28, V_table, S_table);
	
	//ja bym to usunal
	//Tworzenie Kuli bia³ej:)
	whiteBall.drawBall(physicist, 1.0,1.0,1.0);
	cue.drawCue(whiteBall, 1.0,1.0,1.0);
	//physicist.moveBall(whiteBall);*/
	//whiteBall.changePosition(0.0, 0.0, (double)++i/1000.0);
	/*glPushMatrix();
	glTranslatef(0.5,0.05,0);
	glColor3d(1.0,1.0,1.0);
	glScalef(0.05, 0.05, 0.05);
	glutSolidSphere(1,20,10);

	glPopMatrix();*/
	//std::cout<<"jestes w drawScene"<<std::endl;
	//for(i = 0;i < 3000000000; i++);
	//std::cout<<i<<std::endl;

	// Swap buffers for double buffering
	glutSwapBuffers();

	
}

//! Metoda realizuj¹ca obliczenia na potrzeby kolejnych klatek, generuje animacjê
void animate() {

	//whiteBall.drawBall(physicist, 1.0,1.0,1.0);
	//std::cout<<"jestes w animate"<<std::endl;
	physicist.moveBall(whiteBall);
	drawScene();
	

	glutPostRedisplay();


	next_game_tick += SKIP_TICKS;
	sleep_time = next_game_tick - GetTickCount();
	if( sleep_time >= 0 ) {
		Sleep( sleep_time );
	}


}
//! Zmienne opisuj¹ce materia³ i œwiat³o OpenGL
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f }; 

// Initialization routine.
void setup() 
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5, 0.5, 0.5, 0.5);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); 

	next_game_tick = GetTickCount();
	// Register display routine.
	glutDisplayFunc(drawScene);
	// Register idle routine
	glutIdleFunc(animate);
	// Initialize camera manipulator
	zprInit();
	// Initialize first object positions
	initObjects();
}

// Main routine: defines window properties, creates window,
// registers callback routines and begins processing.
int main(int argc, char **argv) 
{  
	bool res = loadOBJ("stol.obj", vertices, uvs, normals);
	// Initialize GLUT.
	glutInit(&argc, argv);

	// Set display mode as double-buffered, RGB color and depth.
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); 

	// Set OpenGL window size.
	glutInitWindowSize(800, 800);

	// Set position of OpenGL window upper-left corner.
	glutInitWindowPosition(50, 50); 


	//gluLookAt( 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );


	
	std::cout<<" "<<vertices.size();
	for(int i = 0; i < vertices.size(); i++){
		//std::cout<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<std::endl;
	}
	//Sleep(10000);

	// Create OpenGL window with title.
	glutCreateWindow("Bilard 3D5");

	// Initialize.
	setup(); 
	// Begin processing.
	glutMainLoop(); 

	return 0;  
}