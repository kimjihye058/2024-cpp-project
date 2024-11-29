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


using namespace std;
using namespace sf;

enum class Scene { StartScreen, GameScreen, BiBimScreen, MixScreen,EndingScreen };

void initializeText(Text& text, const Font& font, const wstring& content, int size, const Vector2f& position) {
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(Color::White);
    FloatRect bounds = text.getGlobalBounds();
    text.setPosition(position.x - bounds.width / 2, position.y - bounds.height / 2);
}

int main() {

    srand(static_cast<unsigned int>(time(nullptr))); // 랜덤 시드 설정

    cout << "프로그램이 시작되었습니다." << endl;

    RenderWindow window(VideoMode(1440, 1024), "bibim period");

    Music backgroundMusic; // 음악 객체 생성

    Texture startBackgroundTexture, gameBackgroundTexture, mixBackgroundTexture;
    if (!startBackgroundTexture.loadFromFile("img/startBg.png")) {
        cout << "시작 화면 배경 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!gameBackgroundTexture.loadFromFile("img/IngredientBg.png")) {
        cout << "게임 화면 배경 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!mixBackgroundTexture.loadFromFile("img/mixBg.png")) {
        cout << "비빔밥 섞기 화면 배경 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }

    // 음악 파일 로드
    if (!backgroundMusic.openFromFile("bibim.ogg")) {
        std::cerr << "음악 파일을 로드할 수 없습니다!" << std::endl;
        return -1; // 오류 종료
    }

    Texture startButtonTexture, bowlTexture,
        beanSproutTexture, brackenTexture, carrotTexture, cucumberTexture, friedEggTexture, meatTexture, mushroomTexture, riceTexture, spinachTexture,
        bibimbabTexture, gochujangTexture, sauceTexture;
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
    if (!bibimbabTexture.loadFromFile("img/bibimbab.png")) {
        cout << "비빔밥 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!gochujangTexture.loadFromFile("img/gochujang.png")) {
        cout << "고추장 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!sauceTexture.loadFromFile("img/sauce.png")) {
        cout << "고추장 소스 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }

    Font font;
    if (!font.loadFromFile("font/Pretendard-Regular.otf")) {
        cout << "폰트를 로드할 수 없습니다." << endl;
        return -1;
    }

    Text gamemessage;
    initializeText(gamemessage, font, L"Bibim 재료를 모두 넣어주세요.", 40, { 720, 974 });

    Text bibimmessage;
    initializeText(bibimmessage, font, L"고추장을 넣어주세요.", 40, { 720, 974 });

    Text mixmessage;
    initializeText(mixmessage, font, L"[space]로 비벼주세요.", 40, { 720, 974 });

    Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(50);
    timerText.setFillColor(Color::White);
    timerText.setPosition(20, 20);

    

    Sprite startBackgroundSprite(startBackgroundTexture);
    Sprite gameBackgroundSprite(gameBackgroundTexture);
    Sprite mixBackgroundSprite(mixBackgroundTexture);
    Sprite startButtonSprite(startButtonTexture);
    startButtonSprite.setPosition(420, 667);

    Sprite bowlSprite(bowlTexture);
    bowlSprite.setPosition(353, 251);

    Sprite bibimbabSprite(bibimbabTexture);
    bibimbabSprite.setPosition(35, 16);

    Sprite gochujangSprite(gochujangTexture);
    gochujangSprite.setPosition(903, 256);

    Sprite sauceSprite(sauceTexture);
    sauceSprite.setPosition(998, 330);

    // 애니메이션 상태 변수
    bool isSauceMoving = false;
    Vector2f sauceStartPosition; // 소스 시작 위치
    Vector2f sauceTargetPosition = bibimbabSprite.getPosition(); // 비빔밥 위치
    Clock sauceClock; // 소스 이동 타이머

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
    Clock transitionClock;


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                cout << "프로그램이 종료되었습니다." << endl;
            }

            // StartScreen 이벤트 처리
            if (currentScene == Scene::StartScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (startButtonSprite.getGlobalBounds().contains(Vector2f(mousePos))) {
                    cout << "버튼이 클릭되었습니다. 게임 화면으로 전환됩니다." << endl;
                    currentScene = Scene::GameScreen;
                    isTimerRunning = true;
                    clock.restart();

                    // 음악 재생
                    backgroundMusic.play();
                }
            }

            // GameScreen 이벤트 처리
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

            // 비빔밥 씬에서의 이벤트 처리
            if (currentScene == Scene::BiBimScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f mousePosF(mousePos.x, mousePos.y);

                // 고추장 클릭 이벤트 처리
                if (sauceSprite.getGlobalBounds().contains(mousePosF)) {
                    // 소스를 비빔밥 위치로 이동 (348, 384)
                    sauceSprite.setPosition(348, 384);
                    // 이전 위치 (998, 330)에서 비빔밥 위치로 이동했음을 표시
                    isSauceMoving = true;
                    sauceClock.restart();
                }
            }

            // 소스 이동 처리
            if (isSauceMoving) {
                float elapsedTime = sauceClock.getElapsedTime().asSeconds();

                if (elapsedTime >= 1.0f) { // 비빔밥 위치에 도달한 후 1초 지나면
                    currentScene = Scene::MixScreen;
                    isSauceMoving = false;
                }
            }

            // MixScreen event handling (for mixing the bibimbap)
            if (currentScene == Scene::MixScreen && event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                cout << "비빔밥을 비벼주세요!" << endl;
                // You can add a mixing animation or gameplay mechanic here
                currentScene = Scene::EndingScreen; // After mixing, go to the ending screen
            }
        }

        if (isTimerRunning) {
            Time elapsed = clock.getElapsedTime();
            int remainingTime = 60 - static_cast<int>(elapsed.asSeconds());

            if (remainingTime <= 0) {
                cout << "1분이 경과되었습니다." << endl;
                isTimerRunning = false;
                window.close();
                timerText.setString("00:00");
            }
            else {
                // 남은 시간을 문자열로 변환
                int minutes = remainingTime / 60;
                int seconds = remainingTime % 60;
                stringstream ss;
                ss << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds;
                timerText.setString(ss.str());
            }
        }


        window.clear();
        // Draw appropriate screen based on the current scene
        switch (currentScene) {
        case Scene::StartScreen:
            window.draw(startBackgroundSprite);
            window.draw(startButtonSprite);
            break;

        case Scene::GameScreen:
            window.draw(gameBackgroundSprite);
            window.draw(timerText);
            window.draw(bowlSprite);
            for (auto& ingredient : ingredients) {
                window.draw(get<0>(ingredient.second)); // Draw each ingredient sprite
            }
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
            break;

        case Scene::EndingScreen:
            window.draw(timerText);
            break;
        }

        window.display();
    }

    return 0;
}
