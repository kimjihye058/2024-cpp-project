#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

using namespace std;
using namespace sf;

enum class Scene { StartScreen, GameScreen, BiBimScreen, MixScreen, EndingScreen, PassScreen, OverScreen };

// 텍스트 초기화 함수
void initializeText(Text& text, const Font& font, const wstring& content, int size, const Vector2f& position) {
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(Color::White);
    FloatRect bounds = text.getGlobalBounds();
    text.setPosition(position.x - bounds.width / 2, position.y - bounds.height / 2);
}

// 재료 클래스
class Ingredient {
public:
    Ingredient(const Texture& texture, int count, const Vector2f& startPos, const Vector2f& targetPos) :
        texture_(texture), count_(count), startPos_(startPos), targetPos_(targetPos) {
        sprite_.setTexture(texture_);
        sprite_.setPosition(startPos_);
    }

    bool isCollected() const { return count_ == 0; }
    void collect() {
        count_--;
        if (count_ == 0) sprite_.setPosition(targetPos_);
    }
    void draw(RenderWindow& window) const { window.draw(sprite_); }
    bool contains(const Vector2f& point) const { return sprite_.getGlobalBounds().contains(point); }
    const Sprite& getSprite() const { return sprite_; }


private:
    Texture texture_;
    Sprite sprite_;
    int count_;
    Vector2f startPos_;
    Vector2f targetPos_;
};


int mixCount = 0;
const int MAX_MIX_COUNT = 100;
RectangleShape mixGauge, mixGaugeBackground;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    cout << "프로그램이 시작되었습니다." << endl;

    RenderWindow window(VideoMode(1440, 1024), "bibim period");
    Music backgroundMusic;

    // 텍스처들을 위한 Texture 객체들을 미리 선언합니다.
    Texture startBackgroundTexture, gameBackgroundTexture, mixBackgroundTexture, passBackgroundTexture, overBackgrountTexture;
    Texture startButtonTexture, retryButtonTexture, bowlTexture;
    Texture beanSproutTexture, brackenTexture, carrotTexture, cucumberTexture, friedEggTexture, meatTexture, mushroomTexture, riceTexture, spinachTexture;
    Texture bibimbabTexture, gochujangTexture, sauceTexture;


    // 텍스처 로딩 (에러 처리 추가)  -  함수로 만들어서 코드를 깔끔하게 정리합니다.
    auto loadTexture = [&](const string& path, Texture& texture) {
        if (!texture.loadFromFile(path)) {
            cerr << "Error loading texture: " << path << endl;
            return false;
        }
        return true;
        };

    // 모든 텍스처 로드하고 에러 체크
    if (!loadTexture("img/startBg.png", startBackgroundTexture) ||
        !loadTexture("img/IngredientBg.png", gameBackgroundTexture) ||
        !loadTexture("img/mixBg.png", mixBackgroundTexture) ||
        !loadTexture("img/passBg.png", passBackgroundTexture) ||
        !loadTexture("img/overBg.png", overBackgrountTexture) ||
        !loadTexture("img/startBtn.png", startButtonTexture) ||
        !loadTexture("img/retryBtn.png", retryButtonTexture) ||
        !loadTexture("img/bowl.png", bowlTexture) ||
        !loadTexture("img/bibimbab.png", bibimbabTexture) ||
        !loadTexture("img/gochujang.png", gochujangTexture) ||
        !loadTexture("img/sauce.png", sauceTexture) ||
        !loadTexture("img/ingredient/beanSprouts.png", beanSproutTexture) ||
        !loadTexture("img/ingredient/bracken.png", brackenTexture) ||
        !loadTexture("img/ingredient/carrot.png", carrotTexture) ||
        !loadTexture("img/ingredient/cucumber.png", cucumberTexture) ||
        !loadTexture("img/ingredient/friedEgg.png", friedEggTexture) ||
        !loadTexture("img/ingredient/meat.png", meatTexture) ||
        !loadTexture("img/ingredient/mushroom.png", mushroomTexture) ||
        !loadTexture("img/ingredient/rice.png", riceTexture) ||
        !loadTexture("img/ingredient/spinach.png", spinachTexture)
        ) {
        return -1;
    }

    if (!backgroundMusic.openFromFile("bibim.ogg")) {
        cerr << "Error loading music file!" << endl;
        return -1;
    }

    Font font;
    if (!font.loadFromFile("font/Pretendard-Regular.otf")) {
        cerr << "Error loading font file!" << endl;
        return -1;
    }

    // 텍스트 초기화
    Text gamemessage, bibimmessage, mixmessage, timerText;
    initializeText(gamemessage, font, L"Bibim 재료를 모두 넣어주세요.", 40, { 720, 974 });
    initializeText(bibimmessage, font, L"고추장을 넣어주세요.", 40, { 720, 974 });
    initializeText(mixmessage, font, L"[space]로 비벼주세요.", 40, { 720, 974 });
    timerText.setFont(font);
    timerText.setCharacterSize(50);
    timerText.setFillColor(Color::White);
    timerText.setPosition(20, 20);

    // 스프라이트 생성 및 위치 설정
    Sprite startBackgroundSprite(startBackgroundTexture), gameBackgroundSprite(gameBackgroundTexture),
        mixBackgroundSprite(mixBackgroundTexture), passBackgroundSprite(passBackgroundTexture),
        overBackgrountSprite(overBackgrountTexture), startButtonSprite(startButtonTexture),
        retryButtonSprite(retryButtonTexture), bowlSprite(bowlTexture), bibimbabSprite(bibimbabTexture),
        gochujangSprite(gochujangTexture), sauceSprite(sauceTexture);

    startButtonSprite.setPosition(420, 667);
    retryButtonSprite.setPosition(20, 20);
    bowlSprite.setPosition(353, 251);
    bibimbabSprite.setPosition(35, 16);
    gochujangSprite.setPosition(903, 256);
    sauceSprite.setPosition(998, 330);

    // 재료 생성 및 초기화
    vector<unique_ptr<Ingredient>> ingredients;
    map<string, tuple<Texture, Vector2f, Vector2f>> ingredientData = {
        {"egg", {friedEggTexture, {904, 644}, {521, 404}}},
        {"cucumber", {cucumberTexture, {1005, 472}, {642, 557}}},
        {"bean", {beanSproutTexture, {1025, 237}, {723, 472}}},
        {"mushroom", {mushroomTexture, {831, 99}, {691, 371}}},
        {"bracken", {brackenTexture, {571, 40}, {513, 325}}},
        {"meat", {meatTexture, {298, 78}, {396, 387}}},
        {"carrot", {carrotTexture, {81, 212}, {360, 481}}},
        {"spinach", {spinachTexture, {72, 452}, {438, 534}}},
        {"rice", {riceTexture, {166, 665}, {545, 487}}}
    };

    for (auto& [name, data] : ingredientData) {
        auto& [texture, startPos, targetPos] = data;
        ingredients.emplace_back(make_unique<Ingredient>(texture, rand() % 5 + 1, startPos, targetPos));
    }

    //믹싱 게이지 설정
    mixGaugeBackground.setSize({ 800, 30 });
    mixGaugeBackground.setPosition({ 320, 700 });
    mixGaugeBackground.setFillColor(Color(100, 100, 100));
    mixGauge.setSize({ 0, 30 });
    mixGauge.setPosition({ 320, 700 });
    mixGauge.setFillColor(Color::Red);

    bool isSauceMoving = false;
    Vector2f sauceStartPosition;
    Vector2f sauceTargetPosition = bibimbabSprite.getPosition();
    Clock sauceClock;
    Scene currentScene = Scene::StartScreen;
    bool isTimerRunning = false;
    Clock clock;
    Clock transitionClock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                cout << "프로그램이 종료되었습니다." << endl;
            }

            if (currentScene == Scene::StartScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (startButtonSprite.getGlobalBounds().contains(Vector2f(mousePos))) {
                    cout << "버튼이 클릭되었습니다. 게임 화면으로 전환됩니다." << endl;
                    currentScene = Scene::GameScreen;
                    isTimerRunning = true;
                    clock.restart();
                    backgroundMusic.play();
                }
            }

            if (currentScene == Scene::GameScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f mousePosF(mousePos.x, mousePos.y);
                for (auto& ingredient : ingredients) {
                    if (!ingredient->isCollected() && ingredient->contains(mousePosF)) {
                        ingredient->collect();
                        break;
                    }
                }
                bool allCollected = true;
                for (const auto& ing : ingredients) {
                    if (!ing->isCollected()) {
                        allCollected = false;
                        break;
                    }
                }
                if (allCollected) currentScene = Scene::BiBimScreen;
            }

            if (currentScene == Scene::BiBimScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f mousePosF(mousePos.x, mousePos.y);
                if (sauceSprite.getGlobalBounds().contains(mousePosF)) {
                    sauceSprite.setPosition(348, 384);
                    isSauceMoving = true;
                    sauceClock.restart();
                }
            }

            if (isSauceMoving) {
                float elapsedTime = sauceClock.getElapsedTime().asSeconds();
                if (elapsedTime > 0.5f) {
                    currentScene = Scene::MixScreen;
                    isSauceMoving = false;
                }
            }

            if (currentScene == Scene::MixScreen && event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                mixCount++;
                float gaugeWidth = (mixCount / static_cast<float>(MAX_MIX_COUNT)) * 800;
                mixGauge.setSize({ gaugeWidth, 30 });
                if (mixCount >= MAX_MIX_COUNT) currentScene = Scene::EndingScreen;
            }

            if (currentScene == Scene::OverScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (retryButtonSprite.getGlobalBounds().contains(Vector2f(mousePos))) {
                    currentScene = Scene::StartScreen;
                }
            }
        }

        if (isTimerRunning) {
            Time elapsed = clock.getElapsedTime();
            int remainingTime = 60 - static_cast<int>(elapsed.asSeconds());
            if (remainingTime <= 0) {
                isTimerRunning = false;
                timerText.setString("00:00");
                currentScene = Scene::OverScreen;
            }
            else {
                int minutes = remainingTime / 60;
                int seconds = remainingTime % 60;
                stringstream ss;
                ss << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds;
                timerText.setString(ss.str());
                if (remainingTime > 0 && currentScene == Scene::EndingScreen) {
                    isTimerRunning = false;
                    currentScene = Scene::PassScreen;
                }
            }
        }

        window.clear();
        switch (currentScene) {
        case Scene::StartScreen:
            window.draw(startBackgroundSprite);
            window.draw(startButtonSprite);
            break;
        case Scene::GameScreen:
            window.draw(gameBackgroundSprite);
            window.draw(timerText);
            window.draw(bowlSprite);
            for (const auto& ing : ingredients) ing->draw(window);
            window.draw(gamemessage);
            break;
        case Scene::BiBimScreen:
            window.draw(gameBackgroundSprite);
            window.draw(timerText);
            window.draw(gochujangSprite);
            window.draw(bibimbabSprite);
            window.draw(sauceSprite);
            window.draw(bibimmessage);
            break;
        case Scene::MixScreen:
            window.draw(mixBackgroundSprite);
            window.draw(timerText);
            window.draw(mixmessage);
            window.draw(mixGaugeBackground);
            window.draw(mixGauge);
            break;
        case Scene::EndingScreen:
            window.draw(timerText);
            break;
        case Scene::PassScreen:
            window.draw(passBackgroundSprite);
            window.draw(timerText);
            timerText.setPosition(600, 187);
            break;
        case Scene::OverScreen:
            window.draw(overBackgrountSprite);
            window.draw(retryButtonSprite);
            break;
        }
        window.display();
    }
    return 0;
}