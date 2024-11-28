#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace sf;

enum class Scene { StartScreen, GameScreen, BiBimScreen, EndingScreen };

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // 랜덤 시드 설정

    cout << "프로그램이 시작되었습니다." << endl;

    RenderWindow window(VideoMode(1440, 1024), "bibim period");

    Texture startBackgroundTexture, gameBackgroundTexture;
    if (!startBackgroundTexture.loadFromFile("img/startBg.png")) {
        cout << "시작 화면 배경 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!gameBackgroundTexture.loadFromFile("img/IngredientBg.png")) {
        cout << "게임 화면 배경 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }

    Texture startButtonTexture, bowlTexture, beanSproutTexture, brackenTexture, carrotTexture, cucumberTexture, friedEggTexture, meatTexture, mushroomTexture, riceTexture, spinachTexture;
    if (!startButtonTexture.loadFromFile("img/startBtn.png")) {
        cout << "start 버튼 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!bowlTexture.loadFromFile("img/bowl.png")) {
        cout << "그릇 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!beanSproutTexture.loadFromFile("img/ingredient/beanSprouts.png")) {
        cout << "콩나물 버튼 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!brackenTexture.loadFromFile("img/ingredient/bracken.png")) {
        cout << "고사리 버튼 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!carrotTexture.loadFromFile("img/ingredient/carrot.png")) {
        cout << "당근 버튼 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!cucumberTexture.loadFromFile("img/ingredient/cucumber.png")) {
        cout << "오이 버튼 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!friedEggTexture.loadFromFile("img/ingredient/friedEgg.png")) {
        cout << "계란 버튼 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!meatTexture.loadFromFile("img/ingredient/meat.png")) {
        cout << "고기 버튼 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!mushroomTexture.loadFromFile("img/ingredient/mushroom.png")) {
        cout << "버섯 버튼 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!riceTexture.loadFromFile("img/ingredient/rice.png")) {
        cout << "밥 버튼 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!spinachTexture.loadFromFile("img/ingredient/spinach.png")) {
        cout << "시금치 버튼 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }

    Font font;
    if (!font.loadFromFile("font/Pretendard-Regular.otf")) {
        cout << "폰트를 로드할 수 없습니다." << endl;
        return -1;
    }

    Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(50);
    timerText.setFillColor(Color::White);
    timerText.setPosition(20, 20);

    Sprite startBackgroundSprite(startBackgroundTexture);
    Sprite gameBackgroundSprite(gameBackgroundTexture);
    Sprite startButtonSprite(startButtonTexture);
    startButtonSprite.setPosition(420, 667);

    Sprite bowlSprite(bowlTexture);
    bowlSprite.setPosition(353, 251);

    map<string, tuple<Sprite, int, Vector2f, Vector2f>> ingredients = {
        {"egg", {Sprite(friedEggTexture), rand() % 5 + 1, {904, 644}, {621, 414}}},
        {"cucumber", {Sprite(cucumberTexture), rand() % 5 + 1, {1005, 472}, {742, 567}}},
        {"bean", {Sprite(beanSproutTexture), rand() % 5 + 1, {1025, 237}, {823, 482}}},
        {"mushroom", {Sprite(mushroomTexture), rand() % 5 + 1, {831, 99}, {791, 381}}},
        {"bracken", {Sprite(brackenTexture), rand() % 5 + 1, {571, 40}, {613, 335}}},
        {"meat", {Sprite(meatTexture), rand() % 5 + 1, {298, 78}, {496, 397}}},
        {"carrot", {Sprite(carrotTexture), rand() % 5 + 1, {81, 212}, {460, 491}}},
        {"spinach", {Sprite(spinachTexture), rand() % 5 + 1, {72, 452}, {538, 544}}},
        {"rice", {Sprite(riceTexture), rand() % 5 + 1, {166, 665}, {645, 497}}}
    };

    for (auto& ingredient : ingredients) {
        auto& [sprite, count, startPos, targetPos] = ingredient.second;
        sprite.setPosition(startPos);
    }

    Scene currentScene = Scene::StartScreen;
    bool isTimerRunning = false;
    Clock clock;

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
                }
            }

            if (currentScene == Scene::GameScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f mousePosF(mousePos.x, mousePos.y);

                for (auto& ingredient : ingredients) {
                    auto& [sprite, count, startPos, targetPos] = ingredient.second;

                    if (count > 0 && sprite.getGlobalBounds().contains(mousePosF)) {
                        cout << ingredient.first << "이(가) 선택되었습니다." << endl;
                        count--;

                        if (count == 0) {
                            sprite.setPosition(targetPos);
                            cout << ingredient.first << "이(가) 마지막입니다. 지정된 위치로 이동합니다." << endl;
                        }
                        else {
                            cout << ingredient.first << "이(가) " << count << "개 남았습니다." << endl;
                        }
                        break;
                    }
                }

                bool allDepleted = true;
                for (const auto& ingredient : ingredients) {
                    if (get<1>(ingredient.second) > 0) {
                        allDepleted = false;
                        break;
                    }
                }

                if (allDepleted) {
                    cout << "모든 재료가 소진되었습니다. 다음 화면으로 전환됩니다." << endl;
                    currentScene = Scene::BiBimScreen;
                }
            }
        }

        if (isTimerRunning) {
            Time elapsed = clock.getElapsedTime();
            int remainingTime = 60 - static_cast<int>(elapsed.asSeconds());

            if (remainingTime <= 0) {
                cout << "1분이 경과되었습니다." << endl;
                isTimerRunning = false;
                timerText.setString("00:00");
            }
            else {
                int minutes = remainingTime / 60;
                int seconds = remainingTime % 60;
                stringstream ss;
                ss << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds;
                timerText.setString(ss.str());
            }
        }

        window.clear();

        if (currentScene == Scene::StartScreen) {
            window.draw(startBackgroundSprite);
            window.draw(startButtonSprite);
            window.draw(timerText);
        }
        else if (currentScene == Scene::GameScreen) {
            window.draw(gameBackgroundSprite);
            window.draw(timerText);
            window.draw(bowlSprite);

            for (const auto& ingredient : ingredients) {
                window.draw(get<0>(ingredient.second));
            }
        }

        window.display();
    }

    return 0;
}
