#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>

#include "../include/Scene.h"
#include "../include/MyGlWindow.h"
#include "../include/SettingsManager.h"

Scene* MyGlWindow::scene_;
bool MyGlWindow::mouse_pressed_;
TwBar* MyGlWindow::load_file_bar_;

MyGlWindow::MyGlWindow()
{
  if (InitGLFW() != 0)
    std::cout << "ERROR: Failed to initialize GLFW!" << std::endl;
  if (InitOpenGL() != 0)
    std::cout << "ERROR: Failed to initialize GLEW!" << std::endl;
  int width, height;
  glfwGetWindowSize(window_, &width, &height);
  float aspect_ratio = static_cast<float>(width) / height;
  scene_ = new Scene::Scene(new Camera(glm::vec3(0.0f, 0.0f, 0.0f),
          200.0f,
          0.1f,
          aspect_ratio));
  InitTW();

  RenderFunction = &Scene::Render;
}

MyGlWindow::~MyGlWindow()
{
  glfwTerminate();
  TwTerminate();
  delete scene_;
}

int MyGlWindow::InitGLFW()
{
	// Initialize glfw
  if (!glfwInit())
      return -1;
  // Modern OpenGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Create a windowed mode window and its OpenGL context
  window_ = glfwCreateWindow(640, 480, "Shader Tests", NULL, NULL);
  if (!window_)
  {
      glfwTerminate();
      return -1;
  }
  // Make the window_'s context current
  glfwMakeContextCurrent(window_);
  // Set callback functions
  //glfwSetErrorCallback(window_, (GLFWerrorcallbackfun)ErrorCallback);
  glfwSetMouseButtonCallback(window_, (GLFWmousebuttonfun)MouseButtonCallback);
  glfwSetCursorPosCallback(window_, (GLFWcursorposfun)MousePosCallback);
  glfwSetScrollCallback(window_, (GLFWscrollfun)ScrollCallback);
  glfwSetWindowSizeCallback(window_, WindowSizeCallback);
  glfwSetKeyCallback(window_, (GLFWkeyfun)KeyCallback);
  glfwSetCharCallback(window_, (GLFWcharfun)CharCallback);

  return 0;
}

int MyGlWindow::InitOpenGL()
{
  glClearColor(0.1f,0.1f,0.2f,0.0f);
  glEnable(GL_DEPTH_TEST);
  // Initialize GLEW
  glewExperimental = true; // Needed in core profile
  if (glewInit() != GLEW_OK) {
      return -1;
  }
  return 0;
}

int MyGlWindow::InitTW()
{
  int width, height;
  glfwGetWindowSize(window_, &width, &height);
    // Initialize anttweakbar
  TwInit(TW_OPENGL_CORE, NULL);
  TwWindowSize(width*2, height*2);

  TwBar *menu_bar;
  menu_bar = TwNewBar("Menu");
  //TwAddSeparator(menu_bar, NULL, " group='File'");
  //TwAddSeparator(menu_bar, NULL, " group='Shader properties'");


  TwAddButton(menu_bar, "Load obj mesh", PreLoadButtonCallback, scene_, " group=File ");
  //TwAddButton(load_file_bar, "Load", LoadButtonCallback, scene_, "");

  TwAddVarRW(
          menu_bar,
          "Number of blur loops",
          TW_TYPE_UINT8,
          &SettingsManager::Instance()->n_blur_loops,
          "min=0 max=16 step=1 group='Shader properties' ");
  TwAddVarRW(
          menu_bar,
          "Blur filter size",
          TW_TYPE_UINT8,
          &SettingsManager::Instance()->filter_size,
          " group='Shader properties' ");
  TwAddVarRW(
          menu_bar,
          "Multiplier 1",
          TW_TYPE_FLOAT,
          &SettingsManager::Instance()->multiplier1,
          "min=-1.0 max=2.0 step=0.1 group='Shader properties' ");
  TwAddVarRW(
          menu_bar,
          "Multiplier 2",
          TW_TYPE_FLOAT,
          &SettingsManager::Instance()->multiplier2,
          "min=-1.0 max=2.0 step=0.1 group='Shader properties' ");
  TwAddVarRW(
          menu_bar,
          "Ambient light intensity", 
          TW_TYPE_FLOAT,
          &scene_->amb_light_.intensity,
          "min=0.0 max=1.0 step=0.01 group='Shader properties' ");
  TwAddVarRW(
          menu_bar,
          "Ambient light color", 
          TW_TYPE_COLOR3F,
          &scene_->amb_light_.color,
          " group='Shader properties' ");

/*  TwEnumVal renderEV[] = { { MyGlWindow::PHONG, "Phong"}, 
                             { MyGlWindow::TOON,  "Toon" } };
*/


  for (int i = 0; i < SettingsManager::Instance()->N_LIGHTSOURCES; ++i)
  {
    std::stringstream group_name;
    group_name << "group = 'Light " << i << " properties' ";

    TwAddVarRW(
            menu_bar,
            "Intensity", 
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].intensity,
            group_name.str().c_str());
    TwAddVarRW(
            menu_bar,
            "Color", 
            TW_TYPE_COLOR3F,
            &scene_->light_sources_[i].color,
            group_name.str().c_str());
    TwAddVarRW(
            menu_bar,
            "Position", 
            TW_TYPE_DIR3F,
            &scene_->light_sources_[i].position,
            group_name.str().c_str());
/*    TwAddVarRW(
            menu_bar,
            "Directional", 
            TW_TYPE_BOOL32,
            &scene_->light_sources_[i].position.w,
            group_name.str().c_str());*/

    TwAddVarRW(
            menu_bar,
            "Constant attenuation",
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].constant_attenuation,
            group_name.str().c_str());
    TwAddVarRW(
            menu_bar,
            "Linear attenuation",
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].linear_attenuation,
            group_name.str().c_str());
    TwAddVarRW(
            menu_bar,
            "Quadratic attenuation",
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].quadratic_attenuation,
            group_name.str().c_str());
    TwAddVarRW(
            menu_bar,
            "Spot cutoff",
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].spot_cutoff,
            group_name.str().c_str());
    TwAddVarRW(
            menu_bar,
            "Spot exponent",
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].spot_exponent,
            group_name.str().c_str());
    TwAddVarRW(
            menu_bar,
            "Spot direction",
            TW_TYPE_DIR3F,
            &scene_->light_sources_[i].spot_direction.x,
            group_name.str().c_str());

    TwDefine(" Menu/Intensity min=0.0 max=100.0 step=1.0 ");
    TwDefine(" Menu/'Constant attenuation' min=0.0 max=1.0 step=0.1 ");
    TwDefine(" Menu/'Linear attenuation' min=0.0 max=1.0 step=0.1 ");
    TwDefine(" Menu/'Quadratic attenuation' min=0.0 max=1.0 step=0.1 ");
    TwDefine(" Menu/'Spot cutoff' min=0.0 max=100.0 step=1.0 ");
    TwDefine(" Menu/'Spot exponent' min=0.0 max=100.0 step=1.0 ");

  }
  TwDefine(" Menu size='300 800' "); // resize bar
  TwDefine(" Menu movable=false "); // resize bar
  TwDefine(" Menu resizable=false "); // resize bar
  TwDefine(" Menu color='64 64 64' alpha=255 text=light "); // light-green bar with dark text color
  TwDefine(" Menu alwaysbottom=true "); // resize bar

  TwDefine(" GLOBAL fontsize=3 ");
  TwDefine(" GLOBAL fontresizable=false "); // font cannot be resized

  return 0;
}

void MyGlWindow::RenderScene(void (Scene::*RenderFunction)(int, int), Scene* s)
{
  int width, height;
  glfwGetWindowSize(window_, &width, &height);
  (s->*RenderFunction)(width, height);
}

void MyGlWindow::MainLoop()
{
  float current_time = glfwGetTime();
  int FPS = 0;
  while (!glfwWindowShouldClose(window_))
  {
    UpdateMousePos();
    scene_->Update();



    RenderScene(RenderFunction, scene_);


    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    TwWindowSize(width*2, height*2);

    TwDraw();  // draw the tweak bar(s)
    // Print FPS
    ++FPS;
    if((glfwGetTime() - current_time) > 1){
      std::string title = "Shader Tests, ";
      std::ostringstream ss;
      ss << FPS;
      std::string s(ss.str());
      title.append(s);
      title.append(" FPS");
      glfwSetWindowTitle (window_,  title.c_str());
      FPS = 0;
      current_time = glfwGetTime();
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

void MyGlWindow::UpdateMousePos()
{
  double cursor_pos_x, cursor_pos_y;
  glfwGetCursorPos(window_, &cursor_pos_x, &cursor_pos_y);
  if (mouse_pressed_)
  {
    double diff_x = prev_cursor_pos_x_ - cursor_pos_x;
    double diff_y = prev_cursor_pos_y_ - cursor_pos_y;
    scene_->GetCamera()->IncrementXrotation(-diff_y*0.01);
    scene_->GetCamera()->IncrementYrotation(-diff_x*0.01);
  }
  prev_cursor_pos_x_ = cursor_pos_x;
  prev_cursor_pos_y_ = cursor_pos_y;
}

void MyGlWindow::ErrorCallback(int error, const char* description)
{
  fputs(description, stderr);
}

void MyGlWindow::KeyCallback(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods)
{
  switch (key)
  {
    case GLFW_KEY_ESCAPE :
    {
      glfwSetWindowShouldClose(window, true);
    }
  }
 
  // Convert from glfw3 to glfw2 which Anttweakbar understands
  switch(key) {
    case GLFW_KEY_UP: key = 256 + 27; break;
    case GLFW_KEY_DOWN: key = 256 + 28; break;
    case GLFW_KEY_LEFT: key = 256 + 29; break;
    case GLFW_KEY_RIGHT: key = 256 + 30; break;
    case GLFW_KEY_LEFT_SHIFT : key = 256 + 31; break;
    case GLFW_KEY_RIGHT_SHIFT : key = 256 + 32; break;
    case GLFW_KEY_LEFT_CONTROL : key = 256 + 33; break;
    case GLFW_KEY_RIGHT_CONTROL : key = 256 + 34; break;
    case GLFW_KEY_LEFT_ALT : key = 256 + 35; break;
    case GLFW_KEY_RIGHT_ALT : key = 256 + 36; break;
    case GLFW_KEY_TAB : key = 256 + 37; break;
    case GLFW_KEY_ENTER : key = 256 + 38; break;
    case GLFW_KEY_BACKSPACE : key = 256 + 39; break;
    case GLFW_KEY_INSERT : key = 256 + 40; break;
    case GLFW_KEY_DELETE : key = 256 + 41; break;
    case GLFW_KEY_PAGE_UP : key = 256 + 42; break;
    case GLFW_KEY_PAGE_DOWN : key = 256 + 43; break;
    case GLFW_KEY_HOME : key = 256 + 44; break;
    case GLFW_KEY_END : key = 256 + 45; break;
    default: break;
  }
  TwEventKeyGLFW(key, action);
}

void MyGlWindow::ScrollCallback(GLFWwindow* window, double x_pos, double y_pos)
{
  scene_->GetCamera()->IncrementZposition(y_pos);
  TwEventMouseWheelGLFW(y_pos);
}

void MyGlWindow::MouseButtonCallback(
        GLFWwindow* window,
        int button,
        int action,
        int mods)
{
  if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
    mouse_pressed_ = true;
  else if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE)
    mouse_pressed_ = false;
  TwEventMouseButtonGLFW(button, action);
}

void MyGlWindow::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
  scene_->GetCamera()->SetAspectRatio(width / static_cast<float>(height));
}

void MyGlWindow::MousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
  TwMouseMotion(int(xpos*2), int(ypos*2));
}

void MyGlWindow::CharCallback(GLFWwindow* window, int codepoint)
{
  TwEventCharGLFW(codepoint, GLFW_PRESS);
}

void TW_CALL MyGlWindow::PreLoadButtonCallback(void* client_data)
{
  load_file_bar_ = TwNewBar("Load obj file");  
  TwAddVarRW(load_file_bar_, "File path", TW_TYPE_CSSTRING(SettingsManager::Instance()->FILENAME_SIZE), SettingsManager::Instance()->file_to_load, "");
  TwAddButton(load_file_bar_, "Load", LoadButtonCallback, scene_, "");
  TwAddButton(load_file_bar_, "Cancel", CancelButtonCallback, load_file_bar_, "");

  TwDefine(" 'Load obj file' size='300 800' "); // resize bar
  TwDefine(" 'Load obj file' position='16 16' "); // resize bar
  TwDefine(" 'Load obj file' movable=false "); // resize bar
  TwDefine(" 'Load obj file' resizable=false "); // resize bar
  TwDefine(" 'Load obj file' alwaystop=true "); // resize bar
  TwDefine(" 'Load obj file' color='100 100 100' alpha=255 text=light "); // light-green bar with dark text color
}

void TW_CALL MyGlWindow::LoadButtonCallback(void* client_data)
{
  MyMesh* new_mesh = new MyMesh(SettingsManager::Instance()->file_to_load);
  if (!new_mesh->IsCorrupt())
  {
    TwBar *mesh_bar;
    std::stringstream bar_name;
    bar_name << "Mesh " << scene_->GetNumberOfMeshes();
    mesh_bar = TwNewBar(bar_name.str().c_str());
    TwAddVarRW(
            mesh_bar,
            "Position", 
            TW_TYPE_DIR3F,
            &new_mesh->position_,
            "");
    TwAddVarRW(
            mesh_bar,
            "Rotation", 
            TW_TYPE_QUAT4F,
            &new_mesh->quaternion_,
            "");

    scene_->AddMesh(new_mesh);

    TwDeleteBar(load_file_bar_);
  }
  else
    delete new_mesh;
}

void TW_CALL MyGlWindow::CancelButtonCallback(void* client_data)
{
  TwDeleteBar(static_cast<TwBar*>(client_data));
}




