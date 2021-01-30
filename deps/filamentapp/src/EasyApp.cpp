//
// Created by lasagnaphil on 1/30/21.
//

#include "filamentapp/EasyApp.h"
#include "filamentapp/FilamentApp.h"

EasyApp::EasyApp(const Config& config, int windowWidth, int windowHeight)
: config(config), windowWidth(windowWidth), windowHeight(windowHeight) {

}

void EasyApp::run() {
    auto setup = [this](Engine* engine, View* view, Scene* scene) { this->setup(engine, view, scene); };
    auto cleanup = [this](Engine* engine, View* view, Scene* scene) { this->cleanup(engine, view, scene); };
    auto update = [this](Engine* engine, View* view, double currentTime) { this->update(engine, view, currentTime); };
    auto imgui = [this](Engine* engine, View* view) { this->imgui(engine, view); };
    auto preRender = [this](Engine* engine, View* view, Scene* scene, Renderer* renderer) {
        this->preRender(engine, view, scene, renderer);
    };
    auto postRender = [this](Engine* engine, View* view, Scene* scene, Renderer* renderer) {
        this->postRender(engine, view, scene, renderer);
    };
    auto resize = [this](Engine* engine, View* view) {
        this->resize(engine, view);
    };
    auto drop = [this](std::string filename) {
        this->drop(filename);
    };
    FilamentApp::get().animate(update);
    FilamentApp::get().resize(resize);
    FilamentApp::get().setDropHandler(drop);
    FilamentApp::get().run(config, setup, cleanup, imgui, preRender, postRender, 1920, 1080);
}
