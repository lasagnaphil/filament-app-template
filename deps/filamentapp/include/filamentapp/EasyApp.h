//
// Created by lasagnaphil on 1/30/21.
//

#ifndef FILAMENT_APP_TEMPLATE_EASYAPP_H
#define FILAMENT_APP_TEMPLATE_EASYAPP_H

#include "filamentapp/Config.h"

#define EASYAPP_CONSTRUCTOR(CLASS) \
CLASS(const Config& config, int windowWidth, int windowHeight) : EasyApp(config, windowWidth, windowHeight) {}

namespace filament {
class Renderer;
class Scene;
class View;
} // namespace filament

class Config;

class EasyApp {
public:
    EasyApp(const Config& config, int windowWidth, int windowHeight);

    void run();

    Config config;
    int windowWidth, windowHeight;

protected:
    virtual void setup(filament::Engine* engine, filament::View* view, filament::Scene* scene) = 0;
    virtual void cleanup(filament::Engine* engine, filament::View* view, filament::Scene* scene) = 0;
    virtual void update(filament::Engine* engine, filament::View* view, double currentTime) = 0;
    virtual void imgui(filament::Engine* engine, filament::View* view) = 0;
    virtual void preRender(filament::Engine* engine, filament::View* view,
                           filament::Scene* scene, filament::Renderer* renderer) = 0;
    virtual void postRender(filament::Engine* engine, filament::View* view,
                            filament::Scene* scene, filament::Renderer* renderer) = 0;
    virtual void resize(filament::Engine* engine, filament::View* view) = 0;
    virtual void drop(std::string filename) = 0;
};

#endif //FILAMENT_APP_TEMPLATE_EASYAPP_H
