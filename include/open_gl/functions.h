#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <constants.h>
#include <config.h>
#include <model.h>

float aspectRatio=1;

Model *testModel;
Model *testModel2;
Model *water;
Model *sky;
Model *rock;
Shader *testShader;

float waves=0;
bool isWavesDirectionReverted=false;
float waveRotation=0.0f;

float cameraRotationX=0;
float cameraRotationY=0;
float cameraPosition=0;

bool isRockFalling=false;
float rockPositionX=0;
float rockPositionY=0;
float rockPositionZ=0;
float rockAnimationCurrentTime=0;
float rockAnimationTime=5;
float rockMaxPositionY=6;

float rockScaleModifier=0.0f;




// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 cameraPos   = glm::vec3(0.0f, 4.0f, -17.0f);
glm::vec3 cameraPosMovement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront =  glm::vec3(0.0f,0.0f,1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f,1.0f,0.0f);

short movementDirection=0;//0 - brak ruchu, 1 - do przodu, 2 - w prawo, 3 - na dół, 4 - w lewo

    float cameraSpeed = 2;
    bool firstMouse=true;
float yaw   = 90.0f;	// yaw jest inicjalizowane na 90 stopni, ponieważ 0 stopni  wskazuje na lewo od środka sceny
float pitch =  0.0f;
float lastX =  700.0f / 2.0;
float lastY =  700.0f / 2.0;

glm::vec3 pathFromCenter=glm::vec3(1.0f);

//Procedura obsługi błędów
void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

void windowResizeCallback(GLFWwindow* window,int width,int height)
{
    if (height==0)
        return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

         if (movementDirection==1)
{
                 cameraPosMovement = cameraSpeed * cameraFront;

}

                     if (movementDirection==4)
                     {
                         cameraPosMovement =-1.0f* glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                     }

                                  if (movementDirection==2)
                                  {
                                      cameraPosMovement = glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

                                  }

                    if (movementDirection==3)
                    {

                               cameraPosMovement =-1.0f*  cameraSpeed * cameraFront;
                    }


    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods)
{
cameraSpeed = 15.0f * deltaTime;
    if (action==GLFW_PRESS)
    {
                if (key==GLFW_KEY_ESCAPE)
            exit(0);

        if (key==GLFW_KEY_LEFT)
            cameraRotationX=PI/2;
        if (key==GLFW_KEY_W)
{
                 cameraPosMovement = cameraSpeed * cameraFront;
             movementDirection=1;
}

                     if (key==GLFW_KEY_A)
                     {
                         cameraPosMovement =-1.0f* glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                         movementDirection=4;
                     }

                                  if (key==GLFW_KEY_D)
                                  {
                                      cameraPosMovement = glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                                      movementDirection=2;
                                  }

                    if (key==GLFW_KEY_S)
                    {

                               cameraPosMovement =-1.0f*  cameraSpeed * cameraFront;
                                                                     movementDirection=3;
                    }

        if (key==GLFW_KEY_RIGHT)
            cameraRotationX=-PI/2;
        if (key==GLFW_KEY_UP)
            cameraRotationY=-PI/2;
        if (key==GLFW_KEY_DOWN)
            cameraRotationY=PI/2;
    }
    if (action==GLFW_RELEASE)
    {
                if (key==GLFW_KEY_W)
                {
                    cameraPosMovement= glm::vec3(0.0f, 0.0f, 0.0f);
                     movementDirection=0;
                }

                                if (key==GLFW_KEY_S)
                                {
                                                        cameraPosMovement= glm::vec3(0.0f, 0.0f, 0.0f);
                                     movementDirection=0;
                                }

                                    if (key==GLFW_KEY_A)
                                    {
                                                         cameraPosMovement= glm::vec3(0.0f, 0.0f, 0.0f);
                                         movementDirection=0;
                                    }

                                    if (key==GLFW_KEY_D)
                                    {
                                                    cameraPosMovement= glm::vec3(0.0f, 0.0f, 0.0f);
                                         movementDirection=0;
                                    }


        if (key==GLFW_KEY_LEFT)
            cameraRotationX=0;
        if (key==GLFW_KEY_RIGHT)
            cameraRotationX=0;
        if (key==GLFW_KEY_UP)
            cameraRotationY=0;
        if (key==GLFW_KEY_DOWN)
            cameraRotationY=0;

    }
}



//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window)
{

    //************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);

    glfwSetWindowSizeCallback(window,windowResizeCallback);
    glfwSetKeyCallback(window,keyCallback);

    testShader=new Shader("v_light.glsl", "f_light.glsl");
    testModel= new Model("models/island/dream_island.obj");
    testModel2= new Model("models/volcano2/volcano.obj");
    water= new Model("models/ocean/ocean.obj");
    sky= new Model("models/sky-round/sky.obj");
    rock= new Model("models/rock/rock.obj");
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window)
{
    //************Tutaj umieszczaj kod, który nale¿y wykonać po zakoñczeniu pętli głównej************
    delete testModel;
    delete testShader;
}

void closeWindow(GLFWwindow* window)
{
    freeOpenGLProgram(window);
    glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
    glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
    exit(EXIT_SUCCESS);
}

//Procedura rysująca zawartoœæ sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y)
{
    cameraPos+=cameraPosMovement;

    if(isWavesDirectionReverted&&waves<=-20 || !isWavesDirectionReverted&&waves>=20)
    {
        isWavesDirectionReverted=!isWavesDirectionReverted;
    }

    if(isWavesDirectionReverted)
    {

        waves-=0.06;
    }
    else
    {

        waves+=0.06;
    }
waveRotation+=0.01;

    //************Tutaj umieszczaj kod rysuj¹cy obraz******************l
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//cameraPos+=position;
glm::mat4 V=glm::lookAt(
                    cameraPos,
                    cameraPos+cameraFront,
                    cameraUp); //Wylicz macierz widoku
//                    V=glm::rotate(V,-angle_x,glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz modelu
//                   V = glm::translate(V, glm::vec3(0.0f, 0.0f, -angle_y*0.2)); // translate it down so it's at the center of the scene
//V = glm::translate(V, glm::vec3(0.0f, 0.0f, position));
                            V=glm::rotate(V,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu
                            V=glm::rotate(V,angle_x,glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz modelu


    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 100.0f); //Wylicz macierz rzutowania

    testShader->use();//Aktywacja programu cieniuj¹cego



    testShader->setVec3("light.direction", -0.2f, -1.0f, -0.3f);

        // light properties
        testShader->setVec3("light.ambient", 0.45f, 0.45f, 0.45f);
        testShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        testShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
                testShader->setInt("material.diffuse", 0);
    testShader->setInt("material.specular", 1);
        testShader->setFloat("material.shininess", 32.0f);

    //Przeslij parametry programu cieniującego do karty graficznej


   //V=glm::rotate(V,angle_x,glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz modelu
   testShader->setMat4("V", V);

   testShader->setMat4("P", P);

        // render the loaded model
        glm::mat4 M = glm::mat4(1.0f);
         M = glm::scale(M, glm::vec3(0.025f, 0.025f, 0.025f));	// it's a bit too big for our scene, so scale it down
glm::vec3 movement=glm::vec3(0.0f, 0.0f, angle_y);
            pathFromCenter*=movement;
        //M = glm::translate(M,pathFromCenter); // translate it down so it's at the center of the scene


       // M = glm::translate(M,-pathFromCenter); // translate it down so it's at the center of the scene


        M = glm::translate(M, glm::vec3(0.0f, -1.0f, 0.0f)); // translate it down so it's at the center of the scene
        //M = glm::scale(M, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
//M = glm::translate(M, movement); // translate it down so it's at the center of the scene




//            M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu




    testShader->setMat4("M", M);

        testModel->Draw(*testShader);

        M = glm::mat4(1.0f);
        M = glm::translate(M, glm::vec3(0.0f, 1.75f, 0.0f)); // translate it down so it's at the center of the scene
        M = glm::scale(M, glm::vec3(1.8f, 1.8f, 1.8f));	// it's a bit too big for our scene, so scale it down
//            M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu
    testShader->setMat4("M", M);

        testModel2->Draw(*testShader);

               M = glm::mat4(1.0f);
        M = glm::translate(M, glm::vec3(rockPositionX, 4.0f+rockPositionY, rockPositionZ)); // translate it down so it's at the center of the scene
        M = glm::scale(M, glm::vec3(0.1f+rockScaleModifier, 0.1f+rockScaleModifier, 0.1f+rockScaleModifier));	// it's a bit too big for our scene, so scale it down
//            M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu
    testShader->setMat4("M", M);

        rock->Draw(*testShader);


    //testShader->setMat4("M", M);

        //testModel->Draw(*testShader);

        M = glm::mat4(1.0f);
        M = glm::translate(M, glm::vec3(0.0f, 0.2f, 0.0f)); // translate it down so it's at the center of the scene
        M = glm::scale(M, glm::vec3(40.0f, 20.0f+waves, 40.0f));	// it's a bit too big for our scene, so scale it down
        //M = glm::rotate(M,waveRotation,glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz modelu
    testShader->setMat4("M", M);

        water->Draw(*testShader);

       //     testShader->setMat4("M", M);

        //testModel->Draw(*testShader);

        M = glm::mat4(1.0f);
        M = glm::translate(M, glm::vec3(0.0f, -4.0f, 0.0f)); // translate it down so it's at the center of the scene
        M = glm::scale(M, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down
//            M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu
    testShader->setMat4("M", M);

        sky->Draw(*testShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
}
