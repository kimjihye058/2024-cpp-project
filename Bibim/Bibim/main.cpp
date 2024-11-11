#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

using namespace std;
using namespace sf;

int main() {
    cout << "프로그램이 시작되었습니다." << endl;

    // 창 생성
    RenderWindow window(VideoMode(1440, 1024), "SFML Window");

    // 배경 이미지 텍스처 로드
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("img/startBg.png")) {
        cout << "배경 이미지를 로드할 수 없습니다." << endl;
        return -1;  // 로드 실패 시 종료
    }

    // 시작버튼 이미지 텍스처 로드
    Texture buttonTexture;
    if (!buttonTexture.loadFromFile("img/startBtn.png")) {
        cout << "버튼 이미지를 로드할 수 없습니다." << endl;
        return -1;
    }

    // 스프라이트에 텍스처 적용
    Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    Sprite buttonSprite;
    buttonSprite.setTexture(buttonTexture);
    buttonSprite.setPosition(420, 667); // 버튼 위치 설정

    // 버튼 상태 관리 변수
    bool isClick = false;
    Clock clock;

    // 메인 루프
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                cout << "프로그램이 종료되었습니다." << endl;
            }
            // 버튼 클릭 이벤트 처리
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (buttonSprite.getGlobalBounds().contains(Vector2f(mousePos))) {
                    cout << "버튼이 클릭되었습니다.\n1분 카운트다운이 시작됩니다." << endl;
                    isClick = true;
                    clock.restart();  // 타이머 시작
                }
            }
        }

        // 카운트다운 진행 확인
        if (isClick) {
            Time elapsed = clock.getElapsedTime();
            if (elapsed.asSeconds() >= 60) {
                cout << "1분이 경과되었습니다." << endl;
                isClick = false;  // 카운트 종료
                window.close();
            }
        }

        // 화면 지우기
        window.clear();

        // 화면에 그리기
        window.draw(backgroundSprite);
        window.draw(buttonSprite);

        // 창에 그린 내용 표시
        window.display();
    }

    return 0;
}
