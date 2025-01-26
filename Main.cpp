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
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, FPS);
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

                camera.processMouseMovement(event.motion.xrel, -event.motion.yrel);

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
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_W), deltaTime);
        }

        if(state[SDL_SCANCODE_S])
        {
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_S), deltaTime);
        }

        if(state[SDL_SCANCODE_A] )
        {
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_A), deltaTime);
        }

        if(state[SDL_SCANCODE_D])
        {
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_D), deltaTime);
        }
        
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // be sure to activate shader when setting uniforms/drawing objects
        shader->Use();
        glUniform3fv(shader->GetShaderSourceUniform("light.position"),1,&lightPos[0]);
        glUniform3fv(shader->GetShaderSourceUniform("viewPos"),1,&camera.cp[0]);

        // light properties
        glUniform3f(shader->GetShaderSourceUniform("light.ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(shader->GetShaderSourceUniform("light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(shader->GetShaderSourceUniform("light.specular"), 1.0f, 1.0f, 1.0f);

        // material properties
        glUniform3f(shader->GetShaderSourceUniform("material.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(shader->GetShaderSourceUniform("material.shininess"), 64.0f);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

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
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // also draw the lamp object
        lightshader->Use();
        glUniformMatrix4fv(lightshader->GetShaderSourceUniform("view"),1,GL_FALSE,&view[0][0]);
        glUniformMatrix4fv(lightshader->GetShaderSourceUniform("projection"),1,GL_FALSE,&projection[0][0]);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        glUniformMatrix4fv(lightshader->GetShaderSourceUniform("model"),1,GL_FALSE,&model[0][0]);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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