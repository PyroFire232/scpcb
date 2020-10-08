#include "ModelImageGenerator.h"

#include "../Models/Model.h"
#include "GraphicsResources.h"
#include "Camera.h"

ModelImageGenerator::ModelImageGenerator(PGE::Graphics* graphics, GraphicsResources* gfxRes) {
    this->graphics = graphics;
    this->gfxRes = gfxRes;

    initialized = false;
}

ModelImageGenerator::~ModelImageGenerator() {
    if (initialized) {
        deinitialize();
    }
}

void ModelImageGenerator::initialize(int texSize) {
    this->texSize = texSize;

    cam = new Camera(gfxRes, texSize, texSize, 0.f, INFINITY, -INFINITY, true);
    cam->updateDrawTransform(0.f);

    oldViewport = graphics->getViewport();
    graphics->setViewport(PGE::Rectanglei(0, 0, texSize, texSize));

    initialized = true;
}

void ModelImageGenerator::deinitialize() {
    delete cam;
    graphics->resetRenderTarget();
    graphics->setViewport(oldViewport);

    initialized = false;
}

bool ModelImageGenerator::getInitialized() const {
    return initialized;
}

PGE::Texture* ModelImageGenerator::generate(const PGE::String& model, float scale, const PGE::Vector3f& rotation, PGE::Vector2f position) {
    if (!initialized) {
        throw std::runtime_error("ModelImageGenerator has not been initialized!");
    }

    PGE::Texture* tex = PGE::Texture::create(graphics, texSize, texSize, true, nullptr, PGE::Texture::FORMAT::RGBA32);

    graphics->setRenderTarget(tex);
    graphics->clear(PGE::Color::Black);

    ModelInstance* mi = gfxRes->getModelInstance(model);

    // Further optimized if in initialization step, requires Model shader to be loaded at that point.
    gfxRes->setCameraUniforms(cam);

    position = position * texSize;
    mi->setPosition(PGE::Vector3f(position.x, position.y, 0.f));
    mi->setRotation(rotation);
    mi->setScale(PGE::Vector3f(scale * texSize));
    mi->render();
    gfxRes->dropModelInstance(mi);

    return tex;
}
