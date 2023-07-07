// #include <GL/glew.h>
#include "../includes/GLEW/include/GL/glew.h"
#include <GLFW/glfw3.h>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>

#include "../Renderer.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

struct NDC_MousePos
{
    float x;
    float y;
};

struct s_vec2
{
    float x;
    float y;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1,
    };
    std::ifstream     shaderFile;
    std::string     vertexShader;
    std::string     fragmentShader;
    std::stringstream ss[2];
    std::string     line;
    int i;

    shaderFile.open(filepath);
    ShaderType mode = ShaderType::NONE;
    i = 0;
    if (shaderFile.is_open())
    {
        while (getline(shaderFile, line))
        {
            // std::cout << "toggle: ," << static_cast<int>(mode) << " line: " << line << std::endl;
        	if (!line.empty() && line.back() == '\r') // This if-statement is necessary to work on Linux
        	{
            	line.pop_back();
        	}

            if (line.compare("#shader fragment") == 0)
            {
				std::cout << " JOOOOOO" << std::endl;
                mode = ShaderType::FRAGMENT;
                i = 0;
            }
            else if (line.compare("#shader vertex") == 0)
            {
				std::cout << " JOOOOOO" << std::endl;
                mode = ShaderType::VERTEX;
				std::cout << "Mode:" << static_cast<int>(mode) << std::endl;
            }
            if (i != 0 && mode != ShaderType::NONE)
                ss[static_cast<int>(mode)] << line << '\n';
            i++;
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char*  message = (char *)alloca(length * sizeof(char)); 
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return (0);
    }
    return (id);
 }

unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();

	if (program == 0)
		std::cout << "Failed to create the program object (glCreateProgram())" << std::endl;

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    if (vs == 0 || fs == 0)
		exit (-1);

	glAttachShader(program, vs);
    glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

	return (program);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        std::cout << "Mouse clicked" << std::endl;
}

static NDC_MousePos getMousePosNDC(GLFWwindow* window, int width, int height)
{
    NDC_MousePos ndcMouse;
    double xpos, ypos;

    glfwGetCursorPos(window, &xpos, &ypos);
    ndcMouse.x = static_cast<float>(xpos) / width * 2.0f - 1.0f;
    ndcMouse.y = 1.0f - static_cast<float>(ypos) / height * 2.0f;
    

    std::cout << "X: " << ndcMouse.x << std::endl;
    std::cout << "Y: " << ndcMouse.y << std::endl;
    return (ndcMouse);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(void)
{
    GLFWwindow*     window;
    NDC_MousePos    mouse;
    const int       width = 1600;
    const int       height = 1600;
    const int       nOfShapes = 150;
    s_vec2          t_pos;


    

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Changed from 3 to 4, might give errors!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "OpenGL_Screensaver_experiment", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing glewInit();" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;


    float t1[] = {
        -1.0f, 	1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
    };

    float t2[] = {
         1.0f, 	1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
    };

    int indexCount = (sizeof(t1) / sizeof(float));
    int nOfVertices = indexCount / 3;

    std::cout << "N of vertices: " << nOfVertices << std::endl;

    unsigned int indices[] = { //These indices define a triangle position
    0, 1, 2,
    2, 3, 0
    };

    glfwSwapInterval(1); //Smooth transition between frames

    unsigned int vao1, vao2;
    glGenVertexArrays(1, &vao1);
    glGenVertexArrays(1, &vao2);


    glBindVertexArray(vao1);
    VertexBuffer vb(t1, sizeof(t1));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(vao2);
    VertexBuffer vb2(t2, sizeof(t2));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glEnableVertexAttribArray(0);


    // IndexBuffer ib(indices, 6);
    ShaderProgramSource shaderSource = ParseShader("/home/flip/Documents/OpenGL_Screensaver/res/shaders/Strength.shader");

    unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    glUseProgram(shader);

    int color = glGetUniformLocation(shader, "u_Color");
    int transformLoc = glGetUniformLocation(shader, "u_Transform");
    int sineDriver = glGetUniformLocation(shader, "sineDriver");
    int timeFrag = glGetUniformLocation(shader, "u_Time");

    float inc = 0.0001f;
    float incX;
    float incY;
    // float x = 0;
	// float y = 0;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    t_pos.x = -0.75;
    t_pos.y = 0.0;

    glUniform3f(transformLoc, t_pos.x, t_pos.y, 0.0);
    glBindVertexArray(vao1);

    float randomX[nOfShapes];
    float randomY[nOfShapes];
    float max = 2.0f;
    float min = -2.0f;
    for (int i = 0; i < nOfShapes; i++)
    {
        randomX[i] = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
        randomY[i] = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    }
    while (!glfwWindowShouldClose(window))
    {
        //Clearing the screen
        glClearColor(0.9f, 0.9f, 0.9f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);


        //Input
        mouse = getMousePosNDC(window, width, height);
        incX = mouse.x - t_pos.x;
        incY = mouse.y - t_pos.y;
        inc += 0.001f;
        t_pos.x += incX / 50;
        t_pos.y += incY / 50;
        processInput(window);

       	glUniform1f(timeFrag, inc * 4);
		glUniform2f(transformLoc, mouse.x, mouse.y);

        //Drawcall
        // for (int i = 0; i < nOfShapes; i++)
        // {
        //     glUniform3f(transformLoc, t_pos.x + randomX[i], t_pos.y + randomY[i], 0.0);
        //     glUniform1f(sineDriver, sin(inc));
        //     glUniform3f(color, randomX[i], randomY[i], sin(inc));
        //     glDrawArrays(GL_TRIANGLES, 0, nOfVertices);
        // }

    	glBindVertexArray(vao1);
        glDrawArrays(GL_TRIANGLES, 0, nOfVertices);
    	glBindVertexArray(vao2);
        glDrawArrays(GL_TRIANGLES, 0, nOfVertices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
