#include <SFML/Graphics.hpp> // SFML 그래픽 라이브러리 헤더 파일 포함
#include <SFML/System.hpp> // SFML 시스템 라이브러리 헤더 파일 포함
#include <SFML/Audio.hpp> // SFML 오디오 라이브러리 헤더 파일 포함
#include <iostream> // C++ 표준 입출력 스트림 헤더 파일 포함
#include <iomanip> // C++ 입출력 조작자 (예: setw, setfill) 헤더 파일 포함
#include <sstream> // C++ 문자열 스트림 헤더 파일 포함
#include <map> // C++ map 컨테이너 헤더 파일 포함
#include <cstdlib> // C++ 표준 일반 유틸리티 함수 (예: srand) 헤더 파일 포함
#include <ctime> // C++ 시간 관련 함수 (예: time) 헤더 파일 포함
#include <cmath> // C++ 수학 함수 헤더 파일 포함
#include <vector> // C++ 벡터 컨테이너 헤더 파일 포함

using namespace std; // std 네임스페이스를 현재 네임스페이스에 포함
using namespace sf; // sf 네임스페이스를 현재 네임스페이스에 포함

enum class Scene { StartScreen, GameScreen, BiBimScreen, MixScreen, PassScreen, OverScreen }; // 게임의 다양한 장면을 나타내는 열거형

// 텍스트 객체를 초기화하는 함수. 폰트, 문자열, 크기, 위치를 설정합니다.
void initializeText(Text& text, const Font& font, const wstring& content, int size, const Vector2f& position) {
    text.setFont(font); // 텍스트에 폰트 설정
    text.setString(content); // 텍스트에 문자열 설정
    text.setCharacterSize(size); // 텍스트 크기 설정
    text.setFillColor(Color::White); // 텍스트 색상을 흰색으로 설정
    FloatRect bounds = text.getGlobalBounds(); // 텍스트의 경계 사각형 정보를 가져옴
    text.setPosition(position.x - bounds.width / 2, position.y - bounds.height / 2); // 텍스트를 position을 중심으로 배치
}

// 비빔밥 재료를 나타내는 클래스
class Ingredient {
public:
    // 생성자: 텍스처, 개수, 초기 위치, 목표 위치를 설정합니다.
    Ingredient(const Texture& texture, int count, const Vector2f& startPos, const Vector2f& targetPos) :
        texture_(texture), count_(count), startPos_(startPos), targetPos_(targetPos) {
        sprite_.setTexture(texture_); // 스프라이트에 텍스처 설정
        sprite_.setPosition(startPos_); // 스프라이트 초기 위치 설정
    }

    // 모든 재료가 사용되었는지 확인합니다.
    bool isCollected() const { return count_ == 0; }
    // 재료를 하나 사용합니다.
    void collect() {
        count_--; // 재료 개수 감소
        if (count_ == 0) sprite_.setPosition(targetPos_); // 개수가 0이면 목표 위치로 이동
    }
    // 스프라이트를 화면에 그립니다.
    void draw(RenderWindow& window) const { window.draw(sprite_); }
    // 주어진 점이 스프라이트 영역 안에 있는지 확인합니다.
    bool contains(const Vector2f& point) const { return sprite_.getGlobalBounds().contains(point); }
    // 스프라이트에 대한 참조를 반환합니다.
    const Sprite& getSprite() const { return sprite_; }


private:
    Texture texture_; // 재료의 텍스처
    Sprite sprite_; // 재료의 스프라이트
    int count_; // 남아있는 재료 개수
    Vector2f startPos_; // 재료의 초기 위치
    Vector2f targetPos_; // 재료의 목표 위치 (그릇 안)
};

bool isSpacePressed = false; // 스페이스바 상태를 추적하는 변수
int mixCount = 0; // 비빔 횟수 변수
const int MAX_MIX_COUNT = 100; // 비빔의 최대 횟수
RectangleShape mixGauge, mixGaugeBackground; // 비빔 게이지와 배경 사각형

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // 난수 생성기 초기화 (현재 시간을 시드로 사용)

    cout << "프로그램이 시작되었습니다." << endl; // 프로그램 시작 메시지 출력

    RenderWindow window(VideoMode(1440, 1024), "bibim period"); // 게임 윈도우 생성 (1440x1024 크기)
    Music backgroundMusic; // 배경 음악 객체

    // 게임에 필요한 다양한 텍스처들을 위한 Texture 객체 선언
    Texture startBackgroundTexture, gameBackgroundTexture, mixBackgroundTexture, passBackgroundTexture, overBackgrountTexture;
    Texture startButtonTexture, retryButtonTexture, bowlTexture;
    Texture beanSproutTexture, brackenTexture, carrotTexture, cucumberTexture, friedEggTexture, meatTexture, mushroomTexture, riceTexture, spinachTexture;
    Texture bibimbabTexture, gochujangTexture, sauceTexture;

    // 텍스처 로딩 함수 (에러 처리 포함)
    auto loadTexture = [&](const string& path, Texture& texture) {
        if (!texture.loadFromFile(path)) { // 파일 로드 실패 시 에러 메시지 출력
            cerr << "Error loading texture: " << path << endl;
            return false; // 실패 반환
        }
        return true; // 성공 반환
        };

    // 모든 필요한 텍스처 로드하고 에러 확인. 하나라도 실패하면 -1 반환.
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
        return -1; // 텍스처 로딩 실패 시 프로그램 종료
    }

    // 배경 음악 로드 및 에러 처리
    if (!backgroundMusic.openFromFile("bibim.ogg")) {
        cerr << "Error loading music file!" << endl;
        return -1; // 음악 로드 실패 시 프로그램 종료
    }

    Font font; // 폰트 객체 생성
    if (!font.loadFromFile("font/Pretendard-Regular.otf")) { // 폰트 로드 및 에러 처리
        cerr << "Error loading font file!" << endl;
        return -1; // 폰트 로드 실패시 프로그램 종료
    }

    // 텍스트 객체 생성 및 초기화
    Text gamemessage, bibimmessage, mixmessage, timerText;
    initializeText(gamemessage, font, L"화면에 있는 Bibim 재료를 클릭하여 모두 넣어주세요.", 40, { 720, 974 });
    initializeText(bibimmessage, font, L"고추장을 더블클릭해 넣어주세요.", 40, { 720, 974 });
    initializeText(mixmessage, font, L"[space]로 게이지가 모두 찰 때까지 비벼주세요.", 40, { 720, 974 });
    timerText.setFont(font);
    timerText.setCharacterSize(50);
    timerText.setFillColor(Color::White);
    timerText.setPosition(20, 20);

    // 스프라이트 객체 생성 및 위치 설정
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

    // 재료 객체 생성 및 초기화 (Ingredient 클래스 사용)
    vector<unique_ptr<Ingredient>> ingredients;
    map<string, tuple<Texture, Vector2f, Vector2f>> ingredientData = {
        {"egg", {friedEggTexture, {904, 644}, {521, 394}}},
        {"cucumber", {cucumberTexture, {1005, 472}, {642, 537}}},
        {"bean", {beanSproutTexture, {1025, 237}, {723, 452}}},
        {"mushroom", {mushroomTexture, {831, 99}, {691, 351}}},
        {"bracken", {brackenTexture, {571, 40}, {513, 305}}},
        {"meat", {meatTexture, {298, 78}, {396, 367}}},
        {"carrot", {carrotTexture, {81, 212}, {360, 461}}},
        {"spinach", {spinachTexture, {72, 452}, {438, 524}}},
        {"rice", {riceTexture, {166, 665}, {545, 467}}}
    };

    for (auto& [name, data] : ingredientData) { // 각 재료 데이터에 대해 반복
        auto& [texture, startPos, targetPos] = data; // 텍스처, 시작 위치, 목표 위치 추출
        ingredients.emplace_back(make_unique<Ingredient>(texture, rand() % 5 + 1, startPos, targetPos)); // Ingredient 객체 생성 및 벡터에 추가 (개수는 1~5개의 랜덤값)
    }

    // 비빔 게이지 설정
    mixGaugeBackground.setSize({ 800, 30 }); // 크기 설정
    mixGaugeBackground.setPosition({ 320, 700 }); // 위치 설정
    mixGaugeBackground.setFillColor(Color(100, 100, 100)); // 색상 설정
    mixGauge.setSize({ 0, 30 }); // 크기 설정 (초기 크기 0)
    mixGauge.setPosition({ 320, 700 }); // 위치 설정
    mixGauge.setFillColor(Color::Red); // 색상 설정

    bool isSauceMoving = false; // 고추장 소스 이동 여부 플래그 
    Vector2f sauceStartPosition; // 고추장 소스 초기 위치 
    Vector2f sauceTargetPosition = bibimbabSprite.getPosition(); // 고추장 소스 목표 위치 (비빔밥 위치) 
    Clock mixTransitionClock; // MixScreen으로 전환을 위한 추가 클럭
    Scene currentScene = Scene::StartScreen; // 현재 게임 장면 
    bool isTimerRunning = false; // 타이머 동작 여부 플래그 
    Clock clock; // 게임 시간 측정용 클럭 
    Clock transitionClock; // 화면 전환 시간 측정용 클럭 

    while (window.isOpen()) { // 윈도우가 열려있는 동안 반복
        Event event; // 이벤트 객체
        while (window.pollEvent(event)) { // 윈도우 이벤트 큐에서 이벤트를 가져옴
            if (event.type == Event::Closed) { // 윈도우 닫기 이벤트 처리
                window.close(); // 윈도우 닫기
                cout << "프로그램이 종료되었습니다." << endl; // 종료 메시지 출력
            }

            // 시작 화면 이벤트 처리: 시작 버튼 클릭
            if (currentScene == Scene::StartScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window); // 마우스 위치 가져오기
                if (startButtonSprite.getGlobalBounds().contains(Vector2f(mousePos))) { // 마우스가 시작 버튼 위에 있는지 확인
                    cout << "버튼이 클릭되었습니다. 게임 화면으로 전환됩니다." << endl; // 메시지 출력
                    currentScene = Scene::GameScreen; // 게임 화면으로 전환
                    isTimerRunning = true; // 타이머 시작
                    clock.restart(); // 타이머 초기화
                    backgroundMusic.play(); // 배경 음악 재생
                }
            }

            // 게임 화면 이벤트 처리: 재료 클릭
            if (currentScene == Scene::GameScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window); // 마우스 위치 가져오기
                Vector2f mousePosF(mousePos.x, mousePos.y); // 마우스 위치를 Vector2f로 변환

                for (auto& ingredient : ingredients) { // 각 재료에 대해 반복
                    if (!ingredient->isCollected() && ingredient->contains(mousePosF)) { // 재료가 아직 사용되지 않았고, 마우스가 재료 위에 있는 경우
                        ingredient->collect(); // 재료 사용
                        break; // 반복문 종료
                    }
                }
                // 모든 재료가 사용되었는지 확인
                bool allCollected = true;
                for (const auto& ing : ingredients) {
                    if (!ing->isCollected()) { // 사용되지 않은 재료가 있는 경우
                        allCollected = false;
                        break;
                    }
                }
                if (allCollected) currentScene = Scene::BiBimScreen; // 모든 재료 사용 시 비빔밥 화면으로 전환
            }

            // 비빔밥 화면 이벤트 처리: 고추장 클릭 
            if (currentScene == Scene::BiBimScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                // 마우스 클릭 위치 저장 (클릭 순간의 위치만 확인)
                Vector2f mousePosF(Mouse::getPosition(window));

                if (sauceSprite.getGlobalBounds().contains(mousePosF)) { // 마우스가 고추장 소스 위에 있는 경우 
                    sauceSprite.setPosition(348, 384); // 고추장 소스 위치를 비빔밥 위로 이동 
                    isSauceMoving = true; // 고추장 소스 이동 시작 플래그 설정 
                    mixTransitionClock.restart(); // MixScreen 전환용 클럭 초기화
                }
            }

            // 고추장 소스 이동 애니메이션 처리 
            if (isSauceMoving) {
                // 1초가 경과한 경우만 MixScreen으로 전환
                if (mixTransitionClock.getElapsedTime().asSeconds() >= 1.f) {
                    currentScene = Scene::MixScreen; // 믹싱 화면으로 전환 
                    isSauceMoving = false; // 고추장 소스 이동 종료 플래그 해제 
                }
            }


            // 게임 로직: 스페이스바를 눌렀을 때 게이지 증가
            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                if (!isSpacePressed) { // 처음 누를 때만 실행
                    if (mixCount < MAX_MIX_COUNT) {
                        mixCount++;
                        float gaugeWidth = static_cast<float>(mixCount) / MAX_MIX_COUNT * 800.0f;
                        mixGauge.setSize({ gaugeWidth, 30 });
                    }
                    if (mixCount >= MAX_MIX_COUNT) { // 믹스 카운트가 최대값에 도달한 경우
                        isTimerRunning = false; // 타이머 정지
                        currentScene = Scene::PassScreen; // 패스 화면으로 전환
                    }
                    isSpacePressed = true; // 상태를 눌림으로 설정
                }
            }

            // 스페이스바가 떼어졌을 때 상태 초기화
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::Space) {
                isSpacePressed = false;
            }

            if (currentScene == Scene::OverScreen && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window); // 마우스 위치 가져오기 (윈도우 로컬 좌표)
                Vector2f worldPos = window.mapPixelToCoords(mousePos); // 로컬 좌표를 월드 좌표로 변환
                if (retryButtonSprite.getGlobalBounds().contains(worldPos)) { // 마우스가 다시 시도 버튼 위에 있는 경우
                    currentScene = Scene::StartScreen; // 시작 화면으로 전환
                }
            }

        }

        // 타이머 동작 처리
        if (isTimerRunning) {
            Time elapsed = clock.getElapsedTime(); // 경과 시간 가져오기
            int remainingTime = 60 - static_cast<int>(elapsed.asSeconds()); // 남은 시간 계산 (60초 제한)

            if (remainingTime <= 0) { // 시간이 다 되었으면
                isTimerRunning = false; // 타이머 정지
                timerText.setString("00:00"); // 타이머 텍스트 업데이트
                currentScene = Scene::OverScreen; // 게임 오버 화면으로 전환
            }
            else { // 시간이 남았으면
                int minutes = remainingTime / 60; // 분 계산
                int seconds = remainingTime % 60; // 초 계산
                stringstream ss; // 문자열 스트림 객체 생성
                ss << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds; // 남은 시간을 "분:초" 형식으로 포맷팅
                timerText.setString(ss.str()); // 타이머 텍스트 업데이트
            }
        }


        window.clear(); // 화면 지우기
        // 현재 장면에 따라 다른 요소들을 그립니다.
        switch (currentScene) {
        case Scene::StartScreen: // 시작 화면
            window.draw(startBackgroundSprite); // 시작 배경 스프라이트 그리기
            window.draw(startButtonSprite); // 시작 버튼 스프라이트 그리기
            break;

        case Scene::GameScreen: // 게임 화면
            window.draw(gameBackgroundSprite); // 게임 배경 스프라이트 그리기
            window.draw(timerText); // 타이머 텍스트 그리기
            window.draw(bowlSprite); // 그릇 스프라이트 그리기
            for (const auto& ing : ingredients) ing->draw(window); // 각 재료 스프라이트 그리기
            window.draw(gamemessage); // 게임 메시지 텍스트 그리기
            break;

        case Scene::BiBimScreen: // 비빔밥 화면
            window.draw(gameBackgroundSprite); // 게임 배경 스프라이트 그리기
            window.draw(timerText); // 타이머 텍스트 그리기
            window.draw(gochujangSprite); // 고추장 스프라이트 그리기
            window.draw(bibimbabSprite); // 비빔밥 스프라이트 그리기
            window.draw(sauceSprite); // 고추장 소스 스프라이트 그리기
            window.draw(bibimmessage); // 비빔 메시지 텍스트 그리기
            break;

        case Scene::MixScreen: // 믹싱 화면
            window.draw(mixBackgroundSprite); // 믹싱 배경 스프라이트 그리기
            window.draw(timerText); // 타이머 텍스트 그리기
            window.draw(mixmessage); // 믹싱 메시지 텍스트 그리기
            window.draw(mixGaugeBackground); // 비빔 게이지 배경 사각형 그리기
            window.draw(mixGauge); // 비빔 게이지 사각형 그리기
            break;

        case Scene::PassScreen: // 패스 화면
            window.draw(passBackgroundSprite); // 패스 화면 배경 스프라이트 그리기
            window.draw(timerText); // 타이머 텍스트 그리기
            timerText.setPosition(600, 20); // 타이머 텍스트 위치 설정
            break;

        case Scene::OverScreen: // 게임 오버 화면
            window.draw(overBackgrountSprite); // 게임 오버 화면 배경 스프라이트 그리기
            window.draw(retryButtonSprite); // 다시 시도 버튼 스프라이트 그리기
            break;
        }

        window.display(); // 화면에 그려진 내용을 표시
    }

    return 0; // 프로그램 정상 종료
}