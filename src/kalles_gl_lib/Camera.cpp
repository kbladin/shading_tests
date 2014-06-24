#include <iostream>

#include "../../include/kalles_gl_lib/Camera.h"

//! Constructor, creates a default camera
/*!
  The default camera is located at (-10,3,0) and looks at (0,1,0).
*/
Camera::Camera(glm::vec3 target, float far_clipping, float near_clipping, float aspect_ratio) {
  target_ = target;// glm::vec3(0.0,0.0,0.0);
  far_clipping_ = far_clipping;// 200.0f;
  near_clipping_ = near_clipping;// 0.1f;
  aspect_ratio_ = aspect_ratio;

  // Values used to delay the camera
  local_translate_goal_ = glm::vec3(0.0f, 0.0f, -10.0f);

  rotate_x_goal_ = 0.0f;
  rotate_y_goal_ = 0.0f;
  //float translate_z_goal_ = -10.0f;

  local_translate_ = glm::vec3(0.0f, 0.0f, -100.0f);
  global_translate_ = glm::vec3(0.0f, 0.0f, 0.0f);;
  rotate_x_ = M_PI / 2.0f;
  rotate_y_ = 0.0f;

  projection_ = glm::perspective(
      45.0f,
      aspect_ratio_,
      near_clipping_,
      far_clipping_);
  // create default camera
  view_ = glm::lookAt(glm::vec3(-10 , 3, 0),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0));
}
//! Constructor that creates a camera from input matrices
/*!
  Sets the cameras view matrix and projection matrix as specified
  \param view matrix
  \param projection matrix
*/
Camera::Camera(glm::mat4 view, glm::mat4 projection) {
  view_ = view;
  projection_ = projection;
  aspect_ratio_ = 1.0f;
}

glm::mat4 Camera::GetViewMatrix(){
  return view_;
}

glm::mat4 Camera::GetProjectionMatrix(){
  return projection_;
}

float Camera::GetFarClipping() {
  return far_clipping_;
}

glm::vec3 Camera::GetTarget(){
  return -target_;
}

void Camera::SetTarget(glm::vec3 target){
  target_ = -target;
}

void Camera::SetAspectRatio(float aspect_ratio)
{
  aspect_ratio_ = aspect_ratio;
}
//! Updates the internal matrices of the camera
/*!
  First the signals from input are delayed. This is for the camera to get a
  more smoother movement. Then the results are used to create the matrices that
  build up the view matrix. The projection matrix is default but uses the
  aspect ratio of the frame.
*/
void Camera::UpdateMatrices(){
  // Delay the signals
  float camera_speed = 0.1f;
  Delay(&local_translate_, local_translate_goal_, camera_speed);
  Delay(&rotate_x_, rotate_x_goal_, camera_speed);
  Delay(&rotate_y_, rotate_y_goal_, camera_speed);
  global_translate_goal_ = target_;
  Delay(&global_translate_, global_translate_goal_, camera_speed);

  // Create the matrices
  glm::mat4 local_translate_mat = glm::translate(local_translate_);
  glm::mat4 rotate_y_mat = glm::rotate(
      glm::mat4(1.0f),
      rotate_y_,
      glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 rotate_x_mat = glm::rotate(
      glm::mat4(1.0f),
      rotate_x_,
      glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 global_translate_mat = glm::translate(global_translate_);
  // Update the view matrix
  view_ =
      local_translate_mat *
      rotate_x_mat *
      rotate_y_mat *
      global_translate_mat;

  // Update the projection matrix
  projection_ = glm::perspective(45.0f, aspect_ratio_, near_clipping_, far_clipping_);
}
//! Increment the x-rotation angle of the camera
void Camera::IncrementXrotation(float h) {
  rotate_x_goal_ += h;
  rotate_x_goal_ = glm::clamp(
      rotate_x_goal_,
      0.0f, static_cast<float>(M_PI) / 2.0f);
}
//! Increment the y-rotation angle of the camera
void Camera::IncrementYrotation(float h) {
  rotate_y_goal_ += h;
}
//! Increment the z-position of the camera
void Camera::IncrementZposition(float h) {
  local_translate_goal_ += glm::vec3(0.0f, 0.0f, h);
  local_translate_goal_.z = glm::clamp(
      local_translate_goal_.z,
      -far_clipping_,
      -near_clipping_);
}

//! Helper function for the camera
/*!
  This function takes class like float or glm::vec3. What it does is to delay
  a signal as if it was to follow y=1 as y=(1-e^(a*t)). The rise time depend
  on the speed parameter.
  \param input is the signal that will be changed to output
  (passed by reference)
  \param end_val is the value that the function should tend toward.
  \param speed determines the rise time. The higher, the faster.
  Speed should be between 0 and 1 and is otherwise clamped.
*/
template <class T>
void Camera::Delay(T* input, T end_val, float speed) {
  if (speed < 0.0f && speed > 1.0f) {
  speed = glm::clamp(speed, 0.0f, 1.0f);
  std::cout << "WARNING: clamping speed between 0.0 and 1.0" << std::endl;
  }
  *input = (end_val - *input) * speed + *input;
}
