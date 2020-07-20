#include <Corrade/Containers/ArrayView.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Math/Color.h>
#include <Corrade/Containers/ArrayViewStl.h>
#include "CubicBezierShader.h"
#include <memory>

namespace Magnum
{
    using namespace Math::Literals;
    namespace Examples
    {
        typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
        typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

        class StringArtBezier : public Platform::GlfwApplication
        {
        public:
            explicit StringArtBezier(const Arguments &arguments);

        private:
            void drawEvent() override;

            // Camera attributes
            std::shared_ptr<Object3D> m_cameraObject;
            std::shared_ptr<SceneGraph::Camera3D> m_camera;

            Scene3D m_scene;
            std::shared_ptr<SceneGraph::DrawableGroup3D> m_drawableGroup;
            std::unique_ptr<CubicBezierShader> m_shader;
            GL::Buffer m_verticeBuffer{NoCreate}, buf2;
            GL::Mesh mesh{NoCreate}, mesh2;
            std::vector<Vector2> vertices;
            Shaders::VertexColor3D vs{NoCreate};
        };

        StringArtBezier::StringArtBezier(const Arguments &arguments) : Platform::Application{arguments,
                                                                                             Configuration{}
                                                                                                 .setTitle("String Art - Cubic Bezier")
                                                                                                 .setWindowFlags(Configuration::WindowFlag::Resizable)
                                                                                                 .setSize({800, 600})}
        {
            m_cameraObject = std::make_shared<Object3D>();
            m_cameraObject->translate(Vector3::zAxis(1.5f));
            m_cameraObject->setParent(&m_scene);
            m_camera = std::make_shared<SceneGraph::Camera3D>(*m_cameraObject);
            m_camera->setProjectionMatrix(Matrix4::orthographicProjection({2.8, 2.1}, 0.1, 100))
                .setViewport(GL::defaultFramebuffer.viewport().size());

            m_shader = std::make_unique<CubicBezierShader>();

            // initial vertices
            vertices = {{-1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, 1.0f}};
            m_verticeBuffer = GL::Buffer(GL::Buffer::TargetHint::Array);
            m_verticeBuffer.setData(vertices);
            mesh = GL::Mesh(GL::MeshPrimitive::Patches);
            mesh.setCount(vertices.size())
                .addVertexBuffer(std::move(m_verticeBuffer), 0, CubicBezierShader::VertexPosition{});

            buf2 = GL::Buffer{};
            buf2.setData({-1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f});
            mesh2 = GL::Mesh(GL::MeshPrimitive::Points);
            mesh2.setCount(3)
                .addVertexBuffer(std::move(buf2), 0, Shaders::VertexColor3D::Position{});
            vs = Shaders::VertexColor3D{};
            vs.setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());
            GL::Renderer::setClearColor(Color4{0.0f, 0.1f, 0.4f, 1.0f});
            GL::Renderer::setPointSize(10);
            GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
            GL::Renderer::setPatchVertexCount(vertices.size());
        }
        void StringArtBezier::drawEvent()
        {

            GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);
            m_shader->setNumberOfSegments(50)
                .setNumberOfStrips(3)
                .setColor(Color3{1.0f, 0.0f, 0.0f})
                .setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());

            m_shader->draw(mesh);
            vs.draw(mesh2);
            swapBuffers();
            redraw();
        }
    } // namespace Examples
} // namespace Magnum
MAGNUM_APPLICATION_MAIN(Magnum::Examples::StringArtBezier)