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

        class TesselationShaderExample : public Platform::GlfwApplication
        {
        public:
            explicit TesselationShaderExample(const Arguments &arguments);

        private:
            void drawEvent() override;

            // Camera attributes
            std::shared_ptr<Object3D> m_cameraObject;
            std::shared_ptr<SceneGraph::Camera3D> m_camera;

            Scene3D m_scene;
            std::shared_ptr<SceneGraph::DrawableGroup3D> m_drawableGroup;
            std::unique_ptr<CubicBezierShader> m_shader;
            GL::Buffer m_controlPointsBuffer{ NoCreate };
            GL::Mesh m_geometryMesh{ NoCreate }, m_controlPointsMesh;
            std::vector<Vector3> m_controlVertices;
            Shaders::VertexColor3D m_controlPointsShaderProgram{ NoCreate };
        };

        TesselationShaderExample::TesselationShaderExample(const Arguments &arguments) : Platform::Application{ arguments,
            Configuration{}
            .setTitle("TesselationShaderExample")
            .setWindowFlags(Configuration::WindowFlag::Resizable)
            .setSize({ 800, 600 }) }
        {
            m_cameraObject = std::make_shared<Object3D>();
            m_cameraObject->translate(Vector3::zAxis(1.5f));
            m_cameraObject->setParent(&m_scene);
            m_camera = std::make_shared<SceneGraph::Camera3D>(*m_cameraObject);
            m_camera->setProjectionMatrix(Matrix4::orthographicProjection({ 2.8, 2.1 }, 0.1, 100))
                .setViewport(GL::defaultFramebuffer.viewport().size());

            m_shader = std::make_unique<CubicBezierShader>();

            // initial vertices
            m_controlVertices.push_back({ -1.0f, -1.0f, 0.0f });
            m_controlVertices.push_back({ -1.0f, 1.0f, 0.0f });
            m_controlVertices.push_back({ 1.0f, 1.0f, 0.0f });
            m_controlPointsBuffer = GL::Buffer(GL::Buffer::TargetHint::Array);
            m_controlPointsBuffer.setData(m_controlVertices);
            m_geometryMesh = GL::Mesh(GL::MeshPrimitive::Patches);
            m_geometryMesh.setCount(m_controlVertices.size())
                .addVertexBuffer(m_controlPointsBuffer, 0, CubicBezierShader::VertexPosition{});

            m_controlPointsMesh = GL::Mesh(GL::MeshPrimitive::Points);
            m_controlPointsMesh.setCount(m_controlVertices.size())
                .addVertexBuffer(m_controlPointsBuffer, 0, Shaders::VertexColor3D::Position{});
            m_controlPointsShaderProgram = Shaders::VertexColor3D{};
            m_controlPointsShaderProgram.setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());
            GL::Renderer::setClearColor(Color4{ 0.0f, 0.1f, 0.4f, 1.0f });
            GL::Renderer::setPointSize(10);
            GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
            GL::Renderer::setPatchVertexCount(m_controlPointsBuffer.size());
        }
        void TesselationShaderExample::drawEvent()
        {

            GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);
            m_shader->setNumberOfSegments(50)
                .setNumberOfStrips(3)
                .setColor(Color3{ 1.0f, 0.0f, 0.0f })
                .setTransformationProjectionMatrix(m_camera->projectionMatrix() * m_camera->cameraMatrix());

            m_shader->draw(m_geometryMesh);
            m_controlPointsShaderProgram.draw(m_controlPointsMesh);
            swapBuffers();
            redraw();
        }
    } // namespace Examples
} // namespace Magnum
MAGNUM_APPLICATION_MAIN(Magnum::Examples::TesselationShaderExample)