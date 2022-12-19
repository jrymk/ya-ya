#include "graphics.h"
#include "gamecontroller.h"

bool graphicsIsFullscreen = false;

void Graphics::loadFont(int idx, std::string path) {
    if (idx >= fonts.size()) {
        fonts.resize(idx + 1);
        isFontValid.resize(idx + 1);
    }
    if (!(isFontValid[idx] = fonts[idx].loadFromFile(path)))
        debug << "Failed to load font: " << path << "\n";
}

void Graphics::setFont(int idx) {
    if (idx >= isFontValid.size())
        debug << "Index out of range\n";
    else if (!isFontValid[idx])
        debug << "Font is invalid (didn't load or failed)\n";
    else
        selectedFont = idx;
}

void Graphics::fillRect(const sf::Color &fillColor, UIVec pos1, UIVec pos2) {
    sf::RectangleShape rect;
    rect.setPosition(pos1.min(pos2).getVec2f());
    rect.setSize(((pos1.max(pos2)) - (pos1.min(pos2))).getVec2f());
    rect.setFillColor(fillColor);
    window->draw(rect);
}

void Graphics::drawRect(const sf::Color &strokeColor, float strokeWidth, UIVec pos1, UIVec pos2) {
    sf::RectangleShape rect;
    rect.setPosition(pos1.min(pos2).getVec2f());
    rect.setSize(((pos1.max(pos2)) - (pos1.min(pos2))).getVec2f());
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(strokeColor);
    rect.setOutlineThickness(-strokeWidth); // positive: grow inwards
    window->draw(rect);
}

void Graphics::drawLine(const sf::Color &strokeColor, float strokeWidth, UIVec pos1, UIVec pos2) {
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(pos1.len(pos2), strokeWidth));
    rect.setOrigin(pos1.len(pos2) / 2., strokeWidth / 2.);
    rect.setPosition(((pos1 + pos2) / 2.).getVec2f());
    rect.setRotation(pos1.angle(pos2) / 3.14159265359 * 180);
    rect.setFillColor(strokeColor);
    window->draw(rect);
}

void Graphics::drawText(const std::string &str, const sf::Color &fillColor, int size, UIVec pos, float align, const sf::Color &outlineColor, float outlineStrokeWidth) {
    sf::Text text;
    text.setPosition(0, 0);
    text.setFont(fonts[selectedFont]);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(fillColor);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition((pos + UIVec(bounds.width * -align, -size)).getVec2f());
    text.setOutlineColor(outlineColor);
    text.setOutlineThickness(outlineStrokeWidth);
    window->draw(text);

    if (showWireframe) {
        // magenta rectangle: actual bounds
        drawRect(sf::Color::Magenta, 1, pos + UIVec(bounds.width * -align, -size + bounds.top),
                 pos + UIVec(bounds.width * -align, -size + bounds.top) + UIVec(bounds.width, bounds.height));
        // cyan rectangle: baseline(input y coord) and size for reference
        drawRect(sf::Color::Cyan, 1, pos + UIVec(bounds.width * -align, -size), pos + UIVec(bounds.width * -align, -size) + UIVec(bounds.width, size));
    }
}

void Graphics::drawText(const std::wstring &str, const sf::Color &fillColor, int size, UIVec pos, float align, const sf::Color &outlineColor, float outlineStrokeWidth) {
    sf::Text text;
    text.setPosition(0, 0);
    text.setFont(fonts[selectedFont]);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(fillColor);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition((pos + UIVec(bounds.width * -align, -size)).getVec2f());
    text.setOutlineColor(outlineColor);
    text.setOutlineThickness(outlineStrokeWidth);
    window->draw(text);

    if (showWireframe) {
        // magenta rectangle: actual bounds
        drawRect(sf::Color::Magenta, 1, pos + UIVec(bounds.width * -align, -size + bounds.top),
                 pos + UIVec(bounds.width * -align, -size + bounds.top) + UIVec(bounds.width, bounds.height));
        // cyan rectangle: baseline(input y coord) and size for reference
        drawRect(sf::Color::Cyan, 1, pos + UIVec(bounds.width * -align, -size), pos + UIVec(bounds.width * -align, -size) + UIVec(bounds.width, size));
    }
}

void Graphics::drawTextBatch(sf::VertexArray &va, const std::string &str, const sf::Color &fillColor, int size, UIVec pos, float align) {
    sf::Text text;
    text.setPosition(0, 0);
    text.setFont(fonts[selectedFont]);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(fillColor);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition((pos + UIVec(bounds.width * -align, -size)).getVec2f());
    window->draw(text);

    if (showWireframe) {
        // magenta rectangle: actual bounds
        drawRect(sf::Color::Magenta, 1, pos + UIVec(bounds.width * -align, -size + bounds.top),
                 pos + UIVec(bounds.width * -align, -size + bounds.top) + UIVec(bounds.width, bounds.height));
        // cyan rectangle: baseline(input y coord) and size for reference
        drawRect(sf::Color::Cyan, 1, pos + UIVec(bounds.width * -align, -size), pos + UIVec(bounds.width * -align, -size) + UIVec(bounds.width, size));
    }
}

void Graphics::drawImage(const Image &image, float zDepth, UIVec pos, UIVec align, float scale, sf::Color color) {
    scale *= image.defaultScale;
    insertQuad(Quad(
            zDepth,
            pos + UIVec(image.textureSize) * UIVec(0., 0.) - (UIVec(image.textureSize) * scale) * align, image.texturePos,
            pos + UIVec(image.textureSize) * UIVec(scale, 0.) - (UIVec(image.textureSize) * scale) * align, image.texturePos + sf::Vector2f(image.textureSize.x, 0.),
            pos + UIVec(image.textureSize) * UIVec(scale, scale) - (UIVec(image.textureSize) * scale) * align, image.texturePos + image.textureSize,
            pos + UIVec(image.textureSize) * UIVec(0., scale) - (UIVec(image.textureSize) * scale) * align, image.texturePos + sf::Vector2f(0., image.textureSize.y),
            color
    ));
}

UIRect Graphics::drawImageGetRect(const Image &image, float zDepth, UIVec pos, UIVec align, float scale, sf::Color color) {
    scale *= image.defaultScale;
    insertQuad(Quad(
            zDepth,
            pos + UIVec(image.textureSize) * UIVec(0., 0.) - (UIVec(image.textureSize) * scale) * align, image.texturePos,
            pos + UIVec(image.textureSize) * UIVec(scale, 0.) - (UIVec(image.textureSize) * scale) * align, image.texturePos + sf::Vector2f(image.textureSize.x, 0.),
            pos + UIVec(image.textureSize) * UIVec(scale, scale) - (UIVec(image.textureSize) * scale) * align, image.texturePos + image.textureSize,
            pos + UIVec(image.textureSize) * UIVec(0., scale) - (UIVec(image.textureSize) * scale) * align, image.texturePos + sf::Vector2f(0., image.textureSize.y),
            color
    ));
    UIRect rect(pos + UIVec(image.textureSize) * UIVec(0., 0.) - (UIVec(image.textureSize) * scale) * align,
                (pos + UIVec(image.textureSize) * UIVec(scale, scale) - (UIVec(image.textureSize) * scale) * align) -
                (pos + UIVec(image.textureSize) * UIVec(0., 0.) - (UIVec(image.textureSize) * scale) * align));
    return rect;
}

void Graphics::appendWireframe(UIVec v0, UIVec v1, UIVec v2, UIVec v3, const sf::Color &color, const sf::Color &bgcolor) {
    wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., 1.), bgcolor));

    wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., -1.), bgcolor));

    wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));

    wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));

    wireframeVertexArray.append(sf::Vertex(v0.getVec2f(), color));
    wireframeVertexArray.append(sf::Vertex(v1.getVec2f(), color));
    wireframeVertexArray.append(sf::Vertex(v1.getVec2f(), color));
    wireframeVertexArray.append(sf::Vertex(v2.getVec2f(), color));
    wireframeVertexArray.append(sf::Vertex(v2.getVec2f(), color));
    wireframeVertexArray.append(sf::Vertex(v3.getVec2f(), color));
    wireframeVertexArray.append(sf::Vertex(v3.getVec2f(), color));
    wireframeVertexArray.append(sf::Vertex(v0.getVec2f(), color));
}

void Graphics::clearQuadsArray() {
    quadsArray.clear();
}

void Graphics::insertQuad(const Graphics::Quad &quad) {
    quadsArray.push_back(quad);
}

void Graphics::insertUserWireframe(UIVec v0, UIVec v1, UIVec v2, UIVec v3, const sf::Color &color, const sf::Color &bgcolor) {
    userWireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., 1.), bgcolor));

    userWireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., -1.), bgcolor));

    userWireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., 1.), bgcolor));

    userWireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));
    userWireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., -1.), bgcolor));

    userWireframeVertexArray.append(sf::Vertex(v0.getVec2f(), color));
    userWireframeVertexArray.append(sf::Vertex(v1.getVec2f(), color));
    userWireframeVertexArray.append(sf::Vertex(v1.getVec2f(), color));
    userWireframeVertexArray.append(sf::Vertex(v2.getVec2f(), color));
    userWireframeVertexArray.append(sf::Vertex(v2.getVec2f(), color));
    userWireframeVertexArray.append(sf::Vertex(v3.getVec2f(), color));
    userWireframeVertexArray.append(sf::Vertex(v3.getVec2f(), color));
    userWireframeVertexArray.append(sf::Vertex(v0.getVec2f(), color));
}

void Graphics::renderQuads(sf::RenderWindow &window, sf::Texture &texture, UIRect viewport) {
    vertexArray.clear();
    vertexArray.setPrimitiveType(sf::PrimitiveType::Quads);
    wireframeVertexArray.clear();
    wireframeVertexArray.setPrimitiveType(sf::PrimitiveType::Lines);

    std::sort(quadsArray.begin(), quadsArray.end());
    outOfSightQuads = 0;

    for (auto &quad: quadsArray) {
        if ((quad.v0.x<(viewport.pos.x - viewport.size.x) || quad.v0.x>(viewport.pos.x + 2. * viewport.size.x) ||
             quad.v0.y<(viewport.pos.y - viewport.size.y) || quad.v0.y>(viewport.pos.y + 2. * viewport.size.y)) &&
            (quad.v1.x<(viewport.pos.x - viewport.size.x) || quad.v1.x>(viewport.pos.x + 2. * viewport.size.x) ||
             quad.v1.y<(viewport.pos.y - viewport.size.y) || quad.v1.y>(viewport.pos.y + 2. * viewport.size.y)) &&
            (quad.v2.x<(viewport.pos.x - viewport.size.x) || quad.v2.x>(viewport.pos.x + 2. * viewport.size.x) ||
             quad.v2.y<(viewport.pos.y - viewport.size.y) || quad.v2.y>(viewport.pos.y + 2. * viewport.size.y)) &&
            (quad.v3.x<(viewport.pos.x - viewport.size.x) || quad.v3.x>(viewport.pos.x + 2. * viewport.size.x) ||
             quad.v3.y<(viewport.pos.y - viewport.size.y) || quad.v3.y>(viewport.pos.y + 2. * viewport.size.y))) {
            if (debugOutOfSight) {
                UIVec center = (quad.v0 + quad.v1 + quad.v2 + quad.v3) / 4.;
                center.x = std::min(std::max(center.x, (float) 5.), viewport.size.x - (float) 5.);
                center.y = std::min(std::max(center.y, (float) 5.), viewport.size.y - (float) 5.);
                appendWireframe(center + UIVec(3., 0.),
                                center + UIVec(0., 3.),
                                center + UIVec(-3., 0.),
                                center + UIVec(0., -3.),
                                sf::Color::Yellow, sf::Color::Black);
            }
            outOfSightQuads++;
        }
        else {
            vertexArray.append(sf::Vertex(quad.v0.getVec2f(), quad.c0, quad.t0));
            vertexArray.append(sf::Vertex(quad.v1.getVec2f(), quad.c1, quad.t1));
            vertexArray.append(sf::Vertex(quad.v2.getVec2f(), quad.c2, quad.t2));
            vertexArray.append(sf::Vertex(quad.v3.getVec2f(), quad.c3, quad.t3));
        }

        // if (showWireframe)
        appendWireframe(quad.v0, quad.v1, quad.v2, quad.v3, sf::Color::Black, sf::Color::Transparent);
    }

    window.draw(vertexArray, &texture);

    if (!showWireframe)
        wireframeVertexArray.clear();
    for (int i = 0; i < userWireframeVertexArray.getVertexCount(); i++)
        wireframeVertexArray.append(userWireframeVertexArray[i]);
    window.draw(wireframeVertexArray);
    userWireframeVertexArray.clear();
}

int Graphics::getQuadCount() {
    return vertexArray.getVertexCount() / 4;
}

void Graphics::resizeView(unsigned int width, unsigned int height) {
    debug << "Resized to " << width << "*" << height << "\n";
    getRenderWindow().setSize(sf::Vector2u(width, height));
    sf::View view = getRenderWindow().getView();
    view.setSize(sf::Vector2f(width, height));
    view.setCenter(sf::Vector2f(width / 2., height / 2.));
    getRenderWindow().setView(view);
}

sf::ContextSettings Graphics::getContextSettings() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    return settings;
}

void Graphics::setupWindow() {
    getRenderWindow().setVerticalSyncEnabled(true);
    getRenderWindow().setKeyRepeatEnabled(false);
}

void Graphics::createWindow(bool fullscreen) {
    if (getRenderWindow().isOpen())
        getRenderWindow().close();
    else
        debug << "Created window, fullscreen: " << fullscreen << "\n";
    if (fullscreen)
        getRenderWindow().create(sf::VideoMode::getFullscreenModes()[0], L"Ya-Ya!", sf::Style::Fullscreen, getContextSettings());
    else
        getRenderWindow().create(sf::VideoMode(1600, 900), L"Ya-Ya!", sf::Style::Default, getContextSettings());
    setupWindow();
}

void Graphics::noobFilter(sf::RenderWindow& window, GameController& controller) {
    sf::RectangleShape rect(sf::Vector2f(window.getSize().x, window.getSize().y));

    // trial noob filter xD
    double timeUntilEnd = DAY_LENGTH - controller.startOfDayTp.elapsed();
    
    if(controller.gameState == GameController::DAY_END_SCENE)
        rect.setFillColor(sf::Color(180, 40, 0, 100));
    else if(controller.gameState == GameController::DAY_START_SCENE)
        rect.setFillColor(sf::Color(225, 240, 255, 25));
    else if(timeUntilEnd > DAY_LENGTH / 2) {
        timeUntilEnd -= DAY_LENGTH / 2;
        rect.setFillColor(sf::Color(255 - 30 * timeUntilEnd / (DAY_LENGTH / 2),
                                    255 - 15 * timeUntilEnd / (DAY_LENGTH / 2),
                                    255,  25 * timeUntilEnd / (DAY_LENGTH / 2)));
    }
    else if(timeUntilEnd < DAY_LENGTH / 3){
        rect.setFillColor(sf::Color(180 +  75 * timeUntilEnd / (DAY_LENGTH / 3),
                                    40  + 215 * timeUntilEnd / (DAY_LENGTH / 3),
                                          255 * timeUntilEnd / (DAY_LENGTH / 3),
                                    100 - 100 * timeUntilEnd / (DAY_LENGTH / 3)));
    }
    else
        rect.setFillColor(sf::Color(255, 255, 255, 0));
    window.draw(rect);
}