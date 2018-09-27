#include <azordx/resources.h>
#include <d3dx9.h>
#include <irrklang/irrKlang.h>
#include <map>

static ID3DXFont* font = nullptr;

namespace azordx {
extern LPDIRECT3DDEVICE9 renderer_device;
extern irrklang::ISoundEngine* sound_engine;
extern HWND window_handle;
std::map<std::string, LPDIRECT3DTEXTURE9> texture_storage;
std::map<std::string, D3DXVECTOR2> texture_dimensions;
std::map<std::string, irrklang::ISoundSource*> sound_source_storage;
std::map<std::string, ID3DXFont*> font_storage;

void resources_clean_up() {
  for (const auto& texture : texture_storage) {
    if (texture.second) {
      texture.second->Release();
    }
  }

  for (const auto& font : font_storage) {
    if (font.second) {
      font.second->Release();
    }
  }
}
}  // namespace azordx

void azordx::Resources::load_texture_from_file(const std::string& file_path,
                                               const std::string& name) {
  if (renderer_device) {
    LPDIRECT3DTEXTURE9 texture = nullptr;

    const auto path = "./resources/textures/" + file_path;
    const auto result =
        D3DXCreateTextureFromFile(renderer_device, path.c_str(), &texture);

    D3DSURFACE_DESC decs;
    texture->GetLevelDesc(0, &decs);

    printf("\nTexture name: %s\n", name.c_str());
    printf("Size: %dx%d\n", decs.Width, decs.Height);

    texture_storage.insert(
        std::pair<std::string, LPDIRECT3DTEXTURE9>(name, texture));

    const auto& dimensions = D3DXVECTOR2(decs.Width, decs.Height);
    texture_dimensions.insert(
        std::pair<std::string, D3DXVECTOR2>(name, dimensions));
  }
}

void azordx::Resources::load_sound_from_file(const std::string& file_path,
                                             const std::string& name,
                                             float default_volume) {
  if (sound_engine) {
    const auto path = "./resources/sounds/" + file_path;

    const auto sound_source =
        sound_engine->addSoundSourceFromFile(path.c_str());
    sound_source->setDefaultVolume(default_volume);
    sound_source_storage.insert(
        std::pair<std::string, irrklang::ISoundSource*>(name, sound_source));
  }
}

void azordx::Resources::load_font(const std::string& font_name,
                                  const std::string& alias, const int font_size,
                                  const int font_weight, const bool italic) {
  const auto hdc = GetDC(window_handle);

  D3DXFONT_DESC lf;
  lf.Height = -MulDiv(font_size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  lf.Width = 0;
  lf.Weight = font_weight;
  lf.Italic = italic;
  lf.CharSet = ANSI_CHARSET;
  lf.OutputPrecision = OUT_TT_ONLY_PRECIS | OUT_TT_PRECIS;
  lf.Quality = ANTIALIASED_QUALITY;
  lf.PitchAndFamily = VARIABLE_PITCH | FF_DONTCARE;
  strcpy(lf.FaceName, font_name.c_str());
  D3DXCreateFontIndirect(renderer_device, &lf, &font);
  font_storage.insert(std::pair<std::string, ID3DXFont*>(alias, font));
  font->PreloadGlyphs(0, 127);
}

D3DXVECTOR2 azordx::Resources::get_texture_size(const std::string& name) {
  return texture_dimensions[name];
}