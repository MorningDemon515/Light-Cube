#define SDL_MAIN_HANDLED
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WIDTH 800
#define HEIGHT 600

SDL_Window* window;
SDL_GLContext glc;

ShaderFromFile* shader;
ShaderFromFile* lightshader;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        " ",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,HEIGHT,SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL
    );

    SDL_SetRelativeMouseMode(SDL_TRUE);

    glc = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window,glc);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    SDL_Event event;
    bool run = true;

    float vertices[] = {
         // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    GLuint VAO,VBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

     // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    GLuint texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("tree.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("container2_specular.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    shader = new ShaderFromFile("vertex.txt","fragment.txt");
    lightshader = new ShaderFromFile("light.vs","light.fs");

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    shader->Use();
    glUniform1i(shader->GetShaderSourceUniform("material.diffuse"),0);
    glUniform1i(shader->GetShaderSourceUniform("material.specular"),1);

    while (run)
    {

        float currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        SDL_PollEvent(&event);

        if(event.type == SDL_QUIT)
        {
            run = false;
        }

        if(event.type == SDL_MOUSEMOTION)
        {
                //printf("Mouse moved: xrel=%d, yrel=%d\n", event.motion.xrel, event.motion.yrel);
                
                int px = event.motion.x;
                int py = event.motion.y;
            
                float xpos = static_cast<float>(px);
                float ypos = static_cast<float>(py);

                if (firstMouse)
                {
                    lastX = xpos;
                    lastY = ypos;
                    firstMouse = false;
                }

                float xoffset = xpos - lastX;
                float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

                lastX = xpos;
                lastY = ypos;

                camera.ProcessMouseMovement(xoffset, yoffset);

        }

        if(event.type == SDL_MOUSEWHEEL)
        {
            int py = event.wheel.y;
            camera.ProcessMouseScroll(static_cast<float>(py));
        }

        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                run = false;
            }

        }

        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W] )
        {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }

        if(state[SDL_SCANCODE_S])
        {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }

        if(state[SDL_SCANCODE_A] )
        {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }

        if(state[SDL_SCANCODE_D])
        {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // be sure to activate shader when setting uniforms/drawing objects
        shader->Use();
        // directional light
        glUniform3f(shader->GetShaderSourceUniform("dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(shader->GetShaderSourceUniform("dirLight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(shader->GetShaderSourceUniform("dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
        glUniform3f(shader->GetShaderSourceUniform("dirLight.specular"), 0.5f, 0.5f, 0.5f);
        // point light 1
        glUniform3fv(shader->GetShaderSourceUniform("pointLights[0].position"),1,&(pointLightPositions[0])[0]);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[0].constant"), 1.0f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[0].linear"), 0.09f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[0].quadratic"), 0.032f);
        // point light 2
        glUniform3fv(shader->GetShaderSourceUniform("pointLights[1].position"),1,&(pointLightPositions[1])[0]);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[1].constant"), 1.0f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[1].linear"), 0.09f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[1].quadratic"), 0.032f);
        // point light 3
        glUniform3fv(shader->GetShaderSourceUniform("pointLights[2].position"),1,&(pointLightPositions[2])[0]);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[2].constant"), 1.0f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[2].linear"), 0.09f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[2].quadratic"), 0.032f);
        // point light 4
        glUniform3fv(shader->GetShaderSourceUniform("pointLights[3].position"),1,&(pointLightPositions[3])[0]);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(shader->GetShaderSourceUniform("pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[3].constant"), 1.0f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[3].linear"), 0.09f);
        glUniform1f(shader->GetShaderSourceUniform("pointLights[3].quadratic"), 0.032f);
        // spotLight
        glUniform3fv(shader->GetShaderSourceUniform("spotLight.position"),1,&camera.Position[0]);
        glUniform3fv(shader->GetShaderSourceUniform("spotLight.direction"),1,&camera.Front[0]);
        glUniform3f(shader->GetShaderSourceUniform("spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(shader->GetShaderSourceUniform("spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(shader->GetShaderSourceUniform("spotLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(shader->GetShaderSourceUniform("spotLight.constant"), 1.0f);
        glUniform1f(shader->GetShaderSourceUniform("spotLight.linear"), 0.09f);
        glUniform1f(shader->GetShaderSourceUniform("spotLight.quadratic"), 0.032f);
        glUniform1f(shader->GetShaderSourceUniform("spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(shader->GetShaderSourceUniform("spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));  

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        glUniformMatrix4fv(shader->GetShaderSourceUniform("model"),1,GL_FALSE,&model[0][0]);
        glUniformMatrix4fv(shader->GetShaderSourceUniform("view"),1,GL_FALSE,&view[0][0]);
        glUniformMatrix4fv(shader->GetShaderSourceUniform("projection"),1,GL_FALSE,&projection[0][0]);
        
        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);;
        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(shader->GetShaderSourceUniform("model"),1,GL_FALSE,&model[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp object
        lightshader->Use();
        glUniformMatrix4fv(lightshader->GetShaderSourceUniform("view"),1,GL_FALSE,&view[0][0]);
        glUniformMatrix4fv(lightshader->GetShaderSourceUniform("projection"),1,GL_FALSE,&projection[0][0]);
        glBindVertexArray(lightCubeVAO);
         for (unsigned int i = 0; i < 4; i++)
         {
             model = glm::mat4(1.0f);
             model = glm::translate(model, pointLightPositions[i]);
             model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
             glUniformMatrix4fv(lightshader->GetShaderSourceUniform("model"),1,GL_FALSE,&model[0][0]);
             glDrawArrays(GL_TRIANGLES, 0, 36);
         }

        SDL_GL_SwapWindow(window);
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    
    delete shader;
    SDL_GL_DeleteContext(glc);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}