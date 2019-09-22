#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <open_gl/open_gl.h>
#include <cstdlib>
#include <ctime>

float getRockPosition(float animationTime)
{
    float parameter=-4.0f*rockMaxPositionY/pow(rockAnimationTime,2);
    return parameter*animationTime*(animationTime-rockAnimationTime);
}

void mainLoop(GLFWwindow* window)
{
    //Główna pêtla
    float angle_x=0; //Aktualny kąt obrotu kamery
    float angle_y=0; //Aktualny kąt obrotu kamery
    //glm::vec3 position=glm::vec3(0.0f, 0.0f, 0.0f); //Aktualne przesunięcie kamery

    glfwSetTime(0); //Zeruj timer

    //Animacja wypadających skał - losowy kierunek i prędkość dla osi X i Z
    srand(time(NULL));


     float rockSpeedX=(float)((float)((std::rand()%401)-200)/100);
     float rockSpeedZ= (float)((float)((std::rand()%401)-200)/100);
    rockScaleModifier= (float)((float)((std::rand()%1001)-500)/10000);

    while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostaæ zamknięte
    {
        // Czas pomiędzy obecną klatką, a poprzednią
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;

        lastFrame = currentFrame;

        angle_x+=cameraRotationX*deltaTime; //Zwiększ/zmniejsz kąt obrotu na podstawie prędkoœci i czasu jaki upłynął od poprzedniej klatki
        angle_y+=cameraRotationY*deltaTime; //Zwiększ/zmniejsz kąt obrotu na podstawie prêdkoœci i czasu jaki upłynął od poprzedniej klatki

        //Lot kamienia w osi Y - funkcja kwadratowa
        rockPositionY=getRockPosition(rockAnimationCurrentTime);
        // Lot kamienia w osiach X i Z na podstawie wcześniej wylosowanych wartości
        rockPositionX+=rockSpeedX*deltaTime;
        rockPositionZ+=rockSpeedZ*deltaTime;



        drawScene(window,angle_x,angle_y); //Wykonaj procedurę rysującą
        glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeñ jakie zaszły.
        rockAnimationCurrentTime+=deltaTime;



        if(rockAnimationCurrentTime>=rockAnimationTime*2)//*2 aby wydłużyć czas opadania
        {
            //Przywrócenie skały do pozycji początkowej
            rockAnimationCurrentTime=0.0f;
            rockPositionX=0.0f;
            rockPositionZ=0.0f;
            rockPositionY=0.0f;



            //reset prędkości w osiach X i Z - nowy kierunek lotu
            rockSpeedX=(float)((float)((std::rand()%401)-200)/100);
            rockSpeedZ=(float)((float)((std::rand()%401)-200)/100);
            rockScaleModifier= (float)((float)((std::rand()%1001)-500)/10000);

        }
    }
}

int main(void)
{

    GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno


    glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

    if (!glfwInit())   //Zainicjuj bibliotekę GLFW
    {
        fprintf(stderr, "Nie można zainicjowaæ GLFW.\n");
        exit(EXIT_FAILURE);
    }

    // Utwórz okno o parametrach ustawionych w pliku config.h

window = glfwCreateWindow(700, 700, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.
glShadeModel(GL_SMOOTH);
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
    {
        fprintf(stderr, "Nie można utworzyć okna.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
    glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

    if (glewInit() != GLEW_OK)   //Zainicjuj bibliotekę GLEW
    {
        fprintf(stderr, "Nie można zainicjować GLEW.\n");
        exit(EXIT_FAILURE);
    }



    initOpenGLProgram(window); //Operacje inicjujące

    mainLoop(window);

    closeWindow(window);
}
