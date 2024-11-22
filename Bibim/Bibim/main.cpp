#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <iomanip>
#include <sstream> // for stringstream

using namespace std;
using namespace sf;

enum class Scene { StartScreen, GameScreen }; // 화면 상태를 나타내는 열거형

int main() {
    cout << "프로그램이 시작되었습니다." << endl;

    // 창 생성
    RenderWindow window(VideoMode(1440, 1024), "SFML Window");

    // 배경 이미지 텍스처 로드
    Texture startBackgroundTexture, gameBackgroundTexture;
    if (!startBackgroundTexture.loadFromFile("img/startBg.png")) {
        cout << "시작 화면 배경 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }
    if (!gameBackgroundTexture.loadFromFile("img/IngredientBg.png")) { // 게임 화면 배경
        cout << "게임 화면 배경 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }

    // 시작버튼 이미지 텍스처 로드
    Texture buttonTexture;
    if (!buttonTexture.loadFromFile("img/startBtn.png")) {
        cout << "버튼 이미지를 로드할 수 없습니다." << endl;
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
    timerText.setPosition(20, 20); // 텍스트를 왼쪽 위로 이동

    // 스프라이트에 텍스처 적용
    Sprite startBackgroundSprite;
    startBackgroundSprite.setTexture(startBackgroundTexture);

    Sprite gameBackgroundSprite;
    gameBackgroundSprite.setTexture(gameBackgroundTexture);

    Sprite buttonSprite;
    buttonSprite.setTexture(buttonTexture);
    buttonSprite.setPosition(420, 667); // 버튼 위치 설정

    // 상태 관리 변수
    Scene currentScene = Scene::StartScreen; // 초기 화면은 시작 화면
    bool isTimerRunning = false;
    Clock clock;

    // 메인 루프
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                cout << "프로그램이 종료되었습니다." << endl;
            }

            if (currentScene == Scene::StartScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                // 버튼 클릭 이벤트 처리
                Vector2i mousePos = Mouse::getPosition(window);
                if (buttonSprite.getGlobalBounds().contains(Vector2f(mousePos))) {
                    cout << "버튼이 클릭되었습니다. 게임 화면으로 전환됩니다." << endl;
                    currentScene = Scene::GameScreen; // 게임 화면으로 전환
                    isTimerRunning = true;            // 타이머 시작
                    clock.restart();                 // 타이머 리셋
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
                // 남은 시간을 문자열로 변환
                int minutes = remainingTime / 60;
                int seconds = remainingTime % 60;
                stringstream ss;
                ss << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds;
                timerText.setString(ss.str());
            }
        }

        // 화면 지우기
        window.clear();

        // 현재 화면 그리기
        if (currentScene == Scene::StartScreen) {
            window.draw(startBackgroundSprite); // 시작 화면 배경
            window.draw(buttonSprite);         // 시작 버튼
            window.draw(timerText);            // 타이머
        }
        else if (currentScene == Scene::GameScreen) {
            window.draw(gameBackgroundSprite); // 게임 화면 배경
            window.draw(timerText);            // 타이머
        }

        // 창에 그린 내용 표시
        window.display();
    }

    return 0;
}
