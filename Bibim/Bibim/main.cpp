#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector> // 그릇에 담긴 재료 관리를 위한 벡터

using namespace std;
using namespace sf;

enum class Scene { StartScreen, GameScreen, BiBimScreen, EndingScreen }; // 화면 상태를 나타내는 열거형

int main() {
    cout << "프로그램이 시작되었습니다." << endl;

    // 창 생성
    RenderWindow window(VideoMode(1440, 1024), "bibim period");

    // 배경 이미지 텍스처 로드
    Texture startBackgroundTexture, gameBackgroundTexture;
    if (!startBackgroundTexture.loadFromFile("img/startBg.png")) {
        cout << "시작 화면 배경 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!gameBackgroundTexture.loadFromFile("img/IngredientBg.png")) {
        cout << "게임 화면 배경 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }

    // 버튼 및 재료 텍스처 로드
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

    // 폰트 로드
    Font font;
    if (!font.loadFromFile("font/Pretendard-Regular.otf")) {
        cout << "폰트를 로드할 수 없습니다." << endl;
        return -1;
    }

    // 텍스트 설정
    Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(50);
    timerText.setFillColor(Color::White);
    timerText.setPosition(20, 20);

    // 스프라이트 설정
    Sprite startBackgroundSprite(startBackgroundTexture);
    Sprite gameBackgroundSprite(gameBackgroundTexture);
    Sprite startButtonSprite(startButtonTexture);
    startButtonSprite.setPosition(420, 667);

    Sprite bowlSprite(bowlTexture);
    bowlSprite.setPosition(433, 211);

    Sprite eggSprite(friedEggTexture);
    eggSprite.setPosition(1004, 644);

    Sprite cucumberSprite(cucumberTexture);
    cucumberSprite.setPosition(1105, 472);

    Sprite beanSprite(beanSproutTexture);
    beanSprite.setPosition(1125, 277);

    Sprite mushroomSprite(mushroomTexture);
    mushroomSprite.setPosition(931, 99);

    Sprite brackenSprite(brackenTexture);
    brackenSprite.setPosition(671, 61);

    Sprite meatSprite(meatTexture);
    meatSprite.setPosition(398, 78);

    Sprite carrotSprite(carrotTexture);
    carrotSprite.setPosition(181, 242);

    Sprite spinachSprite(spinachTexture);
    spinachSprite.setPosition(172, 452);

    Sprite riceSprite(riceTexture);
    riceSprite.setPosition(266, 665);

    // 상태 관리 변수
    Scene currentScene = Scene::StartScreen;
    bool isTimerRunning = false;
    Clock clock;

    vector<Sprite> ingredientsInBowl; // 그릇에 담긴 재료를 관리하는 벡터

    // 메인 루프
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

                // 각 재료의 클릭 여부 확인
                if (eggSprite.getGlobalBounds().contains(mousePosF)) {
                    cout << "계란이 선택되었습니다." << endl;
                    Sprite addedIngredient(eggSprite);
                    addedIngredient.setPosition(635, 485);
                    ingredientsInBowl.push_back(addedIngredient);
                }
                else if (cucumberSprite.getGlobalBounds().contains(mousePosF)) {
                    cout << "오이가 선택되었습니다." << endl;
                    Sprite addedIngredient(cucumberSprite);
                    addedIngredient.setPosition(720, 552);
                    ingredientsInBowl.push_back(addedIngredient);
                }
                else if (beanSprite.getGlobalBounds().contains(mousePosF)) {
                    cout << "콩나물이 선택되었습니다." << endl;
                    Sprite addedIngredient(beanSprite);
                    addedIngredient.setPosition(823, 457);
                    ingredientsInBowl.push_back(addedIngredient);
                }
                else if (mushroomSprite.getGlobalBounds().contains(mousePosF)) {
                    cout << "버섯이 선택되었습니다." << endl;
                    Sprite addedIngredient(mushroomSprite);
                    addedIngredient.setPosition(758, 342);
                    ingredientsInBowl.push_back(addedIngredient);
                }
                else if (brackenSprite.getGlobalBounds().contains(mousePosF)) {
                    cout << "고사리가 선택되었습니다." << endl;
                    Sprite addedIngredient(brackenSprite);
                    addedIngredient.setPosition(635, 304);
                    ingredientsInBowl.push_back(addedIngredient);
                }
                else if (meatSprite.getGlobalBounds().contains(mousePosF)) {
                    cout << "고기가 선택되었습니다." << endl;
                    Sprite addedIngredient(meatSprite);
                    addedIngredient.setPosition(529, 326);
                    ingredientsInBowl.push_back(addedIngredient);
                }
                else if (carrotSprite.getGlobalBounds().contains(mousePosF)) {
                    cout << "당근이 선택되었습니다." << endl;
                    Sprite addedIngredient(carrotSprite);
                    addedIngredient.setPosition(507, 381);
                    ingredientsInBowl.push_back(addedIngredient);
                }
                else if (spinachSprite.getGlobalBounds().contains(mousePosF)) {
                    cout << "시금치가 선택되었습니다." << endl;
                    Sprite addedIngredient(spinachSprite);
                    addedIngredient.setPosition(496, 465);
                    ingredientsInBowl.push_back(addedIngredient);
                }
                else if (riceSprite.getGlobalBounds().contains(mousePosF)) {
                    cout << "밥이 선택되었습니다." << endl;
                    Sprite addedIngredient(riceSprite);
                    addedIngredient.setPosition(545, 567);
                    ingredientsInBowl.push_back(addedIngredient);
                }
            }
        }

        // 타이머 업데이트
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

        // 화면 지우기
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
            window.draw(eggSprite);
            window.draw(cucumberSprite);
            window.draw(beanSprite);
            window.draw(mushroomSprite);
            window.draw(brackenSprite);
            window.draw(meatSprite);
            window.draw(carrotSprite);
            window.draw(spinachSprite);
            window.draw(riceSprite);

            for (const auto& ingredient : ingredientsInBowl) {
                window.draw(ingredient);
            }
        }

        window.display();
    }

    return 0;
}
