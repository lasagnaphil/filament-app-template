
/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <filament/Engine.h>
#include <filament/LightManager.h>
#include <filament/Material.h>
#include <filament/RenderableManager.h>
#include <filament/Scene.h>
#include <filament/TransformManager.h>
#include <filament/View.h>

#include <utils/EntityManager.h>

#include <filameshio/MeshReader.h>

#include <filamentapp/Config.h>
#include <filamentapp/FilamentApp.h>
#include <filamentapp/EasyApp.h>

#include <imgui.h>
#include <implot.h>

#include "resources.h"

using namespace filament;
using namespace filamesh;
using namespace filament::math;

using Backend = Engine::Backend;

class MyApp : public EasyApp {
public:
    EASYAPP_CONSTRUCTOR(MyApp)

protected:
    void setup(Engine* engine, View* view, Scene* scene) override {
        auto& tcm = engine->getTransformManager();
        auto& rcm = engine->getRenderableManager();
        auto& em = utils::EntityManager::get();

        // Instantiate material.
        material = Material::Builder()
                .package(RESOURCES_AIDEFAULTMAT_DATA, RESOURCES_AIDEFAULTMAT_SIZE).build(*engine);
        auto mi = materialInstance = material->createInstance();
        mi->setParameter("baseColor", RgbType::LINEAR, float3{0.8});
        mi->setParameter("metallic", 1.0f);
        mi->setParameter("roughness", 0.4f);
        mi->setParameter("reflectance", 0.5f);

        // Add geometry into the scene.
        mesh = MeshReader::loadMeshFromBuffer(engine, RESOURCES_SUZANNE_DATA, nullptr, nullptr, mi);
        auto ti = tcm.getInstance(mesh.renderable);
        transform = mat4f{ mat3f(1), float3(0, 0, -4) } * tcm.getWorldTransform(ti);
        rcm.setCastShadows(rcm.getInstance(mesh.renderable), false);
        scene->addEntity(mesh.renderable);

        // Add light sources into the scene.
        light = em.create();
        LightManager::Builder(LightManager::Type::SUN)
                .color(Color::toLinear<ACCURATE>(sRGBColor(0.98f, 0.92f, 0.89f)))
                .intensity(110000)
                .direction({ 0.7, -1, -0.8 })
                .sunAngularRadius(1.9f)
                .castShadows(false)
                .build(*engine, light);
        scene->addEntity(light);
    }

    void cleanup(Engine* engine, View* view, Scene* scene) override {
        engine->destroy(light);
        engine->destroy(materialInstance);
        engine->destroy(mesh.renderable);
        engine->destroy(material);
    }

    void update(Engine* engine, View* view, double currentTime) override {
        auto& tcm = engine->getTransformManager();
        auto ti = tcm.getInstance(mesh.renderable);
        tcm.setTransform(ti, transform * mat4f::rotation(currentTime, float3{ 0, 1, 0 }));
    }

    void imgui(Engine* engine, View* view) override {
        ImGui::ShowDemoWindow();
        ImPlot::ShowDemoWindow();
    }

    void preRender(Engine* engine, View* view, Scene* scene,
                   Renderer* renderer) override {

    }

    void postRender(Engine* engine, View* view, Scene* scene,
                    Renderer* renderer) override {

    }

    void resize(Engine* engine, View* view) override {

    }

    void drop(std::string filename) override {

    }

private:
    utils::Entity light;
    Material* material;
    MaterialInstance* materialInstance;
    MeshReader::Mesh mesh;
    mat4f transform;
};

static const char* IBL_FOLDER = "generated/envmap/default_env";

int main(int argc, char** argv) {
    Config config;
    config.title = "hellopbr";
    config.iblDirectory = FilamentApp::getRootAssetsPath() + IBL_FOLDER;

    MyApp app(config, 1920, 1080);
    app.run();

    return 0;
}