#include "lumiere.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>

#include "imgui/backends/imgui_impl_glut.h"
#include "imgui/backends/imgui_impl_opengl2.h"

// for mac osx
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
// only for windows
#ifdef _WIN32
#include <windows.h>
#endif
// for windows and linux
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#endif

float angle_x{45.f};
float angle_y{45.f};
float distance{8.f};

GLfloat angle_satellite;
#define ANG_SPEED 0.1f //radians

Material room_material = {
    // ambient
    {.25f, .25f, .25f, 1.f},
    // diffuse
    {.25f, .25f, .25f, 1.f},
    // specular
    {.0f, .0f, .0f, 1.f},
    // shininess
    0.f
};

Material cube_material = { // green rubber
    // ambient
    { 0.0f,0.05f,0.0f,1.0f },
    // diffuse
    { 0.4f,0.5f,0.4f,1.0f},
    // specular
    {0.04f,0.7f,0.04f,1.0f },
    // shininess
    10.0f
    };

Material sphere_material = {
    // ambient
    { 0.1745f, 0.01175f, 0.01175f, 0.55f },
    // diffuse
    {0.61424f, 0.04136f, 0.04136f, 0.55f },
    // specular
    {0.727811f, 0.626959f, 0.626959f, 0.55f },
    // shininess
    10.0f
};

Light light_properties = {
    // ambient
    {.5f, .5f, .5f, .5f},
    // diffuse
    {.5f, .5f, .5f, .5f},
    // specular
    {.5f, .5f, .5f, .5f},
    // position
    {2.f, 0.f, 0.f, 1.f},
    // directional
    false,
};


/**
 * @brief Draw a room of size size centered on the origin
 * it's a cube with inverted normals, ie pointing inwards
 * @param[in] size size of the room
 */
void glRoom(GLfloat size)
{
    GLfloat v = size/2.f;

    glBegin(GL_QUADS);
        glNormal3f(.0f, .0f, 1.f);
        glVertex3f(-v, -v, -v);
        glVertex3f(v, -v, -v);
        glVertex3f(v, v, -v);
        glVertex3f(-v, v, -v);

        glNormal3f(.0f, .0f, -1.f);
        glVertex3f(v, -v, v);
        glVertex3f(-v, -v, v);
        glVertex3f(-v, v, v);
        glVertex3f(v, v, v);

        glNormal3f(-1.f, .0f, .0f);
        glVertex3f(v, -v, -v);
        glVertex3f(v, -v, v);
        glVertex3f(v, v, v);
        glVertex3f(v, v, -v);

        glNormal3f(1.f, .0f, .0f);
        glVertex3f(-v, -v, v);
        glVertex3f(-v, -v, -v);
        glVertex3f(-v, v, -v);
        glVertex3f(-v, v, v);

        glNormal3f(.0f, -1.f, .0f);
        glVertex3f(-v, v, -v);
        glVertex3f(v, v, -v);
        glVertex3f(v, v, v);
        glVertex3f(-v, v, v);

        glNormal3f(.0f, 1.f, .0f);
        glVertex3f(-v, -v, -v);
        glVertex3f(-v, -v, v);
        glVertex3f(v, -v, v);
        glVertex3f(v, -v, -v);
    glEnd();
}

void glSatellite() {
    glScalef(0.6f, 0.6f, 0.6f);
    glutSolidCube(0.5f);
    glPushMatrix();
        glTranslatef(0.25f + 0.4f, .0f, 0.f);
        glScalef(1.0f,0.05f,0.8f);
        glutSolidCube(0.8f);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.25f - 0.4f, .0f, 0.f);
        glScalef(1.0f,0.05f,0.8f);
        glutSolidCube(0.8f);
    glPopMatrix();
}


/**
 * @brief Place the camera in the scene
 */
void place_camera()
{
    glTranslatef (0.f, 0.f, -distance);
    glRotatef (angle_x, 1.f, .0f, .0f);
    glRotatef (angle_y, .0f, 1.f, .0f);
}

/**
 * @brief Place a light in the scene
 * @param[in] light light properties to use
*/
void place_light(Light& light) 
{
    
    //**********************************
    // set the light position (directional or positional)
    //**********************************
    if (light_properties.directional){
        light_properties.position[3] = 0.f;
    }
    else {
        light_properties.position[3] = 1.f;
    } 
    glLightfv(GL_LIGHT0, GL_AMBIENT, light.ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light.diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light.specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light.position);

    //**********************************
    // draw a yellow point or a yellow line from the origin in the direction of the light
    // to help visualize the light
    //**********************************
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glColor3f(1.0f,1.0f,.0f);
        

        if (light_properties.directional){
            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(light.position[0], light.position[1], light.position[2]);
            glEnd();
        }
        else {
            glTranslatef(light.position[0], light.position[1], light.position[2]);
            glutWireSphere(.10,100,100);
        }  
        
    // glBegin(GL_POINTS);
    //     glColor3f(1,1,0);
    //     glVertex3f(1,1,1);
    // glEnd();
    glPopMatrix();

    //**********************************
    // turn the light and the lighting on
    //**********************************
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

/**
 * @brief Define the material properties for the object
 * @param[in] mat material properties to use
 
*/
void define_material(const Material& mat) 
{
   //**********************************
    // set the ambient property
    //**********************************
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);

    //**********************************
    // set the diffuse property
    //**********************************
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);

    //**********************************
    // set the specular property
    //**********************************
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);

    //**********************************
    // set the shininess property
    //**********************************
    glMaterialf(GL_FRONT, GL_SHININESS, mat.shininess);
}

/**
 * @brief Place the room in the scene
 */
void place_background()
{
    glPushMatrix();
        glScalef(room_size, room_size, room_size);
        glRoom(1.f);
    glPopMatrix ();
}


/**
 * @brief OpenGL Initialization
 */
void init ()
{
    glClearColor(.0f, .0f, .0f, .0f);
    //**********************************
    // activate the Gouraud shading instead of the flat one
    //**********************************
    glShadeModel(GL_SMOOTH);

    //**********************************
    // enable face culling
    //**********************************
    glEnable(GL_CULL_FACE);

    //**********************************
    // enable the depth test
    //**********************************
    glEnable(GL_DEPTH_TEST);


    //**********************************
    // set the ambient light with glLightModelfv to a 50% grey
    //**********************************




    glEnable(GL_NORMALIZE);
}

/**
 * @brief Display callback
*/
void display(double aspect_ratio)
{
    glPushMatrix();
        // define the projection transformation
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, aspect_ratio, 1, 60);

        glPushMatrix();

            glMatrixMode(GL_MODELVIEW);

            glLoadIdentity();

            //place the camera
            place_camera();

            //**********************************
            // place the light in the scene using place_light
            //**********************************
            place_light(light_properties);


            //**********************************
            // define the material for the room (instead of color)
            //**********************************
            define_material(room_material);



            place_background();

            // the 2 objects
            // red shining sphere
            glPushMatrix ();
                glTranslatef (-4.f, 0.f, 0.f);

                //**********************************
                // define the material for the sphere (instead of color)
                //**********************************
                define_material(sphere_material);



                glutSolidSphere(0.8f, 24, 12);

            glPopMatrix();

            // satellite
            glPushMatrix ();
                glRotatef (angle_satellite, 1.f, .0f, .0f);
                glTranslatef (-4.f, 2.f, 0.f);

                //**********************************
                // define the material for the sphere (instead of color)
                //**********************************
                define_material(cube_material);
                
                
                glSatellite();

            glPopMatrix();

            // green cube
            // glPushMatrix();
            //     glTranslatef(2.f, 0.f, 0.f);

            //     //**********************************
            //     // define the material for the cube (instead of color)
            //     //**********************************
            //     define_material(cube_material);



            //     glutSolidCube(2.0);

            // glPopMatrix ();

        glPopMatrix();
    glPopMatrix();
}

/**
 * @brief Callback for keyboard events
 * @param[in] key key pressed
 * @param[in] x not used
 * @param[in] y not used
 */
void keyboard(unsigned char key, int x, int y)
{
    switch (key) 
    {
        //**********************************
        // directional light, use global variable directional
        //**********************************
        case 'd':
            // light_properties.directional = not(light_properties.directional);
        break;

        //**********************************
        // Shininess: 's' to decrement, 'S' to increment
        //**********************************
        case 'S':







        break;

        case 's':




        break;

        case 'q':
        case 27: // [ESC]
            exit(0);
        break;
        default:
        break;
    }
    ImGui_ImplGLUT_KeyboardFunc(key, x, y);
    glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////
//
// NOTHING TO DO BELOW THIS LINE
//
////////////////////////////////////////////////////////////////////////

/**
 *	@brief Callback for window size change
 *	@param[in] w new width of the window
 *	@param[in] h new height of the window
 */
void reshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (60, (GLfloat)w/(GLfloat)h, 0.1, 50);
}

/**
 * Callback for special keys
 */
void special(int key, int x, int y)
{
    static constexpr float DELTA_ANGLE_X{5.f};
    static constexpr float DELTA_ANGLE_Y{5.f};
    static constexpr float DELTA_DISTANCE{.3f};
    static constexpr float DISTANCE_MIN{.0f};

    switch (key) {
        case GLUT_KEY_UP:
            angle_x = std::fmod(angle_x + DELTA_ANGLE_X, 360.f);
        break;
        case GLUT_KEY_DOWN:
            angle_x = std::fmod(angle_x - DELTA_ANGLE_X, 360.f);
        break;
        case GLUT_KEY_LEFT:
            angle_y = std::fmod(angle_y + DELTA_ANGLE_Y, 360.f);
        break;
        case GLUT_KEY_RIGHT:
            angle_y = std::fmod(angle_y - DELTA_ANGLE_Y, 360.f);
        break;
        case GLUT_KEY_PAGE_DOWN:
            distance += DELTA_DISTANCE;
        break;
        case GLUT_KEY_PAGE_UP:
            distance -= (distance > DISTANCE_MIN)? DELTA_DISTANCE: .0f;
        break;

        default:
            break;
    }
    ImGui_ImplGLUT_SpecialFunc(key, x, y);
    glutPostRedisplay();
}

/**
 * @brief Render the scene and the menu
 */
void render()
{
    // ImGui_ImplOpenGL2_NewFrame();
    // ImGui_ImplGLUT_NewFrame();
    // ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    // {

    //     ImGui::Begin("Scene controls");

    //     // ImGui::Text("Room");
    //     // ImGui::ColorEdit4("Room ambient", (float*)&room_material.ambient, ImGuiColorEditFlags_Float);
    //     // ImGui::ColorEdit4("Room diffuse", (float*)&room_material.diffuse, ImGuiColorEditFlags_Float);
    //     // ImGui::ColorEdit4("Room specular", (float*)&room_material.specular, ImGuiColorEditFlags_Float);
    //     // ImGui::SliderFloat("Room shininess", &room_material.shininess, .0f, max_shininess);

    //     // // insert a separator
    //     // ImGui::Separator();
    //     // ImGui::Text("Light position");
    //     // ImGui::Checkbox("Directional", &light_properties.directional);
    //     // ImGui::ColorEdit4("Light ambient", (float*)&light_properties.ambient, ImGuiColorEditFlags_Float);
    //     // ImGui::ColorEdit4("Light diffuse", (float*)&light_properties.diffuse, ImGuiColorEditFlags_Float);
    //     // ImGui::ColorEdit4("Light specular", (float*)&light_properties.specular, ImGuiColorEditFlags_Float);
    //     // ImGui::SliderFloat3("Light position", (float*)&light_properties.position, -room_size, room_size);
    //     // ImGui::Separator();
    //     // // ImGui::Text("Cube");
    //     // // ImGui::ColorEdit4("Cube ambient", (float*)&cube_material.ambient, ImGuiColorEditFlags_Float);
    //     // // ImGui::ColorEdit4("Cube diffuse", (float*)&cube_material.diffuse, ImGuiColorEditFlags_Float);
    //     // // ImGui::ColorEdit4("Cube specular", (float*)&cube_material.specular, ImGuiColorEditFlags_Float);
    //     // // ImGui::SliderFloat("Cube shininess", &cube_material.shininess, .0f, max_shininess);

    //     // // ImGui::Separator();
    //     // ImGui::Text("Sphere");
    //     // ImGui::ColorEdit4("Sphere ambient", (float*)&sphere_material.ambient, ImGuiColorEditFlags_Float);
    //     // ImGui::ColorEdit4("Sphere diffuse", (float*)&sphere_material.diffuse, ImGuiColorEditFlags_Float);
    //     // ImGui::ColorEdit4("Sphere specular", (float*)&sphere_material.specular, ImGuiColorEditFlags_Float);
    //     // ImGui::SliderFloat("Sphere shininess", &sphere_material.shininess, .0f, max_shininess);

    //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    //     ImGui::End();
    // }
    // ImGui::Render();

    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushAttrib(GL_ALL_ATTRIB_BITS);

        display(io.DisplaySize.x/io.DisplaySize.y);

    glPopAttrib();

    // ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers ();
    glutPostRedisplay();
}

void timerCallback(int value){
    angle_satellite += ANG_SPEED;

    glutPostRedisplay();

    glutTimerFunc(10, timerCallback, value);

}

int main (int argc, char **argv)
{
    glutInit (&argc,argv);
    // enable the double buffer and the depth buffer
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // main window
    //glutInitWindowSize (640,480);
    glutInitWindowSize(1280, 1280);
    glutInitWindowPosition(50,50);
    glutCreateWindow ("Testing OpenGL light");

    // // callbacks
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);

    // glutKeyboardFunc (keyboard);
    // glutSpecialFunc (special);

    init();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    // FIXME: Consider reworking this example to install our own GLUT funcs + forward calls ImGui_ImplGLUT_XXX ones, instead of using ImGui_ImplGLUT_InstallFuncs().
    ImGui_ImplGLUT_Init();
    ImGui_ImplOpenGL2_Init();

    // Install GLUT handlers (glutReshapeFunc(), glutMotionFunc(), glutPassiveMotionFunc(), glutMouseFunc(), glutKeyboardFunc() etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    ImGui_ImplGLUT_InstallFuncs();
    // glutReshapeFunc (reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glutTimerFunc(10, timerCallback, 0);

    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return EXIT_SUCCESS;
}
