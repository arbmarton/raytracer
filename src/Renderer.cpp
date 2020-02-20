#include "Renderer.h"
#include "Utilities.h"
#include "Camera.h"
#include "Ray.h"



Renderer::Renderer()
    : quadshader(Shader(utilities::getShaderPath("quad.vs"), utilities::getShaderPath("quad.fs")))
{
    const float fov = glm::radians(90.0f);  // field of view in degrees
    const float halfFov = fov / 2;

    planeDistance = (ScreenDescriptor::WINDOW_WIDTH / 2) / tan(halfFov);

    const float halfUpperFov = glm::atan(((ScreenDescriptor::WINDOW_HEIGHT / 2) / planeDistance));
    const float upperFov = halfUpperFov * 2;
    const float upperFovDegrees = glm::degrees(upperFov);

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    //for (int i = 0; i < 10; ++i)
    //{
    //    spheres.push_back({ utilities::generateRandomVec3(-20, 20), utilities::generateRandomVec3(0, 1), utilities::generateRandomFloat(1.0f, 5.0f) });
    //}

    //for (int i = 0; i < 10; ++i)
    //{
    //    lights.push_back({ utilities::generateRandomVec3(-20, 20), utilities::generateRandomVec3(0, 1), 100 });
    //}

    spheres.push_back({ {0,0,-5}, {1,0,0}, 1.0f });
    spheres.push_back({ {3,0,-5}, {0,0,1}, 1.0f });
    lights.push_back({ {0,0,-10}, {1,1,1}, 100 });
}

glm::vec3 Renderer::trace(const Ray& ray, const int recursionDepth)
{
    if (recursionDepth > maxRecursion)
    {
        return ambientLight;
    }

    std::pair<Sphere*, float> found = intersect(ray);
    if (!found.first)
    {
        return ambientLight;
    }

    const glm::vec3 intersectionPoint = ray.origin + ray.direction * found.second;
    const glm::vec3 sphereNormal = glm::normalize(intersectionPoint - found.first->pos);

    // TODO: emission

    glm::vec3 ambient = found.first->ka * ambientLight;
    glm::vec3 directLight{ 0, 0, 0 };
    glm::vec3 reflectedLight{ 0, 0, 0 };
    glm::vec3 refractedLight{ 0, 0, 0 };

    for (int i = 0; i < lights.size(); ++i)
    {
        const glm::vec3 toLightDir = glm::normalize(lights[i].position - intersectionPoint);
        const Ray toLightRay{ intersectionPoint + toLightDir * 0.01f, toLightDir };

        const float lightDistance = glm::length(lights[i].position - intersectionPoint);

        std::pair<Sphere*, float> blockingSphere = intersect(toLightRay);
        if (!blockingSphere.first || blockingSphere.second > lightDistance)
        {
            const float dotproduct = glm::dot(sphereNormal, toLightDir);

            if (dotproduct <= 0.0f)
            {
                continue;
            }

            const glm::vec3 color = lights[i].color * found.first->color;
            directLight += color * (lights[i].intensity * 10 * dotproduct / (fourPI * lightDistance * lightDistance));
        }
    }

    if (found.first->kr > 0)
    {
        const glm::vec3 reflectionDir = utilities::calculateReflectionDirection(ray, sphereNormal);
        const Ray reflectedRay{ intersectionPoint + reflectionDir * 0.01f, reflectionDir };
        reflectedLight = found.first->kr * trace(reflectedRay, recursionDepth + 1);
    }

    if (found.first->kt > 0)
    {
        const glm::vec3 refractionDir = utilities::calculateRefractionDirection(ray, sphereNormal, 0.5f);
        const Ray refractedRay{ intersectionPoint + refractionDir * 0.01f, refractionDir };
        refractedLight = found.first->kt * trace(refractedRay, recursionDepth + 1);
    }

    return ambient + directLight + reflectedLight + refractedLight;
}

std::pair<Sphere*, float> Renderer::intersect(const Ray& ray)
{
    Sphere* ret = nullptr;
    std::optional<float> val = std::numeric_limits<float>::max();
    for (int i = 0; i < spheres.size(); ++i)
    {
        auto temp = utilities::calculateSphereIntersectionDistance(ray, spheres[i]);
        if (temp && temp < val)
        {
            val = temp;
            ret = &spheres[i];
        }
    }

    return { ret, *val };
}

GLuint Renderer::renderToTexture()
{
    const glm::vec3 camera_plane_center = Camera::intance().getPosition() + Camera::intance().getDirection() * planeDistance;
    const glm::vec3 camera_plane_right = glm::normalize(glm::cross(Camera::intance().getDirection(), Camera::intance().getUp()));
    const glm::vec3 camera_plane_up = glm::normalize(glm::cross(camera_plane_right, Camera::intance().getDirection()));

    const glm::vec3 xOffset = camera_plane_right * (ScreenDescriptor::WINDOW_WIDTH / 2.0f) * -1.0f;
    const glm::vec3 yOffset = camera_plane_up * (ScreenDescriptor::WINDOW_HEIGHT / 2.0f) * -1.0f;

    for (uint32_t i = 0; i < ScreenDescriptor::WINDOW_HEIGHT; ++i)
    {
        const glm::vec3 xVec = float(i) * camera_plane_up + yOffset;
        for (uint32_t j = 0; j < ScreenDescriptor::WINDOW_WIDTH; ++j)
        {
            const glm::vec3 yVec = float(j) * camera_plane_right + xOffset;
            const glm::vec3 curr = camera_plane_center + xVec + yVec;

            const glm::vec3 rayDirection = glm::normalize(curr - Camera::intance().getPosition());

            const Ray ray{ Camera::intance().getPosition(), rayDirection };

            const glm::vec3 color = Renderer::instance().trace(ray, 1);

            const unsigned int index = i * 3 * ScreenDescriptor::WINDOW_WIDTH + j * 3;
            imgArray[index] = unsigned char(std::min(color.r * 255, 255.0f));
            imgArray[index + 1] = unsigned char(std::min(color.g * 255, 255.0f));
            imgArray[index + 2] = unsigned char(std::min(color.b * 255, 255.0f));
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenDescriptor::WINDOW_WIDTH, ScreenDescriptor::WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, imgArray);
    glGenerateMipmap(texture);

    return texture;
}

GLuint Renderer::renderToTextureParallel()
{
    const glm::vec3 camera_plane_center = Camera::intance().getPosition() + Camera::intance().getDirection() * planeDistance;
    const glm::vec3 camera_plane_right = glm::normalize(glm::cross(Camera::intance().getDirection(), Camera::intance().getUp()));
    const glm::vec3 camera_plane_up = glm::normalize(glm::cross(camera_plane_right, Camera::intance().getDirection()));

    const glm::vec3 xOffset = camera_plane_right * (ScreenDescriptor::WINDOW_WIDTH / 2.0f) * -1.0f;
    const glm::vec3 yOffset = camera_plane_up * (ScreenDescriptor::WINDOW_HEIGHT / 2.0f) * -1.0f;

    for (uint32_t i = 0; i < ScreenDescriptor::WINDOW_WIDTH; ++i)
    {
        const glm::vec3 xVec = float(i) * camera_plane_right + xOffset;
        for (uint32_t j = 0; j < ScreenDescriptor::WINDOW_HEIGHT; ++j)
        {
            const glm::vec3 yVec = float(j) * camera_plane_up + yOffset;
            const glm::vec3 curr = camera_plane_center + xVec + yVec;

            const glm::vec3 rayDirection = glm::normalize(curr - Camera::intance().getPosition());

            const Ray ray{ Camera::intance().getPosition(), rayDirection };

            const glm::vec3 color = Renderer::instance().trace(ray, 1);

            const unsigned int index = j * 3 * ScreenDescriptor::WINDOW_WIDTH + i * 3;
            imgArray[index] = unsigned char(std::min(color.r * 255, 255.0f));
            imgArray[index + 1] = unsigned char(std::min(color.g * 255, 255.0f));
            imgArray[index + 2] = unsigned char(std::min(color.b * 255, 255.0f));
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenDescriptor::WINDOW_WIDTH, ScreenDescriptor::WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, imgArray);
    glGenerateMipmap(texture);

    const auto lambda = [camera_plane_right, camera_plane_up, camera_plane_center, xOffset, yOffset, this](const size_t thread_id)
    {
        for (uint32_t i = 0; i < ScreenDescriptor::WINDOW_WIDTH; ++i)
        {
            const glm::vec3 xVec = float(i) * camera_plane_right + xOffset;
            for (uint32_t j = 0; j < ScreenDescriptor::WINDOW_HEIGHT; ++j)
            {
                const glm::vec3 yVec = float(j) * camera_plane_up + yOffset;
                const glm::vec3 curr = camera_plane_center + xVec + yVec;

                const glm::vec3 rayDirection = glm::normalize(curr - Camera::intance().getPosition());

                const Ray ray{ Camera::intance().getPosition(), rayDirection };

                const glm::vec3 color = Renderer::instance().trace(ray, 1);

                const unsigned int index = j * 3 * ScreenDescriptor::WINDOW_WIDTH + i * 3;
                imgArray[index] = unsigned char(std::min(color.r * 255, 255.0f));
                imgArray[index + 1] = unsigned char(std::min(color.g * 255, 255.0f));
                imgArray[index + 2] = unsigned char(std::min(color.b * 255, 255.0f));
            }
        }
    };

    std::vector<std::future<void>> results;

    for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
    {
        results.emplace_back(pool.enqueue(lambda, i));
    }

    for (auto&& result : results)
    {
        result.get();
    }

    //std::vector< std::future<int> > results;

    //for (int i = 0; i < 8; ++i) {
    //    results.emplace_back(
    //        pool.enqueue([i] {
    //        std::cout << "hello " << i << std::endl;
    //        std::this_thread::sleep_for(std::chrono::seconds(1));
    //        std::cout << "world " << i << std::endl;
    //        return i * i;
    //    })
    //    );
    //}

    //for (auto&& result : results)
    //    std::cout << result.get() << ' ';
    //std::cout << std::endl;

    return texture;
}

void Renderer::drawTextureToScreen(const GLuint texture)
{
    glUseProgram(quadshader.getID());
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::render()
{
    // Clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    std::fill(imgArray, imgArray + ScreenDescriptor::WINDOW_WIDTH * ScreenDescriptor::WINDOW_HEIGHT * 3 - 1, unsigned char(0));

    // Draw
    GLuint text = renderToTexture();
    drawTextureToScreen(text);

    // Delete
    glDeleteTextures(1, &text);
}