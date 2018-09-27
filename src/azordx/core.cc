#include <azordx/collision .h>
#include <azordx/input.h>
#include <azordx/timer.h>
// #include <azordx/debug.h>
#include <d3dx9.h>
#include <irrklang/irrKlang.h>
#include <jansson/jansson.h>
#include <windows.h>
#include <cstdio>
#include <string>

#define CLASS_NAME "AzorDx"
#define IDT_TIMER_1 1001

extern void initialize();
extern void load_resources();
extern void clean_up();
extern void on_left_mouse_btn_click();
extern void on_left_mouse_btn_dbclick();
extern void on_key_down(int key);

static UINT frame_count = 0;
static POINT mouse_pos;
static bool debug_mode = true;
// static azordx::Debug* debug_draw;
static azordx::CollisionListener collision_listener{};
static const float time_step = 1 / 60.0f;

namespace azordx {

HINSTANCE app_handle = nullptr;
HWND window_handle = nullptr;
UINT init_width = 0;
UINT init_height = 0;
const char* init_title = nullptr;
LPDIRECT3DDEVICE9 renderer_device;
LPD3DXSPRITE sprite_batch;
irrklang::ISoundEngine* sound_engine = nullptr;
b2World* physics_engine = nullptr;

extern void tick();
extern void poll_mouse();
extern void poll_keyboard();
extern void input_initialize();
extern void input_clean_up();
extern void resources_clean_up();
extern void sound_clean_up();
extern void initialize_timer();
extern void update_scene();
extern void draw_scene();
extern void scene_clean_up();

LRESULT CALLBACK wind_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  static int width = 0;
  static int height = 0;

  switch (msg) {
    case WM_KEYDOWN:
      if (wparam == VK_ESCAPE)
        DestroyWindow(azordx::window_handle);
      else if (wparam == 187)
        debug_mode = !debug_mode;
      else
        on_key_down(wparam);
      return 0;

    case WM_DESTROY:
      KillTimer(azordx::window_handle, IDT_TIMER_1);
      PostQuitMessage(0);
      return 0;

    case WM_TIMER:
      char temp[50];
      sprintf(temp, "%s FPS: %d", azordx::init_title, frame_count);
      SetWindowText(azordx::window_handle, temp);
      frame_count = 0;
      return 0;

    case WM_SIZE:
      width = LOWORD(lparam);
      height = HIWORD(lparam);
      printf("\nClient has been resized to %dx%d\n", width, height);
      return 0;

    case WM_MOVE:
      printf("\nClient has been moved to %dx%d\n", LOWORD(lparam),
             HIWORD(lparam));
      return 0;

    case WM_LBUTTONDOWN:
      on_left_mouse_btn_click();
      return 0;

    case WM_LBUTTONDBLCLK:
      on_left_mouse_btn_dbclick();
      return 0;

    case WM_MOUSEMOVE:
      GetCursorPos(&mouse_pos);
      ScreenToClient(azordx::window_handle, &mouse_pos);
      Input::mouse_position = D3DXVECTOR2(mouse_pos.x, mouse_pos.y);
      return 0;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

HRESULT setup() {
  json_error_t json_err;
  const auto config_data = json_load_file("./.config", 0, &json_err);
  if (!config_data) {
    printf("\nerror message: %s\n", json_err.text);
    printf("error source: %s\n", json_err.source);
    printf("line: %d\n", json_err.line);
    printf("column: %d\n", json_err.column);
    printf("position: %d\n", json_err.position);
    return E_FAIL;
  }

  const auto game_window_data = json_object_get(config_data, "gameWindow");
  init_width =
      json_integer_value(json_object_get(game_window_data, "windowWidth"));
  init_height =
      json_integer_value(json_object_get(game_window_data, "windowHeight"));
  init_title =
      json_string_value(json_object_get(game_window_data, "windowTitle"));
  const auto centered_window =
      json_object_get(game_window_data, "centeredWindow");

  const auto physics_data = json_object_get(config_data, "physics");
  const float gravity =
      json_real_value(json_object_get(physics_data, "gravity"));

  WNDCLASS wc;
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wc.lpfnWndProc = wind_proc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = app_handle;
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
  wc.lpszMenuName = nullptr;
  wc.lpszClassName = CLASS_NAME;

  if (!RegisterClass(&wc)) {
    /* code */
    MessageBox(nullptr, "RegisterClass - Failed", nullptr, 0);
    return false;
  }

  // Center window
  unsigned int xPos = 0;
  unsigned int yPos = 0;
  if (centered_window->type == JSON_TRUE) {
    const auto screen_width = GetSystemMetrics(SM_CXSCREEN);
    const auto screen_height = GetSystemMetrics(SM_CYSCREEN);
    xPos = (screen_width - init_width) / 2;
    yPos = (screen_height - init_height) / 2;
  }

  window_handle = CreateWindow(CLASS_NAME, CLASS_NAME, WS_OVERLAPPEDWINDOW,
                               xPos, yPos, init_width, init_height, nullptr,
                               nullptr, app_handle, nullptr);

  if (!window_handle) {
    /* code */
    MessageBox(nullptr, "CreateWindow - Failed", nullptr, 0);
    return false;
  }

  SetDoubleClickTime(400);
  SetWindowText(window_handle, init_title);
  SetTimer(window_handle, IDT_TIMER_1, 1000, (TIMERPROC) nullptr);

  //////////////////////
  // Initialize D3D:
  /////////////////////

  // Step 1: Create the IDirect3D9 object.
  IDirect3D9* d3d9 = nullptr;
  d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

  if (!d3d9) {
    MessageBox(nullptr, "Direct3DCreate9()", nullptr, 0);
    return E_FAIL;
  }

  // print adapter info
  D3DADAPTER_IDENTIFIER9 adapter_info;
  d3d9->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapter_info);
  std::string manufacturer;

  for (size_t i = 0; i < 512; i++) {
    if (adapter_info.Description[i] != ' ') {
      manufacturer += adapter_info.Description[i];
      continue;
    }
    break;
  }

  printf("Your GPU info:\n");
  printf("\tModel: %s\n", adapter_info.Description);
  printf("\tManufacturer: %s\n\n", manufacturer.c_str());

  // Step 2: Check for hardware vertex processing.
  D3DCAPS9 deviceCapabilities;
  d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &deviceCapabilities);

  UINT vertexProcessing = 0;

  if (deviceCapabilities.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
    vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
  } else {
    vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
  }

  // Step 3: Fill out the D3DPRESENT_PARAMETERS structure.
  D3DPRESENT_PARAMETERS d3dpp;
  d3dpp.BackBufferWidth = init_width;
  d3dpp.BackBufferHeight = init_height;
  d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
  d3dpp.BackBufferCount = 1;
  d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
  d3dpp.MultiSampleQuality = 0;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.hDeviceWindow = window_handle;
  d3dpp.Windowed = true;
  d3dpp.EnableAutoDepthStencil = true;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
  d3dpp.Flags = 0;
  d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
  d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

  // Step 4: Create the device.

  if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                window_handle, vertexProcessing, &d3dpp,
                                &renderer_device))) {
    // try again using a 16-bit depth buffer
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                  window_handle, vertexProcessing, &d3dpp,
                                  &renderer_device))) {
      d3d9->Release();  // done with d3d9 object
      ::MessageBox(0, "CreateDevice() - FAILED", "Error", MB_OK | MB_ICONERROR);
      return E_FAIL;
    }
  }

  d3d9->Release();  // done with d3d9 object

  // create projection matrix
  D3DXMATRIX projection;
  D3DXMatrixOrthoLH(&projection, init_width, init_height, -1.0f, 1.0f);
  renderer_device->SetTransform(D3DTS_PROJECTION, &projection);

  // create ID3DXSprite
  D3DXCreateSprite(renderer_device, &sprite_batch);

  // initializing for load texture
  renderer_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  renderer_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  renderer_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
  renderer_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  renderer_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

  // initializing for sound
  sound_engine = irrklang::createIrrKlangDevice();

  // initializing for physics
  physics_engine = new b2World(b2Vec2(0, gravity));
  // debug_draw = new Debug();
  // debug_draw->SetFlags(b2Draw::e_shapeBit);
  // physics_engine->SetDebugDraw(debug_draw);
  physics_engine->SetContactListener(&collision_listener);

  // initializing for input
  input_initialize();
  return S_OK;
}

int loop() {
  MSG message;
  ZeroMemory(&message, sizeof(MSG));

  printf("\n");

  load_resources();
  initialize();
  initialize_timer();

  ShowCursor(FALSE);
  ShowWindow(window_handle, SW_SHOW);
  UpdateWindow(window_handle);

  //lock cursor
  // RECT client

  if (!renderer_device || !sprite_batch) return false;

  while (message.message != WM_QUIT) {
    if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    } else {
      tick();

      update_scene();
      physics_engine->Step(time_step, 6, 3);
      renderer_device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                             D3DCOLOR_XRGB(51, 77, 77), 1.0f, 0);
      renderer_device->BeginScene();
      if (FAILED(sprite_batch->Begin(D3DXSPRITE_ALPHABLEND |
                                     D3DXSPRITE_DO_NOT_ADDREF_TEXTURE))) {
        return false;
      }

      draw_scene();
      if (debug_mode) {
        physics_engine->DrawDebugData();
      }

      sprite_batch->End();
      renderer_device->EndScene();
      renderer_device->Present(nullptr, nullptr, nullptr, nullptr);

      frame_count++;
    }
  }

  // delete debug_draw;
  clean_up();
  input_clean_up();
  resources_clean_up();
  sound_clean_up();
  scene_clean_up();

  if (renderer_device) renderer_device->Release();
  if (sprite_batch) sprite_batch->Release();

  return message.wParam;
}

}  // namespace azordx

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevhInstance, PSTR cmd_line,
                   int show_cmd) {
  azordx::app_handle = hInstance;

  printf("Welcome to AzorDx - A DirectX 9.0 Game 2D Framework.\n");
  printf("Press 'ESC' or 'Red Cross' button to exit.\n");

  if (FAILED(azordx::setup())) {
    MessageBox(0, "Initialized - Failed", "Error", MB_OK | MB_ICONERROR);
    return -1;
  }

  return azordx::loop();
}