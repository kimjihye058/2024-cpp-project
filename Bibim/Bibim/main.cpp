#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void main() {
    cout << "프로그램이 시작되었습니다." << endl;
	RenderWindow window(VideoMode(1200, 900), "SFML Window");	// sfml 창 생성
    while (window.isOpen()) {   //SFML 메인 루프 - 윈도우가 닫힐때 까지 반복
        Event event;    // 창 열기
        while (window.pollEvent(event)) {       // 이벤트 처리
            if (event.type == Event::Closed) {  // 닫기를 누르면 
                window.close();     // 창 닫기
                cout << "프로그램이 종료되었습니다." << endl;
            }
        }
    }
}