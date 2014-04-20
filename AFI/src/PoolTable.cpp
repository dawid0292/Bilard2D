#include "PoolTable.h"
#include <vector>
#include "imageloader.h"


<<<<<<< HEAD



GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,      //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint _textureId1, _textureId2, _textureId3; //The id of the texture

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	
	Image *image = loadBMP("border.bmp");
	_textureId1 = loadTexture(image);
	Image *image1 = loadBMP("tex_ziel.bmp");
	_textureId2 = loadTexture(image1);
	Image *image2 = loadBMP("tex3.bmp");
	_textureId3 = loadTexture(image2);
	delete image;
	delete image1;
	delete image2;
}

void draw(int &i, std::vector< Vertex > vertices, std::vector< Vertex > normals, std::vector<Vertex> uv, GLuint texture, int n){
	for(int j = 0; j<n; j++){
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLE_STRIP);	
	
		glTexCoord2f(uv[i].x,uv[i].y);
=======
void PoolTable::drawTable(int n, std::vector< Vertex > vertices, std::vector< Vertex > normals)
{
	static GLfloat col[3] = {1.0f, 0.0f, 0.0f};
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < n;){ // tworzymy n/3 Å“cian trÃ³jkÂ¹tnych
		//glColor3fv(col); // kolor wg numeru Å“ciany
		//for(int j = 0; j < 3; j++)
>>>>>>> 85a4c39642cb351d0ace6921e89ad6c11e941fe1
		glNormal3f(normals[i].x, normals[i].y, normals[i].z);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		glTexCoord2f(uv[i+1].x,uv[i+1].y);
		glNormal3f(normals[i+1].x, normals[i+1].y, normals[i+1].z);
		glVertex3f(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z);
		glTexCoord2f(uv[i+2].x,uv[i+2].y);
		glNormal3f(normals[i+2].x, normals[i+2].y, normals[i+2].z);
<<<<<<< HEAD
		glVertex3f(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z);
		glEnd();	
		i+=3;
	}
}

void PoolTable::drawTable(int n, std::vector< Vertex > vertices, std::vector< Vertex > normals, std::vector<Vertex> uv)
{
	 
	initRendering();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_1D);
	int i = 0;
		//£UZA2
		draw(i,vertices,normals,uv,_textureId3,252);
		//£UZA1
		draw(i,vertices,normals,uv,_textureId3,252);
		//NAROZNIK4
		draw(i,vertices,normals,uv,_textureId1,12);
		//BANDA2
		draw(i,vertices,normals,uv,_textureId2,12);
		//£UZA4
		draw(i,vertices,normals,uv,_textureId3,252);
		//PODSTAWA
		draw(i,vertices,normals,uv,_textureId1,12);
		//NAROZNIK3
		draw(i,vertices,normals,uv,_textureId1,12);
		//POLE
		draw(i,vertices,normals,uv,_textureId2,4952);
		//£UZA5
		draw(i,vertices,normals,uv,_textureId3,414);
		//NAROZNIK2
		draw(i,vertices,normals,uv,_textureId1,12);
		//BANDA1
		draw(i,vertices,normals,uv,_textureId2,12);
		//BANDA5
		draw(i,vertices,normals,uv,_textureId2,12);
		//NAROZNIK1
		draw(i,vertices,normals,uv,_textureId1,12);
		//BANDA4
		draw(i,vertices,normals,uv,_textureId2,12);
		//£UZA6
		draw(i,vertices,normals,uv,_textureId3,256);
		//BANDA6
		draw(i,vertices,normals,uv,_textureId2,12);
		//£UZA3
		draw(i,vertices,normals,uv,_textureId3,252);
		//BANDA3
		draw(i,vertices,normals,uv,_textureId2,12);

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_1D);
		

}
=======
		glVertex3d(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z);
		//printf("%f, %f, %f \n", vertices[i].x, vertices[i].y, vertices[i].z);
		//printf("%f, %f, %f \n", vertices[i+1].x, vertices[i+1].y, vertices[i+1].z);
		//printf("%f, %f, %f \n", vertices[i+2].x, vertices[i+2].y, vertices[i+2].z);
		i += 3;

	}
	glEnd();

}/*
 glNormal3f(vn[(f[i].vn1-1)].x, vn[(f[i].vn1-1)].y, vn[(f[i].vn1-1)].z);
 glVertex3f(v[(f[i].v1-1)].x, v[(f[i].v1-1)].y, v[(f[i].v1-1)].z);

 glNormal3f(vn[(f[i].vn2-1)].x, vn[(f[i].vn2-1)].y, vn[(f[i].vn2-1)].z);
 glVertex3f(v[(f[i].v2-1)].x, v[(f[i].v2-1)].y, v[(f[i].v2-1)].z);

 glNormal3f(vn[(f[i].vn3-1)].x, vn[(f[i].vn3-1)].y, vn[(f[i].vn3-1)].z);
 glVertex3f(v[(f[i].v3-1)].x, v[(f[i].v3-1)].y, v[(f[i].v3-1)].z

 glBegin(GL_TRIANGLES);
 for(int i = 0; i < f_count; i++){
 if(f[i].type == 'c'){
 GLfloat * color = new GLfloat[4];
 color[0] = f[i].x;
 color[1] = f[i].y;
 color[2] = f[i].z;
 color[3] = 1;

 glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
 }
 else{
 glVertex3f(vn[(f[i].vn1-1)].x, vn[(f[i].vn1-1)].y, vn[(f[i].vn1-1)].z); // normals
 glVertex3f(vn[(f[i].vn2-1)].x, vn[(f[i].vn2-1)].y, vn[(f[i].vn2-1)].z); // normals
 glVertex3f(vn[(f[i].vn3-1)].x, vn[(f[i].vn3-1)].y, vn[(f[i].vn3-1)].z); // normals

 glVertex3f(v[(f[i].v1-1)].x, v[(f[i].v1-1)].y, v[(f[i].v1-1)].z);
 glVertex3f(v[(f[i].v2-1)].x, v[(f[i].v2-1)].y, v[(f[i].v2-1)].z);
 glVertex3f(v[(f[i].v3-1)].x, v[(f[i].v3-1)].y, v[(f[i].v3-1)].z);
 }
 }
 glEnd();*/
>>>>>>> 85a4c39642cb351d0ace6921e89ad6c11e941fe1
