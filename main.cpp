//=============================================================================
// Sample Application: Lighting (Per Fragment Phong)
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "glm/glm.h"
#include "Objeto3D.h"
#include "FreeImage/FreeImage.h"

//-----------------------------------------------------------------------------
GLuint texid[2];

class myWindow : public cwc::glutWindow
{
protected:
   cwc::glShaderManager SM;
   cwc::glShader *shader;
   cwc::glShader* shader1;
   GLuint ProgramObject;
   clock_t time0,time1;
   float posCamX, posCamY, posCamZ;
   bool moveXI, moveXD, moveYU, moveYD, moveZF, moveZB;
   float timer010;  // timer counting 0->1->0
   bool bUp;        // flag if counting up or down.
   // objetos que seran dibujados
   Objeto3D planta;
   Objeto3D astronauta;
   Objeto3D cristal;
   Objeto3D nave;
   Objeto3D torre;


public:
	myWindow(){}

    void initialize_textures(int numTex, char* nombreArchivo)
    {
        int w, h;
        GLubyte* data = 0;
        //data = glmReadPPM("soccer_ball_diffuse.ppm", &w, &h);
        //std::cout << "Read soccer_ball_diffuse.ppm, width = " << w << ", height = " << h << std::endl;

        //dib1 = loadImage("soccer_ball_diffuse.jpg"); //FreeImage
        
        glGenTextures(1, &texid[numTex]);
        glBindTexture(GL_TEXTURE_2D, texid[numTex]);
        glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Loading JPG file
        FIBITMAP* bitmap = FreeImage_Load(
            FreeImage_GetFileType(nombreArchivo, 0),

            nombreArchivo);  //*** Para Textura: esta es la ruta en donde se encuentra la textura

        FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
        int nWidth = FreeImage_GetWidth(pImage);
        int nHeight = FreeImage_GetHeight(pImage);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
            0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

        FreeImage_Unload(pImage);
        //
        glEnable(GL_TEXTURE_2D);
    }
    
    void moverCamara() {
        if (moveXI == true) {
            //std::cout << "entro";
            posCamX = posCamX + 0.1;
        }
        if (moveXD == true) {
            posCamX = posCamX - 0.1;
        }
        if (moveYU == true) {
            posCamY = posCamY - 0.1;
        }
        if (moveYD == true) {
            posCamY = posCamY + 0.1;
        }
        if (moveZF == true) {
            posCamZ = posCamZ + 0.1;
        }
        if (moveZB == true) {
            posCamZ = posCamZ - 0.1;
        }

        glTranslatef(posCamX, posCamY, posCamZ);
        
    }
	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
      //timer010 = 0.09; //for screenshot!
      glPushMatrix();
      //Movimiento de camara
      moverCamara();
      

      if (shader) shader->begin();
         //glRotatef(timer010*360, 0.5, 1.0f, 0.1f);
        // con esto se dibujan las mallas 
          glPushMatrix();
          nave.DibujarMalla(2.0, 0.0, 2.0, FALSE);
          glPopMatrix();

          glPushMatrix();
          planta.DibujarMalla(-4.0, 0.0, 0.0, FALSE);
          glPopMatrix();

          glPushMatrix();
          cristal.DibujarMalla(2.0, 0.0, -1.0, FALSE);
          cristal.DibujarMalla(-1.0, 0.0, 0.0, FALSE);
          glPopMatrix();

          glPushMatrix();
          glRotatef(270, 0.0, 1.0, 0.0);
          torre.DibujarMalla(-1.0, 0.0, 1.0, FALSE);
          glPopMatrix();


      if (shader) shader->end();

      //Dibuja los texturizados-------------------------------
      if (shader1) shader1->begin();
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texid[0]);
        astronauta.DibujarMalla(0.0, -0.3, 1.0, TRUE);
        glPopMatrix();
        if (shader1) shader1->end();
        
      
      glutSwapBuffers();
      glPopMatrix();

      UpdateTimer();

		Repaint();
	}

	virtual void OnIdle() {}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!
	virtual void OnInit()
	{
        initialize_textures(texid[0], "./modelos/DefaultMaterial_Base_Color.jpg");
		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
        

        // documentos que contienen las mallas
        planta.AbrirMalla("./modelos/AlienPlant.obj");
        astronauta.AbrirMalla("./modelos/Astronauta.obj");
        cristal.AbrirMalla("./modelos/Cristales.obj");
        nave.AbrirMalla("./modelos/Nave1.obj");
        torre.AbrirMalla("./modelos/soloTorre.obj");

		shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
		if (shader==0) 
         std::cout << "Error Loading, compiling or linking shader\n";
      else
      {
         ProgramObject = shader->GetProgramObject();
      }

        //*** Para Textura: abre los shaders para texturas
        shader1 = SM.loadfromFile("vertexshaderT.txt", "fragmentshaderT.txt"); // load (and compile, link) from file
        if (shader1 == 0)
            std::cout << "Error Loading, compiling or linking shader\n";
        else
        {
            ProgramObject = shader1->GetProgramObject();
        }

      time0 = clock();
      timer010 = 0.0f;
      bUp = true;

      DemoLight();
      
      posCamX = 0.0;
      posCamY = 0.0;
      posCamZ = 0.0;
      moveXI = false;
      moveXD = false;
      moveYU = false;
      moveYD = false;
      moveZF = false;
      moveZB = false;
	}

	virtual void OnResize(int w, int h)
   {
      if(h == 0) h = 1;
	   float ratio = 1.0f * (float)w / (float)h;

      glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	
	   glViewport(0, 0, w, h);

      gluPerspective(90,ratio,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   gluLookAt(0.0f,0.0f,4.0f, 
		          0.0,0.0,-1.0,
			       0.0f,1.0f,0.0f);
   }
	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y) {}    
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	virtual void OnKeyDown(int nKey, char cAscii)
	{     
        if (cAscii == 'a')
            moveXI = true;

        else if (cAscii == 'd')
            moveXD = true;
        else if (cAscii == 'w')
            moveYU = true;
        else if (cAscii == 's')
            moveYD = true;
        else if (cAscii == 'q')
            moveZF = true;
        else if (cAscii == 'e')
            moveZB = true;

		else if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		} 
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{

        if (cAscii == 'a')
            moveXI = false;

        else if (cAscii == 'd')
            moveXD = false;
        else if (cAscii == 'w')
            moveYU = false;
        else if (cAscii == 's')
            moveYD = false;
        else if (cAscii == 'q')
            moveZF = false;
        else if (cAscii == 'e')
            moveZB = false;

      else if (cAscii == 'o')      // s: Shader
         shader->enable();
      else if (cAscii == 'f') // f: Fixed Function
         shader->disable();
	}

   void UpdateTimer()
   {
      time1 = clock();
      float delta = static_cast<float>(static_cast<double>(time1-time0)/static_cast<double>(CLOCKS_PER_SEC));
      delta = delta / 4;
      if (delta > 0.00005f)
      {
         time0 = clock();
         if (bUp)
         {
            timer010 += delta;
            if (timer010>=1.0f) { timer010 = 1.0f; bUp = false;}
         }
         else
         {
            timer010 -= delta;
            if (timer010<=0.0f) { timer010 = 0.0f; bUp = true;}
         }
      }
   }

   void DemoLight(void)
   {
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);
     
     // Light model parameters:
     // -------------------------------------------
     /*
     GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
     
     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
     
     // -------------------------------------------
     // Spotlight Attenuation
     
     GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;
     
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
     
     */
     // ------------------------------------------- 
     // Lighting parameters:
     
     GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
     GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
     GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
     GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};
     /*
     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);
     */
     // -------------------------------------------
     // Material parameters:
     
     GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;
     /*
     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
     */
   }
};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() {std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------

